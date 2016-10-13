#ifndef IMPRESOR_FISCAL_P425_100_H
#define IMPRESOR_FISCAL_P425_100_H

#include "P425.h"

class ImpresorFiscalP425_100 : public ImpresorFiscalP425
{
public:

	// Constructor
	ImpresorFiscalP425_100();

	// String de Versi�n
	static std::string Version;

	// M�todos locales
	std::string DescripcionModelo () const;

	// M�todos generales
	void CambiarFechaInicioActividades (const FECHA &Fecha) throw(Excepcion);

	unsigned PaginasDeUltimoDocumento () throw(Excepcion);

	void CAIUltimoDocumentoA (RTA_CAIUltimoDocumentoA *R = NULL) throw (Excepcion);

	// M�todos de configuraci�n

	// M�todos de reportes

	// M�todos de documentos fiscales
	unsigned CerrarDF (unsigned Copias = 1) throw (Excepcion);

	// M�todos de documentos no fiscales
	void CerrarDNF (unsigned Copias = 1) throw (Excepcion);

	// M�todos de documentos no fiscales homologados
	void ImprimirInfoEnDNFH (const unsigned IDCampo, const std::string &Texto, double Cantidad = 0.0) throw (Excepcion);
	unsigned CerrarDNFH (unsigned Copias = 1) throw (Excepcion);

protected:

}; 

#endif


