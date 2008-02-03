#if !defined(AFX_NEWASKCHARGEDLG_H__CF792989_1B69_40CC_93FA_2F6F8A79FEA4__INCLUDED_)
#define AFX_NEWASKCHARGEDLG_H__CF792989_1B69_40CC_93FA_2F6F8A79FEA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewAskChargeDlg.h : header file
//

enum {
	ACD_GO_CHARGE = 10,			// ���� ������ �̵�
	ACD_GO_LEADERS,				// ������ ������ �̵�
	ACD_ALLIN_INSURANCE,		// ���� ���� �ڼ��� ���� 
	ACD_ALLIN_INSURANCE_USE,	// ���� ���� ����ϱ� 
	ACD_ALLIN_INSURANCE_BUY,	// ���� ���� �����ϱ� 
	ACD_INVINCIBLE_BUY,			// õ�Ϲ��� �����ϱ� 
	ACD_INVINCIBLE_DETAILS,		// õ�Ϲ��� �ڼ��� ���� 
	ACD_CLOSEWINDOW,			// â�ݱ�
};


/////////////////////////////////////////////////////////////////////////////
// CNewAskChargeDlg dialog
#include "MyBitmap.h"
#include"./WebControl4Charge/WebCtrl4New.h"
class CNewAskChargeDlg : public CDialog
{
// Construction
public:
	CNewAskChargeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewAskChargeDlg)
	enum { IDD = IDD_NEW_ASKCHARGEDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewAskChargeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CNewAskChargeDlg)
		// NOTE: the ClassWizard will add member functions here
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


protected:
	CMyBitmap Back;
	CWebCtrl4New *m_WebControl;				// ����Ʈ�� 
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWASKCHARGEDLG_H__CF792989_1B69_40CC_93FA_2F6F8A79FEA4__INCLUDED_)
