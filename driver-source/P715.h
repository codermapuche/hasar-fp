#ifndef IMPRESOR_FISCAL_P715_H
#define IMPRESOR_FISCAL_P715_H

#include "ImpresorFiscal8Bits.h"

class ImpresorFiscalP715 : public ImpresorFiscal8Bits
{
public:

	// Constructor
	ImpresorFiscalP715();

	// Métodos generales
	std::string ObtenerVersionDelControlador () throw (Excepcion);
	virtual void ObtenerDatosMemoriaDeTrabajo (RTA_ObtenerDatosMemoriaDeTrabajo *R = NULL) throw (Excepcion);

	void BajaFiscal (const std::string &NumeroDeSerie = "") throw(Excepcion);

	void CambiarCodigoIngresosBrutos (const std::string &CodigoIngBr) throw(Excepcion);
	
	TiposDeDocumento DocumentoEnCurso () throw(Excepcion);
	std::string DescripcionDocumentoEnCurso () throw(Excepcion);
	bool UltimoDocumentoFueCancelado () throw(Excepcion);

	unsigned UltimoDocumentoFiscalA () throw(Excepcion);
	unsigned UltimaNotaCreditoBC () throw(Excepcion);
	unsigned UltimaNotaCreditoA () throw(Excepcion);

	unsigned PrimerNumeroDeDocumentoActual () throw(Excepcion);

	void CancelarComprobanteFiscal () throw(Excepcion);
	void CancelarComprobante () throw(Excepcion);

	void CapacidadDocumentoTicket (bool &SoportaFA, bool &SoportaNC);

	// Métodos de configuración
	std::string ObtenerNombreDeFantasia (unsigned Linea) throw (Excepcion);

	void EspecificarInformacionRemitoComprobanteOriginal (unsigned Linea, const std::string &Texto) throw (Excepcion);
	std::string ObtenerInformacionRemitoComprobanteOriginal (unsigned Linea) throw (Excepcion);
	virtual void BorrarInformacionRemitoComprobanteOriginal () throw (Excepcion);

	virtual void ObtenerConfiguracion (RTA_ObtenerConfiguracion *R = NULL) throw (Excepcion);

	// Métodos de reportes
	virtual void ReporteZ (RTA_ReporteZX *R = NULL) throw (Excepcion);
	virtual void ReporteX (RTA_ReporteZX *R = NULL) throw (Excepcion);
	void ReporteZIndividualPorFecha (FECHA FechaZ, RTA_ReporteZIndividual *R = NULL) throw (Excepcion);
	void ReporteZIndividualPorNumero (unsigned NumeroZ, RTA_ReporteZIndividual *R = NULL) throw (Excepcion);

	// Métodos de documentos fiscales
	void AbrirDF (DocumentosFiscales Tipo) throw (Excepcion);
	void EspecificarPercepcionGlobal (const std::string &Nombre, double Monto) throw (Excepcion);
	void EspecificarPercepcionPorIVA (const std::string &Nombre, double Monto, double IVA) throw (Excepcion);
	void Subtotal (bool Imprime, RTA_Subtotal *R = NULL) throw (Excepcion);

	// Métodos de documentos no fiscales
	void AbrirDNF (TiposDeEstacion EstacionDeImpresion) throw (Excepcion);

	// Métodos de documentos no fiscales homologados
	virtual void AbrirDNFH (DocumentosNoFiscalesHomologados Tipo, const std::string &Nro = "") throw (Excepcion);
	unsigned CerrarDNFH (unsigned Copias = 1) throw (Excepcion);
	void ImprimirVoucher (const std::string &NombreCliente,
						const std::string &NombreTarjeta,
						TiposDeVouchers TipoVoucher,
						const std::string &NumeroDeTarjeta,
						FECHA FechaDeVencimiento,
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

