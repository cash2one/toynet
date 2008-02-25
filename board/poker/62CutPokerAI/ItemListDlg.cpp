// ItemListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "ItemListDlg.h"
#include "MyBitmap.h"

#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// for static members
BOOL CItemListDlg::m_bNowAskRefresh = FALSE;

/////////////////////////////////////////////////////////////////////////////
// CItemListDlg dialog


CItemListDlg::CItemListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CItemListDlg::IDD, pParent)
{
	m_bItemUsed = FALSE;

	m_DefSelItemKind = 0;

	//{{AFX_DATA_INIT(CItemListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CItemListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CItemListDlg)
	DDX_Control(pDX, IDOK, m_OkBtn);
	DDX_Control(pDX, IDC_LIST_GAMEITEM, m_ItemList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CItemListDlg, CDialog)
	//{{AFX_MSG_MAP(CItemListDlg)
	ON_WM_PAINT()
	ON_NOTIFY(NM_CLICK, IDC_LIST_GAMEITEM, OnClickListGameitem)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_GAMEITEM, OnDblclkListGameitem)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemListDlg message handlers

BOOL CItemListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);

	// 버튼 초기화
	m_OkBtn.Init("image\\gameitem\\btn_ItemUse.bmp", 4);

	InitList();

	RebuildItemList();

	if(m_StrMsg.GetLength() == 0) {
		m_StrMsg = "아이템을 사용하시려면 목록에서 아이템을 선택후 더블클릭하거나 [사용하기] 버튼을 클릭하시면 됩니다.";
	}

	m_bItemUsed = FALSE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CItemListDlg::OnOK() 
{
	// TODO: Add extra validation here

	// 게임 아이템 사용 요청을 한다
	UseSelectedGameItem();
	
	////////

	//CDialog::OnOK();
}

void CItemListDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	//CDialog::OnCancel();
}

void CItemListDlg::InitList()
{
	// 이미지 리스트 생성
	if(!m_ItemImage.GetSafeHandle()) {
		// 비트맵 읽기
		CBitmap Bmp;
		Bmp.LoadBitmap(IDB_IMAGE_GAMEITEM);

		m_ItemImage.Create(32, 32, ILC_COLOR8 | ILC_MASK, 4, 0);
		m_ItemImage.Add(&Bmp, RGB(255,0,255));
	}

	m_ItemList.SetImageList(&m_ItemImage, LVSIL_SMALL);
	m_ItemList.SetFullRowSel(TRUE);

	m_ItemList.Init(TRUE, TRUE, 0, RGB(245,245,255), RGB(0,0,0));
	m_ItemList.SetHeaderColors(RGB(120,160,175), RGB(0,0,0));
	m_ItemList.SetScrollBarChange(TRUE);
	//m_ItemList.SetGridLineUse(TRUE);

	//m_ItemList.SetTextColor(RGB(255,255,255));
	//m_ItemList.SetBackColors(RGB(50,95,110), RGB(50,95,110));	
	m_ItemList.SetTextColor(RGB(0,80,100));
	m_ItemList.SetBackColors(RGB(255,255,255), RGB(255,255,255));

	m_ItemList.SetDrawFocusRect(FALSE);	// 아이템이 포커스 받아도 점선을 그리지 않게 설정
	
	//// Column 삽입부

	LV_COLUMN col;
	char szName[256];

	wsprintf(szName, "No");
	
	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_WIDTH | LVCF_TEXT;
	col.fmt  = LVCFMT_LEFT;
	col.cx   = 40;
	col.pszText = szName;
	col.iSubItem = 0;
	m_ItemList.InsertColumn(0, &col);

	col.fmt  = LVCFMT_LEFT;
	col.cx   = 180;
	wsprintf(szName, "아이템 이름");
	col.iSubItem = 1;
	m_ItemList.InsertColumn(1, &col);

	col.fmt  = LVCFMT_LEFT;
	col.cx   = 180;
	wsprintf(szName, "상태");
	col.iSubItem = 2;
	m_ItemList.InsertColumn(2, &col);
}

void CItemListDlg::InsertItem(GAMEITEM *pItem)
{
	// 레코드를 리스트 뷰에 삽입
	LV_ITEM item;
	char szName[256]={0,};

	int index = m_ItemList.GetItemCount();
	int image = GameItem_GetItemImageIndex(pItem->Code);

	// 아이템 인덱스
	wsprintf(szName, "%d", index + 1);
	item.mask = LVIF_TEXT | LVIF_IMAGE;
	item.pszText = szName;
	item.iItem = index;
	item.iSubItem = 0;
	item.iImage = image;
	int nItem = m_ItemList.InsertItem(&item);

	// 아이템 이름
	wsprintf(szName, "%s", GameItem_GetItemName(*pItem));
	item.iSubItem = 1;
	m_ItemList.SetItem(&item);

	// 현재 시간을 얻어서 서버의 시간으로 변환
	CTime time = CTime::GetCurrentTime() - g_CSTimeGap;
	SYSTEMTIME st;
	time.GetAsSystemTime(st);
	// 아이템 상태
	ItemState state = GameItem_GetItemState(*pItem, &st);

	if(state.iState == 0) wsprintf(szName, "%s", "사용불가");
	if(state.iState == 1) {
		if(pItem->UseNum > 1) wsprintf(szName, "사용가능(남은회수:%d)", (pItem->UseNum - pItem->Use));
		else wsprintf(szName, "%s", "사용가능");
	}
	if(state.iState == 2) wsprintf(szName, "%s", "사용중");
	if(state.iState == 3) wsprintf(szName, "%s", "기한만료");
	item.iSubItem = 2;
	m_ItemList.SetItem(&item);

	// 아이템의 색상을 결정(사용 가능한 아이템은 리스트 컨트롤의 기본 텍스트 색상을 사용)
	if(state.iState == 2) {
		// 사용중인 아이템이라면
		m_ItemList.SetItemTextColor(nItem, RGB(0,100,255));
	}
	if(state.iState == 0 || state.iState == 3) {
		// 사용할 수 없거나 기한이 만료된 아이템이라면
		m_ItemList.SetItemTextColor(nItem, RGB(150,150,150));
	}
}

void CItemListDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	DrawAll(dc);

	// Do not call CDialog::OnPaint() for painting messages
}

void CItemListDlg::DrawAll(CDC& dc) 
{
	//CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
	
	CMyBitmap Back;
	Back.LoadBitmapFile("image\\gameitem\\ItemListBack.bmp");
	CBitmap *pOldBmp = cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

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

	// 아이템 설명 찍기
	CString str;
	dc.SetTextColor(RGB(0,80,100));
	rect.SetRect(0,0,400,70);
	rect.OffsetRect(27, 13);
	dc.DrawText(m_StrMsg, &rect, DT_LEFT | DT_WORDBREAK);

	// 아이템 이미지 및 상태 찍기
	DrawGameItem(dc);

	// 아이템 목록 로딩 안내문 찍기
	m_ItemList.GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.OffsetRect(0, 40);
	dc.SelectObject(&Font1);
	dc.SetTextColor(RGB(0,40,60));
	str = "아이템 목록을 불러오는 중입니다. 잠시만 기다려주세요.";
	dc.DrawText(str, &rect, DT_CENTER);

	/*
	// 아이템 경고 메시지 찍기
	dc.SelectObject(&Font1);
	dc.SetTextColor(RGB(255,25,25));
	rect.SetRect(0,0,400,20);
	rect.OffsetRect(30, 335);
	str = "주의 - 보유하신 아이템중 현재 사용가능한 아이템만 보여집니다";
	dc.DrawText(str, &rect, DT_CENTER);
	*/
	
	cdc.SelectObject(pOldFont);
	cdc.SelectObject(pOldBmp);
	cdc.DeleteDC();
	Font1.DeleteObject();
	Back.DeleteObject();

	// Do not call CDialog::OnPaint() for painting messages
}

void CItemListDlg::DrawGameItem(CDC& dc)
{
	int index = GetSelectedItemIndex();
	if(index < 0) return;

	// 현재 시간을 얻어서 서버의 시간으로 변환
	CTime time = CTime::GetCurrentTime() - g_CSTimeGap;
	SYSTEMTIME st;
	time.GetAsSystemTime(st);

	// 아이템의 상태를 얻기
	ItemState state = GameItem_GetItemState(g_MyGameItem.Item[index], &st);

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

	CFont Font2;
	Font2.CreateFont(12,0,0,0,
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

	// 아이템 이미지 찍기
	CMyBitmap bmp;
	bmp.LoadBitmapFile(GameItem_GetItemImageFile(g_MyGameItem.Item[index].Code));
	if(bmp.GetSafeHandle()) {
		CSize bmpsize = bmp.GetBitmapDimension();
		bmp.TransDraw(dc.m_hDC, 21+6, 78+8, bmpsize.cx, bmpsize.cy, 0, 0, RGB(255,0,255));
		bmp.DeleteObject();
	}

	CString str;
	CRect rect;

	// 아이템 상태 찍기
	dc.SelectObject(&Font2);
	dc.SetTextColor(RGB(50,100,140));

	int ax = 127, ay = 83;

	str.Format("◆ 아이템명 : %s", GameItem_GetItemName(g_MyGameItem.Item[index].Code));
	dc.TextOut(ax, ay, str); ay += 20;

	if(state.iState == 0) {
		str.Format("◆ 사용 불가능한 아이템");
		dc.TextOut(ax, ay, str); ay += 20;
	}
	else if(state.iState == 1) {
		if(g_MyGameItem.Item[index].UseNum > 0) {
			str.Format("◆ 사용회수 : %d회", g_MyGameItem.Item[index].UseNum);
			dc.TextOut(ax, ay, str); ay += 20;
		}
		if(g_MyGameItem.Item[index].UseDay > 0) {
			str.Format("◆ 사용기간 : %d일", g_MyGameItem.Item[index].UseDay);
			dc.TextOut(ax, ay, str); ay += 20;
		}
	}
	else if(state.iState == 2) {
		if(g_MyGameItem.Item[index].UseNum > 0) {
			str.Format("◆ 사용회수 : %d / %d", g_MyGameItem.Item[index].Use, g_MyGameItem.Item[index].UseNum);
			dc.TextOut(ax, ay, str); ay += 20;
		}
		if(g_MyGameItem.Item[index].UseDay > 0) {
			str.Format("◆ 만기시일 : %d월 %d일 %d시 %d분", state.EndDate.GetMonth(), state.EndDate.GetDay(), state.EndDate.GetHour(), state.EndDate.GetMinute());
			dc.TextOut(ax, ay, str); ay += 20;
		}
	}
	else if(state.iState == 3) {
		str.Format("◆ 사용 기한이 만료된 아이템");
		dc.TextOut(ax, ay, str); ay += 20;
	}

	if(strlen(g_MyGameItem.Item[index].PresID) > 0) {
		dc.SelectObject(&Font1);
		str.Format("[%s]님이 선물한 아이템", g_MyGameItem.Item[index].PresID);
		dc.TextOut(ax+18, ay, str); ay += 20;
	}
}

// 사용가능한 특정 종류의 아이템을 선택
BOOL CItemListDlg::SelectUsableGameItem(int itemcode)
{
	// 현재 시간을 얻어서 서버의 시간으로 변환
	CTime time = CTime::GetCurrentTime() - g_CSTimeGap;
	SYSTEMTIME st;
	time.GetAsSystemTime(st);

	int kind = GameItem_GetItemKind(itemcode);
	if(kind == 0)
		return FALSE;

	int num = -1;
	for(;;)
	{
		num = m_ItemList.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;

		int index = ::atoi(m_ItemList.GetItemText(num, 0)) - 1;
		if(index < 0) continue;

		// 요청한 아이템과 같은 종류의 아이템인지 검사
		if( kind == GameItem_GetItemKind(g_MyGameItem.Item[index].Code) ) {	
			
			// 아이템의 상태를 얻어옴(pCurTime이 NULL이라면 시스템의 현재 시간을 기준으로 판단)
			ItemState state = GameItem_GetItemState(g_MyGameItem.Item[index], &st);

			// 사용 가능한 아이템이면 리턴
			if(!state.bNowUsing && state.iState == 1) {
				// 선택된 셀렉션으로 마크하고 선택된 리스트가 확실히 보이도록 한다
				m_ItemList.SetItemState(num, LVIS_SELECTED, LVIS_SELECTED);
				m_ItemList.EnsureVisible(num, FALSE);
				return TRUE;
			}
		}
	}

	return FALSE;
}

int CItemListDlg::GetSelectedItemIndex()
{
	// 선택된 아이템중 첫번째 아이템을 얻는다
	POSITION pos = m_ItemList.GetFirstSelectedItemPosition();
	if (pos == NULL) return -1;
	int nSel = m_ItemList.GetNextSelectedItem(pos);

	if(nSel < 0) return -1;
	if(nSel >= m_ItemList.GetItemCount()) return -1;

	CString str;
	str = m_ItemList.GetItemText(nSel, 0);
	if(str.GetLength()==0) return FALSE;

	int index = ::atoi(str.operator LPCTSTR()) - 1;
	if(index < 0) return -1;

	return index;
}

BOOL CItemListDlg::UseSelectedGameItem() 
{
	// TODO: Add extra validation here

	if(m_bItemUsed) return FALSE;

	int index = GetSelectedItemIndex();
	if(index < 0) return FALSE;

	CSV_USEGAMEITEM ugmsg;
	USEGAMEITEMINFO ug;
	strncpy(ug.ID, (char*)(LPCTSTR)g_LoginID, 15);
	ug.Item = g_MyGameItem.Item[index];

	// 현재 시간을 얻어서 서버의 시간으로 변환
	CTime time = CTime::GetCurrentTime() - g_CSTimeGap;
	SYSTEMTIME st;
	time.GetAsSystemTime(st);
	// 아이템의 상태를 얻어옴
	ItemState state = GameItem_GetItemState(g_MyGameItem.Item[index], &st);

	if(state.iState != 1) {
		// 현재 사용가능한 아이템이 아니라면
		CString strmsg;
		switch(state.iState) 
		{
		case 2 : strmsg = "이미 사용중인 아이템입니다."; break;
		default : strmsg = "사용할 수 없는 아이템입니다."; break;
		}
		AfxMessageBox(strmsg);
		return FALSE;
	}

	CString str;
	str.Format("[%s]을 사용할까요?", GameItem_GetItemName(g_MyGameItem.Item[index]));
	if(MessageBox(str, "알림", MB_YESNO) == IDNO) 
		return FALSE;

	// 게임 아이템 사용 요청을 한다
	ugmsg.Set(&ug);
	SockMan.SendData(g_MainSrvSID, ugmsg.pData, ugmsg.GetTotalSize());

	// 게임 아이템 사용 요청 플래그를 두어 중복 요청을 막는다
	m_bItemUsed = TRUE;
	
	return TRUE;
}

void CItemListDlg::AskRefresh()
{
	// 이미 목록 갱신을 요청하였으면 취소
	if(m_bNowAskRefresh) 
		return;

	// 모든 아이템을 삭제하고 아이템 리스트를 숨김
	m_ItemList.DeleteAllItems();
	m_ItemList.ShowWindow(SW_HIDE);

	// 화면을 갱신
	Invalidate(FALSE);

	// 게임 아이템 리스트를 요청을 한다
	CSV_ASK_GAMEITEMLIST agmsg;
	agmsg.Set();
	SockMan.SendData(g_MainSrvSID, agmsg.pData, agmsg.GetTotalSize());

	// 목록 갱신 요청 플래그 설정(목록 갱신이 완료되기 전까지 반복 요청하는 일이 없도록 하기 위함)
	m_bNowAskRefresh = TRUE;
}

void CItemListDlg::RefreshList()
{
	m_bNowAskRefresh = FALSE;

	if(!GetSafeHwnd()) return;

	RebuildItemList();

	// 숨겨둔 리스트를 보이게 함
	m_ItemList.ShowWindow(SW_HIDE);

	// 화면을 갱신
	Invalidate(FALSE);
}

void CItemListDlg::RebuildItemList()
{
	if(!GetSafeHwnd()) return;

	// 모든 아이템을 삭제
	m_ItemList.DeleteAllItems();
	// 모든 아이템을 다시 삽입
	for(int i=0; i<g_MyGameItem.nNum; i++) {
		InsertItem(&g_MyGameItem.Item[i]);
	}

	// 아이템 이름 컬럼을 기준으로 소트
	m_ItemList.SortByColumn(1, TRUE);

	if(m_DefSelItemKind != 0) {
		// 선택할 아이템의 종류를 예약해둔 상태라면 특정 아이템을 선택하도록 함
		SelectUsableGameItem(m_DefSelItemKind);
		// 한번만 선택되도록 플래그 해제
		m_DefSelItemKind = 0;
	}
	else {
		// 예약 아이템이 없다면 첫번째 아이템을 선택된 상태로 전환
		m_ItemList.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	}

}

void CItemListDlg::OnClickListGameitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nSel = pNMListView->iItem;

	if(nSel >= 0) {
		// 아이템 설명을 갱신하기 위하여 리프레쉬
		Invalidate(FALSE);
	}

	*pResult = 0;
}

void CItemListDlg::OnDblclkListGameitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;

	UseSelectedGameItem();
}

void CItemListDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}
