#if !defined(AFX_GAMEVIEW_H__6C3FEBA4_E463_11D3_97A2_0050BF0FBE67__INCLUDED_)
#define AFX_GAMEVIEW_H__6C3FEBA4_E463_11D3_97A2_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GameView.h : header file
//
#include "Ime.h"
#include "Graphic.h"
#include "GraphButton.h"
#include "CharBox.h"

//#include "InfoBox.h"
#include "ChatBox.h"

// 동기화
//#include "ThreadTimer.h"
#include "SyncFPS.h"

/////////////////////////////////////////////////////////////////////////////
// CGameView window
// ### [ Frame++ ] ###
#ifdef _DEBUG
#define REFRESH_FPS 20
#else 
#define REFRESH_FPS 50
#endif

#define GAMEVIEW_WIDTH 800   //588
#define GAMEVIEW_HEIGHT 600  //542//482

#define GAMEVIEW_TIMER 10
#define RESTART_TIMER 20
#define MYTURN_TIMER 30

// 베팅 칩 판  (화면좌표)
#define BETPAGE_X 259 //271//289
#define BETPAGE_Y 226 //239//235

// 버튼
#define IDM_INVITE			(WM_USER+6100)
#define IDM_EXIT			(WM_USER+6101)
#define IDM_CHANGEDISPLAY   (WM_USER+6102)
#define IDM_OPTION			(WM_USER+6103)
#define IDM_BADUSER			(WM_USER+6104)

// 추방
#define IDM_BANISHVOTE		(WM_USER+6105)
#define IDM_SORT			(WM_USER+6106)
#define IDM_MASTER			(WM_USER+6107)
#define IDM_MINIMIZE		(WM_USER+6108)
#define IDM_GAMEITEM		(WM_USER+6109) // 게임 아이템


// 게임 진행 관련
#define IDM_START			(WM_USER+7098) // 시작 버튼
#define IDM_ALLIN			(WM_USER+7099)
#define IDM_DIE				(WM_USER+7100)
#define IDM_CHECK			(WM_USER+7101)
#define IDM_CALL			(WM_USER+7102)

#define IDM_PING			(WM_USER+7103)
#define IDM_DADANG			(WM_USER+7104)
#define IDM_MAX				(WM_USER+7105)

#define IDM_CAPTURE			(WM_USER+7106) // [화면캡쳐]

#define IDM_CHANGE_OK		(WM_USER+7107) // 62cut OK
#define IDM_CHANGE_CANCEL	(WM_USER+7108) // 62cut Cancel

#define IDM_TABVIEWCHAT		(WM_USER+7111) // 족보등급	보이기
#define IDM_TABVIEWRULE		(WM_USER+7112) // 족보등급	보이기
// ### [ 관전기능 ] ###
#define IDM_PARTICIPATION 	(WM_USER+7120)
#define IDM_EMERGE 			(WM_USER+7121)
#define IDM_OBSERVERVIEW	(WM_USER+7122)

#define UM_GAMEVIEW_TIMER	(WM_USER+8000)
#define UM_WINDCARD_TIMER	(WM_USER+8001)




// Center Position
extern int g_CX;
extern int g_CY;

// MyChar Position
extern int g_MX;
extern int g_MY;

// Gamer Position
extern int g_GX;
extern int g_GY;


class CGameView : public CWnd
{
// Attributes
public:
	CIME  Ime;
	CPage Page, BatPage;


	// ### [ Frame++ ] ###
	MMRESULT m_hEventTimer;
	// 동기화
	//CThreadTimer RefreshTimer;
	CSyncFPS syncfps;



	BOOL bIsPlay;
	HCURSOR hCurSel, hCurHand, hCurMove, hCurSize, hCurWait;

	BOOL bSetInvalidate;
	// 폰트
	CFont Font1, Font2;
	// 그래픽 버튼 클래스
	CGraphButton ExitBtn;
	CGraphButton InviteBtn;
	CGraphButton ChangeDisplayBtn;
	CGraphButton OptionBtn;
	CGraphButton MasterBtn;		//[62]
	CGraphButton BanishvoteBtn; //[62]
	CGraphButton MinimizeBtn;
	CGraphButton GameItemBtn;

//	CGraphButton SortBtn;

	// 게임진행 버튼 클래스
	CGraphButton	GameStartBtn;
	CGraphButton	DieBtn;
	CGraphButton	CheckBtn;
	CGraphButton	CallBtn;
	CGraphButton	PingBtn;
	CGraphButton	DadangkBtn;
	CGraphButton	MaxBtn;
	CGraphButton	AllInBtn;
	
	CGraphButton	CaptureBtn;// [화면캡쳐]
	// ### [ 관전기능 ] ###
	CGraphButton	ObserverViewBtn;
	CGraphButton	sInOut_Btn;

	//62cut button
	CGraphButton	ChangeCard_OK;
	CGraphButton	ChangeCard_Cancel;

	// 정보창 클래스
	//CInfoBox InfoBox;
	CChatBox ChatBox;
	// 플레이어 뷰 클래스
	CCharBox CharBox[6]; // 7
	// 마우스 상태변수
	BOOL bLClick;
	// 나가기 예약 상태
	BOOL bExitSubscript;

	// 베팅 칩 판  (화면좌표)
	int  m_betpage_x;
	int  m_betpage_y;
	int  m_diameter; //지름
	int  m_betpagesize_x;
	int  m_betpagesize_y;
	xSprite *m_pgamebackspr;
	xSprite *m_pcarddeckspr;

	// [ 튤팁 플레그 ]
	CString m_BtnMsgTip; // 1 다이 2 체크 3 콜 4 핑 5 따당 6 맥스
	int TipX;
	int TipY;

	// [ 세븐 카드 ]
	CString m_titlestr;
	BOOL m_bBPing,m_bBCheck,m_bBCall,m_bBMax,m_bBDDa,m_bAllin;
	void OnBtnCall();
	void OnBtnCheck();
	void OnBtnFold();
	void OnBtnMax();
	void OnBtnDda();
	void OnBtnPing();
	void OnAutoBatBtn();
	void OnAllInBtn();

	BOOL OnReSetResourceFile();



public:

	void pagebutton_init();
	void gameview_changeinitpos();
	void all_change_initpos(int bplayer);

	// ### [ Frame++ ] ###
	void MessagePump();

	CGameView();
	void Reset();
	void SetInvalidate();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameView)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	// ### [ 관전기능 ] ###
	void SetObserverBtn();
	void Emerge_Out();
	void Participation_In();

	void SetBtnState(BOOL bDisable=TRUE);
	void ClearFlag();
	virtual ~CGameView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGameView)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMEVIEW_H__6C3FEBA4_E463_11D3_97A2_0050BF0FBE67__INCLUDED_)
