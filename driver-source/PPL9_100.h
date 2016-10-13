#ifndef IMPRESOR_FISCAL_PPL9_100_H
#define IMPRESOR_FISCAL_PPL9_100_H

#include "ImpresorFiscal16Bits.h"

class ImpresorFiscalPPL9_100 : public ImpresorFiscal16Bits
{
public:

	// Constructor
        ImpresorFiscalPPL9_100();

	// String de Versión
	static std::string Version;

	// Métodos locales
	std::string DescripcionModelo () const;

	// Métodos generales
	void ObtenerDatosMemoriaDeTrabajo (RTA_ObtenerDatosMemoriaDeTrabajo *R = NULL) throw (Excepcion);

	// Métodos de configuración

	// Métodos de reportes

	// Métodos de documentos fiscales
	void EspecificarIVANoInscripto (double Monto) throw (Excepcion);
	unsigned CerrarDF (unsigned Copias = 1) throw (Excepcion);

	// Métodos de documentos no fiscales
	void AbrirDNF (TiposDeEstacion EstacionDeImpresion) throw (Excepcion);

	// Métodos de documentos no fiscales homologados
	unsigned CerrarDNFH (unsigned Copias = 1) throw (Excepcion);

protected:

}; 

#endif


