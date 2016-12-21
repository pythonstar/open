
#define MappingFileName	"SINGSHAREMEM"
#define WM_HOSTEXIT		WM_USER+103
#define WM_ENTERROOM	WM_USER+104

typedef struct SHAREMEM
{
	HWND	hControlWnd;
	HWND	hDummyWnd;
	HWND	hDllWnd;
	DWORD	dwTreeThis;
	DWORD	dwSkinListThis;
}*PSHAREMEM;
