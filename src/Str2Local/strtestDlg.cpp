// strtestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "strtest.h"
#include "strtestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CstrtestDlg �Ի���


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


// CstrtestDlg ��Ϣ�������

BOOL CstrtestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_strOut = _T("������ȫ���ַ���ת��Ϊ�ֲ������洢����ֹ�������̬������¶�����ַ�����û�������ܴ���\r\n\r\n�������룺C:\\\\file\\\\�ҵ��ļ�.txt\r\n");
	UpdateData(FALSE);
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
} 


//����Ҫ��\r\n
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

	//ע�⣺���ﲻ���ͷţ������ÿ��һ�����ò��ɹ��ġ�
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

	//�Ȼ�ȡUNICODEģʽ
	for ( int i=0; i<strInputW.GetLength(); ++i ){
		chTempW = strInputW.GetAt(i);
		if ( chTempW==L'\\' ){
			//ת���ַ��������ȡһ��
			++i;
			strTempW = strInputW.GetAt(i);
			strText += L"\'\\" + strTempW + L"\', ";
		}else if ( chTempW<0 || chTempW>127 ){
			//���������ַ�
			bContainChinese = TRUE;
			strTempW.Format(L"0x%X, ",chTempW);
			strText += strTempW;
		}else{
			//���������ַ�
			strTempW = chTempW;
			strText += L"\'" + strTempW + L"\', ";
		}

		if ( i==strInputW.GetLength()-1 ){
			strText += L"\'\\0\' };";
		}
	}
	m_strOut = L"T" + strHeader + strText + L"\r\n";

	//�������ַ�������£����ɵ�TCHAR�ַ�����CHAR�ַ����ǲ�һ����
	if ( bContainChinese==TRUE){
		strText.Empty();

		for ( int i=0; i<strInputA.GetLength(); ++i ){
			chTempA = strInputA.GetAt(i);
			if ( chTempA=='\\' ){
				//ת���ַ��������ȡһ��
				++i;
				strTempA = strInputA.GetAt(i);
				strText += "\'\\" + strTempA + "\', ";
			}else if ( chTempA<0 || chTempA>127 ){
				//���������ַ�
				strTempA.Format("0x%X, ",(unsigned char)chTempA);
				strText += strTempA;
			}else{
				//���������ַ�
				strTempA = chTempA;
				strText += "\'" + strTempA + "\', ";
			}

			if ( i==strInputA.GetLength()-1 ){
				strText += "\'\\0\' };";
			}
		}//end for

		m_strOut = _T("#ifdef _UNICODE\r\n") + m_strOut + _T("#else\r\n") + strHeader + strText + _T("\r\n#endif\r\n");
	}

	//��ԭ�ַ�����Ϊע��
	m_strOut = _T("\r\n//") + m_strIN + _T("\r\n") + m_strOut;

	CopyToClipboard(m_strOut);
	UpdateData(FALSE);
}
