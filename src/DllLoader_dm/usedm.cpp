// usedm.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
using namespace std;


#import "dm.dll" rename("SetWindowText","DmSetWindowText") rename("FindWindow","DmFindWindow") rename("FindWindowEx","DmFindWindowEx")
//后面的3个rename是dll函数重命名。我在.Net2010下面还自己整了些API，估计某些头文件重名了，所以改了下名称。
using namespace Dm;

void ShowDmVer()
{
	CoInitialize(NULL);

	CLSID clsid;
	HRESULT hr=CLSIDFromProgID(OLESTR("dm.dmsoft"),&clsid);	//这里的"dm.dmsoft"是dll在注册表里的键值。
	Idmsoft* dme; //Idmsoft是dm.dll的接口方法，dme这个对象名随便换。
	hr=CoCreateInstance(clsid,NULL,CLSCTX_INPROC_SERVER,__uuidof(Idmsoft),(LPVOID*)&dme);  //这里的对象名dme对应上一行的。
	_bstr_t dvr=dme->Ver();	//调用方式：对象名->函数名();
	wstring cdvr(dvr);//把_bstr_t型转换成string型，要注意我这里这么做只是为了输出函数的返回值，实际上在使用中用_bstr_t也是可以的。
	MessageBox(0,cdvr.c_str(),_T("已注册的大漠插件版本为："),0);

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



