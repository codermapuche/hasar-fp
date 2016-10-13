#ifndef IMPRESOR_FISCAL_PPL23_H
#define IMPRESOR_FISCAL_PPL23_H

#include "ImpresorFiscal16Bits.h"

class ImpresorFiscalPPL23 : public ImpresorFiscal16Bits
{
public:

	// Constructor
	ImpresorFiscalPPL23();

	// Métodos locales
	void EstablecerModalidadResumenIVA (TiposDeResumenIVA Modo);

	// Métodos generales
	void ObtenerDatosMemoriaDeTrabajo (RTA_ObtenerDatosMemoriaDeTrabajo *R = NULL) throw (Excepcion);

	void EstadoInternoImpresor (RTA_EstadoInternoImpresor *R = NULL) throw (Excepcion);

	void EstablecerInterlineado (const unsigned LineasPorPulgada) throw (Excepcion);
	unsigned ObtenerInterlineado () throw (Excepcion);
	void ApagarIndicadorDeTambor () throw (Excepcion);

	// Métodos de configuración
	void BorrarFantasiaEncabezadoCola (bool BorrarFantasia, bool BorrarEncabezado, bool BorrarCola) throw (Excepcion);
	void BorrarInformacionRemitoComprobanteOriginal () throw (Excepcion);

	void ObtenerConfiguracion (RTA_ObtenerConfiguracion *R = NULL) throw (Excepcion);

	// Métodos de reportes
	void ReporteZ (RTA_ReporteZX *R = NULL) throw (Excepcion);
	void ReporteX (RTA_ReporteZX *R = NULL) throw (Excepcion);

	// Métodos de documentos fiscales
	void EspecificarIVANoInscripto (double Monto) throw (Excepcion);
	double DevolverPago (const std::string &Texto, double Monto, const std::string &DescripcionAdicional = "") throw (Excepcion);
	unsigned CerrarDF (unsigned Copias = 1) throw (Excepcion);

	// Métodos de documentos no fiscales
	void AbrirDNF (TiposDeEstacion EstacionDeImpresion) throw (Excepcion);

	// Métodos de documentos no fiscales homologados
	unsigned CerrarDNFH (unsigned Copias = 1) throw (Excepcion);

protected:

};

#endif


