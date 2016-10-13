#ifndef IMPRESOR_FISCAL_P1120_100_H
#define IMPRESOR_FISCAL_P1120_100_H

#include "ImpresorFiscal16Bits.h"

class ImpresorFiscalP1120_100 : public ImpresorFiscal16Bits
{
public:

	// Constructor
	ImpresorFiscalP1120_100();

	// String de Versi�n
	static std::string Version;

	// M�todos locales
	std::string DescripcionModelo () const;
	void EstablecerModalidadResumenIVA (TiposDeResumenIVA Modo);

	// M�todos generales
	void ObtenerDatosMemoriaDeTrabajo (RTA_ObtenerDatosMemoriaDeTrabajo *R = NULL) throw (Excepcion);

	// M�todos de configuraci�n
	void BorrarFantasiaEncabezadoCola (bool BorrarFantasia, bool BorrarEncabezado, bool BorrarCola) throw (Excepcion);
	void BorrarInformacionRemitoComprobanteOriginal () throw (Excepcion);

	void ObtenerConfiguracion (RTA_ObtenerConfiguracion *R = NULL) throw (Excepcion);

	// M�todos de reportes
	void ReporteZ (RTA_ReporteZX *R = NULL) throw (Excepcion);
	void ReporteX (RTA_ReporteZX *R = NULL) throw (Excepcion);

	// M�todos de documentos fiscales
	void EspecificarIVANoInscripto (double Monto) throw (Excepcion);
	double DevolverPago (const std::string &Texto, double Monto, const std::string &DescripcionAdicional = "") throw (Excepcion);
	unsigned CerrarDF (unsigned Copias = 1) throw (Excepcion);

	// M�todos de documentos no fiscales
	void AbrirDNF (TiposDeEstacion EstacionDeImpresion) throw (Excepcion);

	// M�todos de documentos no fiscales homologados
	unsigned CerrarDNFH (unsigned Copias = 1) throw (Excepcion);

protected:

};

#endif


