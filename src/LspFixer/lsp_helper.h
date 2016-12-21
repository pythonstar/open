#ifndef __LSP_HELPER_H__
#define __LSP_HELPER_H__

#include <atlmisc.h>

#include "common_def.h"

/*
 * code by Root
 * E-mail: cppcoffee@gmail.com
*/
namespace lsp_helper{

	// ���ֽ�ת���ֽ�
	int MByteToWChar( LPCSTR lpMultiByteStr, 
		LPWSTR lpWideCharStr,
		int cchWideChar );

	// չ����������
	CString ExpandEnvironment(LPCTSTR item_data);

	// ��ȡָ���ļ���˵���͹�˾����
	BOOL GetFileDescAndCompany( LPCTSTR file_path, CString& file_descript, CString& company_name );

	// ����ע���
	BOOL ExportRegPath( LPCTSTR reg_path, LPCTSTR export_path );

	// ��ȡ�����ļ����б�
	BOOL ReadRestoreData( TCHAR* buf, DWORD size, LPCTSTR file_path );

}


#endif