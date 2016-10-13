#include "P441_201.h"


std::string ImpresorFiscalP441_201::Version ("HASAR SMH/P-441F V: 02.01" /* " - 23/04/2013" */);


// ############################################################ //
//																//
//			M E T O D O S      L O C A L E S					//
//																//
// ############################################################ //

//
// Constructor
//
ImpresorFiscalP441_201::ImpresorFiscalP441_201 ()
{
	// Inicializaci�n de variables de uso general

	// Inicializaci�n de variables de tama�o de campos

	// Cargamos los DF soportados

	// Cargamos los DNFH soportados
	DNFHSoportados.insert (COMUNICACION_CLIENTE);
}


//
// Obtener la Descripci�n del Modelo Seleccionado
//
std::string
ImpresorFiscalP441_201::DescripcionModelo () const
{
	return 	(Version);
}



// ############################################################ //
//																//
//			M E T O D O S   G E N E R A L E S					//
//																//
// ############################################################ //



// ############################################################ //
//																//
//			M E T O D O S   D E    C O N F I G U R A C I O N	//
//																//
// ############################################################ //


//
// Obtener Configuracion del Controlador
//
void
ImpresorFiscalP441_201::ObtenerConfiguracion (RTA_ObtenerConfiguracion *R) throw (Excepcion)
{
	// printf ("Comando ObtenerConfiguracion ejecutado en P441_201\n");

	// Ejecutamos el m�todo de la superclase 'ImpresorFiscal16Bits'
	ImpresorFiscal16Bits::ObtenerConfiguracion (R);

	// Completamos los par�metros correspondientes a los campos
	// de respuesta particulares que se agregan en este modelo
	// m�s alla de los comunes a todos los modelos
	if (R != NULL)
	{
		R->ImprimeIDCopiaDNFH	= ((RespuestaString (19, "Obtener Configuracion del Controlador") [0]) == 'P');
		R->ImprimeColorNegro	= ((RespuestaString (20, "Obtener Configuracion del Controlador") [0]) == 'P');
	}
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



// ############################################################ //
//																//
//			M E T O D O S   D E    D N F H						//
//																//
// ############################################################ //



