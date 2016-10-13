#ifndef IMPRESOR_FISCAL_P441_201_H
#define IMPRESOR_FISCAL_P441_201_H

#include "P441.h"

class ImpresorFiscalP441_201 : public ImpresorFiscalP441
{
public:

	// Constructor
	ImpresorFiscalP441_201();

	// String de Versión
	static std::string Version;

	// Métodos locales
	std::string DescripcionModelo () const;

	// Métodos generales

	// Métodos de configuración
	void ObtenerConfiguracion (RTA_ObtenerConfiguracion *R = NULL) throw (Excepcion);

	// Métodos de documentos fiscales

	// Métodos de documentos no fiscales

	// Métodos de documentos no fiscales homologados

protected:

};

#endif


