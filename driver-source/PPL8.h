#ifndef IMPRESOR_FISCAL_PPL8_H
#define IMPRESOR_FISCAL_PPL8_H

#include "ImpresorFiscal16Bits.h"

class ImpresorFiscalPPL8 : public ImpresorFiscal16Bits
{
public:

	// Constructor
	ImpresorFiscalPPL8();

	// M�todos locales

	// M�todos generales

	// M�todos de configuraci�n

	// M�todos de reportes

	// M�todos de documentos fiscales

	// M�todos de documentos no fiscales
	void AbrirDNF (TiposDeEstacion EstacionDeImpresion) throw (Excepcion);

	// M�todos de documentos no fiscales homologados

protected:

}; 

#endif


