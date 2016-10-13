#ifndef IMPRESOR_FISCAL_PPL8_201_H
#define IMPRESOR_FISCAL_PPL8_201_H

#include "PPL8.h"

class ImpresorFiscalPPL8_201 : public ImpresorFiscalPPL8
{
public:

	// Constructor
	ImpresorFiscalPPL8_201();

	// String de Versi�n
	static std::string Version;

	// M�todos locales
	std::string DescripcionModelo () const;

	// M�todos generales
	void ObtenerDatosMemoriaDeTrabajo (RTA_ObtenerDatosMemoriaDeTrabajo *R = NULL) throw (Excepcion);

	// M�todos de configuraci�n

	// M�todos de reportes

	// M�todos de documentos fiscales
	unsigned CerrarDF (unsigned Copias = 1) throw (Excepcion);

	// M�todos de documentos no fiscales

	// M�todos de documentos no fiscales homologados
	unsigned CerrarDNFH (unsigned Copias = 1) throw (Excepcion);

protected:

}; 

#endif


