#ifndef IMPRESOR_FISCAL_PPL23_202_H
#define IMPRESOR_FISCAL_PPL23_202_H

#include "PPL23.h"

class ImpresorFiscalPPL23_202 : public ImpresorFiscalPPL23
{
public:

	// Constructor
	ImpresorFiscalPPL23_202();

	// String de Versión
	static std::string Version;

	// Métodos locales
	std::string DescripcionModelo () const;

	// Métodos generales

	// Métodos de configuración

	// Métodos de reportes

	// Métodos de documentos fiscales

	// Métodos de documentos no fiscales

	// Métodos de documentos no fiscales homologados
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


