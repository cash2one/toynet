#if !defined(AFX_MYBUTTON_H__72F0F8A8_E60B_11D3_97A2_0050BF0FBE67__INCLUDED_)
#define AFX_MYBUTTON_H__72F0F8A8_E60B_11D3_97A2_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyButton.h : header file
//

#include "mmsystem.h"
#include "MyBitmap.h"

typedef struct
{
	BOOL bInit;
	int x;
	int y;
	int cx;
	int cy;
} BTNCLIPINFO;

/////////////////////////////////////////////////////////////////////////////
// CMyButton window

class CMyButton : public CButton
{
// Construction
public:
	CMyButton();

// Attributes
public:
	BOOL bEnable;
	BOOL bCaptured;
	BOOL bClicked;
	int nState;

	BOOL bOneClip;
	BTNCLIPINFO Clip[4];

	BOOL bTransColor;
	COLORREF TransColor;

	CMyBitmap BmpOne;	// 통짜 모드일 경우
	CMyBitmap Bmp1, Bmp2, Bmp3, Bmp4; // 첫번째 버튼 그림, 마우스가 위를 지날때, 눌려졌을때, Disable되었을때
	CString strWav;

	// 사용자 멋대로 쓰는 임시 플래그
	int iUser1;
	int iUser2;
	int iUser3;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyButton)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual BOOL EnableWindow( BOOL bEnable = TRUE );
	void Init(UINT bmp1, UINT bmp2, UINT bmp3, UINT bmp4, char *wavfile=NULL);
	void Init(LPCTSTR filename, int imagenum, LPCTSTR wavfile=NULL);
	void Init(int xp, int yp, LPCTSTR filename, int imagenum, LPCTSTR wavfile=NULL);
	void SetTransColor(COLORREF color);

	void Draw(CDC& dc);

	virtual ~CMyButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyButton)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYBUTTON_H__72F0F8A8_E60B_11D3_97A2_0050BF0FBE67__INCLUDED_)
