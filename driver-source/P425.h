#ifndef IMPRESOR_FISCAL_P425_H
#define IMPRESOR_FISCAL_P425_H

#include "ImpresorFiscal16Bits.h"

class ImpresorFiscalP425 : public ImpresorFiscal16Bits
{
public:

	// Constructor
	ImpresorFiscalP425();

	// M�todos locales

	// M�todos generales
	void AbrirCajon () throw(Excepcion);
	void AvanzarPapel(TiposDePapel Papel, unsigned Lineas) throw (Excepcion);

	void CapacidadDocumentoTicket (bool &SoportaFA, bool &SoportaNC);
	void CapacidadEstacion (bool &SoportaTicket, bool &SoportaSlip);
	void CapacidadCajon (bool &SoportaCajon);

	// M�todos de configuraci�n
	void ObtenerConfiguracion (RTA_ObtenerConfiguracion *R = NULL) throw (Excepcion);

	// M�todos de reportes

	// M�todos de documentos fiscales

	// M�todos de documentos no fiscales

	// M�todos de documentos no fiscales homologados

protected:

}; 

#endif
