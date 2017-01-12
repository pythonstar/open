// Loader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <afx.h>
#include <iostream>
#include <tchar.h>
#include <afxwin.h>
#include <Psapi.h>
#include <shlwapi.h>

//#include "ntimage.h"
#include "Common.h"

using namespace std;


#pragma comment(lib,"Psapi.lib")
#pragma comment(lib,"shlwapi.lib")


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString GetStartPath()
{
	TCHAR szTemp[MAX_PATH*2];
	GetModuleFileName(NULL,szTemp,sizeof(szTemp)/sizeof(TCHAR));
	StrRChr(szTemp,NULL,'\\')[1]=0;
	return szTemp;
}

int main(int argc, char* argv[])
{
	CString strStartPath=GetStartPath();
	//CString strTargetFile=_T("C:\\windows\\notepad.exe");
	CString strTargetFile=strStartPath+_T("notepad.exe");

	//��test.exe�ļ�������ȡ�ļ�����
	TCHAR szDllFile[MAX_PATH] = {0};
	GetModuleFileName(NULL,szDllFile,_countof(szDllFile));
#ifdef _DEBUG
	lstrcpy(&StrRChr(szDllFile,NULL,'\\')[1],_T("dllexe.dll"));
#else
	lstrcpy(&StrRChr(szDllFile,NULL,'\\')[1],_T("dllexe.dll"));
#endif


	//���Ȼ�ȡĿ��exe��OEP
	//////////////////////////////////////////////////////////////////////////

	HANDLE hFile=CreateFile(strTargetFile,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if ( hFile==INVALID_HANDLE_VALUE ){
		AfxMessageBox(_T("���ļ�ʧ��"));
		return -1;
	}

	DWORD size_low,size_high;
	size_low= GetFileSize(hFile,&size_high); 

	HANDLE hMap=CreateFileMapping(hFile,NULL,PAGE_READONLY,size_high,size_low,NULL);
	if ( hMap==NULL ){
		AfxMessageBox(_T("CreateFileMappingʧ��"));
		CloseHandle(hFile);
		return -1;
	}

	PBYTE pFile=(PBYTE)MapViewOfFile(hMap,FILE_MAP_READ,0,0,0);
	if ( pFile==NULL ){
		AfxMessageBox(_T("MapViewOfFileʧ��"));
		CloseHandle(hMap);
		CloseHandle(hFile);
		return -1;
	}

	PIMAGE_DOS_HEADER pDosHdr=(PIMAGE_DOS_HEADER)pFile;
	PIMAGE_NT_HEADERS pNtHdr=(PIMAGE_NT_HEADERS)((PBYTE)pDosHdr+pDosHdr->e_lfanew);
	DWORD dwImageSize=pNtHdr->OptionalHeader.SizeOfImage;
	DWORD dwImageBase=pNtHdr->OptionalHeader.ImageBase;
	DWORD dwEP=pNtHdr->OptionalHeader.AddressOfEntryPoint+dwImageBase;

	UnmapViewOfFile(pFile);
	CloseHandle(hMap);
	CloseHandle(hFile);

	//�Ե��Եķ�ʽ����notepad����
	//////////////////////////////////////////////////////////////////////////
	STARTUPINFO si={sizeof(si)};
	PROCESS_INFORMATION pi;
	//ʹ��DETACHED_PROCESS��־��Ϊ�˲����ӽ���ӵ�и����̵Ŀ���̨����
	CreateProcess(strTargetFile,NULL,NULL,NULL,FALSE,DEBUG_PROCESS|DEBUG_ONLY_THIS_PROCESS|DETACHED_PROCESS,NULL,NULL,&si,&pi);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	CString strText;
	BOOL bPatched=FALSE;
	DEBUG_EVENT stDbgevent;
	CONTEXT stThreadContext;
	DWORD dwProcessId;
	LPVOID lpImageBase;
	HANDLE hProcess;
	HANDLE hThread;
	LPVOID lpEntryPoint;
	DWORD dwReadWrite;
	while ( TRUE ){
		if ( WaitForDebugEvent(&stDbgevent,INFINITE)==0 ){
			//failed
			break;
		}

		if ( stDbgevent.dwDebugEventCode==CREATE_PROCESS_DEBUG_EVENT ){			//�����Խ��̸ձ�����

			//��ȡһЩ������Ϣ
			dwProcessId=stDbgevent.dwProcessId;
			lpImageBase=stDbgevent.u.CreateProcessInfo.lpBaseOfImage;
			lpEntryPoint=stDbgevent.u.CreateProcessInfo.lpStartAddress;
			hProcess=stDbgevent.u.CreateProcessInfo.hProcess;
			hThread=stDbgevent.u.CreateProcessInfo.hThread;

			//����ڴ�д��int3�ϵ�
			BYTE bInt3='\xCC';
			WriteProcessMemory(hProcess,lpEntryPoint,&bInt3,sizeof(BYTE),&dwReadWrite);
			FlushInstructionCache(hProcess,lpEntryPoint,1);	//������д���ڴ���cpuִ��
			ContinueDebugEvent(stDbgevent.dwProcessId, stDbgevent.dwThreadId, DBG_EXCEPTION_NOT_HANDLED ); 

		}else if ( stDbgevent.dwDebugEventCode == EXIT_PROCESS_DEBUG_EVENT ){	//���Խ���
			break;
		}else if ( stDbgevent.dwDebugEventCode == EXCEPTION_DEBUG_EVENT ){
			if ( stDbgevent.u.Exception.ExceptionRecord.ExceptionCode==EXCEPTION_BREAKPOINT 
				&& stDbgevent.u.Exception.ExceptionRecord.ExceptionAddress==lpEntryPoint ){
				//���쳣��������ڴ�ʱ���������µ�int3�ϵ㣬����patch������

				stThreadContext.ContextFlags=CONTEXT_CONTROL;
				GetThreadContext(hThread,&stThreadContext);

				//patch���룬�����µ�EIP
				stThreadContext.Eip=PatchCode(hProcess,lpImageBase,dwImageSize,szDllFile);
				SetThreadContext(hThread,&stThreadContext);

				//���ǵ�Ŀ�����ˣ�detach��������������
				ContinueDebugEvent(stDbgevent.dwProcessId, stDbgevent.dwThreadId, DBG_CONTINUE); 
				DebugActiveProcessStop(stDbgevent.dwProcessId);
				break;
			}else{
				//��DBG_CONTINUE��־�������е���
				ContinueDebugEvent(stDbgevent.dwProcessId, stDbgevent.dwThreadId, DBG_CONTINUE); 
			}

		}else{	//�������������
			ContinueDebugEvent(stDbgevent.dwProcessId, stDbgevent.dwThreadId, DBG_EXCEPTION_NOT_HANDLED); 
		}

	}

	//system("pause");
	return 0;
}

DWORD PatchCode(HANDLE hTargetProcess,LPVOID lpImageBase,DWORD dwImageSize,LPCTSTR lpszFilePath)
{
	DWORD dwNewEip=0;

	//�޸�ҳ��������
	DWORD dwOldProtect=0;
	if ( !VirtualProtectEx(hTargetProcess,lpImageBase, dwImageSize, PAGE_EXECUTE_READWRITE, &dwOldProtect) ){
		AfxMessageBox(_T("VirtualProtectʧ��"));
		return dwNewEip;
	}
	
	HANDLE hFile = CreateFile(lpszFilePath,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	DWORD dwErr = GetLastError();
	if( hFile == INVALID_HANDLE_VALUE ){
		AfxMessageBox(_T("���ļ�ʧ��"));
		return dwNewEip;
	}

	DWORD dwFileSize = GetFileSize( hFile, NULL );
	if( dwFileSize == INVALID_FILE_SIZE ){
		AfxMessageBox(_T("��ȡ�ļ���Сʧ��"));
		CloseHandle(hFile);
		return dwNewEip;
	}

	//�����㹻��Ŀռ䣬���ļ�����
	PVOID pBuffer = (PVOID)new BYTE[dwFileSize];
	if( pBuffer == NULL ){
		AfxMessageBox(_T("�������ļ����ݿռ�ʧ��"));
		CloseHandle( hFile );
		return dwNewEip;
	}

	DWORD dwReadWrite = 0;
	if( ReadFile(hFile,pBuffer,dwFileSize,&dwReadWrite,NULL)==FALSE ){
		AfxMessageBox(_T("��ȡ�ļ�����ʧ��"));
		delete []pBuffer;
		CloseHandle( hFile );
		return dwNewEip;
	}

	//�����ļ��ڴ�����Ĵ�С
	DWORD dwSrcImgSize = GetTotalImageSize(pBuffer,dwFileSize);
	if ( dwSrcImgSize==0 ){
		AfxMessageBox(_T("GetTotalImageSizeʧ��"));
		delete []pBuffer;
		CloseHandle( hFile );
		return dwNewEip;
	}

	//����Ŀ���ַ
	PVOID lpNewImgBase=NULL;
	if ( dwSrcImgSize<=dwImageSize ){	//˵������ֱ��patch����
		lpNewImgBase=lpImageBase;
	}else{	//�ռ䲻�������ڴ�
		lpNewImgBase=VirtualAllocEx(hTargetProcess,NULL,dwSrcImgSize,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
		if ( lpNewImgBase==NULL ){
			AfxMessageBox(_T("VirtualAllocExʧ��"));
			delete []pBuffer;
			CloseHandle( hFile );
			return dwNewEip;
		}
	}

	//��ӳ�䵽��ǰ�����У����ֱ��д��Ŀ�����
	PVOID pImageBuffer=VirtualAllocEx(::GetCurrentProcess(),NULL,dwSrcImgSize,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	if ( lpNewImgBase==NULL ){
		AfxMessageBox(_T("VirtualAllocExʧ��"));
		delete []pBuffer;
		CloseHandle( hFile );
		return dwNewEip;
	}

	//���Ƚ��ڰ��ڴ����ֵ����
	AlignFileToMem(pImageBuffer,dwSrcImgSize,pBuffer,dwFileSize);
	if( pImageBuffer == NULL ){
		AfxMessageBox(_T("�����ļ�����ʧ��"));
		delete []pBuffer;
		CloseHandle( hFile );
		return dwNewEip;
	}

	//�ͷ��ڴ棬�ر��ļ�
	delete []pBuffer;
	CloseHandle( hFile );

	//�����ļ��ɹ������ȴ����������Ϣ
	if( !ProcessImportTable(pImageBuffer/*,lpNewImgBase*/) ){
		AfxMessageBox(_T("���������ʧ��"));
		//VirtualFreeEx(getcurpImageBuffer
		return dwNewEip;
	}

	//���������ض�λ��
	if( !ProcessRelocTable(pImageBuffer,lpNewImgBase) ){
		AfxMessageBox(_T("�����ض�λ��ʧ��"));
		//VirtualFreeEx pImageBuffer
		return dwNewEip;
	}

	//patch����
	WriteProcessMemory(hTargetProcess,lpNewImgBase,pImageBuffer,dwSrcImgSize,&dwReadWrite);
	
	//VirtualFreeEx pImageBuffer

	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pImageBuffer;
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)( (ULONG)pDosHeader + pDosHeader->e_lfanew );
	dwNewEip=pNtHeaders->OptionalHeader.AddressOfEntryPoint+(DWORD)lpNewImgBase;
	return dwNewEip;
}
