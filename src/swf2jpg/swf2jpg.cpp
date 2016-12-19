// swt2jpg.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <afx.h>
#include <Shlwapi.h>
#include <zlib/zlib.h>
#include "PDFLib/pdflib.h"
#include "../swfrip/swf.h"
#import "SWFToImage.dll" 
using namespace SWFToImage;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#ifdef _DEBUG
#pragma comment(lib,"zlibd.lib")
#pragma comment(lib,"../bin/swfripD.lib")
#pragma comment(lib,"libpngd.lib")
#else
#pragma comment(lib,"zlib.lib")
#pragma comment(lib,"../bin/swfrip.lib")
#pragma comment(lib,"libpng.lib")
#endif

//http://download.csdn.net/source/1500314
#pragma comment(lib,"PDFLib/pdflib.lib")


CString GetStartPath()
{
	TCHAR szTemp[MAX_PATH*2];
	GetModuleFileName(NULL,szTemp,sizeof(szTemp)/sizeof(TCHAR));
	StrRChr(szTemp,NULL,'\\')[1]=0;
	return szTemp;
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

void UnCompress(LPCTSTR lpszInFile,LPCTSTR lpszOutFile)
{
	CFile srcFile;

	srcFile.Open(lpszInFile,CFile::modeRead);
	DWORD dwSrcSize = (DWORD)srcFile.GetLength();
	DWORD dwDstSize = (dwSrcSize-8)*2+12;	//(dwSrcSize-8)*1.01+12; 不够大

	BYTE *pSrcBuff = new BYTE[dwSrcSize];
	BYTE *pDstBuff = new BYTE[dwDstSize];

	if ( pSrcBuff && pDstBuff ){
		BYTE *pHeader = pSrcBuff;
		memset(pSrcBuff,0,dwSrcSize-8);
		memset(pDstBuff,0,dwDstSize);

		srcFile.Read(pSrcBuff,dwSrcSize);
		srcFile.Close();

		unsigned long destlen = dwDstSize;
		uncompress( pDstBuff, &destlen, pSrcBuff+8, dwSrcSize-8 );
		pHeader[0]='F';		//change the header indentifier.

		CFile filedes;
		filedes.Open(lpszOutFile,CFile::modeCreate|CFile::modeWrite);
		filedes.Write(pHeader,8);
		filedes.Write(pDstBuff,destlen);
		filedes.Close();
	}
	
	if ( pSrcBuff ){
		delete []pSrcBuff;
	}

	if ( pDstBuff ){
		delete []pDstBuff;
	}
}

CSWF cswfLoaded;

int _tmain(int argc, _TCHAR* argv[])
{
	CString strAppPath = GetStartPath();

	//////////////////////////////////////////////////////////////////////////
	//UnCompress("0.swf","1.swf");
	//////////////////////////////////////////////////////////////////////////

	CoInitialize(NULL);

	CLSID clsid;
	HRESULT hr=CLSIDFromProgID(OLESTR("SWFToImage.SWFToImageObject"),&clsid);	//这里的"SWFToImage.SWFToImageObject"是dll在注册表里的键值。
	
	ISWFToImageObject *obj;
	hr=CoCreateInstance(clsid,NULL,CLSCTX_INPROC_SERVER,__uuidof(ISWFToImageObject),(LPVOID*)&obj);

	obj->InitLibrary("demo", "demo");
	obj->InputSWFFileName = "1.swf";
	obj->ImageOutputType = SWFToImage::TImageOutputType::iotJPG; // ' set output image type to Jpeg (0 = BMP, 1 = JPG, 2 = GIF)
	obj->ImageWidth = 1222;
	obj->ImageHeight = 1680;
	obj->Execute();
	obj->SaveToFile("1.jpg");


#if 1
	PDF *pdf = PDF_new();
	if ( pdf ){
		PDF_open_file(pdf,"1.pdf");
		for ( int i=0; i<3; ++i ){
			int nImage = PDF_open_image_file(pdf,"jpeg",strAppPath+"1.jpg","",0);
			PDF_begin_page(pdf,1222,1680);
			PDF_place_image(pdf, nImage, 0, 0, 1); 
			PDF_close_image(pdf, nImage);
			PDF_end_page(pdf);
		}

		PDF_close(pdf);
	}

#endif


#if 0

	cswfLoaded.SetDeleteTags(true);
	if( !cswfLoaded.Load(strAppPath+"0.swf") ){
		printf("not a valid Macromedia Flash file.");
	}else{
	}

	if( cswfLoaded.Loaded() ){
		cswfLoaded.SaveText((char *)(LPCTSTR)(strAppPath+"0.txt"));
	}

	cswfLoaded.Clear();

#endif

	CoUninitialize();

	return 0;
}

