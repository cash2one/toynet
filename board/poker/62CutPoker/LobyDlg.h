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
#include "AdminTitleDlg.h"// [������ ��� �۾�]
#include "LevelView.h"
#include "LobbyListWnd.h"	// ### [ ���� ����Ʈ ���� ] ###

/////////////////////////////////////////////////////////////////////////////
// CLobyDlg dialog

#define LOBY_TIMER 101

class CLobyDlg : public CDialog
{
// Construction
public:
	void CreateRoom();
	CMyBitmap Back;			// ��� ��Ʈ��
	CMsgDlg m_MsgDlg;		// �� ���� ��ȭ����
	CCharView m_CharView;	// ĳ���� ��
	CMenu m_RoomMenu;		// �˾� �޴�

	CKHSToolTip BadUserTip;	// 112�Ű��ư ����

	CString m_LobyMsg;

	// [ ���� ��Ŀ ] ���� �׽�Ʈ
	CLevelView m_JackPotBox;

	CImageList m_RoomImage;
	CImageList m_UserImage;

	int iAccessRoomState;	// �濡 ���� Ȥ�� ���� ������ ��Ÿ��
	int nRoomSel, nUserSel;

	BOOL bDontClickSound;

	BOOL m_bFullScr;

	int  m_ChoiceKind;//0: all 1 :normal 2: choice 

	int	 m_nLobyCnt;
	
	//////////////////////////////////////////////////
	// ### [ ���� ����Ʈ ���� ] ###
	CLobbyListWnd		m_RoomView;		//	������

	//////////////////////////////////////////////////
	//////////////////////////////////////////////////

public:
	CLobyDlg(CWnd* pParent = NULL);   // standard constructor

	void DrawBkgnd(CDC& dc);
	void RefreshList();

	//void Room_InitList();
	//void Room_InsItem(int roomnum, char *master, char *title, int level, int nowuser, int maxuser,  int observer, int secret, int state, int bat ,BOOL bSuperRoom);// ### [ ������� ] ###
	// [������ ��� �۾�]
	//void Room_SetItem(int roomnum, int gamekind, char *master, int nowuser, int maxuser, int observer, int state, char *title="", BOOL bTitle = FALSE);// ### [ ������� ] ###
	//void Room_SetItem(SMALLROOMINFO *pSRoom ,BOOL bTitle = FALSE);

	// ### [ ���� ����Ʈ ���� ] ###
	void Room_InsItem(ROOMINFO *pRoom);
	void Room_SetItem(void *pRoom, BOOL bTitle= FALSE) ;
	void Room_DelItem(int roomnum);

	void User_InitList();
	void User_InsItem(int unum, char *id, BOOL sex, int winnum, int loosenum, int drawnum, int point, int position, int roomnum, int level );
	void User_SetItem(int unum, char *id, int winnum, int loosenum, int drawnum, int point, int position, int roomnum, int level );
	void User_DelItem(int unum, char *id);

	//[������ ��� �۾�]
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
