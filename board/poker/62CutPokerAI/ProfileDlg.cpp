// ProfileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "ProfileDlg.h"
#include "MyBitmap.h"
#include "Global.h"

#include "SysUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProfileDlg dialog


CProfileDlg::CProfileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProfileDlg::IDD, pParent)
{
	ZeroMemory(&UI, sizeof(UI));
	m_bHideBanishBtn = FALSE;		// [게임 아이템 작업]	<-- 추가
	//{{AFX_DATA_INIT(CProfileDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CProfileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProfileDlg)
	DDX_Control(pDX, IDC_BUTTON_MYHOMPY, m_MyHompyBtn);
	DDX_Control(pDX, IDC_BUTTON_BANISH, m_BanishBtn);
	DDX_Control(pDX, IDOK, m_ProfileCloseBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProfileDlg, CDialog)
	//{{AFX_MSG_MAP(CProfileDlg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_DETAILPROFILE, OnButtonDetailprofile)
	ON_BN_CLICKED(IDC_BUTTON_BANISH, OnButtonBanish)
	ON_BN_CLICKED(IDC_BUTTON_MYHOMPY, OnButtonMyhompy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProfileDlg message handlers

void CProfileDlg::Init(USERINFO *pUI, BOOL bInviteProfile)	// [게임 아이템 작업] <-- 변경
{
	memcpy(&UI, pUI, sizeof(UI));
	m_bHideBanishBtn = bInviteProfile;	// [게임 아이템 작업]	<-- 추가
}

BOOL CProfileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);

	// 배경 그림 읽기
	Back.LoadBitmapFile(".\\image\\Profile\\profileback.bmp");

	// [게임 아이템 작업]
	m_BanishBtn.Init(50, 234,"image\\Profile\\Btn_profile_forcebanish.bmp", 4);

	// 버튼 초기화
	m_ProfileCloseBtn.Init(144, 234, ".\\image\\Commonbtn\\Btn_close.bmp", 4);
	//m_ProfileCloseBtn.Init(111, 228,".\\image\\GBtn_invitec.bmp", 4);

	m_MyHompyBtn.Init(15, 173, ".\\image\\Profile\\Btn_profileMyHome.bmp", 4);


		// [게임 아이템 작업]
	if(g_Where != WH_GAME || m_bHideBanishBtn ) {		// <-- 변경
		// 게임 방이 아니거나 버튼 숨김 옵션이라면 버튼 숨김
		m_BanishBtn.ShowWindow(SW_HIDE);
	}
	// ### [ 관전기능 ] ### 
	else if(strcmp(g_MyInfo.UI.ID, UI.ID) == 0) {
		// 내 프로필 보기라면 버튼 숨김
		m_BanishBtn.ShowWindow(SW_HIDE);
	}


	// 폰트 생성
	if(Font1.m_hObject == NULL)
	Font1.CreateFont(12,0,0,0,
		//FW_NORMAL,
		FW_BOLD,
		FALSE,FALSE,
		0,
		//ANSI_CHARSET,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		"굴림");

	if(Font2.m_hObject == NULL)
	Font2.CreateFont(12,0,0,0,
		FW_NORMAL,
		FALSE,FALSE,
		0,
		//ANSI_CHARSET,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		"굴림");

	// 캐릭터 뷰 초기화
	CRect rc;
	rc.SetRect(0,0,75, 125);
	rc.OffsetRect(12,37);
	m_CharView.Create(this, rc, 1, 3222);

	RECT rc1;
	GetWindowRect( &rc1 );
	rc1.right = rc1.left + 276;
	rc1.bottom = rc1.top + 277;
	MoveWindow( &rc1 );

	//ShowWindow(SW_SHOW);
	//Invalidate();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProfileDlg::OnPaint() 
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

	//int ax = 15, ay = 187;
	int ax = 107, ay = 37;

	// 레벨 찍기
	// 레벨상점 폐쇄
	int level =  GetPokerLevel(UI.PMoney); //UI.nIcon;// GetLevel( UI.WinNum, UI.LooseNum, UI.DrawNum);
	CMyBitmap bmp;
	bmp.LoadBitmap(IDB_LEVEL15);
	bmp.TransDraw(dc.m_hDC, ax, ay, 15, 15, 15*level, 0, RGB(255,0,255));
	bmp.DeleteObject();

	dc.SetBkMode(TRANSPARENT);

	/*
	// 아이디 찍기	deleted by jeong
	dc.SelectObject(&Font1);
	dc.SetTextColor(RGB(0,0,0));
	dc.TextOut(ax+17, ay+2, UI.ID);
	*/

	if(UI.PMoney < 0) UI.PMoney = 0;
	CString str;
	CString strM = g_MakeCommaMoney(UI.PMoney);
	str.Format(g_StrMan.Get(_T("USER_PMONEY")),strM);
	dc.TextOut(13, 212, str);

	ax = 107, ay = 63;


	// 승률 계산
	int winpro = GetWinPro(UI.WinNum, UI.LooseNum);

	// 캐릭터 정보 찍기
	rect.SetRect(0,0,140,84);
	rect.OffsetRect(ax, ay);
	str.Format(g_StrMan.Get(_T("USER_TITLE2")), UI.WinNum, UI.LooseNum);//, UI.DrawNum); 
	dc.DrawText(str, &rect, DT_LEFT | DT_WORDBREAK);

	rect.OffsetRect(0, 15);
	str.Format("%d포기", UI.DrawNum); 
	dc.DrawText(str, &rect, DT_LEFT | DT_WORDBREAK);
	

	dc.SelectObject(&Font1);
//	dc.SetTextColor(RGB(255,220,100)); 
	rect.OffsetRect(0, 16);
	str.Format(g_StrMan.Get(_T("USER_RSF")),UI.PI.nRSF); 
	dc.DrawText(str, &rect, DT_LEFT | DT_WORDBREAK);
	
	rect.OffsetRect(0, 15);
	str.Format(g_StrMan.Get(_T("USER_SF")),UI.PI.nSF); 
	dc.DrawText(str, &rect, DT_LEFT | DT_WORDBREAK);
	
	rect.OffsetRect(0, 15);
	str.Format(g_StrMan.Get(_T("USER_FCARD")),UI.PI.nFCARD); 
	dc.DrawText(str, &rect, DT_LEFT | DT_WORDBREAK);

	// 하고싶은 말 찍기
	dc.SelectObject(&Font2);
	rect.SetRect(0,0,136,55);
	rect.OffsetRect(107, 147);
	str.Format("%s", UI.Desc);
	dc.DrawText(str, &rect, DT_LEFT | DT_WORDBREAK);

	//m_CharView.SetCharacter(UI.Character);
	m_CharView.SetCharacter(&UI);

	::DeleteObject(h);
	h = NULL;
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CProfileDlg::OnOK() 
{
	// TODO: Add extra validation here
	Sound.Play(SND45);
	CDialog::OnOK();
}

void CProfileDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	OnOK();
	//CDialog::OnCancel();
}

void CProfileDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	Back.DeleteObject();
	Font1.DeleteObject();
	Font2.DeleteObject();
}

void CProfileDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

void CProfileDlg::OnButtonDetailprofile() 
{
	// TODO: Add your control notification handler code here
	if(strlen(UI.ID)==0) return;
	g_WebProfileDlg.OpenProfile(UI.ID);
	SendMessage(WM_COMMAND, IDCANCEL);
}

// [게임 아이템 작업]
void CProfileDlg::OnButtonBanish() 
{
	// TODO: Add your control notification handler code here

	if(g_Where != WH_GAME) {
		PostMessage(WM_COMMAND, IDOK);
		return;
	}

	if(!g_RI.bSuperMaster) {
		g_pMainDlg->m_MsgBoxDlg.MsgBox(0,"슈퍼방이 아니므로 강제 추방시킬 수 없습니다.\r\n아이템을 구매하거나 사용하셔서 슈퍼방을 개설하시기 바랍니다.");
	}
	else {
		if(strcmp(g_MyInfo.UI.ID, g_RI.ID) == 0) {// ### [ 관전기능 ] ###
			// 방장인 경우만
			CString str;
			str.Format("[%s] 회원을 '강제 추방'하시겠습니까?", UI.ID);
			if(MessageBox(str, "강제 추방", MB_YESNO) == IDYES) {
				// 강제 추방 요청
				CSV_ASK_FORCEBANISH fbmsg;
				fbmsg.Set(UI.ID);
				SockMan.SendData(g_MainSrvSID, fbmsg.pData, fbmsg.GetTotalSize());
			}
		}
		else {
			// 방장이 아니면
			g_pMainDlg->m_MsgBoxDlg.MsgBox(0,"슈퍼방장이 아니므로 강제 추방시킬 수 없습니다.");
			
		}
	}

	//PostMessage(WM_COMMAND, IDOK);
	OnOK();
}

void CProfileDlg::OnButtonMyhompy() 
{
	// TODO: Add your control notification handler code here3
	OpenNehompy(g_MyUniqNo, UI.ID);

	PostMessage(WM_COMMAND, IDOK);
	
}
