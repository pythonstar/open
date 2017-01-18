
#include "stdafx.h"
#include "global.h"
#include <MsHTML.h>
#include <afxinet.h>
#include <shlwapi.h>
#include <ObjBase.h>
#include <RpcDce.h>
#include <StarLib/MD5/Md5.h>
#include <StarLib/URLEncoder/URLEncode.H>

#pragma comment(lib,"Rpcrt4.lib")

CString g_strRegUrl;

CString GetStartPath()
{
	CHAR szTemp[MAX_PATH*2];
	GetModuleFileName(NULL,szTemp,sizeof(szTemp));
	strrchr(szTemp,'\\')[1]=0;
	return szTemp;
}


CString GetTempFilePath()
{
	TCHAR szPath[MAX_PATH];  
	GetTempPath(MAX_PATH,szPath);  
	GetTempFileName(szPath,_TEXT("tmp"),0,szPath);  
	return szPath;
}

int GenerateGUID(CString& sGUID)
{
	int status = 1;
	sGUID.Empty();

	// Create GUID

	UCHAR *pszUuid = 0; 
	GUID *pguid = NULL;
	pguid = new GUID;
	if(pguid!=NULL){
		HRESULT hr = CoCreateGuid(pguid);
		if(SUCCEEDED(hr)){
			// Convert the GUID to a string
			hr = UuidToString(pguid, &pszUuid);
			if(SUCCEEDED(hr) && pszUuid!=NULL){ 
				status = 0;
				sGUID = pszUuid;
				RpcStringFree(&pszUuid);
			}
		}
		delete pguid; 
	}

	return status;
}

void ConvertUtf8ToGBK(CString& strUtf8)
{
	int len=MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL,0);
	WCHAR * wszGBK = new WCHAR[len+1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK=new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte (CP_ACP, 0, wszGBK, -1, szGBK, len, NULL,NULL);

	strUtf8 = szGBK;
	delete[] szGBK;
	delete[] wszGBK;

	return;
}

CString BrowseFolder(HWND hWnd,LPCTSTR lpszTitle)  
{   
	char szDir[MAX_PATH];
	BROWSEINFO bi;
	ITEMIDLIST *pidl;
	bi.hwndOwner=hWnd;
	bi.pidlRoot=NULL;
	bi.pszDisplayName=szDir;
	bi.lpszTitle=lpszTitle;
	bi.ulFlags=BIF_RETURNONLYFSDIRS;
	bi.lpfn=NULL;
	bi.lParam=0;
	bi.iImage=0;
	pidl=SHBrowseForFolder(&bi);
	if(pidl=NULL){ 
		return _T("");
	}
	if( SHGetPathFromIDList(pidl, szDir)==FALSE ){
		return _T("");
	}

	return CString(szDir);  
}

int GetDirverList(CStringList&driverList)
{
	int nCount=0;
	TCHAR szDrivers[MAX_PATH];
	TCHAR*pszOneDriver=szDrivers;
	CString strSystemPath;

	driverList.RemoveAll();
	GetLogicalDriveStrings(MAX_PATH,szDrivers);
	while (*pszOneDriver){
		UINT nDriverType=GetDriveType(pszOneDriver);
		if ( nDriverType==DRIVE_FIXED || nDriverType==DRIVE_REMOVABLE ){
			if ( _tcsicmp(_T("A:\\"),pszOneDriver)==0 ){
				continue;
			}else{
				if ( strSystemPath.Find(pszOneDriver[0])==0 ){
					driverList.AddHead(pszOneDriver);
				}else{
					driverList.AddTail(pszOneDriver);
				}

				++nCount;
			}

		}

		pszOneDriver+=lstrlen(pszOneDriver)+1;
	}//end while

	return nCount;
}

BOOL SignalToStop(CEvent *pEvent)
{
	if ( pEvent==NULL ){
		return FALSE;
	}

	return ( ::WaitForSingleObject(pEvent->m_hObject,0)!=WAIT_OBJECT_0 );
}