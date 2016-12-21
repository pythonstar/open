#ifndef __COMMON_DEF_H__
#define __COMMON_DEF_H__

#include <atlmisc.h>


/*
 * �ļ��汾�ŵĴ���ҳ�ṹ��
 * code by Root
 * E-mail: cppcoffee@gmail.com
*/
struct LANGANDCODEPAGE
{
	WORD wLanguage;  
	WORD wCodePage;  
};


/*
 * LSP�����Ϣ�Ľṹ��
 * code by Root
 * E-mail: cppcoffee@gmail.com
*/
typedef struct tagLSP_INFO
{
	CString dll_path;
	CString clsid;
	CString lsp_name;
	BOOL bIsValidLsp;
} LSP_INFO, *PLSP_INFO;

#endif