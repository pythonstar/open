
#define MappingFileName	_T("INJECTTESTSHAREMEM")
#define WM_INJECTSUCCESS	WM_USER+1

typedef struct SHAREMEM
{
	HWND hwndLoader;
	DWORD dwOldProtect;
	DWORD pCodeStart;
	int nCodeLen;
	BYTE pOrigCode[1024*10];
}*PSHAREMEM;
