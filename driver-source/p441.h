#ifndef IMPRESOR_FISCAL_P441_H
#define IMPRESOR_FISCAL_P441_H

#include "ImpresorFiscal16Bits.h"

class ImpresorFiscalP441 : public ImpresorFiscal16Bits
{
public:

	// Constructor
	ImpresorFiscalP441();

	// Métodos locales
	void EstablecerModalidadResumenIVA (TiposDeResumenIVA Modo);

	// Métodos generales
	void ObtenerDatosMemoriaDeTrabajo (RTA_ObtenerDatosMemoriaDeTrabajo *R = NULL) throw (Excepcion);

	unsigned UltimoRemito () throw(Excepcion);

	unsigned PaginasDeUltimoDocumento () throw(Excepcion);
	void CAIUltimoDocumentoA (RTA_CAIUltimoDocumentoA *R = NULL) throw (Excepcion);

	void AbrirCajon () throw(Excepcion);
	void AvanzarPapel(TiposDePapel Papel, unsigned Lineas) throw (Excepcion);

	void DatosCliente (	const std::string &RazonSocial,
						const std::string &NroDocumento,
						TiposDeDocumentoCliente TipoDocumento,
						TiposDeResponsabilidadesCliente ResponsabilidadIVA,
						const std::string &Direccion = "") throw(Excepcion);

	void CargarLogo (const std::string &ArchivoBMP = "") throw(Excepcion);

	void CapacidadDocumentoSlip (bool &SoportaFA, bool &SoportaNC);
	void CapacidadDocumentoTicket (bool &SoportaFA, bool &SoportaNC);
	void CapacidadEstacion (bool &SoportaTicket, bool &SoportaSlip);
	void CapacidadCajon (bool &SoportaCajon);

	// Métodos de configuración
	void BorrarFantasiaEncabezadoCola (bool BorrarFantasia, bool BorrarEncabezado, bool BorrarCola) throw (Excepcion);
	void BorrarInformacionRemitoComprobanteOriginal () throw (Excepcion);

	virtual void ObtenerConfiguracion (RTA_ObtenerConfiguracion *R = NULL) throw (Excepcion);

	// Métodos de reportes
	void ReporteZ (RTA_ReporteZX *R = NULL) throw (Excepcion);
	void ReporteX (RTA_ReporteZX *R = NULL) throw (Excepcion);
	void ReporteZIndividualPorFecha (FECHA FechaZ, RTA_ReporteZIndividual *R = NULL) throw (Excepcion);
	void ReporteZIndividualPorNumero (unsigned NumeroZ, RTA_ReporteZIndividual *R = NULL) throw (Excepcion);

	// Métodos de documentos fiscales
	void EspecificarIVANoInscripto (double Monto) throw (Excepcion);
	double ImprimirPago (const std::string &Texto, double Monto, const std::string &DescripcionAdicional = "") throw (Excepcion);
	double DevolverPago (const std::string &Texto, double Monto, const std::string &DescripcionAdicional = "") throw (Excepcion);

	// Métodos de documentos no fiscales
	void AbrirDNF (TiposDeEstacion EstacionDeImpresion) throw (Excepcion);

	// Métodos de documentos no fiscales homologados
	void ImprimirItemEnRemito (const std::string &Texto, double Cantidad) throw (Excepcion);
	void ImprimirItemEnCuenta (const FECHA &FechaComprobante,
							const std::string &NroComprobante,
							const std::string &Texto,
							double Debe, double Haber) throw (Excepcion);
	void ImprimirItemEnCotizacion (const std::string &Texto) throw (Excepcion);
	void ImprimirInfoEnDNFH (const unsigned IDCampo, const std::string &Texto, double Cantidad = 0.0) throw (Excepcion);
	void ImprimirFirmaEnCierreDNFH (bool ImprimeFirma) throw (Excepcion);

protected:

};

#endif


