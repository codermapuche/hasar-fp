#include "ImpresorFiscal8Bits.h"


// ############################################################ //
//																//
//			M E T O D O S      L O C A L E S					//
//																//
// ############################################################ //

//
// Constructor
//
ImpresorFiscal8Bits::ImpresorFiscal8Bits () :
										ImpresorFiscal(PROTOCOLO_ORIGINAL)
{
	// Inicialización de variables de tamaño de campos
	PriceDecimals					= 2;
	CodeIngBrSize					= 20;
	VouchCustNameSize				= 30;
	VouchCardNameSize				= 20;
	VouchVendNameSize				= 20;

	// Cargamos los DF soportados
	DFSoportados.insert (TICKET_C);

	// Cargamos los DNFH soportados

}


//
// Indicador de Impuestos Internos
// Calcula el valor del indicador de Impuestos Internos de
// una venta en base a los parámetros 'Precio', 'IVA' e
// 'ImpuestosInternos' y a las preopiedades 'KIVA', 'IIFijo',
// 'IIFijoPorMonto' y 'PrecioBase'.
//
std::string
ImpresorFiscal8Bits::IndicadorII (double ImpuestosInternos, double Precio, double IVA) throw(Excepcion)
{
	switch (ModalidadII)
	{
	case II_FIJO_KIVA:
		// Pasamos K directamente y le agregamos el prefijo '+'
		return (Caracter (II_FIJO_KIVA) + Numero (ImpuestosInternos, 8));
	case II_FIJO_MONTO:
		double K, PB;

		if (ModalidadPrecio == MODO_PRECIO_BASE)
		{
			// Si la venta es por PrecioBase con II Fijo por Monto
			// se debe cumplir que el PB sea distinto de 0
			if (Precio == 0.0)
				throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_PARAMETRO_NO_VALIDO, "IndicadorII");

			K = (Precio / (Precio + ImpuestosInternos));
		}
		else
		{
			// Si la venta es por PrecioTotal con II Fijo por Monto
			// se debe cumplir que el PF sea mayor que el monto de
			// II y que el IVA sea mayor que 0
			if (Precio <= ImpuestosInternos || IVA < 0.0)
				throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_PARAMETRO_NO_VALIDO, "IndicadorII");

			PB = ((Precio - ImpuestosInternos) / (1 + IVA/100));
			K = (PB / (PB + ImpuestosInternos));
		}
		// Pasamos el K calculado y le agregamos el prefijo '+'
		return (Caracter (II_FIJO_KIVA) + Numero (K, 8));
	case II_VARIABLE_KIVA:
		// Pasamos el K directamente sin prefijo
		return (Numero (ImpuestosInternos, 8));
	case II_VARIABLE_PORCENTUAL:
	default:
		// Pasamos K = (1 / (1 + II/100)) sin prefijo
		return (Numero (1 / (1 + ImpuestosInternos/100), 8));
	}
}


// ############################################################ //
//																//
//			M E T O D O S   G E N E R A L E S					//
//																//
// ############################################################ //

//
// Obtener Datos de Inicialización
//
// Nota: este método no es válido para los modelos P614 y P950.
//
void
ImpresorFiscal8Bits::ObtenerDatosDeInicializacion (RTA_ObtenerDatosDeInicializacion *R) throw (Excepcion)
{
	// printf ("Comando ObtenerDatosDeInicializacion ejecutado en ImpresorFiscal8Bits\n");

	// Ejecutamos el método de la superclase 'ImpresorFiscal'
	ImpresorFiscal::ObtenerDatosDeInicializacion (R);

	// Completamos los parámetros correspondientes a los campos
	// de respuesta particulares para los modelos de 8 Bits.
	if (R != NULL)
	{
		R->CodIngBrutos		= RespuestaString (7, "Obtener Datos de Inicialización");
		R->FechaIniActividades	= FECHA (RespuestaString (8, "Obtener Datos de Inicialización"));
	}
}


//
// Baja Fiscal
//
// Nota: esta implementación es válida para todos los modelos
//		 de 8 bits previos al 615 inclusive, en los cuales no
//		 era necesario identificar el número de serie. Por lo
//		 tanto, los restantes modelos de 8 bits tendrán que
//		 tener sus propias implementaciones (715, PR5 y 272).
//
void
ImpresorFiscal8Bits::BajaFiscal (const std::string &NumeroDeSerie) throw(Excepcion)
{
	// printf ("Comando BajaFiscal ejecutado en ImpresorFiscal8Bits\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_KILL_EPROM));
}


//
// Cambiar Código de Ingresos Brutos
//
// Nota: este método solo es válido para todos los modelos
//		 posteriores al 715 (715, PR5 y 272).
//
void
ImpresorFiscal8Bits::CambiarCodigoIngresosBrutos (const std::string & /* CodigoIngBr */) throw(Excepcion)
{
	// printf ("Comando CambiarCodigoIngresosBrutos ejecutado en ImpresorFiscal8Bits\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Cambiar Código de Ingresos Brutos");
}


//
// Cambiar Fecha de Inicio de Actividades
//
void
ImpresorFiscal8Bits::CambiarFechaInicioActividades (const FECHA & /* Fecha */) throw(Excepcion)
{
	// printf ("Comando CambiarFechaInicioActividades ejecutado en ImpresorFiscal8Bits\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Cambiar Fecha de Inicio de Actividades");
}


//
// Estado del Controlador Fiscal
//
// Nota: este método no es válido para los modelos
//               P614_100, P950_100 y P262_100.
//
ImpresorFiscal::TiposDeEstadoControlador
ImpresorFiscal8Bits::EstadoControlador () throw(Excepcion)
{
	// printf ("Comando EstadoControlador ejecutado en ImpresorFiscal8Bits\n");

	return EstadoCF ();
}


//
// Descripción del Estado del Controlador Fiscal
//
// Nota: este método solo no es válido para los modelos
//		 P614, P950 y P262.
//
std::string
ImpresorFiscal8Bits::DescripcionEstadoControlador () throw(Excepcion)
{
	// printf ("Comando DescripcionEstadoControlador ejecutado en ImpresorFiscal8Bits\n");

	return DescripcionEstadoCF ();
}


//
// Documento en Curso
//
// Nota: este método solo es válido para todos los modelos
//		 posteriores al 715 (715, PR5 y 272).
//
ImpresorFiscal::TiposDeDocumento
ImpresorFiscal8Bits::DocumentoEnCurso () throw(Excepcion)
{
	// printf ("Comando DocumentoEnCurso ejecutado en ImpresorFiscal8Bits\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Documento en Curso");
}


//
// Descripción del Documento en Curso
//
// Nota: este método solo es válido para todos los modelos
//		 posteriores al 715 (715, PR5 y 272).
//
std::string
ImpresorFiscal8Bits::DescripcionDocumentoEnCurso () throw(Excepcion)
{
	// printf ("Comando DescripcionDocumentoEnCurso ejecutado en ImpresorFiscal8Bits\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Descripción del Documento en Curso");
}


//
// Ultimo Documento fue Cancelado
//
// Nota: este método solo es válido para todos los modelos
//		 posteriores al 715 (715, PR5 y 272).
//
bool
ImpresorFiscal8Bits::UltimoDocumentoFueCancelado () throw(Excepcion)
{
	// printf ("Comando UltimoDocumentoFueCancelado ejecutado en ImpresorFiscal8Bits\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Ultimo Documento fue Cancelado");
}


//
// Ultimo Documento Fiscal Tipo A
//
unsigned
ImpresorFiscal8Bits::UltimoDocumentoFiscalA () throw(Excepcion)
{
	// printf ("Comando UltimoDocumentoFiscalA ejecutado en ImpresorFiscal8Bits\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Ultimo Documento Fiscal Tipo A");
}


//
// Ultima Nota de Crédito Tipo BC
//
unsigned
ImpresorFiscal8Bits::UltimaNotaCreditoBC () throw(Excepcion)
{
	// printf ("Comando UltimaNotaCreditoBC ejecutado en ImpresorFiscal8Bits\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Ultima Nota de Crédito Tipo BC");
}


//
// Ultima Nota de Crédito Tipo A
//
unsigned
ImpresorFiscal8Bits::UltimaNotaCreditoA () throw(Excepcion)
{
	// printf ("Comando UltimaNotaCreditoA ejecutado en ImpresorFiscal8Bits\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Ultima Nota de Crédito Tipo A");
}


//
// Ultimo Remito
//
unsigned
ImpresorFiscal8Bits::UltimoRemito () throw(Excepcion)
{
	// printf ("Comando UltimoRemito ejecutado en ImpresorFiscal8Bits\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Ultimo Remito");
}


//
// Primer Número de Documento Actual
//
// Nota: este método solo es válido para todos los modelos
//		 posteriores al 715 (715, PR5 y 272).
//
unsigned
ImpresorFiscal8Bits::PrimerNumeroDeDocumentoActual () throw(Excepcion)
{
	// printf ("Comando PrimerNumeroDeDocumentoActual ejecutado en ImpresorFiscal8Bits\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Primer Número de Documento Actual");
}


//
// Abrir Cajón de Dinero
//
void
ImpresorFiscal8Bits::AbrirCajon () throw(Excepcion)
{
	// printf ("Comando AbrirCajon ejecutado en ImpresorFiscal8Bits\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_OPEN_DRAWER));
}


//
// Avanzar papel en estación especificada
//
void
ImpresorFiscal8Bits::AvanzarPapel(TiposDePapel Papel, unsigned Lineas) throw (Excepcion)
{
	unsigned Comando;

	// printf ("Comando AvanzarPapel ejecutado en ImpresorFiscal8Bits\n");

	switch (Papel)
	{
		case PAPEL_TICKET_Y_DIARIO:
			Comando = CMD_FEED_RECEIPT_JOURNAL;
			break;
		case PAPEL_DIARIO:
			Comando = CMD_FEED_JOURNAL;
			break;
		case PAPEL_TICKET:
		default:
			Comando = CMD_FEED_RECEIPT;
			break;
	}

	// Verificamos que la cantidad de líneas no supere
	// el máximo valor aceptado por el CF
	if (Lineas > 99)
		Lineas = 99;

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(ImpresorFiscal::OpCodes(Comando)) + FS + Numero(Lineas));
}


//
// Cargar Datos del Cliente
//
void
ImpresorFiscal8Bits::DatosCliente (
								const std::string & /* RazonSocial */,
								const std::string & /* NroDocumento */,
								TiposDeDocumentoCliente  /* TipoDocumento */,
								TiposDeResponsabilidadesCliente  /* ResponsabilidadIVA */,
								const std::string & /* Direccion */) throw(Excepcion)
{
	// printf ("Comando DatosCliente ejecutado en ImpresorFiscal8Bits\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Cargar Datos del Cliente");
}


//
// Cancelar Comprobante Fiscal
//
// Nota: Elegimos como implementación default para los modelos
//		 de 8 bits a la que envía el comando de Pago para
//		 cancelar. Los modelos que no utilicen esta opción
//		 deberán implementar su propia opción (715, PR5 y 272).
//
void
ImpresorFiscal8Bits::CancelarComprobanteFiscal () throw(Excepcion)
{
	// printf ("Comando CancelarComprobanteFiscal ejecutado en ImpresorFiscal8Bits\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_TOTAL_TENDER) + FS +
						Cadena (" ", 1)    + FS +
						Numero (0.0, 2) + FS +
						Caracter ('C')  + FS +
						Caracter (ImpresorFiscal::DISPLAY_NO));

	// Reinicializamos 'FirstDocNum' y 'NumberOfPages' en 0 y
	// 'TicketBCAbierto' en false.
	FirstDocNum		= 0;
	NumberOfPages	= 0;
	TicketBCAbierto	= false;
}


//
// Cancelar Comprobante
//
// Nota: este método solo es válido para todos los modelos
//		 posteriores al 715 (715, PR5 y 272).
//
void
ImpresorFiscal8Bits::CancelarComprobante () throw(Excepcion)
{
	// printf ("Comando CancelarComprobante ejecutado en ImpresorFiscal8Bits\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Cancelar Comprobante");
}


//
// Información de características soportadas por las
// diferentes familias de controladores fiscales.
//

// Capacidad de impresión de documentos en estación Slip
void
ImpresorFiscal8Bits::CapacidadDocumentoSlip (bool &SoportaFA, bool &SoportaNC)
{
	// printf ("Comando CapacidadDocumentoSlip ejecutado en ImpresorFiscal8Bits\n");

	SoportaFA = false;
	SoportaNC = false;
}


// Capacidad de impresión de documentos en estación Ticket
void
ImpresorFiscal8Bits::CapacidadDocumentoTicket (bool &SoportaFA, bool &SoportaNC)
{
	// printf ("Comando CapacidadDocumentoTicket ejecutado en ImpresorFiscal8Bits\n");

	SoportaFA = false;
	SoportaNC = false;
}


// Capacidad de impresión en estación Slip / Ticket
void
ImpresorFiscal8Bits::CapacidadEstacion (bool &SoportaTicket, bool &SoportaSlip)
{
	// printf ("Comando CapacidadEstacion ejecutado en ImpresorFiscal8Bits\n");

	SoportaTicket = true;
	SoportaSlip = false;
}


// Capacidad de manejo y soporte de Cajón de Dinero
void
ImpresorFiscal8Bits::CapacidadCajon (bool &SoportaCajon)
{
	// printf ("Comando CapacidadCajon ejecutado en ImpresorFiscal8Bits\n");

	SoportaCajon = true;
}


// ############################################################ //
//																//
//			M E T O D O S   D E    C O N F I G U R A C I O N	//
//																//
// ############################################################ //

//
// Obtener Línea de Nombre de Fantasía
//
// Nota: este método solo es válido para todos los modelos
//		 posteriores al 715 (715, PR5 y 272).
//
std::string
ImpresorFiscal8Bits::ObtenerNombreDeFantasia (unsigned /* Linea */) throw (Excepcion)
{
	// printf ("Comando ObtenerNombreDeFantasia ejecutado en ImpresorFiscal8Bits\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Obtener Nombre de Fantasía");
}


//
// Especificar Información Línea de Remito / Comprobante Original
//
// Nota: este método solo es válido para los modelos 715 y PR5.
//
void
ImpresorFiscal8Bits::EspecificarInformacionRemitoComprobanteOriginal (
									unsigned /* Linea */,
									const std::string & /* Texto */) throw (Excepcion)
{
	// printf ("Comando EspecificarInformacionRemitoComprobanteOriginal ejecutado en ImpresorFiscal8Bits\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Especificar Información Línea de Remito / Comprobante Original");
}


//
// Obtener Información Línea de Remito / Comprobante Original
//
// Nota: este método solo es válido para los modelos 715 y PR5.
//
std::string
ImpresorFiscal8Bits::ObtenerInformacionRemitoComprobanteOriginal (
									unsigned /* Linea */) throw (Excepcion)
{
	// printf ("Comando ObtenerInformacionRemitoComprobanteOriginal ejecutado en ImpresorFiscal8Bits\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Obtener Información Línea de Remito / Comprobante Original");
}


//
// Borrar Información Línea de Remito / Comprobante Original
//
void
ImpresorFiscal8Bits::BorrarInformacionRemitoComprobanteOriginal () throw (Excepcion)
{
	// printf ("Comando BorrarInformacionRemitoComprobanteOriginal ejecutado en ImpresorFiscal8Bits\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Borrar Información Línea de Remito / Comprobante Original");
}


//
// Obtener Configuracion del Controlador
//
// Nota: este método no es válido para los modelos
//		 P614 y P950.
//
void
ImpresorFiscal8Bits::ObtenerConfiguracion (RTA_ObtenerConfiguracion *R) throw (Excepcion)
{
	// printf ("Comando ObtenerConfiguracion ejecutado en ImpresorFiscal8Bits\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_GET_CONFIGURATION_DATA));

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
		R->ImprimeMarco					= false;
		R->ReImprimeDocumentos			= true;
		R->DescripcionDelMedioDePago	= "";
		R->Sonido						= true;
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
// Reporte de Cierre Z Individual por Fecha
//
// Nota: este método no es válido solo para los modelos
//		 de 8 bits P614 y P950.
//
void
ImpresorFiscal8Bits::ReporteZIndividualPorFecha (FECHA FechaZ, RTA_ReporteZIndividual *R) throw (Excepcion)
{
	// printf ("Comando ReporteZIndividualPorFecha ejecutado en ImpresorFiscal8Bits\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_GET_DAILY_REPORT) + FS +
						FechaZ.ToString() + FS + "F");

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
	}
}


//
// Reporte de Cierre Z Individual por Número
//
// Nota: este método no es válido solo para los modelos
//		 de 8 bits P614 y P950.
//
void
ImpresorFiscal8Bits::ReporteZIndividualPorNumero (unsigned NumeroZ, RTA_ReporteZIndividual *R) throw (Excepcion)
{
	// printf ("Comando ReporteZIndividualPorNumero ejecutado en ImpresorFiscal8Bits\n");

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
	}
}


// ############################################################ //
//																//
//			M E T O D O S   D E    D F							//
//																//
// ############################################################ //

//
// Especificar Monto de IVA No Inscripto
//
void
ImpresorFiscal8Bits::EspecificarIVANoInscripto (double /* Monto */) throw (Excepcion)
{
	// printf ("Comando EspecificarIVANoInscripto ejecutado en ImpresorFiscal8Bits\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Especificar Monto de IVA No Inscripto");
}


//
// Especificar Percepción Global
//
void
ImpresorFiscal8Bits::EspecificarPercepcionGlobal (
									const std::string & /* Nombre */,
									double /* Monto */) throw (Excepcion)
{
	// printf ("Comando EspecificarPercepcionGlobal ejecutado en ImpresorFiscal8Bits\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Especificar Percepción Global");
}


//
// Especificar Percepción por IVA
//
void
ImpresorFiscal8Bits::EspecificarPercepcionPorIVA (
									const std::string & /* Nombre */,
									double /* Monto */,
									double /* IVA */) throw (Excepcion)
{
	// printf ("Comando EspecificarPercepcionPorIVA ejecutado en ImpresorFiscal8Bits\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Especificar Percepción por IVA");
}


//
// Cerrar DF
//
unsigned
ImpresorFiscal8Bits::CerrarDF (unsigned Copias) throw (Excepcion)
{
	// printf ("Comando CerrarDF ejecutado en ImpresorFiscal8Bits\n");

	// Verificamos que la cantidad de copias sea 1 (ya que
	// los modelos de 8 bits no manejan copias de DF)
	if (Copias != 1)
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_PARAMETRO_NO_VALIDO, "Cerrar DF");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_CLOSE_FISCAL_RECEIPT));

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
ImpresorFiscal8Bits::CerrarDNF (unsigned Copias) throw (Excepcion)
{
	// printf ("Comando CerrarDNF ejecutado en ImpresorFiscal8Bits\n");

	// Verificamos que la cantidad de copias sea 1 (ya que
	// los modelos de 8 bits no manejan copias de DNF)
	if (Copias != 1)
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_PARAMETRO_NO_VALIDO, "Cerrar DNF");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_CLOSE_NON_FISCAL));
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
// Nota: este método solo es válido para los modelos 715 y PR5.
//
void
ImpresorFiscal8Bits::AbrirDNFH (DocumentosNoFiscalesHomologados /* Tipo */,
								const std::string & /* Nro */) throw (Excepcion)
{
	// printf ("Comando AbrirDNFH ejecutado en ImpresorFiscal8Bits\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Abrir DNFH");
}


//
// Cerrar DNFH
//
unsigned
ImpresorFiscal8Bits::CerrarDNFH (unsigned /* Copias */) throw (Excepcion)
{
	// printf ("Comando CerrarDNFH ejecutado en ImpresorFiscal8Bits\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Cerrar DNFH");
}


//
// Emitir un DNFH de Farmacia
//
// Nota: este método solo no es válido para los modelos
//		 P614 y P950.
//
void
ImpresorFiscal8Bits::DNFHFarmacia (unsigned Copias) throw (Excepcion)
{
	// printf ("Comando DNFHFarmacia ejecutado en ImpresorFiscal8Bits\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_DNFH_FARMACIAS) + FS + Numero (Copias));
}


//
// Emitir un DNFH de Reparto
//
// Nota: este método solo no es válido para los modelos
//		 P614 y P950.
//
void
ImpresorFiscal8Bits::DNFHReparto (unsigned Copias) throw (Excepcion)
{
	// printf ("Comando DNFHReparto ejecutado en ImpresorFiscal8Bits\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_DNFH_REPARTO) + FS + Numero (Copias));
}


//
// Imprimir Voucher de Tarjeta
//
// Nota: este método solo no es válido para los modelos
//		 P614 y P950.
//
void
ImpresorFiscal8Bits::ImprimirVoucher (
							const std::string & /* NombreCliente */,
							const std::string & /* NombreTarjeta */,
							TiposDeVouchers /* TipoVoucher */,
							const std::string & /* NumeroDeTarjeta */,
							FECHA /* FechaDeVencimiento */,
							TiposDeTarjetas /* TipoTarjeta */,
							unsigned /* Cuotas */,
							const std::string & /* CodigoDeComercio */,
							unsigned /* NumeroDeTerminal */,
							unsigned /* NumeroDeLote */,
							unsigned /* NumeroCupon */,
							TiposDeIngresoDeTarjeta /* TipoIngreso */,
							TiposOperacionDeTarjeta /* TipoOperacion */,
							unsigned /* NumeroAutorizacion */,
							const std::string & /* Monto */,
							const std::string & /* NumeroComprobanteAsociado */,
							unsigned /* Copias */,
							const std::string & /* Vendedor */,
							TiposDeEstacion /* Estacion */) throw (Excepcion)
{
	// printf ("Comando ImprimirVoucher ejecutado en ImpresorFiscal8Bits\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Imprimir Voucher de Tarjeta");
}

