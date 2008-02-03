#if !defined(AFX_CONFIGJACKPOTDLG_H__32F41A44_C5FF_472E_ADF6_ED427A998022__INCLUDED_)
#define AFX_CONFIGJACKPOTDLG_H__32F41A44_C5FF_472E_ADF6_ED427A998022__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigJackPotDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigJackPotDlg dialog

class CConfigJackPotDlg : public CDialog
{
// Construction
public:
	void SetControlState();
	void SetControlLog();
	CConfigJackPotDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigJackPotDlg)
	enum { IDD = IDD_CONFIG_DIALOG_JACKPOT };
	CString	m_JackPot;
	CString	m_iJack1;
	CString	m_iJack2;
	CString	m_iJack3;
	CString	m_iJack4;
	CString	m_iJack5;
	int		m_bJackPotMode;
	int		m_rChobo;
	int		m_rFree;
	int		m_rGosu;
	int		m_rJungSu;
	int		m_rGod;
	int		m_jackPotChobo;
	int		m_jackPotDosin;
	int		m_jackPotFree;
	int		m_jackPotGosu;
	int		m_jackPotJungSu;
	BOOL	m_bRealLogUse;
	int		m_nRange;
	BOOL	m_bSaveSetLog;
	int		m_jackPotChoboS;
	int		m_jackPotChoboF;
	int		m_jackPotJungSuF;
	int		m_jackPotJungSuS;
	int		m_jackPotGosuF;
	int		m_jackPotGosuS;
	int		m_jackPotDosinF;
	int		m_jackPotDosinS;
	int		m_jackPotFreeF;
	int		m_jackPotFreeS;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigJackPotDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigJackPotDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioJackpot();
	afx_msg void OnRadioJackpot1();
	afx_msg void OnCheckBlog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGJACKPOTDLG_H__32F41A44_C5FF_472E_ADF6_ED427A998022__INCLUDED_)
