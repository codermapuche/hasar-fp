#ifndef IMPRESOR_FISCAL_P435_H
#define IMPRESOR_FISCAL_P435_H

#include "ImpresorFiscal16Bits.h"

class ImpresorFiscalP435 : public ImpresorFiscal16Bits
{
public:

	// Constructor
	ImpresorFiscalP435();

	// M�todos locales

	// M�todos generales
	void ObtenerDatosMemoriaDeTrabajo (RTA_ObtenerDatosMemoriaDeTrabajo *R = NULL) throw (Excepcion);

	void AbrirCajon () throw(Excepcion);
	void AvanzarPapel(TiposDePapel Papel, unsigned Lineas) throw (Excepcion);

	void CapacidadDocumentoTicket (bool &SoportaFA, bool &SoportaNC);
	void CapacidadEstacion (bool &SoportaTicket, bool &SoportaSlip);
	void CapacidadCajon (bool &SoportaCajon);

	// M�todos de configuraci�n
	void ObtenerConfiguracion (RTA_ObtenerConfiguracion *R = NULL) throw (Excepcion);

	// M�todos de reportes

	// M�todos de documentos fiscales
	void EspecificarIVANoInscripto (double Monto) throw (Excepcion);
	unsigned CerrarDF (unsigned Copias = 1) throw (Excepcion);

	// M�todos de documentos no fiscales

	// M�todos de documentos no fiscales homologados
	unsigned CerrarDNFH (unsigned Copias = 1) throw (Excepcion);

protected:

}; 

#endif


