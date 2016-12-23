// LoaderDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include <Psapi.h>
#include "Loader.h"
#include "LoaderDlg.h"
#include "../../../lib/StarLib/Common/common.h"


#pragma comment(lib,"Psapi.lib")
#pragma comment(linker, "/SECTION:.text,REW" ) //��PE�ڣ�.text,�ɶ���ִ��

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLoaderDlg �Ի���




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


// CLoaderDlg ��Ϣ�������

BOOL CLoaderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_strExeFile=Star::Common::GetStartPath()+_T("target.exe");
	m_strDllFile=Star::Common::GetStartPath()+_T("inject.dll");
	m_strDllFunc=_T("start");
	UpdateData(FALSE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}



void szDllFile();

__declspec(naked) DWORD patchcodestart(){

	__asm {
		//_emit 0xCC; 
	}

	__asm{
		//��ȡkernel32.dll�Ļ�ַ
		push dword ptr[esp];
		call _GetKernelBase;

		//�ض�λ
		call _label0;
_label0:
		pop ebx;
		sub ebx,_label0;

		//��ȡLoadLibraryA�����ĵ�ַ
		lea esi,dword ptr [ebx+szLoadLibraryA];
		push esi;
		push eax;
		call _GetApi;

		//LoadLibraryA(Ŀ��dll)
		lea esi,dword ptr [ebx+szDllFile];
		push esi;
		call eax;

		//��ȡ������Ŀ��dll�ĵ�����������������������ƺ���
		lea esi,dword ptr [ebx+szDllFile];
		add esi,MAX_PATH;	//szDllFile+MAX_PATH��ʼΪĿ��dll�����ĺ�����
		push esi;
		push eax;
		call _GetApi;

		lea esi,dword ptr [ebx+patchcodestart];
		push esi			//���÷��ص�ַΪOEP
		jmp eax;			//ע��������jmp����󵼳�������jmp��OEP��
	}

	//����seh
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

	//��̬��ȡkernel32.dll��ַ�ĺ�������Ҫһ������Ϊ��[esp]
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

	//��̬��ȡdll�е�ĳ����������������1Ϊdll��ַ������2Ϊ������
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

	//�ַ�����LoadLibraryA
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

//����Ĵ���û�о��庬�壬������Ϊ��Ԥ��һ���ڴ棬����д�뽫Ҫע���dll·����,�Լ�dll�ĵ�����������
//����szDllFile��ʼ������Ϊdll·������
//szDllFile+MAX_PATH��ʼ������Ϊ����������
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

//ע�⣺patchcodeend-patchcodestart��Ϊpath�Ĵ����С


void CLoaderDlg::OnBnClickedOk()
{
	UpdateData();

	//���Ȼ�ȡĿ��exe��OEP
	//////////////////////////////////////////////////////////////////////////

	HANDLE hFile=CreateFile(m_strExeFile,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if ( hFile==INVALID_HANDLE_VALUE ){
		AfxMessageBox(_T("���ļ�ʧ��"));
		return;
	}

	DWORD size_low,size_high;
	size_low= GetFileSize(hFile,&size_high); 

	HANDLE hMap=CreateFileMapping(hFile,NULL,PAGE_READONLY,size_high,size_low,NULL);
	if ( hMap==NULL ){
		AfxMessageBox(_T("CreateFileMappingʧ��"));
		CloseHandle(hFile);
		return;
	}

	PBYTE pFile=(PBYTE)MapViewOfFile(hMap,FILE_MAP_READ,0,0,0);
	if ( pFile==NULL ){
		AfxMessageBox(_T("MapViewOfFileʧ��"));
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


	//���������ڴ�
	//////////////////////////////////////////////////////////////////////////
	m_hMapFile=CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,sizeof(SHAREMEM),MappingFileName);
	if (m_hMapFile==NULL){
		::MessageBox(NULL,_T("���ܽ��������ڴ�!"),NULL,MB_OK|MB_ICONERROR);
		return;
	}
	m_pShareMem=(PSHAREMEM)MapViewOfFile(m_hMapFile,FILE_MAP_WRITE|FILE_MAP_READ,0,0,0);
	if (m_pShareMem==NULL){
		CloseHandle(m_hMapFile);
		m_hMapFile=NULL;
		::MessageBox(NULL,_T("����ӳ�乲���ڴ�!"),NULL,MB_OK|MB_ICONERROR);
		return;
	}
	
	//��loader�Ĵ��ھ��д�������ڴ���
	m_pShareMem->hwndLoader=m_hWnd;
	//////////////////////////////////////////////////////////////////////////


	//��Ŀ��dll·������dll��������д��patch������
	//////////////////////////////////////////////////////////////////////////
	strcpy((char*)szDllFile,m_strDllFile);
	strcpy((char*)szDllFile+MAX_PATH,m_strDllFunc);
	//////////////////////////////////////////////////////////////////////////


	//�Թ���ķ�ʽ����Ŀ�����
	//////////////////////////////////////////////////////////////////////////
	STARTUPINFO si={sizeof(si)};
	PROCESS_INFORMATION pi;
	CreateProcess((LPCTSTR)m_strExeFile,NULL,NULL,NULL,FALSE,CREATE_SUSPENDED,NULL,NULL,&si,&pi);


	//patch��ڴ�����
	//////////////////////////////////////////////////////////////////////////
	DWORD dwReadWrite=0;
	m_pShareMem->pCodeStart=dwEP;
	m_pShareMem->nCodeLen=(UINT)patchcodeend - (UINT)patchcodestart;


	//�޸�ҳ��������
	if ( !VirtualProtect((LPVOID)dwEP, m_pShareMem->nCodeLen, PAGE_EXECUTE_READWRITE, &m_pShareMem->dwOldProtect) ){
		AfxMessageBox("VirtualProtectʧ��");
	}

	//����ԭ��ڴ���
	ReadProcessMemory(pi.hProcess,(PVOID)dwEP,m_pShareMem->pOrigCode,m_pShareMem->nCodeLen,&dwReadWrite);
	//patch��ڴ���
	WriteProcessMemory(pi.hProcess,(PVOID)dwEP,patchcodestart,m_pShareMem->nCodeLen,&dwReadWrite);

	//�ָ�����
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

	AfxMessageBox("dllע��ɹ�����Ҳ�����˳���");
	OnOK();

	return 0;
}