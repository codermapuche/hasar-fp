#ifndef IMPRESOR_FISCAL_P441_201_H
#define IMPRESOR_FISCAL_P441_201_H

#include "P441.h"

class ImpresorFiscalP441_201 : public ImpresorFiscalP441
{
public:

	// Constructor
	ImpresorFiscalP441_201();

	// String de Versi�n
	static std::string Version;

	// M�todos locales
	std::string DescripcionModelo () const;

	// M�todos generales

	// M�todos de configuraci�n
	void ObtenerConfiguracion (RTA_ObtenerConfiguracion *R = NULL) throw (Excepcion);

	// M�todos de documentos fiscales

	// M�todos de documentos no fiscales

	// M�todos de documentos no fiscales homologados

protected:

};

#endif


