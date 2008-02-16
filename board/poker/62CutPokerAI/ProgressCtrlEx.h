#if !defined(AFX_PROGRESSCTRLEX_H__594CBCF8_05CE_43EE_BF11_A0DFA6010E84__INCLUDED_)
#define AFX_PROGRESSCTRLEX_H__594CBCF8_05CE_43EE_BF11_A0DFA6010E84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgressCtrlEx.h : header file
//
#include "MyBitmap.h"
/////////////////////////////////////////////////////////////////////////////
// CProgressCtrlEx window

class CProgressCtrlEx : public CProgressCtrl
{
// Construction
public:
	CProgressCtrlEx();

	CMyBitmap bar;

public:


	void SetBkColor(COLORREF color) {m_clrBkGround = color;}
	void SetBarColor(COLORREF color) {m_clrBarColor = color;}
	COLORREF m_clrBkGround,m_clrBarColor;
	int MaxNum;				// 시간
	int start;				// 시작점 
	int mode;				// 그래픽 인지 아닌지
	int m_nCurrentPosition; // 현재위치 
	int SetPosEx(int npos);
	void SetRangeEx(int start, int max,int mode=0);
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressCtrlEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CProgressCtrlEx();

	// Generated message map functions
//protected:
	//{{AFX_MSG(CProgressCtrlEx)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSCTRLEX_H__594CBCF8_05CE_43EE_BF11_A0DFA6010E84__INCLUDED_)
