#if !defined(AFX_OPTIONDLG_H__9699A0F7_8371_4B3F_9309_18CEE050DA96__INCLUDED_)
#define AFX_OPTIONDLG_H__9699A0F7_8371_4B3F_9309_18CEE050DA96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionDlg.h : header file
//

#include "MyButton.h"
#include "MyBitmap.h"
#include "BmpCheckButton.h"
#include "BmpRadioBtn.h"

/////////////////////////////////////////////////////////////////////////////
// COptionDlg dialog
#define MAX_MACRO_NUM 10
#define MODE_OPTION 0
#define MODE_MACRO  1

#define IDC_EDIT_MACRO					1511

class COptionDlg : public CDialog
{
// Construction
public:
	CMyBitmap	Back;
	CBrush		m_HollowBrush;
	CImageList	m_CheckImage;

	int m_ManSndKind;		//남자 사운드 종류
	int m_FemaleSndKind;	//여자 사운드 종류

	BOOL	m_bOffSndFX;
	BOOL	m_bOffMusic;
	int		m_nScreenMode;
	BOOL	m_bNoWhisper;
	BOOL	m_bNoInvite;	

	COptionDlg(CWnd* pParent = NULL);   // standard constructor
	void InitList();
	void AddID();

////////////////////////////
public://메크로 관련 
	CEdit     m_Macroedit[MAX_MACRO_NUM];
	char 	  m_DisPlayMode;

	void	  OnChangeView(char mode);

// Dialog Data
	//{{AFX_DATA(COptionDlg)
	enum { IDD = IDD_OPTION_DIALOG };
	CBmpRadioBtn	m_ManSnd3;
	CBmpRadioBtn	m_FemaleSnd3;
	CBmpRadioBtn	m_StartWindowScreenMode;
	CBmpRadioBtn	m_StartFullScreenMode;
	CBmpRadioBtn	m_ManSnd2;
	CBmpRadioBtn	m_ManSnd1;
	CBmpRadioBtn	m_FemaleSnd2;
	CBmpRadioBtn	m_FemaleSnd1;
	CBmpCheckButton	m_OffSndFx;
	CBmpCheckButton	m_OffMusic;
	CBmpCheckButton	m_NoWhisper;
	CBmpCheckButton	m_NoInvite;
	CMyButton	m_CancelBtn;
	CMyButton	m_OKBtn;
	CListCtrl	m_IDList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClickListShutmouse(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONDLG_H__9699A0F7_8371_4B3F_9309_18CEE050DA96__INCLUDED_)
