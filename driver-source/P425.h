#ifndef IMPRESOR_FISCAL_P425_H
#define IMPRESOR_FISCAL_P425_H

#include "ImpresorFiscal16Bits.h"

class ImpresorFiscalP425 : public ImpresorFiscal16Bits
{
public:

	// Constructor
	ImpresorFiscalP425();

	// Métodos locales

	// Métodos generales
	void AbrirCajon () throw(Excepcion);
	void AvanzarPapel(TiposDePapel Papel, unsigned Lineas) throw (Excepcion);

	void CapacidadDocumentoTicket (bool &SoportaFA, bool &SoportaNC);
	void CapacidadEstacion (bool &SoportaTicket, bool &SoportaSlip);
	void CapacidadCajon (bool &SoportaCajon);

	// Métodos de configuración
	void ObtenerConfiguracion (RTA_ObtenerConfiguracion *R = NULL) throw (Excepcion);

	// Métodos de reportes

	// Métodos de documentos fiscales

	// Métodos de documentos no fiscales

	// Métodos de documentos no fiscales homologados

protected:

}; 

#endif
