// EnumFormVal.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "EnumFormVal.h"

#include <atlbase.h>

CComModule _Module;		// 由于要使用 CComDispatchDriver ATL的智能指针，
						// 所以声明它是必须的

#include <mshtml.h>		// 所有 IHTMLxxxx 的接口声明
#include <atlcom.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

using namespace std;

void EnumIE( void );								//枚举浏览器函数
void EnumFrame( IHTMLDocument2 * pIHTMLDocument2 );	//枚举子框架函数
void EnumForm ( IHTMLDocument2 * pIHTMLDocument2 );	//枚举表单函数

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	::CoInitialize(NULL);	//初始化 COM 公寓

	EnumIE();				//枚举浏览器

	::CoUninitialize();		//释放 COM 公寓

	cout << _T("======完成======") << endl;
	getchar();				//等待回车

	return 0;
}

void EnumIE( void )
{
	cout << _T("开始扫描系统中正在运行的浏览器实例") << endl;

	CComPtr< IShellWindows > spShellWin;
	HRESULT hr = spShellWin.CoCreateInstance( CLSID_ShellWindows );
	if ( FAILED ( hr ) )
	{
		cout << _T("获取 IShellWindows 接口错误") << endl;
		return;
	}

	long nCount = 0;		// 取得浏览器实例个数(Explorer 和 IExplorer)
	spShellWin->get_Count( &nCount );
	if( 0 == nCount )
	{
		cout << _T("没有在运行着的浏览器") << endl;
		return;
	}

	for(int i=0; i<nCount; i++)
	{
		CComPtr< IDispatch > spDispIE;
		hr=spShellWin->Item(CComVariant( (long)i ), &spDispIE );
		if ( FAILED ( hr ) )	continue;

		CComQIPtr< IWebBrowser2 > spBrowser = spDispIE;
		if ( !spBrowser )		continue;

		CComPtr < IDispatch > spDispDoc;
		hr = spBrowser->get_Document( &spDispDoc );
		if ( FAILED ( hr ) )	continue;

		CComQIPtr< IHTMLDocument2 > spDocument2 = spDispDoc;
		if ( !spDocument2 )		continue;

		// 程序运行到此，已经找到了 IHTMLDocument2 的接口指针

		// 删除下行语句的注释，把浏览器的背景改变看看
		// spDocument2->put_bgColor( CComVariant( "green" ) );
		
		EnumForm( spDocument2 );		//枚举所有的表单
	}
}

void EnumFrame( IHTMLDocument2 * pIHTMLDocument2 )
{
	if ( !pIHTMLDocument2 )	return;

	HRESULT hr;

	CComPtr< IHTMLFramesCollection2 > spFramesCollection2;
	pIHTMLDocument2->get_frames( &spFramesCollection2 );	//取得框架frame的集合

	long nFrameCount=0;				//取得子框架个数
	hr = spFramesCollection2->get_length( &nFrameCount );
	if ( FAILED ( hr ) || 0 == nFrameCount )	return;

	for(long i=0; i<nFrameCount; i++)
	{
		CComVariant vDispWin2;		//取得子框架的自动化接口
		hr = spFramesCollection2->item( &CComVariant(i), &vDispWin2 );
		if ( FAILED ( hr ) )	continue;

		CComQIPtr< IHTMLWindow2 > spWin2 = vDispWin2.pdispVal;
		if( !spWin2 )	continue;	//取得子框架的 IHTMLWindow2 接口

		CComPtr < IHTMLDocument2 > spDoc2;
		spWin2->get_document( &spDoc2 );	//取得字框架的 IHTMLDocument2 接口

		EnumForm( spDoc2 );			//递归枚举当前子框架 IHTMLDocument2 上的表单form
	}
}

void EnumForm( IHTMLDocument2 * pIHTMLDocument2 )
{
	if( !pIHTMLDocument2 )	return;

	EnumFrame( pIHTMLDocument2 );	//递归枚举当前 IHTMLDocument2 上的子框架fram

	HRESULT hr;
	CComBSTR bstrTitle;
	pIHTMLDocument2->get_title( &bstrTitle );	//取得文档标题

	USES_CONVERSION;
	cout << _T("====================") << endl;
	cout << _T("开始枚举“") << OLE2CT( bstrTitle ) << _T("”的表单") << endl;
	cout << _T("====================") << endl;

	CComQIPtr< IHTMLElementCollection > spElementCollection;
	hr = pIHTMLDocument2->get_forms( &spElementCollection );	//取得表单集合
	if ( FAILED( hr ) )
	{
		cout << _T("获取表单的集合 IHTMLElementCollection 错误") << endl;
		return;
	}


	long nFormCount=0;				//取得表单数目
	hr = spElementCollection->get_length( &nFormCount );
	if ( FAILED( hr ) )
	{
		cout << _T("获取表单数目错误") << endl;
		return;
	}
	
	for(long i=0; i<nFormCount; i++)
	{
		IDispatch *pDisp = NULL;	//取得第 i 项表单
		hr = spElementCollection->item( CComVariant( i ), CComVariant(), &pDisp );
		if ( FAILED( hr ) )		continue;

		CComQIPtr< IHTMLFormElement > spFormElement = pDisp;
		pDisp->Release();

		long nElemCount=0;			//取得表单中 域 的数目
		hr = spFormElement->get_length( &nElemCount );
		if ( FAILED( hr ) )		continue;

		for(long j=0; j<nElemCount; j++)
		{
			CComDispatchDriver spInputElement;	//取得第 j 项表单域
			hr = spFormElement->item( CComVariant( j ), CComVariant(), &spInputElement );
			if ( FAILED( hr ) )	continue;

			CComVariant vName,vVal,vType;		//取得表单域的 名，值，类型
			hr = spInputElement.GetPropertyByName( L"name", &vName );
			if( FAILED( hr ) )	continue;
			hr = spInputElement.GetPropertyByName( L"value", &vVal );
			if( FAILED( hr ) )	continue;
			hr = spInputElement.GetPropertyByName( L"type", &vType );
			if( FAILED( hr ) )	continue;

			LPCTSTR lpName = vName.bstrVal?
					OLE2CT( vName.bstrVal ) : _T("NULL");	//未知域名
			LPCTSTR lpVal  = vVal.bstrVal?
					OLE2CT( vVal.bstrVal  ) : _T("NULL");	//空值，未输入
			LPCTSTR lpType = vType.bstrVal?
					OLE2CT( vType.bstrVal ) : _T("NULL");	//未知类型

			if ( _tcsicmp( lpVal,_T("登 录") )==0 ){
				//spInputElement.

			}
			cout << _T("[") << lpType << _T("] ");
			cout << lpName << _T(" = ") << lpVal << endl;
		}
		//想提交这个表单吗？删除下面语句的注释吧
		spFormElement->submit();
	}
}

 