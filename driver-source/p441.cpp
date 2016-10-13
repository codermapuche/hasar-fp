#include "P441.h"



// ############################################################ //
//																//
//			M E T O D O S      L O C A L E S					//
//																//
// ############################################################ //

//
// Constructor
//
ImpresorFiscalP441::ImpresorFiscalP441 ()
{
	// Inicialización de variables de uso general

	// Inicialización de variables de tamaño de campos
	PrintNonFiscalTextTicketSize	= 45;
	PrintNonFiscalTextSlipSize		= 0;
	FantasyNameSize					= 45;
	HeaderTrailerSize				= 45;
	HeaderTrailerTicketSize			= 45;
	HeaderTrailerSlipSize			= 0;
	CodeBarMaxSize					= 18;
	RazonSocialSize					= 45;
	DireccionSize					= 45;
	PrintFiscalTextTicketSize		= 31;
	PrintFiscalTextSlipSize			= 0;
	PrintItemTextTicketSize			= 23;
	PrintItemTextSlipSize			= 0;
	TotalTenderTextSize				= 28;
	RemitoTextSize					= 0;
	ResumeTextSize					= 0;
	CotizaTextSize					= 0;
	ReciboTextTicketSize			= 40;
	ReciboTextSlipSize				= 0;
	ReciboLines						= 0;
	PrintDNFHTextSize				= 45;
	VouchCustNameSize				= 45;
	VouchCardNameSize				= 20;

	// Cargamos los DF soportados
	DFSoportados.insert (TICKET_C);
	DFSoportados.insert (TICKET_FACTURA_A);
	DFSoportados.insert (TICKET_FACTURA_B);
	DFSoportados.insert (TICKET_NOTA_DEBITO_A);
	DFSoportados.insert (TICKET_NOTA_DEBITO_B);

	// Cargamos los DNFH soportados
	DNFHSoportados.insert (TICKET_NOTA_CREDITO_A);
	DNFHSoportados.insert (TICKET_NOTA_CREDITO_B);
	DNFHSoportados.insert (TICKET_RECIBO_X);
	DNFHSoportados.insert (TICKET_PAGARE);
	DNFHSoportados.insert (TICKET_PRESUPUESTO);
	DNFHSoportados.insert (TICKET_ENTREGA);
	DNFHSoportados.insert (TICKET_ESTACIONAMIENTO);
	DNFHSoportados.insert (TICKET_COBRO_SERVICIO);
	DNFHSoportados.insert (TICKET_INGRESO_DINERO);
	DNFHSoportados.insert (TICKET_RETIRO_DINERO);
	DNFHSoportados.insert (TICKET_TALON_CAMBIO);
	DNFHSoportados.insert (TICKET_TALON_REPARTO);
	DNFHSoportados.insert (TICKET_TALON_REGALO);
	DNFHSoportados.insert (TICKET_TALON_CUENTA_CORRIENTE);
	DNFHSoportados.insert (TICKET_AVISO_OPERACION_CREDITO);
	DNFHSoportados.insert (TICKET_CUPON_PROMOCION);
	DNFHSoportados.insert (TICKET_USO_INTERNO_FARMACIA);
}


//
// Establecer Modalidad Resumen IVA
//
void
ImpresorFiscalP441::EstablecerModalidadResumenIVA (TiposDeResumenIVA Modo)
{
	ModalidadResumenIVA = Modo;
}


// ############################################################ //
//																//
//			M E T O D O S   G E N E R A L E S					//
//																//
// ############################################################ //

//
// Obtener Datos de Memoria de Trabajo
//
void
ImpresorFiscalP441::ObtenerDatosMemoriaDeTrabajo (RTA_ObtenerDatosMemoriaDeTrabajo *R) throw (Excepcion)
{
	// printf ("Comando ObtenerDatosMemoriaDeTrabajo ejecutado en P441\n");

	// Ejecutamos el método de la superclase 'ImpresorFiscal16Bits'
	ImpresorFiscal16Bits::ObtenerDatosMemoriaDeTrabajo (R);

	// Completamos los parámetros correspondientes a los campos
	// de respuesta particulares que se agregan en este modelo
	// más alla de los comunes a todos los modelos
	if (R != NULL)
	{
		R->CantidadNCCanceladas				= RespuestaInt (20, "Obtener Datos de Memoria de Trabajo");
		R->CantidadDFBCEmitidos				= RespuestaInt (21, "Obtener Datos de Memoria de Trabajo");
		R->CantidadDFAEmitidos				= RespuestaInt (22, "Obtener Datos de Memoria de Trabajo");
		R->CantidadNCBCEmitidas				= RespuestaInt (23, "Obtener Datos de Memoria de Trabajo");
		R->CantidadNCAEmitidas				= RespuestaInt (24, "Obtener Datos de Memoria de Trabajo");
	}
}


//
// Ultimo Remito
//
unsigned
ImpresorFiscalP441::UltimoRemito () throw(Excepcion)
{
	// printf ("Comando UltimoRemito ejecutado en P441\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Ultimo Remito");
}


//
// Páginas de Ultimo Documento
//
unsigned
ImpresorFiscalP441::PaginasDeUltimoDocumento () throw(Excepcion)
{
	// printf ("Comando PaginasDeUltimoDocumento ejecutado en P441\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Páginas de Ultimo Documento");
}


//
// CAI del Ultimo Documento A
//
// Nota: este método solo es válido para aquellos modelos
//		 de 16 bits que soportan este comando.
//
void
ImpresorFiscalP441::CAIUltimoDocumentoA (RTA_CAIUltimoDocumentoA * /* R */) throw (Excepcion)
{
	// printf ("Comando CAIUltimoDocumentoA ejecutado en P441\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "CAI del Ultimo Documento A");
}


//
// Abrir Cajón de Dinero
//
void
ImpresorFiscalP441::AbrirCajon () throw(Excepcion)
{
	// printf ("Comando AbrirCajon ejecutado en P441\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_OPEN_DRAWER));
}


//
// Avanzar papel en estación especificada
//
void
ImpresorFiscalP441::AvanzarPapel(TiposDePapel Papel, unsigned Lineas) throw (Excepcion)
{
	unsigned Comando;

	// printf ("Comando AvanzarPapel ejecutado en P441\n");

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
ImpresorFiscalP441::DatosCliente (
						const std::string &RazonSocial,
						const std::string &NroDocumento,
						TiposDeDocumentoCliente TipoDocumento,
						TiposDeResponsabilidadesCliente ResponsabilidadIVA,
						const std::string &Direccion) throw(Excepcion)
{
	// printf ("Comando DatosCliente ejecutado en P441\n");

	assert(RazonSocialSize != 0);
	assert(DireccionSize != 0);

	// Enviamos el comando fiscal y evaluamos los status

	// Verificamos la existencia del parámetro dirección
	// (si no se recibe la dirección, no se incluye el campo en el comando)
	if (Direccion.length() == 0)
		EnviarComandoFiscal(OpCode(CMD_SET_CUSTOMER_DATA) + FS +
							Cadena (RazonSocial, RazonSocialSize) + FS +
							Cadena (NroDocumento, 11) + FS +
							Caracter (ResponsabilidadIVA) + FS +
							Caracter (TipoDocumento));
	else
		EnviarComandoFiscal(OpCode(CMD_SET_CUSTOMER_DATA) + FS +
							Cadena (RazonSocial, RazonSocialSize) + FS +
							Cadena (NroDocumento, 11) + FS +
							Caracter (ResponsabilidadIVA) + FS +
							Caracter (TipoDocumento) + FS +
							Cadena (Direccion, DireccionSize));
}


//
// Cargar Logo
//
void
ImpresorFiscalP441::CargarLogo (const std::string & /* ArchivoBMP */) throw(Excepcion)
{
	// printf ("Comando CargarLogo ejecutado en P441\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Cargar Logo");
}


//
// Información de características soportadas por las
// diferentes familias de controladores fiscales.
//

// Capacidad de impresión de documentos en estación Slip
void
ImpresorFiscalP441::CapacidadDocumentoSlip (bool &SoportaFA, bool &SoportaNC)
{
	// printf ("Comando CapacidadDocumentoSlip ejecutado en P441\n");

	SoportaFA = false;
	SoportaNC = false;
}


// Capacidad de impresión de documentos en estación Ticket
void
ImpresorFiscalP441::CapacidadDocumentoTicket (bool &SoportaFA, bool &SoportaNC)
{
	// printf ("Comando CapacidadDocumentoTicket ejecutado en P441\n");

	SoportaFA = true;
	SoportaNC = true;
}


// Capacidad de impresión en estación Slip / Ticket
void
ImpresorFiscalP441::CapacidadEstacion (bool &SoportaTicket, bool &SoportaSlip)
{
	// printf ("Comando CapacidadEstacion ejecutado en P441\n");

	SoportaTicket = true;
	SoportaSlip = false;
}


// Capacidad de manejo y soporte de Cajón de Dinero
void
ImpresorFiscalP441::CapacidadCajon (bool &SoportaCajon)
{
	// printf ("Comando CapacidadCajon ejecutado en P441\n");

	SoportaCajon = true;
}


// ############################################################ //
//																//
//			M E T O D O S   D E    C O N F I G U R A C I O N	//
//																//
// ############################################################ //

//
// Borrar Líneas de Nombre de Fantasía, Encabezado y Cola
//
void
ImpresorFiscalP441::BorrarFantasiaEncabezadoCola (bool BorrarFantasia, bool BorrarEncabezado, bool BorrarCola) throw (Excepcion)
{
	// printf ("Comando BorrarFantasiaEncabezadoCola ejecutado en P441\n");

	if ( BorrarFantasia )
		EnviarComandoFiscal(OpCode(CMD_SET_FANTASY_NAME) + FS + "0" + FS + ERASE_LINE);

	if ( BorrarEncabezado )
	{
		if ( BorrarCola )
			EnviarComandoFiscal(OpCode(CMD_SET_HEADER_TRAILER) + FS + "0" + FS + ERASE_LINE);
		else
			EnviarComandoFiscal(OpCode(CMD_SET_HEADER_TRAILER) + FS + "-1" + FS + ERASE_LINE);
	}
	else
		if ( BorrarCola )
			EnviarComandoFiscal(OpCode(CMD_SET_HEADER_TRAILER) + FS + "-2" + FS + ERASE_LINE);
}


//
// Borrar Información Línea de Remito / Comprobante Original
//
void
ImpresorFiscalP441::BorrarInformacionRemitoComprobanteOriginal () throw (Excepcion)
{
	// printf ("Comando BorrarInformacionRemitoComprobanteOriginal ejecutado en P441\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_SET_EMBARK_NUMBER) + FS + "0" + FS + ERASE_LINE);
}


//
// Obtener Configuracion del Controlador
//
void
ImpresorFiscalP441::ObtenerConfiguracion (RTA_ObtenerConfiguracion *R) throw (Excepcion)
{
	// printf ("Comando ObtenerConfiguracion ejecutado en P441\n");

	// Ejecutamos el método de la superclase 'ImpresorFiscal16Bits'
	ImpresorFiscal16Bits::ObtenerConfiguracion (R);

	// Completamos los parámetros correspondientes a los campos
	// de respuesta particulares que se agregan en este modelo
	// más alla de los comunes a todos los modelos
	if (R != NULL)
		R->ChequeoDesbordeCompleto		= ((RespuestaString (17, "Obtener Configuracion del Controlador") [0]) == 'P');
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
ImpresorFiscalP441::ReporteZ (RTA_ReporteZX *R) throw (Excepcion)
{
	// printf ("Comando ReporteZ ejecutado en P441\n");

	// Ejecutamos el método de la superclase 'ImpresorFiscal16Bits'
	ImpresorFiscal16Bits::ReporteZ (R);

	// Completamos los parámetros correspondientes a los campos de
	// respuesta particulares que se agregan en este modelo
	if (R != NULL)
	{
		R->CantidadNCCanceladas				= RespuestaInt (23, "Reporte Z");
		R->CantidadDFBCEmitidos				= RespuestaInt (24, "Reporte Z");
		R->CantidadDFAEmitidos				= RespuestaInt (25, "Reporte Z");
		R->CantidadNCBCEmitidas				= RespuestaInt (26, "Reporte Z");
		R->CantidadNCAEmitidas				= RespuestaInt (27, "Reporte Z");
	}
}


//
// Reporte X
//
void
ImpresorFiscalP441::ReporteX (RTA_ReporteZX *R) throw (Excepcion)
{
	// printf ("Comando ReporteX ejecutado en P441\n");

	// Ejecutamos el método de la superclase 'ImpresorFiscal16Bits'
	ImpresorFiscal16Bits::ReporteX (R);

	// Completamos los parámetros correspondientes a los campos de
	// respuesta particulares que se agregan en este modelo
	if (R != NULL)
	{
		R->CantidadNCCanceladas				= RespuestaInt (23, "Reporte Z");
		R->CantidadDFBCEmitidos				= RespuestaInt (24, "Reporte Z");
		R->CantidadDFAEmitidos				= RespuestaInt (25, "Reporte Z");
		R->CantidadNCBCEmitidas				= RespuestaInt (26, "Reporte Z");
		R->CantidadNCAEmitidas				= RespuestaInt (27, "Reporte Z");
	}
}


//
// Reporte de Cierre Z Individual por Fecha
//
void
ImpresorFiscalP441::ReporteZIndividualPorFecha (FECHA FechaZ, RTA_ReporteZIndividual *R) throw (Excepcion)
{
	// printf ("Comando ReporteZIndividualPorFecha ejecutado en P441\n");

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
		R->UltimoRemito						= 0;
	}
}


//
// Reporte de Cierre Z Individual por Número
//
void
ImpresorFiscalP441::ReporteZIndividualPorNumero (unsigned NumeroZ, RTA_ReporteZIndividual *R) throw (Excepcion)
{
	// printf ("Comando ReporteZIndividualPorNumero ejecutado en P441\n");

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
ImpresorFiscalP441::EspecificarIVANoInscripto (double /* Monto */) throw (Excepcion)
{
	// printf ("Comando EspecificarIVANoInscripto ejecutado en P441\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Especificar Monto de IVA No Inscripto");
}


//
// Imprimir Pago
//
double
ImpresorFiscalP441::ImprimirPago (
						const std::string &Texto,
						double Monto,
						const std::string &DescripcionAdicional) throw (Excepcion)
{
	// printf ("Comando ImprimirPago ejecutado en P441\n");

	assert(TotalTenderTextSize != 0);

	// Enviamos el comando fiscal y evaluamos los status

	// Si no se recibe la descripción adicional, no se incluye el campo en el comando.
	if (DescripcionAdicional.length() == 0)
		EnviarComandoFiscal(OpCode(CMD_TOTAL_TENDER) + FS +
							Cadena (Texto, TotalTenderTextSize) + FS +
							Numero (Monto, 2) + FS + "T" + FS +
							Caracter (ModoDisplay));
	else
		EnviarComandoFiscal(OpCode(CMD_TOTAL_TENDER) + FS +
							Cadena (Texto, TotalTenderTextSize) + FS +
							Numero (Monto, 2) + FS + "T" + FS +
							Caracter (ModoDisplay) + FS +
							Cadena (DescripcionAdicional, TotalTenderTextSize));


	// Retornamos el Vuelto o Monto por Pagar
	return RespuestaDouble (2, "Imprimir Pago");
}


//
// Devolver un pago previamente almacenado
//
double
ImpresorFiscalP441::DevolverPago (
						const std::string &Texto,
						double Monto,
						const std::string &DescripcionAdicional) throw (Excepcion)
{
	// printf ("Comando DevolverPago ejecutado en P441\n");

	assert(TotalTenderTextSize != 0);

	// Enviamos el comando fiscal y evaluamos los status

	// Si no se recibe la descripción adicional, no se incluye el campo en el comando.
	if (DescripcionAdicional.length() == 0)
		EnviarComandoFiscal(OpCode(CMD_TOTAL_TENDER) + FS +
							Cadena (Texto, TotalTenderTextSize) + FS +
							Numero (Monto, 2) + FS + "R" + FS +
							Caracter (ModoDisplay));
	else
		EnviarComandoFiscal(OpCode(CMD_TOTAL_TENDER) + FS +
							Cadena (Texto, TotalTenderTextSize) + FS +
							Numero (Monto, 2) + FS + "R" + FS +
							Caracter (ModoDisplay) + FS +
							Cadena (DescripcionAdicional, TotalTenderTextSize));


	// Retornamos el Vuelto o Monto por Pagar
	return RespuestaDouble (2, "Devolver Pago");
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
ImpresorFiscalP441::AbrirDNF (TiposDeEstacion EstacionDeImpresion) throw (Excepcion)
{
	// printf ("Comando AbrirDNF ejecutado en P441\n");

	// Ejecutamos el método de la SuperClase 'ImpresorFiscal'.
	ImpresorFiscal::AbrirDNF (ESTACION_TICKET);
}


// ############################################################ //
//																//
//			M E T O D O S   D E    D N F H						//
//																//
// ############################################################ //

//
// Imprimir Item en Remito u Orden de Salida
//
void
ImpresorFiscalP441::ImprimirItemEnRemito (const std::string & /* Texto */,
									  double /* Cantidad */) throw (Excepcion)
{
	// printf ("Comando ImprimirItemEnRemito ejecutado en P441\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Imprimir Item en Remito u Orden de Salida");
}


//
// Imprimir Item en Resumen de Cuenta o Cargo a la Habitación
//
void
ImpresorFiscalP441::ImprimirItemEnCuenta (const FECHA & /*FechaComprobante */,
							const std::string & /* NroComprobante */,
							const std::string & /* Texto */,
							double /* Debe */, double /* Haber */) throw (Excepcion)
{
	// printf ("Comando ImprimirItemEnCuenta ejecutado en P441\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Imprimir Item en Resumen de Cuenta o Cargo a la Habitación");
}


//
// Imprimir Item en Cotización
//
void
ImpresorFiscalP441::ImprimirItemEnCotizacion (const std::string & /* Detalle */) throw (Excepcion)
{
	// printf ("Comando ImprimirItemEnCuenta ejecutado en P441\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Imprimir Item en Cotización");
}


//
// Imprimir Información en DNFH
//
void
ImpresorFiscalP441::ImprimirInfoEnDNFH (const unsigned IDCampo,
										  const std::string &Texto,
										  double Cantidad) throw (Excepcion)
{
	// printf ("Comando ImprimirInfoEnDNFH ejecutado en P441\n");

	assert(PrintDNFHTextSize != 0);

	// Enviamos el comando fiscal y evaluamos los status
	if ( Cantidad == 0.0 )
		EnviarComandoFiscal(OpCode(CMD_PRINT_INFO_DNFH) + FS +
							Numero (IDCampo) + FS +
							Cadena (Texto, PrintDNFHTextSize) + FS +
							Caracter (ModoDisplay));
	else
		EnviarComandoFiscal(OpCode(CMD_PRINT_INFO_DNFH) + FS +
							Numero (IDCampo) + FS +
							Cadena (Texto, PrintDNFHTextSize) + FS +
							Caracter (ModoDisplay) + FS +
							Numero (Cantidad, 10));
}


//
// Imprimir Firma en Cierre de DNFH
//
void
ImpresorFiscalP441::ImprimirFirmaEnCierreDNFH (bool ImprimeFirma) throw (Excepcion)
{
	// printf ("Comando ImprimirFirmaEnCierreDNFH ejecutado en P441\n");

	EnviarComandoFiscal(OpCode(CMD_PRINT_SIGN_DNFH) + FS + (ImprimeFirma ? "P" : "N"));
}


