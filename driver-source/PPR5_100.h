#ifndef IMPRESOR_FISCAL_PPR5_100_H
#define IMPRESOR_FISCAL_PPR5_100_H

#include "PPR5.h"

class ImpresorFiscalPPR5_100 : public ImpresorFiscalPPR5
{
public:

	// Constructor
        ImpresorFiscalPPR5_100();

	// String de Versi�n
	static std::string Version;

	// M�todos locales
	std::string DescripcionModelo () const;

	// M�todos generales
	void DatosCliente (	const std::string &RazonSocial,
						const std::string &NroDocumento,
						TiposDeDocumentoCliente TipoDocumento,
						TiposDeResponsabilidadesCliente ResponsabilidadIVA,
						const std::string &Direccion = "") throw(Excepcion);

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

	// M�todos de documentos fiscales

	// M�todos de documentos no fiscales

	// M�todos de documentos no fiscales homologados
	void AbrirDNFH (DocumentosNoFiscalesHomologados Tipo, const std::string &Nro = "") throw (Excepcion);

protected:

};

#endif


