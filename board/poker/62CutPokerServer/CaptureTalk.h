#if !defined(AFX_CAPTURETALK_H__86EA38F0_1A2F_43DC_B8E6_ADC451D9753F__INCLUDED_)
#define AFX_CAPTURETALK_H__86EA38F0_1A2F_43DC_B8E6_ADC451D9753F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CaptureTalk.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCaptureTalk dialog

class CCaptureTalk : public CDialog
{
// Construction
public:
	
	void InitList();
	void InsItem(CString strID);
	void DelItem(CString strID);

	CCaptureTalk(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCaptureTalk)
	enum { IDD = IDD_CAPTURE_DIG };
	CListCtrl	m_ctrlUserID;
	CString	m_strID;
	BOOL	m_bUse;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCaptureTalk)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCaptureTalk)
	afx_msg void OnDblclkListUser(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDelete();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListUser(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAPTURETALK_H__86EA38F0_1A2F_43DC_B8E6_ADC451D9753F__INCLUDED_)
