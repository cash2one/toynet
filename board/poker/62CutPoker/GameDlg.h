#if !defined(AFX_GAMEDLG_H__20C4F9CA_0FEE_11D4_97A2_0050BF0FBE67__INCLUDED_)
#define AFX_GAMEDLG_H__20C4F9CA_0FEE_11D4_97A2_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GameDlg.h : header file
//

#include "GameView.h"
#include "InviteDlg.h"
// ### [ 관전기능 ] ###
#include "ObserverDlg.h"


/////////////////////////////////////////////////////////////////////////////
// CGameDlg dialog

#define RESETGAME_TIMER 323


class CGameDlg : public CDialog
{
// Construction
public:
	CMyBitmap Back;					// 배경 비트맵
	CGameView m_GameView;			// 게임뷰 윈도우
	CInviteDlg m_InviteDlg;			// 초대하기 다이얼로그
	UINT hResetGameTimer;

	// ### [ 관전기능 ] ###
	CObserverDlg m_ObserverDlg;

	// 상태변수
	BOOL bAbnormalOutRoom;			// 비정상적으로 퇴실해야하는가?

	CGameDlg(CWnd* pParent = NULL);   // standard constructor
	void ExitGame();
	void SetResetGameTimer();
	void ClearResetGameTimer();
	void OnExitGame();
//	void SetButtonState();


// Dialog Data
	//{{AFX_DATA(CGameDlg)
	enum { IDD = IDD_GAME_DIALOG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGameDlg)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMEDLG_H__20C4F9CA_0FEE_11D4_97A2_0050BF0FBE67__INCLUDED_)
