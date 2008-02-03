// WebCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "WebCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
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
*/

/////////////////////////////////////////////////////////////////////////////
// CWebCtrl

IMPLEMENT_DYNCREATE(CWebCtrl, CHtmlView)

CWebCtrl::CWebCtrl()
{
	//{{AFX_DATA_INIT(CWebCtrl)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	//m_bHideBorder = FALSE;
}

CWebCtrl::~CWebCtrl()
{
}

void CWebCtrl::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWebCtrl)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWebCtrl, CHtmlView)
	//{{AFX_MSG_MAP(CWebCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebCtrl diagnostics

#ifdef _DEBUG
void CWebCtrl::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CWebCtrl::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

/*
void CWebCtrl::SetOption(BOOL bHideBorder)
{
	m_bHideBorder = bHideBorder;
}

BOOL CWebCtrl::InsertHTML(LPCTSTR strHtml)
{
	MSHTML::IHTMLDocument2Ptr doc = (MSHTML::IHTMLDocument2Ptr)GetHtmlDocument();
	if(doc == NULL) return FALSE;
	MSHTML::IHTMLElementPtr body = (MSHTML::IHTMLElementPtr)doc->body;      
	if(body == NULL) return FALSE;

	body->insertAdjacentHTML("BeforeEnd", strHtml);

	return TRUE;
}

BOOL CWebCtrl::ExecJavascript(LPCTSTR strScript)
{
	//SAMPLE: char* strScript = "document.Send.submit();";

	MSHTML::IHTMLDocument2Ptr doc = (MSHTML::IHTMLDocument2Ptr)GetHtmlDocument();
	if(doc == NULL) return FALSE;
	doc->parentWindow->execScript(strScript, "JavaScript");

	return TRUE;
}

BOOL CWebCtrl::PutCharSet(LPCTSTR strCharset)
{
	//SAMPLE: char* strCharset = "euc-kr";

	WCHAR wstrCSet[256] = {0,};
	MultiByteToWideChar(CP_ACP, MB_COMPOSITE, strCharset, strlen(strCharset), wstrCSet, 255);

	MSHTML::IHTMLDocument2Ptr doc = (MSHTML::IHTMLDocument2Ptr)GetHtmlDocument();
	if(doc == NULL) return FALSE;

	doc->put_charset(wstrCSet);

	return TRUE;
}

void CWebCtrl::SetBorderNone()
{
	MSHTML::IHTMLDocument2Ptr doc = (MSHTML::IHTMLDocument2Ptr)GetHtmlDocument();
	if(doc!=NULL) {
		MSHTML::IHTMLElementPtr body = (MSHTML::IHTMLElementPtr)doc->body;      
		if(body!=NULL)	body->style->border = _T("none");
	}
}
*/

/////////////////////////////////////////////////////////////////////////////
// CWebCtrl message handlers

/*
void CWebCtrl::OnBeforeNavigate2( LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel )
{
	if(!m_bNaviate) {
		m_bNaviate = TRUE;
	} 
	else {
		*pbCancel = TRUE;
		::SendMessage(pMainDlg->m_hWnd,WM_CC_OPENBOARD,(WPARAM)lpszURL,47);
	}

	CHtmlView::OnBeforeNavigate2(lpszURL,nFlags,lpszTargetFrameName,baPostedData,lpszHeaders,pbCancel );
}
*/

void CWebCtrl::OnNavigateComplete2( LPCTSTR strURL )
{
	//if(m_bHideBorder) 
	//	SetBorderNone();

	CHtmlView::OnNavigateComplete2(strURL );
}
