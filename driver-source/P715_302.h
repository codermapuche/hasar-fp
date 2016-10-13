#ifndef IMPRESOR_FISCAL_P715_302_H
#define IMPRESOR_FISCAL_P715_302_H

#include "P715.h"

class ImpresorFiscalP715_302 : public ImpresorFiscalP715
{
public:

	// Constructor
	ImpresorFiscalP715_302();

	// String de Versión
	static std::string Version;

	// Métodos locales
	std::string DescripcionModelo () const;
	void EstablecerModalidadResumenIVA (TiposDeResumenIVA Modo);

	// Métodos generales
	void ObtenerDatosMemoriaDeTrabajo (RTA_ObtenerDatosMemoriaDeTrabajo *R = NULL) throw (Excepcion);

	void CambiarFechaInicioActividades (const FECHA &Fecha) throw(Excepcion);

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
	double ImprimirPago (const std::string &Texto, double Monto, const std::string &DescripcionAdicional = "") throw (Excepcion);
	double DevolverPago (const std::string &Texto, double Monto, const std::string &DescripcionAdicional = "") throw (Excepcion);

	// Métodos de documentos no fiscales

	// Métodos de documentos no fiscales homologados
	void DetalleRecibo (const std::string &Texto = " ") throw (Excepcion);
	void ImprimirInfoEnDNFH (const unsigned IDCampo, const std::string &Texto, double Cantidad = 0.0) throw (Excepcion);

protected:

};

#endif


