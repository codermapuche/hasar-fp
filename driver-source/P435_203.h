#ifndef IMPRESOR_FISCAL_P435_203_H
#define IMPRESOR_FISCAL_P435_203_H

#include "P435.h"

class ImpresorFiscalP435_203 : public ImpresorFiscalP435
{
public:

	// Constructor
	ImpresorFiscalP435_203();

	// String de Versión
	static std::string Version;

	// Métodos locales
	std::string DescripcionModelo () const;
	void EstablecerModalidadResumenIVA (TiposDeResumenIVA Modo);

	// Métodos generales
	void ObtenerDatosMemoriaDeTrabajo (RTA_ObtenerDatosMemoriaDeTrabajo *R = NULL) throw (Excepcion);

	void DatosCliente (	const std::string &RazonSocial,
						const std::string &NroDocumento,
						TiposDeDocumentoCliente TipoDocumento,
						TiposDeResponsabilidadesCliente ResponsabilidadIVA,
						const std::string &Direccion = "") throw(Excepcion);

	// Métodos de configuración
	void BorrarFantasiaEncabezadoCola (bool BorrarFantasia, bool BorrarEncabezado, bool BorrarCola) throw (Excepcion);
	void BorrarInformacionRemitoComprobanteOriginal () throw (Excepcion);

	void ObtenerConfiguracion (RTA_ObtenerConfiguracion *R = NULL) throw (Excepcion);

	// Métodos de reportes
	void ReporteZ (RTA_ReporteZX *R = NULL) throw (Excepcion);
	void ReporteX (RTA_ReporteZX *R = NULL) throw (Excepcion);

	// Métodos de documentos fiscales
	double DevolverPago (const std::string &Texto, double Monto, const std::string &DescripcionAdicional = "") throw (Excepcion);

	// Métodos de documentos no fiscales

	// Métodos de documentos no fiscales homologados
	void ImprimirInfoEnDNFH (const unsigned IDCampo, const std::string &Texto, double Cantidad = 0.0) throw (Excepcion);
	void ImprimirFirmaEnCierreDNFH (bool ImprimeFirma) throw (Excepcion);
	
	void ImprimirVoucher (const std::string &NombreCliente,
							const std::string &NombreTarjeta,
							TiposDeVouchers TipoVoucher,
							const std::string &NumeroDeTarjeta,
							const std::string &FechaDeVencimiento,
							TiposDeTarjetas TipoTarjeta,
							unsigned Cuotas,
							const std::string &CodigoDeComercio,
							unsigned NumeroDeTerminal,
							unsigned NumeroDeLote,
							unsigned NumeroCupon,
							TiposDeIngresoDeTarjeta TipoIngreso,
							TiposOperacionDeTarjeta TipoOperacion,
							unsigned NumeroAutorizacion,
							const std::string &Monto,
							const std::string &NumeroComprobanteAsociado,
							unsigned Copias = 1,
							const std::string &Vendedor = "",
							TiposDeEstacion Estacion = ImpresorFiscal::ESTACION_TICKET) throw (Excepcion);

protected:

};

#endif


