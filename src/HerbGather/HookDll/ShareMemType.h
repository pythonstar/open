
#define MappingFileName	"SINGSHAREMEM"
#define WM_NPCDEAD		WM_USER+102
#define WM_HOSTEXIT		WM_USER+103
#define WM_GETHPADDR	WM_USER+104

typedef struct SHAREMEM
{
	HANDLE	hMainWnd;
	char	buffer[128];
}*PSHAREMEM;
