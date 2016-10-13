#ifndef SSPLIT_H
#define SSPLIT_H

#if defined(P_WIN32) && !defined(P_WIN32_GCC)
#pragma warning(disable:4786) // disable spurious C4786 warnings
#endif
#include <string>
#include <vector>

const std::string split_space = " \t\f\r\n\0x1a"; 

// Quita caracteres al principio y al final del string
std::string trim(const std::string & s, const std::string & padding = split_space);

// Separa campos por secuencias de caracteres contenidos en el separador.
// Quita los separadores al principio y al final.
// Si se especifica una cantidad de campos maxima y hay una cantidad
// mayor, los ultimos quedan sin separar.
std::vector <std::string> ssplit(const std::string & s, unsigned maxfields = 0, 
								 const std::string & separator = split_space);
std::vector <std::string> ssplit(const std::string & s, const std::string & separator,
							unsigned maxfields = 0);

// Separa campos por uno cualquiera de los caracteres contenidos en el
// string separador. No ignora separadores al principio y al final y
// devuelve campos vacios si hay dos separadores consecutivos.
// Opcionalmente, recorta los espacios en blanco al principio y al 
// final de cada campo.
std::vector <std::string> csplit(const std::string & s, const std::string & separator,
							bool trimfields = false);

// Separa campos por espacios en blanco y/o uno cualquiera de los caracteres
// contenidos en el separador opcional. No separa secuencias contenidas 
// entre comillas dobles. Para escapar una comilla doble, usar una barra
// invertida. Quita las barras invertidas delante de las comillas. Un
// separador antes del primer campo o dos separadores consecutivos generan
// un campo vacio.
std::vector <std::string> qsplit(const std::string & s, const std::string & separator = "");

#endif
