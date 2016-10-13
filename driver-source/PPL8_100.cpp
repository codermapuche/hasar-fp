#include "PPL8_100.h"


std::string ImpresorFiscalPPL8_100::Version ("HASAR SMH/PL-8F v1.00" /* " - 18/12/2000" */);


// ############################################################ //
//																//
//			M E T O D O S      L O C A L E S					//
//																//
// ############################################################ //

//
// Constructor
//
ImpresorFiscalPPL8_100::ImpresorFiscalPPL8_100 ()
{
	// Inicialización de variables de uso general

	// Inicialización de variables de tamaño de campos
	RemitoCantDecimals				= 4;

	// Cargamos los DF soportados

	// Cargamos los DNFH soportados

}


//
// Obtener la Descripción del Modelo Seleccionado
//
std::string
ImpresorFiscalPPL8_100::DescripcionModelo () const
{
	return 	(Version);
}


// ############################################################ //
//																//
//			M E T O D O S   G E N E R A L E S					//
//																//
// ############################################################ //

//
// Baja Fiscal
//
void
ImpresorFiscalPPL8_100::BajaFiscal (const std::string &NumeroDeSerie) throw(Excepcion)
{
	// printf ("Comando BajaFiscal ejecutado en PPL8_100\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_KILL_EPROM));
}


//
// Cambiar Fecha de Inicio de Actividades
//
void
ImpresorFiscalPPL8_100::CambiarFechaInicioActividades (const FECHA & /* Fecha */) throw(Excepcion)
{
	// printf ("Comando CambiarFechaInicioActividades ejecutado en PPL8_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Cambiar Fecha de Inicio de Actividades");
}


//
// Páginas de Ultimo Documento
//
// Nota: este método no es válido para este modelo.
//
unsigned
ImpresorFiscalPPL8_100::PaginasDeUltimoDocumento () throw(Excepcion)
{
	// printf ("Comando PaginasDeUltimoDocumento ejecutado en PPL8_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Páginas de Ultimo Documento");
}


//
// CAI del Ultimo Documento A
//
// Nota: este método no es válido para este modelo.
//
void
ImpresorFiscalPPL8_100::CAIUltimoDocumentoA (RTA_CAIUltimoDocumentoA * /* R */) throw (Excepcion)
{
	// printf ("Comando CAIUltimoDocumentoA ejecutado en PPL8_100\n");
	
	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "CAI del Ultimo Documento A");
}


// ############################################################ //
//																//
//			M E T O D O S   D E    C O N F I G U R A C I O N	//
//																//
// ############################################################ //



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

//
// Subtotal
//
void
ImpresorFiscalPPL8_100::Subtotal (bool Imprime, RTA_Subtotal *R) throw (Excepcion)
{
	// printf ("Comando Subtotal ejecutado en PPL8_100\n");
	
	// Ejecutamos el método de la superclase 'ImpresorFiscal'
	ImpresorFiscal::Subtotal (Imprime, R);

	// Completamos los parámetros correspondientes a los campos
	// de respuesta particulares que se agregan en este modelo
	// más alla de los comunes a todos los modelos
	if (R != NULL)
		R->MontoIVANoInscripto		= RespuestaDouble (6, "Subtotal");
}


//
// Cerrar DF
//
unsigned
ImpresorFiscalPPL8_100::CerrarDF (unsigned Copias) throw (Excepcion)
{
	// printf ("Comando CerrarDF ejecutado en PPL8_100\n");

	// Verificamos que la cantidad de copias sea 1 (este
	// modelo no soporta la indicación de copias en el
	// comando de cierre).
	if (Copias != 1)
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_PARAMETRO_NO_VALIDO, "Cerrar DF");

	// Ejecutamos el método de la superclase 'ImpresorFiscal16Bits'.
	return ImpresorFiscal16Bits::CerrarDF (Copias);
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
ImpresorFiscalPPL8_100::CerrarDNF (unsigned Copias) throw (Excepcion)
{
	// printf ("Comando CerrarDNF ejecutado en PPL8_100\n");

	// Verificamos que la cantidad de copias sea 1 (este
	// modelo no soporta la indicación de copias en el
	// comando de cierre).
	if (Copias != 1)
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_PARAMETRO_NO_VALIDO, "Cerrar DF");

	// Ejecutamos el método de la superclase 'ImpresorFiscal16Bits'.
	ImpresorFiscal16Bits::CerrarDNF (Copias);
}


// ############################################################ //
//																//
//			M E T O D O S   D E    D N F H						//
//																//
// ############################################################ //

//
// Imprimir Información en DNFH
//
// Nota: esta método no es válido para este modelo.
//
void
ImpresorFiscalPPL8_100::ImprimirInfoEnDNFH (const unsigned /* IDCampo */,
											const std::string & /* Texto */,
											double /* Cantidad */) throw (Excepcion)
{
	// printf ("Comando ImprimirInfoEnDNFH ejecutado en PPL8_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Imprimir Información en DNFH");
}


//
// Cerrar DNFH
//
unsigned
ImpresorFiscalPPL8_100::CerrarDNFH (unsigned Copias) throw (Excepcion)
{
	// printf ("Comando CerrarDNFH ejecutado en PPL8_100\n");

	// Verificamos que la cantidad de copias sea 1 (este
	// modelo no soporta la indicación de copias en el
	// comando de cierre).
	if (Copias != 1)
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_PARAMETRO_NO_VALIDO, "Cerrar DF");

	// Ejecutamos el método de la superclase 'ImpresorFiscal16Bits'.
	return ImpresorFiscal16Bits::CerrarDNFH (Copias);
}


//
// Imprimir Voucher de Tarjeta
//
void
ImpresorFiscalPPL8_100::ImprimirVoucher (const std::string & /* NombreCliente */,
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
	// printf ("Comando ImprimirVoucher ejecutado en PPL8_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Imprimir Voucher de Tarjeta");
}


