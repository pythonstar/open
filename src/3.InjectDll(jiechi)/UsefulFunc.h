#ifndef USEFULFUNC
#define USEFULFUNC


//MyGetProcAddress
DWORD GetFuncAddress( HMODULE phModule,char* pProcName);

//����exe��hModule,���ҵ�dll,apiname,����exe�õ��뺯����IATλ��
DWORD FindImportAddr(DWORD hModule,PCHAR szDllName,PCHAR szApiName);

#endif