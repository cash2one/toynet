// RoomOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "RoomOptionDlg.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRoomOptionDlg dialog


CRoomOptionDlg::CRoomOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRoomOptionDlg::IDD, pParent)
{
	ZeroMemory(m_aMyCard, sizeof(m_aMyCard));
	m_nMyCard = 0;
	//{{AFX_DATA_INIT(CRoomOptionDlg)
	m_bWhis = FALSE;
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CRoomOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRoomOptionDlg)
	DDX_Control(pDX, IDCANCEL, m_CancelBtn);
	DDX_Control(pDX, IDOK, m_OkBtn);
	DDX_Control(pDX, IDC_COMBO_NAME, m_ctrTitle);
	DDX_Check(pDX, IDC_CHECK_WHIS, m_bWhis);
	DDX_CBString(pDX, IDC_COMBO_NAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRoomOptionDlg, CDialog)
	//{{AFX_MSG_MAP(CRoomOptionDlg)
	ON_BN_CLICKED(IDC_CHECK_WHIS, OnCheckWhis)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRoomOptionDlg message handlers



BOOL CRoomOptionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);

	Back.LoadBitmapFile(".\\image\\Room\\changeroominfobg.bmp");
	m_OkBtn.Init(86,82,".\\image\\Room\\ch_roominfo_ok.BMP",4);
	m_CancelBtn.Init(186,82,".\\image\\commonbtn\\Btn_cancel.bmp",4);

	m_ctrTitle.AddString((LPCTSTR)g_StrMan.Get(_T("RNAME1")));
	m_ctrTitle.AddString((LPCTSTR)g_StrMan.Get(_T("RNAME2")));
	m_ctrTitle.AddString((LPCTSTR)g_StrMan.Get(_T("RNAME3")));
	m_ctrTitle.AddString((LPCTSTR)g_StrMan.Get(_T("RNAME4")));
	m_ctrTitle.AddString((LPCTSTR)g_StrMan.Get(_T("RNAME5")));
	m_ctrTitle.AddString((LPCTSTR)g_StrMan.Get(_T("RNAME6")));
	m_ctrTitle.AddString((LPCTSTR)g_StrMan.Get(_T("RNAME7")));
	m_ctrTitle.AddString((LPCTSTR)g_StrMan.Get(_T("RNAME8")));
	m_ctrTitle.AddString((LPCTSTR)g_StrMan.Get(_T("RNAME9")));
/*
	// 카드 사이즈
	int xSize = 33;
	int ySize = 45;

	Page.Init(xSize, ySize, 16);
	int nCnt = 0;
	int ysize = 0;
	int xfirst = 100;
	int yfirst = 60;
	for(int i=0; i < 17; i++)
	{
		m_aMyCard[i] = i;
		
		if(nCnt == 0)
			CardRect[i].SetRect(xfirst, yfirst+ysize, xfirst+xSize, yfirst+ySize+ysize);
		else
			CardRect[i].SetRect(xfirst+(xSize*nCnt)+nCnt*5, yfirst+ysize, xfirst+(xSize*(nCnt+1))+nCnt*5, yfirst+ySize+ysize);
		
		 
		nCnt++;
		if(nCnt == 8) 
		{
			nCnt= 0;
			ysize = ySize+5;
		}
	}
	*/

	m_strName=g_RI.Title;
	// ### [ 관전기능 ] ###
	m_nMyCard = g_MyInfo.UI.PI.nCard;// 인텍스 0 ~ 15
	UpdateData(FALSE);

	RECT rc1;
	GetWindowRect( &rc1 );
	rc1.right = rc1.left + 358;
	rc1.bottom = rc1.top + 128;
	MoveWindow( &rc1 );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRoomOptionDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if(m_strName.GetLength()==0 || m_strName.GetLength()>30){
		AfxMessageBox(g_StrMan.Get(_T("ROOMTITLE")));
		GetDlgItem(IDC_COMBO_NAME)->SetFocus();
		return;
	}
	// 욕설 검사
	if(BadChat.IsBad(m_strName)==TRUE){
		AfxMessageBox(g_StrMan.Get(_T("BAD_CHAT2")));
		GetDlgItem(IDC_COMBO_NAME)->SetFocus();
		return;
	}

	if(m_aMyCard[m_nMyCard]==(int)g_RI.nCard && strcmp((LPCTSTR)m_strName,g_RI.Title)==0 ) {//&& 
		//m_bWhis==g_RI.bWhis){
		return;
	}

	STYLEROOMINFO sri; memset(&sri,0,sizeof(sri));
	sri.bWhis = m_bWhis;
	sri.nCard = (char)m_aMyCard[m_nMyCard];

	strncpy(sri.Title,(LPCTSTR)m_strName,30);
	strncpy(sri.ID,g_RI.ID,15);
	sri.UNum    = g_RI.UNum;
	sri.RoomNum = g_RI.RoomNum;

	CCL_ASK_CHSTYLEROOMINFO msg;
	msg.Set(&sri);
	SockMan.SendData(g_MainSrvSID, msg.pData, msg.GetTotalSize());

	// ### [ 관전기능 ] ###
	g_MyInfo.UI.PI.nCard = (char)m_aMyCard[m_nMyCard];
	if( !g_MyObserver ) Play[0].UI.PI.nCard = g_MyInfo.UI.PI.nCard;

//	int nSNum = g_pGameDlg->m_PLATE.GetSIndex(0);
//	CNetData SendData;
//	SendData.SetData((LPVOID)(&sri),sizeof(sri),NM_QMDATA,NM_CHANGEROOM,nSNum);
//	g_Send(SendData.GetData(),SendData.GetSize());

//	Hide();
	
	CDialog::OnOK();
}

void CRoomOptionDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CRoomOptionDlg::OnCheckWhis() 
{
	// TODO: Add your control notification handler code here
	
}

void CRoomOptionDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;

	HRGN h = BitmapToRegion( Back.operator HBITMAP(), RGB(255,0,255));
	if(h) SetWindowRgn(h,TRUE);// 사용방법

	cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	::DeleteObject(h);
	h = NULL;
	cdc.DeleteDC();


	//Page.PutSprAuto(0, 0, &BigCardSpr, m_aMyCard[0]);
	
	//for(int i = 0; i < 16; i++)
	//{
	//	Page.FlipToGDI(dc.m_hDC, CardRect[i].left, CardRect[i].top, 1);
	//}

	/*
	CMyBitmap bmp, bmpBig;
	bmp.LoadBitmapFile(".\\image\\1.bmp");
	bmpBig.LoadBitmapFile(".\\image\\4.bmp");
	
	if(bmp.m_hObject == NULL) return;
	if(bmpBig.m_hObject == NULL) return;

	bmpBig.TransDraw(dc.m_hDC, 17,60, 71,96, 71*m_nMyCard, 0, RGB(255, 0, 255));

	for(int i = 0; i < 16; i++)
	{
		bmp.TransDraw(dc.m_hDC, CardRect[i].left, CardRect[i].top, 33, 45, 33*i, 0, RGB(255,0,255));
	}
	bmp.DeleteObject();
	bmpBig.DeleteObject();
	*/

	
	// Do not call CDialog::OnPaint() for painting messages
}

void CRoomOptionDlg::OnDestroy() 
{
	Back.DeleteObject();
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CRoomOptionDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
/*
	for(int i = 0; i < 16; i++)
	{
		if(CardRect[i].PtInRect(point)) SetCursor(g_pGameView->hCurHand);
	}
*/
	CDialog::OnMouseMove(nFlags, point);
}

void CRoomOptionDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
/*
	for(int i = 0; i < 16; i++)
	{
		if(CardRect[i].PtInRect(point)) 
		{
			m_nMyCard = i;
			CRect rect;
			rect.SetRect(17,60, 71+17,96+60);
			InvalidateRect(&rect);
		}
	}
*/
	
	CDialog::OnLButtonDown(nFlags, point);
}
