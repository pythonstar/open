// usedm.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <string>
using namespace std;


#import "dm.dll" rename("SetWindowText","DmSetWindowText") rename("FindWindow","DmFindWindow") rename("FindWindowEx","DmFindWindowEx")
//�����3��rename��dll����������������.Net2010���滹�Լ�����ЩAPI������ĳЩͷ�ļ������ˣ����Ը��������ơ�
using namespace Dm;

void ShowDmVer()
{
	CoInitialize(NULL);

	CLSID clsid;
	HRESULT hr=CLSIDFromProgID(OLESTR("dm.dmsoft"),&clsid);	//�����"dm.dmsoft"��dll��ע�����ļ�ֵ��
	Idmsoft* dme; //Idmsoft��dm.dll�Ľӿڷ�����dme�����������㻻��
	hr=CoCreateInstance(clsid,NULL,CLSCTX_INPROC_SERVER,__uuidof(Idmsoft),(LPVOID*)&dme);  //����Ķ�����dme��Ӧ��һ�еġ�
	_bstr_t dvr=dme->Ver();	//���÷�ʽ��������->������();
	wstring cdvr(dvr);//��_bstr_t��ת����string�ͣ�Ҫע����������ô��ֻ��Ϊ����������ķ���ֵ��ʵ������ʹ������_bstr_tҲ�ǿ��Եġ�
	MessageBox(0,cdvr.c_str(),_T("��ע��Ĵ�Į����汾Ϊ��"),0);

	CoUninitialize();
}


#ifdef _WINDOWS_

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) 
{ 
	ShowDmVer();
} 

#else

int _tmain(int argc, _TCHAR* argv[])
{
	ShowDmVer();
}

#endif



