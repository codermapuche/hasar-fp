#ifndef IMPRESOR_FISCAL_P715_201_H
#define IMPRESOR_FISCAL_P715_201_H

#include "P715.h"

class ImpresorFiscalP715_201 : public ImpresorFiscalP715
{
public:

	// Constructor
	ImpresorFiscalP715_201();

	// String de Versión
	static std::string Version;

	// Métodos locales
	std::string DescripcionModelo () const;

	// Métodos generales
	void CambiarFechaInicioActividades (const FECHA &Fecha) throw(Excepcion);

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


