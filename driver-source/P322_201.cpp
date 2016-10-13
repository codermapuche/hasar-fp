#include "P322_201.h"


std::string ImpresorFiscalP322_201::Version ("HASAR SMH/P-322F V: 02.01" /* " - 22/01/2004" */);


// ############################################################ //
//																//
//			M E T O D O S      L O C A L E S					//
//																//
// ############################################################ //

//
// Constructor
//
ImpresorFiscalP322_201::ImpresorFiscalP322_201 ()
{
	// Inicialización de variables de uso general

	// Inicialización de variables de tamaño de campos

	// Cargamos los DF soportados

	// Cargamos los DNFH soportados
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


//
// Obtener la Descripción del Modelo Seleccionado
//
std::string
ImpresorFiscalP322_201::DescripcionModelo () const
{
	return 	(Version);
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
ImpresorFiscalP322_201::ObtenerDatosMemoriaDeTrabajo (RTA_ObtenerDatosMemoriaDeTrabajo *R) throw (Excepcion)
{
	// printf ("Comando ObtenerDatosMemoriaDeTrabajo ejecutado en P322_201\n");

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
// Cerrar DF
//
unsigned
ImpresorFiscalP322_201::CerrarDF (unsigned Copias) throw (Excepcion)
{
	// printf ("Comando CerrarDF ejecutado en P322_201\n");

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
// Cerrar DNF
//
void
ImpresorFiscalP322_201::CerrarDNF (unsigned Copias) throw (Excepcion)
{
	// printf ("Comando CerrarDNF ejecutado en P322_201\n");

	// Ejecutamos el método de la superclase 'ImpresorFiscal16Bits'.
	ImpresorFiscal16Bits::CerrarDNF (Copias);
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
ImpresorFiscalP322_201::CerrarDNFH (unsigned Copias) throw (Excepcion)
{
	// printf ("Comando CerrarDNFH ejecutado en P322_201\n");

	// Ejecutamos el método de la superclase 'ImpresorFiscal16Bits'
	unsigned NumeroDNFHRecienEmitido = ImpresorFiscal16Bits::CerrarDNFH (Copias);

	// Recuperamos el Número de Hojas Numeradas del DNFH
	// recién emitido de la respuesta.
	NumberOfPages = RespuestaInt(3, "Cerrar DNFH");

	// Retornamos el Número de DNFH recién emitido
	return NumeroDNFHRecienEmitido;
}


