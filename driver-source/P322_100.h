#ifndef IMPRESOR_FISCAL_P322_100_H
#define IMPRESOR_FISCAL_P322_100_H

#include "P322.h"

class ImpresorFiscalP322_100 : public ImpresorFiscalP322
{
public:

	// Constructor
	ImpresorFiscalP322_100();

	// String de Versión
	static std::string Version;

	// Métodos locales
	std::string DescripcionModelo () const;

	// Métodos generales
	void CambiarFechaInicioActividades (const FECHA &Fecha) throw(Excepcion);

	unsigned PaginasDeUltimoDocumento () throw(Excepcion);

	void CAIUltimoDocumentoA (RTA_CAIUltimoDocumentoA *R = NULL) throw (Excepcion);

	// Métodos de configuración

	// Métodos de reportes

	// Métodos de documentos fiscales
	unsigned CerrarDF (unsigned Copias = 1) throw (Excepcion);

	// Métodos de documentos no fiscales
	void CerrarDNF (unsigned Copias = 1) throw (Excepcion);

	// Métodos de documentos no fiscales homologados
	unsigned CerrarDNFH (unsigned Copias = 1) throw (Excepcion);

protected:

}; 

#endif


