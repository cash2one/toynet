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

	// ���̾�α� ��Ʈ�� �����ǰ� ��ġ ������
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);

	// ��ư �ʱ�ȭ
	m_OkBtn.Init("image\\gameitem\\btn_ItemUse.bmp", 4);

	InitList();

	RebuildItemList();

	if(m_StrMsg.GetLength() == 0) {
		m_StrMsg = "�������� ����Ͻ÷��� ��Ͽ��� �������� ������ ����Ŭ���ϰų� [����ϱ�] ��ư�� Ŭ���Ͻø� �˴ϴ�.";
	}

	m_bItemUsed = FALSE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CItemListDlg::OnOK() 
{
	// TODO: Add extra validation here

	// ���� ������ ��� ��û�� �Ѵ�
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
	// �̹��� ����Ʈ ����
	if(!m_ItemImage.GetSafeHandle()) {
		// ��Ʈ�� �б�
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

	m_ItemList.SetDrawFocusRect(FALSE);	// �������� ��Ŀ�� �޾Ƶ� ������ �׸��� �ʰ� ����
	
	//// Column ���Ժ�

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
	wsprintf(szName, "������ �̸�");
	col.iSubItem = 1;
	m_ItemList.InsertColumn(1, &col);

	col.fmt  = LVCFMT_LEFT;
	col.cx   = 180;
	wsprintf(szName, "����");
	col.iSubItem = 2;
	m_ItemList.InsertColumn(2, &col);
}

void CItemListDlg::InsertItem(GAMEITEM *pItem)
{
	// ���ڵ带 ����Ʈ �信 ����
	LV_ITEM item;
	char szName[256]={0,};

	int index = m_ItemList.GetItemCount();
	int image = GameItem_GetItemImageIndex(pItem->Code);

	// ������ �ε���
	wsprintf(szName, "%d", index + 1);
	item.mask = LVIF_TEXT | LVIF_IMAGE;
	item.pszText = szName;
	item.iItem = index;
	item.iSubItem = 0;
	item.iImage = image;
	int nItem = m_ItemList.InsertItem(&item);

	// ������ �̸�
	wsprintf(szName, "%s", GameItem_GetItemName(*pItem));
	item.iSubItem = 1;
	m_ItemList.SetItem(&item);

	// ���� �ð��� �� ������ �ð����� ��ȯ
	CTime time = CTime::GetCurrentTime() - g_CSTimeGap;
	SYSTEMTIME st;
	time.GetAsSystemTime(st);
	// ������ ����
	ItemState state = GameItem_GetItemState(*pItem, &st);

	if(state.iState == 0) wsprintf(szName, "%s", "���Ұ�");
	if(state.iState == 1) {
		if(pItem->UseNum > 1) wsprintf(szName, "��밡��(����ȸ��:%d)", (pItem->UseNum - pItem->Use));
		else wsprintf(szName, "%s", "��밡��");
	}
	if(state.iState == 2) wsprintf(szName, "%s", "�����");
	if(state.iState == 3) wsprintf(szName, "%s", "���Ѹ���");
	item.iSubItem = 2;
	m_ItemList.SetItem(&item);

	// �������� ������ ����(��� ������ �������� ����Ʈ ��Ʈ���� �⺻ �ؽ�Ʈ ������ ���)
	if(state.iState == 2) {
		// ������� �������̶��
		m_ItemList.SetItemTextColor(nItem, RGB(0,100,255));
	}
	if(state.iState == 0 || state.iState == 3) {
		// ����� �� ���ų� ������ ����� �������̶��
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

	// ��Ʈ ����
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
		"����");

	dc.SetBkMode(TRANSPARENT);

	CFont* pOldFont = dc.SelectObject(&Font1);

	// ������ ���� ���
	CString str;
	dc.SetTextColor(RGB(0,80,100));
	rect.SetRect(0,0,400,70);
	rect.OffsetRect(27, 13);
	dc.DrawText(m_StrMsg, &rect, DT_LEFT | DT_WORDBREAK);

	// ������ �̹��� �� ���� ���
	DrawGameItem(dc);

	// ������ ��� �ε� �ȳ��� ���
	m_ItemList.GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.OffsetRect(0, 40);
	dc.SelectObject(&Font1);
	dc.SetTextColor(RGB(0,40,60));
	str = "������ ����� �ҷ����� ���Դϴ�. ��ø� ��ٷ��ּ���.";
	dc.DrawText(str, &rect, DT_CENTER);

	/*
	// ������ ��� �޽��� ���
	dc.SelectObject(&Font1);
	dc.SetTextColor(RGB(255,25,25));
	rect.SetRect(0,0,400,20);
	rect.OffsetRect(30, 335);
	str = "���� - �����Ͻ� �������� ���� ��밡���� �����۸� �������ϴ�";
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

	// ���� �ð��� �� ������ �ð����� ��ȯ
	CTime time = CTime::GetCurrentTime() - g_CSTimeGap;
	SYSTEMTIME st;
	time.GetAsSystemTime(st);

	// �������� ���¸� ���
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
		"����");

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
		"����");

	// ������ �̹��� ���
	CMyBitmap bmp;
	bmp.LoadBitmapFile(GameItem_GetItemImageFile(g_MyGameItem.Item[index].Code));
	if(bmp.GetSafeHandle()) {
		CSize bmpsize = bmp.GetBitmapDimension();
		bmp.TransDraw(dc.m_hDC, 21+6, 78+8, bmpsize.cx, bmpsize.cy, 0, 0, RGB(255,0,255));
		bmp.DeleteObject();
	}

	CString str;
	CRect rect;

	// ������ ���� ���
	dc.SelectObject(&Font2);
	dc.SetTextColor(RGB(50,100,140));

	int ax = 127, ay = 83;

	str.Format("�� �����۸� : %s", GameItem_GetItemName(g_MyGameItem.Item[index].Code));
	dc.TextOut(ax, ay, str); ay += 20;

	if(state.iState == 0) {
		str.Format("�� ��� �Ұ����� ������");
		dc.TextOut(ax, ay, str); ay += 20;
	}
	else if(state.iState == 1) {
		if(g_MyGameItem.Item[index].UseNum > 0) {
			str.Format("�� ���ȸ�� : %dȸ", g_MyGameItem.Item[index].UseNum);
			dc.TextOut(ax, ay, str); ay += 20;
		}
		if(g_MyGameItem.Item[index].UseDay > 0) {
			str.Format("�� ���Ⱓ : %d��", g_MyGameItem.Item[index].UseDay);
			dc.TextOut(ax, ay, str); ay += 20;
		}
	}
	else if(state.iState == 2) {
		if(g_MyGameItem.Item[index].UseNum > 0) {
			str.Format("�� ���ȸ�� : %d / %d", g_MyGameItem.Item[index].Use, g_MyGameItem.Item[index].UseNum);
			dc.TextOut(ax, ay, str); ay += 20;
		}
		if(g_MyGameItem.Item[index].UseDay > 0) {
			str.Format("�� ������� : %d�� %d�� %d�� %d��", state.EndDate.GetMonth(), state.EndDate.GetDay(), state.EndDate.GetHour(), state.EndDate.GetMinute());
			dc.TextOut(ax, ay, str); ay += 20;
		}
	}
	else if(state.iState == 3) {
		str.Format("�� ��� ������ ����� ������");
		dc.TextOut(ax, ay, str); ay += 20;
	}

	if(strlen(g_MyGameItem.Item[index].PresID) > 0) {
		dc.SelectObject(&Font1);
		str.Format("[%s]���� ������ ������", g_MyGameItem.Item[index].PresID);
		dc.TextOut(ax+18, ay, str); ay += 20;
	}
}

// ��밡���� Ư�� ������ �������� ����
BOOL CItemListDlg::SelectUsableGameItem(int itemcode)
{
	// ���� �ð��� �� ������ �ð����� ��ȯ
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

		// ��û�� �����۰� ���� ������ ���������� �˻�
		if( kind == GameItem_GetItemKind(g_MyGameItem.Item[index].Code) ) {	
			
			// �������� ���¸� ����(pCurTime�� NULL�̶�� �ý����� ���� �ð��� �������� �Ǵ�)
			ItemState state = GameItem_GetItemState(g_MyGameItem.Item[index], &st);

			// ��� ������ �������̸� ����
			if(!state.bNowUsing && state.iState == 1) {
				// ���õ� ���������� ��ũ�ϰ� ���õ� ����Ʈ�� Ȯ���� ���̵��� �Ѵ�
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
	// ���õ� �������� ù��° �������� ��´�
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

	// ���� �ð��� �� ������ �ð����� ��ȯ
	CTime time = CTime::GetCurrentTime() - g_CSTimeGap;
	SYSTEMTIME st;
	time.GetAsSystemTime(st);
	// �������� ���¸� ����
	ItemState state = GameItem_GetItemState(g_MyGameItem.Item[index], &st);

	if(state.iState != 1) {
		// ���� ��밡���� �������� �ƴ϶��
		CString strmsg;
		switch(state.iState) 
		{
		case 2 : strmsg = "�̹� ������� �������Դϴ�."; break;
		default : strmsg = "����� �� ���� �������Դϴ�."; break;
		}
		AfxMessageBox(strmsg);
		return FALSE;
	}

	CString str;
	str.Format("[%s]�� ����ұ��?", GameItem_GetItemName(g_MyGameItem.Item[index]));
	if(MessageBox(str, "�˸�", MB_YESNO) == IDNO) 
		return FALSE;

	// ���� ������ ��� ��û�� �Ѵ�
	ugmsg.Set(&ug);
	SockMan.SendData(g_MainSrvSID, ugmsg.pData, ugmsg.GetTotalSize());

	// ���� ������ ��� ��û �÷��׸� �ξ� �ߺ� ��û�� ���´�
	m_bItemUsed = TRUE;
	
	return TRUE;
}

void CItemListDlg::AskRefresh()
{
	// �̹� ��� ������ ��û�Ͽ����� ���
	if(m_bNowAskRefresh) 
		return;

	// ��� �������� �����ϰ� ������ ����Ʈ�� ����
	m_ItemList.DeleteAllItems();
	m_ItemList.ShowWindow(SW_HIDE);

	// ȭ���� ����
	Invalidate(FALSE);

	// ���� ������ ����Ʈ�� ��û�� �Ѵ�
	CSV_ASK_GAMEITEMLIST agmsg;
	agmsg.Set();
	SockMan.SendData(g_MainSrvSID, agmsg.pData, agmsg.GetTotalSize());

	// ��� ���� ��û �÷��� ����(��� ������ �Ϸ�Ǳ� ������ �ݺ� ��û�ϴ� ���� ������ �ϱ� ����)
	m_bNowAskRefresh = TRUE;
}

void CItemListDlg::RefreshList()
{
	m_bNowAskRefresh = FALSE;

	if(!GetSafeHwnd()) return;

	RebuildItemList();

	// ���ܵ� ����Ʈ�� ���̰� ��
	m_ItemList.ShowWindow(SW_HIDE);

	// ȭ���� ����
	Invalidate(FALSE);
}

void CItemListDlg::RebuildItemList()
{
	if(!GetSafeHwnd()) return;

	// ��� �������� ����
	m_ItemList.DeleteAllItems();
	// ��� �������� �ٽ� ����
	for(int i=0; i<g_MyGameItem.nNum; i++) {
		InsertItem(&g_MyGameItem.Item[i]);
	}

	// ������ �̸� �÷��� �������� ��Ʈ
	m_ItemList.SortByColumn(1, TRUE);

	if(m_DefSelItemKind != 0) {
		// ������ �������� ������ �����ص� ���¶�� Ư�� �������� �����ϵ��� ��
		SelectUsableGameItem(m_DefSelItemKind);
		// �ѹ��� ���õǵ��� �÷��� ����
		m_DefSelItemKind = 0;
	}
	else {
		// ���� �������� ���ٸ� ù��° �������� ���õ� ���·� ��ȯ
		m_ItemList.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	}

}

void CItemListDlg::OnClickListGameitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nSel = pNMListView->iItem;

	if(nSel >= 0) {
		// ������ ������ �����ϱ� ���Ͽ� ��������
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
