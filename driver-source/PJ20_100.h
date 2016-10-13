#ifndef IMPRESOR_FISCAL_PJ20_100_H
#define IMPRESOR_FISCAL_PJ20_100_H

#include "ImpresorFiscal16Bits.h"

class ImpresorFiscalPJ20_100 : public ImpresorFiscal16Bits
{
public:

	// Constructor
        ImpresorFiscalPJ20_100();

	// String de Versión
	static std::string Version;

	// Métodos locales
	std::string DescripcionModelo () const;

	// Métodos generales
	void BajaFiscal (const std::string &NumeroDeSerie = "") throw(Excepcion);

	void CambiarFechaInicioActividades (const FECHA &Fecha) throw(Excepcion);

	unsigned PaginasDeUltimoDocumento () throw(Excepcion);

	void CAIUltimoDocumentoA (RTA_CAIUltimoDocumentoA *R = NULL) throw (Excepcion);

	// Métodos de configuración

	// Métodos de reportes

	// Métodos de documentos fiscales
	void Subtotal (bool Imprime, RTA_Subtotal *R = NULL) throw (Excepcion);
	unsigned CerrarDF (unsigned Copias = 1) throw (Excepcion);

	// Métodos de documentos no fiscales
	void AbrirDNF (TiposDeEstacion EstacionDeImpresion) throw (Excepcion);
	void CerrarDNF (unsigned Copias = 1) throw (Excepcion);

	// Métodos de documentos no fiscales homologados
	void ImprimirInfoEnDNFH (const unsigned IDCampo, const std::string &Texto, double Cantidad = 0.0) throw (Excepcion);
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


