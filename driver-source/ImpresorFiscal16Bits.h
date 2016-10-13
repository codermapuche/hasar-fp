#ifndef IMPRESOR_FISCAL_16_BITS_H
#define IMPRESOR_FISCAL_16_BITS_H

#include "ImpresorFiscal.h"

class ImpresorFiscal16Bits : public ImpresorFiscal
{
public:

	// Constructor
	ImpresorFiscal16Bits();

	// Métodos locales
	void CambiarVelocidad (unsigned Velocidad) throw(Excepcion);

	// Métodos generales
	std::string ObtenerVersionDelControlador () throw (Excepcion);
	void ObtenerDatosDeInicializacion (RTA_ObtenerDatosDeInicializacion *R = NULL) throw (Excepcion);
	virtual void ObtenerDatosMemoriaDeTrabajo (RTA_ObtenerDatosMemoriaDeTrabajo *R = NULL) throw (Excepcion);

	virtual void BajaFiscal (const std::string &NumeroDeSerie = "") throw(Excepcion);
	
	virtual void CambiarCodigoIngresosBrutos (const std::string &CodigoIngBr) throw(Excepcion);
	virtual void CambiarFechaInicioActividades (const FECHA &Fecha) throw(Excepcion);

	TiposDeEstadoControlador EstadoControlador () throw(Excepcion);
	std::string DescripcionEstadoControlador () throw(Excepcion);

	TiposDeDocumento DocumentoEnCurso () throw(Excepcion);
	std::string DescripcionDocumentoEnCurso () throw(Excepcion);

	bool UltimoDocumentoFueCancelado () throw(Excepcion);

	unsigned UltimoDocumentoFiscalA () throw(Excepcion);
	unsigned UltimaNotaCreditoBC () throw(Excepcion);
	unsigned UltimaNotaCreditoA () throw(Excepcion);
	unsigned UltimoRemito () throw(Excepcion);

	unsigned PrimerNumeroDeDocumentoActual () throw(Excepcion);
	virtual unsigned PaginasDeUltimoDocumento () throw(Excepcion);

	virtual void CAIUltimoDocumentoA (RTA_CAIUltimoDocumentoA *R = NULL) throw (Excepcion);

	virtual void AbrirCajon () throw(Excepcion);
	virtual void AvanzarPapel (TiposDePapel Papel, unsigned Lineas) throw (Excepcion);

	virtual void DatosCliente (	const std::string &RazonSocial, 
								const std::string &NroDocumento,
								TiposDeDocumentoCliente TipoDocumento,
								TiposDeResponsabilidadesCliente ResponsabilidadIVA,
								const std::string &Direccion = "") throw(Excepcion);

	void CancelarComprobanteFiscal () throw(Excepcion);
	void CancelarComprobante () throw(Excepcion);
	void ReimprimirComprobante () throw(Excepcion);

	virtual void CargarLogo (const std::string &ArchivoBMP = "") throw(Excepcion);

	virtual void CapacidadDocumentoSlip (bool &SoportaFA, bool &SoportaNC);
	virtual void CapacidadDocumentoTicket (bool &SoportaFA, bool &SoportaNC);
	virtual void CapacidadEstacion (bool &SoportaTicket, bool &SoportaSlip);
	virtual void CapacidadCajon (bool &SoportaCajon);

	// Métodos de configuración
	std::string ObtenerNombreDeFantasia (unsigned Linea) throw (Excepcion);

	void EspecificarInformacionRemitoComprobanteOriginal (unsigned Linea, const std::string &Texto) throw (Excepcion);
	std::string ObtenerInformacionRemitoComprobanteOriginal (unsigned Linea) throw (Excepcion);

	void BorrarInformacionRemitoComprobanteOriginal () throw (Excepcion);

	virtual void ConfigurarControladorCompleto (
							bool Imprimir,
							bool Defaults,
							const double *LimiteConsumidorFinal,
							const double *LimiteTicketFactura,
							const double *PorcentajeIVANoInscripto,
							const NumerosDeCopias *TipoDeCopiasMaximo,
							const bool *ImprimeCambio,
							const bool *ImprimeLeyendasOpcionales,
							const TiposDeCorteDePapel *TipoDeCorte,
							const bool *ImprimeMarco,
							const bool *ReImprimeDocumentos,
							const std::string *DescripcionDelMedioDePago,
							const bool *Sonido,
							const TiposDeAltoHoja *AltoHoja,
							const TiposDeAnchoHoja *AnchoHoja,
							const TiposDeEstacion *EstacionImpresionReportesXZ,
							const TiposDeModoImpresion *ModoImpresion) throw (Excepcion);
	virtual void ObtenerConfiguracion (RTA_ObtenerConfiguracion *R = NULL) throw (Excepcion);

	// Métodos de reportes
	void ReporteZ (RTA_ReporteZX *R = NULL) throw (Excepcion);
	void ReporteX (RTA_ReporteZX *R = NULL) throw (Excepcion);
	void ReporteZIndividualPorFecha (FECHA FechaZ, RTA_ReporteZIndividual *R = NULL) throw (Excepcion);
	void ReporteZIndividualPorNumero (unsigned NumeroZ, RTA_ReporteZIndividual *R = NULL) throw (Excepcion);

	// Métodos de documentos fiscales
	virtual void AbrirDF (DocumentosFiscales Tipo) throw (Excepcion);
	virtual void EspecificarIVANoInscripto (double Monto) throw (Excepcion);
	void EspecificarPercepcionGlobal (const std::string &Nombre, double Monto) throw (Excepcion);
	void EspecificarPercepcionPorIVA (const std::string &Nombre, double Monto, double IVA) throw (Excepcion);
	virtual void Subtotal (bool Imprime, RTA_Subtotal *R = NULL) throw (Excepcion);
	virtual unsigned CerrarDF (unsigned Copias = 1) throw (Excepcion);

	// Métodos de documentos no fiscales
	virtual void CerrarDNF (unsigned Copias = 1) throw (Excepcion);

	// Métodos de documentos no fiscales homologados
	virtual void AbrirDNFH (DocumentosNoFiscalesHomologados Tipo, const std::string &Nro = "") throw (Excepcion);
	virtual void ImprimirItemEnRemito (const std::string &Texto, double Cantidad) throw (Excepcion);
	virtual void ImprimirItemEnCuenta (const FECHA &FechaComprobante,
							const std::string &NroComprobante,
							const std::string &Texto,
							double Debe, double Haber) throw (Excepcion);
	virtual void ImprimirItemEnCotizacion (const std::string &Texto) throw (Excepcion);
	virtual void DetalleRecibo (const std::string &Texto = " ") throw (Excepcion);
	virtual void ImprimirInfoEnDNFH (const unsigned IDCampo, const std::string &Texto, double Cantidad = 0.0) throw (Excepcion);
	virtual unsigned CerrarDNFH (unsigned Copias = 1) throw (Excepcion);

	virtual void DNFHFarmacia (unsigned Copias = 1) throw (Excepcion);
	virtual void DNFHReparto (unsigned Copias = 1) throw (Excepcion);
	virtual void ImprimirVoucher (const std::string &NombreCliente,
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

	std::string IndicadorII (double ImpuestosInternos, double Precio = 0.0, double IVA = 0.0) throw(Excepcion);
};

#endif
