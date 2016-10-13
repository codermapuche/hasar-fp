#include "PPR5_201.h"


std::string ImpresorFiscalPPR5_201::Version ("SMH/P-PR5F - Versión 02.01");


// ############################################################ //
//																//
//			M E T O D O S      L O C A L E S					//
//																//
// ############################################################ //

//
// Constructor
//
ImpresorFiscalPPR5_201::ImpresorFiscalPPR5_201 ()
{
	// Inicialización de variables de uso general

	// Inicialización de variables de tamaño de campos
	PriceDecimals					= 4;
}


//
// Obtener la Descripción del Modelo Seleccionado
//
std::string
ImpresorFiscalPPR5_201::DescripcionModelo () const
{
	return 	(Version);
}


//
// Establecer Modalidad Resumen IVA
//
void
ImpresorFiscalPPR5_201::EstablecerModalidadResumenIVA (TiposDeResumenIVA Modo)
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
ImpresorFiscalPPR5_201::ObtenerDatosMemoriaDeTrabajo (RTA_ObtenerDatosMemoriaDeTrabajo *R) throw (Excepcion)
{
	// printf ("Comando ObtenerDatosMemoriaDeTrabajo ejecutado en PPR5_201\n");

	// Ejecutamos el método de la superclase 'ImpresorFiscalPPR5'
	ImpresorFiscalPPR5::ObtenerDatosMemoriaDeTrabajo (R);

	// Completamos los parámetros correspondientes a los campos
	// de respuesta particulares que se agregan en este modelo
	if (R != NULL)
	{
		R->CantidadDFBCEmitidos				= RespuestaInt (21, "Obtener Datos de Memoria de Trabajo");
		R->CantidadDFAEmitidos				= RespuestaInt (22, "Obtener Datos de Memoria de Trabajo");
		R->CantidadNCBCEmitidas				= RespuestaInt (23, "Obtener Datos de Memoria de Trabajo");
		R->CantidadNCAEmitidas				= RespuestaInt (24, "Obtener Datos de Memoria de Trabajo");
	}
}


//
// Cargar Datos del Cliente
//
void
ImpresorFiscalPPR5_201::DatosCliente (
						const std::string &RazonSocial,
						const std::string &NroDocumento,
						TiposDeDocumentoCliente TipoDocumento,
						TiposDeResponsabilidadesCliente ResponsabilidadIVA,
						const std::string &Direccion) throw(Excepcion)
{
	// printf ("Comando DatosCliente ejecutado en PPR5_201\n");

	assert(RazonSocialSize != 0);
	assert(DireccionSize != 0);

	// Enviamos el comando fiscal y evaluamos los status

	// Verificamos la existencia del parámetro dirección
	// (si no se recibe la dirección, no se incluye el campo en el comando)
	if (Direccion.length() == 0)
		EnviarComandoFiscal(OpCode(CMD_SET_CUSTOMER_DATA) + FS +
							Cadena (RazonSocial, RazonSocialSize) + FS +
							Cadena (NroDocumento, 11) + FS +
							Caracter (ResponsabilidadIVA) + FS +
							Caracter (TipoDocumento));
	else
		EnviarComandoFiscal(OpCode(CMD_SET_CUSTOMER_DATA) + FS +
							Cadena (RazonSocial, RazonSocialSize) + FS +
							Cadena (NroDocumento, 11) + FS +
							Caracter (ResponsabilidadIVA) + FS +
							Caracter (TipoDocumento) + FS +
							Cadena (Direccion, DireccionSize));
}


// ############################################################ //
//																//
//			M E T O D O S   D E    C O N F I G U R A C I O N	//
//																//
// ############################################################ //

//
// Borrar Líneas de Nombre de Fantasía, Encabezado y Cola
//
void
ImpresorFiscalPPR5_201::BorrarFantasiaEncabezadoCola (bool BorrarFantasia, bool BorrarEncabezado, bool BorrarCola) throw (Excepcion)
{
	// printf ("Comando BorrarFantasiaEncabezadoCola ejecutado en PPR5_201\n");

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
// Borrar Información Línea de Remito / Comprobante Original
//
void
ImpresorFiscalPPR5_201::BorrarInformacionRemitoComprobanteOriginal () throw (Excepcion)
{
	// printf ("Comando BorrarInformacionRemitoComprobanteOriginal ejecutado en PPR5_201\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_SET_EMBARK_NUMBER) + FS + "0" + FS + ERASE_LINE);
}


//
// Configurar Controlador por Bloque de Parámetros
//
void
ImpresorFiscalPPR5_201::ConfigurarControladorPorBloque (
							double LimiteConsumidorFinal,
							double LimiteTicketFactura,
							double PorcentajeIVANoInscripto,
							NumerosDeCopias TipoDeCopiasMaximo,
							bool ImpresionCambio,
							bool ImpresionLeyendasOpcionales,
							TiposDeCorteDePapel TipoDeCorte) throw (Excepcion)
{
    // printf ("Comando ConfigurarControladorPorBloque ejecutado en PPR5_201\n");

	// Si el TipoDeCopiasMaximo es NO_COPIAS lo corregimos a ORIGINAL
	// (NO_COPIAS no es un valor válido para este modelo) ...
	if (TipoDeCopiasMaximo == NO_COPIAS)
		TipoDeCopiasMaximo = ORIGINAL;

	// Ejecutamos el método de la superclase 'ImpresorFiscal'
	ImpresorFiscal::ConfigurarControladorPorBloque (
							LimiteConsumidorFinal,
							LimiteTicketFactura,
							PorcentajeIVANoInscripto,
							TipoDeCopiasMaximo,
							ImpresionCambio,
							ImpresionLeyendasOpcionales,
							TipoDeCorte);
}


//
// Obtener Configuracion del Controlador
//
void
ImpresorFiscalPPR5_201::ObtenerConfiguracion (RTA_ObtenerConfiguracion *R) throw (Excepcion)
{
	// printf ("Comando ObtenerConfiguracion ejecutado en PPR5_201\n");

	// Ejecutamos el método de la superclase 'ImpresorFiscalPPR5'
	ImpresorFiscalPPR5::ObtenerConfiguracion (R);

	// Completamos los parámetros correspondientes a los campos
	// de respuesta particulares que se agregan en este modelo
	if (R != NULL)
	{
		R->ReImprimeDocumentos			= ((RespuestaString (10, "Obtener Configuracion del Controlador") [0]) == 'P');
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
ImpresorFiscalPPR5_201::ReporteZ (RTA_ReporteZX *R) throw (Excepcion)
{
	// printf ("Comando ReporteZ ejecutado en PPR5_201\n");

	// Ejecutamos el método de la superclase 'ImpresorFiscalPPR5'
	ImpresorFiscalPPR5::ReporteZ (R);

	// Completamos los parámetros correspondientes a los campos de
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
ImpresorFiscalPPR5_201::ReporteX (RTA_ReporteZX *R) throw (Excepcion)
{
	// printf ("Comando ReporteX ejecutado en PPR5_201\n");

	// Ejecutamos el método de la superclase 'ImpresorFiscalPPR5'
	ImpresorFiscalPPR5::ReporteX (R);

	// Completamos los parámetros correspondientes a los campos de
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
// Imprimir Pago
//
double
ImpresorFiscalPPR5_201::ImprimirPago (
						const std::string &Texto,
						double Monto,
						const std::string &DescripcionAdicional) throw (Excepcion)
{
	// printf ("Comando ImprimirPago ejecutado en PPR5_201\n");

	assert(TotalTenderTextSize != 0);

	// Enviamos el comando fiscal y evaluamos los status

	// Si no se recibe la descripción adicional, no se incluye el campo en el comando.
	if (DescripcionAdicional.length() == 0)
		EnviarComandoFiscal(OpCode(CMD_TOTAL_TENDER) + FS +
							Cadena (Texto, TotalTenderTextSize) + FS +
							Numero (Monto, 2) + FS + "T" + FS +
							Caracter (ModoDisplay));
	else
		EnviarComandoFiscal(OpCode(CMD_TOTAL_TENDER) + FS +
							Cadena (Texto, TotalTenderTextSize) + FS +
							Numero (Monto, 2) + FS + "T" + FS +
							Caracter (ModoDisplay) + FS +
							Cadena (DescripcionAdicional, TotalTenderTextSize));


	// Retornamos el Vuelto o Monto por Pagar
	return RespuestaDouble (2, "Imprimir Pago");
}


//
// Devolver un pago previamente almacenado
//
double
ImpresorFiscalPPR5_201::DevolverPago (
						const std::string &Texto,
						double Monto,
						const std::string &DescripcionAdicional) throw (Excepcion)
{
	// printf ("Comando DevolverPago ejecutado en PPR5_201\n");
	
	assert(TotalTenderTextSize != 0);

	// Enviamos el comando fiscal y evaluamos los status

	// Si no se recibe la descripción adicional, no se incluye el campo en el comando.
	if (DescripcionAdicional.length() == 0)
		EnviarComandoFiscal(OpCode(CMD_TOTAL_TENDER) + FS +
							Cadena (Texto, TotalTenderTextSize) + FS +
							Numero (Monto, 2) + FS + "R" + FS +
							Caracter (ModoDisplay));
	else
		EnviarComandoFiscal(OpCode(CMD_TOTAL_TENDER) + FS +
							Cadena (Texto, TotalTenderTextSize) + FS +
							Numero (Monto, 2) + FS + "R" + FS +
							Caracter (ModoDisplay) + FS +
							Cadena (DescripcionAdicional, TotalTenderTextSize));


	// Retornamos el Vuelto o Monto por Pagar
	return RespuestaDouble (2, "Devolver Pago");
}


// ############################################################ //
//																//
//			M E T O D O S   D E    D N F						//
//																//
// ############################################################ //



// ############################################################ //
//																//
//			M E T O D O S   D E    D N F H						//
//																//
// ############################################################ //

//
// Abrir DNFH
// Envía el comando de Apertura de DNFH en la estación indicada
//
void
ImpresorFiscalPPR5_201::AbrirDNFH (DocumentosNoFiscalesHomologados Tipo, const std::string &Nro) throw (Excepcion)
{
	// printf ("Comando AbrirDNFH ejecutado en PPR5_201\n");

	// Ejecutamos el método de la SuperClase 'ImpresorFiscalPPR5'
	ImpresorFiscalPPR5::AbrirDNFH (Tipo, "");	// Ignoramos el Nro
}


