#ifndef IMPRESOR_FISCAL_P614_100_H
#define IMPRESOR_FISCAL_P614_100_H

#include "ImpresorFiscal8Bits.h"

class ImpresorFiscalP614_100 : public ImpresorFiscal8Bits
{
public:

	// Constructor
        ImpresorFiscalP614_100();

	// String de Versión
	static std::string Version;

	// Métodos locales
	std::string DescripcionModelo () const;

	// Métodos generales
	void ObtenerDatosDeInicializacion (RTA_ObtenerDatosDeInicializacion *R = NULL) throw (Excepcion);
	void ObtenerDatosMemoriaDeTrabajo (RTA_ObtenerDatosMemoriaDeTrabajo *R = NULL) throw (Excepcion);

	TiposDeEstadoControlador EstadoControlador () throw(Excepcion);
	std::string DescripcionEstadoControlador () throw(Excepcion);

	void CortarPapel () throw (Excepcion);
	void MostrarMensajeEnDisplay (const std::string &Texto,
						const TiposDeDisplay Destino) throw(Excepcion);

	void ImprimirCodigoDeBarras (TiposDeCodigoDeBarras Tipo,
						const std::string &Codigo,
						bool ImprimeNumeros,
						bool ImprimeAhora)  throw(Excepcion);

	// Métodos de configuración
	void EspecificarNombreDeFantasia (unsigned Linea, const std::string &Texto) throw (Excepcion);
	void BorrarFantasiaEncabezadoCola (bool BorrarFantasia, bool BorrarEncabezado, bool BorrarCola) throw (Excepcion);

	void ConfigurarControlador (ParametrosDeConfiguracion Parametro, const std::string &Valor) throw (Excepcion);
	void ConfigurarControladorPorBloque (
						double LimiteConsumidorFinal,
						double LimiteTicketFactura,
						double PorcentajeIVANoInscripto,
						NumerosDeCopias TipoDeCopiasMaximo,
						bool ImpresionCambio,
						bool ImpresionLeyendasOpcionales,
						TiposDeCorteDePapel TipoDeCorte) throw (Excepcion);
	void ObtenerConfiguracion (RTA_ObtenerConfiguracion *R = NULL) throw (Excepcion);

	// Métodos de reportes
	void ReporteZIndividualPorFecha (FECHA FechaZ, RTA_ReporteZIndividual *R = NULL) throw (Excepcion);
	void ReporteZIndividualPorNumero (unsigned NumeroZ, RTA_ReporteZIndividual *R = NULL) throw (Excepcion);

	void ObtenerTablaIVAs (TablaIVAs &Tabla) throw (Excepcion);

	// Métodos de documentos fiscales
	void AbrirDF (DocumentosFiscales Tipo) throw (Excepcion);
	void ImprimirItem (const std::string &Texto,
						double Cantidad, double Monto,
						double IVA, double ImpuestosInternos,
						bool EnNegativo = false) throw (Excepcion);
	void DescuentoUltimoItem (const std::string &Texto, double Monto, bool EnNegativo = false) throw (Excepcion);
	void DescuentoGeneral (const std::string &Texto, double Monto, bool EnNegativo = false) throw (Excepcion);
	void DevolucionDescuento (const std::string &Texto,
						double Monto, double IVA,
						double ImpuestosInternos,
						bool EnNegativo,
						TiposDeDescuento Tipo) throw (Excepcion);

	// Métodos de documentos no fiscales
	void AbrirDNF (TiposDeEstacion EstacionDeImpresion) throw (Excepcion);

	// Métodos de documentos no fiscales homologados
	void DNFHFarmacia (unsigned Copias = 1) throw (Excepcion);
	void DNFHReparto (unsigned Copias = 1) throw (Excepcion);
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

	double LastIVA;
	double LastII;

}; 

#endif


