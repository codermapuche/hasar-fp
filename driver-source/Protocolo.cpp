#include <stdio.h>
#include <string>
#include "Protocolo.h"
#include "ssplit.h"
#include "debug.h"
#include "Chars.h"

namespace
{
	enum
	{
		NUL, SOH, STX, ETX, EOT, ENQ, ACK, BEL, BS, HT, LF, VT, FF, CR, SO, SI, DLE,
		DC1, DC2, DC3, DC4, NAK, SYN, ETB, CAN, EM, SUB, ESC, FS, GS, RS, US
	};

	const char DOUBLE_WIDTH = '\xf4';

	const char COMM_ANOMALY_PACKET_NUM = '\x7f';

#ifdef P_LINUX
	inline void Sleep(unsigned Millisecs)
	{
		usleep(Millisecs * 1000);
	}
#endif
}

unsigned ProtocoloFiscal::DEFAULT_RETRIES = 3;
unsigned ProtocoloFiscal::DEFAULT_TIMEOUT = 2000; // ms

unsigned ProtocoloFiscal::MIN_RETRIES = 1;
unsigned ProtocoloFiscal::MIN_TIMEOUT = 400; // ms

ProtocoloFiscal::ProtocoloFiscal(LogFile &LogF) : Log(LogF)
{
	AborcionEnProgreso = false;
	PacketNumber = 0x20;
	pEventos = NULL;
	pTransporte = NULL;
	ModoAnsi = false;
	RetryTimeouts = false;
	ConfiguredRetries = DEFAULT_RETRIES;
	ConfiguredTimeout = DEFAULT_TIMEOUT;
	Respuesta = "";
	DC2Count = 0;
	HaveAnomalyComm = false;
}

void
ProtocoloFiscal::ConfigurarParametros(unsigned Reintentos, unsigned Timeout)
{
	ConfiguredRetries = (Reintentos >= MIN_RETRIES) ? Reintentos : MIN_RETRIES;
	ConfiguredTimeout = (Timeout >= MIN_TIMEOUT) ? Timeout : MIN_TIMEOUT; // ms
}

void
ProtocoloFiscal::Ansi(bool Modo)
{
	ModoAnsi = Modo;
}

bool
ProtocoloFiscal::Ansi() const
{
	return ModoAnsi;
}

// EstablecerTransporte
void
ProtocoloFiscal::EstablecerTransporte(Transporte *pTransport) throw(Excepcion)
{
	EliminarTransporte();

	Log.Write("EstablecerTransporte", "Nuevo transporte");

	pTransporte = pTransport;
	pTransporte->EstablecerManejadorBusyWaiting(this);
};

// EliminarTransporte
void
ProtocoloFiscal::EliminarTransporte()
{
	if (pTransporte)
	{
		Log.Write("EliminarTransporte", "Se elimina transporte");
		delete pTransporte;
		pTransporte = NULL;
	}
}

// Destructor de ProitocoloFiscal
ProtocoloFiscal::~ProtocoloFiscal()
{
	EliminarTransporte();
}


void
ProtocoloFiscal::ReintentarErroresComunicaciones(bool Retry)
{
	RetryTimeouts = Retry;
}

bool
ProtocoloFiscal::ReintentarErroresComunicaciones() const
{
	return RetryTimeouts;
}

bool
ProtocoloFiscal::ChecksumOK(const std::string &RespuestaRecibida, const std::string &ChecksumRecibido)
{
	unsigned short Recibido = 0;
	for (unsigned i = 0; i < 4 ; i++)
	{
		Recibido <<= 4;
		char c = ChecksumRecibido[i];
		Recibido |= (isdigit(c) ? c - '0' : toupper(c) - 'A' + 0x0A);
	}

	unsigned short Calculado = STX + ETX;
	for (std::string::const_iterator is = RespuestaRecibida.begin(); is != RespuestaRecibida.end(); is++)
		Calculado += static_cast<unsigned char>(*is);

	return Calculado == Recibido;
}

void
ProtocoloFiscal::RespuestaLista(const std::string &RespuestaOK) throw(Excepcion)
{
	Log.Write("RespuestaLista", "%s", RespuestaOK.c_str());

	Respuesta = RespuestaOK;
	if (ModoAnsi)
	{

		char *BufferANSI = new char[Respuesta.length()+1];

		ConvertFromASCII(Respuesta.c_str(), BufferANSI);
		std::string::size_type n;
		if ((n = Respuesta.find(DOUBLE_WIDTH)) != std::string::npos)
			BufferANSI[n] = DOUBLE_WIDTH;
		
		Respuesta = BufferANSI;
		delete[] BufferANSI;

	}
	unsigned n = OffsetPrimerCampo();
	if (Respuesta.length() <= n)
		throw Excepcion(Excepcion::PROTOCOLO_MAL_FORMATO);
	VectorRespuesta = csplit(Respuesta.substr(n), std::string(1, FS), false);
}

void
ProtocoloFiscal::RaiseEvent(Eventos e)
{
	// Si se trata del evento IMPRESOR_OCUPADO actualizamos
	// el contador de DC2 recibidos.
	if ( e == IMPRESOR_OCUPADO )
		DC2Count++;

	if (!pEventos)
		return;
	pEventos->Evento(e);
}

void
ProtocoloFiscal::CheckAbortion() throw (Excepcion)
{
	if (!AborcionEnProgreso)
		return;
	Log.Write("CheckAbortion", "Aborción en progreso");
	throw Excepcion(Excepcion::PROTOCOLO_ABORCION);
}

void
ProtocoloFiscal::EsperarRespuesta(bool WaitSTX) throw (Excepcion)
{
	std::string r, Check;
	enum States { WAIT_STX, WAIT_BODY, WAIT_CHECKSUM };
	States State = WaitSTX? WAIT_STX : WAIT_BODY;

	const unsigned MAX_RESPUESTA = 512;
	unsigned Retries = ConfiguredRetries;
	while(Retries)
	{
		char c;
		try
		{
			c = pTransporte->LeerByte(ConfiguredTimeout);
			CheckAbortion();
		}
		catch(Excepcion &e)
		{
			CheckAbortion();

			if (e.Codigo == Excepcion::TRANSPORTE_ERROR_TIEMPO_DE_ESPERA_AGOTADO)
			{
				Retries--;
				Log.Write("EsperarRespuesta", "Timeout (%ums), reintento %u/%u",
							ConfiguredTimeout,
							ConfiguredRetries - Retries, ConfiguredRetries);
				// Verificamos si debemos enviar a la fuerza un comando StatPRN
				// para destrabar al CF de la anomalía de comunicaciones ...
				ProtocoloStatPRN *p = dynamic_cast<ProtocoloStatPRN *>(this);
				if (p && HaveAnomalyComm)
					p->ArreglarAnomalia();
				continue;
			}
			Log.Write("EsperarRespuesta", e);
			throw;
		}

		switch(c)
		{
			case DC2:
				Log.Write("EsperarRespuesta", "DC2 (#%u)", DC2Count+1);
				RaiseEvent(IMPRESOR_OCUPADO);
				continue;
			case DC4:
				Log.Write("EsperarRespuesta", "DC4");
				RaiseEvent(FALTA_PAPEL);
				continue;
			case STX:
				State = WAIT_BODY;
				r = "";
				continue;
			case ETX:
				if (State == WAIT_BODY)
				{
					State = WAIT_CHECKSUM;
					Check = "";
					continue;
				}
				else
					State = WAIT_STX;
				break;
			case ACK:
				Log.Write("EsperarRespuesta", "ACK descartado");
				continue;
			case NAK:
				Log.Write("EsperarRespuesta", "NAK descartado");
				continue;
		}
		switch(State)
		{
			case WAIT_STX:
				break;
			case WAIT_BODY:
				r += c;
				if (r.length() > MAX_RESPUESTA)
				{
					r = "";
					Retries--;
					Log.Write("EsperarRespuesta", "Buffer overflow, reintento %u/%u",
							ConfiguredRetries - Retries, ConfiguredRetries);
				}
				break;
			case WAIT_CHECKSUM:
				Check += c;
				if (Check.length() == 4)
				{
					if (ChecksumOK(r, Check))
					{
						pTransporte->EnviarByte(ACK);

						if (ObtenerNumeroPaqueteDeRespuesta(r) == COMM_ANOMALY_PACKET_NUM)
						{
							Log.Write("EsperarRespuesta", "Recibimos paquete con nro especial por anomalía comunicaciones");
							throw Excepcion(Excepcion::PROTOCOLO_ANOMALIA_COMUNICACIONES);
						}

						// Guardamos el Código de Comando de la
						// respuesta en 'CommandNumberInAnswer'
						ObtenerNumeroComandoDeRespuesta(r);

						RespuestaLista(r);
						return;
					}
					pTransporte->EnviarByte(NAK);
					Retries--;
					Log.Write("EsperarRespuesta", "Checksum inválido, reintento %u/%u",
							ConfiguredRetries - Retries, ConfiguredRetries);
					State = WAIT_STX;
				}
				break;
		}
	}
	Log.Write("EsperarRespuesta", "Máxima cantidad de reintentos agotada");
	throw Excepcion(Excepcion::PROTOCOLO_MAXIMOS_REINTENTOS);
}

void
ProtocoloFiscal::EnviarPaquete(const std::string &Paquete) throw(Excepcion)
{
	if (!pTransporte)
		throw Excepcion(Excepcion::PROTOCOLO_NO_HAY_TRANSPORTE_ESPECIFICADO);

	AborcionEnProgreso = false;

	std::string PaqueteProtocolado = ArmarPaqueteFiscal(Paquete);

	while (!pTransporte->InQueueEmpty())
	{
		Log.Write("EnviarPaquete", "Hay una respuesta perdida");
		try
		{
			EsperarRespuesta(true);
		}
		catch(Excepcion)
		{
			Log.Write("EnviarPaquete", "Descartamos excepción de EsperarRespuesta por descarte de respuestas perdidas");
			break;
		}
	}

	unsigned AckRetries = ConfiguredRetries;
	unsigned CommandMismatchRetries = ConfiguredRetries;
	bool DoSendPacket = true;
	while(AckRetries)
	{
		if (DoSendPacket)
		{
			Log.Write("EnviarPaquete", "%s", PaqueteProtocolado.c_str());
			pTransporte->EnviarString(PaqueteProtocolado);
		}

		unsigned char Byte;
		try
		{
			Byte = pTransporte->LeerByte(ConfiguredTimeout);
			CheckAbortion();
		}
		catch(Excepcion &e)
		{
			CheckAbortion();
			if (e.Codigo != Excepcion::TRANSPORTE_ERROR_TIEMPO_DE_ESPERA_AGOTADO)
			{
				Log.Write("EnviarPaquete", e);
				throw;
			}

			AckRetries--;
			Log.Write("EnviarPaquete", "Timeout (%ums), reintento %u/%u",
							ConfiguredTimeout,
							ConfiguredRetries - AckRetries, ConfiguredRetries);
			DoSendPacket = true;
			continue;
		}

		switch(Byte)
		{
			case DC2:
				Log.Write("EnviarPaquete", "DC2 (#%u)", DC2Count+1);
				RaiseEvent(IMPRESOR_OCUPADO);
				DoSendPacket = false;
				break;
			case DC4:
				Log.Write("EnviarPaquete", "DC4");
				RaiseEvent(FALTA_PAPEL);
				DoSendPacket = false;
				break;
			case ACK:
				Log.Write("EnviarPaquete", "ACK");
				try
				{
					EsperarRespuesta(true);
				}
				catch (Excepcion &e)
				{
					if (e.Codigo == Excepcion::PROTOCOLO_ANOMALIA_COMUNICACIONES)
					{
						Log.Write("EnviarPaquete", e);
						continue;
					}
					throw;
				}

				if (NumeroPaqueteEnRespuesta() != PacketNumber)
				{
					Log.Write("EnviarPaquete", "Packet mismatch");
					AckRetries--;
					DoSendPacket = true;
					break;
				}
				// Se excluye a 7F porque se utiliza como paquete de StatPRN
				// para destrabar anomalía de comunicaciones ...
				if (++PacketNumber >= 0x7e)
					PacketNumber = 0x20;

				if (NumeroComandoEnRespuesta() != CommandNumber)
				{
					CommandMismatchRetries--;
					Log.Write("EnviarPaquete", "Command mismatch, reintento %u/%u", 
							ConfiguredRetries - CommandMismatchRetries, ConfiguredRetries);
					if (!CommandMismatchRetries)
					{
						Log.Write("EnviarPaquete", "Máxima cantidad de reintentos por command mismatch agotada");
						throw Excepcion(Excepcion::PROTOCOLO_COMANDO_NO_COINCIDENTE);
					}

					PaqueteProtocolado = ArmarPaqueteFiscal(Paquete);
					DoSendPacket = true;
					continue;
				}
				RaiseEvent(ACK_A_COMANDO);
				return;
			case NAK:
				Log.Write("EnviarPaquete", "NAK");
				AckRetries--;
				DoSendPacket = true;
				break;
 			case STX:
				Log.Write("EnviarPaquete", "STX esperando ACK; a levantar respuesta perdida");
				try
				{
					EsperarRespuesta(false);
				}
				catch(Excepcion)
				{
					Log.Write("EnviarPaquete", "Descartamos excepción de EsperarRespuesta por STX recibido cuando esperabamos ACK");
				}
				DoSendPacket = false;
				break;
		}
	}
	Log.Write("EnviarPaquete", "Máxima cantidad de reintentos agotada");
	throw Excepcion(Excepcion::PROTOCOLO_MAXIMOS_REINTENTOS);
}

void
ProtocoloFiscal::EnviarPaqConReintento(const std::string &Paquete) throw(Excepcion)
{
	while(1)
		try
		{
			EnviarPaquete(Paquete);
			SendMutex.Leave();
			return;
		}
		catch(Excepcion &e)
		{
			Log.Write("EnviarPaqConReintento", e);
			if (e.Codigo != Excepcion::PROTOCOLO_MAXIMOS_REINTENTOS ||
				!RetryTimeouts)
			{
				SendMutex.Leave();
				throw;
			}

			RaiseEvent(RETRYING_TIMEOUT);
			CheckAbortion();
		}
}

void
ProtocoloFiscal::EnviarPaqueteFiscal(const std::string &Paquete) throw(Excepcion)
{
	std::string s(Paquete);
	if (!SendMutex.TryEnter())
	{
		Log.Write("EnviarPaqueteFiscal", "Error de reentrancia, comando: %s", Paquete.c_str());
		throw Excepcion(Excepcion::PROTOCOLO_ERROR_ATOMICIDAD);
	}

	if (ModoAnsi)
	{
		char *BufferASCII = new char[s.length()+1];

		ConvertToASCII (s.c_str(), BufferASCII);
		// Repongo el código de comando
		BufferASCII[0] = s[0];

		std::string::size_type n;
		if ((n = s.find(DOUBLE_WIDTH)) != std::string::npos)
			BufferASCII[n] = DOUBLE_WIDTH;

		s = BufferASCII;
		delete[] BufferASCII;
	}

	// Reemplazamos los caracteres fuera de rango por '.'
	// a partir del primer caracter posterior al OpCode.
	for (std::string::iterator i = (s.begin() + 1); i != s.end(); i++)
	{
		unsigned char c = *i;
		if ((c < 0x20 && c != 0x1c) || (c > 0xaf && c < 0xf0))
			*i = '.';
	}

	// Reseteamos el contador de DC2 recibidos.
	DC2Count = 0;


	// Enviamos el paquete observando si debemos reintentar o no su transmisión.
	EnviarPaqConReintento(s);
}

std::vector<std::string>
ProtocoloFiscal::ObtenerCamposRespuesta() const
{
	return VectorRespuesta;
}

// Abortar
void
ProtocoloFiscal::Abortar()
{
	Log.Write("Abortar", "Aviso de aborción");
	AborcionEnProgreso = true;
};

// Contador de Impresora Ocupada
unsigned
ProtocoloFiscal::ContadorImpresoraOcupada () const
{
	return DC2Count;
}

// EstablecerManejadorDeEventos
void
ProtocoloFiscal::EstablecerManejadorDeEventos(ManejadorDeEventos *p)
{
	pEventos = p;
};

std::string
ProtocoloFiscal::Checksum(const std::string &s) const
{
	unsigned short chk = 0;
	for (std::string::const_iterator i = s.begin(); i != s.end(); i++)
		chk += static_cast<unsigned char>(*i);

	char Buf[5];
#ifdef P_WIN32_VC_2005
	sprintf_s(Buf, sizeof Buf, "%04x", chk);
#else
	sprintf(Buf, "%04x", chk);
#endif
	return std::string(Buf);
}

void
ProtocoloFiscal::Evento(unsigned, unsigned)
{
	RaiseEvent(IDLE_TIME);
}


unsigned char 
ProtocoloFiscal::ObtenerNumeroPaqueteDeRespuesta(const std::string &r)
{
	return r[0];
}

ProtocoloOriginal::ProtocoloOriginal(LogFile &LogF) : ProtocoloFiscal(LogF)
{
}

std::string
ProtocoloOriginal::ArmarPaqueteFiscal(const std::string &s)
{
	std::string Paquete;

	// Guardamos el Código de Comando
	CommandNumber = s [0];

	Paquete += char(STX);
	Paquete += PacketNumber;
	Paquete += s;
	Paquete += char(ETX);
	Paquete += Checksum(Paquete);
	return Paquete;
}

unsigned char
ProtocoloOriginal::NumeroPaqueteEnRespuesta() const
{
	return Respuesta[0];
}

char
ProtocoloOriginal::NumeroComandoEnRespuesta() const
{
	return CommandNumberInAnswer;
}

void
ProtocoloOriginal::ObtenerNumeroComandoDeRespuesta(const std::string &r)
{
	CommandNumberInAnswer = r[1];
}

char
ProtocoloOriginal::NumeroComandoUltimaRespuesta() const
{
	if (Respuesta.length() == 0)
		return 0;
	return Respuesta.c_str() [1];
}

unsigned
ProtocoloOriginal::OffsetPrimerCampo() const
{
	return 3;
}


ProtocoloStatPRN::ProtocoloStatPRN(LogFile &LogF) : ProtocoloFiscal(LogF), CMD_STATPRN('\xa1')
{
	Reintentar = true;
	StatPRNCount = 0;
	WasStatPRN = false;
}

void
ProtocoloStatPRN::ReintentarErroresImpresora(bool Retry)
{
	Reintentar = Retry;
}

bool
ProtocoloStatPRN::ReintentarErroresImpresora() const
{
	return Reintentar;
}

bool
ProtocoloStatPRN::HuboStatPRN()
{
	return WasStatPRN;
}

void
ProtocoloStatPRN::AnomaliaComunicaciones(bool HaveAnomaly)
{
	HaveAnomalyComm = HaveAnomaly;
}

void
ProtocoloStatPRN::ArreglarAnomalia() throw(Excepcion)
{
	std::string Paquete;
	Paquete += char(STX);
	Paquete += char(COMM_ANOMALY_PACKET_NUM);
	Paquete += char(ESC);
	Paquete += std::string(1, CMD_STATPRN);
	Paquete += char(ETX);
	Paquete += Checksum(Paquete);

	Log.Write("ArreglarAnomalia", "Enviando StatPRN <%s>", Paquete.c_str());

	pTransporte->EnviarString(Paquete);
}

std::string
ProtocoloStatPRN::ArmarPaqueteFiscal(const std::string &s)
{
	std::string Paquete;

	// Guardamos el Código de Comando
	if (s [0] != CMD_STATPRN)
		CommandNumber = s [0];

	Paquete += char(STX);
	Paquete += PacketNumber;
	Paquete += char(ESC);
	Paquete += s;
	Paquete += char(ETX);
	Paquete += Checksum(Paquete);
	return Paquete;
}

unsigned char
ProtocoloStatPRN::NumeroPaqueteEnRespuesta() const
{
	return Respuesta[0];
}

char
ProtocoloStatPRN::NumeroComandoEnRespuesta() const
{
	// Si es STATPRN se envía el último comando para arriba,
	// para que el protocolo no piense que es otro comando.
	if (CommandNumberInAnswer == CMD_STATPRN)
		return CommandNumber;
	return CommandNumberInAnswer;
}

void
ProtocoloStatPRN::ObtenerNumeroComandoDeRespuesta(const std::string &r)
{
	CommandNumberInAnswer = r[2];
}

char
ProtocoloStatPRN::NumeroComandoUltimaRespuesta() const
{
	if (Respuesta.length() == 0)
		return 0;
	return Respuesta.c_str() [2];
}

unsigned
ProtocoloStatPRN::OffsetPrimerCampo() const
{
	return 4;
}

unsigned
ProtocoloStatPRN::ContadorStatPRN() const
{
	return StatPRNCount;
}

void
ProtocoloStatPRN::EnviarStatPRN() throw(Excepcion)
{
	// Enviamos el paquete observando si debemos reintentar o no su transmisión.
	EnviarPaqConReintento(std::string(1, CMD_STATPRN));
	WasStatPRN = CommandNumberInAnswer == CMD_STATPRN;
}

void
ProtocoloStatPRN::RespuestaLista(const std::string &r) throw(Excepcion)
{
	// El formato de la respuesta debe ser
	// STX ESC OpCode FS PrinterStatus FS FiscalStatus ... ETX Checksum
	// Si el segundo byte no es ESC, no sigue el protocolo.
	// Si el otro lado está trabajando con protocolo viejo,
	// confundirá el ESC con un OpCode y lo devolverá en
	// la respuesta como comando inválido:
	//		STX ESC FS PrinterStatus FS FiscalStatus ETX Checksum
	// Para detectar esta anomalía, se observa que el FS
	// no esté corrido a la posición donde se espera el
	// Opcode, siempre que el ESC esté en posición.
	if (r[1] != ESC ||
		CommandNumberInAnswer == FS)
		throw Excepcion(Excepcion::PROTOCOLO_EQUIVOCADO);
	ProtocoloFiscal::RespuestaLista(r);
}

void
ProtocoloStatPRN::EnviarPaqueteFiscal(const std::string &Paquete) throw(Excepcion)
{
	unsigned Retries = ConfiguredRetries;

	while(Retries--)
	{
		ProtocoloFiscal::EnviarPaqueteFiscal(Paquete);

		StatPRNCount = 0;
		WasStatPRN = CommandNumberInAnswer == CMD_STATPRN;
		if (!WasStatPRN || !Reintentar)
			return;

		if (!SendMutex.TryEnter())
		{
			Log.Write("EnviarPaqueteFiscal (StatPRN)", "Error de reentrancia, comando: %s", Paquete.c_str());
			throw Excepcion(Excepcion::PROTOCOLO_ERROR_ATOMICIDAD);
		}

		try
		{
			while(CommandNumberInAnswer == CMD_STATPRN)
			{
				StatPRNCount++;
				Log.Write("EnviarPaqueteFiscal", "Es StatPRN (#%u)", StatPRNCount);
				RaiseEvent(STAT_PRN);
				Sleep (500);		// Demora entre StatPRNs.
				EnviarStatPRN();
			}
			SendMutex.Leave();
			return;
		}
		catch(Excepcion &e)
		{
			if (e.Codigo == Excepcion::PROTOCOLO_COMANDO_NO_COINCIDENTE)
			{
				Log.Write("EnviarPaqueteFiscal", "Command mismatch en ciclo StatPRN. Se re-envía el comando original");
				continue;
			}

			SendMutex.Leave();
			throw;
		}
	}
}

