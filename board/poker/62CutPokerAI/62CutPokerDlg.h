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

#include "GameItemDlg.h"	// [���� ������ �۾�]
#include "PremServiceDlg.h"	// [PC�� �۾�]

#include "AdminMessageDlg.h"
#include "MsgBoxDlg.h"
#include "Oring.h"			// [ ������� ]
#include "NewAskChargeDlg.h" // [��ȣõ��] 2004.07.08

// [��ȣõ��] 2004.07.08
#define 	WM_ALLINMESSAGE			(WM_USER+7510)	

#define BADUSER_TIMER	2132		// �Ű� Ÿ�̸�
#define MAX_BADUSERDELAYTIME 60000	// �Ű��� ������ �ð�(1��)

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
	//	���� ����(��������, ���ù���)	
	int  SndFxKind; // ### [���� �߰� �۾�] ###
	BOOL bHideOtherBlock;
	int  MoneyMark; // �Ӵ� ǥ�� ����, �ѱ�	
}CONFIG;

/////////////////////////////////////////////////////////////////////////////
// C62CutPokerDlg dialog

class C62CutPokerDlg : public CDialog
{
// Construction
public:
	BOOL m_bExamServer; // �������Ӽ����� �����ߴ�!!
	int m_nItemServer; // �����۰��Ӽ����� �����ߴ�!! [���� ������ �۾�] [���� ������ �۾�]

	UINT hTimerBadUser;			// 112 �Ű� Ÿ�̸�
	CONFIG CFG;
	CSockCmnd m_SockCmnd;		// ���� �޼��� ó�� Ŭ����
	
	CMsgDlg m_ConnectMsgDlg;	// ���� ǥ�� ��ȭ����
	CMsgDlg m_LogoutMsgDlg;		// �α׾ƿ� ǥ�� ��ȭ����

	CGameItemDlg	m_GameItemDlg;		// ���� ������ ��ȭ����	[���� ������ �۾�]
	CPremServiceDlg	m_PremServiceDlg;	// �����̾� PC�� ǥ�� ��ȭ���� [PC�� �۾�]

	CTitleDlg m_TitleDlg;		// Ÿ��Ʋ ���̾�α�
	CLobyDlg m_LobyDlg;			// ���� ���̾�α�
	CGameDlg m_GameDlg;			// ���� ���̾�α�

	CAdminMessageDlg AMesDlg;     // [������ ��� �۾�] ������ �޽���

	BOOL bNowLogout;			// ���� �α׾ƿ����ΰ�?
	CMsgBoxDlg m_MsgBoxDlg;
	COring m_Oringdlg;

public:	
	C62CutPokerDlg(CWnd* pParent = NULL);	// standard constructor
	BOOL AnalyzeArgument(char *key);	// ���� ��Ʈ�� �м�
	BOOL LoadConfig(LPCTSTR id);
	BOOL SaveConfig(LPCTSTR id);
	void SetupByConfig();
	void ResetBadUserTimer();
	void KillBadUserTimer();
	void ChangeView(int vnum);

	void CreateShortCut();
	void BrowserMove(LPCTSTR targeturl);	// [���� ������ �۾�]
	void ShowItemState();					// [���� ������ �۾�]
	void ShowMyItemBag();					// [���� ������ �۾�]
	void URLEncode(char* output, char* input);	// [PC�� �۾�]
	void DoScreenCapture(); //ȭ��ĸ��
	void LoadSpriteFile();// ### [ ������� ] ###
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
	afx_msg LONG OnAllinMessage(UINT, LONG);// [��ȣõ��] 2004.07.08
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg LRESULT TrayIconMsg( WPARAM wParam, LPARAM lParam );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_62CutPokerDLG_H__D0755936_9389_4213_9DD2_6515CB6ABE68__INCLUDED_)
