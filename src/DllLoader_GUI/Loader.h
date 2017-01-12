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

	//打开test.exe文件，并读取文件内容
	TCHAR szDllFile[MAX_PATH] = {0};
	GetModuleFileName(NULL,szDllFile,_countof(szDllFile));
#ifdef _DEBUG
	lstrcpy(&StrRChr(szDllFile,NULL,'\\')[1],_T("dllexe.dll"));
#else
	lstrcpy(&StrRChr(szDllFile,NULL,'\\')[1],_T("dllexe.dll"));
#endif


	//首先获取目标exe的OEP
	//////////////////////////////////////////////////////////////////////////

	HANDLE hFile=CreateFile(strTargetFile,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if ( hFile==INVALID_HANDLE_VALUE ){
		AfxMessageBox(_T("打开文件失败"));
		return -1;
	}

	DWORD size_low,size_high;
	size_low= GetFileSize(hFile,&size_high); 

	HANDLE hMap=CreateFileMapping(hFile,NULL,PAGE_READONLY,size_high,size_low,NULL);
	if ( hMap==NULL ){
		AfxMessageBox(_T("CreateFileMapping失败"));
		CloseHandle(hFile);
		return -1;
	}

	PBYTE pFile=(PBYTE)MapViewOfFile(hMap,FILE_MAP_READ,0,0,0);
	if ( pFile==NULL ){
		AfxMessageBox(_T("MapViewOfFile失败"));
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

	//以调试的方式创建notepad进程
	//////////////////////////////////////////////////////////////////////////
	STARTUPINFO si={sizeof(si)};
	PROCESS_INFORMATION pi;
	//使用DETACHED_PROCESS标志是为了不让子进程拥有父进程的控制台窗口
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

		if ( stDbgevent.dwDebugEventCode==CREATE_PROCESS_DEBUG_EVENT ){			//被调试进程刚被创建

			//获取一些基本信息
			dwProcessId=stDbgevent.dwProcessId;
			lpImageBase=stDbgevent.u.CreateProcessInfo.lpBaseOfImage;
			lpEntryPoint=stDbgevent.u.CreateProcessInfo.lpStartAddress;
			hProcess=stDbgevent.u.CreateProcessInfo.hProcess;
			hThread=stDbgevent.u.CreateProcessInfo.hThread;

			//在入口处写个int3断点
			BYTE bInt3='\xCC';
			WriteProcessMemory(hProcess,lpEntryPoint,&bInt3,sizeof(BYTE),&dwReadWrite);
			FlushInstructionCache(hProcess,lpEntryPoint,1);	//将代码写入内存让cpu执行
			ContinueDebugEvent(stDbgevent.dwProcessId, stDbgevent.dwThreadId, DBG_EXCEPTION_NOT_HANDLED ); 

		}else if ( stDbgevent.dwDebugEventCode == EXIT_PROCESS_DEBUG_EVENT ){	//调试结束
			break;
		}else if ( stDbgevent.dwDebugEventCode == EXCEPTION_DEBUG_EVENT ){
			if ( stDbgevent.u.Exception.ExceptionRecord.ExceptionCode==EXCEPTION_BREAKPOINT 
				&& stDbgevent.u.Exception.ExceptionRecord.ExceptionAddress==lpEntryPoint ){
				//当异常发生在入口处时，是我们下的int3断点，可以patch代码了

				stThreadContext.ContextFlags=CONTEXT_CONTROL;
				GetThreadContext(hThread,&stThreadContext);

				//patch代码，设置新的EIP
				stThreadContext.Eip=PatchCode(hProcess,lpImageBase,dwImageSize,szDllFile);
				SetThreadContext(hThread,&stThreadContext);

				//我们的目标达成了，detach进程让它跑起来
				ContinueDebugEvent(stDbgevent.dwProcessId, stDbgevent.dwThreadId, DBG_CONTINUE); 
				DebugActiveProcessStop(stDbgevent.dwProcessId);
				break;
			}else{
				//以DBG_CONTINUE标志继续进行调试
				ContinueDebugEvent(stDbgevent.dwProcessId, stDbgevent.dwThreadId, DBG_CONTINUE); 
			}

		}else{	//其他情况不处理
			ContinueDebugEvent(stDbgevent.dwProcessId, stDbgevent.dwThreadId, DBG_EXCEPTION_NOT_HANDLED); 
		}

	}

	//system("pause");
	return 0;
}

DWORD PatchCode(HANDLE hTargetProcess,LPVOID lpImageBase,DWORD dwImageSize,LPCTSTR lpszFilePath)
{
	DWORD dwNewEip=0;

	//修改页保护属性
	DWORD dwOldProtect=0;
	if ( !VirtualProtectEx(hTargetProcess,lpImageBase, dwImageSize, PAGE_EXECUTE_READWRITE, &dwOldProtect) ){
		AfxMessageBox(_T("VirtualProtect失败"));
		return dwNewEip;
	}
	
	HANDLE hFile = CreateFile(lpszFilePath,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	DWORD dwErr = GetLastError();
	if( hFile == INVALID_HANDLE_VALUE ){
		AfxMessageBox(_T("打开文件失败"));
		return dwNewEip;
	}

	DWORD dwFileSize = GetFileSize( hFile, NULL );
	if( dwFileSize == INVALID_FILE_SIZE ){
		AfxMessageBox(_T("获取文件大小失败"));
		CloseHandle(hFile);
		return dwNewEip;
	}

	//分配足够大的空间，将文件读入
	PVOID pBuffer = (PVOID)new BYTE[dwFileSize];
	if( pBuffer == NULL ){
		AfxMessageBox(_T("申请存放文件内容空间失败"));
		CloseHandle( hFile );
		return dwNewEip;
	}

	DWORD dwReadWrite = 0;
	if( ReadFile(hFile,pBuffer,dwFileSize,&dwReadWrite,NULL)==FALSE ){
		AfxMessageBox(_T("读取文件内容失败"));
		delete []pBuffer;
		CloseHandle( hFile );
		return dwNewEip;
	}

	//计算文件内存对齐后的大小
	DWORD dwSrcImgSize = GetTotalImageSize(pBuffer,dwFileSize);
	if ( dwSrcImgSize==0 ){
		AfxMessageBox(_T("GetTotalImageSize失败"));
		delete []pBuffer;
		CloseHandle( hFile );
		return dwNewEip;
	}

	//计算目标基址
	PVOID lpNewImgBase=NULL;
	if ( dwSrcImgSize<=dwImageSize ){	//说明可以直接patch代码
		lpNewImgBase=lpImageBase;
	}else{	//空间不够开辟内存
		lpNewImgBase=VirtualAllocEx(hTargetProcess,NULL,dwSrcImgSize,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
		if ( lpNewImgBase==NULL ){
			AfxMessageBox(_T("VirtualAllocEx失败"));
			delete []pBuffer;
			CloseHandle( hFile );
			return dwNewEip;
		}
	}

	//先映射到当前进程中，最后直接写到目标进程
	PVOID pImageBuffer=VirtualAllocEx(::GetCurrentProcess(),NULL,dwSrcImgSize,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	if ( lpNewImgBase==NULL ){
		AfxMessageBox(_T("VirtualAllocEx失败"));
		delete []pBuffer;
		CloseHandle( hFile );
		return dwNewEip;
	}

	//首先将节按内存对齐值对齐
	AlignFileToMem(pImageBuffer,dwSrcImgSize,pBuffer,dwFileSize);
	if( pImageBuffer == NULL ){
		AfxMessageBox(_T("对齐文件内容失败"));
		delete []pBuffer;
		CloseHandle( hFile );
		return dwNewEip;
	}

	//释放内存，关闭文件
	delete []pBuffer;
	CloseHandle( hFile );

	//读入文件成功后，首先处理输入表信息
	if( !ProcessImportTable(pImageBuffer/*,lpNewImgBase*/) ){
		AfxMessageBox(_T("处理输入表失败"));
		//VirtualFreeEx(getcurpImageBuffer
		return dwNewEip;
	}

	//继续处理重定位表
	if( !ProcessRelocTable(pImageBuffer,lpNewImgBase) ){
		AfxMessageBox(_T("处理重定位表失败"));
		//VirtualFreeEx pImageBuffer
		return dwNewEip;
	}

	//patch代码
	WriteProcessMemory(hTargetProcess,lpNewImgBase,pImageBuffer,dwSrcImgSize,&dwReadWrite);
	
	//VirtualFreeEx pImageBuffer

	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pImageBuffer;
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)( (ULONG)pDosHeader + pDosHeader->e_lfanew );
	dwNewEip=pNtHeaders->OptionalHeader.AddressOfEntryPoint+(DWORD)lpNewImgBase;
	return dwNewEip;
}
