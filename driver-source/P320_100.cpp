#include "P320_100.h"


std::string ImpresorFiscalP320_100::Version ("SMH/P-320F - Versión 1.00" /* " - 11/04/2000" */);


// ############################################################ //
//																//
//			M E T O D O S      L O C A L E S					//
//																//
// ############################################################ //

//
// Constructor
//
ImpresorFiscalP320_100::ImpresorFiscalP320_100 ()
{
	// Inicialización de variables de uso general

	// Inicialización de variables de tamaño de campos
	PrintNonFiscalTextTicketSize	= 120;	// Por seguridad !!!	
	PrintFiscalTextTicketSize		= 50;	// Por seguridad !!!
	PrintItemTextTicketSize			= 50;	// Por seguridad !!!
	RemitoCantDecimals				= 4;
	ReciboTextTicketSize			= 106;	// Por seguridad !!!

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
}


//
// Obtener la Descripción del Modelo Seleccionado
//
std::string
ImpresorFiscalP320_100::DescripcionModelo () const
{
	return 	(Version);
}


// ############################################################ //
//																//
//			M E T O D O S   G E N E R A L E S					//
//																//
// ############################################################ //

//
// Baja Fiscal
//
void
ImpresorFiscalP320_100::BajaFiscal (const std::string &NumeroDeSerie) throw(Excepcion)
{
    // printf ("Comando BajaFiscal ejecutado en P320_100\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_KILL_EPROM));
}


//
// Cambiar Código de Ingresos Brutos
//
void
ImpresorFiscalP320_100::CambiarCodigoIngresosBrutos (const std::string & /* CodigoIngBr */) throw(Excepcion)
{
    // printf ("Comando CambiarCodigoIngresosBrutos ejecutado en P320_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Cambiar Código de Ingresos Brutos");
}


//
// Cambiar Fecha de Inicio de Actividades
//
void
ImpresorFiscalP320_100::CambiarFechaInicioActividades (const FECHA & /* Fecha */) throw(Excepcion)
{
    // printf ("Comando CambiarFechaInicioActividades ejecutado en P320_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Cambiar Fecha de Inicio de Actividades");
}


//
// Páginas de Ultimo Documento
//
// Nota: este método no es válido para este modelo.
//
unsigned
ImpresorFiscalP320_100::PaginasDeUltimoDocumento () throw(Excepcion)
{
    // printf ("Comando PaginasDeUltimoDocumento ejecutado en P320_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Páginas de Ultimo Documento");
}


//
// CAI del Ultimo Documento A
//
// Nota: este método no es válido para este modelo.
//
void
ImpresorFiscalP320_100::CAIUltimoDocumentoA (RTA_CAIUltimoDocumentoA * /* R */) throw (Excepcion)
{
    // printf ("Comando CAIUltimoDocumentoA ejecutado en P320_100\n");
	
	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "CAI del Ultimo Documento A");
}


// ############################################################ //
//																//
//			M E T O D O S   D E    C O N F I G U R A C I O N	//
//																//
// ############################################################ //

//
// Configurar Controlador Completo
//
void
ImpresorFiscalP320_100::ConfigurarControladorCompleto (
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
							const TiposDeModoImpresion *ModoImpresion) throw (Excepcion)
{
    // printf ("Comando ConfigurarControladorCompleto ejecutado en P320_100\n");

	// Ejecutamos el método de la SuperClase 'ImpresorFiscal16Bits'
	ImpresorFiscal16Bits::ConfigurarControladorCompleto (
							Imprimir, Defaults,
							LimiteConsumidorFinal, LimiteTicketFactura,
							PorcentajeIVANoInscripto,
							TipoDeCopiasMaximo, ImprimeCambio,
							ImprimeLeyendasOpcionales, TipoDeCorte,
							ImprimeMarco, ReImprimeDocumentos,
							DescripcionDelMedioDePago,
							Sonido, AltoHoja, AnchoHoja,
							EstacionImpresionReportesXZ, ModoImpresion);

	// Por seguridad re-programamos el parámetro de configuración
	// de 'Tipo de Corte de Papel' con su valor default.
	ImpresorFiscal::ConfigurarControlador (ImpresorFiscal::CORTE_PAPEL, "F");
}


//
// Obtener Configuracion del Controlador
//
void
ImpresorFiscalP320_100::ObtenerConfiguracion (RTA_ObtenerConfiguracion *R) throw (Excepcion)
{
    // printf ("Comando ObtenerConfiguracion ejecutado en P320_100\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_GET_CONFIG_FULL));

	// Para subsanar el efecto del bug del comando 'SetConfigFull'
	// en este modelo (cuando se programa el tipo de corte de
	// papel con alguno de sus valores válidos -'F', 'P' o 'N'-,
	// dicho campo se termina cargando con '\0'), verificamos que
	// la respuesta del impresor nos informe todos los campos que
	// correspondan y de ser menos, re-programamos dicho campo
	// por medio del método 'ConfigurarControlador' y luego
	// volvemos a ejecutar este método.
	if ( VectorRespuesta.size () < 13 )
	{
		// Re-programamos el parámetro de configuración de
		// 'Tipo de Corte de Papel' con su valor default.
		ImpresorFiscal::ConfigurarControlador (ImpresorFiscal::CORTE_PAPEL, "F");

		// Enviamos nuevamente el comando fiscal y evaluamos los status
		EnviarComandoFiscal(OpCode(CMD_GET_CONFIG_FULL));
	}

	// Completamos los parámetros correspondientes a los campos de
	// respuesta comunes a todos los modelos con los valores obtenidos
	// de la respuesta del comando y el resto con sus valores default
	// (luego, según sea el caso, deberán ser actualizados en la
	// implementación de cada modelo en particular).
	if (R != NULL)
	{
		R->LimiteConsumidorFinal		= RespuestaDouble (2, "Obtener Configuracion del Controlador");
		R->LimiteTicketFactura			= RespuestaDouble (3, "Obtener Configuracion del Controlador");
		R->PorcentajeIVANoInscripto		= RespuestaDouble (4, "Obtener Configuracion del Controlador");
		R->TipoDeCopiasMaximo			= static_cast<ImpresorFiscal::NumerosDeCopias>(RespuestaString (5, "Obtener Configuracion del Controlador") [0]);
		R->ImprimeCambio				= ((RespuestaString (6, "Obtener Configuracion del Controlador") [0]) == 'P');
		R->ImprimeLeyendasOpcionales	= ((RespuestaString (7, "Obtener Configuracion del Controlador") [0]) == 'P');
		R->TipoDeCorte					= static_cast<ImpresorFiscal::TiposDeCorteDePapel>(RespuestaString (8, "Obtener Configuracion del Controlador") [0]);
		R->ImprimeMarco					= ((RespuestaString (9, "Obtener Configuracion del Controlador") [0]) == 'P');
		R->ReImprimeDocumentos			= ((RespuestaString (10, "Obtener Configuracion del Controlador") [0]) == 'P');
		R->DescripcionDelMedioDePago	= RespuestaString (11, "Obtener Configuracion del Controlador");
		R->Sonido						= ((RespuestaString (12, "Obtener Configuracion del Controlador") [0]) == 'P');
		R->AltoHoja						= ALTO_REDUCIDO;
		R->AnchoHoja					= ANCHO_REDUCIDO;
		R->EstacionImpresionReportesXZ	= ESTACION_TICKET;
		R->ModoImpresion				= USO_ESTACION_TICKET;
		R->ChequeoDesbordeCompleto		= true;
		R->ChequeoTapaAbierta			= false;
	}
}


// ############################################################ //
//																//
//			M E T O D O S   D E    R E P O R T E S				//
//																//
// ############################################################ //



// ############################################################ //
//																//
//			M E T O D O S   D E    D F							//
//																//
// ############################################################ //

//
// Subtotal
//
void
ImpresorFiscalP320_100::Subtotal (bool Imprime, RTA_Subtotal *R) throw (Excepcion)
{
    // printf ("Comando Subtotal ejecutado en P320_100\n");
	
	// Ejecutamos el método de la superclase 'ImpresorFiscal'
	ImpresorFiscal::Subtotal (Imprime, R);

	// Completamos los parámetros correspondientes a los campos
	// de respuesta particulares que se agregan en este modelo
	// más alla de los comunes a todos los modelos
	if (R != NULL)
		R->MontoIVANoInscripto		= RespuestaDouble (6, "Subtotal");
}


//
// Cerrar DF
//
unsigned
ImpresorFiscalP320_100::CerrarDF (unsigned Copias) throw (Excepcion)
{
    // printf ("Comando CerrarDF ejecutado en P320_100\n");

	// Verificamos que la cantidad de copias sea 1 (este
	// modelo no soporta la indicación de copias en el
	// comando de cierre).
	if (Copias != 1)
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_PARAMETRO_NO_VALIDO, "Cerrar DF");

	// Ejecutamos el método de la superclase 'ImpresorFiscal16Bits'.
	return ImpresorFiscal16Bits::CerrarDF (Copias);
}


// ############################################################ //
//																//
//			M E T O D O S   D E    D N F						//
//																//
// ############################################################ //

//
// Abrir DNF
// Envía el comando de Apertura de DNF en la estación indicada
//
void
ImpresorFiscalP320_100::AbrirDNF (TiposDeEstacion EstacionDeImpresion) throw (Excepcion)
{
    // printf ("Comando AbrirDNF ejecutado en P320_100\n");

	// Ejecutamos el método de la SuperClase 'ImpresorFiscal'.
	ImpresorFiscal::AbrirDNF (ESTACION_SLIP);
}


//
// Cerrar DNF
//
void
ImpresorFiscalP320_100::CerrarDNF (unsigned Copias) throw (Excepcion)
{
    // printf ("Comando CerrarDNF ejecutado en P320_100\n");

	// Verificamos que la cantidad de copias sea 1 (este
	// modelo no soporta la indicación de copias en el
	// comando de cierre).
	if (Copias != 1)
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_PARAMETRO_NO_VALIDO, "Cerrar DF");

	// Ejecutamos el método de la superclase 'ImpresorFiscal16Bits'.
	ImpresorFiscal16Bits::CerrarDNF (Copias);
}


// ############################################################ //
//																//
//			M E T O D O S   D E    D N F H						//
//																//
// ############################################################ //

//
// Imprimir Información en DNFH
//
// Nota: esta método no es válido para este modelo.
//
void
ImpresorFiscalP320_100::ImprimirInfoEnDNFH (
								const unsigned /* IDCampo */,
								const std::string & /* Texto */,
								double /* Cantidad */) throw (Excepcion)
{
    // printf ("Comando ImprimirInfoEnDNFH ejecutado en P320_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Imprimir Información en DNFH");
}


//
// Cerrar DNFH
//
unsigned
ImpresorFiscalP320_100::CerrarDNFH (unsigned Copias) throw (Excepcion)
{
    // printf ("Comando CerrarDNFH ejecutado en P320_100\n");

	// Verificamos que la cantidad de copias sea 1 (este
	// modelo no soporta la indicación de copias en el
	// comando de cierre).
	if (Copias != 1)
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_PARAMETRO_NO_VALIDO, "Cerrar DF");

	// Ejecutamos el método de la superclase 'ImpresorFiscal16Bits'.
	return ImpresorFiscal16Bits::CerrarDNFH (Copias);
}


//
// Imprimir Voucher de Tarjeta
//
void
ImpresorFiscalP320_100::ImprimirVoucher (
							const std::string & /* NombreCliente */,
							const std::string & /* NombreTarjeta */,
							TiposDeVouchers /* TipoVoucher */,
							const std::string & /* NumeroDeTarjeta */,
							FECHA /* FechaDeVencimiento */,
							TiposDeTarjetas /* TipoTarjeta */,
							unsigned /* Cuotas */,
							const std::string & /* CodigoDeComercio */,
							unsigned /* NumeroDeTerminal */,
							unsigned /* NumeroDeLote */,
							unsigned /* NumeroCupon */,
							TiposDeIngresoDeTarjeta /* TipoIngreso */,
							TiposOperacionDeTarjeta /* TipoOperacion */,
							unsigned /* NumeroAutorizacion */,
							const std::string & /* Monto */,
							const std::string & /* NumeroComprobanteAsociado */,
							unsigned /* Copias */,
							const std::string & /* Vendedor */,
							TiposDeEstacion /* Estacion */) throw (Excepcion)
{
    // printf ("Comando ImprimirVoucher ejecutado en P320_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Imprimir Voucher de Tarjeta");
}


