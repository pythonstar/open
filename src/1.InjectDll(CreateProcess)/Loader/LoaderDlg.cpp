// LoaderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include <Psapi.h>
#include "Loader.h"
#include "LoaderDlg.h"
#include "../../../lib/StarLib/Common/common.h"


#pragma comment(lib,"Psapi.lib")
#pragma comment(linker, "/SECTION:.text,REW" ) //设PE节：.text,可读可执行

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLoaderDlg 对话框




CLoaderDlg::CLoaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoaderDlg::IDD, pParent)
	, m_strExeFile(_T(""))
	, m_strDllFile(_T(""))
	, m_strDllFunc(_T(""))
{
	m_hMapFile=NULL;
	m_pShareMem=NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLoaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strExeFile);
	DDX_Text(pDX, IDC_EDIT2, m_strDllFile);
	DDX_Text(pDX, IDC_EDIT3, m_strDllFunc);
}

BEGIN_MESSAGE_MAP(CLoaderDlg, CDialog)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CLoaderDlg::OnBnClickedOk)
	ON_MESSAGE(WM_INJECTSUCCESS,OnInjectSuccess)
END_MESSAGE_MAP()


// CLoaderDlg 消息处理程序

BOOL CLoaderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_strExeFile=Star::Common::GetStartPath()+_T("target.exe");
	m_strDllFile=Star::Common::GetStartPath()+_T("inject.dll");
	m_strDllFunc=_T("start");
	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}



void szDllFile();

__declspec(naked) DWORD patchcodestart(){

	__asm {
		//_emit 0xCC; 
	}

	__asm{
		//获取kernel32.dll的基址
		push dword ptr[esp];
		call _GetKernelBase;

		//重定位
		call _label0;
_label0:
		pop ebx;
		sub ebx,_label0;

		//获取LoadLibraryA函数的地址
		lea esi,dword ptr [ebx+szLoadLibraryA];
		push esi;
		push eax;
		call _GetApi;

		//LoadLibraryA(目标dll)
		lea esi,dword ptr [ebx+szDllFile];
		push esi;
		call eax;

		//获取并调用目标dll的导出函数，让这个函数做下善后工作
		lea esi,dword ptr [ebx+szDllFile];
		add esi,MAX_PATH;	//szDllFile+MAX_PATH开始为目标dll导出的函数名
		push esi;
		push eax;
		call _GetApi;

		lea esi,dword ptr [ebx+patchcodestart];
		push esi			//设置返回地址为OEP
		jmp eax;			//注：这里是jmp，最后导出函数会jmp回OEP的
	}

	//设置seh
	__asm{
_SEHHandler:
		push ebp;
		mov ebp,esp;
		pushad;
		mov esi,dword ptr ss:[ebp+0x8];
		mov edi,dword ptr ss:[ebp+0x10];
		mov eax,dword ptr ss:[ebp+0xC];
		push dword ptr ds:[eax+0xC];
		pop dword ptr ds:[edi+0xB4];
		push dword ptr ds:[eax+0x8];
		pop dword ptr ds:[edi+0xB8];
		push eax;
		pop dword ptr ds:[edi+0xC4];
		popad;
		mov eax,0;
		leave;
		retn 10;
	}

	//动态获取kernel32.dll基址的函数，需要一个参数为：[esp]
	__asm{
_GetKernelBase:
		push ebp;
		mov ebp,esp;
		add esp,-4;
		pushad;
		mov dword ptr ss:[ebp-4],0;
		call _label1;
_label1:
		pop ebx;
		sub ebx,_label1;
		push ebp;
		lea eax,dword ptr ds:[ebx+_PageError];
		push eax;
		lea eax,dword ptr ds:[ebx+_SEHHandler];
		push eax;
		push dword ptr fs:[0];
		mov dword ptr fs:[0],esp;
		mov edi,dword ptr ss:[ebp+8];
		and edi,0xFFFF0000;
_startwhile:
		cmp word ptr ds:[edi],0x5A4D;
		jnz  _PageError;
		mov esi,edi;
		add esi,dword ptr ds:[esi+0x3C];
		cmp word ptr ds:[esi],0x4550;
		jnz  _PageError;
		mov dword ptr ss:[ebp-4],edi;
		jmp short _endwhile;
_PageError:
		sub edi,0x10000;
		cmp edi,0x70000000;
		jb short _endwhile;
		jmp short _startwhile;
_endwhile:
		pop dword ptr fs:[0];
		add esp,0x0C;
		popad;
		mov eax,dword ptr ss:[ebp-4];
		leave;
		retn 4;
	}

	//动态获取dll中的某个导出函数，参数1为dll基址，参数2为函数名
	__asm{
_GetApi:
		push ebp;
		mov ebp,esp;
		add esp,-8;
		pushad;
		mov dword ptr ss:[ebp-4],0;
		call _label2;
_label2:
		pop ebx;
		sub ebx,_label2;
		push ebp;
		lea eax,dword ptr ds:[ebx+_Error];
		push eax;
		lea eax,dword ptr ds:[ebx+_SEHHandler];
		push eax;
		push dword ptr fs:[0];
		mov dword ptr fs:[0],esp;
		mov edi,dword ptr ss:[ebp+0xC];
		mov ecx,-1;
		xor al,al;
		cld;
		repne scas byte ptr es:[edi];
		mov ecx,edi;
		sub ecx,dword ptr ss:[ebp+0xC];
		mov dword ptr ss:[ebp-8],ecx;
		mov esi,dword ptr ss:[ebp+0x8];
		add esi,dword ptr ds:[esi+0x3C];
		mov esi,dword ptr ds:[esi+0x78];
		add esi,dword ptr ss:[ebp+0x8];
		mov ebx,dword ptr ds:[esi+0x20];
		add ebx,dword ptr ss:[ebp+0x8];
		xor edx,edx;
_repeat:
		push esi;
		mov edi,dword ptr ds:[ebx];
		add edi,dword ptr ss:[ebp+8];
		mov esi,dword ptr ss:[ebp+0xC];
		mov ecx,dword ptr ss:[ebp-8];
		repe cmpsb;
		jnz _notmatch;
		pop esi;
		jmp _matched;
_notmatch:
		pop esi;
		add ebx,4;
		inc edx;
		cmp edx,dword ptr ds:[esi+0x18];
		jb _repeat;
		jmp _Error;

_matched:
		sub ebx,dword ptr ds:[esi+0x20];
		sub ebx,dword ptr ss:[ebp+0x8];
		shr ebx,1;
		add ebx,dword ptr ds:[esi+0x24];
		add ebx,dword ptr ss:[ebp+0x8];
		movzx eax,word ptr ds:[ebx];
		shl eax,2;
		add eax,dword ptr ds:[esi+0x1C];
		add eax,dword ptr ss:[ebp+8];
		mov eax,dword ptr ds:[eax];
		add eax,dword ptr ss:[ebp+8];
		mov dword ptr ss:[ebp-4],eax;
_Error:
		pop dword ptr fs:[0];
		add esp,0x0C;
		popad;
		mov eax,dword ptr ss:[ebp-4];
		leave;
		retn 8;


	}

	//字符串：LoadLibraryA
	__asm{
szLoadLibraryA:
		_emit 0x4C; 
		_emit 0x6F;
		_emit 0x61;
		_emit 0x64;
		_emit 0x4C;
		_emit 0x69;
		_emit 0x62;
		_emit 0x72;
		_emit 0x61;
		_emit 0x72;
		_emit 0x79;
		_emit 0x41;
		_emit 0x00;
	}

}

//下面的代码没有具体含义，仅仅是为了预留一段内存，用来写入将要注入的dll路径名,以及dll的导出函数名。
//其中szDllFile开始的内容为dll路径名；
//szDllFile+MAX_PATH开始的内容为导出函数名
__declspec(naked) void szDllFile(){
	__asm{
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
		mov dword ptr [esp+0x577],0x40000;
	}
}


__declspec(naked) void patchcodeend(){
	
}

//注解：patchcodeend-patchcodestart即为path的代码大小


void CLoaderDlg::OnBnClickedOk()
{
	UpdateData();

	//首先获取目标exe的OEP
	//////////////////////////////////////////////////////////////////////////

	HANDLE hFile=CreateFile(m_strExeFile,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if ( hFile==INVALID_HANDLE_VALUE ){
		AfxMessageBox(_T("打开文件失败"));
		return;
	}

	DWORD size_low,size_high;
	size_low= GetFileSize(hFile,&size_high); 

	HANDLE hMap=CreateFileMapping(hFile,NULL,PAGE_READONLY,size_high,size_low,NULL);
	if ( hMap==NULL ){
		AfxMessageBox(_T("CreateFileMapping失败"));
		CloseHandle(hFile);
		return;
	}

	PBYTE pFile=(PBYTE)MapViewOfFile(hMap,FILE_MAP_READ,0,0,0);
	if ( pFile==NULL ){
		AfxMessageBox(_T("MapViewOfFile失败"));
		CloseHandle(hMap);
		CloseHandle(hFile);
		return;
	}

	PIMAGE_DOS_HEADER pDosHdr=(PIMAGE_DOS_HEADER)pFile;
	PIMAGE_NT_HEADERS pNtHdr=(PIMAGE_NT_HEADERS)((PBYTE)pDosHdr+pDosHdr->e_lfanew);
	DWORD dwEP=pNtHdr->OptionalHeader.AddressOfEntryPoint+pNtHdr->OptionalHeader.ImageBase;

	UnmapViewOfFile(pFile);
	CloseHandle(hMap);
	CloseHandle(hFile);


	//创建共享内存
	//////////////////////////////////////////////////////////////////////////
	m_hMapFile=CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,sizeof(SHAREMEM),MappingFileName);
	if (m_hMapFile==NULL){
		::MessageBox(NULL,_T("不能建立共享内存!"),NULL,MB_OK|MB_ICONERROR);
		return;
	}
	m_pShareMem=(PSHAREMEM)MapViewOfFile(m_hMapFile,FILE_MAP_WRITE|FILE_MAP_READ,0,0,0);
	if (m_pShareMem==NULL){
		CloseHandle(m_hMapFile);
		m_hMapFile=NULL;
		::MessageBox(NULL,_T("不能映射共享内存!"),NULL,MB_OK|MB_ICONERROR);
		return;
	}
	
	//将loader的窗口句柄写到共享内存里
	m_pShareMem->hwndLoader=m_hWnd;
	//////////////////////////////////////////////////////////////////////////


	//将目标dll路径名和dll导出函数写到patch代码里
	//////////////////////////////////////////////////////////////////////////
	strcpy((char*)szDllFile,m_strDllFile);
	strcpy((char*)szDllFile+MAX_PATH,m_strDllFunc);
	//////////////////////////////////////////////////////////////////////////


	//以挂起的方式创建目标进程
	//////////////////////////////////////////////////////////////////////////
	STARTUPINFO si={sizeof(si)};
	PROCESS_INFORMATION pi;
	CreateProcess((LPCTSTR)m_strExeFile,NULL,NULL,NULL,FALSE,CREATE_SUSPENDED,NULL,NULL,&si,&pi);


	//patch入口处代码
	//////////////////////////////////////////////////////////////////////////
	DWORD dwReadWrite=0;
	m_pShareMem->pCodeStart=dwEP;
	m_pShareMem->nCodeLen=(UINT)patchcodeend - (UINT)patchcodestart;


	//修改页保护属性
	if ( !VirtualProtect((LPVOID)dwEP, m_pShareMem->nCodeLen, PAGE_EXECUTE_READWRITE, &m_pShareMem->dwOldProtect) ){
		AfxMessageBox("VirtualProtect失败");
	}

	//保存原入口代码
	ReadProcessMemory(pi.hProcess,(PVOID)dwEP,m_pShareMem->pOrigCode,m_pShareMem->nCodeLen,&dwReadWrite);
	//patch入口代码
	WriteProcessMemory(pi.hProcess,(PVOID)dwEP,patchcodestart,m_pShareMem->nCodeLen,&dwReadWrite);

	//恢复进程
	ResumeThread(pi.hThread);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}

LRESULT CLoaderDlg::OnInjectSuccess(WPARAM wParam,LPARAM lParam)
{
	if ( m_pShareMem!=NULL ){
		UnmapViewOfFile(m_pShareMem);
		m_pShareMem=NULL;
	}
	if ( m_hMapFile!=NULL ){
		CloseHandle(m_hMapFile);
		m_hMapFile=NULL;
	}

	AfxMessageBox("dll注入成功，我也可以退出了");
	OnOK();

	return 0;
}