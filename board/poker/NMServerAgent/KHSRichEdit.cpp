// KHSRichEdit.cpp : implementation file
//

#include "stdafx.h"
#include "NMServerAgent.h"
#include "NMServerAgentDlg.h"
#include "KHSRichEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_RICHCHARSIZE  (10000) //최대 맥시멈 글자 설정.
/////////////////////////////////////////////////////////////////////////////
// CKHSRichEdit

CKHSRichEdit::CKHSRichEdit()
{
	m_strSel = _T("");
}

CKHSRichEdit::~CKHSRichEdit()
{
}


BEGIN_MESSAGE_MAP(CKHSRichEdit, CRichEditCtrl)
	//{{AFX_MSG_MAP(CKHSRichEdit)
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(IDC_MENU_COPY, OnMenuCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKHSRichEdit message handlers
int CKHSRichEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CHARFORMAT cf;

	cf.cbSize = sizeof (CHARFORMAT);  
	cf.dwMask = CFM_FACE | CFM_SIZE; 
	cf.yHeight = 180; 
	sprintf(cf.szFaceName, "굴림"); 
 
	SetDefaultCharFormat(cf); 
 
	m_ctrMenu.LoadMenu(IDR_POPMENU);

	return 0;
}


void CKHSRichEdit::AddText(LPCTSTR szTextIn, COLORREF &crNewColor)
{
	int iStartPos = LineIndex(max(GetLineCount()-1, 0));

	int iTotalTextLength = GetWindowTextLength();
	SetSel(iTotalTextLength, iTotalTextLength);
	ReplaceSel(szTextIn);

	CHARFORMAT cf;
	cf.cbSize		= sizeof(CHARFORMAT);
	cf.dwMask		= CFM_COLOR | CFM_UNDERLINE | CFM_BOLD;
	cf.dwEffects	=(unsigned long) ~( CFE_AUTOCOLOR | CFE_UNDERLINE | CFE_BOLD);
	cf.crTextColor	= crNewColor;//RGB(0, 0, 0);

	int iEndPos = iStartPos + strlen(szTextIn);
	SetSel(iStartPos, iEndPos);
	SetSelectionCharFormat(cf);
	HideSelection(TRUE, FALSE);	

//	LineScroll(1);
	//(KHS)추가.
	if(iTotalTextLength>MAX_RICHCHARSIZE){
		SetSel(0,iEndPos-iStartPos); 
		ReplaceSel("");
	}
	// Windows2000의 경우 맨 위로 스크롤되므로 강제로 맨 아랫줄로 스크롤 시킴
	int srMin, srMax;
	GetScrollRange(SB_VERT, &srMin, &srMax);
	if(srMax>0)
	{
		CRect rc;
		GetClientRect(&rc);
		//SetScrollPos(SB_VERT, srMax - rc.Height(), TRUE);
		UINT wParam = MAKELONG((WORD)SB_THUMBPOSITION, srMax-rc.Height());
		CScrollBar* pSB = GetScrollBarCtrl(SB_VERT);
		HWND hWndVSB=NULL;
		if(pSB != NULL) hWndVSB = pSB->m_hWnd;
		SendMessage(WM_VSCROLL, wParam, (UINT)hWndVSB);
	}
}

void CKHSRichEdit::AddText(CString &strTextIn, COLORREF &crNewColor)
{
	int iStartPos = LineIndex(max(GetLineCount()-1, 0));

	int iTotalTextLength = GetWindowTextLength();
	SetSel(iTotalTextLength, iTotalTextLength);
	ReplaceSel((LPCTSTR)strTextIn);
//	int iStartPos = iTotalTextLength;

	CHARFORMAT cf;
	cf.cbSize		= sizeof(CHARFORMAT);
	cf.dwMask		= CFM_COLOR | CFM_UNDERLINE | CFM_BOLD;
	cf.dwEffects	= (unsigned long)~( CFE_AUTOCOLOR | CFE_UNDERLINE | CFE_BOLD);
	cf.crTextColor	= crNewColor;//RGB(0, 0, 0);

	int iEndPos = iStartPos + strTextIn.GetLength();
	SetSel(iStartPos, iEndPos);
	SetSelectionCharFormat(cf);
	HideSelection(TRUE, FALSE);	

	//(KHS)추가.
	if(iTotalTextLength>MAX_RICHCHARSIZE){
		SetSel(0,iEndPos-iStartPos); 
		ReplaceSel("");
	}
	// Windows2000의 경우 맨 위로 스크롤되므로 강제로 맨 아랫줄로 스크롤 시킴
	int srMin, srMax;
	GetScrollRange(SB_VERT, &srMin, &srMax);
	if(srMax>0)
	{
		CRect rc;
		GetClientRect(&rc);
		UINT wParam = MAKELONG((WORD)SB_THUMBPOSITION, srMax-rc.Height());
		CScrollBar* pSB = GetScrollBarCtrl(SB_VERT);
		HWND hWndVSB=NULL;
		if(pSB != NULL) hWndVSB = pSB->m_hWnd;
		SendMessage(WM_VSCROLL, wParam, (UINT)hWndVSB);
	}
}

void CKHSRichEdit::OnRButtonDown(UINT nFlags, CPoint point) 
{
	m_strSel = GetSelText();
	if(m_strSel==_T(""))
		m_ctrMenu.GetSubMenu(2)->EnableMenuItem(IDC_MENU_COPY, MF_GRAYED);
	else
		m_ctrMenu.GetSubMenu(2)->EnableMenuItem(IDC_MENU_COPY, MF_ENABLED);

	// 팝업 메뉴
	CPoint pnt;
	GetCursorPos(&pnt);
	m_ctrMenu.GetSubMenu(2)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pnt.x, pnt.y, this);
	
	CRichEditCtrl::OnRButtonDown(nFlags, point);
}

void CKHSRichEdit::OnMenuCopy() 
{
	if(m_strSel==_T("")) return;

	Copy();	
}
