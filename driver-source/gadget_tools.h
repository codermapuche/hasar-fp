#ifndef GADGET_TOOLS_H
#define GADGET_TOOLS_H

namespace GadgetTools
{
	// Devuelve si el n£mero de COM es un puerto
	// serial USB de gadget.
    bool IsGadgetSerialPort(unsigned nPort);

	// Resetea un puerto Gadget.
	bool ResetGadgetSerialPort(unsigned nPort);
}

#endif
