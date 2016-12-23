#include <windows.h>
#include "DevCode.h"
#include "UsefulFunc.h"


#pragma comment(linker,"/BASE:0x62c20000")
#pragma comment(linker, "/SECTION:.text,REW" ) //设PE节：.text,可读可执行
#pragma comment(linker, "/FILEALIGN:0x200")
#pragma comment(linker, "/entry:DllMain")

typedef void  (__stdcall  * MYAPI)();

typedef int (__stdcall *pNtProtectVirtualMemory)(DWORD, DWORD, DWORD, DWORD, DWORD);
typedef int (__stdcall *pNtWriteVirtualMemory)(DWORD, DWORD, DWORD, DWORD, DWORD);
typedef int (__stdcall *pNtFreeVirtualMemory)(DWORD, DWORD, DWORD, DWORD);
typedef int (__stdcall *pNtOpenProcess)(DWORD, DWORD, DWORD, DWORD);
typedef int (__stdcall *pNtAllocateVirtualMemory)(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD);
typedef int (__stdcall *pNtQueryVirtualMemory)(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD);
typedef int (__stdcall *pNtReadVirtualMemory)(DWORD, DWORD, DWORD, DWORD, DWORD);

pNtProtectVirtualMemory NtProtectVirtualMemory;
pNtWriteVirtualMemory NtWriteVirtualMemory;
pNtFreeVirtualMemory NtFreeVirtualMemory;
pNtOpenProcess NtOpenProcess;
pNtAllocateVirtualMemory NtAllocateVirtualMemory;
pNtQueryVirtualMemory NtQueryVirtualMemory;
pNtReadVirtualMemory NtReadVirtualMemory;





HMODULE lpk_module = NULL;

void ApiInit();
BOOL WINAPI NewDeviceIoControl( HANDLE hDevice, 
							   DWORD dwIoControlCode, 
							   LPVOID lpInBuffer, 
							   DWORD nInBufferSize, 
							   LPVOID lpOutBuffer, 
							   DWORD nOutBufferSize, 
							   LPDWORD lpBytesReturned, 
							   LPOVERLAPPED lpOverlapped );


MYAPI pLpkInitialize;
MYAPI pLpkTabbedTextOut;
MYAPI pLpkDllInitialize;
MYAPI pLpkDrawTextEx;
MYAPI pLpkEditControl;
MYAPI pLpkExtTextOut;
MYAPI pLpkGetCharacterPlacement;
MYAPI pLpkGetTextExtentExPoint;
MYAPI pLpkPSMTextOut;
MYAPI pLpkUseGDIWidthCache;
MYAPI pftsWordBreak;


__declspec(naked) void LpkInitialize(){__asm jmp dword ptr [pLpkInitialize]}
__declspec(naked) void LpkTabbedTextOut(){__asm jmp dword ptr [pLpkTabbedTextOut]}
__declspec(naked) void LpkDllInitialize(){__asm jmp dword ptr [pLpkDllInitialize]}
__declspec(naked) void LpkDrawTextEx(){__asm jmp dword ptr [pLpkDrawTextEx]}

__declspec(naked) void LpkExtTextOut(){__asm jmp dword ptr [pLpkExtTextOut]}
__declspec(naked) void LpkGetCharacterPlacement(){__asm jmp dword ptr [pLpkGetCharacterPlacement]}
__declspec(naked) void LpkGetTextExtentExPoint(){__asm jmp dword ptr [pLpkGetTextExtentExPoint]}
__declspec(naked) void LpkPSMTextOut(){__asm jmp dword ptr [pLpkPSMTextOut]}
__declspec(naked) void LpkUseGDIWidthCache(){__asm jmp dword ptr [pLpkUseGDIWidthCache]}
__declspec(naked) void ftsWordBreak(){
	__asm jmp dword ptr [pftsWordBreak]
		__asm nop
		__asm nop
		__asm nop
		__asm nop
}

__declspec(naked) void LpkEditControl()
{
__asm nop
__asm nop
__asm nop
__asm nop

__asm nop
__asm nop
__asm nop
__asm nop

__asm nop
__asm nop
__asm nop
__asm nop

__asm nop
__asm nop
__asm nop
__asm nop

__asm nop
__asm nop
__asm nop
__asm nop

__asm nop
__asm nop
__asm nop
__asm nop

__asm nop
__asm nop
__asm nop
__asm nop

__asm nop
__asm nop
__asm nop
__asm nop

__asm nop
__asm nop
__asm nop
__asm nop

__asm nop
__asm nop
__asm nop
__asm nop

__asm nop
__asm nop
__asm nop
__asm nop

__asm nop
__asm nop
__asm nop
__asm nop

__asm nop
__asm nop
__asm nop
__asm nop

__asm nop
__asm nop
__asm nop
__asm nop

__asm nop
__asm nop
__asm nop
__asm nop

__asm _emit 0
__asm _emit 0
__asm _emit 0
__asm _emit 0

}

void ApiInit()
{
	HMODULE ntdll_module;
	char reallpk[MAX_PATH]={0};
	GetSystemDirectoryA((LPSTR)reallpk,MAX_PATH);
	strcat(reallpk,"\\lpk.dll");
	if(lpk_module=LoadLibraryA(reallpk))
	{
		pLpkInitialize = (MYAPI) GetProcAddress(lpk_module,"LpkInitialize");
		pLpkTabbedTextOut = (MYAPI) GetProcAddress(lpk_module,"LpkTabbedTextOut");
		pLpkDllInitialize = (MYAPI) GetProcAddress(lpk_module,"LpkDllInitialize");
		pLpkDrawTextEx = (MYAPI) GetProcAddress(lpk_module,"LpkDrawTextEx");
		pLpkExtTextOut = (MYAPI) GetProcAddress(lpk_module,"LpkExtTextOut");
		pLpkGetCharacterPlacement = (MYAPI) GetProcAddress(lpk_module,"LpkGetCharacterPlacement");
		pLpkEditControl = (MYAPI) GetProcAddress(lpk_module,"LpkEditControl");
		pLpkGetTextExtentExPoint = (MYAPI) GetProcAddress(lpk_module,"LpkGetTextExtentExPoint");
		pLpkPSMTextOut = (MYAPI) GetProcAddress(lpk_module,"LpkPSMTextOut");
		pLpkUseGDIWidthCache = (MYAPI) GetProcAddress(lpk_module,"LpkUseGDIWidthCache");
		pftsWordBreak = (MYAPI) GetProcAddress(lpk_module,"ftsWordBreak");
		
		
		CopyMemory((LPVOID)((DWORD)LpkEditControl-4),(PVOID)((DWORD)pLpkEditControl-4),0x44);

		ntdll_module = (HMODULE) LoadLibraryA("ntdll.dll");
		if (ntdll_module)
		{
			NtProtectVirtualMemory = (pNtProtectVirtualMemory) GetProcAddress(ntdll_module,"NtProtectVirtualMemory");
			NtAllocateVirtualMemory = (pNtAllocateVirtualMemory) GetProcAddress(ntdll_module,"NtAllocateVirtualMemory");
			NtFreeVirtualMemory = (pNtFreeVirtualMemory) GetProcAddress(ntdll_module,"NtFreeVirtualMemory");
			NtQueryVirtualMemory = (pNtQueryVirtualMemory) GetProcAddress(ntdll_module,"NtQueryVirtualMemory");
			NtReadVirtualMemory = (pNtReadVirtualMemory) GetProcAddress(ntdll_module,"NtReadVirtualMemory");
			NtWriteVirtualMemory = (pNtWriteVirtualMemory) GetProcAddress(ntdll_module,"NtWriteVirtualMemory");
			NtOpenProcess = (pNtOpenProcess) GetProcAddress(ntdll_module,"NtOpenProcess");
		}
		else
		{
			ExitProcess(0);
		}

	}
	else
	{
		ExitProcess(0);
	}
	
}



BOOL WINAPI NewDeviceIoControl( HANDLE hDevice, 
							   DWORD dwIoControlCode, 
							   LPVOID lpInBuffer, 
							   DWORD nInBufferSize, 
							   LPVOID lpOutBuffer, 
							   DWORD nOutBufferSize, 
							   LPDWORD lpBytesReturned, 
							   LPOVERLAPPED lpOverlapped )
{
	switch(dwIoControlCode)
	{
	case CALL_NTPVM:
		return NtProtectVirtualMemory(*(DWORD*)lpInBuffer,*((DWORD*)lpInBuffer+1),*((DWORD*)lpInBuffer+2),
			*((DWORD*)lpInBuffer+3),*((DWORD*)lpInBuffer+4));
		break;
	case CALL_NTAVM:
		return NtAllocateVirtualMemory(*(DWORD*)lpInBuffer,*((DWORD*)lpInBuffer+1),*((DWORD*)lpInBuffer+2),
			*((DWORD*)lpInBuffer+3),*((DWORD*)lpInBuffer+4),*((DWORD*)lpInBuffer+5));
		break;
	case CALL_NTFVM:
		return NtFreeVirtualMemory(*(DWORD*)lpInBuffer,*((DWORD*)lpInBuffer+1),*((DWORD*)lpInBuffer+2),
			*((DWORD*)lpInBuffer+3));
		break;	
	case CALL_NTQVM:
		return NtQueryVirtualMemory(*(DWORD*)lpInBuffer,*((DWORD*)lpInBuffer+1),*((DWORD*)lpInBuffer+2),
			*((DWORD*)lpInBuffer+3),*((DWORD*)lpInBuffer+4),*((DWORD*)lpInBuffer+5));
		break;	
	case CALL_NTRVM:
		return NtReadVirtualMemory(*(DWORD*)lpInBuffer,*((DWORD*)lpInBuffer+1),*((DWORD*)lpInBuffer+2),
			*((DWORD*)lpInBuffer+3),*((DWORD*)lpInBuffer+4));
		break;	
	case CALL_NTWVM:
		return NtWriteVirtualMemory(*(DWORD*)lpInBuffer,*((DWORD*)lpInBuffer+1),*((DWORD*)lpInBuffer+2),
			*((DWORD*)lpInBuffer+3),*((DWORD*)lpInBuffer+4));
		break;	
	case CALL_NTOP:
		return NtOpenProcess(*(DWORD*)lpInBuffer,*((DWORD*)lpInBuffer+1),*((DWORD*)lpInBuffer+2),
			*((DWORD*)lpInBuffer+3));
		break;
	case SHIT:
		//解压过程，不理它
		return 0;
		break;
	default :
		return DeviceIoControl(hDevice,dwIoControlCode,lpInBuffer,nInBufferSize,lpOutBuffer,nOutBufferSize,lpBytesReturned,lpOverlapped);
		break;
	}

	return 0;
}


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	DWORD dwApi;
	DWORD retlen;
	switch(ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		ApiInit();
		dwApi = FindImportAddr((DWORD)GetModuleHandle(NULL),"kernel32.dll","DeviceIoControl");
		if(dwApi)
		{
			VirtualProtect((LPVOID)dwApi,4,PAGE_READWRITE,&retlen);
			*(DWORD*)dwApi = (DWORD) NewDeviceIoControl;
		}
		break;
	case DLL_PROCESS_DETACH:
		
		break;
	case DLL_THREAD_ATTACH:

		break;
	case DLL_THREAD_DETACH:

		break;
	}
    return TRUE;
}

