// ChangeCharDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "ChangeCharDlg.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChangeCharDlg dialog


CChangeCharDlg::CChangeCharDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChangeCharDlg::IDD, pParent)
{
	NowPage = NowSel = 0;
	//{{AFX_DATA_INIT(CChangeCharDlg)
	a = NULL;
	//}}AFX_DATA_INIT
}


void CChangeCharDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChangeCharDlg)
	DDX_Control(pDX, IDC_RIGHT_BUTTON, m_RightBtn);
	DDX_Control(pDX, IDCANCEL, m_CancelBtn);
	DDX_Control(pDX, IDOK, m_OKBtn);
	DDX_Control(pDX, IDC_LEFT_BUTTON, m_LeftBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChangeCharDlg, CDialog)
	//{{AFX_MSG_MAP(CChangeCharDlg)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_LEFT_BUTTON, OnLeftButton)
	ON_BN_CLICKED(IDC_RIGHT_BUTTON, OnRightButton)
	ON_WM_WINDOWPOSCHANGING()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChangeCharDlg message handlers

BOOL CChangeCharDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);

	// 배경 그림 로드
	Back.LoadBitmapFile(".\\image\\cchangeback.bmp");

	// 버튼 초기화
	m_LeftBtn.Init(IDB_CHANGECHAR_LEFT1, IDB_CHANGECHAR_LEFT2, IDB_CHANGECHAR_LEFT3, NULL);
	m_RightBtn.Init(IDB_CHANGECHAR_RIGHT1, IDB_CHANGECHAR_RIGHT2, IDB_CHANGECHAR_RIGHT3, NULL);
	m_OKBtn.Init(IDB_CHANGECHAR_OK1, IDB_CHANGECHAR_OK2, IDB_CHANGECHAR_OK3, NULL);
	m_CancelBtn.Init(IDB_CHANGECHAR_CANCEL1, IDB_CHANGECHAR_CANCEL2, IDB_CHANGECHAR_CANCEL3, NULL);

//	NowSel = Play[0].UI.Character;
	Sex = Play[0].UI.Sex;

	NowPage = 0;

	CRect rc;
	rc.SetRect(0,0,145,144);
	rc.OffsetRect(28,42);
	m_LCharView.Create(this, rc, 2, 1234);

	rc.SetRect(0,0,45,61);
	rc.OffsetRect(176, 66);
	m_SCharView[0].Create(this, rc, 3, IDC_SCHAR1);
	rc.OffsetRect(49, 0);
	m_SCharView[1].Create(this, rc, 3, IDC_SCHAR2);
	rc.OffsetRect(49, 0);
	m_SCharView[2].Create(this, rc, 3, IDC_SCHAR3);
	rc.OffsetRect(49, 0);
	m_SCharView[3].Create(this, rc, 3, IDC_SCHAR4);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChangeCharDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	Back.DeleteObject();
}

void CChangeCharDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
	cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	/*
	m_LCharView.SetCharacter(NowSel);

	if(Sex)
	{
		int start = (NowPage * 4)+1;
		for(int i=start; i<start+4; i++) m_SCharView[i-start].SetCharacter(i);
	}
	else
	{
		int start = (NowPage * 4)+37;
		for(int i=start; i<start+4; i++) m_SCharView[i-start].SetCharacter(i);
	}
	*/

	// Do not call CDialog::OnPaint() for painting messages
}

void CChangeCharDlg::OnOK() 
{
	// TODO: Add extra validation here
	Sound.Play(SND45);
	CDialog::OnOK();
}

void CChangeCharDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	Sound.Play(SND45);
	CDialog::OnCancel();
}

void CChangeCharDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

void CChangeCharDlg::OnLeftButton() 
{
	// TODO: Add your control notification handler code here
	Sound.Play(SND45);
	if(NowPage > 0)
	{
		NowPage--;
		m_SCharView[0].bNowSeleted = FALSE; 
		m_SCharView[1].bNowSeleted = FALSE; 
		m_SCharView[2].bNowSeleted = FALSE; 
		m_SCharView[3].bNowSeleted = FALSE; 
		Invalidate(FALSE);
	}
}

void CChangeCharDlg::OnRightButton() 
{
	// TODO: Add your control notification handler code here
	Sound.Play(SND45);
	if(NowPage < 9-1)
	{
		NowPage++;
		m_SCharView[0].bNowSeleted = FALSE; 
		m_SCharView[1].bNowSeleted = FALSE; 
		m_SCharView[2].bNowSeleted = FALSE; 
		m_SCharView[3].bNowSeleted = FALSE; 
		Invalidate(FALSE);
	}
}

BOOL CChangeCharDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(wParam == IDC_SCHAR1 || wParam == IDC_SCHAR2 || wParam == IDC_SCHAR3 || wParam == IDC_SCHAR4)
	{
		m_SCharView[0].bNowSeleted = FALSE; 
		m_SCharView[1].bNowSeleted = FALSE; 
		m_SCharView[2].bNowSeleted = FALSE; 
		m_SCharView[3].bNowSeleted = FALSE; 

		Sound.Play(SND45);
	}

	switch(wParam)
	{
	case IDC_SCHAR1: NowSel = m_SCharView[0].CharNum; m_SCharView[0].bNowSeleted = TRUE; Invalidate(FALSE); break;
	case IDC_SCHAR2: NowSel = m_SCharView[1].CharNum; m_SCharView[1].bNowSeleted = TRUE; Invalidate(FALSE); break;
	case IDC_SCHAR3: NowSel = m_SCharView[2].CharNum; m_SCharView[2].bNowSeleted = TRUE; Invalidate(FALSE); break;
	case IDC_SCHAR4: NowSel = m_SCharView[3].CharNum; m_SCharView[3].bNowSeleted = TRUE; Invalidate(FALSE); break;
	}
	return CDialog::OnCommand(wParam, lParam);
}

void CChangeCharDlg::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CDialog::OnWindowPosChanging(lpwndpos);
	
	// TODO: Add your message handler code here
//	lpwndpos->flags &= ~SWP_SHOWWINDOW;
	
}
