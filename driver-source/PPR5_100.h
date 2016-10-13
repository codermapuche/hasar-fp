#ifndef IMPRESOR_FISCAL_PPR5_100_H
#define IMPRESOR_FISCAL_PPR5_100_H

#include "PPR5.h"

class ImpresorFiscalPPR5_100 : public ImpresorFiscalPPR5
{
public:

	// Constructor
        ImpresorFiscalPPR5_100();

	// String de Versión
	static std::string Version;

	// Métodos locales
	std::string DescripcionModelo () const;

	// Métodos generales
	void DatosCliente (	const std::string &RazonSocial,
						const std::string &NroDocumento,
						TiposDeDocumentoCliente TipoDocumento,
						TiposDeResponsabilidadesCliente ResponsabilidadIVA,
						const std::string &Direccion = "") throw(Excepcion);

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

	// Métodos de documentos no fiscales homologados
	void AbrirDNFH (DocumentosNoFiscalesHomologados Tipo, const std::string &Nro = "") throw (Excepcion);

protected:

};

#endif


