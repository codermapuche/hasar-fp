# Microsoft Developer Studio Project File - Name="ClaseFiscal" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=ClaseFiscal - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ClaseFiscal.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ClaseFiscal.mak" CFG="ClaseFiscal - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ClaseFiscal - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ClaseFiscal - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ClaseFiscal - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x2c0a /d "NDEBUG"
# ADD RSC /l 0x2c0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "ClaseFiscal - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GR /GX /ZI /Od /I "." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "P_WIN32" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x2c0a /d "_DEBUG"
# ADD RSC /l 0x2c0a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\win32\winddk\lib\wxp\i386\kernel32.lib ..\win32\winddk\lib\wxp\i386\advapi32.lib ..\win32\winddk\lib\wxp\i386\shell32.lib ..\win32\winddk\lib\wxp\i386\wsock32.lib ..\win32\winddk\lib\wxp\i386\setupapi.lib ..\win32\winddk\lib\wxp\i386\user32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ClaseFiscal - Win32 Release"
# Name "ClaseFiscal - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Chars.cpp
# End Source File
# Begin Source File

SOURCE=.\ClaseFiscal.cpp
# End Source File
# Begin Source File

SOURCE=.\debug.cpp
# End Source File
# Begin Source File

SOURCE=.\Excepcion.cpp
# End Source File
# Begin Source File

SOURCE=.\gadget_tools.cpp

!IF  "$(CFG)" == "ClaseFiscal - Win32 Release"

!ELSEIF  "$(CFG)" == "ClaseFiscal - Win32 Debug"

# ADD CPP /I "..\win32\winddk\inc\crt" /I "..\win32\winddk\inc\wxp"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ImpresorFiscal.cpp
# End Source File
# Begin Source File

SOURCE=.\ImpresorFiscal16Bits.cpp
# End Source File
# Begin Source File

SOURCE=.\ImpresorFiscal8bits.cpp
# End Source File
# Begin Source File

SOURCE=.\Mutex.cpp
# End Source File
# Begin Source File

SOURCE=.\P1120_100.cpp
# End Source File
# Begin Source File

SOURCE=.\P262_100.cpp
# End Source File
# Begin Source File

SOURCE=.\P272.cpp
# End Source File
# Begin Source File

SOURCE=.\P272_100.cpp
# End Source File
# Begin Source File

SOURCE=.\P272_201.cpp
# End Source File
# Begin Source File

SOURCE=.\P320_100.cpp
# End Source File
# Begin Source File

SOURCE=.\P321_100.cpp
# End Source File
# Begin Source File

SOURCE=.\P322.cpp
# End Source File
# Begin Source File

SOURCE=.\P322_100.cpp
# End Source File
# Begin Source File

SOURCE=.\P322_201.cpp
# End Source File
# Begin Source File

SOURCE=.\p330.cpp
# End Source File
# Begin Source File

SOURCE=.\p330_100.cpp
# End Source File
# Begin Source File

SOURCE=.\p330_201.cpp
# End Source File
# Begin Source File

SOURCE=.\p330_202.cpp
# End Source File
# Begin Source File

SOURCE=.\P330_203.cpp
# End Source File
# Begin Source File

SOURCE=.\P340_100.cpp
# End Source File
# Begin Source File

SOURCE=.\p425.cpp
# End Source File
# Begin Source File

SOURCE=.\p425_100.cpp
# End Source File
# Begin Source File

SOURCE=.\p425_201.cpp
# End Source File
# Begin Source File

SOURCE=.\P435.cpp
# End Source File
# Begin Source File

SOURCE=.\P435_100.cpp
# End Source File
# Begin Source File

SOURCE=.\P435_101.cpp
# End Source File
# Begin Source File

SOURCE=.\P435_102.cpp
# End Source File
# Begin Source File

SOURCE=.\P435_203.cpp
# End Source File
# Begin Source File

SOURCE=.\p441.cpp
# End Source File
# Begin Source File

SOURCE=.\P441_100.cpp
# End Source File
# Begin Source File

SOURCE=.\P441_201.cpp
# End Source File
# Begin Source File

SOURCE=.\P614_100.cpp
# End Source File
# Begin Source File

SOURCE=.\P615_100.cpp
# End Source File
# Begin Source File

SOURCE=.\P715.cpp
# End Source File
# Begin Source File

SOURCE=.\P715_100.cpp
# End Source File
# Begin Source File

SOURCE=.\P715_201.cpp
# End Source File
# Begin Source File

SOURCE=.\P715_302.cpp
# End Source File
# Begin Source File

SOURCE=.\P715_403.cpp
# End Source File
# Begin Source File

SOURCE=.\P715_504.cpp
# End Source File
# Begin Source File

SOURCE=.\P950_100.cpp
# End Source File
# Begin Source File

SOURCE=.\P951_100.cpp
# End Source File
# Begin Source File

SOURCE=.\PJ20_100.cpp
# End Source File
# Begin Source File

SOURCE=.\PPL23.cpp
# End Source File
# Begin Source File

SOURCE=.\PPL23_100.cpp
# End Source File
# Begin Source File

SOURCE=.\PPL23_101.cpp
# End Source File
# Begin Source File

SOURCE=.\PPL23_202.cpp
# End Source File
# Begin Source File

SOURCE=.\ppl8.cpp
# End Source File
# Begin Source File

SOURCE=.\PPL8_100.cpp
# End Source File
# Begin Source File

SOURCE=.\PPL8_201.cpp
# End Source File
# Begin Source File

SOURCE=.\PPL9_100.cpp
# End Source File
# Begin Source File

SOURCE=.\PPR4_100.cpp
# End Source File
# Begin Source File

SOURCE=.\PPR5.cpp
# End Source File
# Begin Source File

SOURCE=.\PPR5_100.cpp
# End Source File
# Begin Source File

SOURCE=.\PPR5_201.cpp
# End Source File
# Begin Source File

SOURCE=.\PPR5_302.cpp
# End Source File
# Begin Source File

SOURCE=.\Protocolo.cpp
# End Source File
# Begin Source File

SOURCE=.\PuertoSerie.cpp
# End Source File
# Begin Source File

SOURCE=.\Select.cpp
# End Source File
# Begin Source File

SOURCE=.\Socket.cpp
# End Source File
# Begin Source File

SOURCE=.\ssplit.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Chars.h
# End Source File
# Begin Source File

SOURCE=.\debug.h
# End Source File
# Begin Source File

SOURCE=.\Eventos.h
# End Source File
# Begin Source File

SOURCE=.\Excepcion.h
# End Source File
# Begin Source File

SOURCE=.\ImpresorFiscal.h
# End Source File
# Begin Source File

SOURCE=.\ImpresorFiscal16Bits.h
# End Source File
# Begin Source File

SOURCE=.\ImpresorFiscal8bits.h
# End Source File
# Begin Source File

SOURCE=.\min_max.h
# End Source File
# Begin Source File

SOURCE=.\Mutex.h
# End Source File
# Begin Source File

SOURCE=.\P1120_100.h
# End Source File
# Begin Source File

SOURCE=.\P262_100.h
# End Source File
# Begin Source File

SOURCE=.\P272.h
# End Source File
# Begin Source File

SOURCE=.\P272_100.h
# End Source File
# Begin Source File

SOURCE=.\P272_201.h
# End Source File
# Begin Source File

SOURCE=.\P320_100.h
# End Source File
# Begin Source File

SOURCE=.\P321_100.h
# End Source File
# Begin Source File

SOURCE=.\P322.h
# End Source File
# Begin Source File

SOURCE=.\P322_100.h
# End Source File
# Begin Source File

SOURCE=.\P322_201.h
# End Source File
# Begin Source File

SOURCE=.\p330.h
# End Source File
# Begin Source File

SOURCE=.\p330_100.h
# End Source File
# Begin Source File

SOURCE=.\p330_201.h
# End Source File
# Begin Source File

SOURCE=.\p330_202.h
# End Source File
# Begin Source File

SOURCE=.\P330_203.h
# End Source File
# Begin Source File

SOURCE=.\P340_100.h
# End Source File
# Begin Source File

SOURCE=.\p425.h
# End Source File
# Begin Source File

SOURCE=.\p425_100.h
# End Source File
# Begin Source File

SOURCE=.\p425_201.h
# End Source File
# Begin Source File

SOURCE=.\P435.h
# End Source File
# Begin Source File

SOURCE=.\P435_100.h
# End Source File
# Begin Source File

SOURCE=.\P435_101.h
# End Source File
# Begin Source File

SOURCE=.\P435_102.h
# End Source File
# Begin Source File

SOURCE=.\P435_203.h
# End Source File
# Begin Source File

SOURCE=.\p441.h
# End Source File
# Begin Source File

SOURCE=.\P441_100.h
# End Source File
# Begin Source File

SOURCE=.\P441_201.h
# End Source File
# Begin Source File

SOURCE=.\P614_100.h
# End Source File
# Begin Source File

SOURCE=.\P615_100.h
# End Source File
# Begin Source File

SOURCE=.\P715.h
# End Source File
# Begin Source File

SOURCE=.\P715_100.h
# End Source File
# Begin Source File

SOURCE=.\P715_201.h
# End Source File
# Begin Source File

SOURCE=.\P715_302.h
# End Source File
# Begin Source File

SOURCE=.\P715_403.h
# End Source File
# Begin Source File

SOURCE=.\P715_504.h
# End Source File
# Begin Source File

SOURCE=.\P950_100.h
# End Source File
# Begin Source File

SOURCE=.\P951_100.h
# End Source File
# Begin Source File

SOURCE=.\PJ20_100.h
# End Source File
# Begin Source File

SOURCE=.\PPL23.h
# End Source File
# Begin Source File

SOURCE=.\PPL23_100.h
# End Source File
# Begin Source File

SOURCE=.\PPL23_101.h
# End Source File
# Begin Source File

SOURCE=.\PPL23_202.h
# End Source File
# Begin Source File

SOURCE=.\ppl8.h
# End Source File
# Begin Source File

SOURCE=.\PPL8_100.h
# End Source File
# Begin Source File

SOURCE=.\PPL8_201.h
# End Source File
# Begin Source File

SOURCE=.\PPL9_100.h
# End Source File
# Begin Source File

SOURCE=.\PPR4_100.h
# End Source File
# Begin Source File

SOURCE=.\PPR5.h
# End Source File
# Begin Source File

SOURCE=.\PPR5_100.h
# End Source File
# Begin Source File

SOURCE=.\PPR5_201.h
# End Source File
# Begin Source File

SOURCE=.\PPR5_302.h
# End Source File
# Begin Source File

SOURCE=.\Protocolo.h
# End Source File
# Begin Source File

SOURCE=.\ssplit.h
# End Source File
# Begin Source File

SOURCE=.\Transporte.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ClaseFiscal.dsp
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
