// MsgBox.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "MsgBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgBox dialog


CMsgBox::CMsgBox(CWnd* pParent /*=NULL*/)
	: CDialog(CMsgBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMsgBox)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMsgBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgBox)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMsgBox, CDialog)
	//{{AFX_MSG_MAP(CMsgBox)
	ON_BN_CLICKED(IDYES, OnYes)
	ON_BN_CLICKED(IDNO, OnNo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgBox message handlers

void CMsgBox::OnYes() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}

void CMsgBox::OnNo() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CMsgBox::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CMsgBox::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

int CMsgBox::DoModal(char* strMsg, char* strTitle) 
{
	// TODO: Add your specialized code here and/or call the base class
	Msg.Format(strMsg);
	if(strTitle) Title.Format(strTitle);
	return CDialog::DoModal();
}

BOOL CMsgBox::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_EDIT_MSG)->SetWindowText(Msg);
	if(Title.GetLength() > 0) SetWindowText(Title);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

