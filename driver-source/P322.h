#ifndef IMPRESOR_FISCAL_P322_H
#define IMPRESOR_FISCAL_P322_H

#include "ImpresorFiscal16Bits.h"

class ImpresorFiscalP322 : public ImpresorFiscal16Bits
{
public:

	// Constructor
	ImpresorFiscalP322();

	// M�todos locales

	// M�todos generales

	// M�todos de configuraci�n
	void ObtenerConfiguracion (RTA_ObtenerConfiguracion *R = NULL) throw (Excepcion);

	// M�todos de reportes

	// M�todos de documentos fiscales

	// M�todos de documentos no fiscales
	void AbrirDNF (TiposDeEstacion EstacionDeImpresion) throw (Excepcion);

	// M�todos de documentos no fiscales homologados

protected:

}; 

#endif


