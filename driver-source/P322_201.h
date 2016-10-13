#ifndef IMPRESOR_FISCAL_P322_201_H
#define IMPRESOR_FISCAL_P322_201_H

#include "P322.h"

class ImpresorFiscalP322_201 : public ImpresorFiscalP322
{
public:

	// Constructor
	ImpresorFiscalP322_201();

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
	void CerrarDNF (unsigned Copias = 1) throw (Excepcion);

	// M�todos de documentos no fiscales homologados
	unsigned CerrarDNFH (unsigned Copias = 1) throw (Excepcion);

protected:

}; 

#endif


