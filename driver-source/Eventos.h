#ifndef EVENTOS_H
#define EVENTOS_H

#include <string>

class ManejadorDeEventos
{
public:
	virtual void Evento(unsigned Tipo, unsigned SubTipo = 0) = 0;
};

#endif

