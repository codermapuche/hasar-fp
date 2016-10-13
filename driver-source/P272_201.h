#ifndef IMPRESOR_FISCAL_P272_201_H
#define IMPRESOR_FISCAL_P272_201_H

#include "P272.h"

class ImpresorFiscalP272_201 : public ImpresorFiscalP272
{
public:

	// Constructor
        ImpresorFiscalP272_201();

	// String de Versi�n
	static std::string Version;

	// M�todos locales
	std::string DescripcionModelo () const;
	void EstablecerModalidadResumenIVA (TiposDeResumenIVA Modo);

	// M�todos generales

	// M�todos de configuraci�n
	void BorrarFantasiaEncabezadoCola (bool BorrarFantasia, bool BorrarEncabezado, bool BorrarCola) throw (Excepcion);

	// M�todos de reportes

	// M�todos de documentos fiscales
	double ImprimirPago (const std::string &Texto, double Monto, const std::string &DescripcionAdicional = "") throw (Excepcion);
	double DevolverPago (const std::string &Texto, double Monto, const std::string &DescripcionAdicional = "") throw (Excepcion);

	// M�todos de documentos no fiscales

	// M�todos de documentos no fiscales homologados
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


