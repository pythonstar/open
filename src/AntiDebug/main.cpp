// main.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "AntiDebug.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CAntiDebug anti;
	//anti.Start();
	//anti.Wait();

	//anti.SetUnhandledExceptionFilterMethod();
	//anti.CheckDebugObject();
	//anti.CR4TSD();
	//anti.InstallAntiAttach();
	//anti.InstallAntiAttach2();
	anti.InstallAntiAttach3();
	while ( true ){
		printf("test main!\n");
		Sleep(10000);
	}

	return 0;
}

