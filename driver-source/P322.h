#ifndef IMPRESOR_FISCAL_P322_H
#define IMPRESOR_FISCAL_P322_H

#include "ImpresorFiscal16Bits.h"

class ImpresorFiscalP322 : public ImpresorFiscal16Bits
{
public:

	// Constructor
	ImpresorFiscalP322();

	// Métodos locales

	// Métodos generales

	// Métodos de configuración
	void ObtenerConfiguracion (RTA_ObtenerConfiguracion *R = NULL) throw (Excepcion);

	// Métodos de reportes

	// Métodos de documentos fiscales

	// Métodos de documentos no fiscales
	void AbrirDNF (TiposDeEstacion EstacionDeImpresion) throw (Excepcion);

	// Métodos de documentos no fiscales homologados

protected:

}; 

#endif


