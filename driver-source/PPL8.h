#ifndef IMPRESOR_FISCAL_PPL8_H
#define IMPRESOR_FISCAL_PPL8_H

#include "ImpresorFiscal16Bits.h"

class ImpresorFiscalPPL8 : public ImpresorFiscal16Bits
{
public:

	// Constructor
	ImpresorFiscalPPL8();

	// Métodos locales

	// Métodos generales

	// Métodos de configuración

	// Métodos de reportes

	// Métodos de documentos fiscales

	// Métodos de documentos no fiscales
	void AbrirDNF (TiposDeEstacion EstacionDeImpresion) throw (Excepcion);

	// Métodos de documentos no fiscales homologados

protected:

}; 

#endif


