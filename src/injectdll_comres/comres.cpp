// msimg32.cpp : ���� DLL Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#pragma comment(linker,"/align:4096") //��С�ļ���С

#pragma comment(linker,"/export:COMResModuleInstance=comrec.COMResModuleInstance")


#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	MessageBox(0,L"injected",0,0);
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

