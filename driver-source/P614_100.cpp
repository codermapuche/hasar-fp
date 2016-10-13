#include "P614_100.h"


std::string ImpresorFiscalP614_100::Version ("SMH/P-614F - Versión 1.00");


// ############################################################ //
//																//
//			M E T O D O S      L O C A L E S					//
//																//
// ############################################################ //

//
// Constructor
//
ImpresorFiscalP614_100::ImpresorFiscalP614_100 ()
{
	// Inicialización de variables de uso general
	MaxFiscalTextLinesInvoice		= 3;	// Por seguridad !!!
	MaxFiscalTextLinesTicket		= 3;

	// Inicialización de variables de tamaño de campos
	PrintNonFiscalTextTicketSize	= 40;
	PrintNonFiscalTextSlipSize		= 40;	// Por seguridad !!!
	FantasyNameSize					= 40;
	HeaderTrailerSize				= 40;
	HeaderTrailerTicketSize			= 40;
	PrintFiscalTextTicketSize		= 28;
	PrintFiscalTextSlipSize			= 28;	// Por seguridad !!
	PrintItemTextTicketSize			= 20;
	PrintItemTextSlipSize			= 20;	// Por seguridad !!
	TotalTenderTextSize				= 24;
	VouchImportSize					= 15;	// Por seguridad !!!
	VouchCompNumSize				= 8;	// Por seguridad !!!
	DisplayTextSize					= 16;

	// Inicialización de variables locales de este modelo
	LastIVA							= -1;
	LastII							= -1;
}


//
// Obtener la Descripción del Modelo Seleccionado
//
std::string
ImpresorFiscalP614_100::DescripcionModelo () const
{
	return 	(Version);
}


// ############################################################ //
//																//
//			M E T O D O S   G E N E R A L E S					//
//																//
// ############################################################ //

//
// Obtener Datos de Inicialización
//
// Nota: este método no es válido para este modelo.
//
void
ImpresorFiscalP614_100::ObtenerDatosDeInicializacion (RTA_ObtenerDatosDeInicializacion * /* R */) throw (Excepcion)
{
    // printf ("Comando ObtenerDatosDeInicializacion ejecutado en P614_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Obtener Datos de Inicialización");
}


//
// Obtener Datos de Memoria de Trabajo
//
// Nota: este método no es válido para este modelo.
//
void
ImpresorFiscalP614_100::ObtenerDatosMemoriaDeTrabajo (RTA_ObtenerDatosMemoriaDeTrabajo * /* R */) throw (Excepcion)
{
    // printf ("Comando ObtenerDatosMemoriaDeTrabajo ejecutado en P614_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Obtener Datos de Memoria de Trabajo");
}


//
// Estado del Controlador Fiscal
//
// Nota: este método no es válido para este modelo.
//
ImpresorFiscal::TiposDeEstadoControlador
ImpresorFiscalP614_100::EstadoControlador () throw(Excepcion)
{
    // printf ("Comando EstadoControlador ejecutado en P614_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Estado del Controlador Fiscal");
}


//
// Descripción del Estado del Controlador Fiscal
//
// Nota: este método no es válido para este modelo.
//
std::string
ImpresorFiscalP614_100::DescripcionEstadoControlador () throw(Excepcion)
{
    // printf ("Comando DescripcionEstadoControlador ejecutado en P614_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Descripción del Estado del Controlador Fiscal");
}


//
// Cortar Papel
//
void
ImpresorFiscalP614_100::CortarPapel () throw (Excepcion)
{
    // printf ("Comando CortarPapel ejecutado en P614_100\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_CUT_NON_FISCAL_RECEIPT));
}


//
// Mostrar Mensaje en Display
//
void
ImpresorFiscalP614_100::MostrarMensajeEnDisplay (
									const std::string &Texto,
									const TiposDeDisplay Destino) throw(Excepcion)
{
    // printf ("Comando MostrarMensajeEnDisplay ejecutado en P614_100\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_WRITE_DISPLAY) + FS +
						Caracter ('f') + Caracter (Destino) +
						Cadena (Texto, DisplayTextSize));
}


//
// Imprimir Código de Barras
//
// Nota: este método no es válido para este modelo.
//
void
ImpresorFiscalP614_100::ImprimirCodigoDeBarras (
								TiposDeCodigoDeBarras /* Tipo */,
								const std::string & /* Codigo */,
								bool /* ImprimeNumeros */,
								bool /* ImprimeAhora */)  throw(Excepcion)
{
    // printf ("Comando ImprimirCodigoDeBarras ejecutado en P614_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Imprimir Código de Barras");
}


// ############################################################ //
//																//
//			M E T O D O S   D E    C O N F I G U R A C I O N	//
//																//
// ############################################################ //

//
// Especificar Línea de Nombre de Fantasía
//
// Nota: este método no es válido para este modelo.
//
void
ImpresorFiscalP614_100::EspecificarNombreDeFantasia (
								unsigned /* Linea */,
								const std::string & /* Texto */) throw (Excepcion)
{
    // printf ("Comando EspecificarNombreDeFantasia ejecutado en P614_100\n");
	
	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Especificar Línea de Nombre de Fantasía");
}


//
// Borrar Líneas de Nombre de Fantasía, Encabezado y Cola
//
// Nota: en este modelo solo se tienen en cuenta los flags de borrado
//       de Encabezado y Cola (ya que no reconoce Líneas de Fantasía).
//
void
ImpresorFiscalP614_100::BorrarFantasiaEncabezadoCola (bool BorrarFantasia, bool BorrarEncabezado, bool BorrarCola) throw (Excepcion)
{
	// printf ("Comando BorrarFantasiaEncabezadoCola ejecutado en P614_100\n");
	
	if ( BorrarEncabezado )
		for ( unsigned Linea = First_Header_Line; Linea <= Last_Header_Line; Linea++ )
			EnviarComandoFiscal(OpCode(CMD_SET_HEADER_TRAILER) + FS +
								Numero (Linea) + FS + ERASE_LINE);

	if ( BorrarCola )
		for ( unsigned Linea = First_Trailer_Line; Linea <= Last_Trailer_Line; Linea++ )
			EnviarComandoFiscal(OpCode(CMD_SET_HEADER_TRAILER) + FS +
								Numero (Linea) + FS + ERASE_LINE);
}


//
// Configurar Controlador por Parámetros Individuales
//
// Nota: este método no es válido para este modelo.
//
void
ImpresorFiscalP614_100::ConfigurarControlador (
							ParametrosDeConfiguracion /* Parametro */,
							const std::string & /* Valor */) throw (Excepcion)
{
    // printf ("Comando ConfigurarControlador ejecutado en P614_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Configurar Controlador por Parámetros Individuales");
}


//
// Configurar Controlador por Bloque de Parámetros
//
// Nota: este método no es válido para este modelo.
//
void
ImpresorFiscalP614_100::ConfigurarControladorPorBloque (
							double /* LimiteConsumidorFinal */,
							double /* LimiteTicketFactura */,
							double /* PorcentajeIVANoInscripto */,
							NumerosDeCopias /* NumeroDeCopiasMaximo */,
							bool /* ImpresionCambio */,
							bool /* ImpresionLeyendasOpcionales */,
							TiposDeCorteDePapel /* TipoDeCorte */) throw (Excepcion)
{
    // printf ("Comando ConfigurarControladorPorBloque ejecutado en P614_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Configurar Controlador por Bloque de Parámetros");
}


//
// Obtener Configuracion del Controlador
//
// Nota: este método no es válido para este modelo.
//
void
ImpresorFiscalP614_100::ObtenerConfiguracion (RTA_ObtenerConfiguracion * /* R */) throw (Excepcion)
{
    // printf ("Comando ObtenerConfiguracion ejecutado en P614_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Obtener Configuracion del Controlador");
}


//
// Obtener Tabla de IVAs
//
// Nota: este método no es válido para este modelo.
//
void
ImpresorFiscalP614_100::ObtenerTablaIVAs (TablaIVAs & /* Tabla */) throw (Excepcion)
{
    // printf ("Comando ObtenerTablaIVAs ejecutado en P614_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Obtener Tabla de IVAs");
}


// ############################################################ //
//																//
//			M E T O D O S   D E    R E P O R T E S				//
//																//
// ############################################################ //

//
// Reporte de Cierre Z Individual por Fecha
//
// Nota: este método no es válido para este modelo.
//
void
ImpresorFiscalP614_100::ReporteZIndividualPorFecha (FECHA /* FechaZ */, RTA_ReporteZIndividual * /* R */) throw (Excepcion)
{
    // printf ("Comando ReporteZIndividualPorFecha ejecutado en P614_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Reporte de Cierre Z Individual por Fecha");
}


//
// Reporte de Cierre Z Individual por Número
//
// Nota: este método no es válido para este modelo.
//
void
ImpresorFiscalP614_100::ReporteZIndividualPorNumero (unsigned /* NumeroZ */, RTA_ReporteZIndividual * /* R */) throw (Excepcion)
{
    // printf ("Comando ReporteZIndividualPorNumero ejecutado en P614_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Reporte de Cierre Z Individual por Número");
}


// ############################################################ //
//																//
//			M E T O D O S   D E    D F							//
//																//
// ############################################################ //

//
// Abrir DF
// Envía el comando de Apertura de DF en la estación indicada
//
void
ImpresorFiscalP614_100::AbrirDF (DocumentosFiscales Tipo) throw (Excepcion)
{
    // printf ("Comando AbrirDF ejecutado en P614_100\n");

	// Ejecutamos el método de la SuperClase 'ImpresorFiscal'
	ImpresorFiscal::AbrirDF (Tipo);

	// Reinicializamos 'LastIVA' y 'LastII'
	FirstDocNum = 0;
	LastIVA = -1;
	LastII	= -1;
}


//
// Imprimir Item
//
void
ImpresorFiscalP614_100::ImprimirItem (      const std::string &Texto,
							double Cantidad, double Monto,
							double IVA, double ImpuestosInternos,
							bool EnNegativo) throw (Excepcion)
{
    // printf ("Comando ImprimirItem ejecutado en P614_100\n");

	// Ejecutamos el método de la superclase 'ImpresorFiscal'
	ImpresorFiscal::ImprimirItem (Texto, Cantidad, Monto, IVA, ImpuestosInternos, EnNegativo);

	// Almacenamos IVA e II de la venta para un potencial
	// posterior Descuento a UltimoItem.
	LastIVA = IVA;
	LastII	= ImpuestosInternos;
}


//
// Descuento a Ultimo Item
//
// Nota: en este modelo (por no contar con el comando asociado)
//		 este método se implementa como una venta.
//
void
ImpresorFiscalP614_100::DescuentoUltimoItem (const std::string &Texto, double Monto, bool EnNegativo) throw (Excepcion)
{
    // printf ("Comando DescuentoUltimoItem ejecutado en P614_100\n");

	unsigned PrintItemTextSize = PrintItemTextTicketSize;

	assert(PrintItemTextSize != 0);

	// Verificamos si existió una venta previa.
	if ( LastIVA == -1 || LastII == -1 )
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_ESTADO_NO_VALIDO, "Descuento a Ultimo Item");

	// Ejecutamos el método de la superclase 'ImpresorFiscal'
	ImpresorFiscal::ImprimirItem (Texto, 1.00, Monto, LastIVA, LastII, EnNegativo);

	// Reinicializamos 'LastIVA' y 'LastII'
	LastIVA = -1;
	LastII	= -1;
}


//
// Descuento General
//
// Nota: en este modelo (por no contar con el comando asociado)
//		 este método se implementa como una venta.
//
void
ImpresorFiscalP614_100::DescuentoGeneral (const std::string &Texto, double Monto, bool EnNegativo) throw (Excepcion)
{
    // printf ("Comando DescuentoGeneral ejecutado en P614_100\n");

	unsigned PrintItemTextSize = PrintItemTextTicketSize;

	assert(PrintItemTextSize != 0);

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_PRINT_LINE_ITEM) + FS +
								Cadena (Texto, PrintItemTextSize) + FS +
								Numero (1, 10) + FS +
								Numero (Monto, PriceDecimals) + FS +
								Cadena ("**.**", 5) + FS +
								(EnNegativo ? "m" : "M") + FS +
								Numero (0, 2) + FS +
								Caracter (ModoDisplay) + FS +
								Caracter (ModalidadPrecio));

	// Reinicializamos 'LastIVA' y 'LastII'
	LastIVA = -1;
	LastII	= -1;
}


//
// Bonificación, Recargo y Devolución de Envases
//
// Nota: este método no es válido para este modelo.
//
void
ImpresorFiscalP614_100::DevolucionDescuento (
							const std::string & /* Texto */,
							double /* Monto */, double /* IVA */,
							double /* ImpuestosInternos */,
							bool /* EnNegativo */,
							TiposDeDescuento /* Tipo */) throw (Excepcion)
{
    // printf ("Comando DevolucionDescuento ejecutado en P614_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Bonificación, Recargo y Devolución de Envases");
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
ImpresorFiscalP614_100::AbrirDNF (TiposDeEstacion EstacionDeImpresion) throw (Excepcion)
{
    // printf ("Comando AbrirDNF ejecutado en P614_100\n");

	// Verificamos si la estación indicada es válida en el modelo.
	// En caso de serlo ejecutamos el método de la SuperClase
	// 'ImpresorFiscal'. En caso contrario, damos una excepción.
	if (EstacionDeImpresion != ESTACION_TICKET)
		throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Abrir DNF en estación SLIP");

	ImpresorFiscal::AbrirDNF (EstacionDeImpresion);
}


// ############################################################ //
//																//
//			M E T O D O S   D E    D N F H						//
//																//
// ############################################################ //

//
// Emitir un DNFH de Farmacia
//
// Nota: este método no es válido para este modelo.
//
void
ImpresorFiscalP614_100::DNFHFarmacia (unsigned /* Copias */) throw (Excepcion)
{
    // printf ("Comando DNFHFarmacia ejecutado en P614_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Emitir un DNFH de Farmacia");
}


//
// Emitir un DNFH de Reparto
//
// Nota: este método no es válido para este modelo.
//
void
ImpresorFiscalP614_100::DNFHReparto (unsigned /* Copias */) throw (Excepcion)
{
    // printf ("Comando DNFHReparto ejecutado en P614_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Emitir un DNFH de Reparto");
}


//
// Imprimir Voucher de Tarjeta
//
// Nota: este método no es válido para este modelo.
//
void
ImpresorFiscalP614_100::ImprimirVoucher (
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
    // printf ("Comando ImprimirVoucher ejecutado en P614_100\n");

	throw Excepcion(Excepcion::IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO, "Imprimir Voucher de Tarjeta");
}


