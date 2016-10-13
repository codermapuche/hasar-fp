#include "P951_100.h"


std::string ImpresorFiscalP951_100::Version ("SMH/P-951F - Versión 1.00");


// ############################################################ //
//																//
//			M E T O D O S      L O C A L E S					//
//																//
// ############################################################ //

//
// Constructor
//
ImpresorFiscalP951_100::ImpresorFiscalP951_100 ()
{
	// Inicialización de variables de uso general
	MaxFiscalTextLinesInvoice		= 4;
	MaxFiscalTextLinesTicket		= 3;

	// Inicialización de variables de tamaño de campos
	PrintNonFiscalTextTicketSize	= 40;
	PrintNonFiscalTextSlipSize		= 80;
	FantasyNameSize					= 40;
	HeaderTrailerSize				= 40;
	HeaderTrailerTicketSize			= 40;
	HeaderTrailerSlipSize			= 40;
	RazonSocialSize					= 30;
	PrintFiscalTextTicketSize		= 28;
	PrintFiscalTextSlipSize			= 28;	// Por seguridad !!
	PrintItemTextTicketSize			= 20;
	PrintItemTextSlipSize			= 20;	// Por seguridad !!
	TotalTenderTextSize				= 24;
	VouchImportSize					= 15;
	VouchCompNumSize				= 8;

	// Cargamos los DF soportados
	DFSoportados.insert (TICKET_FACTURA_A);
	DFSoportados.insert (TICKET_FACTURA_B);

	// Cargamos los DNFH soportados

}


//
// Obtener la Descripción del Modelo Seleccionado
//
std::string
ImpresorFiscalP951_100::DescripcionModelo () const
{
	return 	(Version);
}


// ############################################################ //
//																//
//			M E T O D O S   G E N E R A L E S					//
//																//
// ############################################################ //

//
// Ultimo Documento Fiscal Tipo A
//
unsigned
ImpresorFiscalP951_100::UltimoDocumentoFiscalA () throw(Excepcion)
{
    // printf ("Comando UltimoDocumentoFiscalA ejecutado en P951_100\n");

	// Enviamos el comando fiscal
	SolicitarEstado ();

	// Retornamos la información obtenida de la respuesta
	return RespuestaInt(4, "Ultimo Documento Fiscal Tipo A");
}


//
// Cortar Papel
//
void
ImpresorFiscalP951_100::CortarPapel () throw (Excepcion)
{
    // printf ("Comando CortarPapel ejecutado en P951_100\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_CUT_NON_FISCAL_RECEIPT));
}


//
// Cargar Datos del Cliente
//
void
ImpresorFiscalP951_100::DatosCliente (      const std::string &RazonSocial,
									const std::string &NroDocumento,
									TiposDeDocumentoCliente TipoDocumento,
									TiposDeResponsabilidadesCliente ResponsabilidadIVA,
									const std::string &Direccion) throw(Excepcion)
{
    // printf ("Comando DatosCliente ejecutado en P951_100\n");

	assert(RazonSocialSize != 0);

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_SET_CUSTOMER_DATA) + FS +
						Cadena (RazonSocial, RazonSocialSize) + FS +
						Cadena (NroDocumento, 11) + FS +
						Caracter (ResponsabilidadIVA) + FS +
						Caracter (TipoDocumento));
}


// Capacidad de impresión de documentos en estación Ticket
void
ImpresorFiscalP951_100::CapacidadDocumentoTicket (bool &SoportaFA, bool &SoportaNC)
{
    // printf ("Comando CapacidadDocumentoTicket ejecutado en P951_100\n");

	SoportaFA = true;
	SoportaNC = false;
}


// Capacidad de impresión en estación Slip / Ticket
void
ImpresorFiscalP951_100::CapacidadEstacion (bool &SoportaTicket, bool &SoportaSlip)
{
    // printf ("Comando CapacidadEstacion ejecutado en P951_100\n");

	SoportaTicket = true;
	SoportaSlip = true;
}


// ############################################################ //
//																//
//			M E T O D O S   D E    C O N F I G U R A C I O N	//
//																//
// ############################################################ //

//
// Configurar Controlador por Bloque de Parámetros
//
void
ImpresorFiscalP951_100::ConfigurarControladorPorBloque (
							double LimiteConsumidorFinal,
							double LimiteTicketFactura,
							double PorcentajeIVANoInscripto,
							NumerosDeCopias TipoDeCopiasMaximo,
							bool ImpresionCambio,
							bool ImpresionLeyendasOpcionales,
							TiposDeCorteDePapel TipoDeCorte) throw (Excepcion)
{
    // printf ("Comando ConfigurarControladorPorBloque ejecutado en P951_100\n");

	// Si el TipoDeCopiasMaximo es NO_COPIAS lo corregimos a ORIGINAL
	// (NO_COPIAS no es un valor válido para este modelo) ...
	if (TipoDeCopiasMaximo == NO_COPIAS)
		TipoDeCopiasMaximo = ORIGINAL;

	// Ejecutamos el método de la superclase 'ImpresorFiscal'
	ImpresorFiscal::ConfigurarControladorPorBloque (
							LimiteConsumidorFinal,
							LimiteTicketFactura,
							PorcentajeIVANoInscripto,
							TipoDeCopiasMaximo,
							ImpresionCambio,
							ImpresionLeyendasOpcionales,
							TipoDeCorte);
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
ImpresorFiscalP951_100::ReporteZ (RTA_ReporteZX *R) throw (Excepcion)
{
    // printf ("Comando ReporteZ ejecutado en P951_100\n");
	
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
ImpresorFiscalP951_100::ReporteX (RTA_ReporteZX *R) throw (Excepcion)
{
    // printf ("Comando ReporteX ejecutado en P951_100\n");
	
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
// Especificar Monto de IVA No Inscripto
//
void
ImpresorFiscalP951_100::EspecificarIVANoInscripto (double Monto) throw (Excepcion)
{
    // printf ("Comando EspecificarIVANoInscripto ejecutado en P951_100\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_CHARGE_NON_REGISTERED_TAX) + FS +
						Numero (Monto, 2));
}


//
// Especificar Percepción Global
//
void
ImpresorFiscalP951_100::EspecificarPercepcionGlobal (
									const std::string &Nombre,
									double Monto) throw (Excepcion)
{
    // printf ("Comando EspecificarPercepcionGlobal ejecutado en P951_100\n");

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
ImpresorFiscalP951_100::EspecificarPercepcionPorIVA (
									const std::string &Nombre,
									double Monto,
									double IVA) throw (Excepcion)
{
    // printf ("Comando EspecificarPercepcionPorIVA ejecutado en P951_100\n");

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
ImpresorFiscalP951_100::Subtotal (bool Imprime, RTA_Subtotal *R) throw (Excepcion)
{
    // printf ("Comando Subtotal ejecutado en P951_100\n");
	
	// Ejecutamos el método de la superclase 'ImpresorFiscal'
	ImpresorFiscal::Subtotal (Imprime, R);

	// Completamos los parámetros correspondientes a los campos
	// de respuesta particulares que se agregan en este modelo
	// más alla de los comunes a todos los modelos
	if (R != NULL)
		R->MontoIVANoInscripto		= RespuestaDouble (6, "Subtotal");
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
ImpresorFiscalP951_100::AbrirDNF (TiposDeEstacion EstacionDeImpresion) throw (Excepcion)
{
    // printf ("Comando AbrirDNF ejecutado en P951_100\n");

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
ImpresorFiscalP951_100::ImprimirVoucher (const std::string &NombreCliente,
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
    // printf ("Comando ImprimirVoucher ejecutado en P951_100\n");

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
						Cadena (NumeroComprobanteAsociado, VouchCompNumSize));

	// Enviamos el comando fiscal 'PrintVoucher' y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_PRINT_VOUCHER) + FS +
						Numero (Copias));
}

