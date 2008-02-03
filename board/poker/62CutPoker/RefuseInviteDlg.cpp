// RefuseInviteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "RefuseInviteDlg.h"

#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRefuseInviteDlg dialog


CRefuseInviteDlg::CRefuseInviteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRefuseInviteDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRefuseInviteDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CRefuseInviteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRefuseInviteDlg)
	DDX_Control(pDX, IDOK, m_OkBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRefuseInviteDlg, CDialog)
	//{{AFX_MSG_MAP(CRefuseInviteDlg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRefuseInviteDlg message handlers

BOOL CRefuseInviteDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);

	// 배경 그림 읽기
	Back.LoadBitmapFile(".\\image\\Invite\\RefuseInvite.bmp");
	// 버튼 초기화
	m_OkBtn.Init(97,117,".\\image\\commonbtn\\Btn_ok.bmp", 4);

	// TODO: Add extra initialization here
	CString str;
	str = szID + "님이 초대를 거절하였습니다";
//	GetDlgItem(IDC_STATE_STATIC)->SetWindowText(str);
//	GetDlgItem(IDC_REFUSEMSG_EDIT)->SetWindowText(szMsg);

	RECT rc1;
	GetWindowRect( &rc1 );
	rc1.right = rc1.left + 277;
	rc1.bottom = rc1.top + 161;
	MoveWindow( &rc1 );
	


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CRefuseInviteDlg::DoModal(char *id, char *strmsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	szID.Format("%s", id);
	szID += "님이 초대를 거절하였습니다";
	szMsg.Format("%s", strmsg);
	return CDialog::DoModal();
}


void CRefuseInviteDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;

	HRGN h = BitmapToRegion( Back.operator HBITMAP(), RGB(255,0,255));
	if(h) SetWindowRgn(h,TRUE);// 사용방법

	cdc.SelectObject(&Back);

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
	
	//아이디 
	CRect rt;
	rt.SetRect(0,0,247,22);
	rt.OffsetRect(16,42);
	dc.DrawText(szID,&rt, DT_LEFT | DT_WORDBREAK);
	//거절 내용
	rt.SetRect(0,0,203,40);
	rt.OffsetRect(61,72);
	dc.DrawText(szMsg,&rt, DT_LEFT | DT_WORDBREAK);

	::DeleteObject(h);
	h = NULL;
	cdc.DeleteDC();
	Font1.DeleteObject();
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CRefuseInviteDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	Back.DeleteObject();
	
}

void CRefuseInviteDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
