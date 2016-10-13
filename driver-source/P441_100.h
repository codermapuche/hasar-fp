#ifndef IMPRESOR_FISCAL_P441_100_H
#define IMPRESOR_FISCAL_P441_100_H

#include "P441.h"

class ImpresorFiscalP441_100 : public ImpresorFiscalP441
{
public:

	// Constructor
	ImpresorFiscalP441_100();

	// String de Versión
	static std::string Version;

	// Métodos locales
	std::string DescripcionModelo () const;

	// Métodos generales

	// Métodos de configuración

	// Métodos de documentos fiscales

	// Métodos de documentos no fiscales

	// Métodos de documentos no fiscales homologados

protected:

};

#endif


