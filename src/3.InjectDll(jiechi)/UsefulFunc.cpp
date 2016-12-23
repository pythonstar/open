
#include <Windows.h>
#include <stdio.h>
#include "UsefulFunc.h"


#pragma warning(disable:4312)
#pragma warning(disable:4244)



DWORD FindImportAddr(DWORD hModule,PCHAR szDllName,PCHAR szApiName)
{
	PIMAGE_DOS_HEADER pDosHdr;
	PIMAGE_NT_HEADERS pNtHdr;
	PIMAGE_IMPORT_DESCRIPTOR pIID;

	PIMAGE_IMPORT_BY_NAME pImport;
	DWORD* dwName;
	DWORD* pAddress;

	PCHAR lpDllName;

	pDosHdr = (PIMAGE_DOS_HEADER) hModule;
	pNtHdr = (PIMAGE_NT_HEADERS) (pDosHdr->e_lfanew + hModule);

	pIID = (PIMAGE_IMPORT_DESCRIPTOR)(pNtHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress + hModule);

	while(pIID->FirstThunk !=NULL)
	{
		lpDllName = (PCHAR)(pIID->Name + hModule);
		if (strcmp(lpDllName,szDllName))
		{
			goto nextdll;
		}
		if(pIID->OriginalFirstThunk == NULL)
			dwName = (DWORD*)(pIID->FirstThunk + hModule);
		else
			dwName = (DWORD*)(pIID->OriginalFirstThunk + hModule);	

		pAddress = (DWORD*) (pIID->FirstThunk + hModule);
		while (dwName)
		{
			pImport = (PIMAGE_IMPORT_BY_NAME)(*(DWORD*)dwName+hModule);
			if (!strcmp((char *)pImport->Name,szApiName))
			{
				return (DWORD)pAddress;
			}
			dwName ++;
			pAddress++;
		}
		
nextdll:
		//pIID = (PIMAGE_IMPORT_DESCRIPTOR) ((DWORD)pIID + sizeof(pIID) + hOdModule);
		pIID++;
	}
	return 0;
}



DWORD GetFuncAddress( HMODULE phModule,char* pProcName)
{
	if (!phModule)
		return  0;

	PIMAGE_DOS_HEADER  pimDH  =  (PIMAGE_DOS_HEADER)phModule;
	PIMAGE_NT_HEADERS  pimNH  =  (PIMAGE_NT_HEADERS)((char*)phModule+pimDH->e_lfanew);
	PIMAGE_EXPORT_DIRECTORY  pimED  =  (PIMAGE_EXPORT_DIRECTORY)((DWORD)phModule+pimNH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	DWORD  pExportSize  =  pimNH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
	DWORD  pResult  =  0;


	if ((DWORD)pProcName < 0x10000)
	{
		if ((DWORD)pProcName  >=  pimED->NumberOfFunctions+pimED->Base || (DWORD)pProcName < pimED->Base)
			return  0;
		pResult  =  (DWORD)phModule+((DWORD*)((DWORD)phModule+pimED->AddressOfFunctions))[(DWORD)pProcName-pimED->Base];
	}
	else
	{
		DWORD*  pAddressOfNames  =  (DWORD*)((DWORD)phModule+pimED->AddressOfNames);
		for (int i=0;i<pimED->NumberOfNames;i++)
		{
			char*  pExportName  =  (char*)(pAddressOfNames[i]+(DWORD)phModule);
			if (strcmp(pProcName,pExportName) == 0)
			{
				WORD*  pAddressOfNameOrdinals  =  (WORD*)((DWORD)phModule+pimED->AddressOfNameOrdinals);
				pResult    =  (DWORD)phModule+((DWORD*)((DWORD)phModule+pimED->AddressOfFunctions))[pAddressOfNameOrdinals[i]];
				break;
			}
		}
	}
	if  (pResult != 0 && pResult  >=  (DWORD)pimED  &&  pResult <  (DWORD)pimED+pExportSize)
	{
		char*  pDirectStr  =  (char*)pResult;
		bool  pstrok  =  false;
		while (*pDirectStr)
		{
			if (*pDirectStr == '.')
			{
				pstrok  =  true;
				break;
			}
			pDirectStr++;
		}
		if (!pstrok)
			return  0;
		char  pdllname[MAX_PATH];
		int    pnamelen  =  pDirectStr-(char*)pResult;
		if (pnamelen <= 0)
			return  0;
		memcpy(pdllname,(char*)pResult,pnamelen);
		pdllname[pnamelen] = 0;
		HMODULE  phexmodule  =  GetModuleHandle(pdllname);
		if (phexmodule == NULL)
			phexmodule = LoadLibraryA(pdllname);

		pResult  =  GetFuncAddress(phexmodule,pDirectStr+1);
	}
	return pResult;
}


