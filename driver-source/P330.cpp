#include "P330.h"



// ############################################################ //
//																//
//			M E T O D O S      L O C A L E S					//
//																//
// ############################################################ //

//
// Constructor
//
ImpresorFiscalP330::ImpresorFiscalP330 ()
{
	// Inicialización de variables de uso general

	// Inicialización de variables de tamaño de campos
	PrintNonFiscalTextTicketSize	= 120;	// Por seguridad !!!	
	PrintFiscalTextTicketSize		= 50;	// Por seguridad !!!
	PrintItemTextTicketSize			= 50;	// Por seguridad !!!
	RemitoCantDecimals				= 10;
	ReciboTextTicketSize			= 106;	// Por seguridad !!!

	// Cargamos los DF soportados
	DFSoportados.insert (FACTURA_A);
	DFSoportados.insert (FACTURA_B);
	DFSoportados.insert (NOTA_DEBITO_A);
	DFSoportados.insert (NOTA_DEBITO_B);
	DFSoportados.insert (RECIBO_A);
	DFSoportados.insert (RECIBO_B);

	// Cargamos los DNFH soportados
	DNFHSoportados.insert (NOTA_CREDITO_A);
	DNFHSoportados.insert (NOTA_CREDITO_B);
	DNFHSoportados.insert (REMITO);
	DNFHSoportados.insert (RECIBO_X);
	DNFHSoportados.insert (ORDEN_SALIDA);
	DNFHSoportados.insert (RESUMEN_CUENTA);
	DNFHSoportados.insert (CARGO_HABITACION);
	DNFHSoportados.insert (COTIZACION);
	DNFHSoportados.insert (CLAUSULA_CREDITO);
	DNFHSoportados.insert (CLAUSULA_SEGURO);
	DNFHSoportados.insert (PAGARE);
	DNFHSoportados.insert (POLIZA_SEGURO);
	DNFHSoportados.insert (RECORDATORIO);
	DNFHSoportados.insert (SOLICITUD_CREDITO);
	DNFHSoportados.insert (COMUNICACION_CLIENTE);
	DNFHSoportados.insert (OFRECIMIENTO_CREDITO);
	DNFHSoportados.insert (OFRECIMIENTO_TARJETA);
	DNFHSoportados.insert (MINUTA_CREDITO);
	DNFHSoportados.insert (OFRECIMIENTO_PASAPORTE);
	DNFHSoportados.insert (RENOVACION_CREDITO);
	DNFHSoportados.insert (ADELANTO_REMUNERACION);
	DNFHSoportados.insert (SOLICITUD_TARJETA_DEBITO);
	DNFHSoportados.insert (SOLICITUD_CLAVE_TARJETA);
	DNFHSoportados.insert (RESCATE_MERCADERIA);
	DNFHSoportados.insert (INGRESO_EGRESO_SUCURSAL);
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
ImpresorFiscalP330::ObtenerDatosMemoriaDeTrabajo (RTA_ObtenerDatosMemoriaDeTrabajo *R) throw (Excepcion)
{
	// printf ("Comando ObtenerDatosMemoriaDeTrabajo ejecutado en P330\n");

	// Ejecutamos el método de la superclase 'ImpresorFiscal16Bits'
	ImpresorFiscal16Bits::ObtenerDatosMemoriaDeTrabajo (R);

	// Completamos los parámetros correspondientes a los campos
	// de respuesta particulares que se agregan en este modelo
	// más alla de los comunes a todos los modelos
	if (R != NULL)
		R->CantidadNCCanceladas			= RespuestaInt (20, "Obtener Datos de Memoria de Trabajo");
}


// ############################################################ //
//																//
//			M E T O D O S   D E    C O N F I G U R A C I O N	//
//																//
// ############################################################ //

//
// Obtener Configuracion del Controlador
//
void
ImpresorFiscalP330::ObtenerConfiguracion (RTA_ObtenerConfiguracion *R) throw (Excepcion)
{
	// printf ("Comando ObtenerConfiguracion ejecutado en P330\n");

	// Ejecutamos el método de la superclase 'ImpresorFiscal16Bits'
	ImpresorFiscal16Bits::ObtenerConfiguracion (R);

	// Completamos los parámetros correspondientes a los campos
	// de respuesta particulares que se agregan en este modelo
	// más alla de los comunes a todos los modelos
	if (R != NULL)
		R->AltoHoja					= static_cast<ImpresorFiscal::TiposDeAltoHoja>(RespuestaString (13, "Obtener Configuracion del Controlador") [0]);
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

//
// Especificar Monto de IVA No Inscripto
//
void
ImpresorFiscalP330::EspecificarIVANoInscripto (double /* Monto */) throw (Excepcion)
{
	// printf ("Comando EspecificarIVANoInscripto ejecutado en P330\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Especificar Monto de IVA No Inscripto");
}


//
// Cerrar DF
//
unsigned
ImpresorFiscalP330::CerrarDF (unsigned Copias) throw (Excepcion)
{
	// printf ("Comando CerrarDF ejecutado en P330\n");

	// Ejecutamos el método de la superclase 'ImpresorFiscal16Bits'
	unsigned NumeroDFRecienEmitido = ImpresorFiscal16Bits::CerrarDF (Copias);

	// Recuperamos el Número de Hojas Numeradas del DF
	// recién emitido de la respuesta.
	NumberOfPages = RespuestaInt(3, "Cerrar DF");

	// Retornamos el Número de DF recién emitido
	return NumeroDFRecienEmitido;
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
ImpresorFiscalP330::AbrirDNF (TiposDeEstacion EstacionDeImpresion) throw (Excepcion)
{
	// printf ("Comando AbrirDNF ejecutado en P330\n");

	// Ejecutamos el método de la SuperClase 'ImpresorFiscal'.
	ImpresorFiscal::AbrirDNF (ESTACION_SLIP);
}


// ############################################################ //
//																//
//			M E T O D O S   D E    D N F H						//
//																//
// ############################################################ //

//
// Cerrar DNFH
//
unsigned
ImpresorFiscalP330::CerrarDNFH (unsigned Copias) throw (Excepcion)
{
	// printf ("Comando CerrarDNFH ejecutado en P330\n");

	// Ejecutamos el método de la superclase 'ImpresorFiscal16Bits'
	unsigned NumeroDNFHRecienEmitido = ImpresorFiscal16Bits::CerrarDNFH (Copias);

	// Recuperamos el Número de Hojas Numeradas del DNFH
	// recién emitido de la respuesta.
	NumberOfPages = RespuestaInt(3, "Cerrar DNFH");

	// Retornamos el Número de DNFH recién emitido
	return NumeroDNFHRecienEmitido;
}


