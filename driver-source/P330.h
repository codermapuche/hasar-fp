#ifndef IMPRESOR_FISCAL_P330_H
#define IMPRESOR_FISCAL_P330_H

#include "ImpresorFiscal16Bits.h"

class ImpresorFiscalP330 : public ImpresorFiscal16Bits
{
public:

	// Constructor
	ImpresorFiscalP330();

	// M�todos locales

	// M�todos generales
	void ObtenerDatosMemoriaDeTrabajo (RTA_ObtenerDatosMemoriaDeTrabajo *R = NULL) throw (Excepcion);

	// M�todos de configuraci�n
	void ObtenerConfiguracion (RTA_ObtenerConfiguracion *R = NULL) throw (Excepcion);

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


