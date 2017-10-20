#include <stdlib.h>
#include <string.h>
#include "fiscalStructures.h"
#include "P615_100.h"
#include "P715_100.h"
#include "P715_302.h"
#include "P715_403.h"
#include "P715_504.h"
#include "P320_100.h"
#include "P441_100.h"

// ProkaString
char* rtrim(char* source) {
  char* target = source + strlen(source);

  // Go to the end.
  while ((target != source) && (*(target - 1) != '\0')) {
    --target;
  }
  // Skip trim.
  while ((target != source) && (*(target - 1) == ' ')) {
    --target;
  }
  // Set new end.
  *target = '\0';

  return target;
}

// Others
class Eventos: public ManejadorDeEventos {
  public:
	Eventos(ImpresorFiscal *p): pFiscal(p) { }
	void Evento(unsigned Tipo, unsigned SubTipo) {
	  //printf ("!!! %2u %2u %s\n", Tipo, SubTipo,
	  //pFiscal->DescripcionDeEvento((ImpresorFiscal::TiposDeEvento)Tipo, SubTipo).c_str());
	}

  private:
	ImpresorFiscal *pFiscal;
};

void runFiscalCommand(FisCommand command) {
  ImpresorFiscal *pIF;
  Eventos *Evts;
  ImpresorFiscal::RTA_ObtenerDatosDeInicializacion Rta;
  ImpresorFiscal::RTA_ReporteZX RtaZX;
  ImpresorFiscal::RTA_Subtotal RtaSt;
  ImpresorFiscal::FECHA FIni, FFin;	

  try {
	switch (command.printer.model) {
		case FIS_MODEL_H615:
			pIF = new ImpresorFiscalP615_100;
			break;
		case FIS_MODEL_H715:		
			pIF = new ImpresorFiscalP715_100;
			break;
		case FIS_MODEL_H320:		
			pIF = new ImpresorFiscalP320_100;
			break;
		/*/
		 * Mapear el codigo interno unico con el driver propiamente dicho que proporciona el fabricante. 
		 * Puede que tengas que agregar al inicio de dicho archivo, el #include "PMODEL.h" correspondiente
		 *   dentro del archivo del modelo en cuestion, vas a encontrar el nombre que los de hasar le dieron al modelo.
	    /*/
		case FIS_MODEL_H441:		
			pIF = new ImpresorFiscalP441_100;
			break;
	}
	
	Evts = new Eventos(pIF);
	pIF->EstablecerManejadorDeEventos(Evts);
	pIF->EstablecerPuertoSerie(1, 9600);
	
	switch (command.action) {
		case FIS_RESUMEN:
			pIF->ObtenerDatosDeInicializacion (&Rta);
			
		    printf("%s\n", Rta.RazonSocial.c_str());
			printf("%s\n", Rta.NroCUIT.c_str());
			printf("%s\n", Rta.NroSerie.c_str());
			printf("%04u-%02u-%02u\n", Rta.FechaIncializacion.anio(), Rta.FechaIncializacion.mes(), Rta.FechaIncializacion.dia());
			printf("%s\n", Rta.NroPOS.c_str ());
			printf("%s\n", Rta.CodIngBrutos.c_str ());
			printf("%04u-%02u-%02u\n", Rta.FechaIniActividades.anio(), Rta.FechaIniActividades.mes(), Rta.FechaIniActividades.dia());
			printf("%c\n", Rta.RespIVA);
			break;
		case FIS_REPORTE_X:		
				pIF->ReporteX (&RtaZX);
				printf ("%u\n", RtaZX.NumeroReporte);
				printf ("%u\n",	RtaZX.CantidadDFCancelados);
				printf ("%u\n",	RtaZX.CantidadDNFHEmitidos);
				printf ("%u\n",	RtaZX.CantidadDNFEmitidos);
				printf ("%u\n",	RtaZX.CantidadDFEmitidos);
				printf ("%lu\n", RtaZX.UltimoDocFiscalBC);
				printf ("%lu\n", RtaZX.UltimoDocFiscalA);
				printf ("%.2f\n", RtaZX.MontoVentasDocFiscal);
				printf ("%.2f\n", RtaZX.MontoIVADocFiscal);
				printf ("%.2f\n", RtaZX.MontoImpInternosDocFiscal);
				printf ("%.2f\n", RtaZX.MontoPercepcionesDocFiscal);
				printf ("%.2f\n", RtaZX.MontoIVANoInscriptoDocFiscal);
				printf ("%lu\n", RtaZX.UltimaNotaCreditoBC);
				printf ("%lu\n", RtaZX.UltimaNotaCreditoA);
				printf ("%.2f\n", RtaZX.MontoVentasNotaCredito);
				printf ("%.2f\n", RtaZX.MontoIVANotaCredito);
				printf ("%.2f\n", RtaZX.MontoImpInternosNotaCredito);
				printf ("%.2f\n", RtaZX.MontoPercepcionesNotaCredito);
				printf ("%.2f\n", RtaZX.MontoIVANoInscriptoNotaCredito);
				printf ("%lu\n", RtaZX.UltimoRemito);
				printf ("%u\n",	RtaZX.CantidadNCCanceladas);
				printf ("%u\n",	RtaZX.CantidadDFBCEmitidos);
				printf ("%u\n",	RtaZX.CantidadDFAEmitidos);
				printf ("%u\n", RtaZX.CantidadNCBCEmitidas);
				printf ("%u\n",	RtaZX.CantidadNCAEmitidas);
			break;
		case FIS_REPORTE_Z:	
		  {		
			ImpresorFiscal::FECHA FIni(command.report.from), FFin(command.report.to);	
			pIF->ReporteZPorFechas (FIni, FFin, true);
		  }
		  break;
		case FIS_TICKET:
			int iNro,
				iCount = command.ticket.itemsCount;
			ImpresorFiscal::RTA_Subtotal RtaST;

			if (strcmp(command.ticket.client.id, "NONE") != 0) {
			  ImpresorFiscal::TiposDeResponsabilidadesCliente ResponsabilidadIVA;
			  ImpresorFiscal::TiposDeDocumentoCliente TipoDocumento;

			  switch (command.ticket.client.regimen) {
				case RESPONSABLE_INSCRIPTO:
					ResponsabilidadIVA = ImpresorFiscal::RESPONSABLE_INSCRIPTO;
					break;
				case MONOTRIBUTISTA:
					ResponsabilidadIVA = ImpresorFiscal::MONOTRIBUTO;
					break;
				case CONSUMIDOR_FINAL:
					ResponsabilidadIVA = ImpresorFiscal::CONSUMIDOR_FINAL;
					break;
			  }

			  switch (command.ticket.client.idType) {
				case CUIT:
					TipoDocumento = ImpresorFiscal::TIPO_CUIT;
					break;
				case CUIL:
					TipoDocumento = ImpresorFiscal::TIPO_CUIL;
					break;
			  }

			  pIF->DatosCliente(command.ticket.client.name,
								 command.ticket.client.id, 
								 TipoDocumento,
								 ResponsabilidadIVA,
								 command.ticket.client.address);
			} else {			
			  pIF->DatosCliente("Consumidor final",
								 "0000000000", 
								 ImpresorFiscal::TIPO_CUIT,
								 ImpresorFiscal::CONSUMIDOR_FINAL,
								 "Consumidor final");
			}

			switch (command.ticket.type) {
			  case FACTURA_A:
				pIF->AbrirDF(ImpresorFiscal::TICKET_FACTURA_A);
				break;
			  case FACTURA_B:
				pIF->AbrirDF(ImpresorFiscal::TICKET_FACTURA_B);
				break;
			}

			pIF->EstablecerModalidadDescripcionesLargas(true);
			pIF->ImprimirTextoFiscal(command.ticket.text);

			for (iNro=0; iNro<iCount; iNro++) {
			  float discount = (command.ticket.items[iNro].discount / 100) + 
								(command.ticket.discount / 100) -
								(command.ticket.items[iNro].discount * command.ticket.discount / 10000);
			  float price = command.ticket.items[iNro].price * (1 - discount);

			  std::string title = command.ticket.items[iNro].title;	

			  if (command.ticket.discount > 0) {
				char discountStr[8];		
				sprintf(discountStr, " -%.2f", discount * 100);
			    title += discountStr;
			    title += "%";
			  }							
				
			  pIF->ImprimirItem(title, command.ticket.items[iNro].quantity, price, command.ticket.items[iNro].iva, 0, false);
			}

			pIF->Subtotal(true, &RtaST);			
			pIF->ImprimirPago("", RtaST.MontoVentas, "");
			printf ("%lu\n", pIF->CerrarDF());
			break;
	}

  } catch (Excepcion &e) {
    printf("!!ERROR!! (%s) (%s)\n", e.Descripcion.c_str(), e.Contexto.c_str());
  }
}