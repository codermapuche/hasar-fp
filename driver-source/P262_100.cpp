#include "P262_100.h"


std::string ImpresorFiscalP262_100::Version ("SMH/PT-262F - Versión 1.00");


// ############################################################ //
//																//
//			M E T O D O S      L O C A L E S					//
//																//
// ############################################################ //

//
// Constructor
//
ImpresorFiscalP262_100::ImpresorFiscalP262_100 ()
{
	// Inicialización de variables de uso general
	MaxFiscalTextLinesInvoice		= 3;	// Por seguridad !!!
	MaxFiscalTextLinesTicket		= 3;

	// Inicialización de variables de tamaño de campos
	PrintNonFiscalTextTicketSize	= 48;
	PrintNonFiscalTextSlipSize		= 48;	// Por seguridad !!!
	FantasyNameSize					= 48;
	HeaderTrailerSize				= 48;
	HeaderTrailerTicketSize			= 48;
	PrintFiscalTextTicketSize		= 30;
	PrintFiscalTextSlipSize			= 30;	// Por seguridad !!!
	PrintItemTextTicketSize			= 28;
	PrintItemTextSlipSize			= 28;	// Por seguridad !!!
	TotalTenderTextSize				= 30;
	VouchImportSize					= 12;
	VouchCompNumSize				= 8;
}


//
// Obtener la Descripción del Modelo Seleccionado
//
std::string
ImpresorFiscalP262_100::DescripcionModelo () const
{
	return 	(Version);
}


// ############################################################ //
//																//
//			M E T O D O S   G E N E R A L E S					//
//																//
// ############################################################ //

//
// Estado del Controlador Fiscal
//
// Nota: este método no es válido para este modelo.
//
ImpresorFiscal::TiposDeEstadoControlador
ImpresorFiscalP262_100::EstadoControlador () throw(Excepcion)
{
    // printf ("Comando EstadoControlador ejecutado en P262_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Estado del Controlador Fiscal");
}


//
// Descripción del Estado del Controlador Fiscal
//
// Nota: este método no es válido para este modelo.
//
std::string 
ImpresorFiscalP262_100::DescripcionEstadoControlador () throw(Excepcion)
{
    // printf ("Comando DescripcionEstadoControlador ejecutado en P262_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Descripción del Estado del Controlador Fiscal");
}


//
// Cortar Papel
//
void
ImpresorFiscalP262_100::CortarPapel () throw (Excepcion)
{
    // printf ("Comando CortarPapel ejecutado en P262_100\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_CUT_NON_FISCAL_RECEIPT));
}


// ############################################################ //
//																//
//			M E T O D O S   D E    C O N F I G U R A C I O N	//
//																//
// ############################################################ //

//
// Configurar Controlador por Bloque de Parámetros
//
// Nota: este método no es válido para este modelo.
//
void
ImpresorFiscalP262_100::ConfigurarControladorPorBloque (
							double /* LimiteConsumidorFinal */,
							double /* LimiteTicketFactura */,
							double /* PorcentajeIVANoInscripto */,
							NumerosDeCopias /* TipoDeCopiasMaximo */,
							bool /* ImpresionCambio */,
							bool /* ImpresionLeyendasOpcionales */,
							TiposDeCorteDePapel /* TipoDeCorte */) throw (Excepcion)
{
    // printf ("Comando ConfigurarControladorPorBloque ejecutado en P262_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Configurar Controlador por Bloque de Parámetros");
}


// ############################################################ //
//																//
//			M E T O D O S   D E    R E P O R T E S				//
//																//
// ############################################################ //



// ############################################################ //
//																//
//			M E T O D O S   D E    D F							//
//																//
// ############################################################ //



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
ImpresorFiscalP262_100::AbrirDNF (TiposDeEstacion EstacionDeImpresion) throw (Excepcion)
{
    // printf ("Comando AbrirDNF ejecutado en P262_100\n");

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
ImpresorFiscalP262_100::ImprimirVoucher (const std::string &NombreCliente,
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
    // printf ("Comando ImprimirVoucher ejecutado en P262_100\n");

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

