// MsgBoxDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "MsgBoxDlg.h"

#include "MyBitmap.h"
#include "Global.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgBoxDlg dialog


CMsgBoxDlg::CMsgBoxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMsgBoxDlg::IDD, pParent)
{
	iMode = 0;
	hTimerWait = NULL;
	m_iNoticeCharNum = 0;
	m_iNoticeLineNum = 0;
	//{{AFX_DATA_INIT(CMsgBoxDlg)
	//}}AFX_DATA_INIT
}


void CMsgBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgBoxDlg)
	DDX_Control(pDX, IDC_RICHEDIT_NOTIC, m_ctrlNotic);
	DDX_Control(pDX, IDOK, m_OkBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMsgBoxDlg, CDialog)
	//{{AFX_MSG_MAP(CMsgBoxDlg)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgBoxDlg message handlers

void CMsgBoxDlg::MsgBox(int mode, char *msgstr)
{
	iMode = mode;
	szMsgText.Format(msgstr);

	SetLineCount(szMsgText);
	if( ::IsWindow(this->m_hWnd)  ) {
		SetWindowResize();
		return;
	}

	DoModal();
}

void CMsgBoxDlg::MsgBox(int mode, CString &msgstr)
{
	iMode = mode;
	szMsgText.Format(msgstr);

	SetLineCount(szMsgText);
	if( ::IsWindow(this->m_hWnd) ) {
	
		SetWindowResize();
		return;
	}

	DoModal();
}

void CMsgBoxDlg::SetLineCount(CString clNoticeStr)
{
	return;

	////////
	if( clNoticeStr.GetLength() >= MAX_NOTICE_STR_NUM ) return ;
	
	char cTempStr[MAX_NOTICE_STR_NUM];
	ZeroMemory( cTempStr, MAX_NOTICE_STR_NUM);
	sprintf(cTempStr, "%s", clNoticeStr);
	
	for(int i = 0 ; i < 50 ; i++ ) {
		ZeroMemory(m_cNoticeStr[i], MAX_NOTICE_STR_NUM);
	}
	
	int iCount1 = 0;
	int iCount2 = 0;
	
	for( i = 0 ; i < MAX_NOTICE_STR_NUM ; i++ ) {

		if( cTempStr[i] == 0 ||	cTempStr[i] == '\0' ) {
			iCount1++;
			break ;
		} else if( cTempStr[i] == '\n' ) {
				iCount2 = 0;
				iCount1++;
				if( iCount1 >= 50 )	return ;
				continue ;
		} else if( cTempStr[i] == 13 ) {
					iCount2++;			
					continue ;
		}
		
		m_cNoticeStr[iCount1][iCount2] = cTempStr[i];
		iCount2++;
		
		if( iCount2 > m_iNoticeCharNum ) {// 가장 긴 라인
			m_iNoticeCharNum = iCount2;
		}
	}
	m_iNoticeLineNum = iCount1;
	////////
}

BOOL CMsgBoxDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);

	SetWindowResize();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CMsgBoxDlg::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();
}

void CMsgBoxDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialog::OnCancel();
}

void CMsgBoxDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == MSGBOXDLG_WAIT_TIMER)
	{
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CMsgBoxDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}


void CMsgBoxDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC cdc;
//	CDC titleDC;
	cdc.CreateCompatibleDC(&dc);
//	titleDC.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
//	if(titleDC.m_hDC == NULL ) return;

	CMyBitmap Back;
//	if(iMode == 0)
//		Back.LoadBitmapFile("image\\Notice\\SmallMsgBack.bmp");
//	else
	Back.LoadBitmapFile("image\\Notice\\BigMsgBack.bmp");

	HRGN h = BitmapToRegion( Back.operator HBITMAP(), RGB(255,0,255));
	if(h) SetWindowRgn(h,TRUE);// 사용방법

	CBitmap *pOldBmp = cdc.SelectObject(&Back);
	
	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);
//	dc.StretchBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, 348, 160, SRCCOPY);		// 배경 
//	dc.StretchBlt(0, 0, rect.right, 30, &titleDC, 0, 0,373, 30, SRCCOPY);			// 제목
	
	// 폰트 생성
	CFont Font1;
	Font1.CreateFont(12,0,0,0,
		FW_NORMAL,
		//FW_BOLD,
		FALSE,FALSE,
		0,
		//ANSI_CHARSET,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		"굴림");
	
	dc.SetBkMode(TRANSPARENT);
	
	CFont* pOldFont = dc.SelectObject(&Font1);
	dc.SetTextColor(RGB(0,0,0));
/*	
	CRect rt;
	rt.SetRect(0,0, 320,82);
	rt.OffsetRect(15,46); 
	dc.DrawText( szMsgText, &rt, DT_LEFT | DT_WORDBREAK  );
*/
/*
	int iRenderPosX = 10;
	int iRenderPosY = 10;	
	
	int iLinePos = iRenderPosX;
	
	// 제목 줄력부분
	dc.SetTextColor(RGB(255,255,11));
	dc.TextOut( iRenderPosX - 1, iLinePos	 , m_cNoticeStr[0]);
	dc.TextOut( iRenderPosX,	 iLinePos - 1, m_cNoticeStr[0]);
	dc.TextOut( iRenderPosX + 1, iLinePos	 , m_cNoticeStr[0]);
	dc.TextOut( iRenderPosX,     iLinePos + 1, m_cNoticeStr[0]);
	
	dc.SetTextColor(RGB(255,11,11));
	dc.TextOut( iRenderPosX, iLinePos, m_cNoticeStr[0]);

	dc.SetTextColor(RGB(0,0,0));
	iLinePos = iLinePos + 23;

	// 내용 출력 부분
	for(int i = 1 ; i < m_iNoticeLineNum ; i++ )
	{
		dc.TextOut( iRenderPosX, iLinePos, m_cNoticeStr[i]);
		iLinePos = iLinePos + 16;
	}
*/	
	::DeleteObject(h);
	h = NULL;
	cdc.SelectObject(pOldFont);
	cdc.SelectObject(pOldBmp);
	cdc.DeleteDC();
//	titleDC.DeleteDC();
	Back.DeleteObject();
	Font1.DeleteObject();
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CMsgBoxDlg::SetWindowResize()
{
	// 채팅 뷰 리치 에디트 컨트롤 초기화
	CRect rt;
	m_ctrlNotic.GetClientRect(rt);

	if(iMode==1){
		rt.SetRect(0,0,314,rt.Height());
		rt.OffsetRect(CPoint(16,40));
	}
	else rt.OffsetRect(CPoint(84,40));

	m_ctrlNotic.MoveWindow(rt);

	m_ctrlNotic.SetReadOnly(TRUE);
	m_ctrlNotic.SetBackgroundColor(FALSE, RGB(75,223,255));

	m_ctrlNotic.SetWindowText("");
	m_ctrlNotic.AddText(&szMsgText, RGB(0,0,0));
	
	int nFirstVisible = m_ctrlNotic.GetFirstVisibleLine();
	if (nFirstVisible > 0) 	{
		m_ctrlNotic.LineScroll(-nFirstVisible, 0);
	}
				
	
	m_OkBtn.Init(132, 152, ".\\image\\commonbtn\\Btn_ok.bmp", 4);
	RECT rc1;
	GetWindowRect( &rc1 );
	rc1.right = rc1.left + 346;
	rc1.bottom = rc1.top + 194;
	MoveWindow( &rc1 );
	


/*
	int iWinSizeX = m_iNoticeCharNum * 7 + 20;
	int iWinSizeY = m_iNoticeLineNum * 16 + 50;

	MoveWindow(100, 100, iWinSizeX, iWinSizeY);	
	CenterWindow(GetParent());
	m_OkBtn.Init(iWinSizeX / 2 - 20, iWinSizeY - 35, "image\\OPBtn_roomok.BMP", 4);
*/
	
/*	if( szMsgText.GetLength() > 220 ) {
		iMode = 1;
		m_OkBtn.Init(160, 140, ".\\image\\OPBtn_roomok.bmp", 4);
		RECT rc1;
		GetWindowRect( &rc1 );
		rc1.right = rc1.left + 402;
		rc1.bottom = rc1.top + 194;
		MoveWindow( &rc1 );
	}
	else
	{
		iMode = 0;
		m_OkBtn.Init(140, 106, ".\\image\\OPBtn_roomok.bmp", 4);
		RECT rc1;
		GetWindowRect( &rc1 );
		rc1.right = rc1.left + 348;
		rc1.bottom = rc1.top + 160;
		MoveWindow( &rc1 );
	}
*/	
}


int CMsgBoxDlg::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DoModal();
}


