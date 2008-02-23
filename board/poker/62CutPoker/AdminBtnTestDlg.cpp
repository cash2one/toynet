// AdminBtnTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AdminBtnTestDlg.h"
#include "DialogCtrlReposition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdminBtnTestDlg dialog


CAdminBtnTestDlg::CAdminBtnTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAdminBtnTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdminBtnTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAdminBtnTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdminBtnTestDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdminBtnTestDlg, CDialog)
	//{{AFX_MSG_MAP(CAdminBtnTestDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdminBtnTestDlg message handlers

BOOL CAdminBtnTestDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
		
	if(testBackSpr_.Load(".\\data\\Device\\testboard.spr", 555)==FALSE) AfxMessageBox("x2gameback.spr 파일을 읽을 수 없습니다");
	if(testBtnSpr_.Load(".\\data\\Device\\testbutten.spr", 555)==FALSE) AfxMessageBox("StartBtn.spr 파일을 읽을 수 없습니다");


	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);

	page_.Init(800, 300, 16);

	bLeft_=	bRight_ = bSelect_ = 0;

	mouse_ = 0;

	bBtnX_ = bBtnY_ = bBtnZ_ = 0;
	bBtnA_ = bBtnB_ = bBtnC_ = 0;

	//CenterWindow(g_pGameView->GetOwner());

	return TRUE;  // return TRUE unless you set the focus to a control

}

void CAdminBtnTestDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	
	// TODO: Add your message handler code here
	// 더블 버퍼링용 메모리 DC 생성
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);

	// 메모리DC에 버퍼용 비트맵 핸들을 선택
	MemDC.SelectObject(page_.hBmp);

	page_.PutSprAuto(0, 0, &testBackSpr_, 0);

	//<-, O, ->
	page_.PutSprAuto(40, 120, &testBtnSpr_, 0+bLeft_);
	page_.PutSprAuto(100, 120, &testBtnSpr_, 2+bSelect_);
	page_.PutSprAuto(160, 120, &testBtnSpr_, 4+bRight_);
	
	//mouse
	page_.PutSprAuto(270, 100, &testBtnSpr_, 6);

	
	//A,B,C
	page_.PutSprAuto(380, 110, &testBtnSpr_, 11+bBtnX_);
	page_.PutSprAuto(460, 110, &testBtnSpr_, 13+bBtnY_);
	page_.PutSprAuto(520, 110, &testBtnSpr_, 15+bBtnZ_);

	//X,Y,Z
	page_.PutSprAuto(380, 160, &testBtnSpr_, 17+bBtnA_);
	page_.PutSprAuto(460, 160, &testBtnSpr_, 19+bBtnB_);
	page_.PutSprAuto(520, 160, &testBtnSpr_, 21+bBtnC_);
	


	// 마우스 좌표 체크
	MemDC.SetBkMode(TRANSPARENT);
	MemDC.SetTextColor(RGB(255,255,255));


	CRect rect;
	GetClientRect(&rect);

	dc.BitBlt(0, 0, rect.right, rect.bottom, &MemDC, 0, 0, SRCCOPY);
	// 부분 갱신 플래그 해제
//	bSetInvalidate = FALSE;
	MemDC.DeleteDC();
	
	// Do not call CDialog::OnPaint() for painting messages
}



BOOL CAdminBtnTestDlg::PreTranslateMessage(MSG* pMsg) 
{
// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN||pMsg->message == WM_KEYUP) 
	{
		BOOL bDown=0;

		if (pMsg->message == WM_KEYDOWN)
			bDown = 1;

		if(pMsg->wParam == 'A' || pMsg->wParam == 'a')
		{
			bLeft_ = bDown;
		}
		else if(pMsg->wParam == 'D' || pMsg->wParam == 'd')
		{
			bRight_ = bDown;
		}
		else if(pMsg->wParam == 'S' || pMsg->wParam == 's')
		{
			bSelect_ = bDown;
		}
		else if(pMsg->wParam == 'U' || pMsg->wParam == 'u')
		{
			bBtnX_ = bDown;
		}
		else if(pMsg->wParam == 'I' || pMsg->wParam == 'i')
		{
			bBtnY_ = bDown;
		}
		else if(pMsg->wParam == 'O' || pMsg->wParam == 'o')
		{
			bBtnZ_ = bDown;
		}
		else if(pMsg->wParam == 'J' || pMsg->wParam == 'j')
		{
			bBtnA_ = bDown;
		}
		else if(pMsg->wParam == 'K' || pMsg->wParam == 'k')
		{
			bBtnB_ = bDown;
		}
		else if(pMsg->wParam == 'L' || pMsg->wParam == 'l')
		{
			bBtnC_ = bDown;
		}

		Invalidate(FALSE);
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
