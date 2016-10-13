#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#ifdef P_WIN32
#include <io.h>
#define    WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "debug.h"
#ifdef P_LINUX
#define _snprintf snprintf
#define _vsnprintf vsnprintf
#endif

#ifdef P_WIN32_VC_2005
#define _vsnprintf _vsnprintf_s

#define lseek _lseek
#define close _close
#define unlink _unlink
#define write _write
#endif

#ifdef P_WIN32_GCC
#define _snprintf snprintf
#define _vsnprintf vsnprintf
#endif

namespace
{
	std::string FormatHeader(const std::string &ModuleName)
	{

		unsigned day, mon, year, hour, min, sec;
#ifdef P_WIN32
		SYSTEMTIME SystemTime;

		GetLocalTime(&SystemTime);
		day = SystemTime.wDay;
		mon = SystemTime.wMonth;
		year = SystemTime.wYear;
		hour = SystemTime.wHour;
		min = SystemTime.wMinute;
		sec = SystemTime.wSecond;
#endif

#ifdef P_LINUX
		time_t ct;
		struct tm *ptm;
		time (&ct);

		ptm = localtime(&ct);
		day = ptm->tm_mday;
		mon = ptm->tm_mon+1;
		year = 1900 + ptm->tm_year;
		hour = ptm->tm_hour;
		min = ptm->tm_min;
		sec = ptm->tm_sec;
#endif
		char Header[512];
#ifdef P_WIN32_VC_2005
		_snprintf_s(Header, (sizeof Header)-1, _TRUNCATE,
#else
		_snprintf(Header, (sizeof Header)-1, 
#endif
			"%02d.%02d.%04d %02d:%02d:%02d %-20.20s",
			day, mon, year, hour, min, sec, ModuleName.c_str());
		return Header;
	}

	std::string GetASCIIChar(char c)
	{
		const char *Chars[] =
		{
			"NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL", "BS",
			"HT", "LF", "VT", "FF", "CR", "SO", "SI", "DLE", "DC1", "DC2",
			"DC3", "DC4", "NAK", "SYN", "ETB", "CAN", "EM", "SUB", "ESC",
			"FS", "GS", "RS", "US"
		};
		unsigned Index = c;
		if (Index > sizeof Chars)
			return "?";
		return Chars[Index];
	}

	std::string Filter(const std::string &s)
	{
		std::string Result;
		for(unsigned i = 0; i < s.length(); i++)
		{
			if (s[i] > 0 && s[i] < 0x20)
				Result += "[" + GetASCIIChar(s[i]) + "]";
			else
				Result += s[i];
		}
		return Result;
	}
}

int
LogFile::Open(const char *File)
{
	int fd;
	char FileBackUp[512];

	if (!File)
		return -1;

#ifdef P_WIN32_VC_2005
	errno_t err = _sopen_s(&fd, File, O_WRONLY | O_CREAT | O_APPEND, 
					_SH_DENYNO, 0666);
	if (err)
		return err;
#else
	if ((fd = open(File, O_WRONLY | O_CREAT | O_APPEND, 0666)) < 0)
		return fd;
#endif

	if (lseek(fd, 0L, 2) >= 5000000L)
	{
		close (fd);

#ifdef P_WIN32_VC_2005
		strcpy_s(FileBackUp, sizeof FileBackUp, File);
#else
		strcpy (FileBackUp, File);
#endif
		FileBackUp[strlen(File)-2] = '_';

		unlink (FileBackUp);
		rename (File, FileBackUp);

#ifdef P_WIN32_VC_2005
		errno_t err = _sopen_s(&fd, File, O_CREAT | O_TRUNC | O_APPEND | O_WRONLY, 
						_SH_DENYNO, 0666);
		if (err)
			return err;
#else
		fd = open(File, O_CREAT | O_TRUNC | O_APPEND | O_WRONLY, 0666);
#endif
	}
	return fd;
}

void
LogFile::WriteIt(const std::string &s)
{
#if 0
#ifdef P_WIN32_VC_2005
	char *FileLog;
	if ( !FileName.length() )
	{
		size_t len;
		if (_dupenv_s(&FileLog, &len, "FILELOG"))
			return;
	}
	else
		FileLog = FileName.c_str();
#else
	const char *FileLog;
	if ( !FileName.length() )
	{
		if ((FileLog = getenv("FILELOG")) == NULL)
			return;
	}
	else
		FileLog = FileName.c_str();
#endif

    int fd;
    if ((fd = Open(FileLog)) >= 0)
	{
		write (fd, s.c_str(), s.length());
		close (fd);
	}
#ifdef P_WIN32_VC_2005
	free((void*)FileLog);
#endif
#endif
}

void
LogFile::DebugIt(const std::string &Module, const std::string &Info)
{

    std::string s = FormatHeader(Module);
    s += " ";
	s += Filter(Info);
	s += "\n";

	WriteIt(s);
}

void
LogFile::Write(const char *Module, const char *Format, ...)
{
	char BufLog[512];

	va_list argptr;

	va_start(argptr, Format);
	_vsnprintf(BufLog, sizeof BufLog, Format, argptr);
	va_end (argptr);
	DebugIt(Module, BufLog);
}

void
LogFile::Write(const std::string &Module, const Excepcion &e)
{
	std::string s(e.Descripcion);
	if (e.Contexto != "")
		s += " (" + e.Contexto + ")";
	DebugIt(Module, s);
}

void
LogFile::Write(const std::string &Module, const std::string &Info)
{
	DebugIt(Module, Info);
}

void
LogFile::Dump(const std::string &Module, const char *Information, unsigned Length)
{
	const unsigned MAX_BYTES = 8;
	const unsigned MAX_BUFFER = 2048;
	char Buffer[MAX_BUFFER];
	char Binary[100], ASCII[100];
	char *p, *q, *r, *s;
	unsigned char Char;
	unsigned Count;

	if( !Length )
		return;

	std::string Header = FormatHeader(Module);
	unsigned HeaderLen = Header.length();

	r = Buffer;
	for (unsigned Offset = 0; Offset < Length; Offset += MAX_BYTES)
	{
		for (q = Binary, s = ASCII, p = (char *)Information + Offset, Count = 0;
			Count < MAX_BYTES && (Offset + Count) < Length;
			Count++)
		{
			Char = static_cast<unsigned char>(*(p + Count) & 0xffffff);
#ifdef P_WIN32_VC_2005
			q += sprintf_s(q, MAX_BYTES, "%02x ", Char);
			s += sprintf_s(s, MAX_BYTES, "%c", Char < 0x20 ? '.' : Char);
#else
			q += sprintf(q, "%02x ", Char);
			s += sprintf(s, "%c", Char < 0x20 ? '.' : Char);
#endif
		}
#ifdef P_WIN32_VC_2005
		Count = _snprintf_s(r, (MAX_BUFFER-(r-Buffer))-1, _TRUNCATE, 
#else
		Count = _snprintf(r, (MAX_BUFFER-(r-Buffer))-1, 
#endif
					"%*.*s %-*.*s %s\n",
					HeaderLen, HeaderLen, Offset ? "" : Header.c_str(),
					MAX_BYTES, MAX_BYTES, ASCII, Binary);
		if (Count < 0)
			break;
		r += Count;
	}
	WriteIt(Buffer);
}
