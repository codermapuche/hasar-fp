#ifndef IMPRESOR_FISCAL_8_BITS_H
#define IMPRESOR_FISCAL_8_BITS_H

#include "ImpresorFiscal.h"

class ImpresorFiscal8Bits : public ImpresorFiscal
{
public:

	// Constructor
	ImpresorFiscal8Bits();

	// Métodos locales

	// Métodos generales
	virtual void ObtenerDatosDeInicializacion (RTA_ObtenerDatosDeInicializacion *R = NULL) throw (Excepcion);

	virtual void BajaFiscal (const std::string &NumeroDeSerie = "") throw(Excepcion);

	virtual void CambiarCodigoIngresosBrutos (const std::string &CodigoIngBr) throw(Excepcion);
	virtual void CambiarFechaInicioActividades (const FECHA &Fecha) throw(Excepcion);

	virtual TiposDeEstadoControlador EstadoControlador () throw(Excepcion);
	virtual std::string DescripcionEstadoControlador () throw(Excepcion);

	virtual TiposDeDocumento DocumentoEnCurso () throw(Excepcion);
	virtual std::string DescripcionDocumentoEnCurso () throw(Excepcion);

	virtual bool UltimoDocumentoFueCancelado () throw(Excepcion);

	virtual unsigned UltimoDocumentoFiscalA () throw(Excepcion);
	virtual unsigned UltimaNotaCreditoBC () throw(Excepcion);
	virtual unsigned UltimaNotaCreditoA () throw(Excepcion);
	virtual unsigned UltimoRemito () throw(Excepcion);

	virtual unsigned PrimerNumeroDeDocumentoActual () throw(Excepcion);

	void AbrirCajon () throw(Excepcion);
	void AvanzarPapel(TiposDePapel Papel, unsigned Lineas) throw (Excepcion);

	virtual void DatosCliente (	const std::string &RazonSocial, 
								const std::string &NroDocumento,
								TiposDeDocumentoCliente TipoDocumento,
								TiposDeResponsabilidadesCliente ResponsabilidadIVA,
								const std::string &Direccion = "") throw(Excepcion);

	virtual void CancelarComprobanteFiscal () throw(Excepcion);
	virtual void CancelarComprobante () throw(Excepcion);

	virtual void CapacidadDocumentoSlip (bool &SoportaFA, bool &SoportaNC);
	virtual void CapacidadDocumentoTicket (bool &SoportaFA, bool &SoportaNC);
	virtual void CapacidadEstacion (bool &SoportaTicket, bool &SoportaSlip);
	virtual void CapacidadCajon (bool &SoportaCajon);

	// Métodos de configuración
	virtual std::string ObtenerNombreDeFantasia (unsigned Linea) throw (Excepcion);
	virtual void BorrarInformacionRemitoComprobanteOriginal () throw (Excepcion);

	virtual void EspecificarInformacionRemitoComprobanteOriginal (unsigned Linea, const std::string &Texto) throw (Excepcion);
	virtual std::string ObtenerInformacionRemitoComprobanteOriginal (unsigned Linea) throw (Excepcion);

	virtual void ObtenerConfiguracion (RTA_ObtenerConfiguracion *R = NULL) throw (Excepcion);

	// Métodos de reportes
	virtual void ReporteZIndividualPorFecha (FECHA FechaZ, RTA_ReporteZIndividual *R = NULL) throw (Excepcion);
	virtual void ReporteZIndividualPorNumero (unsigned NumeroZ, RTA_ReporteZIndividual *R = NULL) throw (Excepcion);

	// Métodos de documentos fiscales
	virtual void EspecificarIVANoInscripto (double Monto) throw (Excepcion);
	virtual void EspecificarPercepcionGlobal (const std::string &Nombre, double Monto) throw (Excepcion);
	virtual void EspecificarPercepcionPorIVA (const std::string &Nombre, double Monto, double IVA) throw (Excepcion);
	virtual unsigned CerrarDF (unsigned Copias = 1) throw (Excepcion);

	// Métodos de documentos no fiscales
	void CerrarDNF (unsigned Copias = 1) throw (Excepcion);

	// Métodos de documentos no fiscales homologados
	virtual void AbrirDNFH (DocumentosNoFiscalesHomologados Tipo, const std::string &Nro = "") throw (Excepcion);
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
