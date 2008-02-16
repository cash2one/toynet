// SendInviteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "SendInviteDlg.h"

#include "MyBitmap.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSendInviteDlg dialog


CSendInviteDlg::CSendInviteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSendInviteDlg::IDD, pParent)
{
	TargetUNum = 0;
	//{{AFX_DATA_INIT(CSendInviteDlg)
	m_InviteMsg = _T("");
	//}}AFX_DATA_INIT
}


void CSendInviteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSendInviteDlg)
	DDX_Control(pDX, IDCANCEL, m_CancelBtn);
	DDX_Control(pDX, IDOK, m_OkBtn);
	DDX_Text(pDX, IDC_INVITEMSG_EDIT, m_InviteMsg);
	DDV_MaxChars(pDX, m_InviteMsg, 50);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSendInviteDlg, CDialog)
	//{{AFX_MSG_MAP(CSendInviteDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSendInviteDlg message handlers

BOOL CSendInviteDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
//	CString str;
//	str.Format("[%s]님을 초대합니다", TargetID.operator LPCTSTR());
//	SetWindowText(str.operator LPCTSTR());

	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);


	m_OkBtn.Init(62,127,"image\\Invite\\iBtn_sendinvite.bmp", 4);
	m_CancelBtn.Init(162,127,"image\\commonbtn\\Btn_cancel.bmp", 4);
	
	GetDlgItem(IDC_INVITEMSG_EDIT)->SetFocus();

	RECT rc1;
	GetWindowRect( &rc1 );
	rc1.right = rc1.left + 308;
	rc1.bottom = rc1.top + 169;
	MoveWindow( &rc1 );

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSendInviteDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CDialog::OnOK();
}

void CSendInviteDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

int CSendInviteDlg::DoModal(int unum, CString &id) 
{
	// TODO: Add your specialized code here and/or call the base class
	TargetUNum = unum;
	TargetID = id;
	return CDialog::DoModal();
}

void CSendInviteDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
	
	CMyBitmap Back;
	Back.LoadBitmapFile("image\\Invite\\InviteSendMsg.bmp");

	HRGN h = BitmapToRegion( Back.operator HBITMAP(), RGB(255,0,255));
	if(h) SetWindowRgn(h,TRUE);// 사용방법

	CBitmap *pOldBmp = cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

		// 폰트 생성
	CFont Font1;
	Font1.CreateFont(12,0,0,0,
		FW_NORMAL,
		//FW_BOLD,
		FALSE,FALSE,
		0,
		//ANSI_CHARSET,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		"굴림");

	dc.SetBkMode(TRANSPARENT);

	CFont* pOldFont = dc.SelectObject(&Font1);
	
	CString str;
	str.Format("[%s]님을 초대합니다", TargetID.operator LPCTSTR());
	dc.TextOut(15, 40, str);

	
	cdc.SelectObject(pOldFont);
	cdc.SelectObject(pOldBmp);
	::DeleteObject(h);
	h = NULL;
	cdc.DeleteDC();
	Back.DeleteObject();
	Font1.DeleteObject();
	
	// Do not call CDialog::OnPaint() for painting messages
}
