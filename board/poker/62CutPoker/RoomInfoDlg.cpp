// RoomInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "RoomInfoDlg.h"
#include "MyBitmap.h"

#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRoomInfoDlg dialog


CRoomInfoDlg::CRoomInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRoomInfoDlg::IDD, pParent)
{
	ZeroMemory(&m_GI, sizeof(GAMEINFO));
	//{{AFX_DATA_INIT(CRoomInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	/*
	m_CurViewIndex=0;
	m_CurUserNumCnt=0;
	ZeroMemory(m_UserView, sizeof(m_UserView));
	*/
}


void CRoomInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRoomInfoDlg)
	DDX_Control(pDX, IDC_LIST_OBR, m_UserList);
	DDX_Control(pDX, IDC_BUTTON_RIGHT, m_RightBtn);
	DDX_Control(pDX, IDC_BUTTON_LEFT, m_LeftBtn);
	DDX_Control(pDX, IDOK, m_OkBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRoomInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CRoomInfoDlg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_LEFT, OnButtonLeft)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT, OnButtonRight)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_OBR, OnDblclkListObr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRoomInfoDlg message handlers

int CRoomInfoDlg::DoModal(GAMEINFO *pGI)
{
	// TODO: Add your specialized code here and/or call the base class
	memcpy(&m_GI, pGI, sizeof(GAMEINFO));
	
	if(pGI->Ri.bPlayer == 0) {
		g_Max_Player    = MAX_PLAYER - 1;
		g_Max_NewPlayer = g_Max_Player + MAX_OBSERVER;
			
	}
	else {
		g_Max_Player    = MAX_PLAYER;
		g_Max_NewPlayer = g_Max_Player + MAX_OBSERVER;
	}
	
//	for(int k = 0; k <g_Max_NewPlayer-1; k++)
//		memcpy(&m_GI.Ui[k], &m_GI.Ui[0], sizeof(USERINFO));


	for(int i=0; i<g_Max_NewPlayer-1; i++) {
		if(strlen(m_GI.Ui[i].ID) > 0) {
			if(i < g_Max_Player) {
				CharView[i].SetCharacter(&m_GI.Ui[i]);
			}
		}
	}


/*
	int cnt=0;
	for(int i=0; i<g_Max_Player;i++) {
		if(strlen(pGI->Ui[i].ID) > 0) {
			memcpy(&m_GI.Ui[cnt], &pGI->Ui[i], sizeof(USERINFO));
			cnt++;
		}
	}
	m_CurUserNumCnt = cnt;

	cnt=0;
	for( i=0; i<g_Max_Player;i++) {
		if(strlen(m_GI.Ui[i].ID) > 0) {
			memcpy(&m_UserView[cnt], &m_GI.Ui[i],sizeof(USERINFO));
			CharView[cnt].SetCharacter(&m_GI.Ui[i]);
			cnt++;
			if(cnt > 1) break;
		}
	}

	m_CurViewIndex=0; // 0 1 2 3 .. 현재 화면 
*/

	

	return CDialog::DoModal();
}

BOOL CRoomInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here

	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);

	// 배경 그림 읽기
	Back.LoadBitmapFile(".\\image\\Room\\Roominfoback.bmp");

	// 버튼 초기화
	m_OkBtn.Init(191,343, ".\\image\\commonbtn\\Btn_ok.bmp", 4);
//	m_LeftBtn.Init(11,70, ".\\image\\RBtn_roomleft.bmp", 4);
//	m_RightBtn.Init(369,70, ".\\image\\RPBtn_roomright.bmp", 4);

	// 폰트 생성
	if(Font1.m_hObject == NULL)
	Font1.CreateFont(12,0,0,0,
		//FW_NORMAL,
		FW_BOLD,
		FALSE,FALSE,
		0,
		//ANSI_CHARSET,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		"굴림");

	if(Font2.m_hObject == NULL)
	Font2.CreateFont(12,0,0,0,
		FW_NORMAL,
		FALSE,FALSE,
		0,
		//ANSI_CHARSET,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		"굴림");


	// 캐릭터 뷰 초기화
//	CRect rc(0,0,71,58);
	CRect rc(0,0,65,55);
	rc.OffsetRect(13,69);
	CharView[0].Create(this, rc, 4, 1000);
	rc.OffsetRect(0,90);
	CharView[1].Create(this, rc, 4, 1001);
	rc.OffsetRect(0,90);
	CharView[2].Create(this, rc, 4, 1001);

	
	rc.OffsetRect(221, -(90*2));
	CharView[3].Create(this, rc, 4, 1001);

	rc.OffsetRect(0,90);
	CharView[4].Create(this, rc, 4, 1001);

//	rc.OffsetRect(0,90);
//	CharView[5].Create(this, rc, 4, 1001);
//	rc.OffsetRect(0,87);
//	CharView[6].Create(this, rc, 4, 1001);

	User_InitList();

	for(int i=g_Max_Player; i<g_Max_NewPlayer-1; i++) {
		if(strlen(m_GI.Ui[i].ID) > 0) {
				User_InsItem(m_GI.Ui[i].UNum, m_GI.Ui[i].ID, m_GI.Ui[i].Sex, m_GI.Ui[i].nIcon);
		}
	}
		
	RECT rc1;
	GetWindowRect( &rc1 );
	rc1.right = rc1.left + 455;
	rc1.bottom = rc1.top + 388;
	MoveWindow( &rc1 );


	rc.SetRect(0, 0, 199, 49);
	rc.OffsetRect(239,275);
	m_UserList.MoveWindow(&rc);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRoomInfoDlg::OnPaint() 
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

	// 방제 찍기
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0,0,0));
	dc.SelectObject(&Font1);
	rect.SetRect(0,0,362,25);
	rect.OffsetRect(78,42);
	dc.DrawText(m_GI.Ri.Title, strlen(m_GI.Ri.Title), &rect, DT_LEFT);

	// 방장 찍기
	CMyBitmap bmp;
	bmp.LoadBitmap(IDB_LEVEL15);
	//bmp.TransDraw(dc.m_hDC, 339, 265, 15, 15, 15*level, 0, RGB(255,0,255));
	rect.SetRect(0,0,300,25);
	rect.OffsetRect(49,249);
	//dc.DrawText(m_GI.Ri.ID, strlen(m_GI.Ri.ID), &rect, DT_LEFT);


	for(int i=0; i<g_Max_Player; i++)
	{
		if(strlen(m_GI.Ui[i].ID)==0) continue;

		int ax = 87, ay = 70;
		ay += 91*i;

		if(i>=3) {
			ax = 310;
		    ay = 70 + 90*(i-3);
		}

		// 레벨 찍기
		int level =  GetPokerLevel(m_GI.Ui[i].PMoney);
	//	if( strcmp(m_GI.Ui[i].ID, m_GI.Ri.ID ) == 0 )
	//		bmp.TransDraw(dc.m_hDC, 337, 263, 15, 15, 15*level, 0, RGB(255,0,255));
		bmp.TransDraw(dc.m_hDC, ax, ay, 15, 15, 15*level, 0, RGB(255,0,255));
	
		dc.SetBkMode(TRANSPARENT);
		/*
		// 아이디 찍기	deleted by jeong
		ay+=2;
		dc.SelectObject(&Font1);
		dc.TextOut(ax+18, ay, m_GI.Ui[i].ID);
		*/

		// 승률 계산
		int totnum = m_GI.Ui[i].WinNum + m_GI.Ui[i].LooseNum;
		float winpro;
		if(totnum == 0) winpro = 0;
		else winpro = ((float)m_GI.Ui[i].WinNum / totnum)*100;

		// 캐릭터 정보 찍기
		CString str;
		dc.SelectObject(&Font2);
		str.Format("%d승 %d패", m_GI.Ui[i].WinNum, m_GI.Ui[i].LooseNum);
		dc.TextOut(ax, ay+18, str);
		str.Format("%d포기", m_GI.Ui[i].DrawNum);
		dc.TextOut(ax, ay+18+14, str);

		//str.Format("승률: %4.1f%% (%d)", winpro, m_GI.Ui[i].Point);
		str.Format("승률: %4.1f%% ", winpro);
		dc.TextOut(ax, ay+18+16+11, str);
		
		CString strM = NumberToOrientalString(max(0,m_GI.Ui[i].PMoney));
	//	str.Format(g_StrMan.Get(_T("USER_PMONEY")),strM);
		str.Format("%s원",strM);
		dc.TextOut(ax-73, ay+63, str);


	}
	bmp.DeleteObject();

	::DeleteObject(h);
	h = NULL;
//	cdc.DeleteDC();
//	Back.DeleteObject();

	// Do not call CDialog::OnPaint() for painting messages
}

void CRoomInfoDlg::OnOK() 
{
	// TODO: Add extra validation here
	Sound.Play(SND45);
	CDialog::OnOK();
}

void CRoomInfoDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialog::OnCancel();
}

void CRoomInfoDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

void CRoomInfoDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	// TODO: Add your message handler code here
	Back.DeleteObject();
	Font1.DeleteObject();
	Font2.DeleteObject();
}


void CRoomInfoDlg::OnButtonLeft() 
{
	// TODO: Add your control notification handler code here
/*
	m_CurViewIndex--;
	if(m_CurViewIndex < 0) {
		m_CurViewIndex = 0;
		return;
	}

	ZeroMemory(m_UserView, sizeof(m_UserView));
	CharView[0].SetCharacter(&m_UserView[0]);
	CharView[1].SetCharacter(&m_UserView[0]);

	int cnt=0;
	for(int i=m_CurViewIndex*2; i<g_Max_Player;i++) {
		if(strlen(m_GI.Ui[i].ID) > 0) {
			memcpy(&m_UserView[cnt], &m_GI.Ui[i],sizeof(USERINFO));
			CharView[cnt].SetCharacter(&m_UserView[cnt]);
			cnt++;
			if(cnt > 1) break;
		}
	}

	CRect rt;
	rt.SetRect(0,0,321,185);
	rt.OffsetRect(46,43);
	InvalidateRect(rt);
	*/
}

void CRoomInfoDlg::OnButtonRight() 
{
	// TODO: Add your control notification handler code here
/*
	if(m_CurViewIndex+1 > g_Max_Player/2) return;
	if(m_CurViewIndex+1 >= (m_CurUserNumCnt+1)/2) return;
	m_CurViewIndex++;
		
	ZeroMemory(m_UserView, sizeof(m_UserView));
	CharView[0].SetCharacter(&m_UserView[0]);
	CharView[1].SetCharacter(&m_UserView[0]);

	int cnt=0;
	for(int i=m_CurViewIndex*2; i<g_Max_Player;i++) {
		if(strlen(m_GI.Ui[i].ID) > 0) {
			memcpy(&m_UserView[cnt], &m_GI.Ui[i],sizeof(USERINFO));
			CharView[cnt].SetCharacter(&m_UserView[cnt]);
			cnt++;
			if(cnt > 1) break;
		}
	}

	CRect rt;
	rt.SetRect(0,0,321,185);
	rt.OffsetRect(46,43);
	InvalidateRect(rt);
*/
}

void CRoomInfoDlg::User_InitList()
{
		// 비트맵 읽기
	CBitmap Bmp;
	Bmp.LoadBitmap(IDB_LEVEL15);

	// 이미지 리스트 생성
	m_UserImage.Create(15, 15, ILC_COLOR8 | ILC_MASK, 15, 0);
	m_UserImage.Add(&Bmp, RGB(255,0,255));

	m_UserList.SetImageList(&m_UserImage, LVSIL_SMALL);
	m_UserList.SetFullRowSel(TRUE);
	m_UserList.SetTextColor(RGB(0,0,70));

//	=========================================================================================
//	샘플작업
    m_UserList.SetGridLineUse( true );
    m_UserList.SetScrollBarChange( true );
	m_UserList.SetHeaderColors( RGB( 86, 233, 236 ), RGB( 66, 65, 66 ) );
	m_UserList.SetBackColors( RGB( 254, 253, 253 ), RGB( 253, 253, 253 ) );

	m_UserList.Init( true, true, 3, RGB( 45, 44, 0 ), RGB( 45, 44, 0 ) ); //정렬
//	=========================================================================================


	//// Column 삽입부
	LV_COLUMN col;
	char szName[256];
	wsprintf(szName, "아이디");
	
	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_WIDTH | LVCF_TEXT;
	col.fmt  = LVCFMT_LEFT;
	col.cx   = 115;
	col.pszText = szName;
	col.iSubItem = 0;
	m_UserList.InsertColumn(0, &col);

	col.fmt  = LVCFMT_CENTER;
	col.cx   = 40;
	col.pszText = g_StrMan._Get(_T("SEX"));
	col.iSubItem = 1;
	m_UserList.InsertColumn(1, &col);

}

void CRoomInfoDlg::User_InsItem(int unum, char *id, BOOL bsex, int level)
{
	
	LV_ITEM item;
	char szName[256]={0,};

	// 아이디
	wsprintf(szName, "%s", id);
	item.mask = LVIF_TEXT | LVIF_IMAGE;
	item.pszText = szName;
	item.iItem = m_UserList.GetItemCount();
	item.iSubItem = 0;
	item.iImage = level;
	m_UserList.InsertItem(&item);
	// 유저번호
	m_UserList.SetItemData(item.iItem, unum);

	// 성별
	if(bsex==TRUE) wsprintf(szName, "♂");
	else wsprintf(szName, "♀");
	item.iSubItem = 1;
	m_UserList.SetItem(&item);

}

#include "ProfileDlg.h"
void CRoomInfoDlg::OnDblclkListObr(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
		// 선택된 아이템중 첫번째 아이템을 얻는다
	POSITION pos = m_UserList.GetFirstSelectedItemPosition();
	if (pos == NULL) return;
	int nUserSel = m_UserList.GetNextSelectedItem(pos);
	
	if(nUserSel < 0) return;
	if(nUserSel >= m_UserList.GetItemCount()) return;

	CString szID;
	szID = m_UserList.GetItemText(nUserSel, 0);
	int UNum = m_UserList.GetItemData(nUserSel);
	if(szID.GetLength() == 0) return;
	if(UNum < 0) return;

//	g_Wait = TRUE;

	for(int i=0; i<g_Max_NewPlayer; i++)
	{
		if(strlen(m_GI.Ui[i].ID)==0) continue;
		if(strcmp(m_GI.Ui[i].ID, (char*)szID.operator LPCTSTR())==0)
		{
			CProfileDlg Dlg;
			Dlg.Init(&m_GI.Ui[i]);
			Dlg.DoModal();
			break;
		}
	}

	*pResult = 0;
}
