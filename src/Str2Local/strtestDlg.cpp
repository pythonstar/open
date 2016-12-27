// strtestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "strtest.h"
#include "strtestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CstrtestDlg 对话框


CstrtestDlg::CstrtestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CstrtestDlg::IDD, pParent)
	, m_strIN(_T(""))
	, m_strOut(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CstrtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strIN);
	DDX_Text(pDX, IDC_EDIT2, m_strOut);
}

BEGIN_MESSAGE_MAP(CstrtestDlg, CDialog)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CstrtestDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CstrtestDlg 消息处理程序

BOOL CstrtestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_strOut = _T("本程序将全局字符串转换为局部变量存储，防止软件被静态分析暴露敏感字符串，没有做加密处理。\r\n\r\n例如输入：C:\\\\file\\\\我的文件.txt\r\n");
	UpdateData(FALSE);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
} 


//换行要用\r\n
void CopyToClipboard(const CString&strText)
{
	if ( strText.IsEmpty()==TRUE ){
		return;
	}

	CStringA strTextA;
	strTextA = strText;

	HGLOBAL hClip = GlobalAlloc(GHND|GMEM_SHARE,strTextA.GetLength()+1);
	if ( hClip==NULL ){
		return;
	}

	char *pBuff = (char*)GlobalLock(hClip);
	if( pBuff==NULL ){
		GlobalFree(hClip);
		return;
	}

	memcpy(pBuff,strTextA,strTextA.GetLength());
	pBuff[strTextA.GetLength()] = 0;
	GlobalUnlock(hClip);

	if ( OpenClipboard(NULL) ){
		EmptyClipboard();
		SetClipboardData( CF_TEXT,hClip);
		CloseClipboard();
	}

	//注意：这里不能释放，否则会每隔一次设置不成功的。
	//GlobalFree(hClip);
}


void CstrtestDlg::OnBnClickedOk()
{
	UpdateData();
	CString strHeader = _T("CHAR sz") + m_strIN + _T("[] = { ");
	CString strText;
	CStringA strInputA;
	CStringW strInputW;
	CStringA strTempA;
	CStringW strTempW;
	WCHAR chTempW = 0;
	CHAR chTempA = 0;

	strInputA = m_strIN;
	strInputW = m_strIN;

	BOOL bContainChinese = FALSE;

	//先获取UNICODE模式
	for ( int i=0; i<strInputW.GetLength(); ++i ){
		chTempW = strInputW.GetAt(i);
		if ( chTempW==L'\\' ){
			//转义字符，向后再取一个
			++i;
			strTempW = strInputW.GetAt(i);
			strText += L"\'\\" + strTempW + L"\', ";
		}else if ( chTempW<0 || chTempW>127 ){
			//看成中文字符
			bContainChinese = TRUE;
			strTempW.Format(L"0x%X, ",chTempW);
			strText += strTempW;
		}else{
			//其他正常字符
			strTempW = chTempW;
			strText += L"\'" + strTempW + L"\', ";
		}

		if ( i==strInputW.GetLength()-1 ){
			strText += L"\'\\0\' };";
		}
	}
	m_strOut = L"T" + strHeader + strText + L"\r\n";

	//有中文字符的情况下，生成的TCHAR字符串和CHAR字符串是不一样的
	if ( bContainChinese==TRUE){
		strText.Empty();

		for ( int i=0; i<strInputA.GetLength(); ++i ){
			chTempA = strInputA.GetAt(i);
			if ( chTempA=='\\' ){
				//转义字符，向后再取一个
				++i;
				strTempA = strInputA.GetAt(i);
				strText += "\'\\" + strTempA + "\', ";
			}else if ( chTempA<0 || chTempA>127 ){
				//看成中文字符
				strTempA.Format("0x%X, ",(unsigned char)chTempA);
				strText += strTempA;
			}else{
				//其他正常字符
				strTempA = chTempA;
				strText += "\'" + strTempA + "\', ";
			}

			if ( i==strInputA.GetLength()-1 ){
				strText += "\'\\0\' };";
			}
		}//end for

		m_strOut = _T("#ifdef _UNICODE\r\n") + m_strOut + _T("#else\r\n") + strHeader + strText + _T("\r\n#endif\r\n");
	}

	//把原字符串作为注释
	m_strOut = _T("\r\n//") + m_strIN + _T("\r\n") + m_strOut;

	CopyToClipboard(m_strOut);
	UpdateData(FALSE);
}
