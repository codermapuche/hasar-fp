#include "P715_302.h"


std::string ImpresorFiscalP715_302::Version ("SMH/P-715F - Versión 03.02");


// ############################################################ //
//																//
//			M E T O D O S      L O C A L E S					//
//																//
// ############################################################ //

//
// Constructor
//
ImpresorFiscalP715_302::ImpresorFiscalP715_302 ()
{
	// Inicialización de variables de uso general

	// Inicialización de variables de tamaño de campos
	PriceDecimals					= 4;
	ReciboTextTicketSize			= 38;
	PrintDNFHTextSize				= 40;

	// Cargamos los DF soportados

	// Cargamos los DNFH soportados
	DNFHSoportados.insert (TICKET_RECIBO_X);
	DNFHSoportados.insert (TICKET_PAGARE);
	DNFHSoportados.insert (TICKET_PRESUPUESTO);
}


//
// Obtener la Descripción del Modelo Seleccionado
//
std::string
ImpresorFiscalP715_302::DescripcionModelo () const
{
	return 	(Version);
}


//
// Establecer Modalidad Resumen IVA
//
void
ImpresorFiscalP715_302::EstablecerModalidadResumenIVA (TiposDeResumenIVA Modo)
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
ImpresorFiscalP715_302::ObtenerDatosMemoriaDeTrabajo (RTA_ObtenerDatosMemoriaDeTrabajo *R) throw (Excepcion)
{
	// printf ("Comando ObtenerDatosMemoriaDeTrabajo ejecutado en P715_302\n");

	// Ejecutamos el método de la superclase 'ImpresorFiscalP715'
	ImpresorFiscalP715::ObtenerDatosMemoriaDeTrabajo (R);

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
// Cambiar Fecha de Inicio de Actividades
//
void
ImpresorFiscalP715_302::CambiarFechaInicioActividades (const FECHA &Fecha) throw(Excepcion)
{
	// printf ("Comando CambiarFechaInicioActividades ejecutado en P715_302\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_CHG_INI_ACT_DATE) + FS + Fecha.ToString());
}


//
// Cargar Datos del Cliente
//
void
ImpresorFiscalP715_302::DatosCliente (
						const std::string &RazonSocial,
						const std::string &NroDocumento,
						TiposDeDocumentoCliente TipoDocumento,
						TiposDeResponsabilidadesCliente ResponsabilidadIVA,
						const std::string &Direccion) throw(Excepcion)
{
	// printf ("Comando DatosCliente ejecutado en P715_302\n");

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
ImpresorFiscalP715_302::BorrarFantasiaEncabezadoCola (bool BorrarFantasia, bool BorrarEncabezado, bool BorrarCola) throw (Excepcion)
{
	// printf ("Comando BorrarFantasiaEncabezadoCola ejecutado en P715_302\n");

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
ImpresorFiscalP715_302::BorrarInformacionRemitoComprobanteOriginal () throw (Excepcion)
{
	// printf ("Comando BorrarInformacionRemitoComprobanteOriginal ejecutado en P715_302\n");

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_SET_EMBARK_NUMBER) + FS + "0" + FS + ERASE_LINE);
}


//
// Obtener Configuracion del Controlador
//
void
ImpresorFiscalP715_302::ObtenerConfiguracion (RTA_ObtenerConfiguracion *R) throw (Excepcion)
{
	// printf ("Comando ObtenerConfiguracion ejecutado en P715_302\n");

	// Ejecutamos el método de la superclase 'ImpresorFiscalP715'
	ImpresorFiscalP715::ObtenerConfiguracion (R);

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
ImpresorFiscalP715_302::ReporteZ (RTA_ReporteZX *R) throw (Excepcion)
{
	// printf ("Comando ReporteZ ejecutado en P715_302\n");

	// Ejecutamos el método de la superclase 'ImpresorFiscalP715
	ImpresorFiscalP715::ReporteZ (R);

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
ImpresorFiscalP715_302::ReporteX (RTA_ReporteZX *R) throw (Excepcion)
{
	// printf ("Comando ReporteX ejecutado en P715_302\n");

	// Ejecutamos el método de la superclase 'ImpresorFiscalP715'
	ImpresorFiscalP715::ReporteX (R);

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
ImpresorFiscalP715_302::ImprimirPago (
						const std::string &Texto,
						double Monto,
						const std::string &DescripcionAdicional) throw (Excepcion)
{
	// printf ("Comando ImprimirPago ejecutado en P715_302\n");

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
ImpresorFiscalP715_302::DevolverPago (
						const std::string &Texto,
						double Monto,
						const std::string &DescripcionAdicional) throw (Excepcion)
{
	// printf ("Comando DevolverPago ejecutado en P715_302\n");
	
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
// Detalle Recibo
//
void
ImpresorFiscalP715_302::DetalleRecibo (const std::string &Texto) throw (Excepcion)
{
	// printf ("Comando DetalleRecibo ejecutado en P715_302\n");

	unsigned i, j, OffCnt;
	unsigned TextLen, Lines, TotalWidth;

	unsigned Size = ReciboTextTicketSize;

	assert(Size != 0);
	
	std::string Description(Texto);
	std::string Line;

	const char *Trailer;

	// Chequea que la descripción haya sido asignado y
	// elimina los espacios a la derecha.
	unsigned Ini;
	unsigned Fin = Description.find_last_not_of (" ");

	if ( Description.length() == 0 || Fin == std::string::npos )
		Description = " ";
	else
		Description = Description.substr (0, Fin+1);

	// Verifica si el tamaño del texto no supera el máximo.
	// En tal caso no modifica el texto. En caso contrario
	// evalúa como debe adaptar el texto para que no se den
	// errores, para lo cual tiene en cuenta si debe operar
	// o no con 'DescripcionesLargas'.
	if (Description.length() > Size)
	{
		// Si no estamos operando con 'DescripcionesLargas' o
		// si el texto supera el máximo y no tiene ningún ' '
		// en medio lo truncamos al tamaño máximo menos 3 y
		// completamos con '...' al final, indicando que fue
		// truncado.

		if ( !DescripcionesLargas ||
			 Description.find (" ") == std::string::npos )
		{
			Description = (Description.substr(0, Size-3) + "...");
		}
		// En caso contrario procedemos a repartir el texto entre
		// las líneas de concepto de recibo.
		else
		{
			TotalWidth = Description.length();
			TextLen = TotalWidth - Size;

			// Solo se chequea la cantidad de líneas cuando se trata
			// de un recibo emitido en estación de carro ancho.
			for (i = 0, Lines = 0; i < TextLen;  )
			{
				// Saltea los espacios a la izquerda si no es
				// la primer línea de concepto de recibo.
				if ( Lines != 0 )
					i += Description.substr (i).find_first_not_of (" ");

				// Busca el corte para la palabra
				j = Description.find_last_of (" ", i + Size);

				// Borra Cnt. de Offset de Líneas que superan el
				// tamaño de la línea de concepto de recibo.
				OffCnt = 0;

				// Si no encontró espacios de atrás para adelante,
				// verifica si quedan caracteres para la última línea
				// de concepto de recibo (busca al menos un ' ').
				if (j == std::string::npos || j <= i)
				{
					j = Size;
					if ( Description.find (" ", i + j) != std::string::npos )
						// Si restan caracteres para la última línea de
						// concepto de recibo, termina la línea con "..."
						// y se posiciona en el caracter siguiente al
						// primer ' ' para continuar con el procesamiento
						// de la siguiente línea.
						OffCnt = Description.substr (i+j).find_first_of (" ") + 1;
					else
						// Si no restan caracteres para la última línea
						// de concepto de recibo, sale del lazo.
						break;
				}
				// Si encontró algún espacio, corto donde encontró el espacio.
				else
					j -= i;

				Lines++;
				// Si la ultima línea supera el tamaño una línea de concepto,
				// corta la línea y la concluye con "...".
				if ( OffCnt != 0 )
				{
					if ( ((j = Description.substr (i).find_last_not_of (" ", j)) != std::string::npos) &&
						  (j >= (Size - 3)) )
						j -= 3;
					Trailer = "...";
				}
				else
					Trailer = "";
				
				Line = Description.substr (i, j) + Trailer;
				i += OffCnt + Line.length();

				// Verifica que la linea completa no sea de ' ' (solo
				// puede ocurrir en la primera linea ya que es la unica
				// en la que no se eliminan los ' ' a la izquierda) y
				// en caso de serlo no se genera la línea de concepto y
				// se eliminan los espacios que siguen para que la
				// siguiente línea comience con una palabra.
				if ( Line.find_last_not_of (" ", j-1) != std::string::npos )
					// Enviamos el comando fiscal y evaluamos los status
					EnviarComandoFiscal(OpCode(CMD_RECEIPT_TEXT) + FS +
										Cadena (Line, Size));
				else
				{
					Lines--;
					// Elimina los espacios a la izquerda de la
					// siguiente linea ...
					i = Description.find_first_not_of (" ", i);
				}
			}

			// Al salir del lazo del for nos quedamos solo con
			// la parte restante del texto.
			Description = Description.substr(i);

			// Si hubo alguna línea de concepto de recibo previa
			// elimina los espacios a la izquierda del texto restante.
			if ( Lines != 0 )
				if ((Ini = Description.find_first_not_of (" ")) != std::string::npos)
					Description = Description.substr (Ini);

			// Si quedó más de lo que entra en una línea de concepto
			// de recibo, la corto de atrás para adelante.
			if (Description.length() > Size)
				Description = "..." + Description.substr(Description.length()-Size+3);
		}
	}

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_RECEIPT_TEXT) + FS +
						Cadena (Description, Size));
}


//
// Imprimir Información en DNFH
//
void
ImpresorFiscalP715_302::ImprimirInfoEnDNFH (const unsigned IDCampo,
											const std::string &Texto,
											double Cantidad) throw (Excepcion)
{
	// printf ("Comando ImprimirInfoEnDNFH ejecutado en P715_302\n");

	assert(PrintDNFHTextSize != 0);

	// Enviamos el comando fiscal y evaluamos los status
	EnviarComandoFiscal(OpCode(CMD_PRINT_INFO_DNFH) + FS +
						Numero (IDCampo) + FS +
						Cadena (Texto, PrintDNFHTextSize) + FS +
						Caracter (ModoDisplay));
}


