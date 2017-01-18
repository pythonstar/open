
#pragma once
#include <afx.h>
#include <vector>
using namespace std;
#include <afxmt.h>
#include <tinyxml/tinyxml.h>

typedef struct ALBUMINFO
{
	CString strAlbumUrl;
	CString strAlbumId;
	CString strName;
	CString strPreviewUrl;
	CString	strTotal;
	int		nTotal;
}*PALBUMINFO;

typedef struct PHOTOINFO
{
	CString strName;
	CString strPreviewUrl;
	CString strOriginUrl;
	CString strID;
}*PPHOTOINFO;

typedef struct UPDATEINFO
{
	CString strName;		//该程序的名称
	CString strVersion;		//最新版本信息
	CString strDownUrl;		//下载地址
	BOOL bCompressed;		//是否是压缩的
	CString strRegUrl;		//购买网址
}*PUPDATEINFO;

extern CString g_strRegUrl;

CString GetStartPath();
UINT GetHttpFileSaveAs(LPCTSTR lpszUrl,LPCTSTR lpszSaveAs);
UINT GetHttpFileContent(LPCTSTR lpszUrl,CString&strHtml);
int DeleteDirectory(CString DirName,BOOL bDeleteSelf=false);
void InitializeSeed();
int Uniform(int Low,int High);
BOOL CheckUpdateInfo(CString strUrl,UPDATEINFO&stUpdateInfo);
CString GetTempFilePath();

int GenerateGUID(CString& sGUID);
void ConvertUtf8ToGBK(CString& strUtf8);
CString BrowseFolder(HWND hWnd,LPCTSTR lpszTitle); 

int GetDirverList(CStringList&driverList);
BOOL SignalToStop(CEvent *pEvent);
