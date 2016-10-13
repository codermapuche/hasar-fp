#ifndef IMPRESOR_FISCAL_P615_100_H
#define IMPRESOR_FISCAL_P615_100_H

#include "ImpresorFiscal8Bits.h"

class ImpresorFiscalP615_100 : public ImpresorFiscal8Bits
{
public:

	// Constructor
        ImpresorFiscalP615_100();

	// String de Versi�n
	static std::string Version;

	// M�todos locales
	std::string DescripcionModelo () const;

	// M�todos generales
	unsigned UltimoDocumentoFiscalA () throw(Excepcion);

	void DatosCliente(	const std::string &RazonSocial,
						const std::string &NroDocumento,
						TiposDeDocumentoCliente TipoDocumento,
						TiposDeResponsabilidadesCliente ResponsabilidadIVA,
						const std::string &Direccion = "") throw(Excepcion);

	void MostrarMensajeEnDisplay (const std::string &Texto,
						const TiposDeDisplay Destino) throw(Excepcion);

	void ImprimirCodigoDeBarras (TiposDeCodigoDeBarras Tipo,
						const std::string &Codigo,
						bool ImprimeNumeros,
						bool ImprimeAhora)  throw(Excepcion);

	void CapacidadDocumentoTicket (bool &SoportaFA, bool &SoportaNC);

	// M�todos de configuraci�n
	void ConfigurarControladorPorBloque (
						double LimiteConsumidorFinal,
						double LimiteTicketFactura,
						double PorcentajeIVANoInscripto,
						NumerosDeCopias TipoDeCopiasMaximo,
						bool ImpresionCambio,
						bool ImpresionLeyendasOpcionales,
						TiposDeCorteDePapel TipoDeCorte) throw (Excepcion);

	// M�todos de reportes
	void ReporteZ (RTA_ReporteZX *R = NULL) throw (Excepcion);
	void ReporteX (RTA_ReporteZX *R = NULL) throw (Excepcion);

	// M�todos de documentos fiscales
	void EspecificarIVANoInscripto (double Monto) throw (Excepcion);
	void EspecificarPercepcionGlobal (const std::string &Nombre, double Monto) throw (Excepcion);
	void EspecificarPercepcionPorIVA (const std::string &Nombre, double Monto, double IVA) throw (Excepcion);
	void Subtotal (bool Imprime, RTA_Subtotal *R = NULL) throw (Excepcion);

	// M�todos de documentos no fiscales
	void AbrirDNF (TiposDeEstacion EstacionDeImpresion) throw (Excepcion);

	// M�todos de documentos no fiscales homologados
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


