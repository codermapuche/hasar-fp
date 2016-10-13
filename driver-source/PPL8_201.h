#ifndef IMPRESOR_FISCAL_PPL8_201_H
#define IMPRESOR_FISCAL_PPL8_201_H

#include "PPL8.h"

class ImpresorFiscalPPL8_201 : public ImpresorFiscalPPL8
{
public:

	// Constructor
	ImpresorFiscalPPL8_201();

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


