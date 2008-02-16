#if !defined(AFX_GAMEITEMDLG_H__E5C4CA05_0F66_4527_B437_5AF6AF8FC32F__INCLUDED_)
#define AFX_GAMEITEMDLG_H__E5C4CA05_0F66_4527_B437_5AF6AF8FC32F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GameItemDlg.h : header file
//

#include "ItemWebCtrl.h"
#include "ItemListDlg.h"
#include "WebCtrl.h"
#include "MyButton.h"



/////////////////////////////////////////////////////////////////////////////
// CGameItemDlg dialog

class CGameItemDlg : public CDialog
{
// Construction
public:
	CGameItemDlg(CWnd* pParent = NULL);   // standard constructor
	~CGameItemDlg();

	BOOL			m_bModal;

	CWebCtrl		*m_pHelpWeb;
	CItemListDlg	m_ItemListDlg;	
//	CItemWebCtrl	*m_pShopWeb;

	int				m_CurrentTab;
	int				m_FirstTab;
	int				m_DefShopItemKind;

	BOOL			m_bRefreshList;
	BOOL			m_bHelpWebNavigated;
	BOOL			m_bShopWebNavigated;

	BOOL DoModaless(int firsttab=0, BOOL bRefreshList=TRUE, int iDefSelItemKind=0);
	void OpenShopPage();
	void ItemRefresh();
	void ChangeTab(int tabno);
	BOOL SafeClose();

// Dialog Data
	//{{AFX_DATA(CGameItemDlg)
	enum { IDD = IDD_GAMEITEM_DIALOG };
	CMyButton	m_ShopBtn;
	CMyButton	m_ItemBtn;
	CMyButton	m_HelpBtn;
	CMyButton	m_CashBtn;
	CMyButton	m_CancelBtn;
	CGameItemDlg*	a;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameItemDlg)
	public:
	virtual int DoModal(int firsttab=0, BOOL bRefreshList=TRUE, int iDefSelItemKind=0);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGameItemDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonHelp();
	afx_msg void OnButtonItem();
	afx_msg void OnButtonShop();
	afx_msg void OnButtonCash();
	afx_msg void OnPaint();
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMEITEMDLG_H__E5C4CA05_0F66_4527_B437_5AF6AF8FC32F__INCLUDED_)
