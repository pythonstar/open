#ifndef __LSP_FIX_TOOL_H__
#define __LSP_FIX_TOOL_H__

#include <vector>

#include "common_def.h"


/*
 * ö��LSPʱ���õĻص���������
 * code by Root
 * E-mail: cppcoffee@gmail.com
*/
typedef BOOL (WINAPI *PEnumLspCallBack)(void* param,
										const LPCTSTR lsp_name, 
										const LPCTSTR dll_file_path,
										const LPCTSTR clsid);


/*
 * ������ע����е�LSP�ṹ����Ϣ
 * code by Root
 * E-mail: cppcoffee@gmail.com
*/
typedef struct tagPACKED_CATALOG_ITEM
{
	char spi_path[MAX_PATH];
	WSAPROTOCOL_INFO protocol_info;
} PACKED_CATALOG_ITEM, *PPACKED_CATALOG_ITEM;


namespace lspfix_tool{

	// �޸�LSP
	BOOL FixLSP(std::vector<PLSP_INFO>& lspinfo);

	// �ָ�LSP�����һ��״̬
	BOOL RestoreLSP();

	// ö��LSP(�����ǻص�����)
	BOOL EnumLspData(PEnumLspCallBack CallBackFuncPtr, void* param);

	// ��ȡProtocolCatalog����ע���·��
	CString GetCurrentProtocolCatalogPath();

	// ��ȡ��ȡ��¼�������
	int GetNumCatalogEntries(LPCTSTR catalog_path);

	// ��ȡָ��LSP����
	BOOL GetPackedCatalogItem( LPCTSTR catalog_path, 
		LPCTSTR item_name, 
		PPACKED_CATALOG_ITEM buffer );

	// ��ȡLSP��dll����·��
	CString GetPathForPackedItem(char* item_data);

	// ��clsidת����CString
	CString GetClsidFromItem( PPACKED_CATALOG_ITEM item_data );

	// �Ƿ��Ѿ���ȷ��LSP
	BOOL IsValidLsp( LPCTSTR file_path, LPCTSTR clsid );

	// ����LSP��Ϣ
	void BackupLspInfo(std::vector<PLSP_INFO>& lspinfo);

	// ����LSP DLL�ļ�
	void BackupDllFile(std::vector<PLSP_INFO>& lspinfo);

	// ����LSP��DLL�ļ���ָ����·��
	BOOL CopyLspDllFile(LPCTSTR src, LPCTSTR dst);

	// rundll32.exe /u /s
	BOOL DeinstRundll(LPCTSTR dll_path);

	// ɾ��LSP
	void DeinstallProvider(LPCTSTR clsid);

	// ����ע����ļ�
	BOOL ImportRegFile(LPCTSTR regpath);

	// ��ԭ�����LSP DLL
	BOOL RestoreDllFile();

}


#endif