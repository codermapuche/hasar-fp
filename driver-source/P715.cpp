#include "P715.h"



// ############################################################ //
//																//
//			M E T O D O S      L O C A L E S					//
//																//
// ############################################################ //

//
// Constructor
//
ImpresorFiscalP715::ImpresorFiscalP715 ()
{
	// Inicialización de variables de uso general
	MaxFiscalTextLinesInvoice		= 4;
	MaxFiscalTextLinesTicket		= 4;

	// Inicialización de variables de tamaño de campos
	PrintNonFiscalTextTicketSize	= 40;
	PrintNonFiscalTextSlipSize		= 40;	// Por seguridad !!!
	FantasyNameSize					= 40;
	HeaderTrailerSize				= 40;
	HeaderTrailerTicketSize			= 40;
	RazonSocialSize					= 40;
	DireccionSize					= 40;
	PrintFiscalTextTicketSize		= 28;
	PrintFiscalTextSlipSize			= 28;	// Por seguridad !!
	PrintItemTextTicketSize			= 20;
	PrintItemTextSlipSize			= 20;	// Por seguridad !!
	TotalTenderTextSize				= 24;
	VouchImportSize					= 15;
	VouchCompNumSize				= 20;

	// Cargamos los DF soportados
	DFSoportados.insert (TICKET_FACTURA_A);
	DFSoportados.insert (TICKET_FACTURA_B);
	DFSoportados.insert (TICKET_NOTA_DEBITO_A);
	DFSoportados.insert (TICKET_NOTA_DEBITO_B);

	// Cargamos los DNFH soportados
	DNFHSoportados.insert (TICKET_NOTA_CREDITO_A);
	DNFHSoportados.insert (TICKET_NOTA_CREDITO_B);
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
ImpresorFiscalP715::ObtenerVersionDelControlador () throw (Excepcion)
{
	// printf ("Comando ObtenerVersionDelControlador ejecutado en P715\n");
	
	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_GET_VERSION));

	// Retornamos la información obtenida de la respuesta
	return RespuestaString(2, "Obtener Versión del Controlador Fiscal");
}


//
// Obtener Datos de Memoria de Trabajo
//
void
ImpresorFiscalP715::ObtenerDatosMemoriaDeTrabajo (RTA_ObtenerDatosMemoriaDeTrabajo *R) throw (Excepcion)
{
	// printf ("Comando ObtenerDatosMemoriaDeTrabajo ejecutado en P715\n");

	// Ejecutamos el método de la superclase 'ImpresorFiscal'
	ImpresorFiscal::ObtenerDatosMemoriaDeTrabajo (R);

	// Completamos los parámetros correspondientes a los campos
	// de respuesta particulares que se agregan en este modelo
	// más alla de los comunes a todos los modelos
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
		R->CantidadNCCanceladas				= RespuestaInt (20, "Obtener Datos de Memoria de Trabajo");
	}
}


//
// Baja Fiscal
//
void
ImpresorFiscalP715::BajaFiscal (const std::string &NumeroDeSerie) throw(Excepcion)
{
	// printf ("Comando BajaFiscal ejecutado en P715\n");

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
ImpresorFiscalP715::CambiarCodigoIngresosBrutos (const std::string &CodigoIngBr) throw(Excepcion)
{
	// printf ("Comando CambiarCodigoIngresosBrutos ejecutado en P715\n");

	assert(CodeIngBrSize != 0);

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_CHG_ING_BR) + FS +
						Cadena (CodigoIngBr, CodeIngBrSize, 0));
}


//
// Documento en Curso
//
ImpresorFiscal::TiposDeDocumento
ImpresorFiscalP715::DocumentoEnCurso () throw(Excepcion)
{
	// printf ("Comando DocumentoEnCurso ejecutado en P715\n");

	return DocumentoCF ();
}


//
// Descripción del Documento en Curso
//
std::string
ImpresorFiscalP715::DescripcionDocumentoEnCurso () throw(Excepcion)
{
	// printf ("Comando DescripcionDocumentoEnCurso ejecutado en P715\n");

	return DescripcionDocumentoCF ();
}


//
// Ultimo Documento fue Cancelado
//
bool
ImpresorFiscalP715::UltimoDocumentoFueCancelado () throw(Excepcion)
{
	// printf ("Comando UltimoDocumentoCancelado ejecutado en P715\n");

	return IndicadorDeDocumentoPrevioCancelado ();
}


//
// Ultimo Documento Fiscal Tipo A
//
unsigned
ImpresorFiscalP715::UltimoDocumentoFiscalA () throw(Excepcion)
{
	// printf ("Comando UltimoDocumentoFiscalA ejecutado en P715\n");

	// Enviamos el comando fiscal
	SolicitarEstado ();

	// Retornamos la información obtenida de la respuesta
	return RespuestaInt(4, "Ultimo Documento Fiscal Tipo A");
}


//
// Ultima Nota de Crédito Tipo BC
//
unsigned
ImpresorFiscalP715::UltimaNotaCreditoBC () throw(Excepcion)
{
	// printf ("Comando UltimaNotaCreditoBC ejecutado en P715\n");

	// Enviamos el comando fiscal
	SolicitarEstado ();

	// Retornamos la información obtenida de la respuesta
	return RespuestaInt(6, "Ultima Nota de Crédito Tipo BC");
}


//
// Ultima Nota de Crédito Tipo A
//
unsigned
ImpresorFiscalP715::UltimaNotaCreditoA () throw(Excepcion)
{
	// printf ("Comando UltimaNotaCreditoA ejecutado en P715\n");

	// Enviamos el comando fiscal
	SolicitarEstado ();

	// Retornamos la información obtenida de la respuesta
	return RespuestaInt(7, "Ultima Nota de Crédito Tipo A");
}


//
// Primer Número de Documento Actual
//
unsigned
ImpresorFiscalP715::PrimerNumeroDeDocumentoActual () throw(Excepcion)
{
	// printf ("Comando PrimerNumeroDeDocumentoActual ejecutado en P715\n");

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
ImpresorFiscalP715::CancelarComprobanteFiscal () throw(Excepcion)
{
	// printf ("Comando CancelarComprobanteFiscal ejecutado en P715\n");

	// En este modelo el comando de Cancelación utilizado
	// para todos los documentos es el mismo
	ImpresorFiscalP715::CancelarComprobante ();
}


//
// Cancelar Comprobante
//
void
ImpresorFiscalP715::CancelarComprobante () throw(Excepcion)
{
	// printf ("Comando CancelarComprobante ejecutado en P715\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_CANCEL));

	// Reinicializamos 'FirstDocNum' y 'NumberOfPages' en 0 y
	// 'TicketBCAbierto' en false.
	FirstDocNum		= 0;
	NumberOfPages	= 0;
	TicketBCAbierto	= false;
}


// Capacidad de impresión de documentos en estación Ticket
void
ImpresorFiscalP715::CapacidadDocumentoTicket (bool &SoportaFA, bool &SoportaNC)
{
	// printf ("Comando CapacidadDocumentoTicket ejecutado en P715\n");

	SoportaFA = true;
	SoportaNC = true;
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
ImpresorFiscalP715::ObtenerNombreDeFantasia (unsigned Linea) throw (Excepcion)
{
	// printf ("Comando ObtenerNombreDeFantasia ejecutado en P715\n");
	
	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_GET_FANTASY_NAME) + FS + Numero (Linea));

	// Retornamos la información obtenida de la respuesta
	return RespuestaString(2, "Obtener Línea de Nombre de Fantasía");
}


//
// Especificar Información Línea de Remito / Comprobante Original
//
void
ImpresorFiscalP715::EspecificarInformacionRemitoComprobanteOriginal (unsigned Linea, const std::string &Texto) throw (Excepcion)
{
	// printf ("Comando EspecificarInformacionRemitoComprobanteOriginal ejecutado en P715\n");

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
ImpresorFiscalP715::ObtenerInformacionRemitoComprobanteOriginal (unsigned Linea) throw (Excepcion)
{
	// printf ("Comando ObtenerInformacionRemitoComprobanteOriginal ejecutado en P715\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_GET_EMBARK_NUMBER) + FS + Numero (Linea));

	// Retornamos la información obtenida de la respuesta
	return RespuestaString(2, "Obtener Información Línea de Remito / Comprobante Original");
}


//
// Borrar Información Línea de Remito / Comprobante Original
//
void
ImpresorFiscalP715::BorrarInformacionRemitoComprobanteOriginal () throw (Excepcion)
{
	// printf ("Comando BorrarInformacionRemitoComprobanteOriginal ejecutado en P715\n");

	// Enviamos el comando fiscal y evaluamos los status
	for ( unsigned Linea = First_EmbarkNum_Line; Linea <= Last_EmbarkNum_Line; Linea++ )
		EnviarComandoFiscal(OpCode(CMD_SET_EMBARK_NUMBER) + FS +
							Numero (Linea) + FS + ERASE_LINE);
}


//
// Obtener Configuracion del Controlador
//
void
ImpresorFiscalP715::ObtenerConfiguracion (RTA_ObtenerConfiguracion *R) throw (Excepcion)
{
	// printf ("Comando ObtenerConfiguracion ejecutado en P715\n");

	// Ejecutamos el método de la superclase 'ImpresorFiscal8Bits'
	ImpresorFiscal8Bits::ObtenerConfiguracion (R);

	// Completamos los parámetros correspondientes a los campos
	// de respuesta particulares que se agregan en este modelo
	// más alla de los comunes a todos los modelos
	if (R != NULL)
		R->DescripcionDelMedioDePago	= RespuestaString (11, "Obtener Configuracion del Controlador");
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
ImpresorFiscalP715::ReporteZ (RTA_ReporteZX *R) throw (Excepcion)
{
	// printf ("Comando ReporteZ ejecutado en P715\n");
	
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
	}
}


//
// Reporte X
//
void
ImpresorFiscalP715::ReporteX (RTA_ReporteZX *R) throw (Excepcion)
{
	// printf ("Comando ReporteX ejecutado en P715\n");

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
	}
}


//
// Reporte de Cierre Z Individual por Fecha
//
void
ImpresorFiscalP715::ReporteZIndividualPorFecha (FECHA FechaZ, RTA_ReporteZIndividual *R) throw (Excepcion)
{
	// printf ("Comando ReporteZIndividualPorFecha ejecutado en P715\n");

	// Ejecutamos el método de la superclase 'ImpresorFiscal8Bits'
	ImpresorFiscal8Bits::ReporteZIndividualPorFecha (FechaZ, R);

	// Completamos los parámetros correspondientes a los campos de
	// respuesta particulares que se agregan en este modelo más alla
	// de los comunes a todos los modelos
	if (R != NULL)
	{
		R->MontoPercepcionesDocFiscal		= RespuestaDouble (9, "Reporte de Cierre Z Individual por Fecha");
		R->MontoIVANoInscriptoDocFiscal		= RespuestaDouble (10, "Reporte de Cierre Z Individual por Fecha");
		R->UltimaNotaCreditoBC				= RespuestaInt (11, "Reporte de Cierre Z Individual por Fecha");
		R->UltimaNotaCreditoA				= RespuestaInt (12, "Reporte de Cierre Z Individual por Fecha");
		R->MontoVentasNotaCredito			= RespuestaDouble (13, "Reporte de Cierre Z Individual por Fecha");
		R->MontoIVANotaCredito				= RespuestaDouble (14, "Reporte de Cierre Z Individual por Fecha");
		R->MontoImpInternosNotaCredito		= RespuestaDouble (15, "Reporte de Cierre Z Individual por Fecha");
		R->MontoPercepcionesNotaCredito		= RespuestaDouble (16, "Reporte de Cierre Z Individual por Fecha");
		R->MontoIVANoInscriptoNotaCredito	= RespuestaDouble (17, "Reporte de Cierre Z Individual por Fecha");
	}
}


//
// Reporte de Cierre Z Individual por Número
//
void
ImpresorFiscalP715::ReporteZIndividualPorNumero (unsigned NumeroZ, RTA_ReporteZIndividual *R) throw (Excepcion)
{
	// printf ("Comando ReporteZIndividualPorNumero ejecutado en P715\n");

	// Ejecutamos el método de la superclase 'ImpresorFiscal8Bits'
	ImpresorFiscal8Bits::ReporteZIndividualPorNumero (NumeroZ, R);

	// Completamos los parámetros correspondientes a los campos de
	// respuesta particulares que se agregan en este modelo más alla
	// de los comunes a todos los modelos
	if (R != NULL)
	{
		R->MontoPercepcionesDocFiscal		= RespuestaDouble (9, "Reporte de Cierre Z Individual por Fecha");
		R->MontoIVANoInscriptoDocFiscal		= RespuestaDouble (10, "Reporte de Cierre Z Individual por Fecha");
		R->UltimaNotaCreditoBC				= RespuestaInt (11, "Reporte de Cierre Z Individual por Fecha");
		R->UltimaNotaCreditoA				= RespuestaInt (12, "Reporte de Cierre Z Individual por Fecha");
		R->MontoVentasNotaCredito			= RespuestaDouble (13, "Reporte de Cierre Z Individual por Fecha");
		R->MontoIVANotaCredito				= RespuestaDouble (14, "Reporte de Cierre Z Individual por Fecha");
		R->MontoImpInternosNotaCredito		= RespuestaDouble (15, "Reporte de Cierre Z Individual por Fecha");
		R->MontoPercepcionesNotaCredito		= RespuestaDouble (16, "Reporte de Cierre Z Individual por Fecha");
		R->MontoIVANoInscriptoNotaCredito	= RespuestaDouble (17, "Reporte de Cierre Z Individual por Fecha");
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
ImpresorFiscalP715::AbrirDF (DocumentosFiscales Tipo) throw (Excepcion)
{
	// printf ("Comando AbrirDF ejecutado en P715\n");

	// Ejecutamos el método de la SuperClase 'ImpresorFiscal'
	ImpresorFiscal::AbrirDF (Tipo);

	// Actualizamos 'FirstDocNum' con el número de DF abierto.
	FirstDocNum = RespuestaInt (2, "Abrir DF");
}


//
// Especificar Percepción Global
//
void
ImpresorFiscalP715::EspecificarPercepcionGlobal (
									const std::string &Nombre,
									double Monto) throw (Excepcion)
{
	// printf ("Comando EspecificarPercepcionGlobal ejecutado en P715\n");

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
ImpresorFiscalP715::EspecificarPercepcionPorIVA (
									const std::string &Nombre,
									double Monto,
									double IVA) throw (Excepcion)
{
	// printf ("Comando EspecificarPercepcionPorIVA ejecutado en P715\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_SET_PERCEPTION) + FS +
						Numero (IVA, 2) + FS +
						Cadena (Nombre, PerceptionDescSize) + FS +
						Numero (Monto, 2));
}


//
// Subtotal
//
void
ImpresorFiscalP715::Subtotal (bool Imprime, RTA_Subtotal *R) throw (Excepcion)
{
	// printf ("Comando Subtotal ejecutado en P715\n");
	
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
ImpresorFiscalP715::AbrirDNF (TiposDeEstacion EstacionDeImpresion) throw (Excepcion)
{
	// printf ("Comando AbrirDNF ejecutado en P715\n");

	// Verificamos si la estación indicada es válida en el modelo.
	// En caso de serlo ejecutamos el método de la SuperClase
	// 'ImpresorFiscal'. En caso contrario, damos una excepción.
	if (EstacionDeImpresion != ESTACION_TICKET)
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Abrir DNF en estación SLIP");

	ImpresorFiscal::AbrirDNF (EstacionDeImpresion);
}


// ############################################################ //
//																//
//			M E T O D O S   D E    D N F H						//
//																//
// ############################################################ //

//
// Abrir DNFH
// Envía el comando de Apertura de DNFH en la estación indicada
//
void
ImpresorFiscalP715::AbrirDNFH (DocumentosNoFiscalesHomologados Tipo, const std::string &Nro) throw (Excepcion)
{
	// printf ("Comando AbrirDNFH ejecutado en P715\n");

	// Verificamos si el Tipo de DNFH es válido para este modelo
	if ( DNFHSoportados.find (Tipo) == DNFHSoportados.end () )
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Abrir DNFH en ImpresorFiscalP715");

	assert(DNFHNumSize != 0);

	// Verificamos si el campo 'Nro' fue programado para los
	// DNFH para los cuales es obligatorio.
	if ( Nro.length () == 0 && Tipo ==  TICKET_RECIBO_X )
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_PARAMETRO_OPCIONAL, "Abrir DNFH");

	// Seteamos la estación del DNFH
	Estacion = ESTACION_TICKET;

	// Obtenemos el Identificador de DNFH que reconoe el CF
	IdentificadoresDeDNFH Id = IdentificadorDeDNFH (Tipo);

	if ( Id == DNFH_NO_EXISTENTE )
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Identificador de DNFH no válido");

	// Enviamos el comando fiscal y evaluamos los status

	// Verificamos la existencia del parámetro Nro
	// (si no se recibe el Nro, no se incluye el campo en el comando)
	if (Nro.length() == 0)
		EnviarComandoFiscal(OpCode(CMD_OPEN_DNFH) + FS +
							Caracter (Id) + FS +
							Caracter (Estacion));
	else
		EnviarComandoFiscal(OpCode(CMD_OPEN_DNFH) + FS +
							Caracter (Id) + FS +
							Caracter (Estacion) + FS +
							Cadena (Nro, DNFHNumSize));

	// Actualizamos 'FirstDocNum' con el número de DNFH abierto.
	FirstDocNum = RespuestaInt (2, "Abrir DNFH");

	// Reinicializamos 'NumberOfPages' y 'TicketBCAbierto'
	NumberOfPages	= 0;
	TicketBCAbierto = false;
}


//
// Cerrar DNFH
//
unsigned
ImpresorFiscalP715::CerrarDNFH (unsigned Copias) throw (Excepcion)
{
	// printf ("Comando CerrarDNFH ejecutado en P715\n");

	// Verificamos que la cantidad de copias sea 1 (ya que
	// los modelos de 8 bits no manejan copias de DNFH)
	if (Copias != 1)
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_PARAMETRO_NO_VALIDO, "Cerrar DNFH");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_CLOSE_DNFH));

	// Recuperamos el Número de DNFH recién emitido de la respuesta
	unsigned NumeroDNFHRecienEmitido = RespuestaInt(2, "Cerrar DNFH");

	// Reinicializamos 'FirstDocNum' y 'NumberOfPages' en 0 y
	// 'TicketBCAbierto' en false.
	FirstDocNum		= 0;
	NumberOfPages	= 0;
	TicketBCAbierto	= false;

	// Retornamos el Número de DF recién emitido
	return NumeroDNFHRecienEmitido;
}


//
// Imprimir Voucher de Tarjeta
//
void
ImpresorFiscalP715::ImprimirVoucher (const std::string &NombreCliente,
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
	// printf ("Comando ImprimirVoucher ejecutado en P715\n");

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

