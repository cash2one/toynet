// MyRichEdit.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "ChatViewEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChatViewEdit

CChatViewEdit::CChatViewEdit()
{
	oldline = 0;
}

CChatViewEdit::~CChatViewEdit()
{
}


BEGIN_MESSAGE_MAP(CChatViewEdit, CRichEditCtrl)
	//{{AFX_MSG_MAP(CChatViewEdit)
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatViewEdit message handlers

void CChatViewEdit::SetTextFormat(int kind)
{
	/*
	CHARFORMAT cf;
	cf.cbSize = sizeof(cf);
	
	SetWordCharFormat(cf);
	
	
	switch(kind)
	{
	case 0:
		GetDefaultCharFormat(cf);
		cf.dwMask = CFM_BOLD | CFM_CHARSET | CFM_COLOR | CFM_FACE | CFM_ITALIC | CFM_OFFSET | CFM_PROTECTED | CFM_SIZE | CFM_STRIKEOUT | CFM_UNDERLINE;
		cf.dwEffects &= !CFE_UNDERLINE & !CFE_AUTOCOLOR;
		break;
	case 1:
		GetDefaultCharFormat(cf);
		cf.dwMask |= CFM_BOLD;
		cf.dwEffects |= CFE_BOLD;
		break;
	}

	SetWordCharFormat(cf);
	*/
}

void CChatViewEdit::AddText(CString *str, COLORREF crNewColor, BOOL bScroll)
{
	AddText( (char*)str->operator LPCTSTR(), crNewColor, bScroll);
}

void CChatViewEdit::AddText(char *str2,  COLORREF crNewColor, BOOL bScroll) {
	// 색상을 바꿀 택스트의 시작위치를 구함
	// GetLength()로 구하지 않는 이유는 Windows2000에서는 값이 더 크게 나오기때문
	int iStartPos = LineIndex(max(GetLineCount()-1, 0));

	CString str;
	str.Format("%s",str2);
	if(str.Right(1) == "\n") str.TrimRight("\n");

	if(GetWindowTextLength()>0)
	{
		int totlength = GetWindowTextLength();	// 컨트롤 내의 텍스트의 총 길이를 구하고
		SetSel(totlength, totlength);			// 마지막 문자로 셀렉션을 옮긴다
		ReplaceSel("\n");						// 마지막 문자 뒤에 글자를 추가한다.
		iStartPos = LineIndex(max(GetLineCount()-1, 0));
	}

	int totlength = GetWindowTextLength();	// 컨트롤 내의 텍스트의 총 길이를 구하고
	SetSel(totlength, totlength);			// 마지막 문자로 셀렉션을 옮긴다
	ReplaceSel(str);	// 마지막 문자 뒤에 글자를 추가한다.

	// 글자색을 넣는다
	CHARFORMAT cf;
	cf.cbSize		= sizeof(CHARFORMAT);
	cf.dwMask		= CFM_COLOR | CFM_UNDERLINE | CFM_BOLD;
	cf.dwEffects	=(unsigned long) ~( CFE_AUTOCOLOR | CFE_UNDERLINE | CFE_BOLD);
	cf.crTextColor	= crNewColor;//RGB(0, 0, 0);

	int iEndPos = iStartPos + strlen(str);
	SetSel(iStartPos, iEndPos);
	SetSelectionCharFormat(cf);
	HideSelection(TRUE, FALSE);	

	int newline = GetLineCount();
	int deline = 0;
	
	if(!bScroll) newline = oldline = 0;

	// 라인수가 90줄 이상이면 위의 줄은 삭제한다
	if(newline > 90) 
	{ 
		int delline = newline-90; 
		int endchar = LineIndex(delline);
		SetSel(0, endchar);
		ReplaceSel("");
		totlength = GetTextLength();
		SetSel(totlength, totlength);
		oldline = newline;
		newline = oldline + delline;
	}

	int sclamount = newline-oldline;
	if(sclamount < 0) sclamount = 0;
	if(bScroll)
	{
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
		//else LineScroll(sclamount);// 늘어난 줄 수 만큼 스크롤 한다
	}

	oldline = newline;
	//SetTextFormat(0);
}


void CChatViewEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CRichEditCtrl::OnSetFocus(pOldWnd);
	::SetFocus(pOldWnd->m_hWnd);

	// TODO: Add your message handler code here
}

LRESULT CChatViewEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

	return CRichEditCtrl::WindowProc(message, wParam, lParam);
}
