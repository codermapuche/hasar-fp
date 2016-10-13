#ifdef P_WIN32
#ifndef P_WIN32_GCC
#pragma warning(disable:4786) // disable spurious C4786 warnings
#endif
#endif

#include <stdexcept>
#include "ImpresorFiscal.h"
#include "Chars.h"

const std::string ImpresorFiscal::FS		  (1, '\x1c');
const std::string ImpresorFiscal::ERASE_LINE  (1, '\x7f');
const std::string ImpresorFiscal::DOBLE_ANCHO (1, '\xf4');

const unsigned ImpresorFiscal::First_Fantasy_Line   =  1;
const unsigned ImpresorFiscal::Last_Fantasy_Line    =  2;

const unsigned ImpresorFiscal::First_Header_Line    =  1;
const unsigned ImpresorFiscal::Last_Header_Line     = 10;
const unsigned ImpresorFiscal::First_Trailer_Line   = 11;
const unsigned ImpresorFiscal::Last_Trailer_Line    = 20;

const unsigned ImpresorFiscal::First_EmbarkNum_Line =  1;
const unsigned ImpresorFiscal::Last_EmbarkNum_Line  =  2;



// ############################################################ //
//																//
//			F U N C I O N E S      L O C A L E S				//
//																//
// ############################################################ //

//
// Verifica si se trata de un caracter hexa ascii v�lido
//
bool
ishexachar (const char n)
{
	return (isdigit (n) || (n >= 'A' && n <= 'F') || (n >= 'a' && n <= 'f'));
}

//
// Convierte un caracter hexa ascii a su correspondiente unsigned
//
unsigned
hexatobin (const char n)
{
	unsigned char c = n;

	if ((c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
		c -= 7;

	return (c & 0x0f);
}


//
// Convierte un n�mero double a unsigned redondeando.
//
unsigned
dtou (const double d)
{
	unsigned signo	= (d >= 0) ? 1 : -1;
	double num	= d * signo;
	unsigned entera = (unsigned) num;

	if ( (num - entera) >= 0.5 )
		entera++;

	return (entera * signo);
}


// ############################################################ //
//																//
//			M E T O D O S      L O C A L E S					//
//																//
// ############################################################ //

//
// Constructor
//
ImpresorFiscal::ImpresorFiscal(Protocolos Prot)
{
	if (Prot == PROTOCOLO_STATPRN)
		Protocolo = new ProtocoloStatPRN (Log);
	else
		Protocolo = new ProtocoloOriginal (Log);

	ModoAnsi(true);
	
	Protocolo->EstablecerManejadorDeEventos (this);
	pfEventos = NULL;

	// Inicializaci�n de variables de uso general
	PrinterError		= false;
	DrawerClosed		= false;
	OutOfPaper			= false;
	ModoDisplay			= DISPLAY_SI;
	TicketBCAbierto		= true;
	Estacion			= ESTACION_TICKET;

	DescripcionesLargas	= true;
	ModalidadPrecio		= MODO_PRECIO_TOTAL;
	ModalidadII			= II_VARIABLE_PORCENTUAL;
	ModalidadResumenIVA	= RESUMEN_IVA_DOCUMENTO;

	FirstDocNum			= 0;
	NumberOfPages		= 0;

	MaxFiscalTextLinesInvoice		= 0;
	MaxFiscalTextLinesTicket		= 0;

	// Inicializaci�n de variables de tama�o de campos
	PrintNonFiscalTextTicketSize	= 0;
	PrintNonFiscalTextSlipSize		= 0;
	FantasyNameSize					= 0;
	HeaderTrailerSize				= 0;
	HeaderTrailerTicketSize			= 0;
	HeaderTrailerSlipSize			= 0;
	InfoRemitoCompOriSize			= 20;
	SerialNumberSize				= 10;
	CodeIngBrSize					= 0;
	CodeBarMaxSize					= 32;
	RazonSocialSize					= 0;
	DireccionSize					= 0;
	PrintFiscalTextTicketSize		= 0;
	PrintFiscalTextSlipSize			= 0;
	PrintItemTextTicketSize			= 0;
	PrintItemTextSlipSize			= 0;
	PriceDecimals					= 0;
	PerceptionDescSize				= 20;
	TotalTenderTextSize				= 0;
	DNFHNumSize						= 20;
	RemitoTextSize					= 0;
	RemitoCantDecimals				= 0;
	ResumeTextSize					= 0;
	CotizaTextSize					= 0;
	ReciboTextTicketSize			= 0;
	ReciboTextSlipSize				= 0;
	ReciboLines						= 0;
	PrintDNFHTextSize				= 0;
	VouchCustNameSize				= 0;
	VouchCardNameSize				= 0;
	VouchCardNumSize				= 16;
	VouchComCodeSize				= 15;
	VouchImportSize					= 0;
	VouchCompNumSize				= 0;
	VouchVendNameSize				= 0;
	DisplayTextSize					= 0;
}

//
// Destructor
//
ImpresorFiscal::~ImpresorFiscal()
{
	delete Protocolo;
}

//
// Probar Transporte
//
void
ImpresorFiscal::ProbarTransporte () throw(Excepcion)
{
	// Si viene una respuesta de un comando diferente al
	// enviado, porque tiene el mismo n�mero de paquete,
	// se estar� evaluando una respuesta err�nea.
	// Para esto, se env�a un comando dummy, cuya
	// respuesta no le interesa a nadie.
	try
	{
		Protocolo->EnviarPaqueteFiscal(OpCode(CMD_STATUS_REQUEST));
		EnviarComandoFiscal(OpCode(CMD_STATUS_REQUEST));
	}
	catch(Excepcion &e)
	{
		if (e.Codigo == Excepcion::PROTOCOLO_MAXIMOS_REINTENTOS)
			throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_CONTROLADOR_NO_DETECTADO);
		throw;
	}
}


//
// Establecer transporte Puerto Serie
// (con autodetecci�n de velocidad)
//
unsigned
ImpresorFiscal::EstablecerPuertoSerie(unsigned nPuerto) throw(Excepcion)
{
	unsigned Velocidades[] = { 9600, 19200, 38400, 57600, 115200, 4800, 2400, 1200};
	const unsigned MAX_VELOCIDADES = sizeof(Velocidades) / sizeof(unsigned);

	unsigned i;
	for (i = 0; i < MAX_VELOCIDADES; i++)
	{
		RaiseEvent(EVENTO_DE_DETECCION_EN_PROGRESO, Velocidades[i]);

		try
		{
			Protocolo->EstablecerTransporte(new PuertoSerie(Log, nPuerto, Velocidades[i]));
			ProbarTransporte();
			break;
		}
		catch(Excepcion &e)
		{
			if (e.Codigo != Excepcion::IMPRESOR_FISCAL_ERROR_CONTROLADOR_NO_DETECTADO)
				throw;
		}
	}
	if (i == MAX_VELOCIDADES)
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_CONTROLADOR_NO_DETECTADO);
	return Velocidades[i];
}


//
// Establecer transporte Puerto Serie
// (a velocidad establecida)
//
void
ImpresorFiscal::EstablecerPuertoSerie  (unsigned nPuerto,
									    unsigned Velocidad)
										throw(Excepcion)
{
	Protocolo->EstablecerTransporte(new PuertoSerie (Log, nPuerto, Velocidad));
	ProbarTransporte();
}


//
// Establecer transporte Socket TCP
//
void
ImpresorFiscal::EstablecerSocketTCP    (const std::string &DireccionRemota,
										unsigned short PuertoRemoto)
										throw(Excepcion)
{
	Protocolo->EstablecerTransporte(new SocketTCP (Log, DireccionRemota, PuertoRemoto));
	ProbarTransporte();
}


//
// Establecer transporte Socket UDP
//
void
ImpresorFiscal::EstablecerSocketUDP    (const std::string &DireccionRemota,
										unsigned short PuertoRemoto)
										throw(Excepcion)
{
	Protocolo->EstablecerTransporte(new SocketUDP (Log, DireccionRemota, PuertoRemoto));
	ProbarTransporte();
}


//
// Configurar par�metros del protocolo
//
void
ImpresorFiscal::ConfigurarParametrosProtocolo(unsigned Reintentos, unsigned TiempoDeEspera)
{
	Protocolo->ConfigurarParametros(Reintentos, TiempoDeEspera);
}


//
// Cambiar Velocidad del Controlador Fiscal
//
void
ImpresorFiscal::CambiarVelocidad(unsigned /* Velocidad */) throw (Excepcion)
{
	// printf ("Comando CambiarVelocidad ejecutado en ImpresorFiscal\n");
	
	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Cambiar Velocidad del Controlador Fiscal");
}


//
// Finalizar Comunicaciones
//
void
ImpresorFiscal::FinalizarComunicaciones()
{
	Protocolo->EliminarTransporte();
}


//
// Establecer Manejador de Eventos
//
void
ImpresorFiscal::EstablecerManejadorDeEventos(ManejadorDeEventos *p)
{
	pfEventos = p;
}


//
// Capturador de Eventos del Protocolo
//
void
ImpresorFiscal::Evento (unsigned Tipo, unsigned)
{
	const char *p;
	unsigned short DummyErrorFiscal;

	switch (Tipo)
	{
		case ProtocoloFiscal::FALTA_PAPEL:
			OutOfPaper = true;
			RaiseEvent (EVENTO_DE_FALTA_PAPEL, 0);
			p = "DC4";
			break;

		case ProtocoloFiscal::IMPRESOR_OCUPADO:
			RaiseEvent (EVENTO_DE_IMPRESOR_OCUPADO, 0);
			p = "DC2";
			break;

		case ProtocoloFiscal::STAT_PRN:
			EvaluarRespuesta (DummyErrorFiscal);
			p = "STAT_PRN";
			break;

		case ProtocoloFiscal::IDLE_TIME:
			RaiseEvent (EVENTO_DE_TIEMPO_ESPERA_LIBRE, 0);
			p = "IDLE_TIME";
			break;

		case ProtocoloFiscal::RETRYING_TIMEOUT:
			RaiseEvent (EVENTO_DE_COMANDO_REINTENTADO, 0);
			p = "RETRYING_TIMEOUT";
			break;

		case ProtocoloFiscal::ACK_A_COMANDO:
			RaiseEvent (EVENTO_DE_COMANDO_RECIBIDO, 0);
		default:
			return;
	}

	// printf ("%s\n", p);
}


//
// Disparador de Eventos del ImpresorFiscal
//
void
ImpresorFiscal::RaiseEvent(TiposDeEvento Tipo, unsigned SubTipo)
{
	if (!pfEventos)
		return;
	pfEventos->Evento(Tipo, SubTipo);
}


//
// Descripciones de Eventos
//
std::string
ImpresorFiscal::DescripcionDeEvento(TiposDeEvento Tipo, unsigned SubTipo)
{
	struct DESCRIPCION_EVENTO
	{
		TiposDeEvento	Tipo;
		unsigned		SubTipo;
		const char			*Descripcion;
	};

	static const DESCRIPCION_EVENTO DescripcionEvento [] =
	{
		// Eventos asociados al Estado de Impresor
		{	EVENTO_DE_ESTADO_IMPRESOR,		ESTADO_IMPRESOR_ERROR_MECANICO,				"Error mec�nico de impresora"			},
		{	EVENTO_DE_ESTADO_IMPRESOR,		ESTADO_IMPRESOR_FUERA_DE_LINEA,				"Impresora fuera de l�nea"				},
		{	EVENTO_DE_ESTADO_IMPRESOR,		ESTADO_IMPRESOR_TAPA_ABIERTA,				"La tapa de la impresora est� abierta"	},

		{	EVENTO_DE_IMPRESOR_OCUPADO,		0,											"Impresora ocupada"						},

		{	EVENTO_DE_FALTA_PAPEL,			ESTADO_IMPRESOR_FALTA_PAPEL_JOURNAL,		"Falta papel en diario"					},
		{	EVENTO_DE_FALTA_PAPEL,			ESTADO_IMPRESOR_FALTA_PAPEL_RECEIPT,		"Falta papel en ticket"					},
		{	EVENTO_DE_FALTA_PAPEL,			0,											"Falta papel"							},

		{	EVENTO_DE_ESTADO_CAJON,			0,											"El caj�n de dinero est� cerrado"		},
		{	EVENTO_DE_ESTADO_CAJON,			1,											"El caj�n de dinero est� abierto"		},

		// Eventos asociados al Estado Fiscal
		{	EVENTO_DE_MEMORIA_FISCAL_LLENA,	0,											"Memoria fiscal llena"					},

		{	EVENTO_DE_ERROR_COMANDO,		ESTADO_FISCAL_ERROR_MEMORIA_FISCAL,			"Falla de memoria fiscal"				},
		{	EVENTO_DE_ERROR_COMANDO,		ESTADO_FISCAL_ERROR_MEMORIA_TRABAJO,		"Falla de memoria de trabajo"			},
		{	EVENTO_DE_ERROR_COMANDO,		ESTADO_FISCAL_COMANDO_DESCONOCIDO,			"Comando no reconocido"					},
		{	EVENTO_DE_ERROR_COMANDO,		ESTADO_FISCAL_CAMPO_DATOS_INVALIDO,			"Campo de datos inv�lido"				},
		{	EVENTO_DE_ERROR_COMANDO,		ESTADO_FISCAL_COMANDO_INVALIDO,				"Comando inv�lido para el estado actual"},
		{	EVENTO_DE_ERROR_COMANDO,		ESTADO_FISCAL_DESBORDE_ACUMULADOR,			"Rebose de total"						},

		{	EVENTO_DE_ESTADO_FISCAL,		ESTADO_FISCAL_MEMORIA_FISCAL_CASI_LLENA,	"Memoria fiscal a punto de llenarse"	},
		{	EVENTO_DE_ESTADO_FISCAL,		ESTADO_FISCAL_TERMINAL_CERTIFICADA,			"Terminal fiscal certificada"			},
		{	EVENTO_DE_ESTADO_FISCAL,		ESTADO_FISCAL_TERMINAL_FISCALIZADA,			"Terminal fiscal inicializada"			},
		{	EVENTO_DE_ESTADO_FISCAL,		ESTADO_FISCAL_DOCUMENTO_FISCAL_ABIERTO,		"Comprobante fiscal abierto"			},
		{	EVENTO_DE_ESTADO_FISCAL,		ESTADO_FISCAL_DOCUMENTO_ABIERTO,			"Comprobante abierto"					},

		// Eventos asociados a ejecuci�n de comandos
		{	EVENTO_DE_COMANDO_RECIBIDO,		0,											"Comando recibido"						},
		{	EVENTO_DE_COMANDO_PROCESADO,	0,											"Comando procesado"						},
		{	EVENTO_DE_COMANDO_REINTENTADO,	0,											"Comando reintentado"					},
		{	EVENTO_DE_DETECCION_EN_PROGRESO,0,											"Progreso de detecci�n de velocidades"	},

		// Evento de Tiempo de espera libre
		{	EVENTO_DE_TIEMPO_ESPERA_LIBRE,	0,											"Tiempo de espera libre"				}
	};

	for (unsigned i = 0; i < (sizeof DescripcionEvento / sizeof (DESCRIPCION_EVENTO)); i++)
	{
		if (DescripcionEvento[i].Tipo == Tipo &&
			DescripcionEvento[i].SubTipo == SubTipo)
			return DescripcionEvento[i].Descripcion;
	}

	return "Evento desconocido";
}


//
// Contador de Impresora Ocupada
//
unsigned
ImpresorFiscal::ContadorImpresoraOcupada () const
{
	return Protocolo->ContadorImpresoraOcupada ();
}


//
// Contador de StatPRN
//
unsigned
ImpresorFiscal::ContadorStatPRN() const throw(Excepcion)
{
	ProtocoloStatPRN *p = dynamic_cast<ProtocoloStatPRN *>(Protocolo);
	if (!p)
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "StatPRN no implementando en este modelo");
	return p->ContadorStatPRN();
}


//
// Abortar
//
void
ImpresorFiscal::Abortar()
{
	Protocolo->Abortar();
}


//
// EnviarComandoFiscal
// Env�a el comando fiscal a trav�s del protocolo seleccionado y
// luego evalua bits de los status fiscal de de impresor de la
// respuesta para disparar los eventos que sea necesario.
//
void
ImpresorFiscal::EnviarComandoFiscal(const std::string &Comando) throw (Excepcion)
{
	unsigned short ErrorFiscal;

	// Enviamos el comando
	Protocolo->EnviarPaqueteFiscal(Comando);

	// Evaluamos la respuesta y disparamos los eventos
	// asociados a cada uno de los estados
	EvaluarRespuesta (ErrorFiscal);

	// Verificamos si hubo error fiscal en la ejecuci�n del comando
	if (ErrorFiscal)
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_EJECUTADO, DescripcionBitsEstadoFiscal(ErrorFiscal));
}


//
// Modo StatPRN
//
bool
ImpresorFiscal::ModoStatPRN() const
{
	ProtocoloStatPRN *p = dynamic_cast<ProtocoloStatPRN *>(Protocolo);
	
	return p != NULL;
}


//
// Configurar modo Ansi/Ascii
//
void
ImpresorFiscal::ModoAnsi(bool Modo)
{
	Protocolo->Ansi(Modo);
}


//
// Obtener modo Ansi/Ascii
//
bool
ImpresorFiscal::ModoAnsi() const
{	
	return Protocolo->Ansi();
}


//
// Establecer el modo de reintento errores de impresora
//
void
ImpresorFiscal::ReintentarErroresImpresora(bool Reintentar) throw(Excepcion)
{
	ProtocoloStatPRN *p = dynamic_cast<ProtocoloStatPRN *>(Protocolo);
	if (!p)
	{
		if (Reintentar)
			throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "StatPRN no implementando en este modelo");
		else
			return;
	}

	p->ReintentarErroresImpresora(Reintentar);
}


//
// Obtener el modo de reintento errores de impresora
//
bool
ImpresorFiscal::ReintentarErroresImpresora() const throw(Excepcion)
{
	ProtocoloStatPRN *p = dynamic_cast<ProtocoloStatPRN *>(Protocolo);
	if (!p)
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "StatPRN no implementando en este modelo");
	return p->ReintentarErroresImpresora();
}


//
// Establecer el modo de reintento errores de comunicaci�n
//
void
ImpresorFiscal::ReintentarErroresComunicacion(bool Reintentar)
{
	Protocolo->ReintentarErroresComunicaciones(Reintentar);
}


//
// Obtener el modo de reintento errores de comunicaci�n
//
bool
ImpresorFiscal::ReintentarErroresComunicacion() const
{
	return Protocolo->ReintentarErroresComunicaciones();
}


//
// Reintentar el �ltimo error de impresora
// (transmite StatPRN al impresor)
//
void
ImpresorFiscal::ReintentarUltimoErrorImpresora() throw(Excepcion)
{
	unsigned short ErrorFiscal;

	ProtocoloStatPRN *p = dynamic_cast<ProtocoloStatPRN *>(Protocolo);
	if (!p)
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "StatPRN no implementando en este modelo");
	p->EnviarStatPRN();
	EvaluarRespuesta (ErrorFiscal);

	// Verificamos si hubo error fiscal en la ejecuci�n del comando
	if (ErrorFiscal)
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_EJECUTADO, DescripcionBitsEstadoFiscal(ErrorFiscal));
}

//
// EvaluarRespuesta
// Evalua los bits de los estados fiscal y de impresor de la
// respuesta para disparar los eventos que sean necesarios.
//
void
ImpresorFiscal::EvaluarRespuesta(unsigned short &ErrorFiscal)
{
	unsigned Mask;
	unsigned Status;
	unsigned i;

	// Cargamos el VectorRespuesta
	VectorRespuesta = Protocolo->ObtenerCamposRespuesta();

	// Evaluamos la respuesta y disparamos los eventos
	// asociados a cada uno de los status

	// Analizo el Estado de Impresor y disparo eventos
	Status = strtoul(VectorRespuesta[0].c_str(), NULL, 16);
	PrinterError = false;
	OutOfPaper	 = false;

	Mask =	Mascara (ESTADO_IMPRESOR_ERROR_MECANICO) |
			Mascara (ESTADO_IMPRESOR_FUERA_DE_LINEA) |
			Mascara (ESTADO_IMPRESOR_TAPA_ABIERTA);

	// Si alguno de estos flags est� activo, seteamos PrinterError
	if (Status & Mask)
		PrinterError = true;

	for(i = 0; i < 16; i++)
		if ((Mask & Mascara(i)) && (Status & Mascara(i)))
			RaiseEvent(EVENTO_DE_ESTADO_IMPRESOR, i);

	Mask =	Mascara (ESTADO_IMPRESOR_FALTA_PAPEL_JOURNAL) |
			Mascara (ESTADO_IMPRESOR_FALTA_PAPEL_RECEIPT);

	// Si alguno de estos flags est� activo, seteamos OutOfPaper
	if (Status & Mask)
	{
		OutOfPaper = true;
		RaiseEvent(EVENTO_DE_FALTA_PAPEL, 0);
	}

	// Analizo el Estado del Caj�n de Dinero y disparo su evento
	bool CajonCerrado = (Status & Mascara(ESTADO_IMPRESOR_CAJON_CERRADO)) != 0;
	if (CajonCerrado != DrawerClosed)
	{
		DrawerClosed = CajonCerrado;
		RaiseEvent(EVENTO_DE_ESTADO_CAJON, !CajonCerrado); // par�metro del evento -> true = caj�n abierto
	}

	// Analizo el Estado Fiscal y disparo eventos
	Status = strtoul(VectorRespuesta[1].c_str(), NULL, 16);

	Mask =	Mascara (ESTADO_FISCAL_ERROR_MEMORIA_FISCAL) |
			Mascara (ESTADO_FISCAL_ERROR_MEMORIA_TRABAJO) |
			Mascara (ESTADO_FISCAL_COMANDO_DESCONOCIDO) |
			Mascara (ESTADO_FISCAL_CAMPO_DATOS_INVALIDO) |
			Mascara (ESTADO_FISCAL_COMANDO_INVALIDO) |
			Mascara (ESTADO_FISCAL_DESBORDE_ACUMULADOR);

	// Si alguno de estos flags est� activo, lo reflejamos
	// en ErrorFiscal.
	ErrorFiscal = Status & Mask;

	for(i = 0; i < 16; i++)
		if ((Mask & Mascara(i)) && (Status & Mascara(i)))
			RaiseEvent(EVENTO_DE_ERROR_COMANDO, i);

	Mask =	Mascara (ESTADO_FISCAL_MEMORIA_FISCAL_CASI_LLENA) |
			Mascara (ESTADO_FISCAL_TERMINAL_CERTIFICADA) |
			Mascara (ESTADO_FISCAL_TERMINAL_FISCALIZADA) |
			Mascara (ESTADO_FISCAL_DOCUMENTO_FISCAL_ABIERTO) |
			Mascara (ESTADO_FISCAL_DOCUMENTO_ABIERTO);
	for(i = 0; i < 16; i++)
		if ((Mask & Mascara(i)) && (Status & Mascara(i)))
			RaiseEvent(EVENTO_DE_ESTADO_FISCAL, i);

	if (Status & Mascara (ESTADO_FISCAL_MEMORIA_FISCAL_LLENA))
	{
		// Si este flag est� activo, tambi�n lo reflejamos
		// en ErrorFiscal.
		ErrorFiscal |= Mascara (ESTADO_FISCAL_MEMORIA_FISCAL_LLENA);
		RaiseEvent(EVENTO_DE_MEMORIA_FISCAL_LLENA, 0);
	}

	// Luego de procesar la respuesta disparamos el evento
	// de Comando Procesado.
	RaiseEvent(EVENTO_DE_COMANDO_PROCESADO, 0);
}


//
// Convierte un n�mero hexa ascii de 'size' bytes (m�ximo 4)
// a su equivalente unsigned
//
unsigned
ImpresorFiscal::xtoi (const char *num, unsigned size) const
{
	unsigned n,i;
	
	for (i = 0, n = 0 ; *num && i < 4 && i < size ; i++, num++)
	{
		if (!ishexachar(*num))
			break;

		n <<= 4;
		n += hexatobin (*num);
	}

	return n;
}


//
// Numero
//
std::string
ImpresorFiscal::Numero (double f, unsigned Decimales) const
{
	char Buf[50];
	_snprintf(Buf, sizeof Buf, "%.*f", Decimales, f);
	return Buf;
}

std::string
ImpresorFiscal::Numero (unsigned n) const
{
	char Buf[50];
	_snprintf(Buf, sizeof Buf, "%u", n);
	return Buf;
}


//
// Cadena
// Formatea un dato de tipo std::string considerando las
// dimensiones m�xima y m�nima de la cadena y se si debe
// justificar a la izquierda o a la derecha.
//
std::string
ImpresorFiscal::Cadena (const std::string &_s,
						unsigned max,
						unsigned min /*= 1*/,
						bool JustifIzq /*= true*/) const
{
	std::string s(_s);

	if (s.length() > max)
		s = s.substr(0, max);

	if (min && s.length() < min)
	{
		std::string Espacios = std::string(min - s.length(), ' ');
		if (JustifIzq)
			s += Espacios;
		else
			s = Espacios + s;
   }

   return s;
}


//
// OpCode
//
std::string
ImpresorFiscal::OpCode (OpCodes o) const
{
	return std::string(1, o);
}




//
// Campo de Respuesta en formato String
//
std::string
ImpresorFiscal::RespuestaString(unsigned Campo, std::string Contexto) throw(Excepcion)
{
	// Retornamos la informaci�n obtenida de la respuesta
	try
	{
		return VectorRespuesta.at(Campo);
	}
	catch(std::out_of_range)
	{
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_CAMPO_NO_ENCONTRADO, Contexto);	
	}
}


//
// Campo de Respuesta en formato Integer
//
unsigned
ImpresorFiscal::RespuestaInt(unsigned Campo, std::string Contexto) throw(Excepcion)
{
	// Retornamos la informaci�n obtenida de la respuesta
	try
	{
		return atoi (VectorRespuesta.at(Campo).c_str ());
	}
	catch(std::out_of_range)
	{
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_CAMPO_NO_ENCONTRADO, Contexto);
	}
}


//
// Campo de Respuesta en formato Double
//
double
ImpresorFiscal::RespuestaDouble(unsigned Campo, std::string Contexto) throw(Excepcion)
{
	// Retornamos la informaci�n obtenida de la respuesta
	try
	{
		return atof (VectorRespuesta.at(Campo).c_str ());
	}
	catch(std::out_of_range)
	{
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_CAMPO_NO_ENCONTRADO, Contexto);
	}
}


//
// Establecer Modo de Display
//
void
ImpresorFiscal::EstablecerModoDeDisplay (ModosDeDisplay Modo)
{
	ModoDisplay	= Modo;
}


//
// Obtener Modo de Display
//
ImpresorFiscal::ModosDeDisplay
ImpresorFiscal::ObtenerModoDeDisplay () const
{
	return ModoDisplay;
}


//
// Establecer Modalidad de Descripciones Largas
//
void
ImpresorFiscal::EstablecerModalidadDescripcionesLargas (bool Modo)
{
	DescripcionesLargas = Modo;
}


//
// Obtener Modalidad de Descripciones Largas
//
bool
ImpresorFiscal::ObtenerModalidadDescripcionesLargas ()
{
	return DescripcionesLargas;
}


//
// Establecer Modalidad de Precio Base
//
void
ImpresorFiscal::EstablecerModalidadPrecioBase (bool PrecioBase)
{
	ModalidadPrecio = PrecioBase ? MODO_PRECIO_BASE : MODO_PRECIO_TOTAL;
}


//
// Obtener Modalidad de Precio Base
//
bool
ImpresorFiscal::ObtenerModalidadPrecioBase ()
{
	return (ModalidadPrecio == MODO_PRECIO_BASE);
}


//
// Establecer Modalidad de Impuestos Internos
//
void
ImpresorFiscal::EstablecerModalidadDeImpuestosInternos (ModosDeImpuestosInternos Modo)
{
	ModalidadII = Modo;
}


//
// Obtener Modalidad de Impuestos Internos
//
ImpresorFiscal::ModosDeImpuestosInternos
ImpresorFiscal::ObtenerModalidadDeImpuestosInternos ()
{
	return ModalidadII;
}


//
// Establecer Modalidad Resumen IVA
//
// Nota: esta implementaci�n es v�lida para todos los modelos de CF
//	     que no reconocen la opci�n RESUMEN_IVA_DIARIO.
//		 Aquellos modelos que s� la reconocen deben contar con su
//		 implementaci�n propia.
//
void
ImpresorFiscal::EstablecerModalidadResumenIVA (TiposDeResumenIVA Modo)
{
	if (Modo == RESUMEN_IVA_DIARIO)
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Resumen IVA diario no implementando en este modelo");

	ModalidadResumenIVA = Modo;
}


//
// Obtener Modalidad Resumen IVA
//
ImpresorFiscal::TiposDeResumenIVA
ImpresorFiscal::ObtenerModalidadResumenIVA ()
{
	return ModalidadResumenIVA;
}


//
// Hubo Error Mec�nico
//
bool
ImpresorFiscal::HuboErrorMecanico ()
{
	return PrinterError;
}


//
// Hubo Falta de Papel
//
bool
ImpresorFiscal::HuboFaltaPapel ()
{
	return OutOfPaper;
}


//
// Hubo StatPRN
//
bool
ImpresorFiscal::HuboStatPRN() throw(Excepcion)
{
	ProtocoloStatPRN *p = dynamic_cast<ProtocoloStatPRN *>(Protocolo);
	if (!p)
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "StatPRN no implementando en este modelo");
	return p->HuboStatPRN();
}


//
// Solicitar el Estado General del CF
//
void
ImpresorFiscal::SolicitarEstado () throw (Excepcion)
{
	// Verificamos si la �ltima respuesta recibida corresponde
	// a un comando de Solicitud de Estado. En tal caso no
	// repetimos el env�o del comando.
	if ( Protocolo->NumeroComandoUltimaRespuesta() != CMD_STATUS_REQUEST )
	{
		// Enviamos el comando de Solicitud de Estado
		Protocolo->EnviarPaqueteFiscal(OpCode(CMD_STATUS_REQUEST));

		// Cargamos el VectorRespuesta
		VectorRespuesta = Protocolo->ObtenerCamposRespuesta();
	}
}


//
// Estado CF
//
ImpresorFiscal::TiposDeEstadoControlador
ImpresorFiscal::EstadoCF () throw(Excepcion)
{
	// Enviamos el comando fiscal
	SolicitarEstado ();

	// Retornamos la informaci�n obtenida de la respuesta
	return static_cast<TiposDeEstadoControlador>(xtoi(RespuestaString(3, "Estado CF").substr (2, 2).c_str(), 2));
}


//
// Descripci�n Estado CF
//
std::string
ImpresorFiscal::DescripcionEstadoCF () throw(Excepcion)
{
	// printf ("Comando DescripcionEstadoCF ejecutado en ImpresorFiscal\n");

	// Utilizamos un 'map' est�tico en lugar de un array de
	// estructuras solo para agilizar las b�squedas (en map
	// las b�squedas son binarias).

	static std::map<TiposDeEstadoControlador /* State */, std::string /* Description */> DescripcionStatusCF;

	// La primera vez que se invoca al m�todo se carga el mapa.
	if (DescripcionStatusCF.size() == 0)
	{
		DescripcionStatusCF[S_NONFORMATTED_MEMORY]		= "Memoria fiscal no formateada";
		DescripcionStatusCF[S_NONINITIALIZED_MEMORY]	= "Memoria fiscal no inicializada";
		DescripcionStatusCF[S_RECEIPT_NOT_OPENED]		= "No hay ning�n comprobante abierto";
		DescripcionStatusCF[S_FISCAL_RECEIPT_OPENED]	= "Un comprobante fiscal se encuentra abierto";
		DescripcionStatusCF[S_FISCAL_TEXT_ISSUED]		= "Comprobante fiscal abierto. Se acaba de imprimir un texto fiscal";
		DescripcionStatusCF[S_NONFISCAL_RECEIPT_OPENED]	= "Un comprobante no fiscal se encuentra abierto";
		DescripcionStatusCF[S_TENDER]					= "Comprobante fiscal abierto. Se realiz� al menos un pago";
		DescripcionStatusCF[S_TENDER_CLEARED]			= "Comprobante fiscal abierto. Se sald� el monto";
		DescripcionStatusCF[S_PERCEPTION]				= "Comprobante fiscal abierto. Se ha emitido el comando de percepciones";
		DescripcionStatusCF[S_KILLED]					= "El controlador ha sido dado de baja";
		DescripcionStatusCF[S_RETURN_RECHARGE]			= "Comprobante fiscal abierto. Se realiz� un descuento/recargo general";
		DescripcionStatusCF[S_DISCOUNT_CHARGE]			= "Comprobante fiscal abierto. Se realiz� una bonificaci�n/recargo/devoluci�n de envases";
		DescripcionStatusCF[S_RECEIPT_CONCEPT]			= "Recibo fiscal abierto. Se acaba de imprimir una l�nea con el concepto del recibo";
		DescripcionStatusCF[S_CREDIT_NOTE]				= "Una nota de cr�dito se encuenta abierta. Se puede hacer un cr�dito";
		DescripcionStatusCF[S_CREDIT_NOTE_DISCOUNT]		= "Nota de cr�dito abierta. Se realiz� un descuento/recargo general";
		DescripcionStatusCF[S_CREDIT_NOTE_RETURN]		= "Nota de cr�dito abierta. Se realiz� una bonificaci�n/recargo/devoluci�n de envases";
		DescripcionStatusCF[S_CREDIT_NOTE_PERCEPTION]	= "Nota de cr�dito abierta. Se ha emitido el comando de percepciones";
		DescripcionStatusCF[S_CREDIT_NOTE_TEXT]			= "Nota de cr�dito abierta. Se acaba de escribir una l�nea de texto";
		DescripcionStatusCF[S_INTERNAL_USE_RECEIPT]		= "Recibo de uso interno abierto. Se acaba de imprimir una l�nea con el concepto del recibo";
		DescripcionStatusCF[S_QUOTATION]				= "Una cotizaci�n se encuentra abierta";
		DescripcionStatusCF[S_EMBARK]					= "Un remito/orden de salida se encuentra abierto";
		DescripcionStatusCF[S_ACCOUNT]					= "Un resumen de cuenta / cargo a la habitaci�n se encuentra abierto";
		DescripcionStatusCF[S_BLOCKED]					= "El controlador se encuentra bloqueado";
		DescripcionStatusCF[S_DNFH_RECEIPT_OPENED]		= "Un Documento No Fiscal Homologado espec�fico se encuentra abierto";
		DescripcionStatusCF[S_CREDIT_NOTE_TENDER]		= "Nota de cr�dito abierta. Se realiz� al menos un pago";
	}

	// Buscamos el estado del CF en el map.
	std::map<TiposDeEstadoControlador, std::string>::const_iterator i =
			DescripcionStatusCF.find (EstadoCF ());

	if (i != DescripcionStatusCF.end ())
		return i->second;
	
	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_BIT_NO_VALIDO, "DescripcionEstadoCF");
}


//
// Documento CF
//
ImpresorFiscal::TiposDeDocumento
ImpresorFiscal::DocumentoCF () throw(Excepcion)
{
	// Enviamos el comando fiscal
	SolicitarEstado ();

	// Retornamos la informaci�n obtenida de la respuesta
	return static_cast<TiposDeDocumento>(xtoi (RespuestaString(5, "Documento CF").c_str(), 2));
}


//
// Descripci�n Documento CF
//
std::string
ImpresorFiscal::DescripcionDocumentoCF () throw(Excepcion)
{
	// printf ("Comando DescripcionDocumentoCF ejecutado en ImpresorFiscal\n");

	// Utilizamos un 'map' est�tico en lugar de un array de
	// estructuras solo para agilizar las b�squedas (en map
	// las b�squedas son binarias).

	static std::map<TiposDeDocumento /* Doc */, std::string /* Description */> DescripcionDocCF;

	// La primera vez que se invoca al m�todo se carga el mapa.
	if (DescripcionDocCF.size() == 0)
	{
		// Documentos Fiscales
		DescripcionDocCF[D_FACTURA_A]					= "Factura A";
		DescripcionDocCF[D_FACTURA_B]					= "Factura B";
		DescripcionDocCF[D_FACTURA_C]					= "Factura C";
		DescripcionDocCF[D_NOTA_DEBITO_A]				= "Nota de D�bito A";
		DescripcionDocCF[D_NOTA_DEBITO_B]				= "Nota de D�bito B";
		DescripcionDocCF[D_NOTA_DEBITO_C]				= "Nota de D�bito C";
		DescripcionDocCF[D_RECIBO_A]					= "Recibo A";
		DescripcionDocCF[D_RECIBO_B]					= "Recibo B";
		DescripcionDocCF[D_RECIBO_C]					= "Recibo C";
		DescripcionDocCF[D_TICKET_BC]					= "Ticket";

		// Documentos No Fiscales
		DescripcionDocCF[D_DOC_NO_FISCAL]				= "Documento No Fiscal General";
		
		// Documentos No Fiscales Homologados
		DescripcionDocCF[D_NOTA_CREDITO_A]				= "Nota de Cr�dito A";
		DescripcionDocCF[D_NOTA_CREDITO_B]				= "Nota de Cr�dito B";
		DescripcionDocCF[D_NOTA_CREDITO_C]				= "Nota de Cr�dito C";
		DescripcionDocCF[D_REMITO]						= "Remito";
		DescripcionDocCF[D_RECIBO_X]					= "Recibo X";
		DescripcionDocCF[D_ORDEN_SALIDA]				= "Orden de Salida";
		DescripcionDocCF[D_RESUMEN_CUENTA]				= "Resumen de Cuenta";
		DescripcionDocCF[D_CARGO_HABITACION]			= "Cargo a la Habitaci�n";
		DescripcionDocCF[D_COTIZACION]					= "Cotizaci�n";
		DescripcionDocCF[D_VOUCHER_TARJETA]				= "Voucher de Tarjeta";
		DescripcionDocCF[D_CLAUSULA_CREDITO]			= "Cl�usulas de Cr�dito en Cuota Fija";
		DescripcionDocCF[D_CLAUSULA_SEGURO]				= "Cl�usulas de Segura de Desempleo";
		DescripcionDocCF[D_PAGARE]						= "Pagar�";
		DescripcionDocCF[D_POLIZA_SEGURO]				= "P�liza de Seguro de Garant�a Complementaria";
		DescripcionDocCF[D_RECORDATORIO]				= "Recordatorio";
		DescripcionDocCF[D_SOLICITUD_CREDITO]			= "Solicitud de Cr�dito";
		DescripcionDocCF[D_COMUNICACION_CLIENTE]		= "Comunicaci�n con Nuestros Clientes";
		DescripcionDocCF[D_OFRECIMIENTO_CREDITO]		= "Ofrecimiento de Cr�dito en Efectivo";
		DescripcionDocCF[D_OFRECIMIENTO_TARJETA]		= "Ofrecimiento de Tarjeta de Cr�dito";
		DescripcionDocCF[D_MINUTA_CREDITO]				= "Minuta de Cr�dito";
		DescripcionDocCF[D_OFRECIMIENTO_PASAPORTE]		= "Ofrecimiento de Pasaporte";
		DescripcionDocCF[D_RENOVACION_CREDITO]			= "Renovaci�n de Cr�dito";
		DescripcionDocCF[D_ADELANTO_REMUNERACION]		= "Adelanto de Remuneraci�n";
		DescripcionDocCF[D_SOLICITUD_TARJETA_DEBITO]	= "Solicitud de Tarjeta de D�bito";
		DescripcionDocCF[D_SOLICITUD_CLAVE_TARJETA]		= "Solicitud de Clave de Tarjeta";
		DescripcionDocCF[D_RESCATE_MERCADERIA]			= "Rescate de Mercader�a";
		DescripcionDocCF[D_INGRESO_EGRESO_SUCURSAL]		= "Ingresos y Egresos Internos de la Sucursal";
		DescripcionDocCF[D_PRESUPUESTO]					= "Presupuesto";
		DescripcionDocCF[D_ENTREGA]						= "Comprobante de Entrega";
		DescripcionDocCF[D_ESTACIONAMIENTO]				= "Estacionamiento";
		DescripcionDocCF[D_COBRO_SERVICIO]				= "Cobro de Servicio";
		DescripcionDocCF[D_INGRESO_DINERO]				= "Ingreso de Dinero";
		DescripcionDocCF[D_RETIRO_DINERO]				= "Retiro de Dinero";
		DescripcionDocCF[D_TALON_CAMBIO]				= "Tal�n de Cambio";
		DescripcionDocCF[D_TALON_REPARTO]				= "Tal�n de Reparto";
		DescripcionDocCF[D_TALON_REGALO]				= "Tal�n de Regalo";
		DescripcionDocCF[D_TALON_CUENTA_CORRIENTE]		= "Tal�n de Cuenta Corriente";
		DescripcionDocCF[D_AVISO_OPERACION_CREDITO]		= "Aviso de Operaci�n de Cr�dito";
		DescripcionDocCF[D_CUPON_PROMOCION]				= "Cup�n de Promoci�n";
		DescripcionDocCF[D_USO_INTERNO_FARMACIA]		= "Uso Interno Farmacia";
		
		// Opciones extras
		DescripcionDocCF[D_NO_DOCUMENTO_EN_CURSO]		= "No hay documento en curso";
	}

	// Buscamos el tipo de documento del CF en el map.
	std::map<TiposDeDocumento, std::string>::const_iterator i =
			DescripcionDocCF.find (DocumentoCF ());

	if (i != DescripcionDocCF.end ())
		return i->second;
	
	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_BIT_NO_VALIDO, "DescripcionDocumentoCF");
}


//
// Indicador de Documento Previo Cancelado
//
bool
ImpresorFiscal::IndicadorDeDocumentoPrevioCancelado () throw(Excepcion)
{
	// Enviamos el comando fiscal
	SolicitarEstado ();

	// Retornamos la informaci�n obtenida de la respuesta
	return (RespuestaString(5, "Indicador de Documento Previo Cancelado").substr (3, 1) != "0");
}


//
// Obtener Ultima Respuesta
//
std::string
ImpresorFiscal::ObtenerUltimaRespuesta (unsigned Campo) throw (Excepcion)
{
	// printf ("Comando ObtenerUltimaRespuesta ejecutado en ImpresorFiscal\n");

	std::string Rta;

	// Si el par�metro 'Campo' es 0, debemos retornar la �ltima
	// respuesta completa.
	if (Campo == 0)
		for (std::vector<std::string>::const_iterator i =
				VectorRespuesta.begin ();
					i != VectorRespuesta.end (); i++)
		{
			if (i != VectorRespuesta.begin ())
				Rta += FS;
			Rta += *i;
		}
	// En caso contrario, debemos retornar el campo de respuesta
	// cuya posici�n sea precisamente 'Campo'.
	else
		Rta = RespuestaString (Campo-1, "Obtener Ultima Respuesta");

	return Rta;
}


//
// Enviar Comando Crudo
//
void
ImpresorFiscal::EnviarComandoCrudo (const std::string &Comando) throw (Excepcion)
{
	// printf ("Comando EnviarComandoCrudo ejecutado en ImpresorFiscal\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(Comando);
}


//
// Establecer Nombre de Archivo de Log
//
void
ImpresorFiscal::EstablecerNombreArchivoLog (const std::string &Nombre)
{
	// printf ("Comando EstablecerNombreArchivoLog ejecutado en ImpresorFiscal\n");

	Log.SetFileName (Nombre);
}


//
// Obtener Nombre de Archivo de Log
//
std::string
ImpresorFiscal::ObtenerNombreArchivoLog ()
{
	// printf ("Comando ObtenerNombreArchivoLog ejecutado en ImpresorFiscal\n");

	return Log.GetFileName ();
}


//
// � � � � �       N O T A     I M P O R T A N T E       ! ! ! ! !
//
// Este m�dulo contendr� la implementaci�n de todos los m�todos comunes
// a todos los controladores fiscales. En cuanto a los m�todos particulares
// no comunes, cada uno de ellos solo levantar� una excepci�n de tipo
// IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO que ser� el error default para
// los modelos que no soporten dichos m�todos (no tengan su propia
// implementaci�n) ...
//

// ############################################################ //
//																//
//			M E T O D O S   G E N E R A L E S					//
//																//
// ############################################################ //

//
// Enviar el Comando de Pedido de Estado al CF.
//
// Nota: no se retorna la infomaci�n conjunta de la respuesta
//		 porque la misma se puede obtener con otros m�todos
//		 asociados a los diferentes campos y sub-campos.
//
void
ImpresorFiscal::EnviarComandoDePedidoDeEstado () throw (Excepcion)
{
	// printf ("Comando EnviarComandoDePedidoDeEstado ejecutado en ImpresorFiscal\n");
	
	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_STATUS_REQUEST));
}


//
// Obtener Versi�n del Controlador Fiscal
//
std::string
ImpresorFiscal::ObtenerVersionDelControlador () throw (Excepcion)
{
	// printf ("Comando ObtenerVersionDelControlador ejecutado en ImpresorFiscal\n");
	
	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Obtener Versi�n del Controlador Fiscal");
}


//
// Obtener Datos de Inicializaci�n
//
// Nota: este m�todo no es v�lido solo para los modelos
//		 de 8 bits P614 y P950.
//
void
ImpresorFiscal::ObtenerDatosDeInicializacion (RTA_ObtenerDatosDeInicializacion *R) throw (Excepcion)
{
	// printf ("Comando ObtenerDatosDeInicializacion ejecutado en ImpresorFiscal\n");
	
	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_GET_INIT_DATA));

	// Retornamos los valores de la respuesta
	if (R != NULL)
	{
		R->NroCUIT				= RespuestaString (2, "Obtener Datos de Inicializaci�n");
		R->RazonSocial			= RespuestaString (3, "Obtener Datos de Inicializaci�n");
		R->NroSerie				= RespuestaString (4, "Obtener Datos de Inicializaci�n");
		R->FechaIncializacion	= FECHA (RespuestaString (5, "Obtener Datos de Inicializaci�n"));
		R->NroPOS				= RespuestaString (6, "Obtener Datos de Inicializaci�n");

		// Los campos CodIngBrutos y FechaIniActividades difieren
		// seg�n sea modelo de 8 o de 16 Bits. Es por ello que no
		// se completan ac�.

		R->RespIVA				= static_cast<ImpresorFiscal::TiposDeResponsabilidadesImpresor>(RespuestaString (9, "Obtener Datos de Inicializaci�n") [0]);
	}
}


//
// Obtener Datos de Memoria de Trabajo
//
// Nota: este m�todo no es v�lido solo para los modelos
//		 de 8 bits P614 y P950.
//
void
ImpresorFiscal::ObtenerDatosMemoriaDeTrabajo (RTA_ObtenerDatosMemoriaDeTrabajo *R) throw (Excepcion)
{
	// printf ("Comando ObtenerDatosMemoriaDeTrabajo ejecutado en ImpresorFiscal\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_GET_WORKING_MEMORY));

	// Completamos los par�metros correspondientes a los campos de
	// respuesta comunes a todos los modelos (los de DF) con los
	// valores obtenidos de la respuesta del comando y el resto con
	// sus valores default (luego, seg�n sea el caso, deber�n ser
	// actualizados en al implementaci�n de cada modelo en particular)
	if (R != NULL)
	{
		R->CantidadDFCancelados				= RespuestaInt (2, "Obtener Datos de Memoria de Trabajo");
		R->CantidadDNFEmitidos				= RespuestaInt (3, "Obtener Datos de Memoria de Trabajo");
		R->CantidadDFEmitidos				= RespuestaInt (4, "Obtener Datos de Memoria de Trabajo");
		R->UltimoDocFiscalBC				= RespuestaInt (5, "Obtener Datos de Memoria de Trabajo");
		R->UltimoDocFiscalA					= RespuestaInt (6, "Obtener Datos de Memoria de Trabajo");
		R->MontoVentasDocFiscal				= RespuestaDouble (7, "Obtener Datos de Memoria de Trabajo");
		R->MontoIVADocFiscal				= RespuestaDouble (8, "Obtener Datos de Memoria de Trabajo");
		R->MontoImpInternosDocFiscal		= RespuestaDouble (9, "Obtener Datos de Memoria de Trabajo");
		R->MontoPercepcionesDocFiscal		= 0.0;
		R->MontoIVANoInscriptoDocFiscal		= 0.0;
		R->UltimaNotaCreditoBC				= 0;
		R->UltimaNotaCreditoA				= 0;
		R->MontoVentasNotaCredito			= 0.0;
		R->MontoIVANotaCredito				= 0.0;
		R->MontoImpInternosNotaCredito		= 0.0;
		R->MontoPercepcionesNotaCredito		= 0.0;
		R->MontoIVANoInscriptoNotaCredito	= 0.0;
		R->UltimoRemito						= 0;
		R->CantidadNCCanceladas				= 0;
		R->CantidadDFBCEmitidos				= 0;
		R->CantidadDFAEmitidos				= 0;
		R->CantidadNCBCEmitidas				= 0;
		R->CantidadNCAEmitidas				= 0;
	}
}


//
// Cambiar Responsabilidad frente al IVA
//
void
ImpresorFiscal::CambiarResponsabilidadIVA (const TiposDeResponsabilidadesImpresor RespIVA) throw(Excepcion)
{
	// printf ("Comando CambiarResponsabilidadIVA ejecutado en ImpresorFiscal\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_CHG_IVA_RESP) + FS +
						Caracter (RespIVA));
}


//
// Reporte de Estado por bits del Estado de Impresor
//
bool
ImpresorFiscal::EstadoImpresor (EstadosImpresor Estado, bool ActualizarEstado) throw (Excepcion)
{
	// Verificamos si debemos informar el Estado actual o bien
	// el de la �ltima respuesta recibida
	if (ActualizarEstado)
		// Enviamos el comando fiscal
		SolicitarEstado ();

	// Retornamos la informaci�n obtenida a partir de la respuesta
	return ( (xtoi (RespuestaString (0, "Reporte de Estado por bits del Estado de Impresor").c_str(), 4) & (1 << Estado)) != 0 );
}


//
// Descripci�n individual de Bits del Estado de Impresor
//
std::string
ImpresorFiscal::DescripcionEstadoImpresor (EstadosImpresor Estado) throw(Excepcion)
{
	// printf ("Comando DescripcionEstadoImpresor ejecutado en ImpresorFiscal\n");

	// Utilizamos un 'map' est�tico en lugar de un array de
	// estructuras solo para agilizar las b�squedas (en map
	// las b�squedas son binarias).

	static std::map<EstadosImpresor /* State */, std::string /* Description */> DescripcionBitEstadoImpresor;

	// La primera vez que se invoca al m�todo se carga el mapa.
	if (DescripcionBitEstadoImpresor.size() == 0)
	{
		DescripcionBitEstadoImpresor[ESTADO_IMPRESOR_ERROR_MECANICO]		= "Error mec�nico de impresora";
		DescripcionBitEstadoImpresor[ESTADO_IMPRESOR_FUERA_DE_LINEA]		= "Impresora fuera de l�nea";
		DescripcionBitEstadoImpresor[ESTADO_IMPRESOR_FALTA_PAPEL_JOURNAL]	= "Falta papel en diario";
		DescripcionBitEstadoImpresor[ESTADO_IMPRESOR_FALTA_PAPEL_RECEIPT]	= "Falta papel en ticket";
		DescripcionBitEstadoImpresor[ESTADO_IMPRESOR_TAPA_ABIERTA]			= "La tapa de la impresora est� abierta";
		DescripcionBitEstadoImpresor[ESTADO_IMPRESOR_CAJON_CERRADO]			= "El caj�n de dinero est� cerrado o ausente";
	}

	// Buscamos el estado del CF en el map.
	std::map<EstadosImpresor, std::string>::const_iterator i =
			DescripcionBitEstadoImpresor.find (Estado);

	if (i != DescripcionBitEstadoImpresor.end ())
		return i->second;
	
	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_BIT_NO_VALIDO, "Descripci�n de Bits del Estado de Impresor");
}


//
// Descripci�n conjunta de Bits del Estado de Impresor
//
std::string
ImpresorFiscal::DescripcionBitsEstadoImpresor(unsigned short Flags)  throw (Excepcion)
{
	std::string s;
	for (unsigned i = 0; i < 16; i++)
		if ((1 << i) & Flags)
		{
			if (s != "")
				s += "; ";
			s += DescripcionEstadoImpresor(EstadosImpresor(i));
		}
	return s;
}


//
// Reporte de Estado por bits del Estado Fiscal
//
bool
ImpresorFiscal::EstadoFiscal (EstadosFiscal Estado, bool ActualizarEstado) throw (Excepcion)
{
	// Verificamos si debemos informar el Estado actual o bien
	// el de la �ltima respuesta recibida
	if (ActualizarEstado)
		// Enviamos el comando fiscal
		SolicitarEstado ();

	// Retornamos la informaci�n obtenida de la respuesta
	return ( (xtoi (RespuestaString (1, "Reporte de Estado por bits del Estado Fiscal").c_str(), 4) & (1 << Estado)) != 0 );
}


//
// Descripci�n individual de Bits del Estado Fiscal
//
std::string
ImpresorFiscal::DescripcionEstadoFiscal (EstadosFiscal Estado) throw(Excepcion)
{
	// printf ("Comando DescripcionEstadoFiscal ejecutado en ImpresorFiscal\n");

	// Utilizamos un 'map' est�tico en lugar de un array de
	// estructuras solo para agilizar las b�squedas (en map
	// las b�squedas son binarias).

	static std::map<EstadosFiscal /* State */, std::string /* Description */> DescripcionBitEstadoFiscal;

	// La primera vez que se invoca al m�todo se carga el mapa.
	if (DescripcionBitEstadoFiscal.size() == 0)
	{
		DescripcionBitEstadoFiscal[ESTADO_FISCAL_ERROR_MEMORIA_FISCAL]			= "Falla de memoria fiscal";
		DescripcionBitEstadoFiscal[ESTADO_FISCAL_ERROR_MEMORIA_TRABAJO]			= "Falla de memoria de trabajo";
		DescripcionBitEstadoFiscal[ESTADO_FISCAL_COMANDO_DESCONOCIDO]			= "Comando no reconocido";
		DescripcionBitEstadoFiscal[ESTADO_FISCAL_CAMPO_DATOS_INVALIDO]			= "Campo de datos inv�lido";
		DescripcionBitEstadoFiscal[ESTADO_FISCAL_COMANDO_INVALIDO]				= "Comando inv�lido para el estado actual";
		DescripcionBitEstadoFiscal[ESTADO_FISCAL_DESBORDE_ACUMULADOR]			= "Rebose de total";
		DescripcionBitEstadoFiscal[ESTADO_FISCAL_MEMORIA_FISCAL_LLENA]			= "Memoria fiscal llena, bloqueada o dada de baja";
		DescripcionBitEstadoFiscal[ESTADO_FISCAL_MEMORIA_FISCAL_CASI_LLENA]		= "Memoria fiscal a punto de llenarse";
		DescripcionBitEstadoFiscal[ESTADO_FISCAL_TERMINAL_CERTIFICADA]			= "Terminal fiscal certificada";
		DescripcionBitEstadoFiscal[ESTADO_FISCAL_TERMINAL_FISCALIZADA]			= "Terminal fiscal inicializada";
		DescripcionBitEstadoFiscal[ESTADO_FISCAL_DOCUMENTO_FISCAL_ABIERTO]		= "Comprobante fiscal abierto";
		DescripcionBitEstadoFiscal[ESTADO_FISCAL_DOCUMENTO_ABIERTO]				= "Comprobante abierto";
	}

	// Buscamos el estado del CF en el map.
	std::map<EstadosFiscal, std::string>::const_iterator i =
			DescripcionBitEstadoFiscal.find (Estado);

	if (i != DescripcionBitEstadoFiscal.end ())
		return i->second;
	
	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_BIT_NO_VALIDO, "Descripci�n de Bits del Estado Fiscal");
}


//
// Descripci�n conjunta de Bits del Estado Fiscal
//
std::string
ImpresorFiscal::DescripcionBitsEstadoFiscal(unsigned short Flags)  throw (Excepcion)
{
	std::string s;
	for (unsigned i = 0; i < 16; i++)
		if ((1 << i) & Flags)
		{
			if (s != "")
				s += "; ";
			s += DescripcionEstadoFiscal(EstadosFiscal(i));
		}
	return s;
}


//
// Ultimo Documento Fiscal Tipo BC
//
unsigned
ImpresorFiscal::UltimoDocumentoFiscalBC () throw(Excepcion)
{
	// printf ("Comando UltimoDocumentoFiscalBC ejecutado en ImpresorFiscal\n");

	// Enviamos el comando fiscal
	SolicitarEstado ();

	// Retornamos la informaci�n obtenida de la respuesta
	return RespuestaInt(2, "Ultimo Documento Fiscal Tipo BC");
}


//
// P�ginas de Ultimo Documento
//
// Nota: este m�todo solo es v�lido para aquellos modelos de 16
//		 bits que indican en los comandos de cierre de DF y DNFH
//		 el n�mero de p�ginas del documento reci�n emitido.
//
unsigned
ImpresorFiscal::PaginasDeUltimoDocumento () throw(Excepcion)
{
	// printf ("Comando PaginasDeUltimoDocumento ejecutado en ImpresorFiscal\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "P�ginas de Ultimo Documento");
}


//
// CAI del Ultimo Documento A
//
// Nota: este m�todo solo es v�lido para aquellos modelos
//		 de 16 bits que soportan este comando.
//
void
ImpresorFiscal::CAIUltimoDocumentoA (RTA_CAIUltimoDocumentoA * /* R */) throw (Excepcion)
{
	// printf ("Comando CAIUltimoDocumentoA ejecutado en ImpresorFiscal\n");
	
	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "CAI del Ultimo Documento A");
}


//
// Caj�n Abierto
//
bool
ImpresorFiscal::CajonAbierto () throw(Excepcion)
{
	// printf ("Reportando EstadoCaj�n <%s> en ImpresorFiscal\n", DrawerClosed ? "Cerrado" : "Abierto");

	return !DrawerClosed;
}


//
// Cortar Papel
//
void
ImpresorFiscal::CortarPapel () throw (Excepcion)
{
	// printf ("Comando CortarPapel ejecutado en ImpresorFiscal\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Cortar papel");
}


//
// Mostrar Mensaje en Display
//
void
ImpresorFiscal::MostrarMensajeEnDisplay (const std::string & /* Texto */,
										 const TiposDeDisplay /* Destino */) throw(Excepcion)
{
	// printf ("Comando MostrarMensajeEnDisplay ejecutado en ImpresorFiscal\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Mostrar Mensaje en Display");
}


//
// Establecer Fecha y Hora Fiscal
//
void
ImpresorFiscal::EstablecerFechaHoraFiscal (const FECHA &fecha, const HORA &hora) throw(Excepcion)
{
	// printf ("Comando EstablecerFechaHoraFiscal ejecutado en ImpresorFiscal\n");
	
	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_SET_DATE_TIME) + FS +
						fecha.ToString() + FS + hora.ToString());
}


//
// Obtener Fecha y Hora Fiscal
//
void
ImpresorFiscal::ObtenerFechaHoraFiscal (FECHA &fecha, HORA &hora) throw(Excepcion)
{
	// printf ("Comando ObtenerFechaHoraFiscal ejecutado en ImpresorFiscal\n");
	
	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_GET_DATE_TIME));

	// Retornamos la informaci�n obtenida del CF en las
	// estructuras 'fecha' y 'hora'
	fecha = FECHA (RespuestaString (2, "Obtener Fecha y Hora Fiscal"));
	hora  = HORA  (RespuestaString (3, "Obtener Fecha y Hora Fiscal"));
}


//
// Imprimir C�digo de Barras
//
// Nota: este m�todo no es v�lido solo para los modelos
//		 de 8 bits P614, P950 y P615.
//
void
ImpresorFiscal::ImprimirCodigoDeBarras (TiposDeCodigoDeBarras Tipo,
										const std::string &Codigo,
										bool ImprimeNumeros,
										bool ImprimeAhora)  throw(Excepcion)
{
	// printf ("Comando ImprimirCodigoDeBarras ejecutado en ImpresorFiscal\n");
	
	assert(CodeBarMaxSize != 0);

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_PRINT_BARCODE) + FS +
						Caracter (Tipo) + FS +
						Cadena (Codigo, CodeBarMaxSize) + FS +
						Caracter (ImprimeNumeros == true ? 'N': ' ') + FS +
						Caracter (ImprimeAhora == true ? 'P' : ' '));
}


//
// Reimprimir Comprobante
//
void
ImpresorFiscal::ReimprimirComprobante () throw(Excepcion)
{
	// printf ("Comando ReimprimirComprobante ejecutado en ImpresorFiscal\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Reimprimir Comprobante");
}


//
// Cargar Logo
//
// Nota: este m�todo es v�lido solo para los modelos de 16 Bits.
//
void
ImpresorFiscal::CargarLogo (const std::string & /* ArchivoBMP */) throw(Excepcion)
{
	// printf ("Comando CargarLogo ejecutado en ImpresorFiscal\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Cargar Logo");
}


//
// Informaci�n de caracter�sticas soportadas por las
// diferentes familias de controladores fiscales.
//

//
// Capacidad de Longitud de Texto Fiscal (Ticket y Slip)
//
void
ImpresorFiscal::CapacidadLongitudTextoFiscal (unsigned &TextTicketSize, unsigned &TextSlipSize)
{
	// printf ("Comando CapacidadLongitudTextoFiscal ejecutado en ImpresorFiscal\n");

	bool SoportaTicket, SoportaSlip, SoportaFASlip, SoportaNCSlip;
	CapacidadEstacion (SoportaTicket, SoportaSlip);
	CapacidadDocumentoSlip (SoportaFASlip, SoportaNCSlip);

	TextTicketSize = SoportaTicket ? PrintFiscalTextTicketSize : 0;
	TextSlipSize   = (SoportaSlip && (SoportaFASlip || SoportaNCSlip))
						? PrintFiscalTextSlipSize : 0;
}


//
// Capacidad de Longitud de Texto de Venta (Ticket y Slip)
//
void
ImpresorFiscal::CapacidadLongitudTextoVenta (unsigned &TextTicketSize, unsigned &TextSlipSize)
{
	// printf ("Comando CapacidadLongitudTextoVenta ejecutado en ImpresorFiscal\n");

	bool SoportaTicket, SoportaSlip, SoportaFASlip, SoportaNCSlip;
	CapacidadEstacion (SoportaTicket, SoportaSlip);
	CapacidadDocumentoSlip (SoportaFASlip, SoportaNCSlip);

	TextTicketSize = SoportaTicket ? PrintItemTextTicketSize : 0;
	TextSlipSize   = (SoportaSlip && (SoportaFASlip || SoportaNCSlip))
						? PrintItemTextSlipSize : 0;
}


//
// Capacidad de Longitud de Texto No Fiscal (Ticket y Slip)
//
void
ImpresorFiscal::CapacidadLongitudTextoNoFiscal (unsigned &TextTicketSize, unsigned &TextSlipSize)
{
	// printf ("Comando CapacidadLongitudTextoNoFiscal ejecutado en ImpresorFiscal\n");

	bool SoportaTicket, SoportaSlip;
	CapacidadEstacion (SoportaTicket, SoportaSlip);

	TextTicketSize = SoportaTicket ? PrintNonFiscalTextTicketSize : 0;
	TextSlipSize   = SoportaSlip ? PrintNonFiscalTextSlipSize : 0;
}


//
// Capacidad de Longitud de �ncabezado / Cola (Ticket y Slip)
//
void
ImpresorFiscal::CapacidadLongitudTextoEncabezadoCola (unsigned &TextTicketSize, unsigned &TextSlipSize)
{
	// printf ("Comando CapacidadLongitudTextoEncabezadoCola ejecutado en ImpresorFiscal\n");

	bool SoportaTicket, SoportaSlip;
	CapacidadEstacion (SoportaTicket, SoportaSlip);

	TextTicketSize = SoportaTicket ? HeaderTrailerTicketSize : 0;
	TextSlipSize   = SoportaSlip ? HeaderTrailerSlipSize : 0;
}


//
// Capacidad de DF soportado
//
// Indica si el DF indicado en el par�metro Tipo es soportado por el modelo actual
//
void
ImpresorFiscal::CapacidadDFSoportado (unsigned Tipo, bool &Soportado)
{
	// printf ("Comando CapacidadDFSoportado ejecutado en ImpresorFiscal\n");

	Soportado = DFSoportados.find (Tipo) != DFSoportados.end ();
}


//
// Capacidad de DNFH soportado
//
// Indica si el DNFH indicado en el par�metro Tipo es soportado por el modelo actual
//
void
ImpresorFiscal::CapacidadDNFHSoportado (unsigned Tipo, bool &Soportado)
{
	// printf ("Comando CapacidadDNFHSoportado ejecutado en ImpresorFiscal\n");

	Soportado = DNFHSoportados.find (Tipo) != DNFHSoportados.end ();
}


//
// Estado Interno del Impresor
//
// Nota: este m�todo es v�lido solo para el modelo PL23.
//
void
ImpresorFiscal::EstadoInternoImpresor (RTA_EstadoInternoImpresor * /* R */) throw (Excepcion)
{
	// printf ("Comando EstadoInternoImpresor ejecutado en ImpresorFiscal\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Estado Interno del Impresor");
}


//
// Establecer Interlineado de Impresi�n
//
// Nota: este m�todo es v�lido solo para el modelo PL23.
//
void
ImpresorFiscal::EstablecerInterlineado (const unsigned /* LineasPorPulgada */) throw (Excepcion)
{
	// printf ("Comando EstablecerInterlineado ejecutado en ImpresorFiscal\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Establecer Interlineado de Impresi�n");
}


//
// Obtener Interlineado de Impresi�n
//
// Nota: este m�todo es v�lido solo para el modelo PL23.
//
unsigned
ImpresorFiscal::ObtenerInterlineado () throw (Excepcion)
{
	// printf ("Comando ObtenerInterlineado ejecutado en ImpresorFiscal\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Obtener Interlineado de Impresi�n");
}


//
// Apagar el Indicador del Tambor del Impresor
//
// Nota: este m�todo es v�lido solo para el modelo PL23.
//
void
ImpresorFiscal::ApagarIndicadorDeTambor () throw (Excepcion)
{
	// printf ("Comando ApagarIndicadorDeTambor ejecutado en ImpresorFiscal\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Apagar el Indicador del Tambor del Impresor");
}


// ############################################################ //
//																//
//			M E T O D O S   D E    C O N F I G U R A C I O N	//
//																//
// ############################################################ //

//
// Especificar L�nea de Nombre de Fantas�a
//
// Nota: este m�todo no es v�lido solo para los modelos
//		 de 8 bits P614 y P950.
//
void
ImpresorFiscal::EspecificarNombreDeFantasia (unsigned Linea, const std::string &Texto) throw (Excepcion)
{
	// printf ("Comando EspecificarNombreDeFantasia ejecutado en ImpresorFiscal\n");
	
	assert(FantasyNameSize != 0);

	// Enviamos el comando fiscal y evaluamos los status
	if (Texto.length() != 0)
		EnviarComandoFiscal(OpCode(CMD_SET_FANTASY_NAME) + FS +
							Numero (Linea) + FS +
							Cadena (Texto, FantasyNameSize));
	else
		EnviarComandoFiscal(OpCode(CMD_SET_FANTASY_NAME) + FS +
							Numero (Linea) + FS + ERASE_LINE);
}


//
// Especificar L�nea de Encabezado o Cola
//
void
ImpresorFiscal::EspecificarEncabezadoCola (unsigned Linea, const std::string &Texto) throw (Excepcion)
{
	// printf ("Comando EspecificarEncabezadoCola ejecutado en ImpresorFiscal\n");

	assert(HeaderTrailerSize != 0);

	// Enviamos el comando fiscal y evaluamos los status
	if (Texto.length() != 0)
		EnviarComandoFiscal(OpCode(CMD_SET_HEADER_TRAILER) + FS +
							Numero (Linea) + FS +
							Cadena (Texto, HeaderTrailerSize));
	else
		EnviarComandoFiscal(OpCode(CMD_SET_HEADER_TRAILER) + FS +
							Numero (Linea) + FS + ERASE_LINE);
}


//
// Obtener L�nea de Encabezado o Cola
//
std::string
ImpresorFiscal::ObtenerEncabezadoCola (unsigned Linea) throw (Excepcion)
{
	// printf ("Comando ObtenerEncabezadoCola ejecutado en ImpresorFiscal\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_GET_HEADER_TRAILER) + FS + Numero (Linea));

	// Retornamos el texto de la L�nea de Encabezamiento o Cola
	return RespuestaString(2, "Obtener L�nea de Encabezado o Cola");
}


//
// Borrar L�neas de Nombre de Fantas�a, Encabezado y Cola
//
// Nota: este m�todo tendr� una implementaci�n particular para los modelos
//		 de 8 bits P614 y P950 (no reconocen L�neas de Fantas�a por lo que
//       en los mismos solo se podr� borrar el resto) y PR5 versi�n 2.01.
//
void
ImpresorFiscal::BorrarFantasiaEncabezadoCola (bool BorrarFantasia, bool BorrarEncabezado, bool BorrarCola) throw (Excepcion)
{
	// printf ("Comando BorrarFantasiaEncabezadoCola ejecutado en ImpresorFiscal\n");
	
	if ( BorrarFantasia )
		for ( unsigned Linea = First_Fantasy_Line; Linea <= Last_Fantasy_Line; Linea++ )
			EnviarComandoFiscal(OpCode(CMD_SET_FANTASY_NAME) + FS +
								Numero (Linea) + FS + ERASE_LINE);

	if ( BorrarEncabezado )
		for ( unsigned Linea = First_Header_Line; Linea <= Last_Header_Line; Linea++ )
			EnviarComandoFiscal(OpCode(CMD_SET_HEADER_TRAILER) + FS +
								Numero (Linea) + FS + ERASE_LINE);

	if ( BorrarCola )
		for ( unsigned Linea = First_Trailer_Line; Linea <= Last_Trailer_Line; Linea++ )
			EnviarComandoFiscal(OpCode(CMD_SET_HEADER_TRAILER) + FS +
								Numero (Linea) + FS + ERASE_LINE);
}


//
// Configurar Controlador por Par�metros Individuales
//
// Nota: este m�todo no es v�lido solo para los modelos
//		 de 8 bits P614 y P950.
//
void
ImpresorFiscal::ConfigurarControlador (ParametrosDeConfiguracion Parametro, const std::string &Valor) throw (Excepcion)
{
	// printf ("Comando ConfigurarControlador ejecutado en ImpresorFiscal\n");

	assert(TotalTenderTextSize != 0);

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_CONFIGURE_CONTROLLER_BY_ONE) + FS +
						Caracter (Parametro) + FS +
						Cadena (Valor, TotalTenderTextSize));
}


//
// Configurar Controlador por Bloque de Par�metros
//
// Nota: este m�todo no es v�lido solo para los modelos
//               de 8 bits P614_100, P950_100, P262_100
//               y P272_100.
//
void
ImpresorFiscal::ConfigurarControladorPorBloque (
							double LimiteConsumidorFinal,
							double LimiteTicketFactura,
							double PorcentajeIVANoInscripto,
							NumerosDeCopias TipoDeCopiasMaximo,
							bool ImpresionCambio,
							bool ImpresionLeyendasOpcionales,
							TiposDeCorteDePapel TipoDeCorte) throw (Excepcion)
{
	// printf ("Comando ConfigurarControladorPorBloque ejecutado en ImpresorFiscal\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_CONFIGURE_CONTROLLER_BY_BLOCK) + FS +
						Numero (LimiteConsumidorFinal, 2) + FS +
						Numero (LimiteTicketFactura, 2) + FS +
						Numero (PorcentajeIVANoInscripto, 2) + FS +
						Caracter (TipoDeCopiasMaximo) + FS +
						(ImpresionCambio ? "P" : "N") + FS +
						(ImpresionLeyendasOpcionales ? "P": "N") + FS +
						Caracter (TipoDeCorte));
}


//
// Configurar Controlador Completo
//
// Nota: esta m�todo solo es v�lido para modelos de CF
//		 de 16 bits.
//
void
ImpresorFiscal::ConfigurarControladorCompleto (
							bool /* Imprimir */,
							bool /* Defaults */,
							const double * /* LimiteConsumidorFinal */,
							const double * /* LimiteTicketFactura */,
							const double * /* PorcentajeIVANoInscripto */,
							const NumerosDeCopias * /* TipoDeCopiasMaximo */,
							const bool * /* ImprimeCambio */,
							const bool * /* ImprimeLeyendasOpcionales */,
							const TiposDeCorteDePapel * /* TipoDeCorte */,
							const bool * /* ImprimeMarco */,
							const bool * /* ReImprimeDocumentos */,
							const std::string * /* DescripcionDelMedioDePago */,
							const bool * /* Sonido */,
							const TiposDeAltoHoja * /* AltoHoja */,
							const TiposDeAnchoHoja * /* AnchoHoja */,
							const TiposDeEstacion * /* EstacionImpresionReportesXZ */,
							const TiposDeModoImpresion * /* ModoImpresion */) throw (Excepcion)
{
	// printf ("Comando ConfigurarControladorCompleto ejecutado en ImpresorFiscal\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Configurar Controlador Completo");
}


// ############################################################ //
//																//
//			M E T O D O S   D E    R E P O R T E S				//
//																//
// ############################################################ //

//
// Reporte Z
//
void
ImpresorFiscal::ReporteZ (RTA_ReporteZX *R) throw (Excepcion)
{
	// printf ("Comando ReporteZ ejecutado en ImpresorFiscal\n");
	
	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_DAILY_CLOSE) + FS + "Z");

	// Completamos los par�metros correspondientes a los campos de
	// respuesta comunes a todos los modelos (los de DF) con los
	// valores obtenidos de la respuesta del comando y el resto con
	// sus valores default (luego, seg�n sea el caso, deber�n ser
	// actualizados en la implementaci�n de cada modelo en particular)
	if (R != NULL)
	{
		R->NumeroReporte					= RespuestaInt (2, "Reporte Z");
		R->CantidadDFCancelados				= RespuestaInt (3, "Reporte Z");
		R->CantidadDNFHEmitidos				= RespuestaInt (4, "Reporte Z");
		R->CantidadDNFEmitidos				= RespuestaInt (5, "Reporte Z");
		R->CantidadDFEmitidos				= RespuestaInt (6, "Reporte Z");
		R->UltimoDocFiscalBC				= RespuestaInt (8, "Reporte Z");
		R->UltimoDocFiscalA					= RespuestaInt (9, "Reporte Z");
		R->MontoVentasDocFiscal				= RespuestaDouble (10, "Reporte Z");
		R->MontoIVADocFiscal				= RespuestaDouble (11, "Reporte Z");
		R->MontoImpInternosDocFiscal		= 0.0;
		R->MontoPercepcionesDocFiscal		= 0.0;
		R->MontoIVANoInscriptoDocFiscal		= 0.0;
		R->UltimaNotaCreditoBC				= 0;
		R->UltimaNotaCreditoA				= 0;
		R->MontoVentasNotaCredito			= 0.0;
		R->MontoIVANotaCredito				= 0.0;
		R->MontoImpInternosNotaCredito		= 0.0;
		R->MontoPercepcionesNotaCredito		= 0.0;
		R->MontoIVANoInscriptoNotaCredito	= 0.0;
		R->UltimoRemito						= 0;
		R->CantidadNCCanceladas				= 0;
		R->CantidadDFBCEmitidos				= 0;
		R->CantidadDFAEmitidos				= 0;
		R->CantidadNCBCEmitidas				= 0;
		R->CantidadNCAEmitidas				= 0;
	}

	// Reinicializamos 'NumberOfPages' en 0 y
	// 'TicketBCAbierto' en false.
	NumberOfPages	= 0;
	TicketBCAbierto	= false;
}


//
// Reporte X
//
void
ImpresorFiscal::ReporteX (RTA_ReporteZX *R) throw (Excepcion)
{
	// printf ("Comando ReporteX ejecutado en ImpresorFiscal\n");
	
	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_DAILY_CLOSE) + FS + "X");

	// Completamos los par�metros correspondientes a los campos de
	// respuesta comunes a todos los modelos (los de DF) con los
	// valores obtenidos de la respuesta del comando y el resto con
	// sus valores default (luego, seg�n sea el caso, deber�n ser
	// actualizados en la implementaci�n de cada modelo en particular)
	if (R != NULL)
	{
		R->NumeroReporte					= RespuestaInt (2, "Reporte X");
		R->CantidadDFCancelados				= RespuestaInt (3, "Reporte X");
		R->CantidadDNFHEmitidos				= RespuestaInt (4, "Reporte X");
		R->CantidadDNFEmitidos				= RespuestaInt (5, "Reporte X");
		R->CantidadDFEmitidos				= RespuestaInt (6, "Reporte X");
		R->UltimoDocFiscalBC				= RespuestaInt (8, "Reporte X");
		R->UltimoDocFiscalA					= RespuestaInt (9, "Reporte X");
		R->MontoVentasDocFiscal				= RespuestaDouble (10, "Reporte X");
		R->MontoIVADocFiscal				= RespuestaDouble (11, "Reporte X");
		R->MontoImpInternosDocFiscal		= 0.0;
		R->MontoPercepcionesDocFiscal		= 0.0;
		R->MontoIVANoInscriptoDocFiscal		= 0.0;
		R->UltimaNotaCreditoBC				= 0;
		R->UltimaNotaCreditoA				= 0;
		R->MontoVentasNotaCredito			= 0.0;
		R->MontoIVANotaCredito				= 0.0;
		R->MontoImpInternosNotaCredito		= 0.0;
		R->MontoPercepcionesNotaCredito		= 0.0;
		R->MontoIVANoInscriptoNotaCredito	= 0.0;
		R->UltimoRemito						= 0;
		R->CantidadNCCanceladas				= 0;
		R->CantidadDFBCEmitidos				= 0;
		R->CantidadDFAEmitidos				= 0;
		R->CantidadNCBCEmitidas				= 0;
		R->CantidadNCAEmitidas				= 0;
	}

	// Reinicializamos 'NumberOfPages' en 0 y
	// 'TicketBCAbierto' en false.
	NumberOfPages	= 0;
	TicketBCAbierto	= false;
}


//
// Reporte Impreso de Auditor�a por Fechas de Z
//
void
ImpresorFiscal::ReporteZPorFechas (FECHA FIni, FECHA FFin, bool ReporteGlobal)  throw (Excepcion)
{
	// printf ("Comando ReporteZPorFechas ejecutado en ImpresorFiscal\n");
	
	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_DAILY_CLOSE_BY_DATE) + FS +
						FIni.ToString() + FS + FFin.ToString () + FS +
						(ReporteGlobal ? "T" : "I"));

	// Reinicializamos 'NumberOfPages' en 0 y
	// 'TicketBCAbierto' en false.
	NumberOfPages	= 0;
	TicketBCAbierto	= false;
}


//
// Reporte Impreso de Auditor�a por N�meros de Z
//
void
ImpresorFiscal::ReporteZPorNumeros (unsigned ZIni, unsigned ZFin, bool ReporteGlobal)  throw (Excepcion)
{
	// printf ("Comando ReporteZPorNumeros ejecutado en ImpresorFiscal\n");
	
	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_DAILY_CLOSE_BY_NUMBER) + FS +
						Numero (ZIni) + FS + Numero (ZFin) + FS +
						(ReporteGlobal ? "T" : "I"));

	// Reinicializamos 'NumberOfPages' en 0 y
	// 'TicketBCAbierto' en false.
	NumberOfPages	= 0;
	TicketBCAbierto	= false;
}


//
// Capacidad de Cierres Z Restantes
//
void
ImpresorFiscal::CapacidadRestante (RTA_CapacidadRestante *R) throw (Excepcion)
{
	// printf ("Comando CapacidadRestante ejecutado en ImpresorFiscal\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_HISTORY_CAPACITY));

	// Retornamos el n�mero de la Zeta actual y las Zetas restantes
	if (R != NULL)
	{
		R->TotalCierresZeta = RespuestaInt (2, "Capacidad de Cierres Z Restantes");
		R->CierreZetaActual = RespuestaInt (3, "Capacidad de Cierres Z Restantes");
	}
}


//
// Obtener Tabla de IVAs
//
// Nota: este m�todo no es v�lido solo para los modelos
//		 de 8 bits P614 y P950.
//
void
ImpresorFiscal::ObtenerTablaIVAs (TablaIVAs &Tabla) throw (Excepcion)
{
	// printf ("Comando ObtenerTablaIVAs ejecutado en ImpresorFiscal\n");

	IVA Iva;
	PERCEPCION Percepcion;
	double Monto;
	PERCEPCION_POR_DESCRIPCION PercPorDescripcion;

	// Inicializamos Tabla.
	Tabla = TablaIVAs ();

	// Recuperamos el primer registro usando 'SendFirstIVA'.

	// Enviamos el comando fiscal y evaluamos los status

	// Evaluamos si se trata de Resumen de IVA del dia o del documento.
	if (ModalidadResumenIVA == RESUMEN_IVA_DIARIO)
		EnviarComandoFiscal(OpCode(CMD_SEND_FIRST_IVA) + FS + "D");
	else
		EnviarComandoFiscal(OpCode(CMD_SEND_FIRST_IVA));

	TiposDeRegistrosIVA Registro = static_cast<ImpresorFiscal::TiposDeRegistrosIVA>(RespuestaString (2, "Obtener Tabla de IVAs") [0]);

	while ( Registro != REGISTRO_FIN_INFO_IVAS )
	{

		// Evaluamos el valor del Registro y en funci�n del mismo
		// procesamos las diferentes respuestas.
		switch (Registro)
		{
			case REGISTRO_ALICUOTA_IVA_DF:
			case REGISTRO_ALICUOTA_IVA_NC:

				// Cargamos la estructura 'Iva' con los valores
				// recuperados de la respuesta.
				Iva.Alicuota			= RespuestaDouble (3, "Obtener Tabla de IVAs");
				Iva.MontoIVA			= RespuestaDouble (4, "Obtener Tabla de IVAs");
				Iva.MontoImpInternos	= RespuestaDouble (5, "Obtener Tabla de IVAs");
				Iva.MontoIVANoInscripto = RespuestaDouble (6, "Obtener Tabla de IVAs");
				Iva.MontoNetoSinIVA		= RespuestaDouble (7, "Obtener Tabla de IVAs");

				// Insertamos 'Iva' en la lista de IVAs que corresponda.
				if (Registro == REGISTRO_ALICUOTA_IVA_DF)
					Tabla.DocFiscales.IVAs.push_back (Iva);
				else
					Tabla.NotasCredito.IVAs.push_back (Iva);

				break;

			case REGISTRO_PERCEPCION_DF:
			case REGISTRO_PERCEPCION_NC:

				// Verificamos si se trata de una Percepci�n a una
				// determinada al�cuota IVA o si se trata de una
				// Percepci�n General.
				if ( RespuestaString (3, "Obtener Tabla de IVAs").substr (0, 1) == "*" )
				{
					// Si es Percepci�n General recuperamos el monto.
					Monto				= RespuestaDouble (4, "Obtener Tabla de IVAs");

					// Insertamos 'Monto' en la lista de
					// PercepcionesGenerales que corresponda.
					if (Registro == REGISTRO_PERCEPCION_DF)
						Tabla.DocFiscales.PercepcionesGenerales.push_back (Monto);
					else
						Tabla.NotasCredito.PercepcionesGenerales.push_back (Monto);
				}
				else
				{
					// Si es Percepci�n a una determinada al�cuota
					// IVA cargamos la estructura 'Percepcion' con
					// los valores recuperados de la respuesta.
					Percepcion.Alicuota = RespuestaDouble (3, "Obtener Tabla de IVAs");
					Percepcion.Monto	= RespuestaDouble (4, "Obtener Tabla de IVAs");

					// Insertamos 'Percepcion' en la lista de
					// PercepcionesIVA que corresponda.
					if (Registro == REGISTRO_PERCEPCION_DF)
						Tabla.DocFiscales.PercepcionesIVA.push_back (Percepcion);
					else
						Tabla.NotasCredito.PercepcionesIVA.push_back (Percepcion);
				}

				break;

			case REGISTRO_MONTO_TOTAL_DF:

				// Recuperamos el monto total de la respuesta y
				// lo guardamos en DcoFiscales.
				Tabla.DocFiscales.MontoTotal = RespuestaDouble (3, "Obtener Tabla de IVAs");

				break;

			case REGISTRO_MONTO_TOTAL_NC:

				// Recuperamos el monto total de la respuesta y
				// lo guardamos en NotasCredito.
				Tabla.NotasCredito.MontoTotal = RespuestaDouble (3, "Obtener Tabla de IVAs");

				break;

			case REGISTRO_PERC_DESCRIP_DF:
			case REGISTRO_PERC_DESCRIP_NC:

				// Cargamos la estructura 'PercPorDescripcion'
				// con los valores recuperados de la respuesta.
				PercPorDescripcion.Descripcion	= RespuestaString (3, "Obtener Tabla de IVAs");
				PercPorDescripcion.Monto		= RespuestaDouble (4, "Obtener Tabla de IVAs");

				// Insertamos 'PercPorDescripcion' en la lista
				// de PercepcionesPorDescripcion que corresponda.
				if (Registro == REGISTRO_PERC_DESCRIP_DF)
					Tabla.DocFiscales.PercepcionesPorDescripcion.push_back (PercPorDescripcion);
				else
					Tabla.NotasCredito.PercepcionesPorDescripcion.push_back (PercPorDescripcion);

				break;

			default:
				break;
		}

		// Recuperamos el siguiente registro usando 'SendNextIVA'.

		// Enviamos el comando fiscal y evaluamos los status
		EnviarComandoFiscal(OpCode(CMD_SEND_NEXT_IVA));

		Registro = static_cast<ImpresorFiscal::TiposDeRegistrosIVA>(RespuestaString (2, "Obtener Tabla de IVAs") [0]);
	};
}


// ############################################################ //
//																//
//			M E T O D O S   D E    D F							//
//																//
// ############################################################ //

//
// Abrir DF
// Env�a el comando de Apertura de DF en la estaci�n asociada
//
void
ImpresorFiscal::AbrirDF (DocumentosFiscales Tipo) throw (Excepcion)
{
	// printf ("Comando AbrirDF ejecutado en ImpresorFiscal\n");

	// Verificamos si el Tipo de DF es v�lido para este modelo
	if ( DFSoportados.find (Tipo) == DFSoportados.end () )
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Abrir DF en ImpresorFiscal");

	// Seteamos la estaci�n del DF
	Estacion = (Tipo == TICKET_FACTURA_A	 ||
				Tipo == TICKET_FACTURA_B	 ||
				Tipo == TICKET_NOTA_DEBITO_A ||
				Tipo == TICKET_NOTA_DEBITO_B ||
				Tipo == TICKET_C)
					? ESTACION_TICKET : ESTACION_SLIP;

	// Seteamos el tipo de DF apropiado

	// La siguiente tabla contiene los Tipos de DF que
	// deben ser convertidos antes de armar el comando
	struct IdentificadorDF
	{
		DocumentosFiscales Tipo;
		DocumentosFiscales Id;
	};

	IdentificadorDF Identificadores[] =
	{
		{
			FACTURA_A,				TICKET_FACTURA_A
		},
		{
			FACTURA_B,				TICKET_FACTURA_B
		},
		{
			TICKET_NOTA_DEBITO_A,	NOTA_DEBITO_A
		},
		{
			TICKET_NOTA_DEBITO_B,	NOTA_DEBITO_B
		}
	};

	const unsigned MAX_TIPOS = sizeof(Identificadores) / sizeof(IdentificadorDF);
	for (unsigned i = 0; i < MAX_TIPOS; i++)
		if (Identificadores[i].Tipo == Tipo)
		{
			Tipo = Identificadores[i].Id;
			break;
		}
		
	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_OPEN_FISCAL_RECEIPT) + FS +
						Caracter (Tipo) + FS +
						Caracter (Estacion));

	// Reinicializamos 'FirstDocNum' y 'NumberOfPages' en 0.
	FirstDocNum = 0;
	NumberOfPages = 0;

	// Inicializamos la variable 'TicketBCAbierto'
	TicketBCAbierto = (Tipo == TICKET_C);
}


//
// Imprimir Texto Fiscal
//
void
ImpresorFiscal::ImprimirTextoFiscal (const std::string &Texto) throw (Excepcion)
{
	// printf ("Comando ImprimirTextoFiscal ejecutado en ImpresorFiscal\n");

	unsigned PrintFiscalTextSize = (Estacion == ImpresorFiscal::ESTACION_TICKET)
		? PrintFiscalTextTicketSize : PrintFiscalTextSlipSize;

	assert(PrintFiscalTextSize != 0);

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_PRINT_FISCAL_TEXT) + FS +
								Cadena (Texto, PrintFiscalTextSize) + FS +
								Caracter(ModoDisplay));
}


//
// Imprimir Item
//
void
ImpresorFiscal::ImprimirItem (	const std::string &Texto,
								double Cantidad, double Monto,
								double IVA, double ImpuestosInternos,
								bool EnNegativo) throw (Excepcion)
{
	// printf ("Comando ImprimirItem ejecutado en ImpresorFiscal\n");

	unsigned i, j, OffCnt;
	unsigned FiscalTextLen, FiscalTextLines, TotalWidth;

	unsigned MaxFiscalTextLines = ( TicketBCAbierto ? MaxFiscalTextLinesTicket
													: MaxFiscalTextLinesInvoice );

	unsigned PrintFiscalTextSize = (Estacion == ImpresorFiscal::ESTACION_TICKET)
		? PrintFiscalTextTicketSize : PrintFiscalTextSlipSize;

	unsigned PrintItemTextSize = (Estacion == ImpresorFiscal::ESTACION_TICKET)
		? PrintItemTextTicketSize : PrintItemTextSlipSize;

	assert(PrintFiscalTextSize != 0);
	assert(PrintItemTextSize != 0);
	assert(PriceDecimals != 0);
	assert(MaxFiscalTextLines != 0);

	std::string Description(Texto);
	std::string Line;

	const char *Trailer;

	// Chequea que la descripci�n haya sido asignado y
	// elimina los espacios a la derecha.
	unsigned Ini;
	unsigned Fin = Description.find_last_not_of (" ");

	if ( Description.length() == 0 || Fin == std::string::npos )
		Description = " ";
	else
		Description = Description.substr (0, Fin+1);

	// Verifica si el tama�o del texto no supera el m�ximo.
	// En tal caso no modifica el texto. En caso contrario
	// eval�a como debe adaptar el texto para que no se den
	// errores, para lo cual tiene en cuenta si debe operar
	// o no con 'DescripcionesLargas'.
	if (Description.length() > PrintItemTextSize)
	{
		// Si no estamos operando con 'DescripcionesLargas' o
		// si el texto supera el m�ximo y no tiene ning�n ' '
		// en medio lo truncamos al tama�o m�ximo menos 3 y
		// completamos con '...' al final, indicando que fue
		// truncado.

		if ( !DescripcionesLargas ||
			 Description.find (" ") == std::string::npos )
		{
			Description = (Description.substr(0, PrintItemTextSize-3) + "...");
		}
		// En caso contrario procedemos a repartir el texto entre
		// las l�neas de texto Fiscal y la l�nea de descrici�n del
		// item propiamente dicha.
		else
		{
			TotalWidth = Description.length();
			FiscalTextLen = TotalWidth - PrintItemTextSize;
			for (i = 0, FiscalTextLines = 0; i < FiscalTextLen &&
				 FiscalTextLines < MaxFiscalTextLines;  )
			{
				// Saltea los espacios a la izquerda si no es
				// la primer l�nea de texto fiscal.
				if ( FiscalTextLines != 0 )
					i += Description.substr (i).find_first_not_of (" ");

				// Busca el corte para la palabra
				j = Description.find_last_of (" ", i + PrintFiscalTextSize);

				// Borra Cnt. de Offset de L�neas que superan PFT.
				OffCnt = 0;

				// Si no encontr� espacios de atr�s para adelante,
				// verifica si quedan caracteres para el PLI
				// (busca al menos un ' ').
				if (j == std::string::npos || j <= i)
				{
					j = PrintFiscalTextSize;
					if ( Description.find (" ", i + j) != std::string::npos )
						// Si restan caracteres para el PLI, termina la
						// l�nea con "..." y se posiciona en el caracter
						// siguiente al primer ' ' para continuar con el
						// procesamiento de la siguiente l�nea.
						OffCnt = Description.substr (i+j).find_first_of (" ") + 1;
					else
						// Si no restan caracteres para el PLI, no genera
						// el texto fiscal y procesa la l�nea como PLI.
						break;
				}
				// Si encontr� alg�n espacio, corto donde encontr� el espacio.
				else
					j -= i;

				FiscalTextLines++;
				// Si es la ultima l�nea de PFT y restan m�s caracteres de
				// los que ocupa PLI o si la ultima l�nea superaba el tama�o
				// de PFT, corta la l�nea y la concluye con "...".
				if ( ( FiscalTextLines == MaxFiscalTextLines &&
					   Description.substr (i+j).length () > PrintItemTextSize &&
					   j >= (PrintFiscalTextSize - 3) ) ||
					 ( OffCnt != 0 ) )
				{
					if ( ((j = Description.substr (i).find_last_not_of (" ", j)) != std::string::npos) &&
						  (j >= (PrintFiscalTextSize - 3)) )
						j -= 3;
					Trailer = "...";
				}
				else
					Trailer = "";
				
				Line = Description.substr (i, j) + Trailer;
				i += OffCnt + Line.length();

				// Verifica que la linea completa no sea de ' ' (solo
				// puede ocurrir en la primera linea ya que es la unica
				// en la que no se eliminan los ' ' a la izquierda) y
				// en caso de serlo no se genera el PFT y se eliminan
				// los espacios que siguen para que la siguiente linea
				// comience con una palabra.
				if ( Line.find_last_not_of (" ", j-1) != std::string::npos )
					ImprimirTextoFiscal(Line);
				else
				{
					FiscalTextLines--;
					// Elimina los espacios a la izquerda de la
					// siguiente linea ...
					i = Description.find_first_not_of (" ", i);
				}
			}

			// Al salir del lazo del for nos quedamos solo con
			// la parte restante del texto.
			Description = Description.substr(i);

			// Si hubo alguna l�nea de Texto Fiscal previa elimina
			// los espacios a la izquierda de PLI.
			if ( FiscalTextLines != 0 )
				if ((Ini = Description.find_first_not_of (" ")) != std::string::npos)
					Description = Description.substr (Ini);

			// Si qued� m�s de lo que entra en una l�nea de PrintLineItem,
			// la corto de atr�s para adelante.
			if (Description.length() > PrintItemTextSize)
				Description = "..." + Description.substr(Description.length()-PrintItemTextSize+3);
		}
	}

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_PRINT_LINE_ITEM) + FS +
								Cadena (Description, PrintItemTextSize) + FS +
								Numero (Cantidad, 10) + FS +
								Numero (Monto, PriceDecimals) + FS +
								Numero (IVA, 2) + FS +
								(EnNegativo ? "m" : "M") + FS +
								IndicadorII (ImpuestosInternos, Monto, IVA) + FS +
								Caracter (ModoDisplay) + FS +
								Caracter (ModalidadPrecio));
}


//
// Descuento a Ultimo Item
//
// Nota: esta implementaci�n vale para todos los modelos de CF
//       excepto para P614 y P950, los cuales por no tener el
//       comando deber�n implementarlo como una venta.
//
void
ImpresorFiscal::DescuentoUltimoItem (const std::string &Texto, double Monto, bool EnNegativo) throw (Excepcion)
{
	// printf ("Comando DescuentoUltimoItem ejecutado en ImpresorFiscal\n");

	unsigned PrintItemTextSize = (Estacion == ImpresorFiscal::ESTACION_TICKET)
		? PrintItemTextTicketSize : PrintItemTextSlipSize;

	assert(PrintItemTextSize != 0);

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_LAST_ITEM_DISCOUNT) + FS +
								Cadena (Texto, PrintItemTextSize) + FS +
								Numero (Monto, 2) + FS +
								(EnNegativo ? "m" : "M") + FS +
								Caracter (ModoDisplay) + FS +
								Caracter (ModalidadPrecio));
}


//
// Descuento General
//
// Nota: esta implementaci�n vale para todos los modelos de CF
//       excepto para P614 y P950, los cuales por no tener el
//       comando deber�n implementarlo como una venta.
//
void
ImpresorFiscal::DescuentoGeneral (const std::string &Texto, double Monto, bool EnNegativo) throw (Excepcion)
{
	// printf ("Comando DescuentoGeneral ejecutado en ImpresorFiscal\n");

	unsigned PrintItemTextSize = (Estacion == ImpresorFiscal::ESTACION_TICKET)
		? PrintItemTextTicketSize : PrintItemTextSlipSize;

	assert(PrintItemTextSize != 0);

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_GENERAL_DISCOUNT) + FS +
								Cadena (Texto, PrintItemTextSize) + FS +
								Numero (Monto, 2) + FS +
								(EnNegativo ? "m" : "M") + FS +
								Caracter (ModoDisplay) + FS +
								Caracter (ModalidadPrecio));
}


//
// Bonificaci�n, Recargo y Devoluci�n de Envases
//
// Nota: este m�todo no es v�lido solo para los modelos
//		 de 8 bits P614 y P950.
//
void
ImpresorFiscal::DevolucionDescuento (const std::string &Texto,
									 double Monto, double IVA,
									 double ImpuestosInternos,
									 bool EnNegativo,
									 TiposDeDescuento Tipo) throw (Excepcion)
{
	// printf ("Comando DevolucionDescuento ejecutado en ImpresorFiscal\n");

	unsigned PrintItemTextSize = (Estacion == ImpresorFiscal::ESTACION_TICKET)
		? PrintItemTextTicketSize : PrintItemTextSlipSize;

	assert(PrintItemTextSize != 0);

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_RETURN_RECHARGE) + FS +
								Cadena (Texto, PrintItemTextSize) + FS +
								Numero (Monto, 2) + FS +
								Numero (IVA, 2) + FS +
								(EnNegativo ? "m" : "M") + FS +
								IndicadorII (ImpuestosInternos, Monto, IVA) + FS +
								Caracter (ModoDisplay) + FS +
								Caracter (ModalidadPrecio) + FS +
								Caracter (Tipo));
}


//
// Subtotal
//
void
ImpresorFiscal::Subtotal (bool Imprime, RTA_Subtotal *R) throw (Excepcion)
{
	// printf ("Comando Subtotal ejecutado en ImpresorFiscal\n");
	
	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_SUBTOTAL) + FS +
						(Imprime ? "P" : "N") + FS +
						" " + FS + Caracter (ModoDisplay));

	// Completamos los par�metros correspondientes a los campos
	// de respuesta comunes a todos los modelos con los valores
	// obtenidos de la respuesta del comando y el resto con sus
	// valores default (luego, seg�n sea el caso, deber�n ser
	// actualizados en al implementaci�n de cada modelo en
	// particular)
	if (R != NULL)
	{
		R->CantidadItemsVendidos		= RespuestaDouble (2, "Subtotal");
		R->MontoVentas					= RespuestaDouble (3, "Subtotal");
		R->MontoIVA						= RespuestaDouble (4, "Subtotal");
		R->MontoPagado					= RespuestaDouble (5, "Subtotal");
		R->MontoIVANoInscripto			= 0.0;
		R->MontoImpuestosInternos		= 0.0;
	}
}


//
// Imprimir Pago
//
double
ImpresorFiscal::ImprimirPago (const std::string &Texto, double Monto,
							  const std::string &DescripcionAdicional) throw (Excepcion)
{
	// printf ("Comando ImprimirPago ejecutado en ImpresorFiscal\n");
	
	assert(TotalTenderTextSize != 0);

	// La descripci�n adicional solo se utiliza en aquellos modelos
	// que la soportan. En la implementaci�n general se descarta.

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_TOTAL_TENDER) + FS +
						Cadena (Texto, TotalTenderTextSize) + FS +
						Numero (Monto, 2) + FS + "T" + FS +
						Caracter (ModoDisplay));

	// Retornamos el Vuelto o Monto por Pagar
	return RespuestaDouble (2, "Imprimir Pago");
}


//
// Devolver un pago previamente almacenado
//
// Nota: esta m�todo solo es v�lido para el modelo de CF
//		 PR5 versi�n 2.01.
//
double
ImpresorFiscal::DevolverPago (const std::string &Texto, double Monto,
							  const std::string &DescripcionAdicional) throw (Excepcion)
{
	// printf ("Comando DevolverPago ejecutado en ImpresorFiscal\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Devolver un pago previamente almacenado");
}


// ############################################################ //
//																//
//			M E T O D O S   D E    D N F						//
//																//
// ############################################################ //

//
// Abrir DNF
// Env�a el comando de Apertura de DNF en la estaci�n indicada
//
void
ImpresorFiscal::AbrirDNF (TiposDeEstacion EstacionDeImpresion) throw (Excepcion)
{
	// printf ("Comando AbrirDNF ejecutado en ImpresorFiscal\n");

	Estacion = EstacionDeImpresion;

	// Enviamos el comando fiscal y evaluamos los status
	if (EstacionDeImpresion == ESTACION_TICKET)
		EnviarComandoFiscal(OpCode(CMD_OPEN_NON_FISCAL_RECEIPT));
	else
		EnviarComandoFiscal(OpCode(CMD_OPEN_NON_FISCAL_SLIP));

	// Reinicializamos 'NumberOfPages' en 0 y
	// 'TicketBCAbierto' en false.
	NumberOfPages	= 0;
	TicketBCAbierto	= false;
}


//
// Imprimir Texto No Fiscal
// Env�a el comando de Impresi�n de texto no fiscal en la estaci�n
// indicada en el comando de apertura (variable 'Estacion')
//
void
ImpresorFiscal::ImprimirTextoNoFiscal (const std::string &Texto) throw (Excepcion)
{
	// printf ("Comando ImprimirTextoNoFiscal ejecutado en ImpresorFiscal\n");

	unsigned Size = (Estacion == ImpresorFiscal::ESTACION_TICKET)
		? PrintNonFiscalTextTicketSize : PrintNonFiscalTextSlipSize;

	assert(Size != 0);

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_PRINT_NON_FISCAL_TEXT) + FS +
						Cadena (Texto, Size) + FS +
						Caracter(ModoDisplay));
}


// ############################################################ //
//																//
//			M E T O D O S   D E    D N F H						//
//																//
// ############################################################ //

//
// Identificador de DNFH
//
// Retorna el identificador real de un DNFH (es el que reconoce el CF)
// a partir de un dado Tipo de 'DocumentosNoFiscalesHomologados' ...
//

ImpresorFiscal::IdentificadoresDeDNFH
ImpresorFiscal::IdentificadorDeDNFH (DocumentosNoFiscalesHomologados Tipo)
{
	// Utilizamos un 'map' est�tico en lugar de un array de
	// estructuras solo para agilizar las b�squedas (en map
	// las b�squedas son binarias).

	static std::map<DocumentosNoFiscalesHomologados /* Tipo */, IdentificadoresDeDNFH /* Id */> IdentificadorDNFH;

	// La primera vez que se invoca al m�todo se carga el mapa.
	if (IdentificadorDNFH.size() == 0)
	{
		IdentificadorDNFH[TICKET_NOTA_CREDITO_A]			= DNFH_NOTA_CREDITO_A;
		IdentificadorDNFH[TICKET_NOTA_CREDITO_B]			= DNFH_NOTA_CREDITO_B;
		IdentificadorDNFH[NOTA_CREDITO_A]					= DNFH_NOTA_CREDITO_A;
		IdentificadorDNFH[NOTA_CREDITO_B]					= DNFH_NOTA_CREDITO_B;
		IdentificadorDNFH[REMITO]							= DNFH_REMITO;
		IdentificadorDNFH[TICKET_RECIBO_X]					= DNFH_RECIBO_X;
		IdentificadorDNFH[RECIBO_X]							= DNFH_RECIBO_X;
		IdentificadorDNFH[ORDEN_SALIDA]						= DNFH_ORDEN_SALIDA;
		IdentificadorDNFH[RESUMEN_CUENTA]					= DNFH_RESUMEN_CUENTA;
		IdentificadorDNFH[CARGO_HABITACION]					= DNFH_CARGO_HABITACION;
		IdentificadorDNFH[COTIZACION]						= DNFH_COTIZACION;
		IdentificadorDNFH[CLAUSULA_CREDITO]					= DNFH_CLAUSULA_CREDITO;
		IdentificadorDNFH[CLAUSULA_SEGURO]					= DNFH_CLAUSULA_SEGURO;
		IdentificadorDNFH[TICKET_PAGARE]					= DNFH_PAGARE;
		IdentificadorDNFH[PAGARE]							= DNFH_PAGARE;
		IdentificadorDNFH[POLIZA_SEGURO]					= DNFH_POLIZA_SEGURO;
		IdentificadorDNFH[RECORDATORIO]						= DNFH_RECORDATORIO;
		IdentificadorDNFH[SOLICITUD_CREDITO]				= DNFH_SOLICITUD_CREDITO;
		IdentificadorDNFH[COMUNICACION_CLIENTE]				= DNFH_COMUNICACION_CLIENTE;
		IdentificadorDNFH[OFRECIMIENTO_CREDITO]				= DNFH_OFRECIMIENTO_CREDITO;
		IdentificadorDNFH[OFRECIMIENTO_TARJETA]				= DNFH_OFRECIMIENTO_TARJETA;
		IdentificadorDNFH[MINUTA_CREDITO]					= DNFH_MINUTA_CREDITO;
		IdentificadorDNFH[OFRECIMIENTO_PASAPORTE]			= DNFH_OFRECIMIENTO_PASAPORTE;
		IdentificadorDNFH[RENOVACION_CREDITO]				= DNFH_RENOVACION_CREDITO;
		IdentificadorDNFH[ADELANTO_REMUNERACION]			= DNFH_ADELANTO_REMUNERACION;
		IdentificadorDNFH[SOLICITUD_TARJETA_DEBITO]			= DNFH_SOLICITUD_TARJETA_DEBITO;
		IdentificadorDNFH[SOLICITUD_CLAVE_TARJETA]			= DNFH_SOLICITUD_CLAVE_TARJETA;
		IdentificadorDNFH[RESCATE_MERCADERIA]				= DNFH_RESCATE_MERCADERIA;
		IdentificadorDNFH[INGRESO_EGRESO_SUCURSAL]			= DNFH_INGRESO_EGRESO_SUCURSAL;
		IdentificadorDNFH[TICKET_PRESUPUESTO]				= DNFH_PRESUPUESTO;
		IdentificadorDNFH[TICKET_ENTREGA]					= DNFH_ENTREGA;
		IdentificadorDNFH[TICKET_ESTACIONAMIENTO]			= DNFH_ESTACIONAMIENTO;
		IdentificadorDNFH[TICKET_COBRO_SERVICIO]			= DNFH_COBRO_SERVICIO;
		IdentificadorDNFH[TICKET_INGRESO_DINERO]			= DNFH_INGRESO_DINERO;
		IdentificadorDNFH[TICKET_RETIRO_DINERO]				= DNFH_RETIRO_DINERO;
		IdentificadorDNFH[TICKET_TALON_CAMBIO]				= DNFH_TALON_CAMBIO;
		IdentificadorDNFH[TICKET_TALON_REPARTO]				= DNFH_TALON_REPARTO;
		IdentificadorDNFH[TICKET_TALON_REGALO]				= DNFH_TALON_REGALO;
		IdentificadorDNFH[TICKET_TALON_CUENTA_CORRIENTE]	= DNFH_TALON_CUENTA_CORRIENTE;
		IdentificadorDNFH[TICKET_AVISO_OPERACION_CREDITO]	= DNFH_AVISO_OPERACION_CREDITO;
		IdentificadorDNFH[TICKET_CUPON_PROMOCION]			= DNFH_CUPON_PROMOCION;
		IdentificadorDNFH[TICKET_USO_INTERNO_FARMACIA]		= DNFH_USO_INTERNO_FARMACIA;
	}

	IdentificadoresDeDNFH Id = DNFH_NO_EXISTENTE;

	// Buscamos el estado del CF en el map.
	std::map<DocumentosNoFiscalesHomologados, IdentificadoresDeDNFH>::const_iterator i =
			IdentificadorDNFH.find (Tipo);

	if (i != IdentificadorDNFH.end ())
		Id = i->second;

	return Id;
}


// Imprimir Item en Remito u Orden de Salida
//
// Nota: esta m�todo solo es v�lido para modelos de CF
//		 de 16 bits.
//
void
ImpresorFiscal::ImprimirItemEnRemito (const std::string & /* Texto */,
									  double /* Cantidad */) throw (Excepcion)
{
	// printf ("Comando ImprimirItemEnRemito ejecutado en ImpresorFiscal\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Imprimir Item en Remito u Orden de Salida");
}


//
// Imprimir Item en Resumen de Cuenta o Cargo a la Habitaci�n
//
// Nota: esta m�todo solo es v�lido para modelos de CF
//		 de 16 bits.
//
void
ImpresorFiscal::ImprimirItemEnCuenta (const FECHA & /*FechaComprobante */,
							const std::string & /* NroComprobante */,
							const std::string & /* Texto */,
							double /* Debe */, double /* Haber */) throw (Excepcion)
{
	// printf ("Comando ImprimirItemEnCuenta ejecutado en ImpresorFiscal\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Imprimir Item en Resumen de Cuenta o Cargo a la Habitaci�n");
}


//
// Imprimir Item en Cotizaci�n
//
// Nota: esta m�todo solo es v�lido para modelos de CF
//		 de 16 bits.
//
void
ImpresorFiscal::ImprimirItemEnCotizacion (const std::string & /* Detalle */) throw (Excepcion)
{
	// printf ("Comando ImprimirItemEnCuenta ejecutado en ImpresorFiscal\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Imprimir Item en Cotizaci�n");
}


//
// Detalle Recibo
//
// Nota: esta m�todo solo es v�lido para modelos de CF
//		 de 16 bits.
//
void
ImpresorFiscal::DetalleRecibo (const std::string & /* Texto */) throw (Excepcion)
{
	// printf ("Comando DetalleRecibo ejecutado en ImpresorFiscal\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Detalle Recibo");
}


//
// Imprimir Informaci�n en DNFH
//
// Nota: esta m�todo es v�lido para todos los modelos de CF de
//		 16 bits excepto 320, PJ20, 321 y PL8 (version 1.00).
//
void
ImpresorFiscal::ImprimirInfoEnDNFH (const unsigned /* IDCampo */,
									const std::string & /* Texto */,
									double /* Cantidad */) throw (Excepcion)
{
	// printf ("Comando ImprimirInfoEnDNFH ejecutado en ImpresorFiscal\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Imprimir Informaci�n en DNFH");
}


//
// Imprimir Firma en Cierre de DNFH
//
// Nota: este m�todo condiciona la impresi�n de la Firma solo en algunos
//		 de los nuevos DNFH emitidos en estaci�n Ticket ...
//
void
ImpresorFiscal::ImprimirFirmaEnCierreDNFH (bool ImprimeFirma) throw (Excepcion)
{
	// printf ("Comando ImprimirFirmaEnCierreDNFH ejecutado en ImpresorFiscal\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Imprimir Firma en Cierre de DNFH");
}


//
// Imprimir Voucher de Tarjeta
//
void
ImpresorFiscal::ImprimirVoucher (const std::string &NombreCliente,
							const std::string &NombreTarjeta,
							TiposDeVouchers TipoVoucher,
							const std::string &NumeroDeTarjeta,
							FECHA FechaDeVencimiento,
							TiposDeTarjetas TipoTarjeta,
							unsigned Cuotas,
							const std::string &CodigoDeComercio,
							unsigned NumeroDeTerminal,
							unsigned NumeroDeLote,
							unsigned NumeroCupon,
							TiposDeIngresoDeTarjeta TipoIngreso,
							TiposOperacionDeTarjeta TipoOperacion,
							unsigned NumeroAutorizacion,
							const std::string &Monto,
							unsigned NumeroComprobanteAsociado,
							unsigned Copias,
							const std::string &Vendedor,
							TiposDeEstacion Estacion) throw (Excepcion)
{
	// Ejecutamos el m�todo que soporta NumeroComprobanteAsociado
	// como string en ambos casos.
	ImprimirVoucher (	NombreCliente,
						NombreTarjeta, TipoVoucher, NumeroDeTarjeta, FechaDeVencimiento, TipoTarjeta,
						Cuotas, CodigoDeComercio, NumeroDeTerminal, NumeroDeLote, NumeroCupon,
						TipoIngreso, TipoOperacion, NumeroAutorizacion, Monto,
						Numero (NumeroComprobanteAsociado), Copias, Vendedor, Estacion);
}


//
// Imprimir Voucher de Tarjeta
//
// Nota: esta m�todo -en donde la fecha de vencimiento pasa a ser un string,
//		 lo cual permite ocultar dicha fecha- solo es v�lido para los modelos
//		 de CF de 16 bits P435 (version 2.03) y posteriores, y para los modelos
//		 de CF de  8 bits PPR5 (versi�n 3.02) y posteriores.
//
void
ImpresorFiscal::ImprimirVoucher (const std::string &NombreCliente,
									const std::string &NombreTarjeta,
									TiposDeVouchers TipoVoucher,
									const std::string &NumeroDeTarjeta,
									const std::string &FechaDeVencimiento,
									TiposDeTarjetas TipoTarjeta,
									unsigned Cuotas,
									const std::string &CodigoDeComercio,
									unsigned NumeroDeTerminal,
									unsigned NumeroDeLote,
									unsigned NumeroCupon,
									TiposDeIngresoDeTarjeta TipoIngreso,
									TiposOperacionDeTarjeta TipoOperacion,
									unsigned NumeroAutorizacion,
									const std::string &Monto,
									const std::string &NumeroComprobanteAsociado,
									unsigned Copias,
									const std::string &Vendedor,
									TiposDeEstacion Estacion) throw (Excepcion)
{
	// printf ("Comando ImprimirVoucher ejecutado en ImpresorFiscal\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Imprimir Voucher de Tarjeta");
}


//
// Imprimir Voucher de Tarjeta
//
// Nota: esta m�todo -en donde la fecha de vencimiento pasa a ser un string,
//		 lo cual permite ocultar dicha fecha- solo es v�lido para los modelos
//		 de CF de 16 bits P435 (version 2.03) y posteriores, y para los modelos
//		 de CF de  8 bits PPR5 (versi�n 3.02) y posteriores.
//
void
ImpresorFiscal::ImprimirVoucher (const std::string &NombreCliente,
							const std::string &NombreTarjeta,
							TiposDeVouchers TipoVoucher,
							const std::string &NumeroDeTarjeta,
							const std::string &FechaDeVencimiento,
							TiposDeTarjetas TipoTarjeta,
							unsigned Cuotas,
							const std::string &CodigoDeComercio,
							unsigned NumeroDeTerminal,
							unsigned NumeroDeLote,
							unsigned NumeroCupon,
							TiposDeIngresoDeTarjeta TipoIngreso,
							TiposOperacionDeTarjeta TipoOperacion,
							unsigned NumeroAutorizacion,
							const std::string &Monto,
							unsigned NumeroComprobanteAsociado,
							unsigned Copias,
							const std::string &Vendedor,
							TiposDeEstacion Estacion) throw (Excepcion)
{
	// Ejecutamos el m�todo que soporta NumeroComprobanteAsociado
	// como string en ambos casos.
	ImprimirVoucher (	NombreCliente,
						NombreTarjeta, TipoVoucher, NumeroDeTarjeta, FechaDeVencimiento, TipoTarjeta,
						Cuotas, CodigoDeComercio, NumeroDeTerminal, NumeroDeLote, NumeroCupon,
						TipoIngreso, TipoOperacion, NumeroAutorizacion, Monto,
						Numero (NumeroComprobanteAsociado), Copias, Vendedor, Estacion);
}


