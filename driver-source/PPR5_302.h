#ifndef IMPRESOR_FISCAL_PPR5_302_H
#define IMPRESOR_FISCAL_PPR5_302_H

#include "PPR5.h"

class ImpresorFiscalPPR5_302 : public ImpresorFiscalPPR5
{
public:

	// Constructor
	ImpresorFiscalPPR5_302();

	// String de Versi�n
	static std::string Version;

	// M�todos locales
	std::string DescripcionModelo () const;
	void EstablecerModalidadResumenIVA (TiposDeResumenIVA Modo);

	// M�todos generales
	void ObtenerDatosMemoriaDeTrabajo (RTA_ObtenerDatosMemoriaDeTrabajo *R = NULL) throw (Excepcion);

	void DatosCliente (	const std::string &RazonSocial,
						const std::string &NroDocumento,
						TiposDeDocumentoCliente TipoDocumento,
						TiposDeResponsabilidadesCliente ResponsabilidadIVA,
						const std::string &Direccion = "") throw(Excepcion);

	// M�todos de configuraci�n
	void BorrarFantasiaEncabezadoCola (bool BorrarFantasia, bool BorrarEncabezado, bool BorrarCola) throw (Excepcion);
	void BorrarInformacionRemitoComprobanteOriginal () throw (Excepcion);

	void ObtenerConfiguracion (RTA_ObtenerConfiguracion *R = NULL) throw (Excepcion);

	// M�todos de reportes
	void ReporteZ (RTA_ReporteZX *R = NULL) throw (Excepcion);
	void ReporteX (RTA_ReporteZX *R = NULL) throw (Excepcion);

	// M�todos de documentos fiscales
	double ImprimirPago (const std::string &Texto, double Monto, const std::string &DescripcionAdicional = "") throw (Excepcion);
	double DevolverPago (const std::string &Texto, double Monto, const std::string &DescripcionAdicional = "") throw (Excepcion);

	// M�todos de documentos no fiscales

	// M�todos de documentos no fiscales homologados
	void DetalleRecibo (const std::string &Texto = " ") throw (Excepcion);
	void ImprimirInfoEnDNFH (const unsigned IDCampo, const std::string &Texto, double Cantidad = 0.0) throw (Excepcion);
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


