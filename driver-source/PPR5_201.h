#ifndef IMPRESOR_FISCAL_PPR5_201_H
#define IMPRESOR_FISCAL_PPR5_201_H

#include "PPR5.h"

class ImpresorFiscalPPR5_201 : public ImpresorFiscalPPR5
{
public:

	// Constructor
	ImpresorFiscalPPR5_201();

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

	void ConfigurarControladorPorBloque (
						double LimiteConsumidorFinal,
						double LimiteTicketFactura,
						double PorcentajeIVANoInscripto,
						NumerosDeCopias TipoDeCopiasMaximo,
						bool ImpresionCambio,
						bool ImpresionLeyendasOpcionales,
						TiposDeCorteDePapel TipoDeCorte) throw (Excepcion);
	void ObtenerConfiguracion (RTA_ObtenerConfiguracion *R = NULL) throw (Excepcion);

	// M�todos de reportes
	void ReporteZ (RTA_ReporteZX *R = NULL) throw (Excepcion);
	void ReporteX (RTA_ReporteZX *R = NULL) throw (Excepcion);

	// M�todos de documentos fiscales
	double ImprimirPago (const std::string &Texto, double Monto, const std::string &DescripcionAdicional = "") throw (Excepcion);
	double DevolverPago (const std::string &Texto, double Monto, const std::string &DescripcionAdicional = "") throw (Excepcion);

	// M�todos de documentos no fiscales

	// M�todos de documentos no fiscales homologados
	void AbrirDNFH (DocumentosNoFiscalesHomologados Tipo, const std::string &Nro = "") throw (Excepcion);

protected:

};

#endif


