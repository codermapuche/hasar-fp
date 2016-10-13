#ifndef IMPRESOR_FISCAL_P425_201_H
#define IMPRESOR_FISCAL_P425_201_H

#include "P425.h"

class ImpresorFiscalP425_201 : public ImpresorFiscalP425
{
public:

	// Constructor
	ImpresorFiscalP425_201();

	// String de Versión
	static std::string Version;

	// Métodos locales
	std::string DescripcionModelo () const;

	// Métodos generales
	void ObtenerDatosMemoriaDeTrabajo (RTA_ObtenerDatosMemoriaDeTrabajo *R = NULL) throw (Excepcion);

	// Métodos de configuración

	// Métodos de reportes

	// Métodos de documentos fiscales
	unsigned CerrarDF (unsigned Copias = 1) throw (Excepcion);

	// Métodos de documentos no fiscales

	// Métodos de documentos no fiscales homologados
	unsigned CerrarDNFH (unsigned Copias = 1) throw (Excepcion);

protected:

}; 

#endif
