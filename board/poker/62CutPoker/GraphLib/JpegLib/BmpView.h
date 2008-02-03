#if !defined(AFX_BMPVIEW_H__F26A5234_E543_11D3_A41D_918C5F36946E__INCLUDED_)
#define AFX_BMPVIEW_H__F26A5234_E543_11D3_A41D_918C5F36946E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BmpView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBmpView view

class CBmpView : public CView
{
protected:
	DECLARE_DYNCREATE(CBmpView)

// Attributes
public:
	CBmpView();           // protected constructor used by dynamic creation
	virtual ~CBmpView();

// Operations
public:
	BOOL ShowBmp(CDC* pdc, int x, int y, CString filename);
	BOOL ShowBmp(CDC* pdc, int x, int y, CString filename, int maskcolor);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBmpView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CBmpView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMPVIEW_H__F26A5234_E543_11D3_A41D_918C5F36946E__INCLUDED_)
