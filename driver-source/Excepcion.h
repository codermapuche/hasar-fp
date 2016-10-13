#ifndef EXCEPCION_H
#define EXCEPCION_H

#include <string>

class Excepcion
{
public:
	enum Errores
	{
		NINGUN_ERROR,

		// Módulo: Transporte
		TRANSPORTE_ERROR_APERTURA,
		TRANSPORTE_ERROR_YA_ABIERTO,
		TRANSPORTE_ERROR_PARAMETROS_SERIE,
		TRANSPORTE_ERROR_PUERTO_NO_ABIERTO,
		TRANSPORTE_ERROR_TIEMPO_DE_ESPERA_AGOTADO,
		TRANSPORTE_ERROR_LECTURA,
		TRANSPORTE_ERROR_ESCRITURA,
		TRANSPORTE_ERROR_INICIALIZACION_SOCKET,
		TRANSPORTE_ERROR_BIND_SOCKET,
		TRANSPORTE_ERROR_CONEXION,
		TRANSPORTE_ERROR_SELECT,
		TRANSPORTE_ERROR_VERSION_SOCKET,
		TRANSPORTE_ERROR_FORMATO_IP,

		// Módulo: Protocolo
		PROTOCOLO_ERROR_ATOMICIDAD,
		PROTOCOLO_ABORCION,
		PROTOCOLO_NO_HAY_TRANSPORTE_ESPECIFICADO,
		PROTOCOLO_MAXIMOS_REINTENTOS,
		PROTOCOLO_EQUIVOCADO,
		PROTOCOLO_MAL_FORMATO,
		PROTOCOLO_ANOMALIA_COMUNICACIONES,
		PROTOCOLO_COMANDO_NO_COINCIDENTE,

		// Módulo: Impresor Fiscal
		IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO,
		IMPRESOR_FISCAL_ERROR_CAMPO_NO_ENCONTRADO,
		IMPRESOR_FISCAL_ERROR_NO_EJECUTADO,
		IMPRESOR_FISCAL_ERROR_CONTROLADOR_NO_DETECTADO,
		IMPRESOR_FISCAL_ERROR_FECHA_NO_VALIDA,
		IMPRESOR_FISCAL_ERROR_HORA_NO_VALIDA,
		IMPRESOR_FISCAL_ERROR_PARAMETRO_NO_VALIDO,
		IMPRESOR_FISCAL_ERROR_BIT_NO_VALIDO,
		IMPRESOR_FISCAL_ERROR_PARAMETRO_OPCIONAL,
		IMPRESOR_FISCAL_ERROR_PARAMETRO_OBLIGATORIO,
		IMPRESOR_FISCAL_ERROR_ESTADO_NO_VALIDO,
		IMPRESOR_FISCAL_ERROR_NOMBRE_ARCHIVO
	};

	Excepcion(Errores Err, const std::string &Cont = "")
	{
		struct ErrDesc
		{
			Errores Err;
			const char *Descripcion;
		};
		static ErrDesc ErrDescs[] =
		{
			{
				NINGUN_ERROR,
				""
			},

			// Módulo: Transporte
			{
				TRANSPORTE_ERROR_APERTURA,
				"Error de apertura (transporte)"
			},
			{
				TRANSPORTE_ERROR_YA_ABIERTO,
				"El dispositivo ya estaba abierto"
			},
			{
				TRANSPORTE_ERROR_PARAMETROS_SERIE,
				"Error de parámetros de puerto serie"
			},
			{
				TRANSPORTE_ERROR_PUERTO_NO_ABIERTO,
				"Puerto no abierto"
			},
			{
				TRANSPORTE_ERROR_TIEMPO_DE_ESPERA_AGOTADO,
				"Tiempo de espera agotado"
			},
			{
				TRANSPORTE_ERROR_LECTURA,
				"Error de lectura (transporte)"
			},
			{
				TRANSPORTE_ERROR_ESCRITURA,
				"Error de escritura (transporte)"
			},
			{
				TRANSPORTE_ERROR_INICIALIZACION_SOCKET,
				"Error de inicialización de socket"
			},
			{
				TRANSPORTE_ERROR_VERSION_SOCKET,
				"Error de versión de socket"
			},
			{
				TRANSPORTE_ERROR_BIND_SOCKET,
				"Error de bind()"
			},
			{
				TRANSPORTE_ERROR_CONEXION,
				"Error de conexión"
			},
			{
				TRANSPORTE_ERROR_SELECT,
				"Error en función select()"
			},
			{
				TRANSPORTE_ERROR_FORMATO_IP,
				"Error de formato en dirección de IP"
			},

			// Módulo: Protocolo
			{
				PROTOCOLO_ERROR_ATOMICIDAD,
				"Error de reentrancia en la ejecución"
			},
			{
				PROTOCOLO_ABORCION,
				"Operación abortada"
			},
			{
				PROTOCOLO_NO_HAY_TRANSPORTE_ESPECIFICADO,
				"No se ha inicializado un transporte"
			},
			{
				PROTOCOLO_MAXIMOS_REINTENTOS,
				"Se han agotado la cantidad máxima de reintentos"
			},
			{
				PROTOCOLO_EQUIVOCADO,
				"El impresor conectado no corresponde al modelo seleccionado"
			},
			{
				PROTOCOLO_MAL_FORMATO,
				"Error en protocolo fiscal"
			},
			{
				PROTOCOLO_ANOMALIA_COMUNICACIONES,
				"Se ha corregido anomalía de comunicaciones"
			},
			{
				PROTOCOLO_COMANDO_NO_COINCIDENTE,
				"Se han agotado los reintentos por código de comando no coincidente"
			},

			// Módulo: Impresor Fiscal
			{
				IMPRESOR_FISCAL_ERROR_NO_IMPLEMENTADO,
				"Función no soportada en versión de controlador fiscal seleccionada"
			},
			{
				IMPRESOR_FISCAL_ERROR_CAMPO_NO_ENCONTRADO,
				"Campo de respuesta no encontrado"
			},
			{
				IMPRESOR_FISCAL_ERROR_NO_EJECUTADO,
				"Función no ejecutada con éxito"				
			},
			{
				IMPRESOR_FISCAL_ERROR_CONTROLADOR_NO_DETECTADO,
				"Controlador no detectado"
			},
			{
				IMPRESOR_FISCAL_ERROR_FECHA_NO_VALIDA,
				"Fecha no válida"				
			},
			{
				IMPRESOR_FISCAL_ERROR_HORA_NO_VALIDA,
				"Hora no válida"
			},
			{
				IMPRESOR_FISCAL_ERROR_PARAMETRO_NO_VALIDO,
				"Parámetro no válido"
			},
			{
				IMPRESOR_FISCAL_ERROR_BIT_NO_VALIDO,
				"Identificador de bit no válido"
			},
			{
				IMPRESOR_FISCAL_ERROR_PARAMETRO_OPCIONAL,
				"Un parámetro opcional no es válido o está ausente cuando es necesario"
			},
			{
				IMPRESOR_FISCAL_ERROR_PARAMETRO_OBLIGATORIO,
				"Un parámetro obligatorio no ha sido suministrado"
			},
			{
				IMPRESOR_FISCAL_ERROR_ESTADO_NO_VALIDO,
				"Estado no válido para el requerimiento solicitado"
			},
			{
				IMPRESOR_FISCAL_ERROR_NOMBRE_ARCHIVO,
				"El nombre de archivo no es válido"
			}
		};
		Codigo = Err;
		Contexto = Cont;
		const unsigned nTabla = sizeof ErrDescs / sizeof(ErrDesc);
		for (unsigned i = 0; i < nTabla; i++)
			if (ErrDescs[i].Err == Err)
			{
				Descripcion = ErrDescs[i].Descripcion;
				return;
			}
		Descripcion = "Error desconocido";
	}
	Errores Codigo;
	std::string Descripcion;
	std::string Contexto;

	static std::string GetLastErrorDesc(unsigned ErrNo = 0);
	static std::string SocketGetLastErrorDesc(unsigned ErrNo = 0);
};

#endif
