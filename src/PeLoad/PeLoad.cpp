// PeLoad.cpp : Defines the entry point for the console application.
//

/*
ʱ�䣺2011��2��12��
���ߣ�angelkiss
�������ܣ�Reload And Run����Ŀ¼�µ�test.exe����
*/

#include "stdafx.h"

#include <windows.h>
#include <iostream>

//#include "ntimage.h"
#include "PeLoad.h"

using namespace std;


int main(int argc, char* argv[])
{
	MessageBox( NULL, "Angelkiss", NULL, MB_OK );
	
	//��test.exe�ļ�������ȡ�ļ�����
	char pFilePath[256] = {0};
	GetModuleFileName(NULL,pFilePath,_countof(pFilePath));
#ifdef _DEBUG
	strcpy(&strrchr(pFilePath,'\\')[1],"dllD.dll");
#else
	strcpy(&strrchr(pFilePath,'\\')[1],"dll.dll");
#endif

	HANDLE hFile = CreateFile( pFilePath, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	DWORD dwErr = GetLastError();
	if( hFile == INVALID_HANDLE_VALUE )
	{
		cout<<"���ļ�ʧ��"<<endl;
		cin.get();
		return -1;
	}
	
	DWORD dwFileSize = GetFileSize( hFile, NULL );
	if( dwFileSize == INVALID_FILE_SIZE )
	{
		cout<<"��ȡ�ļ���Сʧ��"<<endl;
		CloseHandle( hFile );
		cin.get();
		return -1;
	}
	//�����㹻��Ŀռ䣬���ļ�����
	PVOID pBuffer = malloc( dwFileSize );
	if( pBuffer == NULL )
	{
		cout<<"�������ļ����ݿռ�ʧ��"<<endl;
		CloseHandle( hFile );
		cin.get();
		return -1;
	}
	OVERLAPPED stOverLapped = {0};
	DWORD dwRealSize = 0;
	bool bRes = ReadFile( hFile, pBuffer, dwFileSize, &dwRealSize, &stOverLapped );
	if( !bRes )
	{
		cout<<"��ȡ�ļ�����ʧ��"<<endl;
		CloseHandle( hFile );
		free( pBuffer );
		cin.get();
		return -1;
	}

	//���Ƚ��ڰ��ڴ����ֵ����
	PVOID pImageBuffer = AlignFileToMem( pBuffer, dwFileSize );
	if( pImageBuffer == NULL )
	{
		cout<<"�����ļ�����ʧ��"<<endl;
		CloseHandle( hFile );
		free( pBuffer );
		cin.get();
		return -1;
	}
	free( pBuffer );
	CloseHandle( hFile );

	//�����ļ��ɹ������ȴ����������Ϣ
	bRes = ProcessImportTable( pImageBuffer );
	if( !bRes )
	{
		cout<<"���������ʧ��"<<endl;
		free( pImageBuffer );
		cin.get();
		return -1;

	}

	//���������ض�λ��
	bRes = ProcessRelocTable( pImageBuffer );
	if( !bRes )
	{
		cout<<"�����ض�λ��ʧ��"<<endl;
		free( pImageBuffer );
		cin.get();
		return -1;
	}
	//����Reload�Ѿ���ɣ�������Run�Ĵ��룬������ôRun�͸�����ͨ��

	//����ֻ�Ǵ������̣߳��̺߳���ָ��ӳ����ڵ�ַ
	DWORD dwFuncStart = (DWORD)pImageBuffer + ((PIMAGE_NT_HEADERS)(((PIMAGE_DOS_HEADER)pImageBuffer)->e_lfanew + (DWORD)pImageBuffer))->OptionalHeader.AddressOfEntryPoint;
	//DWORD dwFuncStart = (DWORD)pImageBuffer + 0x1080;
	cout<<hex<<dwFuncStart<<endl;
	HANDLE hThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)dwFuncStart, NULL, 0, NULL );
	if( hThread == NULL )
	{
		cout<<"�����߳�ʧ��"<<endl;
		free( pImageBuffer );
		cin.get();
		return -1;
	}
	//WaitForSingleObject( hThread, INFINITE );

	system("pause");
	return 0;
}

/*
��������������PE�ļ��ṹ�������Ŀ¼��Ϣ����ȡÿ�����뺯���ĵ�ַ
������
	pBaseAddr:δLoad��PE�ļ���ַ
����ֵ�������������Ϣ�ɹ����
*/
bool ProcessImportTable( PVOID pBaseAddr )
{
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pBaseAddr;
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)( (ULONG)pDosHeader + pDosHeader->e_lfanew );

	//��ȡRtlImageDirectoryEntryToData������ַ
	HMODULE hNtDll = LoadLibrary( "ntdll.dll" );
	PRTLIMAGEDIRECTORYENTRYTODATA pRtlImageDirectoryEntryToData = (PRTLIMAGEDIRECTORYENTRYTODATA)GetProcAddress( hNtDll, "RtlImageDirectoryEntryToData" );
	if( pRtlImageDirectoryEntryToData == NULL )
	{
		cout<<"��ȡRtlImageDirectoryEntryToData������ַʧ��"<<endl;
		FreeLibrary( hNtDll );
		return false;
	}
	DWORD dwImportSize = 0;
	PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)pRtlImageDirectoryEntryToData( pBaseAddr, true, IMAGE_DIRECTORY_ENTRY_IMPORT, &dwImportSize );
	if( pImportDescriptor == NULL )
	{
		cout<<"��ȡ�������Ϣ����"<<endl;
		FreeLibrary( hNtDll );
		return false;
	}

	FreeLibrary( hNtDll );


	while( pImportDescriptor->Name != 0 )
	{
		PIMAGE_THUNK_DATA32 pFirstThunkData = (PIMAGE_THUNK_DATA32)( (ULONG)pBaseAddr + pImportDescriptor->FirstThunk);
		PIMAGE_THUNK_DATA32 pOriginalThunkData = (PIMAGE_THUNK_DATA32)( (ULONG)pBaseAddr + pImportDescriptor->OriginalFirstThunk);

		//��ȡdll��
		char *pDllName = (char*)( (ULONG)pBaseAddr + pImportDescriptor->Name );
		cout<<"���������DLL��:"<<pDllName<<endl;

		//ѭ�������dll�е�ÿ���������
		while( (pOriginalThunkData->u1.Ordinal != 0 ) && !( pOriginalThunkData->u1.Ordinal&0x80000000) )
		{
			PIMAGE_IMPORT_BY_NAME pImageImportByName = (PIMAGE_IMPORT_BY_NAME)( (ULONG)pBaseAddr + (ULONG)(pOriginalThunkData->u1.AddressOfData) );
			char *pFuncName = (char*)(&pImageImportByName->Name);
			DWORD dwFuncAddr = GetFuncAddrFromModule( pDllName, pFuncName );
			cout<<"\t\t������:"<<pFuncName<<"������ַ:"<<hex<<dwFuncAddr<<endl;

			if( dwFuncAddr == 0 )
			{
				cout<<"��ȡ���뺯����ַ����"<<endl;
				return false;
			}
			*(PULONG)pFirstThunkData = dwFuncAddr;

			pFirstThunkData++;
			pOriginalThunkData++;
		}
		pImportDescriptor++;
	}

	return true;

}

/*
������������ȡָ��dllģ�鵼��������ַ
������
	pDllName:dllģ����
	pFuncName:��Ҫ��ȡ��ַ�ĺ�����
����ֵ��
	���غ�����ַ
*/
DWORD GetFuncAddrFromModule( char *pDllName, char *pFuncName )
{
	//���Ȼ�ȡģ�����ַ,����pDllName������ϵͳ�Ѿ����ص�ģ�飬���ReloadAndRun��Ŀ���ļ������Լ���dll����ô�ͻ�ʧ��
	

	DWORD dwModuleBase = (DWORD)GetModuleHandle( pDllName );
	if ( dwModuleBase==NULL ){
		dwModuleBase = (DWORD)LoadLibrary( pDllName );
	}

	//���ҵ�ģ���ַ�󣬽����䵼�����ȡָ��������ַ
	//�ٴλ�ȡRtlImageDirectoryEntryToData������ַ
	//��ȡRtlImageDirectoryEntryToData������ַ
	HMODULE hNtDll = LoadLibrary( "ntdll.dll" );
	PRTLIMAGEDIRECTORYENTRYTODATA pRtlImageDirectoryEntryToData = (PRTLIMAGEDIRECTORYENTRYTODATA)GetProcAddress( hNtDll, "RtlImageDirectoryEntryToData" );
	if( pRtlImageDirectoryEntryToData == NULL )
	{
		cout<<"��ȡRtlImageDirectoryEntryToData������ַʧ��"<<endl;
		FreeLibrary( hNtDll );
		return 0;
	}
	DWORD dwExportSize = 0;
	PIMAGE_EXPORT_DIRECTORY pExportDescriptor = (PIMAGE_EXPORT_DIRECTORY)pRtlImageDirectoryEntryToData( (PVOID)dwModuleBase, true, IMAGE_DIRECTORY_ENTRY_EXPORT, &dwExportSize );
	if( pExportDescriptor == NULL )
	{
		cout<<"��ȡ������ṹʧ��"<<endl;
		FreeLibrary( hNtDll );
		return 0;
	}
	FreeLibrary( hNtDll );

	//���ö��ֲ��ҷ����Һ�����ַ
	PULONG pNameTableBase = (PULONG)(dwModuleBase + pExportDescriptor->AddressOfNames);
	PUSHORT pNameOrdinalTableBase = (PUSHORT)(dwModuleBase + pExportDescriptor->AddressOfNameOrdinals);

	DWORD dwLow = 0;
	DWORD dwHigh = pExportDescriptor->NumberOfNames - 1;
	DWORD dwMid = 0;
	while( dwLow <= dwHigh )
	{
		dwMid = (dwLow + dwHigh) >> 1;
		LONG lRes = strcmp( (char*)(dwModuleBase+pNameTableBase[dwMid]), pFuncName );
		if( lRes > 0 )
			dwHigh = dwMid - 1;
		else if(lRes < 0 )
			dwLow = dwMid + 1;
		else
			break;
	}
	if( dwLow > dwHigh )
	{
		cout<<"���Һ���ʧ��"<<endl;
		return 0;
	}
	DWORD dwOridinalName = pNameOrdinalTableBase[dwMid];
	if( dwOridinalName > pExportDescriptor->NumberOfFunctions )
	{
		cout<<"��ȡ�ĺ�����Ŵ���"<<endl;
		return 0;
	}
	PULONG pAddressTableBase = (PULONG)(dwModuleBase + pExportDescriptor->AddressOfFunctions);
	DWORD dwFuncAddr = dwModuleBase + pAddressTableBase[dwOridinalName];

	return dwFuncAddr;

}

/*
�������������������ļ����ݰ��ڴ�����ʽ����
������
	pFileBuffer:�ļ����ݻ���
����ֵ���ڴ�������ļ�����
*/
PVOID AlignFileToMem( PVOID pFileBuffer, DWORD dwFileSize )
{
	//���������ַ������Լ���������ļ���С��������_IMAGE_NT_HEADERS�ṹ�У�Ȼ�����ÿ����ͷ�����ݽ�ͷ�еĽ��ڴ�ƫ�ƿ����ļ�����
	//����ʱ�ڱ��еĽ�ƫ�Ʋ�׼ȷ�����������������PE�ļ���ʽ�Լ�����

	//�����ļ��ڴ�����Ĵ�С
	DWORD dwSize = GetTotalImageSize( pFileBuffer, dwFileSize );
	if( dwSize == 0 )
	{
		cout<<"�����ļ�������Сʧ��"<<endl;
		return NULL;
	}
	PVOID pImageBuffer = VirtualAllocEx( GetCurrentProcess(), NULL, dwSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE );
	if( pImageBuffer == NULL )
	{
		cout<<"����ӳ���ڴ�ʧ��"<<endl;
		return NULL;
	}
	//���ӳ���ڴ�
	memset( pImageBuffer, 0, dwSize );

	//����PEͷ��Ϣ
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)( (ULONG)pFileBuffer + ((PIMAGE_DOS_HEADER)pFileBuffer)->e_lfanew );
	PIMAGE_SECTION_HEADER pSectionHeader = (PIMAGE_SECTION_HEADER)( (ULONG)pNtHeaders + sizeof(IMAGE_NT_HEADERS) );
	DWORD dwCpySize = pNtHeaders->OptionalHeader.SizeOfHeaders;
	for( DWORD dwIndex = 0; dwIndex < pNtHeaders->FileHeader.NumberOfSections; dwIndex++ )
	{
		if( (pSectionHeader[dwIndex].PointerToRawData) != 0 && (pSectionHeader[dwIndex].PointerToRawData<dwCpySize) )
			dwCpySize = pSectionHeader[dwIndex].PointerToRawData;
	}
	memcpy( pImageBuffer, pFileBuffer, dwCpySize );
	PVOID pt = (PVOID)((ULONG)pImageBuffer + GetAlignSize( pNtHeaders->OptionalHeader.SizeOfHeaders, pNtHeaders->OptionalHeader.SectionAlignment ));
	for( DWORD dwIndex = 0; dwIndex < pNtHeaders->FileHeader.NumberOfSections; dwIndex++ )
	{
		if( pSectionHeader[dwIndex].VirtualAddress != 0 )
			pt = (PVOID)( (DWORD)pImageBuffer + pSectionHeader[dwIndex].VirtualAddress);
		if( pSectionHeader[dwIndex].SizeOfRawData != 0 )
		{
			memcpy( pt, (PVOID)( (DWORD)pFileBuffer + pSectionHeader[dwIndex].PointerToRawData), pSectionHeader[dwIndex].SizeOfRawData );
			if( pSectionHeader[dwIndex].SizeOfRawData > pSectionHeader[dwIndex].Misc.VirtualSize )
				pt = (PVOID)( (ULONG)pt + GetAlignSize(pSectionHeader[dwIndex].SizeOfRawData, pNtHeaders->OptionalHeader.SectionAlignment ) );
			else
				pt = (PVOID)( (ULONG)pt + GetAlignSize(pSectionHeader[dwIndex].Misc.VirtualSize, pNtHeaders->OptionalHeader.SectionAlignment ) );

		}
		else
			pt = (PVOID)( (ULONG)pt + GetAlignSize(pSectionHeader[dwIndex].Misc.VirtualSize, pNtHeaders->OptionalHeader.SectionAlignment ) );
	}
	return pImageBuffer;
}

/*
����˵���������ļ��ڴ�����Ĵ�С,���ȼ����ļ�ͷ����ֵ��С���ټ���ÿ�ڵĶ���ֵ��С������ڱ��е�VirtualAddress�ǿգ�VirtualSize��0����VirtualSizeֵ�������ֵ
	//���Ϊ0����SizeOfRawData�������ֵ����֮�����VirtualAddressΪ�գ�˭��Ͱ�˭�������ֵ
������
	pFileBuffer:�ļ�������ʼ��ַ
����ֵ�������ļ����ݰ��ڴ�����Ĵ�С
*/
DWORD GetTotalImageSize( PVOID pFileBuffer, DWORD dwFileSize )
{
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pFileBuffer;
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)( (ULONG)pFileBuffer + pDosHeader->e_lfanew );

	DWORD dwTotalSize = GetAlignSize( pNtHeaders->OptionalHeader.SizeOfHeaders, pNtHeaders->OptionalHeader.SectionAlignment );

	PIMAGE_SECTION_HEADER pSectionHeader = (PIMAGE_SECTION_HEADER)( (ULONG)pNtHeaders + sizeof(IMAGE_NT_HEADERS) );

	DWORD dwSectionCnt = pNtHeaders->FileHeader.NumberOfSections;
	for( DWORD dwIndex = 0; dwIndex < dwSectionCnt; dwIndex++ )
	{
		if( (pSectionHeader[dwIndex].PointerToRawData + pSectionHeader[dwIndex].SizeOfRawData) > dwFileSize )
		{
			cout<<"�ļ��ڱ���Ϣ������"<<endl;
			return 0;
		}
		else if( pSectionHeader[dwIndex].VirtualAddress != NULL )
		{
			if( pSectionHeader[dwIndex].Misc.VirtualSize != 0 )
				dwTotalSize += GetAlignSize( pSectionHeader[dwIndex].Misc.VirtualSize, pNtHeaders->OptionalHeader.SectionAlignment );
			else
				dwTotalSize += GetAlignSize( pSectionHeader[dwIndex].SizeOfRawData, pNtHeaders->OptionalHeader.SectionAlignment );
		}
		else 
		{
			if( pSectionHeader[dwIndex].SizeOfRawData > pSectionHeader[dwIndex].Misc.VirtualSize )
				dwTotalSize += GetAlignSize( pSectionHeader[dwIndex].SizeOfRawData, pNtHeaders->OptionalHeader.SectionAlignment );
			else
				dwTotalSize += GetAlignSize( pSectionHeader[dwIndex].Misc.VirtualSize, pNtHeaders->OptionalHeader.SectionAlignment );
		}
	}
	return dwTotalSize;
}

DWORD GetAlignSize( DWORD dwSize, DWORD dwAlignSize )
{
	return (dwSize+dwAlignSize-1)/dwAlignSize*dwAlignSize;
}

/*
����˵����������ڴ������ļ����ض�λ��
������
	pImageBuffer:�������ļ���ַ
*/
bool ProcessRelocTable( PVOID pImageBuffer )
{

	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pImageBuffer;
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)( (ULONG)pDosHeader + pDosHeader->e_lfanew );

	//��ȡRtlImageDirectoryEntryToData������ַ
	HMODULE hNtDll = LoadLibrary( "ntdll.dll" );
	PRTLIMAGEDIRECTORYENTRYTODATA pRtlImageDirectoryEntryToData = (PRTLIMAGEDIRECTORYENTRYTODATA)GetProcAddress( hNtDll, "RtlImageDirectoryEntryToData" );
	if( pRtlImageDirectoryEntryToData == NULL )
	{
		cout<<"��ȡRtlImageDirectoryEntryToData������ַʧ��"<<endl;
		FreeLibrary( hNtDll );
		return false;
	}
	DWORD dwRelocSize = 0; 
	PIMAGE_BASE_RELOCATION pRelocDescriptor = (PIMAGE_BASE_RELOCATION)pRtlImageDirectoryEntryToData( pImageBuffer, true, IMAGE_DIRECTORY_ENTRY_BASERELOC, &dwRelocSize );
	if( pRelocDescriptor == NULL )
	{
		FreeLibrary( hNtDll );
		cout<<"���ض�λ��"<<endl;
		return true;
	}
	FreeLibrary( hNtDll );

	DWORD dwRelocaSize = pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size;
	DWORD dwDelta = (DWORD)pImageBuffer - pNtHeaders->OptionalHeader.ImageBase;
	while( dwRelocaSize > 0 )
	{
		PUSHORT pFixup = (PUSHORT)((ULONG)pRelocDescriptor + sizeof(IMAGE_BASE_RELOCATION));
		for( DWORD dwIndex = 0; dwIndex< ( (pRelocDescriptor->SizeOfBlock-sizeof(IMAGE_BASE_RELOCATION))/2 ); dwIndex++ )
		{
			
			if( (pFixup[dwIndex]>>12) == IMAGE_REL_BASED_HIGHLOW )
			{
				DWORD dwAddr = (DWORD)pImageBuffer + pRelocDescriptor->VirtualAddress + (pFixup[dwIndex]&0xfff);
				*(PULONG)dwAddr += dwDelta;
			}
			
			//DWORD dwAddr = (DWORD)pImageBuffer + pRelocDescriptor->VirtualAddress + (pFixup[dwIndex]&0xfff);
		//	*(PULONG)dwAddr += dwDelta;
		}
		dwRelocaSize -= pRelocDescriptor->SizeOfBlock;
		pRelocDescriptor = (PIMAGE_BASE_RELOCATION)((ULONG)pRelocDescriptor + pRelocDescriptor->SizeOfBlock);
	}
	return true;

}