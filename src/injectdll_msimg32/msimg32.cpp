// msimg32.cpp : ���� DLL Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#pragma comment(linker,"/align:4096") //��С�ļ���С

#pragma comment(linker,"/export:TransparentBlt=msimg.TransparentBlt")
#pragma comment(linker,"/export:GradientFill=msimg.GradientFill")
#pragma comment(linker,"/export:DllInitialize=msimg.DllInitialize")
#pragma comment(linker,"/export:AlphaBlend=msimg.AlphaBlend")
#pragma comment(linker,"/export:vSetDdrawflag=msimg.vSetDdrawflag")


#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  dwReason,
                       LPVOID lpReserved
					 )
{
	switch ( dwReason )
	{
	case DLL_PROCESS_ATTACH:
		MessageBox(0,L"DLL_PROCESS_ATTACH",0,0);
		break;
	case DLL_PROCESS_DETACH:
		MessageBox(0,L"DLL_PROCESS_DETACH",0,0);
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

