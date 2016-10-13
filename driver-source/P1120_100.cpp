#include "P1120_100.h"


std::string ImpresorFiscalP1120_100::Version ("HASAR SMH/P-1120F V: 01.00" /* " - 15/09/2008" */);


// ############################################################ //
//																//
//			M E T O D O S      L O C A L E S					//
//																//
// ############################################################ //

//
// Constructor
//
ImpresorFiscalP1120_100::ImpresorFiscalP1120_100 ()
{
	// Inicializaci�n de variables de uso general

	// Inicializaci�n de variables de tama�o de campos
	PrintNonFiscalTextTicketSize	= 120;	// Por seguridad !!!
	PrintFiscalTextTicketSize		= 50;	// Por seguridad !!!
	PrintItemTextTicketSize			= 50;	// Por seguridad !!!
	RemitoCantDecimals				= 10;
	ReciboTextTicketSize			= 106;	// Por seguridad !!!

	// Seteamos modelo con anomal�a de comunicaciones ...
	Protocolo->AnomaliaComunicaciones(true);

	// Cargamos los DF soportados
	DFSoportados.insert (FACTURA_A);
	DFSoportados.insert (FACTURA_B);
	DFSoportados.insert (NOTA_DEBITO_A);
	DFSoportados.insert (NOTA_DEBITO_B);
	DFSoportados.insert (RECIBO_A);
	DFSoportados.insert (RECIBO_B);

	// Cargamos los DNFH soportados
	DNFHSoportados.insert (NOTA_CREDITO_A);
	DNFHSoportados.insert (NOTA_CREDITO_B);
	DNFHSoportados.insert (REMITO);
	DNFHSoportados.insert (RECIBO_X);
	DNFHSoportados.insert (ORDEN_SALIDA);
	DNFHSoportados.insert (RESUMEN_CUENTA);
	DNFHSoportados.insert (CARGO_HABITACION);
	DNFHSoportados.insert (COTIZACION);
	DNFHSoportados.insert (CLAUSULA_CREDITO);
	DNFHSoportados.insert (CLAUSULA_SEGURO);
	DNFHSoportados.insert (PAGARE);
	DNFHSoportados.insert (POLIZA_SEGURO);
	DNFHSoportados.insert (RECORDATORIO);
	DNFHSoportados.insert (SOLICITUD_CREDITO);
	DNFHSoportados.insert (COMUNICACION_CLIENTE);
	DNFHSoportados.insert (OFRECIMIENTO_CREDITO);
	DNFHSoportados.insert (OFRECIMIENTO_TARJETA);
	DNFHSoportados.insert (MINUTA_CREDITO);
	DNFHSoportados.insert (OFRECIMIENTO_PASAPORTE);
	DNFHSoportados.insert (RENOVACION_CREDITO);
	DNFHSoportados.insert (ADELANTO_REMUNERACION);
	DNFHSoportados.insert (SOLICITUD_TARJETA_DEBITO);
	DNFHSoportados.insert (SOLICITUD_CLAVE_TARJETA);
	DNFHSoportados.insert (RESCATE_MERCADERIA);
	DNFHSoportados.insert (INGRESO_EGRESO_SUCURSAL);
}


//
// Obtener la Descripci�n del Modelo Seleccionado
//
std::string
ImpresorFiscalP1120_100::DescripcionModelo () const
{
	return 	(Version);
}


//
// Establecer Modalidad Resumen IVA
//
void
ImpresorFiscalP1120_100::EstablecerModalidadResumenIVA (TiposDeResumenIVA Modo)
{
	ModalidadResumenIVA = Modo;
}


// ############################################################ //
//																//
//			M E T O D O S   G E N E R A L E S					//
//																//
// ############################################################ //

//
// Obtener Datos de Memoria de Trabajo
//
void
ImpresorFiscalP1120_100::ObtenerDatosMemoriaDeTrabajo (RTA_ObtenerDatosMemoriaDeTrabajo *R) throw (Excepcion)
{
    // printf ("Comando ObtenerDatosMemoriaDeTrabajo ejecutado en P1120_100\n");

	// Ejecutamos el m�todo de la superclase 'ImpresorFiscal16Bits'
	ImpresorFiscal16Bits::ObtenerDatosMemoriaDeTrabajo (R);

	// Completamos los par�metros correspondientes a los campos
	// de respuesta particulares que se agregan en este modelo
	// m�s alla de los comunes a todos los modelos
	if (R != NULL)
	{
		R->CantidadNCCanceladas				= RespuestaInt (20, "Obtener Datos de Memoria de Trabajo");
		R->CantidadDFBCEmitidos				= RespuestaInt (21, "Obtener Datos de Memoria de Trabajo");
		R->CantidadDFAEmitidos				= RespuestaInt (22, "Obtener Datos de Memoria de Trabajo");
		R->CantidadNCBCEmitidas				= RespuestaInt (23, "Obtener Datos de Memoria de Trabajo");
		R->CantidadNCAEmitidas				= RespuestaInt (24, "Obtener Datos de Memoria de Trabajo");
	}
}


// ############################################################ //
//																//
//			M E T O D O S   D E    C O N F I G U R A C I O N	//
//																//
// ############################################################ //

//
// Borrar L�neas de Nombre de Fantas�a, Encabezado y Cola
//
void
ImpresorFiscalP1120_100::BorrarFantasiaEncabezadoCola (bool BorrarFantasia, bool BorrarEncabezado, bool BorrarCola) throw (Excepcion)
{
	// printf ("Comando BorrarFantasiaEncabezadoCola ejecutado en P1120_100\n");

	if ( BorrarFantasia )
		EnviarComandoFiscal(OpCode(CMD_SET_FANTASY_NAME) + FS + "0" + FS + ERASE_LINE);

	if ( BorrarEncabezado )
	{
		if ( BorrarCola )
			EnviarComandoFiscal(OpCode(CMD_SET_HEADER_TRAILER) + FS + "0" + FS + ERASE_LINE);
		else
			EnviarComandoFiscal(OpCode(CMD_SET_HEADER_TRAILER) + FS + "-1" + FS + ERASE_LINE);
	}
	else
		if ( BorrarCola )
			EnviarComandoFiscal(OpCode(CMD_SET_HEADER_TRAILER) + FS + "-2" + FS + ERASE_LINE);
}


//
// Borrar Informaci�n L�nea de Remito / Comprobante Original
//
void
ImpresorFiscalP1120_100::BorrarInformacionRemitoComprobanteOriginal () throw (Excepcion)
{
	// printf ("Comando BorrarInformacionRemitoComprobanteOriginal ejecutado en P1120_100\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_SET_EMBARK_NUMBER) + FS + "0" + FS + ERASE_LINE);
}


//
// Obtener Configuracion del Controlador
//
void
ImpresorFiscalP1120_100::ObtenerConfiguracion (RTA_ObtenerConfiguracion *R) throw (Excepcion)
{
	// printf ("Comando ObtenerConfiguracion ejecutado en P1120_100\n");

	// Ejecutamos el m�todo de la superclase 'ImpresorFiscal16Bits'
	ImpresorFiscal16Bits::ObtenerConfiguracion (R);

	// Completamos los par�metros correspondientes a los campos
	// de respuesta particulares que se agregan en este modelo
	if (R != NULL)
	{
		R->AltoHoja						= static_cast<ImpresorFiscal::TiposDeAltoHoja>(RespuestaString (13, "Obtener Configuracion del Controlador") [0]);
		R->ChequeoDesbordeCompleto		= ((RespuestaString (17, "Obtener Configuracion del Controlador") [0]) == 'P');
	}
}


// ############################################################ //
//																//
//			M E T O D O S   D E    R E P O R T E S				//
//																//
// ############################################################ //

//
// Reporte Z
//
void
ImpresorFiscalP1120_100::ReporteZ (RTA_ReporteZX *R) throw (Excepcion)
{
	// printf ("Comando ReporteZ ejecutado en P1120_100\n");

	// Ejecutamos el m�todo de la superclase 'ImpresorFiscal16Bits'
	ImpresorFiscal16Bits::ReporteZ (R);

	// Completamos los par�metros correspondientes a los campos de
	// respuesta particulares que se agregan en este modelo
	if (R != NULL)
	{
		R->CantidadNCCanceladas				= RespuestaInt (23, "Reporte Z");
		R->CantidadDFBCEmitidos				= RespuestaInt (24, "Reporte Z");
		R->CantidadDFAEmitidos				= RespuestaInt (25, "Reporte Z");
		R->CantidadNCBCEmitidas				= RespuestaInt (26, "Reporte Z");
		R->CantidadNCAEmitidas				= RespuestaInt (27, "Reporte Z");
	}
}


//
// Reporte X
//
void
ImpresorFiscalP1120_100::ReporteX (RTA_ReporteZX *R) throw (Excepcion)
{
	// printf ("Comando ReporteX ejecutado en P1120_100\n");

	// Ejecutamos el m�todo de la superclase 'ImpresorFiscal16Bits'
	ImpresorFiscal16Bits::ReporteX (R);

	// Completamos los par�metros correspondientes a los campos de
	// respuesta particulares que se agregan en este modelo
	if (R != NULL)
	{
		R->CantidadNCCanceladas				= RespuestaInt (23, "Reporte Z");
		R->CantidadDFBCEmitidos				= RespuestaInt (24, "Reporte Z");
		R->CantidadDFAEmitidos				= RespuestaInt (25, "Reporte Z");
		R->CantidadNCBCEmitidas				= RespuestaInt (26, "Reporte Z");
		R->CantidadNCAEmitidas				= RespuestaInt (27, "Reporte Z");
	}
}


// ############################################################ //
//																//
//			M E T O D O S   D E    D F							//
//																//
// ############################################################ //

//
// Especificar Monto de IVA No Inscripto
//
void
ImpresorFiscalP1120_100::EspecificarIVANoInscripto (double /* Monto */) throw (Excepcion)
{
    // printf ("Comando EspecificarIVANoInscripto ejecutado en P1120_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Especificar Monto de IVA No Inscripto");
}


//
// Devolver un pago previamente almacenado
//
double
ImpresorFiscalP1120_100::DevolverPago (
						const std::string &Texto,
						double Monto,
						const std::string &DescripcionAdicional) throw (Excepcion)
{
	// printf ("Comando DevolverPago ejecutado en P1120_100\n");

	assert(TotalTenderTextSize != 0);

	// Enviamos el comando fiscal y evaluamos los status

	EnviarComandoFiscal(OpCode(CMD_TOTAL_TENDER) + FS +
						Cadena (Texto, TotalTenderTextSize) + FS +
						Numero (Monto, 2) + FS + "R" + FS +
						Caracter (ModoDisplay));

	// Retornamos el Vuelto o Monto por Pagar
	return RespuestaDouble (2, "Devolver Pago");
}


//
// Cerrar DF
//
unsigned
ImpresorFiscalP1120_100::CerrarDF (unsigned Copias) throw (Excepcion)
{
    // printf ("Comando CerrarDF ejecutado en P1120_100\n");

	// Ejecutamos el m�todo de la superclase 'ImpresorFiscal16Bits'
	unsigned NumeroDFRecienEmitido = ImpresorFiscal16Bits::CerrarDF (Copias);

	// Recuperamos el N�mero de Hojas Numeradas del DF
	// reci�n emitido de la respuesta.
	NumberOfPages = RespuestaInt(3, "Cerrar DF");

	// Retornamos el N�mero de DF reci�n emitido
	return NumeroDFRecienEmitido;
}


// ############################################################ //
//																//
//			M E T O D O S   D E    D N F						//
//																//
// ############################################################ //

//
// Abrir DNF
// Env�a el comando de Apertura de DNF en la estaci�n indicada
//
void
ImpresorFiscalP1120_100::AbrirDNF (TiposDeEstacion EstacionDeImpresion) throw (Excepcion)
{
    // printf ("Comando AbrirDNF ejecutado en P1120_100\n");

	// Ejecutamos el m�todo de la SuperClase 'ImpresorFiscal'.
	ImpresorFiscal::AbrirDNF (ESTACION_SLIP);
}


// ############################################################ //
//																//
//			M E T O D O S   D E    D N F H						//
//																//
// ############################################################ //

//
// Cerrar DNFH
//
unsigned
ImpresorFiscalP1120_100::CerrarDNFH (unsigned Copias) throw (Excepcion)
{
    // printf ("Comando CerrarDNFH ejecutado en P1120_100\n");

	// Ejecutamos el m�todo de la superclase 'ImpresorFiscal16Bits'
	unsigned NumeroDNFHRecienEmitido = ImpresorFiscal16Bits::CerrarDNFH (Copias);

	// Recuperamos el N�mero de Hojas Numeradas del DNFH
	// reci�n emitido de la respuesta.
	NumberOfPages = RespuestaInt(3, "Cerrar DNFH");

	// Retornamos el N�mero de DNFH reci�n emitido
	return NumeroDNFHRecienEmitido;
}


