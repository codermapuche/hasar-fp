#ifndef IMPRESOR_FISCAL_H
#define IMPRESOR_FISCAL_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Protocolo.h"
#include "Eventos.h"
#include "Excepcion.h"

#ifdef P_LINUX
#define  _snprintf snprintf
#endif

#ifdef P_WIN32_VC_2005
#define _snprintf _snprintf_s
#endif

class ImpresorFiscal : public ManejadorDeEventos
{
public:
	enum Protocolos
	{
		PROTOCOLO_ORIGINAL, PROTOCOLO_STATPRN
	};

	enum TiposDeEvento
	{
		// Eventos asociados al Estado de Impresor
		EVENTO_DE_ESTADO_IMPRESOR,
		EVENTO_DE_IMPRESOR_OCUPADO,
		EVENTO_DE_FALTA_PAPEL,
		EVENTO_DE_ESTADO_CAJON,

		// Eventos asociados al Estado Fiscal
		EVENTO_DE_MEMORIA_FISCAL_LLENA,
		EVENTO_DE_ERROR_COMANDO,
		EVENTO_DE_ESTADO_FISCAL,

		// Eventos asociados a la Ejecución de Comandos
		EVENTO_DE_COMANDO_RECIBIDO,
		EVENTO_DE_COMANDO_PROCESADO,
		EVENTO_DE_COMANDO_REINTENTADO,
		EVENTO_DE_DETECCION_EN_PROGRESO,

		// Evento de Tiempo de espera libre
		// (durante tiempos muertos del protocolo)
		EVENTO_DE_TIEMPO_ESPERA_LIBRE
	};

	enum EstadosImpresor						// Bit	   Máscara
	{
		ESTADO_IMPRESOR_ERROR_MECANICO			= 2,	// 0x0004,
		ESTADO_IMPRESOR_FUERA_DE_LINEA			= 3,	// 0x0008,
		ESTADO_IMPRESOR_FALTA_PAPEL_JOURNAL		= 4,	// 0x0010,
		ESTADO_IMPRESOR_FALTA_PAPEL_RECEIPT		= 5,	// 0x0020,
		ESTADO_IMPRESOR_TAPA_ABIERTA			= 8,	// 0x0100,
		ESTADO_IMPRESOR_CAJON_CERRADO			= 14,	// 0x4000,
		ESTADO_IMPRESOR_OR_LOGICO_2_3_4_5_8_14	= 15	// 0x8000,
	};

	enum EstadosFiscal							// Bit	   Máscara
    {
		ESTADO_FISCAL_ERROR_MEMORIA_FISCAL		= 0,	// 0x0001,
		ESTADO_FISCAL_ERROR_MEMORIA_TRABAJO		= 1,	// 0x0002,
		ESTADO_FISCAL_COMANDO_DESCONOCIDO		= 3,	// 0x0008,
		ESTADO_FISCAL_CAMPO_DATOS_INVALIDO		= 4,	// 0x0010,
		ESTADO_FISCAL_COMANDO_INVALIDO			= 5,	// 0x0020,
		ESTADO_FISCAL_DESBORDE_ACUMULADOR		= 6,	// 0x0040,
		ESTADO_FISCAL_MEMORIA_FISCAL_LLENA		= 7,	// 0x0080,
		ESTADO_FISCAL_MEMORIA_FISCAL_CASI_LLENA	= 8,	// 0x0100,
		ESTADO_FISCAL_TERMINAL_CERTIFICADA		= 9,	// 0x0200,
		ESTADO_FISCAL_TERMINAL_FISCALIZADA		= 10,	// 0x0400,
		ESTADO_FISCAL_DOCUMENTO_FISCAL_ABIERTO	= 12,	// 0x1000,
		ESTADO_FISCAL_DOCUMENTO_ABIERTO			= 13,	// 0x2000,
		ESTADO_FISCAL_OR_LOGICO_0_8				= 15	// 0x8000,
    };

	enum TiposDeEstadoControlador
	{
		S_NONFORMATTED_MEMORY		= 0,	// Memoria fiscal no formateada.
		S_NONINITIALIZED_MEMORY		= 1,	// Memoria fiscal no inicializada.
		S_RECEIPT_NOT_OPENED		= 2,	// No hay ningún comprobante abierto.
		S_FISCAL_RECEIPT_OPENED		= 3,	// Un comprobante fiscal se encuentra abierto. Se puede vender.
		S_FISCAL_TEXT_ISSUED		= 4,	// Comprobante fiscal abierto. Se acaba de imprimir un texto fiscal.
		S_NONFISCAL_RECEIPT_OPENED	= 5,	// Un comprobante no fiscal se encuentra abierto.
		S_TENDER					= 6,	// Comprobante fiscal abierto. Se realizó al menos un pago.
		S_TENDER_CLEARED			= 7,	// Comprobante fiscal abierto. Se saldó el monto.
		S_PERCEPTION				= 8,	// Comprobante fiscal abierto. Se ha emitido el comando de percepciones.
		S_KILLED					= 9,	// El controlador ha sido dado de baja.
		S_RETURN_RECHARGE			= 10,	// Comprobante fiscal abierto. Se realizó un descuento / recargo general.
		S_DISCOUNT_CHARGE			= 11,	// Comprobante fiscal abierto. Se realizó una bonificación / recargo / devolución de envases.
		S_RECEIPT_CONCEPT			= 12,	// Recibo fiscal abierto. Se acaba de imprimir una línea con el concepto del recibo.
		S_CREDIT_NOTE				= 13,	// Una nota de crédito se encuenta abierta. Se puede hacer un crédito.
		S_CREDIT_NOTE_DISCOUNT		= 14,	// Nota de crédito abierta. Se realizó un descuento / recargo general.
		S_CREDIT_NOTE_RETURN		= 15,	// Nota de crédito abierta. Se realizó una bonificación / recargo / devolución de envases.
		S_CREDIT_NOTE_PERCEPTION	= 16,	// Nota de crédito abierta. Se ha emitido el comando de percepciones.
		S_CREDIT_NOTE_TEXT			= 17,	// Nota de crédito abierta. Se acaba de escribir una línea de texto.
		S_INTERNAL_USE_RECEIPT		= 18,	// Recibo de uso interno abierto. Se acaba de imprimir una línea con el concepto del recibo.
		S_QUOTATION					= 19,	// Una cotización se encuentra abierta.
		S_EMBARK					= 20,	// Un remito / orden de salida se encuentra abierto.
		S_ACCOUNT					= 21,	// Un resumen de cuenta / cargo a la habitación se encuentra abierto.
		S_BLOCKED					= 22,	// El controlador se encuentra bloqueado.
		S_DNFH_RECEIPT_OPENED		= 23,	// Un Documento No Fiscal Homologado específico se encuentra abierto.
		S_CREDIT_NOTE_TENDER		= 24,	// Nota de crédito abierta. Se realizó al menos un pago.
		S_NONE						= 0xFF	// Estado desconocido.
    };

	enum TiposDeDocumento
	{
		// Documentos Fiscales
		D_FACTURA_A					=	0x01,	// Factura A
		D_FACTURA_B					=	0x02,	// Factura B
		D_FACTURA_C					=	0x03,	// Factura C
		D_NOTA_DEBITO_A				=	0x04,	// Nota de Débito A
		D_NOTA_DEBITO_B				=	0x05,	// Nota de Débito B
		D_NOTA_DEBITO_C				=	0x06,	// Nota de Débito C
		D_RECIBO_A					=	0x07,	// Recibo A
		D_RECIBO_B					=	0x08,	// Recibo B
		D_RECIBO_C					=	0x09,	// Recibo C
		D_TICKET_BC					= 	0x0a,	// Ticket

		// Documentos No Fiscales
		D_DOC_NO_FISCAL				=	0x20,	// Documento No Fiscal General

		// Documentos No Fiscales Homologados
		D_NOTA_CREDITO_A			=	0x40,	// Nota de Crédito A
		D_NOTA_CREDITO_B			=	0x41,	// Nota de Crédito B
		D_NOTA_CREDITO_C			=	0x42,	// Nota de Crédito C
		D_REMITO					=	0x43,	// Remito
		D_RECIBO_X					=	0x44,	// Recibo X
		D_ORDEN_SALIDA				=	0x45,	// Orden de Salida
		D_RESUMEN_CUENTA			=	0x46,	// Resumen de Cuenta
		D_CARGO_HABITACION			=	0x47,	// Cargo a la Habitación
		D_COTIZACION				=	0x48,	// Cotización
		D_VOUCHER_TARJETA			=	0x49,	// Voucher de Tarjeta
		D_CLAUSULA_CREDITO			=	0x50,	// Cláusulas de Crédito en Cuota Fija
		D_CLAUSULA_SEGURO			=	0x51,	// Cláusulas de Segura de Desempleo
		D_PAGARE					=	0x52,	// Pagaré
		D_POLIZA_SEGURO				=	0x53,	// Póliza de Seguro de Garantía Complementaria
		D_RECORDATORIO				=	0x54,	// Recordatorio
		D_SOLICITUD_CREDITO			=	0x55,	// Solicitud de Crédito
		D_COMUNICACION_CLIENTE		=	0x56,	// Comunicación con Nuestros Clientes
		D_OFRECIMIENTO_CREDITO		=	0x57,	// Ofrecimiento de Crédito en Efectivo
		D_OFRECIMIENTO_TARJETA		=	0x58,	// Ofrecimiento de Tarjeta de Crédito
		D_MINUTA_CREDITO			=	0x59,	// Minuta de Crédito
		D_OFRECIMIENTO_PASAPORTE	=	0x5a,	// Ofrecimiento de Pasaporte
		D_RENOVACION_CREDITO		=	0x5b,	// Renovación de Crédito
		D_ADELANTO_REMUNERACION		=	0x5c,	// Adelanto de Remuneración
		D_SOLICITUD_TARJETA_DEBITO	=	0x5d,	// Solicitud de Tarjeta de Débito
		D_SOLICITUD_CLAVE_TARJETA	=	0x5e,	// Solicitud de Clave de Tarjeta
		D_RESCATE_MERCADERIA		=	0x5f,	// Rescate de Mercadería
		D_INGRESO_EGRESO_SUCURSAL	=	0x60,	// Ingresos y Egresos Internos de la Sucursal
		D_PRESUPUESTO				=	0x61,	// Presupuesto
		D_ENTREGA					=	0x62,	// Comprobante de Entrega
	    D_ESTACIONAMIENTO			=   0x63,	// Estacionamiento
		D_COBRO_SERVICIO			=	0x64,	// Cobro de Servicio
		D_INGRESO_DINERO			=	0x65,	// Ingreso de Dinero
		D_RETIRO_DINERO				=	0x66,	// Retiro de Dinero
		D_TALON_CAMBIO				=	0x67,	// Talón de Cambio
		D_TALON_REPARTO				=	0x68,	// Talón de Reparto
		D_TALON_REGALO				=	0x69,	// Talón de Regalo
		D_TALON_CUENTA_CORRIENTE	=	0x6A,	// Talón de Cuenta Corriente
		D_AVISO_OPERACION_CREDITO	=	0x6B,	// Aviso de Operaci¢n de Crédito
		D_CUPON_PROMOCION			=	0x6C,	// Cupón de Promoción
		D_USO_INTERNO_FARMACIA		=	0x6D,	// Uso Interno Farmacia

		D_NO_DOCUMENTO_EN_CURSO		=	0x00	// No hay documento en curso.
    };

	enum DocumentosFiscales
	{
		TICKET_FACTURA_A				=	0x41,	// ('A') Ticket Factura A
		TICKET_FACTURA_B				=	0x42,	// ('B') Ticket Factura B/C
		FACTURA_A						=	0x30,	// ('0') Factura A
		FACTURA_B						=	0x31,	// ('1') Factura B/C
		TICKET_NOTA_DEBITO_A			=	0x32,	// ('2') Nota de Débito A
		TICKET_NOTA_DEBITO_B			=	0x33,	// ('3') Nota de Débito B/C
		NOTA_DEBITO_A					=	0x44,	// ('D') Nota de Débito A
		NOTA_DEBITO_B					=	0x45,	// ('E') Nota de Débito B/C
		RECIBO_A						=	0x61,	// ('a') Recibo A
		RECIBO_B						=	0x62,	// ('b') Recibo B/C
		TICKET_C						= 	0x54	// ('T') Ticket B/C
	};


	enum DocumentosNoFiscalesHomologados
	{
		// DNFH originales a partir del modelo P320
		TICKET_NOTA_CREDITO_A			=	0x34,	// ('4') Ticket Nota de Crédito A
		TICKET_NOTA_CREDITO_B			=	0x35,	// ('5') Ticket Nota de Crédito B/C
		NOTA_CREDITO_A					=	0x52,	// ('R') Nota de Crédito A
		NOTA_CREDITO_B					=	0x53,	// ('S') Nota de Crédito B/C
		REMITO							=	0x72,	// ('r') Remito
		TICKET_RECIBO_X					=	0x36,	// ('6') Ticket Recibo X
		RECIBO_X						=	0x78,	// ('x') Recibo X
		ORDEN_SALIDA					=	0x73,	// ('s') Orden de Salida
		RESUMEN_CUENTA					=	0x74,	// ('t') Resumen de Cuenta
		CARGO_HABITACION				=	0x55,	// ('U') Cargo a la Habitación
		COTIZACION						=	0x75,	// ('u') Cotización

		// DNFH Nuevos de 1º generación
		CLAUSULA_CREDITO				=	0x3a,	// (':') Cláusulas de Crédito en Cuota Fija
		CLAUSULA_SEGURO					=	0x3b,	// (';') Cláusulas de Segura de Desempleo
		TICKET_PAGARE					=	0x37,	// ('7') Ticket Pagaré
		PAGARE							=	0x3c,	// ('<') Pagaré
		POLIZA_SEGURO					=	0x3d,	// ('=') Póliza de Seguro de Garantía Complementaria
		RECORDATORIO					=	0x3e,	// ('>') Recordatorio
		SOLICITUD_CREDITO				=	0x3f,	// ('?') Solicitud de Crédito

		// DNFH Nuevos de 2º generación
		COMUNICACION_CLIENTE			=	0x21,	// ('!') Comunicación con Nuestros Clientes
		OFRECIMIENTO_CREDITO			=	0x22,	// ('"') Ofrecimiento de Crédito en Efectivo
		OFRECIMIENTO_TARJETA			=	0x23,	// ('#') Ofrecimiento de Tarjeta de Crédito
		MINUTA_CREDITO					=	0x24,	// ('$') Minuta de Crédito
		OFRECIMIENTO_PASAPORTE			=	0x25,	// ('%') Ofrecimiento de Pasaporte
		RENOVACION_CREDITO				=	0x26,	// ('&') Renovación de Crédito
		ADELANTO_REMUNERACION			=	0x27,	// (''') Adelanto de Remuneración
		SOLICITUD_TARJETA_DEBITO		=	0x28,	// ('(') Solicitud de Tarjeta de Débito
		SOLICITUD_CLAVE_TARJETA			=	0x29,	// (')') Solicitud de Clave de Tarjeta
		RESCATE_MERCADERIA				=	0x2a,	// ('*') Rescate de Mercadería
		INGRESO_EGRESO_SUCURSAL			=	0x2b,	// ('+') Ingresos y Egresos Internos de la Sucursal

		// DNFH Nuevos de 2º generación
		TICKET_PRESUPUESTO				=	0x2c,	// (',') Ticket Presupuesto
		TICKET_ENTREGA					=	0x2d,	// ('-') Ticket Comprobante de Entrega
	    TICKET_ESTACIONAMIENTO			=   0x2e,	// ('.') Estacionamiento
		TICKET_COBRO_SERVICIO			=	0x2f,	// ('/') Cobro de Servicio
		TICKET_INGRESO_DINERO			=	0x40,	// ('@') Ingreso de Dinero
		TICKET_RETIRO_DINERO			=	0x41,	// ('A') Retiro de Dinero
		TICKET_TALON_CAMBIO				=	0x42,	// ('B') Talón de Cambio
		TICKET_TALON_REPARTO			=	0x43,	// ('C') Talón de Reparto
		TICKET_TALON_REGALO				=	0x44,	// ('D') Talón de Regalo
		TICKET_TALON_CUENTA_CORRIENTE	=	0x45,	// ('E') Talón de Cuenta Corriente
		TICKET_AVISO_OPERACION_CREDITO	=	0x46,	// ('F') Aviso de Operaci¢n de Crédito
		TICKET_CUPON_PROMOCION			=	0x47,	// ('G') Cupón de Promoción
		TICKET_USO_INTERNO_FARMACIA		=	0x48	// ('H') Uso Interno Farmacia
	};

	enum IdentificadoresDeDNFH
	{
		// DNFH originales a partir del modelo P320
		DNFH_NOTA_CREDITO_A				=	0x52,	// ('R') Nota de Crédito A
		DNFH_NOTA_CREDITO_B				=	0x53,	// ('S') Nota de Crédito B/C
		DNFH_REMITO						=	0x72,	// ('r') Remito
		DNFH_RECIBO_X					=	0x78,	// ('x') Recibo X
		DNFH_ORDEN_SALIDA				=	0x73,	// ('s') Orden de Salida
		DNFH_RESUMEN_CUENTA				=	0x74,	// ('t') Resumen de Cuenta
		DNFH_CARGO_HABITACION			=	0x55,	// ('U') Cargo a la Habitación
		DNFH_COTIZACION					=	0x75,	// ('u') Cotización

		// DNFH Nuevos de 1º generación
		DNFH_CLAUSULA_CREDITO			=	0x3a,	// (':') Cláusulas de Crédito en Cuota Fija
		DNFH_CLAUSULA_SEGURO			=	0x3b,	// (';') Cláusulas de Segura de Desempleo
		DNFH_PAGARE						=	0x3c,	// ('<') Pagaré
		DNFH_POLIZA_SEGURO				=	0x3d,	// ('=') Póliza de Seguro de Garantía Complementaria
		DNFH_RECORDATORIO				=	0x3e,	// ('>') Recordatorio
		DNFH_SOLICITUD_CREDITO			=	0x3f,	// ('?') Solicitud de Crédito

		// DNFH Nuevos de 2º generación
		DNFH_COMUNICACION_CLIENTE		=	0x21,	// ('!') Comunicación con Nuestros Clientes
		DNFH_OFRECIMIENTO_CREDITO		=	0x22,	// ('"') Ofrecimiento de Crédito en Efectivo
		DNFH_OFRECIMIENTO_TARJETA		=	0x23,	// ('#') Ofrecimiento de Tarjeta de Crédito
		DNFH_MINUTA_CREDITO				=	0x24,	// ('$') Minuta de Crédito
		DNFH_OFRECIMIENTO_PASAPORTE		=	0x25,	// ('%') Ofrecimiento de Pasaporte
		DNFH_RENOVACION_CREDITO			=	0x26,	// ('&') Renovación de Crédito
		DNFH_ADELANTO_REMUNERACION		=	0x27,	// (''') Adelanto de Remuneración
		DNFH_SOLICITUD_TARJETA_DEBITO	=	0x28,	// ('(') Solicitud de Tarjeta de Débito
		DNFH_SOLICITUD_CLAVE_TARJETA	=	0x29,	// (')') Solicitud de Clave de Tarjeta
		DNFH_RESCATE_MERCADERIA			=	0x2a,	// ('*') Rescate de Mercadería
		DNFH_INGRESO_EGRESO_SUCURSAL	=	0x2b,	// ('+') Ingresos y Egresos Internos de la Sucursal

		// DNFH Nuevos de 2º generación
		DNFH_PRESUPUESTO				=	0x2c,	// (',') Ticket Presupuesto
		DNFH_ENTREGA					=	0x2d,	// ('-') Ticket Comprobante de Entrega
	    DNFH_ESTACIONAMIENTO			=   0x2e,	// ('.') Estacionamiento
		DNFH_COBRO_SERVICIO				=	0x2f,	// ('/') Cobro de Servicio
		DNFH_INGRESO_DINERO				=	0x30,	// ('0') Ingreso de Dinero
		DNFH_RETIRO_DINERO				=	0x31,	// ('1') Retiro de Dinero
		DNFH_TALON_CAMBIO				=	0x32,	// ('2') Talón de Cambio
		DNFH_TALON_REPARTO				=	0x33,	// ('3') Talón de Reparto
		DNFH_TALON_REGALO				=	0x34,	// ('4') Talón de Regalo
		DNFH_TALON_CUENTA_CORRIENTE		=	0x35,	// ('5') Talón de Cuenta Corriente
		DNFH_AVISO_OPERACION_CREDITO	=	0x36,	// ('6') Aviso de Operaci¢n de Crédito
		DNFH_CUPON_PROMOCION			=	0x37,	// ('7') Cupón de Promoción
		DNFH_USO_INTERNO_FARMACIA		=	0x38,	// ('8') Uso Interno Farmacia

		DNFH_NO_EXISTENTE				=	0x00	//       DNFH no existente
	};

	enum TiposDeDocumentoCliente
	{
		TIPO_CUIT		= 'C',
		TIPO_CUIL		= 'L',
		TIPO_LE			= '0',
		TIPO_LC			= '1',
		TIPO_DNI		= '2',
		TIPO_PASAPORTE	= '3',
		TIPO_CI			= '4',
		TIPO_NINGUNO	= ' '
	};

	enum TiposDeResponsabilidadesCliente
	{
		RESPONSABLE_INSCRIPTO			= 'I',
		RESPONSABLE_NO_INSCRIPTO		= 'N',
		RESPONSABLE_EXENTO				= 'E',
		NO_RESPONSABLE					= 'A',
		CONSUMIDOR_FINAL				= 'C',
		BIENES_DE_USO					= 'B',
		NO_CATEGORIZADO					= 'T',
		MONOTRIBUTO						= 'M',
		MONOTRIBUTO_SOCIAL				= 'S',
		EVENTUAL						= 'V',
		EVENTUAL_SOCIAL					= 'W'
	};

	enum TiposDeResponsabilidadesImpresor
	{
		CLASE_RESPONSABLE_INSCRIPTO		= 'I',
		CLASE_RESPONSABLE_NO_INSCRIPTO	= 'N',
		CLASE_RESPONSABLE_EXENTO		= 'E',
		CLASE_NO_RESPONSABLE			= 'A',
		CLASE_MONOTRIBUTO				= 'M',
		CLASE_MONOTRIBUTO_SOCIAL		= 'S'
	};

	enum TiposDeEstacion
	{
		ESTACION_TICKET	= 'T',
		ESTACION_SLIP	= 'S'
	};

	enum TiposDePapel
    {
		PAPEL_TICKET			= 0,
		PAPEL_DIARIO			= PAPEL_TICKET + 1,
		PAPEL_TICKET_Y_DIARIO	= PAPEL_DIARIO + 1
    };

	enum ModosDeDisplay
	{
		DISPLAY_NO			= '0',
		DISPLAY_SI			= '1',
		DISPLAY_REP			= '2'
	};

	enum TiposDeDisplay
    {
		LINEA_SUPERIOR			= 'L',
		LINEA_INFERIOR			= 'K',
		SECCION_DE_REPETICION	= 'N'
    };

	enum TiposDeCodigoDeBarras
	{
		CODIGO_DE_BORRADO	= '0',
		CODIGO_TIPO_EAN_13	= '1',
		CODIGO_TIPO_EAN_8	= '2',
		CODIGO_TIPO_UPCA	= '3',
		CODIGO_TIPO_ITS		= '4'
	};

	enum ModosDePrecio
	{
		MODO_PRECIO_BASE		= 'B',
		MODO_PRECIO_TOTAL		= 'T'
	};

	enum ModosDeImpuestosInternos
	{
		II_VARIABLE_KIVA		= '0',
		II_VARIABLE_PORCENTUAL	= '%',
		II_FIJO_KIVA			= '+',
		II_FIJO_MONTO			= '$'
	};

	enum TiposDeDescuento
	{
		DESCUENTO_RECARGO		= 'B',
		DEVOLUCION_DE_ENVASES	= 'E'
	};

	enum TiposDeVouchers
	{
		VOUCHER_DE_COMPRA					= 'C',
		VOUCHER_DE_CANCELACION_COMPRA		= 'V',
		VOUCHER_DE_DEVOLUCION				= 'D',
		VOUCHER_DE_CANCELACION_DEVOLUCION	= 'A'
	};

	enum TiposDeTarjetas
	{
		TARJETA_CREDITO	= 'C',
		TARJETA_DEBITO	= 'D'
	};

	enum TiposDeIngresoDeTarjeta
	{
		INGRESO_DE_TARJETA_MANUAL		= '*',
		INGRESO_DE_TARJETA_AUTOMATIZADO	= ' '
	};

	enum TiposOperacionDeTarjeta
	{
		OPERACION_TARJETA_ONLINE	= 'N',
		OPERACION_TARJETA_OFFLINE	= 'F'
	};

	enum ParametrosDeConfiguracion
	{
		IMPRESION_CAMBIO		= '4',
		IMPRESION_LEYENDAS		= '5',
		CORTE_PAPEL				= '6',
		IMPRESION_MARCO			= '7',
		REIMPRESION_CANCELADOS	= '8',
		COPIAS_DOCUMENTOS		= '9',
		PAGO_SALDO				= ':',
		SONIDO_AVISO			= ';',
		ALTO_HOJA				= '<',
		ANCHO_HOJA				= '=',
		ESTACION_REPORTES_XZ	= '>',
		MODO_IMPRESION			= '?',
		CHEQUEO_DESBORDE		= '@',
		CHEQUEO_TAPA_ABIERTA	= 'A',
		IMPRESION_ID_COPIA_DNFH	= 'B',
		IMPRESION_COLOR_NEGRO	= 'C'
	};

	enum NumerosDeCopias
	{
		NO_COPIAS				= '0',
		ORIGINAL				= '1',
		DUPLICADO				= '2',
		TRIPLICADO				= '3',
		CUADRUPLICADO			= '4'
	};

	enum TiposDeCorteDePapel
	{
		CORTE_TOTAL				= 'F',
		CORTE_PARCIAL			= 'P',
		NO_CORTE				= 'N'
	};

	enum TiposDeAltoHoja
	{
		ALTO_REDUCIDO			= 'M',
		ALTO_A4					= 'A',
		ALTO_OFICIO				= 'O',
		ALTO_CARTA				= 'C'
	};

	enum TiposDeAnchoHoja
	{
		ANCHO_REDUCIDO			= 'M',
		ANCHO_NORMAL			= 'N'
	};

	enum TiposDeModoImpresion
	{
		USO_ESTACION_TICKET		= 'M',
		NO_USO_ESTACION_TICKET	= 'A'
	};

	// Estructura de Fecha
	struct FECHA
	{
	public:
		// Setters de d, m y a
		void dia (unsigned di)
		{
			d = di;
		}
		void mes (unsigned me)
		{
			m = me;
		}
		void anio (unsigned an)
		{
			a = (an % 100);
			a += (a > 95) ? 1900 : 2000;
		}

		// Getters de d, m y a
		unsigned dia () const
		{
			return d;
		}
		unsigned mes () const
		{
			return m;
		}
		unsigned anio () const
		{
			return a;
		}

		// Constructor default
		FECHA ()
		{
#ifdef P_WIN32
			SYSTEMTIME CurrentDateTime;
			GetLocalTime (&CurrentDateTime);
			dia  (CurrentDateTime.wDay);
			mes  (CurrentDateTime.wMonth);
			anio (CurrentDateTime.wYear);
#endif

#ifdef P_LINUX
			time_t ct;
			struct tm *ptm;

			time (&ct);
			ptm = localtime(&ct);

			dia(ptm->tm_mday);
			mes(ptm->tm_mon+1);
			anio(1900 + ptm->tm_year);
#endif
		}

		// Constructor con parámetros día, mes y año
		FECHA (unsigned di, unsigned me, unsigned an)
		{
			dia (di);
			mes (me);
			anio (an);
		}

		// Constructor con parámetros AAMMDD en string
		FECHA (const std::string &AAMMDD) throw(Excepcion) {
			anio (atoi (AAMMDD.substr (0, 2).c_str()));
			mes (atoi (AAMMDD.substr (2, 2).c_str()));
			dia (atoi (AAMMDD.substr (4, 2).c_str()));
		}

		// Convierte información en estructura FECHA a string
		std::string ToString () const
		{
			char Buf[10];
			_snprintf(Buf, sizeof Buf, "%02u%02u%02u", a % 100, m, d);
			return Buf;
		}

	private:
		unsigned d;
		unsigned m;
		unsigned a;
	};

	// Estructura de Hora
	struct HORA
	{
	public:
		// Setters de hora, minutos y segundos
		void hora (unsigned ho)
		{
			h = ho;
		}
		void minutos (unsigned mi)
		{
			m = mi;
		}
		void segundos (unsigned se)
		{
			s = se;
		}

		// Getters de hora, minutos y segundos
		unsigned hora () const
		{
			return h;
		}
		unsigned minutos () const
		{
			return m;
		}
		unsigned segundos () const
		{
			return s;
		}


		// Constructor default
		HORA ()
		{
#ifdef P_WIN32
			SYSTEMTIME CurrentDateTime;
			GetLocalTime (&CurrentDateTime);
			hora     (CurrentDateTime.wHour);
			minutos  (CurrentDateTime.wMinute);
			segundos (CurrentDateTime.wSecond);
#endif

#ifdef P_LINUX
			time_t ct;
			struct tm *ptm;

			time (&ct);
			ptm = localtime(&ct);

			hora     (ptm->tm_hour);
			minutos  (ptm->tm_min);
			segundos (ptm->tm_sec);
#endif
		}

		// Constructor con parámetros hora, minutos y segundos
		HORA (unsigned ho, unsigned mi, unsigned se)
		{
			hora     (ho);
			minutos  (mi);
			segundos (se);
		}

		// Constructor con parámetros HHMMSS en string
		HORA (const std::string &HHMMSS) throw(Excepcion)
		{
			unsigned i;
			for (i = 0; isdigit(HHMMSS [i]); i++)
				;

			if (HHMMSS.length () != 6 || i != 6)
				throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_HORA_NO_VALIDA, "Constructor HORA (HHMMSS)");

			hora     (atoi (HHMMSS.substr (0, 2).c_str()));
			minutos  (atoi (HHMMSS.substr (2, 2).c_str()));
			segundos (atoi (HHMMSS.substr (4, 2).c_str()));
		}

		// Convierte información en estructura HORA a string
		std::string ToString() const
		{
			char Buf[10];
			_snprintf(Buf, sizeof Buf, "%02u%02u%02u", h, m, s);
			return Buf;
		}

	private:
		unsigned h;
		unsigned m;
		unsigned s;
	};

	// Grupos de características soportadas por las
	// distintas familias de controladores fiscales
	enum CapacidadesSoportadas
	{
		CAPACIDAD_DOCUMENTO_SLIP,		// TiposDeDocumentoSoportado
		CAPACIDAD_DOCUMENTO_TICKET,		// TiposDeDocumentoSoportado
		CAPACIDAD_ESTACION,				// TiposDeEstacion
		CAPACIDAD_CAJON,				//
		CAPACIDAD_TEXTO_FISCAL,			// PrintFiscalTextTicketSize / PrintItemTextSlipSize
		CAPACIDAD_TEXTO_VENTA,			// PrintItemTextTicketSize / PrintItemTextSlipSize
		CAPACIDAD_TEXTO_NO_FISCAL,		// PrintNonFiscalTextTicketSize / PrintNonFiscalTextSlipSize
		CAPACIDAD_TEXTO_ENCABEZADO_COLA,// HeaderTrailerTicketSize / HeaderTrailerSlipSize
		CAPACIDAD_DF_SOPORTADO,			// DocumentosFiscales
		CAPACIDAD_DNFH_SOPORTADO		// DocumentosNoFiscalesHomologados
	};

	enum TiposDeDocumentoSoportado
	{
		SOPORTA_FACTURA,
		SOPORTA_NOTA_CREDITO
	};

	enum TiposDeResumenIVA
	{
		RESUMEN_IVA_DIARIO,
		RESUMEN_IVA_DOCUMENTO
	};

	// Estructura de respuesta del método
	// 'ObtenerDatosDeInicializacion'.
	struct RTA_ObtenerDatosDeInicializacion
	{
		std::string NroCUIT;
		std::string RazonSocial;
		std::string NroSerie;
		FECHA FechaIncializacion;
		std::string NroPOS;
		std::string CodIngBrutos;
		FECHA FechaIniActividades;
		TiposDeResponsabilidadesImpresor RespIVA;
	};

	// Estructura de respuesta del método
	// 'ObtenerDatosMemoriaDeTrabajo'.
	struct RTA_ObtenerDatosMemoriaDeTrabajo
	{
		unsigned CantidadDFCancelados;
		unsigned CantidadDNFEmitidos;
		unsigned CantidadDFEmitidos;
		unsigned UltimoDocFiscalBC;
		unsigned UltimoDocFiscalA;
		double MontoVentasDocFiscal;
		double MontoIVADocFiscal;
		double MontoImpInternosDocFiscal;
		double MontoPercepcionesDocFiscal;
		double MontoIVANoInscriptoDocFiscal;
		unsigned UltimaNotaCreditoBC;
		unsigned UltimaNotaCreditoA;
		double MontoVentasNotaCredito;
		double MontoIVANotaCredito;
		double MontoImpInternosNotaCredito;
		double MontoPercepcionesNotaCredito;
		double MontoIVANoInscriptoNotaCredito;
		unsigned UltimoRemito;
		unsigned CantidadNCCanceladas;
		unsigned CantidadDFBCEmitidos;
		unsigned CantidadDFAEmitidos;
		unsigned CantidadNCBCEmitidas;
		unsigned CantidadNCAEmitidas;
	};

	// Estructura de respuesta del método
	// 'CAIUltimoDocumentoA'.
	struct RTA_CAIUltimoDocumentoA
	{
		TiposDeDocumento Documento;
		unsigned PrimerNumero;
		unsigned UltimoNumero;
		std::string CAI;
	};

	// Estructura de respuesta del método
	// 'ObtenerConfiguracion'.
	struct RTA_ObtenerConfiguracion
	{
		double LimiteConsumidorFinal;
		double LimiteTicketFactura;
		double PorcentajeIVANoInscripto;
		NumerosDeCopias TipoDeCopiasMaximo;
		bool ImprimeCambio;
		bool ImprimeLeyendasOpcionales;
		TiposDeCorteDePapel TipoDeCorte;
		bool ImprimeMarco;
		bool ReImprimeDocumentos;
		std::string DescripcionDelMedioDePago;
		bool Sonido;
		TiposDeAltoHoja AltoHoja;
		TiposDeAnchoHoja AnchoHoja;
		TiposDeEstacion EstacionImpresionReportesXZ;
		TiposDeModoImpresion ModoImpresion;
		bool ChequeoDesbordeCompleto;
		bool ChequeoTapaAbierta;
		bool ImprimeIDCopiaDNFH;
		bool ImprimeColorNegro;
	};

	// Estructura de respuesta del método
	// 'ReporteZ' o 'ReporteX'.
	struct RTA_ReporteZX
	{
		unsigned NumeroReporte;
		unsigned CantidadDFCancelados;
		unsigned CantidadDNFHEmitidos;
		unsigned CantidadDNFEmitidos;
		unsigned CantidadDFEmitidos;
		unsigned UltimoDocFiscalBC;
		unsigned UltimoDocFiscalA;
		double MontoVentasDocFiscal;
		double MontoIVADocFiscal;
		double MontoImpInternosDocFiscal;
		double MontoPercepcionesDocFiscal;
		double MontoIVANoInscriptoDocFiscal;
		unsigned UltimaNotaCreditoBC;
		unsigned UltimaNotaCreditoA;
		double MontoVentasNotaCredito;
		double MontoIVANotaCredito;
		double MontoImpInternosNotaCredito;
		double MontoPercepcionesNotaCredito;
		double MontoIVANoInscriptoNotaCredito;
		unsigned UltimoRemito;
		unsigned CantidadNCCanceladas;
		unsigned CantidadDFBCEmitidos;
		unsigned CantidadDFAEmitidos;
		unsigned CantidadNCBCEmitidas;
		unsigned CantidadNCAEmitidas;
	};

	// Estructura de respuesta del método
	// 'ReporteZIndividualPorFecha' y
	// 'ReporteZIndividualPorNumero'.
	struct RTA_ReporteZIndividual
	{
		FECHA FechaZeta;
		unsigned NumeroZeta;
		unsigned UltimoDocFiscalBC;
		unsigned UltimoDocFiscalA;
		double MontoVentasDocFiscal;
		double MontoIVADocFiscal;
		double MontoImpInternosDocFiscal;
		double MontoPercepcionesDocFiscal;
		double MontoIVANoInscriptoDocFiscal;
		unsigned UltimaNotaCreditoBC;
		unsigned UltimaNotaCreditoA;
		double MontoVentasNotaCredito;
		double MontoIVANotaCredito;
		double MontoImpInternosNotaCredito;
		double MontoPercepcionesNotaCredito;
		double MontoIVANoInscriptoNotaCredito;
		unsigned UltimoRemito;
	};

	// Estructura de respuesta del método
	// 'CapacidadRestante'.
	struct RTA_CapacidadRestante
	{
		unsigned TotalCierresZeta;
		unsigned CierreZetaActual;
	};

	// Estructura de respuesta del método
	// 'Subtotal'.
	struct RTA_Subtotal
	{
		double CantidadItemsVendidos;
		double MontoVentas;
		double MontoIVA;
		double MontoPagado;
		double MontoIVANoInscripto;
		double MontoImpuestosInternos;
	};

	// Estructura de respuesta del método
	// 'EstadoInternoImpresor'.
	struct RTA_EstadoInternoImpresor
	{
		unsigned Estado;
		std::string Descripcion;
	};

	// Estructuras de respuesta del método
	// 'ObtenerTablaIVAs'.

	// Estructuras de la Tabla de IVAs
	enum TiposDeRegistrosIVA
	{
		REGISTRO_FIN_INFO_IVAS		= '0',
		REGISTRO_ALICUOTA_IVA_DF	= '1',
		REGISTRO_PERCEPCION_DF		= '2',
		REGISTRO_ALICUOTA_IVA_NC	= '3',
		REGISTRO_PERCEPCION_NC		= '4',
		REGISTRO_MONTO_TOTAL_DF		= '5',
		REGISTRO_MONTO_TOTAL_NC		= '6',
		REGISTRO_PERC_DESCRIP_DF	= '7',
		REGISTRO_PERC_DESCRIP_NC	= '8'
	};

	// Estructura de Alícuota IVA
	struct IVA
	{
		double Alicuota;
		double MontoIVA;
		double MontoImpInternos;
		double MontoIVANoInscripto;
		double MontoNetoSinIVA;
	};

	// Estructura de Percepción a un IVA
	struct PERCEPCION
	{
		double Alicuota;
		double Monto;
	};

	// Estructura de Percepción por descripción
	struct PERCEPCION_POR_DESCRIPCION
	{
		std::string Descripcion;
		double Monto;
	};

	// Estructura de Acumulados
	struct ACUMULADO
	{
		std::list <IVA> IVAs;
		std::list <PERCEPCION> PercepcionesIVA;
		std::list <double> PercepcionesGenerales;
		double MontoTotal;
		std::list <PERCEPCION_POR_DESCRIPCION> PercepcionesPorDescripcion;

		// Constructor default
		ACUMULADO ()
		{
			MontoTotal = 0;
		}

	};

	// Tabla de IVAs y Percepciones de DF y NC.
	struct TablaIVAs
	{
		ACUMULADO DocFiscales;
		ACUMULADO NotasCredito;

		bool IsEmpty()
		{
			return	!DocFiscales.IVAs.size() &&
					!DocFiscales.PercepcionesIVA.size() &&
					!DocFiscales.PercepcionesGenerales.size() &&
					!DocFiscales.PercepcionesPorDescripcion.size() &&
					!NotasCredito.IVAs.size() &&
					!NotasCredito.PercepcionesIVA.size() &&
					!NotasCredito.PercepcionesGenerales.size() &&
					!NotasCredito.PercepcionesPorDescripcion.size();
		}

		void Clear()
		{
			DocFiscales.IVAs.clear();
			DocFiscales.PercepcionesIVA.clear();
			DocFiscales.PercepcionesGenerales.clear();
			DocFiscales.MontoTotal = 0;
			DocFiscales.PercepcionesPorDescripcion.clear();
			NotasCredito.IVAs.clear();
			NotasCredito.PercepcionesIVA.clear();
			NotasCredito.PercepcionesGenerales.clear();
			NotasCredito.MontoTotal = 0;
			NotasCredito.PercepcionesPorDescripcion.clear();
		}
	};

	// Constructor / Destructor
	ImpresorFiscal(Protocolos);
	virtual ~ImpresorFiscal();

	// Métodos locales
	virtual std::string DescripcionModelo () const = 0;

	unsigned EstablecerPuertoSerie (unsigned nPuerto) throw(Excepcion);
	void EstablecerPuertoSerie (unsigned nPuerto,
								unsigned Velocidad) throw(Excepcion);
	void EstablecerSocketTCP   (const std::string &DireccionRemota,
								unsigned short PuertoRemoto) throw(Excepcion);
	void EstablecerSocketUDP   (const std::string &DireccionRemota,
								unsigned short PuertoRemoto) throw(Excepcion);
	void ConfigurarParametrosProtocolo(unsigned Reintentos, unsigned TiempoDeEspera);
	void FinalizarComunicaciones();

	void EstablecerManejadorDeEventos (ManejadorDeEventos *pEventos);
	virtual std::string DescripcionDeEvento (TiposDeEvento Tipo, unsigned SubTipo = 0);

	virtual void CambiarVelocidad (unsigned Velocidad) throw(Excepcion);

	bool ModoStatPRN() const;

	void ModoAnsi (bool Modo);
	bool ModoAnsi () const;

	void ReintentarErroresImpresora(bool Reintentar) throw(Excepcion);
	bool ReintentarErroresImpresora() const throw(Excepcion);

	void ReintentarErroresComunicacion(bool Reintentar);
	bool ReintentarErroresComunicacion() const;

	void ReintentarUltimoErrorImpresora() throw(Excepcion);

	unsigned ContadorImpresoraOcupada () const;
	unsigned ContadorStatPRN() const throw(Excepcion);

	void Abortar();

	void EstablecerModoDeDisplay (ModosDeDisplay Modo);
 	ModosDeDisplay ObtenerModoDeDisplay () const;

	void EstablecerModalidadDescripcionesLargas (bool Modo);
	bool ObtenerModalidadDescripcionesLargas ();

	void EstablecerModalidadPrecioBase (bool PrecioBase);
	bool ObtenerModalidadPrecioBase ();

	void EstablecerModalidadDeImpuestosInternos (ModosDeImpuestosInternos Modo);
	ModosDeImpuestosInternos ObtenerModalidadDeImpuestosInternos ();

	virtual void EstablecerModalidadResumenIVA (TiposDeResumenIVA Modo);
	TiposDeResumenIVA ObtenerModalidadResumenIVA ();

	bool HuboErrorMecanico ();
	bool HuboFaltaPapel ();
	bool HuboStatPRN() throw(Excepcion);

	std::string ObtenerUltimaRespuesta (unsigned Campo = 0) throw (Excepcion);
	void EnviarComandoCrudo (const std::string &Comando) throw (Excepcion);

	void EstablecerNombreArchivoLog (const std::string &Nombre);
	std::string ObtenerNombreArchivoLog ();

	// Métodos generales
	void EnviarComandoDePedidoDeEstado () throw (Excepcion);
	virtual std::string ObtenerVersionDelControlador () throw (Excepcion);
	virtual void ObtenerDatosDeInicializacion (RTA_ObtenerDatosDeInicializacion *R = NULL) throw (Excepcion);
	virtual void ObtenerDatosMemoriaDeTrabajo (RTA_ObtenerDatosMemoriaDeTrabajo *R = NULL) throw (Excepcion);

	virtual void BajaFiscal (const std::string &NumeroDeSerie = "") throw(Excepcion) = 0;

	virtual void CambiarResponsabilidadIVA (const TiposDeResponsabilidadesImpresor RespIVA) throw(Excepcion);
	virtual void CambiarCodigoIngresosBrutos (const std::string &CodigoIngBr) throw(Excepcion) = 0;
	virtual void CambiarFechaInicioActividades (const FECHA &Fecha) throw(Excepcion) = 0;

	bool EstadoImpresor (EstadosImpresor Estado, bool ActualizarEstado = false) throw (Excepcion);
	std::string DescripcionEstadoImpresor (EstadosImpresor Estado) throw(Excepcion);
	std::string DescripcionBitsEstadoImpresor(unsigned short Flags)  throw (Excepcion);

	bool EstadoFiscal (EstadosFiscal Estado, bool ActualizarEstado = false) throw (Excepcion);
	std::string DescripcionEstadoFiscal (EstadosFiscal Estado) throw(Excepcion);
	std::string DescripcionBitsEstadoFiscal(unsigned short Flags)  throw (Excepcion);

	virtual TiposDeEstadoControlador EstadoControlador () throw(Excepcion) = 0;
	virtual std::string DescripcionEstadoControlador () throw(Excepcion) = 0;

	virtual TiposDeDocumento DocumentoEnCurso () throw(Excepcion) = 0;
	virtual std::string DescripcionDocumentoEnCurso () throw(Excepcion) = 0;

	virtual bool UltimoDocumentoFueCancelado () throw(Excepcion) = 0;

	unsigned UltimoDocumentoFiscalBC () throw(Excepcion);
	virtual unsigned UltimoDocumentoFiscalA () throw(Excepcion) = 0;
	virtual unsigned UltimaNotaCreditoBC () throw(Excepcion) = 0;
	virtual unsigned UltimaNotaCreditoA () throw(Excepcion) = 0;
	virtual unsigned UltimoRemito () throw(Excepcion) = 0;

	virtual unsigned PrimerNumeroDeDocumentoActual () throw(Excepcion) = 0;
	virtual unsigned PaginasDeUltimoDocumento () throw(Excepcion);

	virtual void CAIUltimoDocumentoA (RTA_CAIUltimoDocumentoA *R = NULL) throw (Excepcion);

	bool CajonAbierto () throw(Excepcion);
	virtual void AbrirCajon () throw(Excepcion) = 0;
	virtual void AvanzarPapel (TiposDePapel Papel, unsigned Lineas) throw (Excepcion) = 0;
	virtual void CortarPapel () throw (Excepcion);
	virtual void MostrarMensajeEnDisplay (const std::string &Texto,
								const TiposDeDisplay Destino) throw(Excepcion);

	void EstablecerFechaHoraFiscal (const FECHA &fecha, const HORA &hora) throw(Excepcion);
	void ObtenerFechaHoraFiscal (FECHA &fecha, HORA &hora) throw(Excepcion);

	virtual void DatosCliente (	const std::string &RazonSocial,
								const std::string &NroDocumento,
								TiposDeDocumentoCliente TipoDocumento,
								TiposDeResponsabilidadesCliente ResponsabilidadIVA,
								const std::string &Direccion = "") throw(Excepcion) = 0;

	virtual void ImprimirCodigoDeBarras (TiposDeCodigoDeBarras Tipo,
								const std::string &Codigo,
								bool ImprimeNumeros,
								bool ImprimeAhora)  throw(Excepcion);

	virtual void CancelarComprobanteFiscal () throw(Excepcion) = 0;
	virtual void CancelarComprobante () throw(Excepcion) = 0;
	virtual void ReimprimirComprobante () throw(Excepcion);

	virtual void CargarLogo (const std::string &ArchivoBMP = "") throw(Excepcion);

	virtual void CapacidadDocumentoSlip (bool &SoportaFA, bool &SoportaNC) = 0;
	virtual void CapacidadDocumentoTicket (bool &SoportaFA, bool &SoportaNC) = 0;
	virtual void CapacidadEstacion (bool &SoportaTicket, bool &SoportaSlip) = 0;
	virtual void CapacidadCajon (bool &SoportaCajon) = 0;
	void CapacidadLongitudTextoFiscal (unsigned &TextTicketSize, unsigned &TextSlipSize);
	void CapacidadLongitudTextoVenta (unsigned &TextTicketSize, unsigned &TextSlipSize);
	void CapacidadLongitudTextoNoFiscal (unsigned &TextTicketSize, unsigned &TextSlipSize);
	void CapacidadLongitudTextoEncabezadoCola (unsigned &TextTicketSize, unsigned &TextSlipSize);
	void CapacidadDFSoportado (unsigned Tipo, bool &Soportado);
	void CapacidadDNFHSoportado (unsigned Tipo, bool &Soportado);

	virtual void EstadoInternoImpresor (RTA_EstadoInternoImpresor *R = NULL) throw (Excepcion);

	virtual void EstablecerInterlineado (const unsigned LineasPorPulgada) throw (Excepcion);
	virtual unsigned ObtenerInterlineado () throw (Excepcion);
	virtual void ApagarIndicadorDeTambor () throw (Excepcion);

	// Métodos de configuración
	virtual void EspecificarNombreDeFantasia (unsigned Linea, const std::string &Texto) throw (Excepcion);
	virtual std::string ObtenerNombreDeFantasia (unsigned Linea) throw (Excepcion) = 0;

	void EspecificarEncabezadoCola (unsigned Linea, const std::string &Texto) throw (Excepcion);
	std::string ObtenerEncabezadoCola (unsigned Linea) throw (Excepcion);

	virtual void BorrarFantasiaEncabezadoCola (bool BorrarFantasia, bool BorrarEncabezado, bool BorrarCola) throw (Excepcion);

	virtual void EspecificarInformacionRemitoComprobanteOriginal (unsigned Linea, const std::string &Texto) throw (Excepcion) = 0;
	virtual std::string ObtenerInformacionRemitoComprobanteOriginal (unsigned Linea) throw (Excepcion) = 0;

	virtual void BorrarInformacionRemitoComprobanteOriginal () throw (Excepcion) = 0;

	virtual void ConfigurarControlador (ParametrosDeConfiguracion Parametro, const std::string &Valor) throw (Excepcion);
	virtual void ConfigurarControladorPorBloque (
							double LimiteConsumidorFinal,
							double LimiteTicketFactura,
							double PorcentajeIVANoInscripto,
							NumerosDeCopias TipoDeCopiasMaximo,
							bool ImpresionCambio,
							bool ImpresionLeyendasOpcionales,
							TiposDeCorteDePapel TipoDeCorte) throw (Excepcion);
	virtual void ConfigurarControladorCompleto (
							bool Imprimir,
							bool Defaults,
							const double *LimiteConsumidorFinal,
							const double *LimiteTicketFactura,
							const double *PorcentajeIVANoInscripto,
							const NumerosDeCopias *TipoDeCopiasMaximo,
							const bool *ImprimeCambio,
							const bool *ImprimeLeyendasOpcionales,
							const TiposDeCorteDePapel *TipoDeCorte,
							const bool *ImprimeMarco,
							const bool *ReImprimeDocumentos,
							const std::string *DescripcionDelMedioDePago,
							const bool *Sonido,
							const TiposDeAltoHoja *AltoHoja,
							const TiposDeAnchoHoja *AnchoHoja,
							const TiposDeEstacion *EstacionImpresionReportesXZ,
							const TiposDeModoImpresion *ModoImpresion) throw (Excepcion);
	virtual void ObtenerConfiguracion (RTA_ObtenerConfiguracion *R = NULL) throw (Excepcion) = 0;

	// Métodos de reportes
	virtual void ReporteZ (RTA_ReporteZX *R = NULL) throw (Excepcion);
	virtual void ReporteX (RTA_ReporteZX *R = NULL) throw (Excepcion);
	void ReporteZPorFechas (FECHA FIni, FECHA FFin, bool ReporteGlobal)  throw (Excepcion);
	void ReporteZPorNumeros (unsigned ZIni, unsigned ZFin, bool ReporteGlobal)  throw (Excepcion);
	virtual void ReporteZIndividualPorFecha (FECHA FechaZ, RTA_ReporteZIndividual *R = NULL) throw (Excepcion) = 0;
	virtual void ReporteZIndividualPorNumero (unsigned NumeroZ, RTA_ReporteZIndividual *R = NULL) throw (Excepcion) = 0;
	void CapacidadRestante (RTA_CapacidadRestante *R = NULL) throw (Excepcion);
	virtual void ObtenerTablaIVAs (TablaIVAs &Tabla) throw (Excepcion);

	// Métodos de documentos fiscales
	virtual void AbrirDF (DocumentosFiscales Tipo) throw (Excepcion);
	void ImprimirTextoFiscal (const std::string &Texto) throw (Excepcion);
	virtual void ImprimirItem (const std::string &Texto,
							double Cantidad, double Monto,
							double IVA, double ImpuestosInternos,
							bool EnNegativo = false) throw (Excepcion);
	virtual void DescuentoUltimoItem (const std::string &Texto, double Monto, bool EnNegativo = false) throw (Excepcion);
	virtual void DescuentoGeneral (const std::string &Texto, double Monto, bool EnNegativo = false) throw (Excepcion);
	virtual void DevolucionDescuento (const std::string &Texto,
							double Monto, double IVA,
							double ImpuestosInternos,
							bool EnNegativo,
							TiposDeDescuento Tipo) throw (Excepcion);
	virtual void EspecificarIVANoInscripto (double Monto) throw (Excepcion) = 0;
	virtual void EspecificarPercepcionGlobal (const std::string &Nombre, double Monto) throw (Excepcion) = 0;
	virtual void EspecificarPercepcionPorIVA (const std::string &Nombre, double Monto, double IVA) throw (Excepcion) = 0;
	virtual void Subtotal (bool Imprime, RTA_Subtotal *R = NULL) throw (Excepcion);
	virtual double ImprimirPago (const std::string &Texto, double Monto, const std::string &DescripcionAdicional = "") throw (Excepcion);
	virtual double DevolverPago (const std::string &Texto, double Monto, const std::string &DescripcionAdicional = "") throw (Excepcion);
	virtual unsigned CerrarDF (unsigned Copias = 1) throw (Excepcion) = 0;

	// Métodos de documentos no fiscales
	virtual void AbrirDNF (TiposDeEstacion EstacionDeImpresion) throw (Excepcion);
	void ImprimirTextoNoFiscal (const std::string &Texto = " ") throw (Excepcion);
	virtual void CerrarDNF (unsigned Copias = 1) throw (Excepcion) = 0;

	// Métodos de documentos no fiscales homologados
	virtual void AbrirDNFH (DocumentosNoFiscalesHomologados Tipo, const std::string &Nro = "") throw (Excepcion) = 0;
	virtual void ImprimirItemEnRemito (const std::string &Texto, double Cantidad) throw (Excepcion);
	virtual void ImprimirItemEnCuenta (const FECHA &FechaComprobante,
							const std::string &NroComprobante,
							const std::string &Texto,
							double Debe, double Haber) throw (Excepcion);
	virtual void ImprimirItemEnCotizacion (const std::string &Texto) throw (Excepcion);
	virtual void DetalleRecibo (const std::string &Texto = " ") throw (Excepcion);
	virtual void ImprimirInfoEnDNFH (const unsigned IDCampo, const std::string &Texto, double Cantidad = 0.0) throw (Excepcion);
	virtual void ImprimirFirmaEnCierreDNFH (bool ImprimeFirma) throw (Excepcion);
	virtual unsigned CerrarDNFH (unsigned Copias = 1) throw (Excepcion) = 0;

	virtual void DNFHFarmacia (unsigned Copias = 1) throw (Excepcion) = 0;
	virtual void DNFHReparto (unsigned Copias = 1) throw (Excepcion) = 0;
	virtual void ImprimirVoucher (const std::string &NombreCliente,
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
							TiposDeEstacion Estacion = ImpresorFiscal::ESTACION_TICKET) throw (Excepcion) = 0;
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
							unsigned NumeroComprobanteAsociado,
							unsigned Copias = 1,
							const std::string &Vendedor = "",
							TiposDeEstacion Estacion = ImpresorFiscal::ESTACION_TICKET) throw (Excepcion);
	virtual void ImprimirVoucher (const std::string &NombreCliente,
							const std::string &NombreTarjeta,
							TiposDeVouchers TipoVoucher,
							const std::string &NumeroDeTarjeta,
							const std::string &FechaDeVencimiento,
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
	void ImprimirVoucher (const std::string &NombreCliente,
							const std::string &NombreTarjeta,
							TiposDeVouchers TipoVoucher,
							const std::string &NumeroDeTarjeta,
							const std::string &FechaDeVencimiento,
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
							unsigned NumeroComprobanteAsociado,
							unsigned Copias = 1,
							const std::string &Vendedor = "",
							TiposDeEstacion Estacion = ImpresorFiscal::ESTACION_TICKET) throw (Excepcion);

// Constantes

	static const std::string FS;
	static const std::string ERASE_LINE;
	static const std::string DOBLE_ANCHO;

protected:

	static const unsigned First_Fantasy_Line;
	static const unsigned Last_Fantasy_Line;

	static const unsigned First_Header_Line;
	static const unsigned Last_Header_Line;
	static const unsigned First_Trailer_Line;
	static const unsigned Last_Trailer_Line;

	static const unsigned First_EmbarkNum_Line;
	static const unsigned Last_EmbarkNum_Line;


	// Definiciones de Tamaños de Campos
	// Aca se declaran todas las variables de este tipo.
	// Después, en el constructor de la clase ImpresorFiscal
	// se inicializan todas las variables cuyo valor es común
	// para todos los modelos y de ahi para abajo en cada clase
	// ImpresorFiscalxxx se inicializan explicitamente solo las
	// que toman valores diferentes a los default.

	unsigned MaxFiscalTextLinesInvoice;
	unsigned MaxFiscalTextLinesTicket;

	unsigned PrintNonFiscalTextTicketSize;
	unsigned PrintNonFiscalTextSlipSize;
	unsigned FantasyNameSize;
	unsigned HeaderTrailerSize;
	unsigned HeaderTrailerTicketSize;
	unsigned HeaderTrailerSlipSize;
	unsigned InfoRemitoCompOriSize;
	unsigned SerialNumberSize;
	unsigned CodeIngBrSize;
	unsigned CodeBarMaxSize;
	unsigned RazonSocialSize;
	unsigned DireccionSize;
	unsigned PrintFiscalTextTicketSize;
	unsigned PrintFiscalTextSlipSize;
	unsigned PrintItemTextTicketSize;
	unsigned PrintItemTextSlipSize;
	unsigned PriceDecimals;
	unsigned PerceptionDescSize;
	unsigned TotalTenderTextSize;
	unsigned DNFHNumSize;
	unsigned RemitoTextSize;
	unsigned RemitoCantDecimals;
	unsigned ResumeTextSize;
	unsigned CotizaTextSize;
	unsigned ReciboTextTicketSize;
	unsigned ReciboTextSlipSize;
	unsigned ReciboLines;
	unsigned PrintDNFHTextSize;
	unsigned VouchCustNameSize;
	unsigned VouchCardNameSize;
	unsigned VouchCardNumSize;
	unsigned VouchComCodeSize;
	unsigned VouchImportSize;
	unsigned VouchCompNumSize;
	unsigned VouchVendNameSize;
	unsigned DisplayTextSize;


	// --------------------------------------------- //
	// ---	C o m a n d o s     F i s c a l e s  --- //
	// --------------------------------------------- //

	enum OpCodes
	{
		CMD_STATUS_REQUEST					= 0x2a,	// Pedido de Estado  del CF     ...
		CMD_STATUS_PJL						= 0x2b,	// Status interno      (PL23)	...
//		CMD_EJECT_PAGE						= 0x2f,	// Eyección de hoja    (HP1200)	...

		CMD_HISTORY_CAPACITY				= 0x37,	// Reporte Cierres Z Remanentes ...
		CMD_DAILY_CLOSE						= 0x39,	// Cierre de Jornada Fiscal	Z/X ...
		CMD_DAILY_CLOSE_BY_DATE				= 0x3a,	// Reporte de Audit. por Fecha	...
		CMD_DAILY_CLOSE_BY_NUMBER			= 0x3b,	// Reporte de Audit. por Z		...
		CMD_GET_DAILY_REPORT				= 0x3c,	// Reporte Cierre Z individual	...

		CMD_OPEN_FISCAL_RECEIPT				= 0x40,	// Apertura de DF               ...
		CMD_PRINT_FISCAL_TEXT				= 0x41,	// Línea Desc. Item adicional   ...
		CMD_PRINT_LINE_ITEM					= 0x42,	// Venta de un Item             ...
		CMD_SUBTOTAL						= 0x43,	// Subtotal						...
		CMD_TOTAL_TENDER					= 0x44,	// Total - Pago	- Cancelación   ...
		CMD_CLOSE_FISCAL_RECEIPT			= 0x45,	// Cierre   de DF               ...

		CMD_OPEN_NON_FISCAL_SLIP			= 0x47,	// Apertura de DNF en Slip      ...
		CMD_OPEN_NON_FISCAL_RECEIPT			= 0x48,	// Apertura de DNF en Ticket    ...
		CMD_PRINT_NON_FISCAL_TEXT			= 0x49,	// Impresión Texto No Fiscal    ...
		CMD_CLOSE_NON_FISCAL				= 0x4a,	// Cierre   de DNF              ...

		CMD_CUT_NON_FISCAL_RECEIPT			= 0x4b,	// Activación del Cutter		...
		CMD_FEED_RECEIPT					= 0x50,	// Avanzar Receipt              ...
		CMD_FEED_JOURNAL					= 0x51,	// Avanzar Journal		        ...
		CMD_FEED_RECEIPT_JOURNAL			= 0x52,	// Avanzar Receipt y Journal    ...
		CMD_FEED_SLIP						= 0x53,	// Avanzar Slip                 ...

		CMD_GENERAL_DISCOUNT				= 0x54,	// Descuento general		    ...
		CMD_LAST_ITEM_DISCOUNT				= 0x55,	// Descuento sobre último item  ...

//		CMD_PRINT_MODE						= 0x57,	// Setea Modo Impresión         ...

		CMD_SET_DATE_TIME					= 0x58,	// Setear fecha y hora			...
		CMD_GET_DATE_TIME					= 0x59,	// Obtener la conf. de hora		...
		CMD_PRINT_BARCODE					= 0x5a,	// Setear/Imprimir C¢d. Barras	...
		CMD_SET_HEADER_TRAILER				= 0x5d,	// Setear  encabezamiento/cola  ...
		CMD_GET_HEADER_TRAILER				= 0x5e,	// Obtener encabezamiento/cola  ...
		CMD_SET_FANTASY_NAME				= 0x5f,	// Setear nombre de fantas¡a    ...

		CMD_SET_PERCEPTION					= 0x60,	// Carga Percepciones 	        ...
		CMD_CHARGE_NON_REGISTERED_TAX		= 0x61,	// Carga Monto Iva No Inscripto ...

		CMD_SET_CUSTOMER_DATA				= 0x62,	// Setear Datos del Cliente	    ...

		CMD_CHG_IVA_RESP					= 0x63,	// Cambio Responsabilidad IVA   ...

		CMD_CONFIGURE_CONTROLLER_BY_ONE		= 0x64,	// Setear  Config. Individual   ...
		CMD_CONFIGURE_CONTROLLER_BY_BLOCK	= 0x65,	// Setear  Config. General      ...
		CMD_GET_CONFIGURATION_DATA			= 0x66,	// Obtener Config. General      ...

		CMD_GET_WORKING_MEMORY				= 0x67,	// Obtener Info Memoria Trabajo ...

		CMD_DNFH_FARMACIAS					= 0x68,	// Emitir DNFH de Farmacias     ...
		CMD_DNFH_REPARTO					= 0x69,	// Emitir DNFH de Reparto       ...

		CMD_SET_VOUCHER_DATA_1				= 0x6a,	// Carga Inicial Datos Voucher  ...
		CMD_SET_VOUCHER_DATA_2				= 0x6b,	// Carga Final   Datos Voucher  ...
		CMD_PRINT_VOUCHER					= 0x6c,	// Impresi¢n de Voucher         ...

		CMD_RETURN_RECHARGE					= 0x6d,	// Bonificacion, Recargo, Dev.  ...

		CMD_CHG_ING_BR						= 0x6e,	// Cambio de Ingresos Brutos    ...

//		CMD_PRINT_CHECK						= 0x6f,	// Impresión de Cheque			...

		CMD_SEND_FIRST_IVA					= 0x70,	// Obtener Inicio info IVA      ...
		CMD_SEND_NEXT_IVA					= 0x71,	// Obtener Resto  info IVA      ...
//		CMD_ABORT_IVA						= 0x72,	// Abortar obtener info IVA     ...

		CMD_GET_INIT_DATA					= 0x73,	// Obtener Datos Inicialización ...

		CMD_SET_CONFIG_PAGE					= 0x74,	// Setear  conf. hoja  (PL23)	...
		CMD_GET_CONFIG_PAGE					= 0x75,	// Obtener conf. hoja  (PL23)	...

//		CMD_SET_CFG_MONEY					= 0x76,	// Setear  conf. Moneda Prog.	...
//		CMD_GET_CFG_MONEY					= 0x77,	// Obtener conf. Moneda Prog.	...

		CMD_CHG_INI_ACT_DATE				= 0x78,	// Cambio Fecha Inicio Activ.   ...

// 		CMD_TRAINING_OFF					= 0x79,	// Deshabilita modo entrenam.   ...

//		CMD_FORMAT_EPROM_FISCAL				= 0x7a,	// Formatear la Memoria Fiscal  ...

		CMD_OPEN_DRAWER						= 0x7b,	// Apertura de Cajón de Dinero  ...
//		CMD_OPEN_DRAWER_2					= 0x7c,	// Abrir cajón de dinero 2  	...

		CMD_GET_CAI_LAST_DOC_A				= 0x7e,	// Obtener CAI Ult. Documento A	...

		CMD_GET_VERSION						= 0x7f,	// Obtener la Versión del CF    ...

		CMD_OPEN_DNFH						= 0x80,	// Apertura de DNFH				...
		CMD_CLOSE_DNFH						= 0x81,	// Cierre   de DNFH				...

		CMD_PRINT_EMBARK_ITEM				= 0x82,	// Item Remito / Orden salida	...
		CMD_PRINT_ACCOUNT_ITEM				= 0x83,	// Item resumen cuenta			...
		CMD_PRINT_QUOTATION_ITEM			= 0x84,	// Cotización					...
		CMD_PRINT_INFO_DNFH					= 0x85,	// Imprimir texto en DNFH nuevo ...
		CMD_PRINT_SIGN_DNFH					= 0x86,	// Imprimir firma en cierre de DNFH nuevo ...

		CMD_LOAD_LOGO						= 0x90,	// Cargar logo clt./fondo Hoja	...
		CMD_RESET_LOGO						= 0x91,	// Borrar logo clt./fondo Hoja	...

		CMD_GET_FANTASY_NAME				= 0x92,	// Obtener nombre de fantas¡a   ...

		CMD_SET_EMBARK_NUMBER				= 0x93,	// Setear  número de remito		...
		CMD_GET_EMBARK_NUMBER				= 0x94,	// Obtener número de remito		...

		CMD_SET_CONFIG_FULL					= 0x95,	// Setear  la conf. completa	...
		CMD_GET_CONFIG_FULL					= 0x96,	// Obtener la conf. completa	...

		CMD_RECEIPT_TEXT					= 0x97,	// Setea lineas texto en recibo	...

		CMD_CANCEL							= 0x98,	// Cancelar un documento		...
		CMD_REPRINT							= 0x99,	// Copiar ultimo documento		...

		CMD_CHANGE_COM_SPEED				= 0xa0,	// Cambio de velocidad		...
		CMD_STATPRN			  				= 0xa1,	// Status del printer		...

		CMD_INIT_EPROM						= 0xb0,	// Inicializaci¢n del CF	...
		CMD_KILL_EPROM						= 0xb1,	// Baja           del CF    ...

		CMD_WRITE_DISPLAY					= 0xb2	// Escribir en Display		...
	};


// Propiedades

	ProtocoloFiscal *Protocolo;

	ManejadorDeEventos *pfEventos;

	bool						PrinterError;
	bool						DrawerClosed;
	bool						OutOfPaper;
	ModosDeDisplay				ModoDisplay;

	bool						TicketBCAbierto;

	TiposDeEstacion				Estacion;

	bool						DescripcionesLargas;

	ModosDePrecio				ModalidadPrecio;
	ModosDeImpuestosInternos	ModalidadII;

	TiposDeResumenIVA			ModalidadResumenIVA;

	unsigned					FirstDocNum;
	unsigned 					NumberOfPages;

	std::vector<std::string>	VectorRespuesta;

	std::set<unsigned>			DFSoportados;
	std::set<unsigned>			DNFHSoportados;

	LogFile Log;

// Métodos

	unsigned xtoi (const char *num, unsigned size) const;

	std::string Numero (double f, unsigned Decimales = 0) const;
	std::string Numero (unsigned n) const;
	template<typename T> std::string Caracter (const T &c) const
	{
		return std::string(1, char(c));
	}
	std::string Cadena (const std::string &_s, unsigned max, unsigned min = 1, bool JustifIzq = true) const;
	std::string OpCode (OpCodes) const;

	std::string RespuestaString(unsigned Campo, std::string Contexto) throw(Excepcion);
	unsigned RespuestaInt(unsigned Campo, std::string Contexto) throw(Excepcion);
	double RespuestaDouble(unsigned Campo, std::string Contexto) throw(Excepcion);

	inline unsigned Mascara(unsigned bit) {	return 1 << bit; }

	void ProbarTransporte() throw(Excepcion);

	virtual void Evento(unsigned Tipo, unsigned SubTipo);
	void RaiseEvent(TiposDeEvento Tipo, unsigned SubTipo);

	void EnviarComandoFiscal(const std::string &Comando) throw (Excepcion);
	void EvaluarRespuesta(unsigned short &ErrorFiscal);

	void SolicitarEstado () throw (Excepcion);

	TiposDeEstadoControlador EstadoCF () throw(Excepcion);
	std::string DescripcionEstadoCF () throw(Excepcion);

	TiposDeDocumento DocumentoCF () throw(Excepcion);
	std::string DescripcionDocumentoCF () throw(Excepcion);

	bool IndicadorDeDocumentoPrevioCancelado () throw(Excepcion);

	virtual std::string IndicadorII (double ImpuestosInternos, double Precio = 0.0, double IVA = 0.0) throw(Excepcion) = 0;

	ImpresorFiscal::IdentificadoresDeDNFH IdentificadorDeDNFH (DocumentosNoFiscalesHomologados Tipo);
};


#endif
