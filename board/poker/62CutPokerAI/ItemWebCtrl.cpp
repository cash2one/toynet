// ItemWebCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ItemWebCtrl.h"

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
// CItemWebCtrl

IMPLEMENT_DYNCREATE(CItemWebCtrl, CHtmlView)

CItemWebCtrl::CItemWebCtrl()
{
	m_bHideBorder = FALSE;
	m_NavigatedCount = 0;

	//{{AFX_DATA_INIT(CItemWebCtrl)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CItemWebCtrl::~CItemWebCtrl()
{
}

void CItemWebCtrl::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CItemWebCtrl)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CItemWebCtrl, CHtmlView)
	//{{AFX_MSG_MAP(CItemWebCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemWebCtrl diagnostics

#ifdef _DEBUG
void CItemWebCtrl::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CItemWebCtrl::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

void CItemWebCtrl::SetOption(BOOL bHideBorder)
{
	m_bHideBorder = bHideBorder;
}

BOOL CItemWebCtrl::_InsertHTML()
{
	MSHTML::IHTMLDocument2Ptr doc = (MSHTML::IHTMLDocument2Ptr)GetHtmlDocument();
	if(doc == NULL) return FALSE;
	MSHTML::IHTMLElementPtr body = (MSHTML::IHTMLElementPtr)doc->body;      
	if(body == NULL) return FALSE;

	body->insertAdjacentHTML("BeforeEnd", (LPCTSTR)m_strInsHtml);

	m_strInsHtml = "";

	return TRUE;
}

BOOL CItemWebCtrl::InsertHTML(LPCTSTR strHtml)
{
	m_strInsHtml += strHtml;
	if(GetReadyState() != READYSTATE_COMPLETE) 
		return FALSE;
	return _InsertHTML();
}

BOOL CItemWebCtrl::_ExecJavascript()
{
	MSHTML::IHTMLDocument2Ptr doc = (MSHTML::IHTMLDocument2Ptr)GetHtmlDocument();
	if(doc == NULL) return FALSE;
	doc->parentWindow->execScript((LPCTSTR)m_strRunScript, "JavaScript");

	m_strRunScript = "";

	return TRUE;
}

BOOL CItemWebCtrl::ExecJavascript(LPCTSTR strScript)
{
	//SAMPLE: char* strScript = "document.Send.submit();";
	m_strRunScript += strScript;
	if(GetReadyState() != READYSTATE_COMPLETE) 
		return FALSE;
	return _ExecJavascript();
}

BOOL CItemWebCtrl::_PutCharSet()
{
	WCHAR wstrCSet[256] = {0,};
	MultiByteToWideChar(CP_ACP, MB_COMPOSITE, m_strCharset, strlen(m_strCharset), wstrCSet, 255);

	MSHTML::IHTMLDocument2Ptr doc = (MSHTML::IHTMLDocument2Ptr)GetHtmlDocument();
	if(doc == NULL) return FALSE;

	doc->put_charset(wstrCSet);

	m_strCharset = "";

	return TRUE;
}

BOOL CItemWebCtrl::PutCharSet(LPCTSTR strCharset)
{
	//SAMPLE: char* strCharset = "euc-kr";
	m_strCharset = strCharset;
	if(GetReadyState() != READYSTATE_COMPLETE) 
		return FALSE;

	return _PutCharSet();
}

void CItemWebCtrl::SetBorderNone()
{
	MSHTML::IHTMLDocument2Ptr doc = (MSHTML::IHTMLDocument2Ptr)GetHtmlDocument();
	if(doc!=NULL) {
		MSHTML::IHTMLElementPtr body = (MSHTML::IHTMLElementPtr)doc->body;      
		if(body!=NULL)	body->style->border = _T("none");
	}
}

/////////////////////////////////////////////////////////////////////////////
// CItemWebCtrl message handlers

void CItemWebCtrl::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if(m_bNavigateStart) {
		m_NavigatedCount += 1;
	}

	m_strCharset = "";
	m_strInsHtml = "";
	m_strRunScript = "";

	CHtmlView::OnBeforeNavigate2(lpszURL, nFlags, lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);
}

int CItemWebCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CHtmlView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_bNavigateStart = FALSE;
	
	return 0;
}

BOOL CItemWebCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_LBUTTONDOWN) {
		// 마우스 버튼이 눌리면 네비게이션이 시작됨
		m_bNavigateStart = TRUE;
	}

	return CHtmlView::PreTranslateMessage(pMsg);
}

void CItemWebCtrl::OnNavigateComplete2( LPCTSTR strURL )
{
	if(m_bHideBorder) 
		SetBorderNone();

	if(m_strCharset.GetLength() > 0)
		_PutCharSet();
	if(m_strInsHtml.GetLength() > 0)
		_InsertHTML();
	if(m_strRunScript.GetLength() > 0)
		_ExecJavascript();

	CHtmlView::OnNavigateComplete2(strURL );
}
