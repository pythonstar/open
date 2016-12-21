// EnumFormVal.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "EnumFormVal.h"

#include <atlbase.h>

CComModule _Module;		// ����Ҫʹ�� CComDispatchDriver ATL������ָ�룬
						// �����������Ǳ����

#include <mshtml.h>		// ���� IHTMLxxxx �Ľӿ�����
#include <atlcom.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

using namespace std;

void EnumIE( void );								//ö�����������
void EnumFrame( IHTMLDocument2 * pIHTMLDocument2 );	//ö���ӿ�ܺ���
void EnumForm ( IHTMLDocument2 * pIHTMLDocument2 );	//ö�ٱ�����

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	::CoInitialize(NULL);	//��ʼ�� COM ��Ԣ

	EnumIE();				//ö�������

	::CoUninitialize();		//�ͷ� COM ��Ԣ

	cout << _T("======���======") << endl;
	getchar();				//�ȴ��س�

	return 0;
}

void EnumIE( void )
{
	cout << _T("��ʼɨ��ϵͳ���������е������ʵ��") << endl;

	CComPtr< IShellWindows > spShellWin;
	HRESULT hr = spShellWin.CoCreateInstance( CLSID_ShellWindows );
	if ( FAILED ( hr ) )
	{
		cout << _T("��ȡ IShellWindows �ӿڴ���") << endl;
		return;
	}

	long nCount = 0;		// ȡ�������ʵ������(Explorer �� IExplorer)
	spShellWin->get_Count( &nCount );
	if( 0 == nCount )
	{
		cout << _T("û���������ŵ������") << endl;
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

		// �������е��ˣ��Ѿ��ҵ��� IHTMLDocument2 �Ľӿ�ָ��

		// ɾ����������ע�ͣ���������ı����ı俴��
		// spDocument2->put_bgColor( CComVariant( "green" ) );
		
		EnumForm( spDocument2 );		//ö�����еı�
	}
}

void EnumFrame( IHTMLDocument2 * pIHTMLDocument2 )
{
	if ( !pIHTMLDocument2 )	return;

	HRESULT hr;

	CComPtr< IHTMLFramesCollection2 > spFramesCollection2;
	pIHTMLDocument2->get_frames( &spFramesCollection2 );	//ȡ�ÿ��frame�ļ���

	long nFrameCount=0;				//ȡ���ӿ�ܸ���
	hr = spFramesCollection2->get_length( &nFrameCount );
	if ( FAILED ( hr ) || 0 == nFrameCount )	return;

	for(long i=0; i<nFrameCount; i++)
	{
		CComVariant vDispWin2;		//ȡ���ӿ�ܵ��Զ����ӿ�
		hr = spFramesCollection2->item( &CComVariant(i), &vDispWin2 );
		if ( FAILED ( hr ) )	continue;

		CComQIPtr< IHTMLWindow2 > spWin2 = vDispWin2.pdispVal;
		if( !spWin2 )	continue;	//ȡ���ӿ�ܵ� IHTMLWindow2 �ӿ�

		CComPtr < IHTMLDocument2 > spDoc2;
		spWin2->get_document( &spDoc2 );	//ȡ���ֿ�ܵ� IHTMLDocument2 �ӿ�

		EnumForm( spDoc2 );			//�ݹ�ö�ٵ�ǰ�ӿ�� IHTMLDocument2 �ϵı�form
	}
}

void EnumForm( IHTMLDocument2 * pIHTMLDocument2 )
{
	if( !pIHTMLDocument2 )	return;

	EnumFrame( pIHTMLDocument2 );	//�ݹ�ö�ٵ�ǰ IHTMLDocument2 �ϵ��ӿ��fram

	HRESULT hr;
	CComBSTR bstrTitle;
	pIHTMLDocument2->get_title( &bstrTitle );	//ȡ���ĵ�����

	USES_CONVERSION;
	cout << _T("====================") << endl;
	cout << _T("��ʼö�١�") << OLE2CT( bstrTitle ) << _T("���ı�") << endl;
	cout << _T("====================") << endl;

	CComQIPtr< IHTMLElementCollection > spElementCollection;
	hr = pIHTMLDocument2->get_forms( &spElementCollection );	//ȡ�ñ�����
	if ( FAILED( hr ) )
	{
		cout << _T("��ȡ���ļ��� IHTMLElementCollection ����") << endl;
		return;
	}


	long nFormCount=0;				//ȡ�ñ���Ŀ
	hr = spElementCollection->get_length( &nFormCount );
	if ( FAILED( hr ) )
	{
		cout << _T("��ȡ����Ŀ����") << endl;
		return;
	}
	
	for(long i=0; i<nFormCount; i++)
	{
		IDispatch *pDisp = NULL;	//ȡ�õ� i ���
		hr = spElementCollection->item( CComVariant( i ), CComVariant(), &pDisp );
		if ( FAILED( hr ) )		continue;

		CComQIPtr< IHTMLFormElement > spFormElement = pDisp;
		pDisp->Release();

		long nElemCount=0;			//ȡ�ñ��� �� ����Ŀ
		hr = spFormElement->get_length( &nElemCount );
		if ( FAILED( hr ) )		continue;

		for(long j=0; j<nElemCount; j++)
		{
			CComDispatchDriver spInputElement;	//ȡ�õ� j �����
			hr = spFormElement->item( CComVariant( j ), CComVariant(), &spInputElement );
			if ( FAILED( hr ) )	continue;

			CComVariant vName,vVal,vType;		//ȡ�ñ���� ����ֵ������
			hr = spInputElement.GetPropertyByName( L"name", &vName );
			if( FAILED( hr ) )	continue;
			hr = spInputElement.GetPropertyByName( L"value", &vVal );
			if( FAILED( hr ) )	continue;
			hr = spInputElement.GetPropertyByName( L"type", &vType );
			if( FAILED( hr ) )	continue;

			LPCTSTR lpName = vName.bstrVal?
					OLE2CT( vName.bstrVal ) : _T("NULL");	//δ֪����
			LPCTSTR lpVal  = vVal.bstrVal?
					OLE2CT( vVal.bstrVal  ) : _T("NULL");	//��ֵ��δ����
			LPCTSTR lpType = vType.bstrVal?
					OLE2CT( vType.bstrVal ) : _T("NULL");	//δ֪����

			if ( _tcsicmp( lpVal,_T("�� ¼") )==0 ){
				//spInputElement.

			}
			cout << _T("[") << lpType << _T("] ");
			cout << lpName << _T(" = ") << lpVal << endl;
		}
		//���ύ�������ɾ����������ע�Ͱ�
		spFormElement->submit();
	}
}

 