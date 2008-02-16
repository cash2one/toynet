#if !defined(AFX_TITLEDLG_H__20C4F9C8_0FEE_11D4_97A2_0050BF0FBE67__INCLUDED_)
#define AFX_TITLEDLG_H__20C4F9C8_0FEE_11D4_97A2_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TitleDlg.h : header file
//

#include "ListCtrlEx.h"
#include "ChatViewEdit.h"
#include "MyButton.h"
#include "MyBitmap.h"
#include "KHSToolTip.h"
#include "MsgDlg.h"
#include "WebCtrl.h"

#include "LevelView.h"

/////////////////////////////////////////////////////////////////////////////
// CTitleDlg dialog

class CTitleDlg : public CDialog
{
// Construction
public:
	
	CWebCtrl *m_pLocWebCtrl;	// <- 추가

	void CreateBanner();		// <- 추가
	void DestroyBanner();		// <- 추가


	CMyBitmap Back;				// 배경 비트맵

	CImageList m_ChanImage;		// 채널 리스트용 아이콘 이미지
	CMsgDlg m_MsgDlg;
	CWebCtrl *m_pWebCtrl;

	CString m_TitleMsg; // 메세지


	// [ 세븐 포커 ] 잭팟 테스트
	CLevelView m_JackPotBox;

	BOOL bDontClickSound;
	int LastSelChan;

	CTitleDlg(CWnd* pParent = NULL);   // standard constructor

	void DrawBkgnd(CDC& dc);

	void Chan_InitList();
	void Chan_InsItem(int cnum, char *title, int nowuser, int maxuser, int channelstyle); 	// ### [ 채널 입장 구분 ] ###
	void Chan_DeleteList();
	void Chan_SelectLast();
	CString GetChannelName(int cnum);

// Dialog Data
	//{{AFX_DATA(CTitleDlg)
	enum { IDD = IDD_TITLE_DIALOG };
	CChatViewEdit	m_ChatViewNotic;
	CListCtrlEx	m_ChanList;
	CMyButton	m_ExitBtn;
	CMyButton	m_EnterBtn;
	CMyButton	m_RefreshBtn;
	CMyButton	m_ChangeDisplayBtn;
	CMyButton	m_OptionBtn;
	CMyButton	m_FindIDBtn;
	CString	m_strFindID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTitleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTitleDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDblclkListChannel(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonRefresh();
	afx_msg void OnButtonEnter();
	afx_msg void OnDestroy();
	afx_msg void OnButtonFind();
	afx_msg void OnButtonOption();
	afx_msg void OnButtonChangedisplay();
	afx_msg void OnButtonExit();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TITLEDLG_H__20C4F9C8_0FEE_11D4_97A2_0050BF0FBE67__INCLUDED_)
