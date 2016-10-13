#include "P715_201.h"


std::string ImpresorFiscalP715_201::Version ("SMH/P-715F - Versi�n 02.01");


// ############################################################ //
//																//
//			M E T O D O S      L O C A L E S					//
//																//
// ############################################################ //

//
// Constructor
//
ImpresorFiscalP715_201::ImpresorFiscalP715_201 ()
{
	// Inicializaci�n de variables de uso general

	// Inicializaci�n de variables de tama�o de campos
	PriceDecimals					= 4;
}


//
// Obtener la Descripci�n del Modelo Seleccionado
//
std::string
ImpresorFiscalP715_201::DescripcionModelo () const
{
	return 	(Version);
}


// ############################################################ //
//																//
//			M E T O D O S   G E N E R A L E S					//
//																//
// ############################################################ //

//
// Cambiar Fecha de Inicio de Actividades
//
void
ImpresorFiscalP715_201::CambiarFechaInicioActividades (const FECHA &Fecha) throw(Excepcion)
{
	// printf ("Comando CambiarFechaInicioActividades ejecutado en P715_201\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_CHG_INI_ACT_DATE) + FS + Fecha.ToString());
}


//
// Cargar Datos del Cliente
//
void
ImpresorFiscalP715_201::DatosCliente (
						const std::string &RazonSocial,
						const std::string &NroDocumento,
						TiposDeDocumentoCliente TipoDocumento,
						TiposDeResponsabilidadesCliente ResponsabilidadIVA,
						const std::string &Direccion) throw(Excepcion)
{
	// printf ("Comando DatosCliente ejecutado en P715_201\n");

	assert(RazonSocialSize != 0);
	assert(DireccionSize != 0);

	// Enviamos el comando fiscal y evaluamos los status

	// Verificamos la existencia del par�metro direcci�n
	// (si no se recibe la direcci�n, no se incluye el campo en el comando)
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
// Configurar Controlador por Bloque de Par�metros
//
void
ImpresorFiscalP715_201::ConfigurarControladorPorBloque (
							double LimiteConsumidorFinal,
							double LimiteTicketFactura,
							double PorcentajeIVANoInscripto,
							NumerosDeCopias TipoDeCopiasMaximo,
							bool ImpresionCambio,
							bool ImpresionLeyendasOpcionales,
							TiposDeCorteDePapel TipoDeCorte) throw (Excepcion)
{
    // printf ("Comando ConfigurarControladorPorBloque ejecutado en P715_201\n");

	// Si el TipoDeCopiasMaximo es NO_COPIAS lo corregimos a ORIGINAL
	// (NO_COPIAS no es un valor v�lido para este modelo) ...
	if (TipoDeCopiasMaximo == NO_COPIAS)
		TipoDeCopiasMaximo = ORIGINAL;

	// Ejecutamos el m�todo de la superclase 'ImpresorFiscal'
	ImpresorFiscal::ConfigurarControladorPorBloque (
							LimiteConsumidorFinal,
							LimiteTicketFactura,
							PorcentajeIVANoInscripto,
							TipoDeCopiasMaximo,
							ImpresionCambio,
							ImpresionLeyendasOpcionales,
							TipoDeCorte);
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
// Env�a el comando de Apertura de DNFH en la estaci�n indicada
//
void
ImpresorFiscalP715_201::AbrirDNFH (DocumentosNoFiscalesHomologados Tipo, const std::string &Nro) throw (Excepcion)
{
	// printf ("Comando AbrirDNFH ejecutado en P715_201\n");

	// Ejecutamos el m�todo de la SuperClase 'ImpresorFiscalP715'
	ImpresorFiscalP715::AbrirDNFH (Tipo, "");	// Ignoramos el Nro
}

