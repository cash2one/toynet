// NoticeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "NoticeDlg.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNoticeDlg dialog


CNoticeDlg::CNoticeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNoticeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNoticeDlg)
	//}}AFX_DATA_INIT
	
	m_iNoticeCharNum = 0;
	m_iNoticeLineNum = 0;
	
	
}


void CNoticeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNoticeDlg)
	DDX_Control(pDX, IDC_OK, m_clOKBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNoticeDlg, CDialog)
	//{{AFX_MSG_MAP(CNoticeDlg)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNoticeDlg message handlers

void CNoticeDlg::SetNoticeStr(CString clNoticeStr)
{
	if( clNoticeStr.GetLength() >= MAX_NOTICE_STR_NUM ) return ;
	
	char cTempStr[MAX_NOTICE_STR_NUM];
	ZeroMemory( cTempStr, MAX_NOTICE_STR_NUM);
	sprintf(cTempStr, "%s", clNoticeStr);

	for(int i = 0 ; i < 50 ; i++ )
	{
		ZeroMemory(m_cNoticeStr[i], MAX_NOTICE_STR_NUM);
	}


	int iCount1 = 0;
	int iCount2 = 0;

	for( i = 0 ; i < MAX_NOTICE_STR_NUM ; i++ )
	{
		if( cTempStr[i] == 0 ||
			cTempStr[i] == '\0' )
		{
			iCount1++;
			break ;
		}
		else
		if( cTempStr[i] == '\n' )
		{
			iCount2 = 0;
			iCount1++;
			if( iCount1 >= 50 )	return ;
			continue ;
		}
		else
		if( cTempStr[i] == 13 )
		{
			iCount2++;			
			continue ;
		}

		m_cNoticeStr[iCount1][iCount2] = cTempStr[i];
		iCount2++;

		if( iCount2 > m_iNoticeCharNum )
		{
			m_iNoticeCharNum = iCount2;
		}
	}

	m_iNoticeLineNum = iCount1;
}



void CNoticeDlg::OnOk() 
{
	// TODO: Add your control notification handler code here

	CDialog::OnOK();	
}

BOOL CNoticeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here


	m_clBack.LoadBitmapFile(".\\image\\NoticeDlg.BMP");
	m_clTitle.LoadBitmapFile(".\\image\\NoticeDlgTitle.bmp");
	

	int iWinSizeX = m_iNoticeCharNum * 7 + 20;
	int iWinSizeY = m_iNoticeLineNum * 16 + 50;
		
	MoveWindow(100, 100, iWinSizeX, iWinSizeY);	
	CenterWindow(GetParent());
			
	m_clOKBtn.Init(iWinSizeX / 2 - 20, iWinSizeY - 35, "image\\Noticebtn.BMP", 4);

	// 폰트 생성	
	m_clFont.CreateFont(14,0,0,0,
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
		"굴림체");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNoticeDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	CDC cdc, titileDc;
	cdc.CreateCompatibleDC(&dc);
	titileDc.CreateCompatibleDC(&dc);
	
	if(cdc.m_hDC == NULL) return;
	

	CBitmap *pOldBmp = cdc.SelectObject(&m_clBack);
	CBitmap *pTitileOldBmp = titileDc.SelectObject(&m_clTitle);
			
	CRect rect;
	GetClientRect(&rect);
	//dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);
	dc.StretchBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0,373, 293, SRCCOPY);		// 배경 
	dc.StretchBlt(0, 0, rect.right, 30, &titileDc, 0, 0,373, 30, SRCCOPY);			// 제목
		

	dc.SetBkMode(TRANSPARENT);
	CFont* pOldFont = dc.SelectObject(&m_clFont);


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
	
	
	cdc.SelectObject(pOldFont);
	cdc.SelectObject(pOldBmp);
	titileDc.SelectObject(pTitileOldBmp);
	cdc.DeleteDC();	
	titileDc.DeleteDC();
	
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CNoticeDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here

	if(m_clBack.m_hObject) m_clBack.DeleteObject();
	if(m_clTitle.m_hObject) m_clTitle.DeleteObject();
	

	m_clFont.DeleteObject();

	
}
