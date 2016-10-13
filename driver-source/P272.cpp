#include "P272.h"



// ############################################################ //
//																//
//			M E T O D O S      L O C A L E S					//
//																//
// ############################################################ //

//
// Constructor
//
ImpresorFiscalP272::ImpresorFiscalP272 ()
{
	// Inicialización de variables de uso general
	MaxFiscalTextLinesInvoice		= 4;	// Por seguridad !!!
	MaxFiscalTextLinesTicket		= 4;

	// Inicialización de variables de tamaño de campos
	PrintNonFiscalTextTicketSize	= 48;
	PrintNonFiscalTextSlipSize		= 48;	// Por seguridad !!!
	FantasyNameSize					= 48;
	HeaderTrailerSize				= 48;
	HeaderTrailerTicketSize			= 48;
	PrintFiscalTextTicketSize		= 36;
	PrintFiscalTextSlipSize			= 36;	// Por seguridad !!
	PrintItemTextTicketSize			= 28;
	PrintItemTextSlipSize			= 28;	// Por seguridad !!
	TotalTenderTextSize				= 32;
	VouchImportSize					= 15;
	VouchCompNumSize				= 20;
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
ImpresorFiscalP272::ObtenerVersionDelControlador () throw (Excepcion)
{
    // printf ("Comando ObtenerVersionDelControlador ejecutado en P272\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_GET_VERSION));

	// Retornamos la información obtenida de la respuesta
	return RespuestaString(2, "Obtener Versión del Controlador Fiscal");
}


//
// Baja Fiscal
//
void
ImpresorFiscalP272::BajaFiscal (const std::string &NumeroDeSerie) throw(Excepcion)
{
    // printf ("Comando BajaFiscal ejecutado en P272\n");

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
void
ImpresorFiscalP272::CambiarCodigoIngresosBrutos (const std::string &CodigoIngBr) throw(Excepcion)
{
    // printf ("Comando CambiarCodigoIngresosBrutos ejecutado en P272\n");

	assert(CodeIngBrSize != 0);

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_CHG_ING_BR) + FS +
						Cadena (CodigoIngBr, CodeIngBrSize, 0));
}


//
// Cambiar Fecha de Inicio de Actividades
//
void
ImpresorFiscalP272::CambiarFechaInicioActividades (const FECHA &Fecha) throw(Excepcion)
{
    // printf ("Comando CambiarFechaInicioActividades ejecutado en P272\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_CHG_INI_ACT_DATE) + FS + Fecha.ToString());
}


//
// Documento en Curso
//
ImpresorFiscal::TiposDeDocumento
ImpresorFiscalP272::DocumentoEnCurso () throw(Excepcion)
{
    // printf ("Comando DocumentoEnCurso ejecutado en P272\n");

	return DocumentoCF ();
}


//
// Descripción del Documento en Curso
//
std::string
ImpresorFiscalP272::DescripcionDocumentoEnCurso () throw(Excepcion)
{
    // printf ("Comando DescripcionDocumentoEnCurso ejecutado en P272\n");

	return DescripcionDocumentoCF ();
}


//
// Ultimo Documento fue Cancelado
//
bool
ImpresorFiscalP272::UltimoDocumentoFueCancelado () throw(Excepcion)
{
    // printf ("Comando UltimoDocumentoCancelado ejecutado en P272\n");

	return IndicadorDeDocumentoPrevioCancelado ();
}


//
// Primer Número de Documento Actual
//
unsigned
ImpresorFiscalP272::PrimerNumeroDeDocumentoActual () throw(Excepcion)
{
    // printf ("Comando PrimerNumeroDeDocumentoActual ejecutado en P272\n");

	// Si FirstDocNum es 0 indica que estamos fuera de todo
	// documento, por lo que su contenido carece de validez
	if (FirstDocNum == 0)
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_ESTADO_NO_VALIDO, "Primer Número de Documento Actual");

	return FirstDocNum;
}


//
// Cancelar Comprobante Fiscal
//
void
ImpresorFiscalP272::CancelarComprobanteFiscal () throw(Excepcion)
{
    // printf ("Comando CancelarComprobanteFiscal ejecutado en P272\n");

	// En este modelo el comando de Cancelación utilizado
	// para todos los documentos es el mismo
        ImpresorFiscalP272::CancelarComprobante ();
}


//
// Cancelar Comprobante
//
void
ImpresorFiscalP272::CancelarComprobante () throw(Excepcion)
{
    // printf ("Comando CancelarComprobante ejecutado en P272\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_CANCEL));

	// Reinicializamos 'FirstDocNum' y 'NumberOfPages' en 0 y
	// 'TicketBCAbierto' en false.
	FirstDocNum		= 0;
	NumberOfPages	= 0;
	TicketBCAbierto	= false;
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
ImpresorFiscalP272::ObtenerNombreDeFantasia (unsigned Linea) throw (Excepcion)
{
    // printf ("Comando ObtenerNombreDeFantasia ejecutado en P272\n");
	
	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_GET_FANTASY_NAME) + FS + Numero (Linea));

	// Retornamos la información obtenida de la respuesta
	return RespuestaString(2, "Obtener Línea de Nombre de Fantasía");
}


//
// Configurar Controlador por Bloque de Parámetros
//
// Nota: este método no es válido para este modelo.
//
void
ImpresorFiscalP272::ConfigurarControladorPorBloque (
							double /* LimiteConsumidorFinal */,
							double /* LimiteTicketFactura */,
							double /* PorcentajeIVANoInscripto */,
							NumerosDeCopias /* TipoDeCopiasMaximo */,
							bool /* ImpresionCambio */,
							bool /* ImpresionLeyendasOpcionales */,
							TiposDeCorteDePapel /* TipoDeCorte */) throw (Excepcion)
{
    // printf ("Comando ConfigurarControladorPorBloque ejecutado en P272\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Configurar Controlador por Bloque de Parámetros");
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
ImpresorFiscalP272::ReporteZ (RTA_ReporteZX *R) throw (Excepcion)
{
    // printf ("Comando ReporteZ ejecutado en P272\n");
	
	// Ejecutamos el método de la superclase 'ImpresorFiscal'
	ImpresorFiscal::ReporteZ (R);

	// Completamos los parámetros correspondientes a los campos de
	// respuesta particulares que se agregan en este modelo más alla
	// de los comunes a todos los modelos
	if (R != NULL)
		R->MontoImpInternosDocFiscal		= RespuestaDouble (12, "Reporte Z");
}


//
// Reporte X
//
void
ImpresorFiscalP272::ReporteX (RTA_ReporteZX *R) throw (Excepcion)
{
    // printf ("Comando ReporteX ejecutado en P272\n");
	
	// Ejecutamos el método de la superclase 'ImpresorFiscal'
	ImpresorFiscal::ReporteX (R);

	// Completamos los parámetros correspondientes a los campos de
	// respuesta particulares que se agregan en este modelo más alla
	// de los comunes a todos los modelos
	if (R != NULL)
		R->MontoImpInternosDocFiscal		= RespuestaDouble (12, "Reporte X");
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
ImpresorFiscalP272::AbrirDF (DocumentosFiscales Tipo) throw (Excepcion)
{
    // printf ("Comando AbrirDF ejecutado en P272\n");

	// Ejecutamos el método de la SuperClase 'ImpresorFiscal'
	ImpresorFiscal::AbrirDF (Tipo);

	// Actualizamos 'FirstDocNum' con el número de DF abierto.
	FirstDocNum = RespuestaInt (2, "Abrir DF");
}


//
// Subtotal
//
void
ImpresorFiscalP272::Subtotal (bool Imprime, RTA_Subtotal *R) throw (Excepcion)
{
    // printf ("Comando Subtotal ejecutado en P272\n");
	
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


// ############################################################ //
//																//
//			M E T O D O S   D E    D N F						//
//																//
// ############################################################ //

//
// Abrir DNF
// Envía el comando de Apertura de DNF en la estación indicada
//
void
ImpresorFiscalP272::AbrirDNF (TiposDeEstacion EstacionDeImpresion) throw (Excepcion)
{
    // printf ("Comando AbrirDNF ejecutado en P272\n");

	// Verificamos si la estación indicada es válida en el modelo.
	// En caso de serlo ejecutamos el método de la SuperClase
	// 'ImpresorFiscal'. En caso contrario, damos una excepción.
	if (EstacionDeImpresion != ESTACION_TICKET)
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Abrir DNF en estación SLIP");

	// Ejecutamos el método de la SuperClase 'ImpresorFiscal'.
	ImpresorFiscal::AbrirDNF (EstacionDeImpresion);
}


// ############################################################ //
//																//
//			M E T O D O S   D E    D N F H						//
//																//
// ############################################################ //

//
// Imprimir Voucher de Tarjeta
//
void
ImpresorFiscalP272::ImprimirVoucher (const std::string &NombreCliente,
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
    // printf ("Comando ImprimirVoucher ejecutado en P272\n");

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
						Numero (Copias));
}

