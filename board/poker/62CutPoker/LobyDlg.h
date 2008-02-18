#if !defined(AFX_LOBYDLG_H__20C4F9C9_0FEE_11D4_97A2_0050BF0FBE67__INCLUDED_)
#define AFX_LOBYDLG_H__20C4F9C9_0FEE_11D4_97A2_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LobyDlg.h : header file
//

#include "ListCtrlEx.h"
#include "ChatViewEdit.h"
#include "SendEdit.h"
#include "MyButton.h"
#include "MsgDlg.h"
#include "CreateRoomDlg.h"
#include "CharView.h"
#include "MyBitmap.h"
#include "BannerWnd.h"
#include "KHSToolTip.h"
#include "AdminTitleDlg.h"// [관리자 모드 작업]
#include "LevelView.h"
#include "LobbyListWnd.h"	// ### [ 대기실 리스트 관련 ] ###

/////////////////////////////////////////////////////////////////////////////
// CLobyDlg dialog

#define LOBY_TIMER 101

class CLobyDlg : public CDialog
{
// Construction
public:
	void CreateRoom();
	CMyBitmap Back;			// 배경 비트맵
	CMsgDlg m_MsgDlg;		// 방 연결 대화상자
	CCharView m_CharView;	// 캐릭터 뷰
	CMenu m_RoomMenu;		// 팝업 메뉴

	CKHSToolTip BadUserTip;	// 112신고버튼 툴팁

	CString m_LobyMsg;

	// [ 세븐 포커 ] 잭팟 테스트
	CLevelView m_JackPotBox;

	CImageList m_RoomImage;
	CImageList m_UserImage;

	int iAccessRoomState;	// 방에 접속 혹은 생성 중인지 나타냄
	int nRoomSel, nUserSel;

	BOOL bDontClickSound;

	BOOL m_bFullScr;

	int  m_ChoiceKind;//0: all 1 :normal 2: choice 

	int	 m_nLobyCnt;
	
	//////////////////////////////////////////////////
	// ### [ 대기실 리스트 관련 ] ###
	CLobbyListWnd		m_RoomView;		//	방정보

	//////////////////////////////////////////////////
	//////////////////////////////////////////////////

public:
	CLobyDlg(CWnd* pParent = NULL);   // standard constructor

	void DrawBkgnd(CDC& dc);
	void RefreshList();

	//void Room_InitList();
	//void Room_InsItem(int roomnum, char *master, char *title, int level, int nowuser, int maxuser,  int observer, int secret, int state, int bat ,BOOL bSuperRoom);// ### [ 관전기능 ] ###
	// [관리자 모드 작업]
	//void Room_SetItem(int roomnum, int gamekind, char *master, int nowuser, int maxuser, int observer, int state, char *title="", BOOL bTitle = FALSE);// ### [ 관전기능 ] ###
	//void Room_SetItem(SMALLROOMINFO *pSRoom ,BOOL bTitle = FALSE);

	// ### [ 대기실 리스트 관련 ] ###
	void Room_InsItem(ROOMINFO *pRoom);
	void Room_SetItem(void *pRoom, BOOL bTitle= FALSE) ;
	void Room_DelItem(int roomnum);

	void User_InitList();
	void User_InsItem(int unum, char *id, BOOL sex, int winnum, int loosenum, int drawnum, int point, int position, int roomnum, int level );
	void User_SetItem(int unum, char *id, int winnum, int loosenum, int drawnum, int point, int position, int roomnum, int level );
	void User_DelItem(int unum, char *id);

	//[관리자 모드 작업]
	int  GetUserNum(CString strID);
	BOOL CLobyDlg::GetStyleRoomInfo(int rnum, STYLEROOMINFO &sri);
// Dialog Data
	//{{AFX_DATA(CLobyDlg)
	enum { IDD = IDD_LOBY_DIALOG };
	CMyButton	m_btnNormal;
	CMyButton	m_btnAllKind;
	CMyButton	m_btnChoice;
	CMyButton	m_CaptureBtn;
	CMyButton	m_ItemBagBtn;
	CMyButton	m_MiniBtn;
	CMyButton	m_QuickStart;
	CMyButton	m_Option;
	CMyButton	m_ExitLoby;
	CMyButton	m_CardShop;
	CMyButton	m_FindIDBtn;
	CBannerWnd	m_BannerWnd;
	CMyButton	m_BadUserBtn;
	CMyButton	m_ChangeDisplayBtn;
	CMyButton	m_RoomInfoBtn;
	CMyButton	m_EnterRoomBtn;
	CMyButton	m_CreateRoomBtn;
	CMyButton	m_ChangeProfileBtn;
	CMyButton	m_ViewProfileBtn;
	CMyButton	m_LogoutBtn;
	CListCtrlEx m_RoomList;
	CListCtrlEx m_UserList;
	CChatViewEdit m_ChatViewEdit;
	CSendEdit	m_SendEdit;
	BOOL	m_RefuseInviteCheck;
	BOOL	m_RefuseWhisper;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLobyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLobyDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDblclkListRoom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedListRoom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListUser(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedListUser(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	afx_msg void OnButtonEntergame();
	afx_msg void OnButtonCreateroom();
	afx_msg void OnButtonHompage();
	afx_msg void OnButtonLogout();
	afx_msg void OnButtonViewprofile();
	afx_msg void OnButtonChangeprofile();
	afx_msg void OnDestroy();
	afx_msg void OnButtonRoominfo();
	afx_msg void OnRclickListRoom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListUser(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonWhisper();
	afx_msg void OnButtonChangedisplay();
	afx_msg void OnButtonBaduser();
	afx_msg void OnButtonFindid();
	afx_msg void OnButtonCard();
	afx_msg void OnButtonOption();
	afx_msg void OnButtonExit1();
	afx_msg void OnButtonQuickstart();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonMinimize();
	afx_msg void OnButtonItembag();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnButtonScreencaptureloby();
	afx_msg void OnMenuAdmWarnUser();
	afx_msg void OnMenuAdmUserinfo();
	afx_msg void OnMenuAdmSendwhispher();
	afx_msg void OnCheckRefusewhisper();
	afx_msg void OnCheckRefuseinvite();
	afx_msg void OnButtonNormal();
	afx_msg void OnButtonChoice();
	afx_msg void OnButtonAllkind();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/*
afx_msg void OnMenuAdmInroom();
	afx_msg void OnMenuAdmMakeroom();
	afx_msg void OnMenuAdmRenameroom();
	afx_msg void OnMenuAdmRoomdel();
	afx_msg void OnMenuAdmRoominfo();
	afx_msg void OnMenuAdmRoommaster();
*/

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOBYDLG_H__20C4F9C9_0FEE_11D4_97A2_0050BF0FBE67__INCLUDED_)
