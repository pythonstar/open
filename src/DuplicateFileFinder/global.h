
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
	CString strName;		//�ó��������
	CString strVersion;		//���°汾��Ϣ
	CString strDownUrl;		//���ص�ַ
	BOOL bCompressed;		//�Ƿ���ѹ����
	CString strRegUrl;		//������ַ
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
