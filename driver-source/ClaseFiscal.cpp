#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jsmn.h"
#include "fiscalStructures.h"
#include "fiscalengine.h"

char* ReadFile(char *filename) {
  char *buffer = NULL;
  int filesize, readsize;
  FILE *handler = fopen(filename, "r");

  if (handler) {
    fseek(handler, 0, SEEK_END);
    filesize = ftell(handler);
    rewind(handler);
    buffer = (char*) malloc(sizeof(char) * (filesize + 1));
    readsize = fread(buffer, sizeof(char), filesize, handler);
    buffer[filesize] = '\0';

	  if (filesize != readsize) {
			free(buffer);
			buffer = NULL;
	  }

    fclose(handler);
  }

  return buffer;
}

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */
static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

int main (int argc, char* argv[]) { 
    char *p = strrchr(argv[0], '\\');
	int pNro; // Param number
	int read;
	jsmn_parser parser;	
	FisCommand command;
	jsmntok_t toks[1024]; /* We expect no more than 1024 tokens */
	char *commands;

	if (p) {
	  p[0] = 0;
	}
    commands = ReadFile(strcat(argv[0], "\\command.json"));		
		
	jsmn_init(&parser);
	read = jsmn_parse(&parser, commands, strlen(commands), toks, sizeof(toks) / sizeof(toks[0]));
	
	/* Assume the top-level element is an object */
	if (read < 1 || toks[0].type != JSMN_OBJECT) {
		printf("Failed to parse JSON: %d\n", read);
		return 1;
	}

	// Init command
	strncpy(command.ticket.client.id, "NONE", 5);
	
	for (pNro = 1; pNro < read; pNro++) {
		
		/***********************************/
		/** Parse model of fiscal printer **/
		/***********************************/ 		
		if (jsoneq(commands, &toks[pNro], "model") == 0) {
			pNro++;
			
			if (jsoneq(commands, &toks[pNro], "615") == 0) {
				command.printer.model = FIS_MODEL_H615;				
				continue;
			}			
			
			if (jsoneq(commands, &toks[pNro], "715") == 0) {
				command.printer.model = FIS_MODEL_H715;				
				continue;
			}
			
			printf("Invalid token: %s\n", toks[pNro]);
			return 1;
		}
				
		/*****************************/
		/** Parse action of command **/
		/*****************************/ 	
		if (jsoneq(commands, &toks[pNro], "action") == 0) {
			pNro++;
			
			if (jsoneq(commands, &toks[pNro], "TICKET") == 0) {
				command.action = FIS_TICKET;
				// Populate defaults
				command.ticket.discount = 0;		
				continue;
			}			
						
			if (jsoneq(commands, &toks[pNro], "REPORTE_X") == 0) {
				command.action = FIS_REPORTE_X;				
				continue;
			}		
			
			if (jsoneq(commands, &toks[pNro], "REPORTE_Z") == 0) {
				command.action = FIS_REPORTE_Z;				
				continue;
			}		
			
			if (jsoneq(commands, &toks[pNro], "RESUMEN") == 0) {
				command.action = FIS_RESUMEN;				
				continue;
			}		

			printf("Invalid token: %s\n", toks[pNro]);
			return 1;
		}
				
		/***************************/
		/** Parse type of command **/
		/***************************/ 	
		if (jsoneq(commands, &toks[pNro], "type") == 0) {
			pNro++;
			if (command.action == FIS_TICKET) {
				if (jsoneq(commands, &toks[pNro], "TICKET_FACTURA_B") == 0) {
					command.ticket.type = FACTURA_B;				
					continue;
				}
				
				if (jsoneq(commands, &toks[pNro], "TICKET_FACTURA_A") == 0) {
					command.ticket.type = FACTURA_A;				
					continue;
				}
			}	

			printf("Invalid token: %s\n", toks[pNro]);
			return 1;
		}
					
		/*****************************/
		/** Parse range of Z report **/
		/*****************************/ 	
		if (jsoneq(commands, &toks[pNro], "from") == 0) {
			pNro++;
			if (command.action == FIS_REPORTE_Z) {
				int len = toks[pNro].end - toks[pNro].start;
				strncpy(command.report.from, commands + toks[pNro].start, 6);
				continue;
			}			

			printf("Invalid token: %s\n", toks[pNro]);
			return 1;
		}
		
		if (jsoneq(commands, &toks[pNro], "to") == 0) {
			pNro++;
			if (command.action == FIS_REPORTE_Z) {
				int len = toks[pNro].end - toks[pNro].start;
				strncpy(command.report.to, commands + toks[pNro].start, 6);
				continue;
			}			

			printf("Invalid token: %s\n", toks[pNro]);
			return 1;
		}
		
		if (jsoneq(commands, &toks[pNro], "text") == 0) {
			pNro++;
			if (command.action == FIS_TICKET) {
				int count = toks[pNro].end - toks[pNro].start;
				strncpy(command.ticket.text, commands + toks[pNro].start, count);
				command.ticket.text[count] = '\0';	
				continue;
			}			

			printf("Invalid token: %s\n", toks[pNro]);
			return 1;				
		}
		
						
		if (jsoneq(commands, &toks[pNro], "discount") == 0) {	
			char value[6];
			pNro++;	
			strncpy(value, commands + toks[pNro].start, toks[pNro].end - toks[pNro].start);
			command.ticket.discount = atof(value);			
			continue;
		}
		
		/*****************************/
		/** Parse client of tickets **/
		/*****************************/ 	
		if (jsoneq(commands, &toks[pNro], "client") == 0) {
			pNro++;
			if (command.action == FIS_TICKET && toks[pNro].type == JSMN_OBJECT) {
				int cpNro;
				int cpCount = toks[pNro].size;	
			
				for (cpNro = 0; cpNro < cpCount; cpNro++) {				
					pNro++;
					
					if (jsoneq(commands, &toks[pNro], "name") == 0) {	
						int count;
						pNro++;
						count = toks[pNro].end - toks[pNro].start;
						strncpy(command.ticket.client.name, commands + toks[pNro].start, count);
						command.ticket.client.name[count] = '\0';
						continue;
					}
					
					if (jsoneq(commands, &toks[pNro], "id") == 0) {		
						pNro++;
						strncpy(command.ticket.client.id, commands + toks[pNro].start, toks[pNro].end - toks[pNro].start);
						continue;
					}
					
					if (jsoneq(commands, &toks[pNro], "idType") == 0) {		
						pNro++;
						if (jsoneq(commands, &toks[pNro], "CUIT") == 0) {		
							command.ticket.client.idType = CUIT;
							continue;
						}
						if (jsoneq(commands, &toks[pNro], "CUIL") == 0) {		
							command.ticket.client.idType = CUIL;
							continue;
						}
						

						printf("Invalid token: %s\n", toks[pNro]);
						return 1;
					}
					
					if (jsoneq(commands, &toks[pNro], "address") == 0) {	
						int count;
						pNro++;
						count = toks[pNro].end - toks[pNro].start;
						strncpy(command.ticket.client.address, commands + toks[pNro].start, count);
						command.ticket.client.address[count] = '\0';
						continue;
					}
					
					if (jsoneq(commands, &toks[pNro], "regimen") == 0) {		
						pNro++;
						if (jsoneq(commands, &toks[pNro], "RESPONSABLE_INSCRIPTO") == 0) {		
							command.ticket.client.regimen = RESPONSABLE_INSCRIPTO;
							continue;
						}

						if (jsoneq(commands, &toks[pNro], "MONOTRIBUTISTA") == 0) {		
							command.ticket.client.regimen = MONOTRIBUTISTA;
							continue;
						}

						if (jsoneq(commands, &toks[pNro], "CONSUMIDOR_FINAL") == 0) {		
							command.ticket.client.regimen = CONSUMIDOR_FINAL;
							continue;
						}
						

						printf("Invalid token: %s\n", toks[pNro]);
						return 1;
					}
					

					printf("Invalid token: %s\n", toks[pNro]);
					return 1;
				}
				
				continue;
			}
			

			printf("Invalid token: %s\n", toks[pNro]);
			return 1;
		}
		
		/*****************************/
		/** Parse items of tickets **/
		/*****************************/ 	
		if (jsoneq(commands, &toks[pNro], "items") == 0) {	
			pNro++;
			if (command.action == FIS_TICKET && toks[pNro].type == JSMN_ARRAY) {					
				int iNro; // Item number
				int itemsCount = toks[pNro].size;			
					
				command.ticket.itemsCount = itemsCount;
				command.ticket.items = (ItemTicket *) malloc(itemsCount * sizeof(ItemTicket));
	
				for (iNro = 0; iNro < itemsCount; iNro++) {
					int ipNro; // Item prop number
					int itemPropsCount = toks[pNro + 1].size;

					// Populate defaults:
					command.ticket.items[iNro].price = 0;
					command.ticket.items[iNro].quantity = 0;
					command.ticket.items[iNro].iva = 0;
					command.ticket.items[iNro].discount = 0;

					pNro++;

					/* Assume the item element is an object */
					if (toks[pNro].type != JSMN_OBJECT) {
						printf("Invalid token: %s\n", toks[pNro]);
						return 1;
					}				
					
					for (ipNro = 0; ipNro < itemPropsCount; ipNro++) {
						pNro++;
						
						if (jsoneq(commands, &toks[pNro], "title") == 0) {
							int count;	
							pNro++;	
						    count = toks[pNro].end - toks[pNro].start;
							strncpy(command.ticket.items[iNro].title, commands + toks[pNro].start, count);
							command.ticket.items[iNro].title[count] = '\0';		
							continue;
						}
						
						if (jsoneq(commands, &toks[pNro], "price") == 0) {
							char price[10];
							pNro++;
							strncpy(price, commands + toks[pNro].start, toks[pNro].end - toks[pNro].start);
							command.ticket.items[iNro].price = atof(price);
							continue;
						}
						
						if (jsoneq(commands, &toks[pNro], "quantity") == 0) {
							char quantity[6];
							pNro++;	
							strncpy(quantity, commands + toks[pNro].start, toks[pNro].end - toks[pNro].start);
							command.ticket.items[iNro].quantity = atof(quantity);
							continue;
						}
						
						if (jsoneq(commands, &toks[pNro], "iva") == 0) {
							char iva[6];
							pNro++;	
							strncpy(iva, commands + toks[pNro].start, toks[pNro].end - toks[pNro].start);
							command.ticket.items[iNro].iva = atof(iva);
							continue;
						}
												
						if (jsoneq(commands, &toks[pNro], "discount") == 0) {									
							char value[6];
							pNro++;	
							strncpy(value, commands + toks[pNro].start, toks[pNro].end - toks[pNro].start);
							command.ticket.items[iNro].discount = atof(value);			
							continue;
						}
						

						printf("Invalid token: %s\n", toks[pNro]);
						return 1;
					}				
				}		
				
				continue;
			}
			

			printf("Invalid token: %s\n", toks[pNro]);
			return 1;
		}	
				

		printf("Invalid token: %s\n", toks[pNro]);
		return 1;
	}
	
	runFiscalCommand(command);	
	return 0;
}
