#include "P425_100.h"


std::string ImpresorFiscalP425_100::Version ("HASAR SMH/P-425F v1.00" /* " - 04/06/2001" */);


// ############################################################ //
//																//
//			M E T O D O S      L O C A L E S					//
//																//
// ############################################################ //

//
// Constructor
//
ImpresorFiscalP425_100::ImpresorFiscalP425_100 ()
{
	// Inicializaci�n de variables de uso general

	// Inicializaci�n de variables de tama�o de campos

	// Cargamos los DF soportados

	// Cargamos los DNFH soportados

}


//
// Obtener la Descripci�n del Modelo Seleccionado
//
std::string
ImpresorFiscalP425_100::DescripcionModelo () const
{
	return 	(Version);
}


// ############################################################ //
//																//
//			M E T O D O S   G E N E R A L E S					//
//																//
// ############################################################ //

//
// Cambiar Fecha de Inicio de Actividades
//
void
ImpresorFiscalP425_100::CambiarFechaInicioActividades (const FECHA & /* Fecha */) throw(Excepcion)
{
	// printf ("Comando CambiarFechaInicioActividades ejecutado en P425_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Cambiar Fecha de Inicio de Actividades");
}


//
// P�ginas de Ultimo Documento
//
// Nota: este m�todo no es v�lido para este modelo.
//
unsigned
ImpresorFiscalP425_100::PaginasDeUltimoDocumento () throw(Excepcion)
{
	// printf ("Comando PaginasDeUltimoDocumento ejecutado en P425_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "P�ginas de Ultimo Documento");
}


//
// CAI del Ultimo Documento A
//
// Nota: este m�todo no es v�lido para este modelo.
//
void
ImpresorFiscalP425_100::CAIUltimoDocumentoA (RTA_CAIUltimoDocumentoA * /* R */) throw (Excepcion)
{
	// printf ("Comando CAIUltimoDocumentoA ejecutado en P425_100\n");
	
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
// Cerrar DF
//
unsigned
ImpresorFiscalP425_100::CerrarDF (unsigned Copias) throw (Excepcion)
{
	// printf ("Comando CerrarDF ejecutado en P425_100\n");

	// Verificamos que la cantidad de copias sea 1 (este
	// modelo no soporta la indicaci�n de copias en el
	// comando de cierre).
	if (Copias != 1)
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_PARAMETRO_NO_VALIDO, "Cerrar DF");

	// Ejecutamos el m�todo de la superclase 'ImpresorFiscal16Bits'.
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
ImpresorFiscalP425_100::CerrarDNF (unsigned Copias) throw (Excepcion)
{
	// printf ("Comando CerrarDNF ejecutado en P425_100\n");

	// Verificamos que la cantidad de copias sea 1 (este
	// modelo no soporta la indicaci�n de copias en el
	// comando de cierre).
	if (Copias != 1)
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_PARAMETRO_NO_VALIDO, "Cerrar DF");

	// Ejecutamos el m�todo de la superclase 'ImpresorFiscal16Bits'.
	ImpresorFiscal16Bits::CerrarDNF (Copias);
}


// ############################################################ //
//																//
//			M E T O D O S   D E    D N F H						//
//																//
// ############################################################ //

//
// Imprimir Informaci�n en DNFH
//
// Nota: esta m�todo no es v�lido para este modelo.
//
void
ImpresorFiscalP425_100::ImprimirInfoEnDNFH (
									const unsigned /* IDCampo */,
									const std::string & /* Texto */,
									double /* Cantidad */) throw (Excepcion)
{
	// printf ("Comando ImprimirInfoEnDNFH ejecutado en P425_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Imprimir Informaci�n en DNFH");
}


//
// Cerrar DNFH
//
unsigned
ImpresorFiscalP425_100::CerrarDNFH (unsigned Copias) throw (Excepcion)
{
	// printf ("Comando CerrarDNFH ejecutado en P425_100\n");

	// Verificamos que la cantidad de copias sea 1 (este
	// modelo no soporta la indicaci�n de copias en el
	// comando de cierre).
	if (Copias != 1)
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_PARAMETRO_NO_VALIDO, "Cerrar DF");

	// Ejecutamos el m�todo de la superclase 'ImpresorFiscal16Bits'.
	return ImpresorFiscal16Bits::CerrarDNFH (Copias);
}
