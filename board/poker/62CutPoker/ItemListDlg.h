#if !defined(AFX_ITEMLISTDLG_H__ED7A6314_2F77_415D_B03F_F9138CCDAB90__INCLUDED_)
#define AFX_ITEMLISTDLG_H__ED7A6314_2F77_415D_B03F_F9138CCDAB90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ItemListDlg.h : header file
//

#include "MyButton.h"
#include "ListCtrlEx.h"
#include "UserStruct.h"

/////////////////////////////////////////////////////////////////////////////
// CItemListDlg dialog

class CItemListDlg : public CDialog
{
// Construction
public:
	CItemListDlg(CWnd* pParent = NULL);   // standard constructor

	/////// member variables

	CBrush		m_HollowBrush;
	CImageList	m_ItemImage;

	CString		m_StrMsg;
	BOOL		m_bItemUsed;
	int			m_DefSelItemKind;

	/////// static members

	static BOOL m_bNowAskRefresh;

	/////// member functions

	void InitList();
	void InsertItem(GAMEITEM *pItem);
	void DrawAll(CDC& dc);
	void DrawGameItem(CDC& dc);
	BOOL SelectUsableGameItem(int itemcode);
	int  GetSelectedItemIndex();
	BOOL UseSelectedGameItem();
	void AskRefresh();
	void RefreshList();
	void RebuildItemList();

// Dialog Data
	//{{AFX_DATA(CItemListDlg)
	enum { IDD = IDD_ITEMLIST_DIALOG };
	CMyButton	m_OkBtn;
	CListCtrlEx	m_ItemList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CItemListDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg void OnClickListGameitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListGameitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEMLISTDLG_H__ED7A6314_2F77_415D_B03F_F9138CCDAB90__INCLUDED_)
