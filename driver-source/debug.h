#ifndef DEBUG_H
#define DEBUG_H

#include <string>
#include "Excepcion.h"

class LogFile
{
public:
	void Write(const char *Module, const char *Format, ...);
	void Write(const std::string &Module, const std::string &Info);
	void Write(const std::string &Module, const Excepcion &e);
	void Dump(const std::string &Module, const char *Information, unsigned Length);

	void SetFileName (const std::string Name) { FileName = Name; }
	std::string GetFileName (void) { return FileName; }

private:
	int Open(const char *File);
	void WriteIt(const std::string &s);
	void DebugIt(const std::string &Module, const std::string &Info);

	std::string FileName;
};

#endif
