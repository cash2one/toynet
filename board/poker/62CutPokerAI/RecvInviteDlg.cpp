// RecvInviteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "RecvInviteDlg.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecvInviteDlg dialog


CRecvInviteDlg::CRecvInviteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRecvInviteDlg::IDD, pParent)
{
	TargetUNum = 0;
	RoomNum = 0;
	//{{AFX_DATA_INIT(CRecvInviteDlg)
	m_RefuseMsg = _T("");
	m_RoomInfo = _T("");
	//}}AFX_DATA_INIT
}


void CRecvInviteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecvInviteDlg)
	DDX_Control(pDX, IDCANCEL, m_CancelBtn);
	DDX_Control(pDX, IDOK, m_OkBtn);
	DDX_Text(pDX, IDC_REFUSEMSG_EDIT, m_RefuseMsg);
	DDV_MaxChars(pDX, m_RefuseMsg, 50);
	DDX_Text(pDX, IDC_ROOMINFO_EDIT, m_RoomInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRecvInviteDlg, CDialog)
	//{{AFX_MSG_MAP(CRecvInviteDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecvInviteDlg message handlers

BOOL CRecvInviteDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);


	GetDlgItem(IDC_ROOMINFO_EDIT)->SetWindowText(szMsg);
	GetDlgItem(IDC_REFUSEMSG_EDIT)->SetFocus();

	m_OkBtn.Init(77,167,"image\\Invite\\iBtn_okinvite.bmp", 4);
	m_CancelBtn.Init(177,167,"image\\Invite\\iBtn_refuse.bmp", 4);

	// 초대 알림 사운드
	Sound.Play(SND40);
	
	RECT rc1;
	GetWindowRect( &rc1 );
	rc1.right = rc1.left + 331;
	rc1.bottom = rc1.top + 211;
	MoveWindow( &rc1 );

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRecvInviteDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CDialog::OnOK();
}

void CRecvInviteDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	UpdateData(TRUE);
	CDialog::OnCancel();
}

int CRecvInviteDlg::DoModal(int tarunum, char *tarid, int roomnum, char *strmsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	TargetUNum = tarunum;
	TargetID = (LPCTSTR)tarid;
	RoomNum = roomnum;
	szMsg = (LPCTSTR)strmsg;
	return CDialog::DoModal();
}

void CRecvInviteDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
	
	CMyBitmap Back;
	Back.LoadBitmapFile("image\\Invite\\Invite.bmp");

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
	CRect rt;
	str.Format("[%s]님이 %d번 방에서 당신을 초대합니다", TargetID.operator LPCTSTR(), RoomNum);
	//SetWindowText(str);	
	rt.SetRect(0,0,299,16);
	rt.OffsetRect(16,43);	
	dc.DrawText(str,&rt,DT_LEFT | DT_WORDBREAK);
	rt.SetRect(0,0,299,32);
	rt.OffsetRect(16,82);
	dc.DrawText(szMsg,&rt,DT_LEFT | DT_WORDBREAK);


	
	cdc.SelectObject(pOldFont);
	cdc.SelectObject(pOldBmp);
	::DeleteObject(h);
	h = NULL;
	cdc.DeleteDC();
	Back.DeleteObject();
	Font1.DeleteObject();
	
	// Do not call CDialog::OnPaint() for painting messages
}
