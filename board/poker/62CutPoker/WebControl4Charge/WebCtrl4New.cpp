// WebCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "WebCtrl4New.h"
#include "../Global.h"
#include "mshtml.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// for mshtml

#pragma warning(push)
#pragma warning(disable: 4099)
#pragma warning(disable: 4146)
#pragma warning(disable: 4192)
#import <mshtml.tlb>
#pragma warning(default: 4192)
#pragma warning(default: 4049)
#pragma warning(default: 4146)
#pragma warning(pop)

/////////////////////////////////////////////////////////////////////////////
// CWebCtrl
IMPLEMENT_DYNCREATE(CWebCtrl4New, CHtmlView)

CWebCtrl4New::CWebCtrl4New()
{
	//{{AFX_DATA_INIT(CWebCtrl)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bNaviate = FALSE;
	m_bHideBorder = TRUE;
}

CWebCtrl4New::~CWebCtrl4New()
{
}

void CWebCtrl4New::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWebCtrl)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWebCtrl4New, CHtmlView)
	//{{AFX_MSG_MAP(CWebCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebCtrl diagnostics

#ifdef _DEBUG
void CWebCtrl4New::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CWebCtrl4New::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG


void CWebCtrl4New::SetOption(BOOL bHideBorder)
{
	m_bHideBorder = bHideBorder;
}


void CWebCtrl4New::SetBorderNone()
{
	MSHTML::IHTMLDocument2Ptr doc = (MSHTML::IHTMLDocument2Ptr)GetHtmlDocument();
	if(doc!=NULL) {
		MSHTML::IHTMLElementPtr body = (MSHTML::IHTMLElementPtr)doc->body;      
		if(body!=NULL)	body->style->border = _T("none");
	}
}


/////////////////////////////////////////////////////////////////////////////
// CWebCtrl message handlers
void CWebCtrl4New::OnBeforeNavigate2( LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel )
{
	CString MacroString = lpszURL;

	if(MacroString.Find("_충전소가기_",0)>=0) 
	{
		*pbCancel = TRUE;
		::SendMessage(g_pMainDlg->m_hWnd,WM_ALLINMESSAGE,ACD_GO_CHARGE,(DWORD)GetParent()->m_hWnd);
	} else 
	if(MacroString.Find("_리더스_",0)>=0) 
	{
		*pbCancel = TRUE;
		::SendMessage(g_pMainDlg->m_hWnd,WM_ALLINMESSAGE,ACD_GO_LEADERS,(DWORD)GetParent()->m_hWnd);
	} else 
	if(MacroString.Find("_수호천사자세히보기_",0)>=0) 
	{
		*pbCancel = TRUE;
		::SendMessage(g_pMainDlg->m_hWnd,WM_ALLINMESSAGE,ACD_ALLIN_INSURANCE,(DWORD)GetParent()->m_hWnd);
	} else 
	if(MacroString.Find("_천하무적구매하기_",0)>=0) 
	{
		*pbCancel = TRUE;
		::SendMessage(g_pMainDlg->m_hWnd,WM_ALLINMESSAGE,ACD_INVINCIBLE_BUY,(DWORD)GetParent()->m_hWnd);
	} else 
	if(MacroString.Find("_천하무적자세히보기_",0)>=0) 
	{
		*pbCancel = TRUE;
		::SendMessage(g_pMainDlg->m_hWnd,WM_ALLINMESSAGE,ACD_INVINCIBLE_DETAILS,(DWORD)GetParent()->m_hWnd);
	} else

	if(MacroString.Find("_수호천사구매하기_",0)>=0) 
	{
		*pbCancel = TRUE;
		::SendMessage(g_pMainDlg->m_hWnd,WM_ALLINMESSAGE,ACD_ALLIN_INSURANCE_BUY,(DWORD)GetParent()->m_hWnd);
	} else

	if(MacroString.Find("_수호천사사용하기_",0)>=0) 
	{
		*pbCancel = TRUE;
		::SendMessage(g_pMainDlg->m_hWnd,WM_ALLINMESSAGE,ACD_ALLIN_INSURANCE_USE,(DWORD)GetParent()->m_hWnd);
		
	} else

	if(MacroString.Find("_게임종료_",0)>=0) 
	{
		*pbCancel = TRUE;
		::SendMessage(g_pMainDlg->m_hWnd,WM_ALLINMESSAGE,ACD_CLOSEWINDOW,(DWORD)GetParent()->m_hWnd);
	}

	if(!m_bNaviate) {
		m_bNaviate = TRUE;
	} 
	else {
		*pbCancel = TRUE;
	}

	CHtmlView::OnBeforeNavigate2(lpszURL,nFlags,lpszTargetFrameName,baPostedData,lpszHeaders,pbCancel );
}


void CWebCtrl4New::OnNavigateComplete2( LPCTSTR strURL )
{
	if(m_bHideBorder) SetBorderNone();
	CHtmlView::OnNavigateComplete2(strURL );
}
