#ifndef USEFULFUNC
#define USEFULFUNC


//MyGetProcAddress
DWORD GetFuncAddress( HMODULE phModule,char* pProcName);

//传入exe的hModule,带找的dll,apiname,返回exe该导入函数的IAT位置
DWORD FindImportAddr(DWORD hModule,PCHAR szDllName,PCHAR szApiName);

#endif