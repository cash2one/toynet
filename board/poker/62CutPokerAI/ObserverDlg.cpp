// ObserverDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "Global.h"
#include "ObserverDlg.h"
#include "ProfileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObserverDlg dialog


CObserverDlg::CObserverDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CObserverDlg::IDD, pParent)
{
	ViewMode = 0;
	nUserSel = -1;
	//{{AFX_DATA_INIT(CObserverDlg)
	//}}AFX_DATA_INIT
}


void CObserverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObserverDlg)
	DDX_Control(pDX, IDC_BUTTON_OUT, m_OutBtn);
	DDX_Control(pDX, IDC_BUTTON_IN, m_InBtn);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_CloseBtn);
	DDX_Control(pDX, IDC_BTN_PROFILE, m_ProfileBtn);
	DDX_Control(pDX, IDC_LIST_USER, m_UserList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CObserverDlg, CDialog)
	//{{AFX_MSG_MAP(CObserverDlg)
	ON_BN_CLICKED(IDC_BTN_PROFILE, OnBtnProfile)
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_USER, OnDblclkListUser)
	ON_BN_CLICKED(IDC_BUTTON_IN, OnButtonIn)
	ON_BN_CLICKED(IDC_BUTTON_OUT, OnButtonOut)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObserverDlg message handlers

BOOL CObserverDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);

	// 사이즈 저장
	GetWindowRect(&OrgWindRect);
	GetDlgItem(IDC_LIST_USER)->GetWindowRect(&OrgListRect);
	ScreenToClient(&OrgListRect);

	// 버튼 초기화
	m_ProfileBtn.Init(35,164,".\\image\\Observer\\Btn_profile.bmp",4);
	m_CloseBtn.Init(286,164,"image\\CommonBtn\\Btn_close.bmp", 4);

	m_InBtn.Init(127, 164, "image\\Observer\\OBtn_In.bmp", 4);
	m_OutBtn.Init(127, 164, "image\\Observer\\OBtn_Out.bmp", 4);

	User_InitList();

	RECT rc1;
	GetWindowRect( &rc1 );
	rc1.right = rc1.left + 405;
	rc1.bottom = rc1.top + 213;
	MoveWindow( &rc1 );

	//
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CObserverDlg::OnBtnProfile() 
{
	// TODO: Add your control notification handler code here
	Sound.Play(SND45);

	// 선택된 아이템중 첫번째 아이템을 얻는다
	POSITION pos = m_UserList.GetFirstSelectedItemPosition();
	if (pos == NULL) return;
	nUserSel = m_UserList.GetNextSelectedItem(pos);
	
	if(nUserSel < 0) return;
	if(nUserSel >= m_UserList.GetItemCount()) return;

	CString szID;
	szID = m_UserList.GetItemText(nUserSel, 0);
	int UNum = m_UserList.GetItemData(nUserSel);
	if(szID.GetLength() == 0) return;
	if(UNum < 0) return;

	g_Wait = TRUE;

	for(int i=0; i<g_Max_NewPlayer; i++)
	{
		if(strlen(Play[i].UI.ID)==0) continue;
		if(strcmp(Play[i].UI.ID, (char*)szID.operator LPCTSTR())==0)
		{
			CProfileDlg Dlg;
			Dlg.Init(&Play[i].UI);
			Dlg.DoModal();
			break;
		}
	}

	
/*
	CSV_ASK_USERINFO aumsg;
	aumsg.Set(UNum, (char*)szID.operator LPCTSTR());
	SockMan.SendData(g_MainSrvSID, aumsg.pData, aumsg.GetTotalSize());
*/
	
}

void CObserverDlg::OnBtnClose() 
{
	// TODO: Add your control notification handler code here
	Sound.Play(SND45);

	PostMessage(WM_COMMAND, IDCANCEL, 0);

	
}

void CObserverDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	//CDialog::OnOK();
}

void CObserverDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	ShowWindow(SW_HIDE);
//	CDialog::OnCancel();
}

void CObserverDlg::User_InitList()
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
	m_UserList.SetHeaderColors( RGB( 255, 255, 255 ), RGB( 66, 65, 66 ) );
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

	wsprintf(szName,"보유금액");
	col.fmt  = LVCFMT_RIGHT;
	col.cx   = 140;
	col.pszText = szName;
	col.iSubItem = 2;
	m_UserList.InsertColumn(2, &col);



	wsprintf(szName, "예약");
	col.fmt  = LVCFMT_LEFT;
	col.cx   = 52;
	col.iSubItem = 3;
	m_UserList.InsertColumn(3, &col);

}

void CObserverDlg::User_InsItem(int unum, char *id, BOOL sex, int level, INT64 money, int seq)
{
	// 대기실 보기 상태이면 위치가 대기실이 아닌 사용자는 삽입 취소
//	if(ViewMode==0 ) return;
	// 레코드를 리스트 뷰에 삽입
	if(g_Where != WH_GAME) return;
	LV_ITEM item;
	char szName[256];

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
	if(sex==TRUE) wsprintf(szName, "♂");
	else wsprintf(szName, "♀");
	item.iSubItem = 1;
	m_UserList.SetItem(&item);

	//보유금액
	CString strM;
	strM = g_MakeCommaMoney(money);
	item.pszText = (char*)strM.operator LPCTSTR();
	item.iSubItem = 2;
	m_UserList.SetItem(&item);


	// 예약
	item.iSubItem = 3;
	switch(seq)
	{
	case 0: wsprintf(szName, "1"); break;
	case 1: wsprintf(szName, "대기"); break;
	case 2: wsprintf(szName, "2"); break;
	case 3: wsprintf(szName, "3"); break;
	}
	item.pszText = szName;

	m_UserList.SetItem(&item);

}

void CObserverDlg::User_SetItem(int unum, char *id, BOOL sex, int level, INT64 money, int seq)
{
	// 대기실 보기 상태이고 위치가 대기실이 아니면 삭제
//	if(ViewMode==0 )
	{
//		User_DelItem(unum, id);
//		return;
	}
	
	int num=-1;
	CString szID;
	char szName[256]={0,};
	
	for(;;)
	{
		num = m_UserList.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;
		szID = m_UserList.GetItemText(num, 0);
		int UNum = m_UserList.GetItemData(num);
		if(strcmp(id, szID.operator LPCTSTR())==0 && UNum == unum)
		{
			// 레벨
			LV_ITEM item;
			item.mask = LVIF_IMAGE;
			item.iItem = num;
			item.iSubItem = 0;
			item.iImage = level;
			m_UserList.SetItem(&item);

			
			wsprintf(szName, "%I64d", money);
			m_UserList.SetItemText(num, 2, szName);



			
			// 위치
			switch(seq)
			{
			case 0: wsprintf(szName, "1"); break;
			case 1: wsprintf(szName, "대기"); break;
			case 2: wsprintf(szName, "2"); break;
			case 3: wsprintf(szName, "3"); break;
			}
			m_UserList.SetItemText(num, 3, szName);
			break;
		}
	}

}

void CObserverDlg::User_DelItem(int unum, char *id)
{
	int num=-1;
	CString szID;
	char szName[256]={0,};
	
	for(;;)
	{
		num = m_UserList.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;
		szID = m_UserList.GetItemText(num, 0);
		int UNum = m_UserList.GetItemData(num);
		if(strcmp(id, szID.operator LPCTSTR())==0 && UNum == unum) {m_UserList.DeleteItem(num);	break;}
	}

}

void CObserverDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
	
	if(Back.m_hObject) Back.DeleteObject();
	Back.LoadBitmapFile(".\\image\\Observer\\ObserverBack.bmp");

	HRGN h = BitmapToRegion( Back.operator HBITMAP(), RGB(255,0,255));
	if(h) SetWindowRgn(h,TRUE);// 사용방법

	CBitmap *pOldBmp = cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	cdc.SelectObject(pOldBmp);
	::DeleteObject(h);
	h = NULL;
	cdc.DeleteDC();
	Back.DeleteObject();
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CObserverDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	Back.DeleteObject();
	
}

void CObserverDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
		
	CDialog::OnLButtonDown(nFlags, point);
}

void CObserverDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	g_pGameDlg->SetFocus();
	
}

void CObserverDlg::OnDblclkListUser(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	nUserSel = pNMListView->iItem;
	SendMessage(WM_COMMAND, IDC_BTN_PROFILE);
	*pResult = 0;
}

void CObserverDlg::User_SetItemReservation(int unum, char *id, int seq)
{
	int num=-1;
	CString szID;
	char szName[256]={0,};
	
	for(;;)
	{
		num = m_UserList.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;
		szID = m_UserList.GetItemText(num, 0);
		int UNum = m_UserList.GetItemData(num);
		if(strcmp(id, szID.operator LPCTSTR())==0 && UNum == unum)
		{
			// 위치
			wsprintf(szName, "%d순위", seq+1);
			m_UserList.SetItemText(num, 3, szName);
			break;
		}
	}

}

void CObserverDlg::OnButtonIn() 
{
	// TODO: Add your control notification handler code here
	g_pGameView->SendMessage(WM_COMMAND, IDM_PARTICIPATION);
	
}

void CObserverDlg::OnButtonOut() 
{
	// TODO: Add your control notification handler code here
	g_pGameView->SendMessage(WM_COMMAND, IDM_EMERGE);
	
}


void CObserverDlg::SetObserverViewBtn()
{

	m_OutBtn.ShowWindow(SW_HIDE);
	m_InBtn.ShowWindow(SW_HIDE);

	if(g_MyObserver == TRUE) //관전 -> 참여
	{
		m_InBtn.ShowWindow(SW_SHOW);
	}
	else                     //참여 -> 관전
	{
		m_OutBtn.ShowWindow(SW_SHOW); // 관전
	}

}
