// 62CutPokerDlg.h : header file
//

#if !defined(AFX_62CutPokerDLG_H__D0755936_9389_4213_9DD2_6515CB6ABE68__INCLUDED_)
#define AFX_62CutPokerDLG_H__D0755936_9389_4213_9DD2_6515CB6ABE68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "62CutPoker.h"
#include "SockCmnd.h"
#include "MsgDlg.h"
#include "TitleDlg.h"
#include "LobyDlg.h"
#include "GameDlg.h"
#include "MyButton.h"

#include "GameItemDlg.h"	// [게임 아이템 작업]
#include "PremServiceDlg.h"	// [PC방 작업]

#include "AdminMessageDlg.h"
#include "MsgBoxDlg.h"
#include "Oring.h"			// [ 올인잡업 ]
#include "NewAskChargeDlg.h" // [수호천사] 2004.07.08

// [수호천사] 2004.07.08
#define 	WM_ALLINMESSAGE			(WM_USER+7510)	

#define BADUSER_TIMER	2132		// 신고 타이머
#define MAX_BADUSERDELAYTIME 60000	// 신고기능 딜레이 시간(1분)

#define CAPTIONBAR_HEIGHT 28

#define  WM_TRAYICON_MSG WM_USER + 1

typedef struct
{
	BOOL bOffSndFX;
	BOOL bOffMusic;
	int  nScreenMode;
	BOOL bNoInvite;
	BOOL bNoWhisper;
	BOOL bFailToFullScreen;
	//	음성 구분(느끼버전, 섹시버전)	
	int  SndFxKind; // ### [사운드 추가 작업] ###
	BOOL bHideOtherBlock;
	int  MoneyMark; // 머니 표기 숫자, 한글	
}CONFIG;

/////////////////////////////////////////////////////////////////////////////
// C62CutPokerDlg dialog

class C62CutPokerDlg : public CDialog
{
// Construction
public:
	BOOL m_bExamServer; // 연습게임서버에 접속했다!!
	int m_nItemServer; // 아이템게임서버에 접속했다!! [게임 아이템 작업] [점프 아이템 작업]

	UINT hTimerBadUser;			// 112 신고 타이머
	CONFIG CFG;
	CSockCmnd m_SockCmnd;		// 소켓 메세지 처리 클래스
	
	CMsgDlg m_ConnectMsgDlg;	// 접속 표시 대화상자
	CMsgDlg m_LogoutMsgDlg;		// 로그아웃 표시 대화상자

	CGameItemDlg	m_GameItemDlg;		// 게임 아이템 대화상자	[게임 아이템 작업]
	CPremServiceDlg	m_PremServiceDlg;	// 프리미엄 PC방 표시 대화상자 [PC방 작업]

	CTitleDlg m_TitleDlg;		// 타이틀 다이얼로그
	CLobyDlg m_LobyDlg;			// 대기실 다이얼로그
	CGameDlg m_GameDlg;			// 게임 다이얼로그

	CAdminMessageDlg AMesDlg;     // [관리자 모드 작업] 관리자 메시지

	BOOL bNowLogout;			// 지금 로그아웃중인가?
	CMsgBoxDlg m_MsgBoxDlg;
	COring m_Oringdlg;

public:	
	C62CutPokerDlg(CWnd* pParent = NULL);	// standard constructor
	BOOL AnalyzeArgument(char *key);	// 인자 스트링 분석
	BOOL LoadConfig(LPCTSTR id);
	BOOL SaveConfig(LPCTSTR id);
	void SetupByConfig();
	void ResetBadUserTimer();
	void KillBadUserTimer();
	void ChangeView(int vnum);

	void CreateShortCut();
	void BrowserMove(LPCTSTR targeturl);	// [게임 아이템 작업]
	void ShowItemState();					// [게임 아이템 작업]
	void ShowMyItemBag();					// [게임 아이템 작업]
	void URLEncode(char* output, char* input);	// [PC방 작업]
	void DoScreenCapture(); //화면캡쳐
	void LoadSpriteFile();// ### [ 관전기능 ] ###
	void load_spritefile_7();
	void Reload_spritefile();

	void RegistTrayIcon();
	void KillProcess();

// Dialog Data
	//{{AFX_DATA(C62CutPokerDlg)
	enum { IDD = IDD_62CUTPOKER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C62CutPokerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL

// Implementation
public:
	
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(C62CutPokerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg LONG OnPacketNotify(UINT, LONG);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LONG OnAllinMessage(UINT, LONG);// [수호천사] 2004.07.08
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg LRESULT TrayIconMsg( WPARAM wParam, LPARAM lParam );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_62CutPokerDLG_H__D0755936_9389_4213_9DD2_6515CB6ABE68__INCLUDED_)
