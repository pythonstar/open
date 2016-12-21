

#include <windows.h>
#include "HookSpyDll.h"


//-------------------------------------------------------
// DllMain
//
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	::MessageBox(0,0,0,0);
    return TRUE;
}

