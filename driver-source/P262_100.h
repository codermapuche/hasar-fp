#ifndef IMPRESOR_FISCAL_P262_100_H
#define IMPRESOR_FISCAL_P262_100_H

#include "ImpresorFiscal8Bits.h"

class ImpresorFiscalP262_100 : public ImpresorFiscal8Bits
{
public:

	// Constructor
        ImpresorFiscalP262_100();

	// String de Versión
	static std::string Version;

	// Métodos locales
	std::string DescripcionModelo () const;

	// Métodos generales
	TiposDeEstadoControlador EstadoControlador () throw(Excepcion);
	std::string DescripcionEstadoControlador () throw(Excepcion);

	void CortarPapel () throw (Excepcion);

	// Métodos de configuración
	void ConfigurarControladorPorBloque (
						double LimiteConsumidorFinal,
						double LimiteTicketFactura,
						double PorcentajeIVANoInscripto,
						NumerosDeCopias TipoDeCopiasMaximo,
						bool ImpresionCambio,
						bool ImpresionLeyendasOpcionales,
						TiposDeCorteDePapel TipoDeCorte) throw (Excepcion);

	// Métodos de reportes

	// Métodos de documentos fiscales

	// Métodos de documentos no fiscales
	void AbrirDNF (TiposDeEstacion EstacionDeImpresion) throw (Excepcion);

	// Métodos de documentos no fiscales homologados
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
