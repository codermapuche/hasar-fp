#include "ImpresorFiscal16Bits.h"

#ifdef P_WIN32
#include <io.h>
#endif

// ############################################################ //
//																//
//			F U N C I O N E S      L O C A L E S				//
//																//
// ############################################################ //

//
// CharToHexa
// Devuelve el valor hexa en ascii de c.
//
char
CharToHexa (char c)
{
	return (c > 9) ? char(c-10+'A') : char(c+'0');
}

//
// CodifData
// Toma cada byte de input, lo parte en dos nibles y
// los convierte en su valor hexadecimal en ascii.
//
void
CodifData (char *input, int size, char *output)
{
	char *inputPtr;

	for ( inputPtr = input; inputPtr < input+size; inputPtr++ )
	{
		*output++ = CharToHexa (char(*inputPtr >> 4) & 0x0f);
		*output++ = CharToHexa (char(*inputPtr     ) & 0x0f);
	}
}


// ############################################################ //
//																//
//			M E T O D O S      L O C A L E S					//
//																//
// ############################################################ //

//
// Constructor
//
ImpresorFiscal16Bits::ImpresorFiscal16Bits () :
										ImpresorFiscal(PROTOCOLO_STATPRN)
{
	// Inicialización de variables de uso general
	MaxFiscalTextLinesInvoice		= 4;
	MaxFiscalTextLinesTicket		= 4;

	// Inicialización de variables de tamaño de campos
	PrintNonFiscalTextSlipSize		= 120;
	FantasyNameSize					= 50;
	HeaderTrailerSize				= 120;
	HeaderTrailerSlipSize			= 120;
	CodeIngBrSize					= 30;
	RazonSocialSize					= 50;
	DireccionSize					= 50;
	PrintFiscalTextSlipSize			= 50;
	PrintItemTextSlipSize			= 50;
	PriceDecimals					= 4;
	TotalTenderTextSize				= 80;
	RemitoTextSize					= 108;
	ResumeTextSize					= 55;
	CotizaTextSize					= 120;
	ReciboTextSlipSize				= 106;
	ReciboLines						= 9;
	PrintDNFHTextSize				= 120;
	VouchCustNameSize				= 50;
	VouchCardNameSize				= 50;
	VouchImportSize					= 15;
	VouchCompNumSize				= 20;
	VouchVendNameSize				= 20;
}


//
// Cambiar Velocidad del Controlador Fiscal
//
void
ImpresorFiscal16Bits::CambiarVelocidad(unsigned Velocidad) throw (Excepcion)
{
	// printf ("Comando CambiarVelocidad ejecutado en ImpresorFiscal16Bits\n");

	// Verificamos si se trata de una velocidad válida.
	if (Velocidad !=  1200 && Velocidad !=  2400 &&
		Velocidad !=  4800 && Velocidad !=  9600 &&
		Velocidad != 19200 && Velocidad != 38400 &&
		Velocidad != 57600 && Velocidad != 115200)
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_PARAMETRO_NO_VALIDO, "Cambiar Velocidad del Controlador Fiscal");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_CHANGE_COM_SPEED) + FS +
						Numero (Velocidad));
}


//
// Indicador de Impuestos Internos
// Calcula el valor del indicador de Impuestos Internos de
// una venta en base a los parámetros 'Precio', 'IVA' e
// 'ImpuestosInternos' y a las preopiedades 'KIVA', 'IIFijo',
// 'IIFijoPorMonto' y 'PrecioBase'.
//
std::string
ImpresorFiscal16Bits::IndicadorII (double ImpuestosInternos, double Precio, double IVA) throw(Excepcion)
{
	switch (ModalidadII)
	{
	case II_FIJO_KIVA:
		return (Caracter (II_FIJO_KIVA) + Numero (ImpuestosInternos, 8));
	case II_FIJO_MONTO:
		return (Caracter (II_FIJO_MONTO) + Numero (ImpuestosInternos, 8));
	case II_VARIABLE_KIVA:
		return (Numero (ImpuestosInternos, 8));
	case II_VARIABLE_PORCENTUAL:
	default:
		return (Caracter (II_VARIABLE_PORCENTUAL) + Numero (ImpuestosInternos, 8));
	}
}


// ############################################################ //
//																//
//			M E T O D O S   G E N E R A L E S					//
//																//
// ############################################################ //

//
// Obtener Versión del Controlador Fiscal
//
std::string
ImpresorFiscal16Bits::ObtenerVersionDelControlador () throw (Excepcion)
{
	// printf ("Comando ObtenerVersionDelControlador ejecutado en ImpresorFiscal16Bits\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_GET_VERSION));

	// Retornamos la información obtenida de la respuesta
	return RespuestaString(2, "Obtener Versión del Controlador Fiscal");
}


//
// Obtener Datos de Inicialización
//
void
ImpresorFiscal16Bits::ObtenerDatosDeInicializacion (RTA_ObtenerDatosDeInicializacion *R) throw (Excepcion)
{
	// printf ("Comando ObtenerDatosDeInicializacion ejecutado en ImpresorFiscal16Bits\n");

	// Ejecutamos el método de la superclase 'ImpresorFiscal'
	ImpresorFiscal::ObtenerDatosDeInicializacion (R);

	// Completamos los parámetros correspondientes a los campos de
	// respuesta particulares para todos los modelos de 16 Bits.
	if (R != NULL)
	{
		R->FechaIniActividades	= FECHA (RespuestaString (7, "Obtener Datos de Inicialización"));
		R->CodIngBrutos			= RespuestaString (8, "Obtener Datos de Inicialización");
	}
}


//
// Obtener Datos de Memoria de Trabajo
//

void
ImpresorFiscal16Bits::ObtenerDatosMemoriaDeTrabajo (RTA_ObtenerDatosMemoriaDeTrabajo *R) throw (Excepcion)
{
	// printf ("Comando ObtenerDatosMemoriaDeTrabajo ejecutado en ImpresorFiscal16Bits\n");

	// Ejecutamos el método de la superclase 'ImpresorFiscal'
	ImpresorFiscal::ObtenerDatosMemoriaDeTrabajo (R);

	// Completamos los parámetros correspondientes a los campos
	// de respuesta comunes que se agregaron para todos los
	// modelos de 16 Bits.
	if (R != NULL)
	{
		R->MontoPercepcionesDocFiscal		= RespuestaDouble (10, "Obtener Datos de Memoria de Trabajo");
		R->MontoIVANoInscriptoDocFiscal		= RespuestaDouble (11, "Obtener Datos de Memoria de Trabajo");
		R->UltimaNotaCreditoBC				= RespuestaInt (12, "Obtener Datos de Memoria de Trabajo");
		R->UltimaNotaCreditoA				= RespuestaInt (13, "Obtener Datos de Memoria de Trabajo");
		R->MontoVentasNotaCredito			= RespuestaDouble (14, "Obtener Datos de Memoria de Trabajo");
		R->MontoIVANotaCredito				= RespuestaDouble (15, "Obtener Datos de Memoria de Trabajo");
		R->MontoImpInternosNotaCredito		= RespuestaDouble (16, "Obtener Datos de Memoria de Trabajo");
		R->MontoPercepcionesNotaCredito		= RespuestaDouble (17, "Obtener Datos de Memoria de Trabajo");
		R->MontoIVANoInscriptoNotaCredito	= RespuestaDouble (18, "Obtener Datos de Memoria de Trabajo");
		R->UltimoRemito						= RespuestaInt (19, "Obtener Datos de Memoria de Trabajo");
	}
}


//
// Baja Fiscal
//
// Nota: esta implementación es válida para todos los modelos
//		 de 16 bits posteriores al 321 inclusive, para los
//		 cuales se incorporó obligatoriamente la carga del
//		 paámetro que identifica al del número de serie del
//		 CF. Por lo tanto, los restantes modelos de 16 bits
//		 tendrán que tener sus propias implementaciones (320,
//		 PJ20 y PL8 versión 1.00).
//
void
ImpresorFiscal16Bits::BajaFiscal (const std::string &NumeroDeSerie) throw(Excepcion)
{
	// printf ("Comando BajaFiscal ejecutado en ImpresorFiscal16Bits\n");

	// Verificamos si el Nro de Serie fue pasado.
	if ( NumeroDeSerie.length () == 0 )
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_PARAMETRO_OBLIGATORIO, "Baja Fiscal");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_KILL_EPROM) + FS +
						Cadena (NumeroDeSerie, SerialNumberSize));
}


//
// Cambiar Código de Ingresos Brutos
//
// Nota: este método solo no es válido para el modelo P320.
//
void
ImpresorFiscal16Bits::CambiarCodigoIngresosBrutos (const std::string &CodigoIngBr) throw(Excepcion)
{
	// printf ("Comando CambiarCodigoIngresosBrutos ejecutado en ImpresorFiscal16Bits\n");

	assert(CodeIngBrSize != 0);

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_CHG_ING_BR) + FS +
						Cadena (CodigoIngBr, CodeIngBrSize, 0));
}


//
// Cambiar Fecha de Inicio de Actividades
//
// Nota: este método no es válido para los modelos 320, PJ20,
//		 PL8, 321, 322 y 425.
//
void
ImpresorFiscal16Bits::CambiarFechaInicioActividades (const FECHA &Fecha) throw(Excepcion)
{
	// printf ("Comando CambiarFechaInicioActividades ejecutado en ImpresorFiscal16Bits\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_CHG_INI_ACT_DATE) + FS + Fecha.ToString());
}


//
// Estado del Controlador Fiscal
//
ImpresorFiscal::TiposDeEstadoControlador
ImpresorFiscal16Bits::EstadoControlador () throw(Excepcion)
{
	// printf ("Comando EstadoControlador ejecutado en ImpresorFiscal16Bits\n");

	return EstadoCF ();
}


//
// Descripción del Estado del Controlador Fiscal
//
std::string
ImpresorFiscal16Bits::DescripcionEstadoControlador () throw(Excepcion)
{
	// printf ("Comando DescripcionEstadoControlador ejecutado en ImpresorFiscal16Bits\n");

	return DescripcionEstadoCF ();
}


//
// Documento en Curso
//
ImpresorFiscal::TiposDeDocumento
ImpresorFiscal16Bits::DocumentoEnCurso () throw(Excepcion)
{
	// printf ("Comando DocumentoEnCurso ejecutado en ImpresorFiscal16Bits\n");

	return DocumentoCF ();
}


//
// Descripción del Documento en Curso
//
std::string
ImpresorFiscal16Bits::DescripcionDocumentoEnCurso () throw(Excepcion)
{
	// printf ("Comando DescripcionDocumentoEnCurso ejecutado en ImpresorFiscal16Bits\n");

	return DescripcionDocumentoCF ();
}


//
// Ultimo Documento fue Cancelado
//
bool
ImpresorFiscal16Bits::UltimoDocumentoFueCancelado () throw(Excepcion)
{
	// printf ("Comando UltimoDocumentoCancelado ejecutado en ImpresorFiscal16Bits\n");

	return IndicadorDeDocumentoPrevioCancelado ();
}


//
// Ultimo Documento Fiscal Tipo A
//
unsigned
ImpresorFiscal16Bits::UltimoDocumentoFiscalA () throw(Excepcion)
{
	// printf ("Comando UltimoDocumentoFiscalA ejecutado en ImpresorFiscal16Bits\n");

	// Enviamos el comando fiscal
	SolicitarEstado ();

	// Retornamos la información obtenida de la respuesta
	return RespuestaInt(4, "Ultimo Documento Fiscal Tipo A");
}


//
// Ultima Nota de Crédito Tipo BC
//
unsigned
ImpresorFiscal16Bits::UltimaNotaCreditoBC () throw(Excepcion)
{
	// printf ("Comando UltimaNotaCreditoBC ejecutado en ImpresorFiscal16Bits\n");

	// Enviamos el comando fiscal
	SolicitarEstado ();

	// Retornamos la información obtenida de la respuesta
	return RespuestaInt(6, "Ultima Nota de Crédito Tipo BC");
}


//
// Ultima Nota de Crédito Tipo A
//
unsigned
ImpresorFiscal16Bits::UltimaNotaCreditoA () throw(Excepcion)
{
	// printf ("Comando UltimaNotaCreditoA ejecutado en ImpresorFiscal16Bits\n");

	// Enviamos el comando fiscal
	SolicitarEstado ();

	// Retornamos la información obtenida de la respuesta
	return RespuestaInt(7, "Ultima Nota de Crédito Tipo A");
}


//
// Ultimo Remito
//
unsigned
ImpresorFiscal16Bits::UltimoRemito () throw(Excepcion)
{
	// printf ("Comando UltimoRemito ejecutado en ImpresorFiscal16Bits\n");

	// Enviamos el comando fiscal
	SolicitarEstado ();

	// Retornamos la información obtenida de la respuesta
	return RespuestaInt(8, "Ultimo Remito");
}


//
// Primer Número de Documento Actual
//
unsigned
ImpresorFiscal16Bits::PrimerNumeroDeDocumentoActual () throw(Excepcion)
{
	// printf ("Comando PrimerNumeroDeDocumentoActual ejecutado en ImpresorFiscal16Bits\n");

	// Si FirstDocNum es 0 indica que estamos fuera de todo
	// documento, por lo que su contenido carece de validez
	if (FirstDocNum == 0)
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_ESTADO_NO_VALIDO, "Primer Número de Documento Actual");

	return FirstDocNum;
}


//
// Páginas de Ultimo Documento
//
// Nota: este método solo es válido para aquellos modelos de 16
//		 bits que indican en los comandos de cierre de DF y DNFH
//		 el número de páginas del documento recién emitido, o sea
//		 425 v2.01, PL8 v2.01, 322 v2.01, 330, 435, 330 v2.01 y
//		 PL9.
//
unsigned
ImpresorFiscal16Bits::PaginasDeUltimoDocumento () throw(Excepcion)
{
	// printf ("Comando PaginasDeUltimoDocumento ejecutado en ImpresorFiscal16Bits\n");

	// Si NumberOfPages es 0 indica que no estamos en condiciones
	// de informar el Número de Paginas del Documento Recién
	// emitido.
	if (NumberOfPages == 0)
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_ESTADO_NO_VALIDO, "Páginas de Ultimo Documento");

	return NumberOfPages;
}


//
// CAI del Ultimo Documento A
//
// Nota: este método solo es válido para aquellos modelos
//		 de 16 bits que soportan este comando, o sea
//		 425 v2.01, PL8 v2.01, 322 v2.01, 330, 435, 330 v2.01
//		 y PL9.
//
void
ImpresorFiscal16Bits::CAIUltimoDocumentoA (RTA_CAIUltimoDocumentoA *R) throw (Excepcion)
{
	// printf ("Comando CAIUltimoDocumentoA ejecutado en ImpresorFiscal16Bits\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_GET_CAI_LAST_DOC_A));

	// Retornamos la información obtenida de la respuesta
	if (R != NULL)
	{
		R->Documento	= static_cast<TiposDeDocumento>(xtoi (RespuestaString(2, "CAI del Ultimo Documento A").c_str(), 2));
		R->PrimerNumero	= RespuestaInt (3, "CAI del Ultimo Documento A");
		R->UltimoNumero	= RespuestaInt (4, "CAI del Ultimo Documento A");
		R->CAI			= RespuestaString(5, "CAI del Ultimo Documento A");
	}
}


//
// Abrir Cajón de Dinero
//
// Nota: este método solo es válido para los modelos de 16 Bits
//		 425, 425 v2.01 y 435.
//
void
ImpresorFiscal16Bits::AbrirCajon () throw(Excepcion)
{
	// printf ("Comando AbrirCajon ejecutado en ImpresorFiscal16Bits\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Abrir Cajón de Dinero");
}


//
// Avanzar papel en estación especificada
//
// Nota: este método solo es válido para los modelos de 16 Bits
//		 425, 425 v2.01 y 435.
//
void
ImpresorFiscal16Bits::AvanzarPapel (TiposDePapel /* Papel */,
									unsigned /* Lineas */) throw (Excepcion)
{
	// printf ("Comando AvanzarPapel ejecutado en ImpresorFiscal16Bits\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Avance de papel");
}


//
// Cargar Datos del Cliente
//
void
ImpresorFiscal16Bits::DatosCliente (	const std::string &RazonSocial,
								const std::string &NroDocumento,
								TiposDeDocumentoCliente TipoDocumento,
								TiposDeResponsabilidadesCliente ResponsabilidadIVA,
								const std::string &Direccion) throw(Excepcion)
{
	// printf ("Comando DatosCliente ejecutado en ImpresorFiscal16Bits\n");

	assert(RazonSocialSize != 0);
	assert(DireccionSize != 0);

	// Verificamos la existencia del parámetro dirección
	// (es obligatorio para todos los modelos de 16 bits)
	if (Direccion.length() == 0)
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_PARAMETRO_OPCIONAL, "Cargar Datos del Cliente");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_SET_CUSTOMER_DATA) + FS +
						Cadena (RazonSocial, RazonSocialSize) + FS +
						Cadena (NroDocumento, 11) + FS +
						Caracter (ResponsabilidadIVA) + FS +
						Caracter (TipoDocumento) + FS +
						Cadena (Direccion, DireccionSize));
}


//
// Cancelar Comprobante Fiscal
//
void
ImpresorFiscal16Bits::CancelarComprobanteFiscal () throw(Excepcion)
{
	// printf ("Comando CancelarComprobanteFiscal ejecutado en ImpresorFiscal16Bits\n");

	// En los modelos de 16 bits el comando de Cancelación
	// utilizado para todos los documentos es el mismo
	ImpresorFiscal16Bits::CancelarComprobante ();
}


//
// Cancelar Comprobante
//
void
ImpresorFiscal16Bits::CancelarComprobante () throw(Excepcion)
{
	// printf ("Comando CancelarComprobante ejecutado en ImpresorFiscal16Bits\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_CANCEL));

	// Reinicializamos 'FirstDocNum' y 'NumberOfPages' en 0 y
	// 'TicketBCAbierto' en false.
	FirstDocNum		= 0;
	NumberOfPages	= 0;
	TicketBCAbierto	= false;
}


//
// Reimprimir Comprobante
//
void
ImpresorFiscal16Bits::ReimprimirComprobante () throw(Excepcion)
{
	// printf ("Comando ReimprimirComprobante ejecutado en ImpresorFiscal16Bits\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_REPRINT));
}


//
// Cargar Logo
//
void
ImpresorFiscal16Bits::CargarLogo (const std::string &ArchivoBMP) throw(Excepcion)
{
	// printf ("Comando CargarLogo ejecutado en ImpresorFiscal16Bits\n");

	// Verificamos si debemos ejecutar el borrado del logo.
	if (ArchivoBMP.length() == 0)
	{
		// Enviamos el comando fiscal y evaluamos los status
		EnviarComandoFiscal(OpCode(CMD_RESET_LOGO));

		return;
	}

	static char InputBuff[64];
	static char OutputBuff[128];

	unsigned 		Qty;
	unsigned long	QtyAcum;
	unsigned		Size;
	FILE * 			fpLogo;

	const	char FIRST_LINE = 'I';
	const	char ANY_LINE	= 'C';
	const	char END_LINE	= 'F';

	// Abrimos el archivo del logo.
#ifdef P_WIN32_VC_2005
	if (fopen_s(&fpLogo, ArchivoBMP.c_str(), "rb" ))
#else
	if ( (fpLogo = fopen(ArchivoBMP.c_str(), "rb" )) == NULL )
#endif
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NOMBRE_ARCHIVO, "Cargar Logo");

	// Recuperamos el tamaño del archivo del logo.
#ifdef P_WIN32

#ifdef P_WIN32_VC_2005
#define fileno _fileno
#endif

	Size = _filelength (fileno (fpLogo));
#endif
#ifdef P_LINUX
	int fd = fileno(fpLogo);
	Size = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
#endif
	if( (Qty = fread (InputBuff, 1, sizeof InputBuff, fpLogo)) == 0 )
	{
		fclose (fpLogo);
		return;
	}

	// El primer comando se genera con el header
	// de inicio de carga.

	QtyAcum = Qty;
	CodifData (InputBuff, Qty, OutputBuff);

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_LOAD_LOGO) + FS +
						(Size <= sizeof InputBuff ? END_LINE : FIRST_LINE) + FS +
						Cadena (OutputBuff, Qty * 2));

	// A partir de ahora generamos comandos con el header
	// de carga en curso, excepto el último paquete que se
	// genera con el header de fin de carga.

	while( (Qty = fread (InputBuff, 1, sizeof InputBuff, fpLogo)) != 0 )
	{
		QtyAcum += Qty;
		CodifData (InputBuff, Qty, OutputBuff);

		// Enviamos el comando fiscal y evaluamos los status
		EnviarComandoFiscal(OpCode(CMD_LOAD_LOGO) + FS +
							(QtyAcum == Size ? END_LINE : ANY_LINE) + FS +
							Cadena (OutputBuff, Qty * 2));
	}

	// Cerramos el archivo.
	fclose (fpLogo);
}


//
// Información de características soportadas por las
// diferentes familias de controladores fiscales.
//

// Capacidad de impresión de documentos en estación Slip
void
ImpresorFiscal16Bits::CapacidadDocumentoSlip (bool &SoportaFA, bool &SoportaNC)
{
	// printf ("Comando CapacidadDocumentoSlip ejecutado en ImpresorFiscal16Bits\n");

	SoportaFA = true;
	SoportaNC = true;
}


// Capacidad de impresión de documentos en estación Ticket
void
ImpresorFiscal16Bits::CapacidadDocumentoTicket (bool &SoportaFA, bool &SoportaNC)
{
	// printf ("Comando CapacidadDocumentoTicket ejecutado en ImpresorFiscal16Bits\n");

	SoportaFA = false;
	SoportaNC = false;
}


// Capacidad de impresión en estación Slip / Ticket
void
ImpresorFiscal16Bits::CapacidadEstacion (bool &SoportaTicket, bool &SoportaSlip)
{
	// printf ("Comando CapacidadEstacion ejecutado en ImpresorFiscal16Bits\n");

	SoportaTicket = false;
	SoportaSlip = true;
}


// Capacidad de manejo y soporte de Cajón de Dinero
void
ImpresorFiscal16Bits::CapacidadCajon (bool &SoportaCajon)
{
	// printf ("Comando CapacidadCajon ejecutado en ImpresorFiscal16Bits\n");

	SoportaCajon = false;
}


// ############################################################ //
//																//
//			M E T O D O S   D E    C O N F I G U R A C I O N	//
//																//
// ############################################################ //

//
// Obtener Línea de Nombre de Fantasía
//
std::string
ImpresorFiscal16Bits::ObtenerNombreDeFantasia (unsigned Linea) throw (Excepcion)
{
	// printf ("Comando ObtenerNombreDeFantasia ejecutado en ImpresorFiscal16Bits\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_GET_FANTASY_NAME) + FS + Numero (Linea));

	// Retornamos la información obtenida de la respuesta
        return RespuestaString(2, "Obtener Línea de Nombre de Fantasía");
}


//
// Especificar Información Línea de Remito / Comprobante Original
//
void
ImpresorFiscal16Bits::EspecificarInformacionRemitoComprobanteOriginal (unsigned Linea, const std::string &Texto) throw (Excepcion)
{
	// printf ("Comando EspecificarInformacionRemitoComprobanteOriginal ejecutado en ImpresorFiscal16Bits\n");

	assert(InfoRemitoCompOriSize != 0);

	// Enviamos el comando fiscal y evaluamos los status
	if (Texto.length() != 0)
		EnviarComandoFiscal(OpCode(CMD_SET_EMBARK_NUMBER) + FS +
							Numero (Linea) + FS +
							Cadena (Texto, InfoRemitoCompOriSize));
	else
		EnviarComandoFiscal(OpCode(CMD_SET_EMBARK_NUMBER) + FS +
							Numero (Linea) + FS + ERASE_LINE);
}


//
// Obtener Información Línea de Remito / Comprobante Original
//
std::string
ImpresorFiscal16Bits::ObtenerInformacionRemitoComprobanteOriginal (unsigned Linea) throw (Excepcion)
{
	// printf ("Comando ObtenerInformacionRemitoComprobanteOriginal ejecutado en ImpresorFiscal16Bits\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_GET_EMBARK_NUMBER) + FS + Numero (Linea));

	// Retornamos la información obtenida de la respuesta
	return RespuestaString(2, "Obtener Información Línea de Remito / Comprobante Original");
}


//
// Borrar Información Línea de Remito / Comprobante Original
//
void
ImpresorFiscal16Bits::BorrarInformacionRemitoComprobanteOriginal () throw (Excepcion)
{
	// printf ("Comando BorrarInformacionRemitoComprobanteOriginal ejecutado en ImpresorFiscal16Bits\n");

	// Enviamos el comando fiscal y evaluamos los status
	for ( unsigned Linea = First_EmbarkNum_Line; Linea <= Last_EmbarkNum_Line; Linea++ )
		EnviarComandoFiscal(OpCode(CMD_SET_EMBARK_NUMBER) + FS +
							Numero (Linea) + FS + ERASE_LINE);
}


//
// Configurar Controlador Completo
//
void
ImpresorFiscal16Bits::ConfigurarControladorCompleto (
							bool Imprimir,
							bool Defaults,
							const double *LimiteConsumidorFinal,
							const double *LimiteTicketFactura,
							const double *PorcentajeIVANoInscripto,
							const NumerosDeCopias *TipoDeCopiasMaximo,
							const bool *ImprimeCambio,
							const bool *ImprimeLeyendasOpcionales,
							const TiposDeCorteDePapel *TipoDeCorte,
							const bool *ImprimeMarco,
							const bool *ReImprimeDocumentos,
							const std::string *DescripcionDelMedioDePago,
							const bool *Sonido,
							const TiposDeAltoHoja *AltoHoja,
							const TiposDeAnchoHoja *AnchoHoja,
							const TiposDeEstacion *EstacionImpresionReportesXZ,
							const TiposDeModoImpresion *ModoImpresion) throw (Excepcion)
{
	// printf ("Comando ConfigurarControladorCompleto ejecutado en ImpresorFiscal16Bits\n");

	assert(TotalTenderTextSize != 0);

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_SET_CONFIG_FULL) + FS +
						(Imprimir ? "P" : "N") + FS +
						(Defaults ? "P" : "N") + FS +
						(LimiteConsumidorFinal		 ? Numero (*LimiteConsumidorFinal, 2)		: "") + FS +
						(LimiteTicketFactura		 ? Numero (*LimiteTicketFactura, 2)			: "") + FS +
						(PorcentajeIVANoInscripto	 ? Numero (*PorcentajeIVANoInscripto, 2)	: "") + FS +
						(TipoDeCopiasMaximo			 ? Caracter (*TipoDeCopiasMaximo)			: "") + FS +
						(ImprimeCambio				 ? (*ImprimeCambio ? "P" : "N")				: "") + FS +
						(ImprimeLeyendasOpcionales	 ? (*ImprimeLeyendasOpcionales ? "P": "N")	: "") + FS +
						(TipoDeCorte				 ? Caracter (*TipoDeCorte)					: "") + FS +
						(ImprimeMarco				 ? (*ImprimeMarco ? "P" : "N")				: "") + FS +
						(ReImprimeDocumentos		 ? (*ReImprimeDocumentos ? "P" : "N")		: "") + FS +
						(DescripcionDelMedioDePago	 ? Cadena (*DescripcionDelMedioDePago, TotalTenderTextSize) : "") + FS +
						(Sonido						 ? (*Sonido ? "P" : "N")					: "") + FS +
						(AltoHoja					 ? Caracter (*AltoHoja)						: "") + FS +
						(AnchoHoja					 ? Caracter (*AnchoHoja)					: "") + FS +
						(EstacionImpresionReportesXZ ? Caracter (*EstacionImpresionReportesXZ)	: "") + FS +
						(ModoImpresion				 ? Caracter (*ModoImpresion)				: "") );
}


//
// Obtener Configuracion del Controlador
//
void
ImpresorFiscal16Bits::ObtenerConfiguracion (RTA_ObtenerConfiguracion *R) throw (Excepcion)
{
	// printf ("Comando ObtenerConfiguracion ejecutado en ImpresorFiscal16Bits\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_GET_CONFIG_FULL));

	// Completamos los parámetros correspondientes a los campos de
	// respuesta comunes a todos los modelos con los valores obtenidos
	// de la respuesta del comando y el resto con sus valores default
	// (luego, según sea el caso, deberán ser actualizados en la
	// implementación de cada modelo en particular).
	if (R != NULL)
	{
		R->LimiteConsumidorFinal		= RespuestaDouble (2, "Obtener Configuracion del Controlador");
		R->LimiteTicketFactura			= RespuestaDouble (3, "Obtener Configuracion del Controlador");
		R->PorcentajeIVANoInscripto		= RespuestaDouble (4, "Obtener Configuracion del Controlador");
		R->TipoDeCopiasMaximo			= static_cast<ImpresorFiscal::NumerosDeCopias>(RespuestaString (5, "Obtener Configuracion del Controlador") [0]);
		R->ImprimeCambio				= ((RespuestaString (6, "Obtener Configuracion del Controlador") [0]) == 'P');
		R->ImprimeLeyendasOpcionales	= ((RespuestaString (7, "Obtener Configuracion del Controlador") [0]) == 'P');
		R->TipoDeCorte					= static_cast<ImpresorFiscal::TiposDeCorteDePapel>(RespuestaString (8, "Obtener Configuracion del Controlador") [0]);
		R->ImprimeMarco					= ((RespuestaString (9, "Obtener Configuracion del Controlador") [0]) == 'P');
		R->ReImprimeDocumentos			= ((RespuestaString (10, "Obtener Configuracion del Controlador") [0]) == 'P');
		R->DescripcionDelMedioDePago	= RespuestaString (11, "Obtener Configuracion del Controlador");
		R->Sonido						= ((RespuestaString (12, "Obtener Configuracion del Controlador") [0]) == 'P');
		R->AltoHoja						= ALTO_REDUCIDO;
		R->AnchoHoja					= ANCHO_REDUCIDO;
		R->EstacionImpresionReportesXZ	= ESTACION_TICKET;
		R->ModoImpresion				= USO_ESTACION_TICKET;
		R->ChequeoDesbordeCompleto		= true;
		R->ChequeoTapaAbierta			= false;
		R->ImprimeIDCopiaDNFH			= false;
		R->ImprimeColorNegro			= true;
	}
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
ImpresorFiscal16Bits::ReporteZ (RTA_ReporteZX *R) throw (Excepcion)
{
	// printf ("Comando ReporteZ ejecutado en ImpresorFiscal16Bits\n");

	// Ejecutamos el método de la superclase 'ImpresorFiscal'
	ImpresorFiscal::ReporteZ (R);

	// Completamos los parámetros correspondientes a los campos de
	// respuesta particulares que se agregan en este modelo más alla
	// de los comunes a todos los modelos
	if (R != NULL)
	{
		R->MontoImpInternosDocFiscal		= RespuestaDouble (12, "Reporte Z");
		R->MontoPercepcionesDocFiscal		= RespuestaDouble (13, "Reporte Z");
		R->MontoIVANoInscriptoDocFiscal		= RespuestaDouble (14, "Reporte Z");
		R->UltimaNotaCreditoBC				= RespuestaInt (15, "Reporte Z");
		R->UltimaNotaCreditoA				= RespuestaInt (16, "Reporte Z");
		R->MontoVentasNotaCredito			= RespuestaDouble (17, "Reporte Z");
		R->MontoIVANotaCredito				= RespuestaDouble (18, "Reporte Z");
		R->MontoImpInternosNotaCredito		= RespuestaDouble (19, "Reporte Z");
		R->MontoPercepcionesNotaCredito		= RespuestaDouble (20, "Reporte Z");
		R->MontoIVANoInscriptoNotaCredito	= RespuestaDouble (21, "Reporte Z");
		R->UltimoRemito						= RespuestaInt (22, "Reporte Z");
	}
}


//
// Reporte X
//
void
ImpresorFiscal16Bits::ReporteX (RTA_ReporteZX *R) throw (Excepcion)
{
	// printf ("Comando ReporteX ejecutado en ImpresorFiscal16Bits\n");

	// Ejecutamos el método de la superclase 'ImpresorFiscal'
	ImpresorFiscal::ReporteX (R);

	// Completamos los parámetros correspondientes a los campos de
	// respuesta particulares que se agregan en este modelo más alla
	// de los comunes a todos los modelos
	if (R != NULL)
	{
		R->MontoImpInternosDocFiscal		= RespuestaDouble (12, "Reporte X");
		R->MontoPercepcionesDocFiscal		= RespuestaDouble (13, "Reporte X");
		R->MontoIVANoInscriptoDocFiscal		= RespuestaDouble (14, "Reporte X");
		R->UltimaNotaCreditoBC				= RespuestaInt (15, "Reporte X");
		R->UltimaNotaCreditoA				= RespuestaInt (16, "Reporte X");
		R->MontoVentasNotaCredito			= RespuestaDouble (17, "Reporte X");
		R->MontoIVANotaCredito				= RespuestaDouble (18, "Reporte X");
		R->MontoImpInternosNotaCredito		= RespuestaDouble (19, "Reporte X");
		R->MontoPercepcionesNotaCredito		= RespuestaDouble (20, "Reporte X");
		R->MontoIVANoInscriptoNotaCredito	= RespuestaDouble (21, "Reporte X");
		R->UltimoRemito						= RespuestaInt (22, "Reporte Z");
	}
}


//
// Reporte de Cierre Z Individual por Fecha
//
void
ImpresorFiscal16Bits::ReporteZIndividualPorFecha (FECHA FechaZ, RTA_ReporteZIndividual *R) throw (Excepcion)
{
	// printf ("Comando ReporteZIndividualPorFecha ejecutado en ImpresorFiscal16Bits\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_GET_DAILY_REPORT) + FS +
						FechaZ.ToString() + FS + "T");

	// Retornamos los parámetros recibidos en la respuesta con sus
	// correspondientes valores y los que no viene en la respuesta
	// se retornan con valores default
	if (R != NULL)
	{
		R->FechaZeta						= FECHA (RespuestaString (2, "Reporte de Cierre Z Individual por Fecha"));
		R->NumeroZeta						= RespuestaInt (3, "Reporte de Cierre Z Individual por Fecha");
		R->UltimoDocFiscalBC				= RespuestaInt (4, "Reporte de Cierre Z Individual por Fecha");
		R->UltimoDocFiscalA					= RespuestaInt (5, "Reporte de Cierre Z Individual por Fecha");
		R->MontoVentasDocFiscal				= RespuestaDouble (6, "Reporte de Cierre Z Individual por Fecha");
		R->MontoIVADocFiscal				= RespuestaDouble (7, "Reporte de Cierre Z Individual por Fecha");
		R->MontoImpInternosDocFiscal		= RespuestaDouble (8, "Reporte de Cierre Z Individual por Fecha");
		R->MontoPercepcionesDocFiscal		= RespuestaDouble (9, "Reporte de Cierre Z Individual por Fecha");
		R->MontoIVANoInscriptoDocFiscal		= RespuestaDouble (10, "Reporte de Cierre Z Individual por Fecha");
		R->UltimaNotaCreditoBC				= RespuestaInt (11, "Reporte de Cierre Z Individual por Fecha");
		R->UltimaNotaCreditoA				= RespuestaInt (12, "Reporte de Cierre Z Individual por Fecha");
		R->MontoVentasNotaCredito			= RespuestaDouble (13, "Reporte de Cierre Z Individual por Fecha");
		R->MontoIVANotaCredito				= RespuestaDouble (14, "Reporte de Cierre Z Individual por Fecha");
		R->MontoImpInternosNotaCredito		= RespuestaDouble (15, "Reporte de Cierre Z Individual por Fecha");
		R->MontoPercepcionesNotaCredito		= RespuestaDouble (16, "Reporte de Cierre Z Individual por Fecha");
		R->MontoIVANoInscriptoNotaCredito	= RespuestaDouble (17, "Reporte de Cierre Z Individual por Fecha");
		R->UltimoRemito						= RespuestaInt (18, "Reporte de Cierre Z Individual por Fecha");
	}
}


//
// Reporte de Cierre Z Individual por Número
//
void
ImpresorFiscal16Bits::ReporteZIndividualPorNumero (unsigned NumeroZ, RTA_ReporteZIndividual *R) throw (Excepcion)
{
	// printf ("Comando ReporteZIndividualPorNumero ejecutado en ImpresorFiscal16Bits\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_GET_DAILY_REPORT) + FS +
						Numero (NumeroZ) + FS + "Z");

	// Retornamos los parámetros recibidos en la respuesta con sus
	// correspondientes valores y los que no viene en la respuesta
	// se retornan con valores default
	if (R != NULL)
	{
		R->FechaZeta						= FECHA (RespuestaString (2, "Reporte de Cierre Z Individual por Número"));
		R->NumeroZeta						= RespuestaInt (3, "Reporte de Cierre Z Individual por Número");
		R->UltimoDocFiscalBC				= RespuestaInt (4, "Reporte de Cierre Z Individual por Número");
		R->UltimoDocFiscalA					= RespuestaInt (5, "Reporte de Cierre Z Individual por Número");
		R->MontoVentasDocFiscal				= RespuestaDouble (6, "Reporte de Cierre Z Individual por Número");
		R->MontoIVADocFiscal				= RespuestaDouble (7, "Reporte de Cierre Z Individual por Número");
		R->MontoImpInternosDocFiscal		= RespuestaDouble (8, "Reporte de Cierre Z Individual por Número");
		R->MontoPercepcionesDocFiscal		= RespuestaDouble (9, "Reporte de Cierre Z Individual por Número");
		R->MontoIVANoInscriptoDocFiscal		= RespuestaDouble (10, "Reporte de Cierre Z Individual por Número");
		R->UltimaNotaCreditoBC				= RespuestaInt (11, "Reporte de Cierre Z Individual por Número");
		R->UltimaNotaCreditoA				= RespuestaInt (12, "Reporte de Cierre Z Individual por Número");
		R->MontoVentasNotaCredito			= RespuestaDouble (13, "Reporte de Cierre Z Individual por Número");
		R->MontoIVANotaCredito				= RespuestaDouble (14, "Reporte de Cierre Z Individual por Número");
		R->MontoImpInternosNotaCredito		= RespuestaDouble (15, "Reporte de Cierre Z Individual por Número");
		R->MontoPercepcionesNotaCredito		= RespuestaDouble (16, "Reporte de Cierre Z Individual por Número");
		R->MontoIVANoInscriptoNotaCredito	= RespuestaDouble (17, "Reporte de Cierre Z Individual por Número");
		R->UltimoRemito						= RespuestaInt (18, "Reporte de Cierre Z Individual por Numero");
	}
}


// ############################################################ //
//																//
//			M E T O D O S   D E    D F							//
//																//
// ############################################################ //

//
// Abrir DF
// Envía el comando de Apertura de DF en la estación indicada
//
void
ImpresorFiscal16Bits::AbrirDF (DocumentosFiscales Tipo) throw (Excepcion)
{
	// printf ("Comando AbrirDF ejecutado en ImpresorFiscal16Bits\n");

	// Ejecutamos el método de la SuperClase 'ImpresorFiscal'
	ImpresorFiscal::AbrirDF (Tipo);

	// Actualizamos 'FirstDocNum' con el número de DF abierto.
	FirstDocNum = RespuestaInt (2, "Abrir DF");
}


//
// Especificar Monto de IVA No Inscripto
//
// Nota: este método no es válido solo para los modelos
//		 de 16 bits P330, P330_201, P435 y PL9.
//
void
ImpresorFiscal16Bits::EspecificarIVANoInscripto (double Monto) throw (Excepcion)
{
	// printf ("Comando EspecificarIVANoInscripto ejecutado en ImpresorFiscal16Bits\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_CHARGE_NON_REGISTERED_TAX) + FS +
						Numero (Monto, 2));
}


//
// Especificar Percepción Global
//
void
ImpresorFiscal16Bits::EspecificarPercepcionGlobal (
									const std::string &Nombre,
									double Monto) throw (Excepcion)
{
	// printf ("Comando EspecificarPercepcionGlobal ejecutado en ImpresorFiscal16Bits\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_SET_PERCEPTION) + FS +
						"**.**" + FS +
						Cadena (Nombre, PerceptionDescSize) + FS +
						Numero (Monto, 2));
}


//
// Especificar Percepción por IVA
//
void
ImpresorFiscal16Bits::EspecificarPercepcionPorIVA (
									const std::string &Nombre,
									double Monto,
									double IVA) throw (Excepcion)
{
	// printf ("Comando EspecificarPercepcionPorIVA ejecutado en ImpresorFiscal16Bits\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_SET_PERCEPTION) + FS +
						Numero (IVA, 2) + FS +
						Cadena (Nombre, PerceptionDescSize) + FS +
						Numero (Monto, 2));
}


//
// Subtotal
//
// Nota: esta implementación es válida para todos los modelos
//		 de 16 Bits excepto para P320, PJ20 y PL8, en los que
//		 la respuesta no cuenta con el ultimo campo 'MontoII'.
//
void
ImpresorFiscal16Bits::Subtotal (bool Imprime, RTA_Subtotal *R) throw (Excepcion)
{
	// printf ("Comando Subtotal ejecutado en ImpresorFiscal16Bits\n");

	// Ejecutamos el método de la superclase 'ImpresorFiscal'
	ImpresorFiscal::Subtotal (Imprime, R);

	// Completamos los parámetros correspondientes a los campos
	// de respuesta particulares que se agregan en este modelo
	// más alla de los comunes a todos los modelos
	if (R != NULL)
	{
		R->MontoIVANoInscripto		= RespuestaDouble (6, "Subtotal");
		R->MontoImpuestosInternos	= RespuestaDouble (7, "Subtotal");
	}
}


//
// Cerrar DF
//
unsigned
ImpresorFiscal16Bits::CerrarDF (unsigned Copias) throw (Excepcion)
{
	// printf ("Comando CerrarDF ejecutado en ImpresorFiscal16Bits\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_CLOSE_FISCAL_RECEIPT) + FS +
						Numero (Copias));

	// Recuperamos el Número de DF recién emitido de la respuesta
	unsigned NumeroDFRecienEmitido = RespuestaInt(2, "Cerrar DF");

	// Reinicializamos 'FirstDocNum' y 'NumberOfPages' en 0 y
	// 'TicketBCAbierto' en false.
	FirstDocNum		= 0;
	NumberOfPages	= 0;
	TicketBCAbierto	= false;

	// Retornamos el Número de DF recién emitido
	return NumeroDFRecienEmitido;
}


// ############################################################ //
//																//
//			M E T O D O S   D E    D N F						//
//																//
// ############################################################ //

//
// Cerrar DNF
//
void
ImpresorFiscal16Bits::CerrarDNF (unsigned Copias) throw (Excepcion)
{
	// printf ("Comando CerrarDNF ejecutado en ImpresorFiscal16Bits\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_CLOSE_NON_FISCAL) + FS +
						Numero (Copias));
}


// ############################################################ //
//																//
//			M E T O D O S   D E    D N F H						//
//																//
// ############################################################ //

//
// Abrir DNFH
// Envía el comando de Apertura de DNFH en la estación asociada
//
void
ImpresorFiscal16Bits::AbrirDNFH (DocumentosNoFiscalesHomologados Tipo, const std::string &Nro) throw (Excepcion)
{
	// printf ("Comando AbrirDNFH ejecutado en ImpresorFiscal16Bits\n");

	// Verificamos si el Tipo de DNFH es válido para este modelo
	if ( DNFHSoportados.find (Tipo) == DNFHSoportados.end () )
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Abrir DNFH en ImpresorFiscal16Bits");

	assert(DNFHNumSize != 0);

	// Verificamos si el campo 'Nro' fue programado para los
	// DNFH para los cuales es obligatorio.
	if ( Nro.length () == 0 &&
			(Tipo ==  TICKET_RECIBO_X  ||
			 Tipo ==  RECIBO_X		   ||
			 Tipo ==  ORDEN_SALIDA	   ||
			 Tipo ==  RESUMEN_CUENTA   ||
			 Tipo ==  CARGO_HABITACION ||
			 Tipo ==  COTIZACION) )
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_PARAMETRO_OPCIONAL, "Abrir DNFH");

	// Seteamos la estación del DNFH
	Estacion = (Tipo == TICKET_NOTA_CREDITO_A		   ||
				Tipo == TICKET_NOTA_CREDITO_B		   ||
				Tipo == TICKET_RECIBO_X				   ||
				Tipo == TICKET_PAGARE				   ||
				Tipo == TICKET_PRESUPUESTO			   ||
				Tipo == TICKET_ENTREGA				   ||
				Tipo == TICKET_ESTACIONAMIENTO		   ||
				Tipo == TICKET_COBRO_SERVICIO		   ||
				Tipo == TICKET_INGRESO_DINERO		   ||
				Tipo == TICKET_RETIRO_DINERO		   ||
				Tipo == TICKET_TALON_CAMBIO			   ||
				Tipo == TICKET_TALON_REPARTO		   ||
				Tipo == TICKET_TALON_REGALO			   ||
				Tipo == TICKET_TALON_CUENTA_CORRIENTE  ||
				Tipo == TICKET_AVISO_OPERACION_CREDITO ||
				Tipo == TICKET_CUPON_PROMOCION		   ||
				Tipo == TICKET_USO_INTERNO_FARMACIA)
					? ESTACION_TICKET : ESTACION_SLIP;

	// Obtenemos el Identificador de DNFH que reconoe el CF
	IdentificadoresDeDNFH Id = IdentificadorDeDNFH (Tipo);

	if ( Id == DNFH_NO_EXISTENTE )
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Identificador de DNFH no válido");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_OPEN_DNFH) + FS +
						Caracter (Id) + FS +
						Caracter (Estacion) + FS +
						Cadena (Nro, DNFHNumSize));

	// Actualizamos 'FirstDocNum' con el número de DF abierto.
	FirstDocNum = RespuestaInt (2, "Abrir DNFH");

	// Reinicializamos 'NumberOfPages' y 'TicketBCAbierto'
	NumberOfPages	= 0;
	TicketBCAbierto = false;
}


//
// Imprimir Item en Remito u Orden de Salida
//
void
ImpresorFiscal16Bits::ImprimirItemEnRemito (const std::string &Texto, double Cantidad) throw (Excepcion)
{
	// printf ("Comando ImprimirItemEnRemito ejecutado en ImpresorFiscal16Bits\n");

	assert(RemitoTextSize != 0);
	assert(RemitoCantDecimals != 0);

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_PRINT_EMBARK_ITEM) + FS +
						Cadena (Texto, RemitoTextSize) + FS +
						Numero (Cantidad, RemitoCantDecimals) + FS +
						Caracter (ModoDisplay));
}


//
// Imprimir Item en Resumen de Cuenta o Cargo a la Habitación
//
void
ImpresorFiscal16Bits::ImprimirItemEnCuenta (const FECHA &FechaComprobante,
							const std::string &NroComprobante,
							const std::string &Texto,
							double Debe, double Haber) throw (Excepcion)
{
	// printf ("Comando ImprimirItemEnCuenta ejecutado en ImpresorFiscal16Bits\n");

	assert(InfoRemitoCompOriSize != 0);
	assert(ResumeTextSize != 0);

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_PRINT_ACCOUNT_ITEM) + FS +
						FechaComprobante.ToString() + FS +
						Cadena (NroComprobante, InfoRemitoCompOriSize) + FS +
						Cadena (Texto, ResumeTextSize) + FS +
						Numero (Debe, 2) + FS +
						Numero (Haber, 2) + FS +
						Caracter (ModoDisplay));
}


//
// Imprimir Item en Cotización
//
//
void
ImpresorFiscal16Bits::ImprimirItemEnCotizacion (const std::string &Texto) throw (Excepcion)
{
	// printf ("Comando ImprimirItemEnCuenta ejecutado en ImpresorFiscal16Bits\n");

	assert(CotizaTextSize != 0);

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_PRINT_QUOTATION_ITEM) + FS +
						Cadena (Texto, CotizaTextSize) + FS +
						Caracter (ModoDisplay));
}


//
// Detalle Recibo
//
void
ImpresorFiscal16Bits::DetalleRecibo (const std::string &Texto) throw (Excepcion)
{
	// printf ("Comando DetalleRecibo ejecutado en ImpresorFiscal16Bits\n");

	unsigned i, j, OffCnt;
	unsigned TextLen, Lines, TotalWidth;

	unsigned Size = (Estacion == ImpresorFiscal::ESTACION_TICKET)
		? ReciboTextTicketSize : ReciboTextSlipSize;

	assert(Size != 0);

	std::string Description(Texto);
	std::string Line;

	const char *Trailer;

	// Chequea que la descripción haya sido asignado y
	// elimina los espacios a la derecha.
	unsigned Ini;
	unsigned Fin = Description.find_last_not_of (" ");

	if ( Description.length() == 0 || Fin == std::string::npos )
		Description = " ";
	else
		Description = Description.substr (0, Fin+1);

	// Verifica si el tamaño del texto no supera el máximo.
	// En tal caso no modifica el texto. En caso contrario
	// evalúa como debe adaptar el texto para que no se den
	// errores, para lo cual tiene en cuenta si debe operar
	// o no con 'DescripcionesLargas'.
	if (Description.length() > Size)
	{
		// Si no estamos operando con 'DescripcionesLargas' o
		// si el texto supera el máximo y no tiene ningún ' '
		// en medio lo truncamos al tamaño máximo menos 3 y
		// completamos con '...' al final, indicando que fue
		// truncado.

		if ( !DescripcionesLargas ||
			 Description.find (" ") == std::string::npos )
		{
			Description = (Description.substr(0, Size-3) + "...");
		}
		// En caso contrario procedemos a repartir el texto entre
		// las líneas de concepto de recibo.
		else
		{
			TotalWidth = Description.length();
			TextLen = TotalWidth - Size;

			// Solo se chequea la cantidad de líneas cuando se trata
			// de un recibo emitido en estación de carro ancho.
			for (i = 0, Lines = 0; i < TextLen;  )
			{
				// Solo se chequea la cantidad de líneas cuando
				// se trata de un recibo emitido en estación de
				// carro ancho.
				if ( Estacion == ImpresorFiscal::ESTACION_SLIP &&
					 Lines >= ReciboLines-1 )
					break;

				// Saltea los espacios a la izquerda si no es
				// la primer línea de concepto de recibo.
				if ( Lines != 0 )
					i += Description.substr (i).find_first_not_of (" ");

				// Busca el corte para la palabra
				j = Description.find_last_of (" ", i + Size);

				// Borra Cnt. de Offset de Líneas que superan el
				// tamaño de la línea de concepto de recibo.
				OffCnt = 0;

				// Si no encontró espacios de atrás para adelante,
				// verifica si quedan caracteres para la última línea
				// de concepto de recibo (busca al menos un ' ').
				if (j == std::string::npos || j <= i)
				{
					j = Size;
					if ( Description.find (" ", i + j) != std::string::npos )
						// Si restan caracteres para la última línea de
						// concepto de recibo, termina la línea con "..."
						// y se posiciona en el caracter siguiente al
						// primer ' ' para continuar con el procesamiento
						// de la siguiente línea.
						OffCnt = Description.substr (i+j).find_first_of (" ") + 1;
					else
						// Si no restan caracteres para la última línea
						// de concepto de recibo, sale del lazo.
						break;
				}
				// Si encontró algún espacio, corto donde encontró el espacio.
				else
					j -= i;

				Lines++;
				// Si se trata de la ante-última línea de concepto de recibo
				// emitido en estación de carro ancho y restan más caracteres
				// de los que ocupa la última línea, o si la ultima línea
				// superaba el tamaño una línea de concepto, corta la línea y
				// la concluye con "...".
				if ( ( Estacion == ImpresorFiscal::ESTACION_SLIP &&
					   Lines == ReciboLines-1 &&
					   Description.substr (i+j).length () > Size &&
					   j >= (Size - 3) ) ||
					 ( OffCnt != 0 ) )
				{
					if ( ((j = Description.substr (i).find_last_not_of (" ", j)) != std::string::npos) &&
						  (j >= (Size - 3)) )
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
				// en caso de serlo no se genera la línea de concepto y
				// se eliminan los espacios que siguen para que la
				// siguiente línea comience con una palabra.
				if ( Line.find_last_not_of (" ", j-1) != std::string::npos )
					// Enviamos el comando fiscal y evaluamos los status
					EnviarComandoFiscal(OpCode(CMD_RECEIPT_TEXT) + FS +
										Cadena (Line, Size));
				else
				{
					Lines--;
					// Elimina los espacios a la izquerda de la
					// siguiente linea ...
					i = Description.find_first_not_of (" ", i);
				}
			}

			// Al salir del lazo del for nos quedamos solo con
			// la parte restante del texto.
			Description = Description.substr(i);

			// Si hubo alguna línea de concepto de recibo previa
			// elimina los espacios a la izquierda del texto restante.
			if ( Lines != 0 )
				if ((Ini = Description.find_first_not_of (" ")) != std::string::npos)
					Description = Description.substr (Ini);

			// Si quedó más de lo que entra en una línea de concepto
			// de recibo, la corto de atrás para adelante.
			if (Description.length() > Size)
				Description = "..." + Description.substr(Description.length()-Size+3);
		}
	}

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_RECEIPT_TEXT) + FS +
						Cadena (Description, Size));
}


//
// Imprimir Información en DNFH
//
// Nota: esta método es válido para todos los modelos de CF de
//		 16 bits excepto 320, PJ20, 321 y PL8 (version 1.00).
//
void
ImpresorFiscal16Bits::ImprimirInfoEnDNFH (const unsigned IDCampo,
										  const std::string &Texto,
										  double Cantidad) throw (Excepcion)
{
	// printf ("Comando ImprimirInfoEnDNFH ejecutado en ImpresorFiscal16Bits\n");

	assert(PrintDNFHTextSize != 0);

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_PRINT_INFO_DNFH) + FS +
						Numero (IDCampo) + FS +
						Cadena (Texto, PrintDNFHTextSize) + FS +
						Caracter (ModoDisplay));
}


//
// Cerrar DNFH
//
unsigned
ImpresorFiscal16Bits::CerrarDNFH (unsigned Copias) throw (Excepcion)
{
	// printf ("Comando CerrarDNFH ejecutado en ImpresorFiscal16Bits\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_CLOSE_DNFH) + FS +
						Numero (Copias));

	// Recuperamos el Número de DF recién emitido de la respuesta
	unsigned NumeroDNFHRecienEmitido = RespuestaInt(2, "Cerrar DNFH");

	// Reinicializamos 'FirstDocNum' y 'NumberOfPages' en 0 y
	// 'TicketBCAbierto' en false.
	FirstDocNum		= 0;
	NumberOfPages	= 0;
	TicketBCAbierto	= false;

	// Retornamos el Número de DNFH recién emitido
	return NumeroDNFHRecienEmitido;
}


//
// Emitir un DNFH de Farmacia
//
void
ImpresorFiscal16Bits::DNFHFarmacia (unsigned /* Copias */) throw (Excepcion)
{
	// printf ("Comando DNFHFarmacia ejecutado en ImpresorFiscal16Bits\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "DNFH Farmacia");
}


//
// Emitir un DNFH de Reparto
//
void
ImpresorFiscal16Bits::DNFHReparto (unsigned /* Copias */) throw (Excepcion)
{
	// printf ("Comando DNFHReparto ejecutado en ImpresorFiscal16Bits\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "DNFH Reparto");
}


//
// Imprimir Voucher de Tarjeta
//
// Nota: esta método es válido para todos los modelos de CF de
//		 16 bits excepto 320, PJ20, 321 y PL8 (version 1.00).
//
void
ImpresorFiscal16Bits::ImprimirVoucher (const std::string &NombreCliente,
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
									const std::string &NumeroComprobanteAsociado,
									unsigned Copias,
									const std::string &Vendedor,
									TiposDeEstacion Estacion) throw (Excepcion)
{
	// printf ("Comando ImprimirVoucher ejecutado en ImpresorFiscal16Bits\n");

	// Enviamos el comando fiscal 'SetVoucher1' y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_SET_VOUCHER_DATA_1) + FS +
						Cadena (NombreCliente, VouchCustNameSize) + FS +
						Cadena (NombreTarjeta, VouchCardNameSize) + FS +
						Caracter (TipoVoucher) + FS +
						Cadena (NumeroDeTarjeta, VouchCardNumSize) + FS +
						FechaDeVencimiento.ToString().substr(0, 4) + FS +
						Caracter (TipoTarjeta) + FS +
						Numero (Cuotas));

	// Enviamos el comando fiscal 'SetVoucher2' y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_SET_VOUCHER_DATA_2) + FS +
						Cadena (CodigoDeComercio, VouchComCodeSize) + FS +
						Numero (NumeroDeTerminal) + FS +
						Numero (NumeroDeLote) + FS +
						Numero (NumeroCupon) + FS +
						Caracter (TipoIngreso) + FS +
						Caracter (TipoOperacion) + FS +
						Numero (NumeroAutorizacion) + FS +
						Cadena (Monto, VouchImportSize) + FS +
						Cadena (NumeroComprobanteAsociado, VouchCompNumSize) + FS +
						Cadena (Vendedor, VouchVendNameSize));

	// Enviamos el comando fiscal 'PrintVoucher' y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_PRINT_VOUCHER) + FS +
						Numero (Copias) + FS +
						Caracter (Estacion));
}

