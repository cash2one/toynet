// ItemHelpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SPoker2.h"
#include "ItemHelpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CItemHelpDlg dialog


CItemHelpDlg::CItemHelpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CItemHelpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CItemHelpDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CItemHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CItemHelpDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CItemHelpDlg, CDialog)
	//{{AFX_MSG_MAP(CItemHelpDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemHelpDlg message handlers

BOOL CItemHelpDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	CString strhelp =	"���Ӿ������� ���ӿ��� ����� �� �ִ� �������� ���մϴ�.\r\n\r\n\r\n"
						"[��������]�� Ŭ���Ͽ�, �����ϰ� �ִ� �������� Ȯ���� �� �ֽ��ϴ�.\r\n\r\n"
						"[��������]�� �����ϰ� �ִ� �������� �ٷ� ����� �� �� �ֽ��ϴ�.\r\n\r\n"
						"�������� �����ϰ� ���� ���� ���, [�����۱���]�� ���Ͽ�, \r\n\r\n"
						"�������� �ٷ� ������ �� �ֽ��ϴ�.\r\n\r\n";

	GetDlgItem(IDC_EDIT_ITEMHELP)->SetWindowText(strhelp);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CItemHelpDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	//CDialog::OnOK();
}

void CItemHelpDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
