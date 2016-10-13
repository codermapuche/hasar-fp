#ifndef IMPRESOR_FISCAL_PPL9_100_H
#define IMPRESOR_FISCAL_PPL9_100_H

#include "ImpresorFiscal16Bits.h"

class ImpresorFiscalPPL9_100 : public ImpresorFiscal16Bits
{
public:

	// Constructor
        ImpresorFiscalPPL9_100();

	// String de Versi�n
	static std::string Version;

	// M�todos locales
	std::string DescripcionModelo () const;

	// M�todos generales
	void ObtenerDatosMemoriaDeTrabajo (RTA_ObtenerDatosMemoriaDeTrabajo *R = NULL) throw (Excepcion);

	// M�todos de configuraci�n

	// M�todos de reportes

	// M�todos de documentos fiscales
	void EspecificarIVANoInscripto (double Monto) throw (Excepcion);
	unsigned CerrarDF (unsigned Copias = 1) throw (Excepcion);

	// M�todos de documentos no fiscales
	void AbrirDNF (TiposDeEstacion EstacionDeImpresion) throw (Excepcion);

	// M�todos de documentos no fiscales homologados
	unsigned CerrarDNFH (unsigned Copias = 1) throw (Excepcion);

protected:

}; 

#endif


