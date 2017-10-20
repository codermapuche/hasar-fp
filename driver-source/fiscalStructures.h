/*/
 * Fiscal parser primitive structures
/*/

typedef enum {
	// 1 Is prefix of Hasar
	FIS_MODEL_H615 = 1615,
	FIS_MODEL_H715 = 1715,
	FIS_MODEL_H320 = 1320,
	/*/
	 * Agregar un nuevo codigo interno unico del modelo.
	/*/
	FIS_MODEL_H441 = 1441
} FIS_MODELS;

typedef enum {
	FIS_TICKET = 1,
	FIS_REPORTE_X = 2,
	FIS_REPORTE_Z = 3,
	FIS_RESUMEN = 4
} FIS_ACTION;

typedef enum {
	FACTURA_B = 1,
	FACTURA_A = 2
} FIS_TICKET_TYPE;

typedef enum {
	CUIT = 1,
	CUIL = 2
} FIS_ID_TYPE;

typedef enum {
	RESPONSABLE_INSCRIPTO = 1,
	MONOTRIBUTISTA = 2,
	CONSUMIDOR_FINAL = 3
} FIS_REGIMEN;

typedef struct {
  FIS_MODELS model;
} FisPrinter;

typedef struct {
  char title[50];
  float price;
  float quantity;
  float iva;
  float discount;
} ItemTicket;

typedef struct {
  char name[100];
  char address[100];
  char id[20];
  FIS_ID_TYPE idType;
  FIS_REGIMEN regimen;
} ClientTicket;

typedef struct {
  char text[60];
  int itemsCount;
  float discount;
  FIS_TICKET_TYPE type;
  ItemTicket *items;
  ClientTicket client;
} Ticket;

typedef struct {
  char from[6];
  char to[6];
} Report;

typedef struct {
  FisPrinter printer;
  FIS_ACTION action;	
	Ticket ticket;
	Report report;
} FisCommand;