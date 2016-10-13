#include <stdio.h>
#include <windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>

#include <tchar.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <objbase.h>

#ifdef P_MINGW
#include <cfgmgr32_mingw.h>
#else
#include <cfgmgr32.h>
#endif
#include <gadget_tools.h>

#ifndef ARRAYSIZE
#define ARRAYSIZE(a)                (sizeof(a)/sizeof(a[0]))
#endif

#define EXIT_OK      (0)
#define EXIT_REBOOT  (1)
#define EXIT_FAIL    (2)
#define EXIT_USAGE   (3)

namespace
{
	LPTSTR GetDeviceStringProperty(const HDEVINFO Devs,	const PSP_DEVINFO_DATA DevInfo,	const DWORD	Prop)
	{
		LPTSTR buffer;
		DWORD size;
		DWORD reqSize;
		DWORD dataType;
		DWORD szChars;

		size = 1024; //	initial	guess
		buffer = new TCHAR[(size/sizeof(TCHAR))+1];
		if(!buffer)	{
			return NULL;
		}
		while(!SetupDiGetDeviceRegistryProperty(Devs,DevInfo,Prop,&dataType,(LPBYTE)buffer,size,&reqSize)) {
			if(GetLastError() != ERROR_INSUFFICIENT_BUFFER)	{
				goto failed;
			}
			if(dataType	!= REG_SZ) {
				goto failed;
			}
			size = reqSize;
			delete [] buffer;
			buffer = new TCHAR[(size/sizeof(TCHAR))+1];
			if(!buffer)	{
				goto failed;
			}
		}
		szChars	= reqSize/sizeof(TCHAR);
		buffer[szChars]	= TEXT('\0');
		return buffer;

	failed:
		if(buffer) {
			delete [] buffer;
		}
		return NULL;
	}


	LPTSTR GetDeviceDescription(const HDEVINFO Devs, const PSP_DEVINFO_DATA	DevInfo)
	{
		LPTSTR desc;
		desc = GetDeviceStringProperty(Devs,DevInfo,SPDRP_FRIENDLYNAME);
		if(!desc) {
			desc = GetDeviceStringProperty(Devs,DevInfo,SPDRP_DEVICEDESC);
		}
		return desc;
	}

	const char *GADGET_ID = "USB\\VID_0525&PID_A4A7";

	bool DoPort(unsigned Port, bool Reset, const char *desc, bool &rc, HDEVINFO devs, SP_DEVINFO_DATA &devInfo, const TCHAR *devID)
	{
		if (strncmp(devID, GADGET_ID, strlen(GADGET_ID)) !=	0)
			return false;
		const char *p =	strchr(desc, '(');
		if (!p)
			return false;
		char COMName[100];
		p++;
		char *q	= COMName;
		while(*p &&	*p != ')')
			*q++ = *p++;
		*q = 0;
		q =	strchr(COMName,	'M'); // COM
		if (!q)
			return false;
		unsigned nCom =	atoi(q+1);
		if (nCom !=	Port)
			return false;
		if (Reset)
		{
			int	failcode = EXIT_FAIL;
			SP_PROPCHANGE_PARAMS pcp;
			SP_DEVINSTALL_PARAMS devParams;

			pcp.ClassInstallHeader.cbSize =	sizeof(SP_CLASSINSTALL_HEADER);
			pcp.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
			pcp.StateChange	= DICS_PROPCHANGE;
			pcp.Scope =	DICS_FLAG_CONFIGSPECIFIC;
			pcp.HwProfile =	0;

			if(!SetupDiSetClassInstallParams(devs, &devInfo, &pcp.ClassInstallHeader, sizeof(pcp))	||
			   !SetupDiCallClassInstaller(DIF_PROPERTYCHANGE,devs,&devInfo))
			   failcode	= EXIT_FAIL; //	Restart	failed
			else
			{
				devParams.cbSize = sizeof(devParams);
				if(SetupDiGetDeviceInstallParams(devs, &devInfo, &devParams) && (devParams.Flags & (DI_NEEDRESTART|DI_NEEDREBOOT)))
					failcode = EXIT_FAIL; // Reboot	failed
				else
					failcode = EXIT_OK;

				rc = failcode == EXIT_OK;
			}
		}
		else
			rc = true;
		return true;
	}

	bool FindGadgetDevices(unsigned Port, bool Reset)
	{
		bool rc	= false;

		//
		// add all ids to list
		// if there's a class, filter on specified class
		//
		HDEVINFO devs =	SetupDiGetClassDevsEx(NULL,
									 NULL,
									 NULL,
									 DIGCF_ALLCLASSES |	DIGCF_PRESENT,
									 NULL,
									 NULL,
									 NULL);

		if(devs	!= INVALID_HANDLE_VALUE)
		{
			SP_DEVINFO_LIST_DETAIL_DATA	devInfoListDetail;
			devInfoListDetail.cbSize = sizeof(devInfoListDetail);
			if(SetupDiGetDeviceInfoListDetail(devs,	&devInfoListDetail))
			{
				SP_DEVINFO_DATA	devInfo;
				devInfo.cbSize = sizeof(devInfo);
				bool End = false;
				for(DWORD devIndex = 0;	!End &&	SetupDiEnumDeviceInfo(devs,	devIndex, &devInfo); devIndex++)
				{
					TCHAR devID[MAX_DEVICE_ID_LEN];
					SP_DEVINFO_LIST_DETAIL_DATA	devInfoListDetail;
					LPTSTR desc	= NULL;

					desc = GetDeviceDescription(devs,&devInfo);

					devInfoListDetail.cbSize = sizeof(devInfoListDetail);
					if(	(!SetupDiGetDeviceInfoListDetail(devs, &devInfoListDetail))
						||
						(CM_Get_Device_ID_Ex(	devInfo.DevInst,
												devID,
												MAX_DEVICE_ID_LEN,
												0,
												devInfoListDetail.RemoteMachineHandle) != CR_SUCCESS)
						)
						continue;

					if (desc)
					{
						End = DoPort(Port, Reset, desc, rc, devs, devInfo, devID);
						delete [] desc;
					}
				}
			}
			SetupDiDestroyDeviceInfoList(devs);
		}
		return rc;
	}


}

bool
GadgetTools::IsGadgetSerialPort(unsigned nPort)
{
	return FindGadgetDevices(nPort, false);
}

bool
GadgetTools::ResetGadgetSerialPort(unsigned nPort)
{
	return FindGadgetDevices(nPort, true);
}


