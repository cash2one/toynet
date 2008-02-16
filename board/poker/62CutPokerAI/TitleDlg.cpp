// TitleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "TitleDlg.h"

#include "OptionDlg.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTitleDlg dialog

CTitleDlg::CTitleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTitleDlg::IDD, pParent)
{
	g_pTitleDlg = this;
	m_pLocWebCtrl = NULL;	// <- �߰�
	m_pWebCtrl = NULL;
	LastSelChan = -1;
	m_TitleMsg ="";
	//{{AFX_DATA_INIT(CTitleDlg)
	m_strFindID = _T("");
	//}}AFX_DATA_INIT
}


void CTitleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTitleDlg)
	DDX_Control(pDX, IDC_RICHEDIT_NOTIC, m_ChatViewNotic);
	DDX_Control(pDX, IDC_LIST_CHANNEL, m_ChanList);
	DDX_Control(pDX, IDC_BUTTON_EXIT, m_ExitBtn);
	DDX_Control(pDX, IDC_BUTTON_ENTER, m_EnterBtn);
	DDX_Control(pDX, IDC_BUTTON_REFRESH, m_RefreshBtn);
	DDX_Control(pDX, IDC_BUTTON_CHANGEDISPLAY, m_ChangeDisplayBtn);
	DDX_Control(pDX, IDC_BUTTON_OPTION, m_OptionBtn);
	DDX_Control(pDX, IDC_BUTTON_FIND, m_FindIDBtn);
	DDX_Text(pDX, IDC_EDIT_FINDID, m_strFindID);
	DDV_MaxChars(pDX, m_strFindID, 15);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTitleDlg, CDialog)
	//{{AFX_MSG_MAP(CTitleDlg)
	ON_WM_PAINT()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CHANNEL, OnDblclkListChannel)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_ENTER, OnButtonEnter)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_FIND, OnButtonFind)
	ON_BN_CLICKED(IDC_BUTTON_OPTION, OnButtonOption)
	ON_BN_CLICKED(IDC_BUTTON_CHANGEDISPLAY, OnButtonChangedisplay)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_WINDOWPOSCHANGING()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTitleDlg message handlers

void CTitleDlg::OnOK() 
{
	// TODO: Add extra validation here
	//CDialog::OnOK();
}

void CTitleDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	//CDialog::OnCancel();
}

BOOL CTitleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	// ���̾�α� ��Ʈ�� �����ǰ� ��ġ ������
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);
	
	// ��ư �ʱ�ȭ
/*
	m_FindIDBtn.Init(IDB_TITLE_FINDID1, IDB_TITLE_FINDID2, IDB_TITLE_FINDID3, NULL);
	m_OptionBtn.Init(IDB_TITLE_OPTION1, IDB_TITLE_OPTION2, IDB_TITLE_OPTION3, NULL);
	m_ChangeDisplayBtn.Init(IDB_TITLE_FULLSCREEN1, IDB_TITLE_FULLSCREEN2, IDB_TITLE_FULLSCREEN3, NULL);
	m_RefreshBtn.Init(IDB_TITLE_REFRESH1, IDB_TITLE_REFRESH2, IDB_TITLE_REFRESH3, NULL);
	m_EnterBtn.Init(IDB_TITLE_ENTERCHAN1, IDB_TITLE_ENTERCHAN2, IDB_TITLE_ENTERCHAN3, NULL);
	m_ExitBtn.Init(IDB_TITLE_EXIT1, IDB_TITLE_EXIT2, IDB_TITLE_EXIT3, NULL);
*/
	m_FindIDBtn.Init(453, 504,".\\image\\Title\\TBtn_findid.bmp", 4);
	m_OptionBtn.Init(596, 502,".\\image\\Title\\TBtn_option.BMP", 4);
	m_ChangeDisplayBtn.Init(693, 502,".\\image\\Title\\TBtn_fullscreen.bmp", 4);
	m_RefreshBtn.Init(20, 554,".\\image\\Title\\TBtn_refresh.bmp", 4);
	m_EnterBtn.Init(193, 554,".\\image\\Title\\TBtn_channel.bmp", 4);
	m_ExitBtn.Init(653, 554,".\\image\\Title\\TBtn_exit.bmp", 4);

	Chan_InitList();
	LastSelChan = -1;
	bDontClickSound = FALSE;

	/*
	// �� ��Ʈ�� ����
	CRect rc(0,0,450,400);
	//rc.OffsetRect(328,83);
	rc.OffsetRect(328,91);

	if(!m_pWebCtrl) m_pWebCtrl = new CWebCtrl();
	m_pWebCtrl->Create(NULL, "WebControl", WS_VISIBLE|WS_CHILD, rc, this, 1321);
*/
	//m_pWebCtrl->Navigate("http://www.netmarble.net/gamedown/notice/poweral/notice.htm");

	// ��ġ �˸��� �� ��Ʈ�� ����(������ �ʴ� ������ �Űܵ�)
	CRect rc(0,0,80,80);
	//rc.OffsetRect(0,0);
	if(!m_pLocWebCtrl) m_pLocWebCtrl = new CWebCtrl();
	m_pLocWebCtrl->Create(NULL, "WebControl", WS_VISIBLE|WS_CHILD, rc, this, 1322);
	m_pLocWebCtrl->ShowWindow(SW_HIDE);

	// Ÿ��Ʋ ��� �� ��Ʈ�� ����
	CreateBanner();

	
	// [ ���� ��Ŀ ] ���� �׽�Ʈ
	rc.SetRect(0, 0, 241, 40);	
	rc.OffsetRect(270 , 37);
	m_JackPotBox.Create(this, rc, 0, 1223);
	
	// ä�� �� ��ġ ����Ʈ ��Ʈ�� �ʱ�ȭ
	m_ChatViewNotic.SetReadOnly(TRUE);
	m_ChatViewNotic.SetBackgroundColor(FALSE, RGB(61,116,161));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTitleDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	// �� ��Ʈ�� �ı�
	//if(m_pWebCtrl) { delete m_pWebCtrl; m_pWebCtrl = NULL; }
	
	// ��ġ �˸��� �� ��Ʈ�� �ı�
	if(m_pLocWebCtrl) { 
		m_pLocWebCtrl->Navigate("about:blank");
		delete m_pLocWebCtrl; m_pLocWebCtrl = NULL; 
	}
	// Ÿ��Ʋ ��� �� ��Ʈ�� �ı�
	DestroyBanner();
}

void CTitleDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	DrawBkgnd(dc);

	// Do not call CDialog::OnPaint() for painting messages
}

void CTitleDlg::Chan_InitList()
{
	// ��Ʈ�� �б�
	CBitmap Bmp;
	Bmp.LoadBitmap(IDB_IMAGE_CHAN);

	// �̹��� ����Ʈ ����
	m_ChanImage.Create(32, 16, ILC_COLOR8 | ILC_MASK, 11, 0);
	m_ChanImage.Add(&Bmp, RGB(255,255,255));

	m_ChanList.SetImageList(&m_ChanImage, LVSIL_SMALL);
	m_ChanList.SetFullRowSel(TRUE); 	// ### [ ä�� ���� ���� ] ###
	m_ChanList.SetTextBkColor(RGB(45,33,55));
//	m_ChanList.SetBkColor(RGB(45,33,55));
//	m_ChanList.SetTextColor(RGB(235,255,235));

	// ��ü�� ����Ʈ ����
	m_ChanList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	// ### [ ä�� ���� ���� ] ###
	//////////////////////////////////////////////////////////////////////////////
	//m_ChanList.Init(FALSE, TRUE, 0, RGB(45,33,55), RGB(235,255,235));
	m_ChanList.Init(FALSE, TRUE, 0, RGB(255,255,255), RGB(235,255,235));
	m_ChanList.SetHeaderColors(RGB(210,210,255), RGB(0,0,0));
	m_ChanList.SetScrollBarChange(TRUE);
	//////////////////////////////////////////////////////////////////////////////

	//// Column ���Ժ�
	LV_COLUMN col;
	char szName[256];

	wsprintf(szName, "����");
	
	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_WIDTH | LVCF_TEXT;
	col.fmt  = LVCFMT_LEFT;
	col.cx   = 40;
	col.pszText = szName;
	col.iSubItem = 0;
	m_ChanList.InsertColumn(0, &col);

	col.fmt  = LVCFMT_CENTER;
	col.cx   = 140;
	wsprintf(szName, "ä�θ�");
	col.iSubItem = 1;
	m_ChanList.InsertColumn(2, &col);

	col.fmt  = LVCFMT_RIGHT;
	col.cx   = 60;
	wsprintf(szName, "�ο�");
	col.iSubItem = 2;
	m_ChanList.InsertColumn(4, &col);
}

void CTitleDlg::Chan_InsItem(int cnum, char *title, int nowuser, int maxuser, int channelstyle)
{
	// ���ڵ带 ����Ʈ �信 ����
	LV_ITEM item;
	char szName[256]={0,};

	int image = nowuser / (maxuser/10);
	if(nowuser % (maxuser/10)) image += 1;

	// ä�ι�ȣ
	//wsprintf(szName, "%d", cnum);
	wsprintf(szName, " ");
	item.mask = LVIF_TEXT | LVIF_IMAGE;
	item.pszText = szName;
	item.iItem = m_ChanList.GetItemCount();
	item.iSubItem = 0;
	item.iImage=image;

	// 2003 0620 ī����� ä�� ǥ�� �� ���� ������ ��� ����
	int nItem = m_ChanList.InsertItem(&item);
	int grade = GetMyLeveltoGrade(Play[0].UI.nIcon); // ��� ����
	
	//////////////////////////////////////////////////////////////////////////////
	// ### [ ä�� ���� ���� ] ###
	// ä�� ����Ʈ ���� ����
	if(channelstyle == 3 && grade == 4) {// ������ ��� ���� ���尡��(��Ŀ2)
		m_ChanList.SetItemTextColor(nItem, RGB(0,0,0));	
	}
/*
	else if(channelstyle == 1 && channelstyle < grade) // �ʺ�����ä�� (���̷ο�, �Ƕ�)
	{// ����Ұ�
		m_ChanList.SetItemTextColor(nItem, RGB(128,128,128));	
	}
	else if(channelstyle == 0 || channelstyle <= grade )  // ����ä�� ��� ���� ����(���̷ο�, �Ƕ�)
*/	else if(channelstyle == 0 || channelstyle == grade )  {// ����ä��, ���尡�� ��� (��Ŀ2)
		m_ChanList.SetItemTextColor(nItem, RGB(0,0,0));	
	}
	else if(channelstyle == grade+1)  { // ���� ��밡��
		m_ChanList.SetItemTextColor(nItem, RGB(255,128,0));	
	}
	else { // ����Ұ�
		m_ChanList.SetItemTextColor(nItem, RGB(128,128,128));	
	}
	//////////////////////////////////////////////////////////////////////////////

	// ä�θ�
	wsprintf(szName, "%s", title);
	item.iSubItem = 1;
	m_ChanList.SetItem(&item);

	//////////////////////////////////////////////////////////////////////////////
	// ä�� ����Ʈ ���� ����
//	m_ChanList.SetItemTextColor(nItem, RGB(235,255,235));	// [���� ������ �۾�]
	/////////////////////////////////////////////////////////////////////////

	// �ο�
	wsprintf(szName, "%d/%d", nowuser, maxuser);
	item.iSubItem = 2;
	m_ChanList.SetItem(&item);

	// ä�� ��ȣ ����
	m_ChanList.SetItemData(item.iItem, cnum);

}

void CTitleDlg::Chan_DeleteList()
{
	m_ChanList.DeleteAllItems();
}

void CTitleDlg::Chan_SelectLast()
{
	if(LastSelChan != -1) {
		m_ChanList.SetItemState(LastSelChan, LVIS_SELECTED, LVIS_SELECTED);
	}
}

CString CTitleDlg::GetChannelName(int cnum)
{
	CString str;
	if(cnum < 0) return str;
	
	int num = -1;
	for(;;) {
		num = m_ChanList.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;
		if(cnum == (int)m_ChanList.GetItemData(num)) {
			str = m_ChanList.GetItemText(num, 1);
			break;
		}
	}

	return str;
}

void CTitleDlg::OnDblclkListChannel(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnButtonEnter();
	*pResult = 0;
}

void CTitleDlg::OnButtonRefresh() 
{
	// TODO: Add your control notification handler code here

	if(g_Where != WH_TITLE) return; // <- �߰�
	if(g_bChannel ) return; // <- �߰�  ������ ���� ��ư ��Ȱ��ȭ

	if(!bDontClickSound) Sound.Play(SND45);
	bDontClickSound = FALSE;

	// ������ ���� �׸��� ����
	// ���õ� �������� ù��° �������� ��´�
	POSITION pos = m_ChanList.GetFirstSelectedItemPosition();
	if(pos == NULL) LastSelChan = -1;
	else LastSelChan = m_ChanList.GetNextSelectedItem(pos);

	Chan_DeleteList();

	// ��ü ä�� ����� �䱸��
	CSV_ASK_ALLCHANINFO acmsg;
	acmsg.Set();
	SockMan.SendData(g_MainSrvSID, acmsg.pData, acmsg.GetTotalSize());

	GetDlgItem(IDC_BUTTON_REFRESH)->EnableWindow(FALSE);

	// �޽���
	m_MsgDlg.MsgBox(3, "��ü ä�� ����� �����޴� ���Դϴ�");
}

void CTitleDlg::OnButtonEnter() 
{
	// TODO: Add your control notification handler code here
	if(g_Wait == TRUE) return;
	if(g_Where != WH_TITLE) return; // <- �߰�
	if(g_bChannel ) return; // <- �߰�  ������ ���� ��ư ��Ȱ��ȭ

	Sound.Play(SND45);

	// ���õ� �������� ù��° �������� ��´�
	POSITION pos = m_ChanList.GetFirstSelectedItemPosition();
	if (pos == NULL) return;
	int nNowSel = m_ChanList.GetNextSelectedItem(pos);
	
	if(nNowSel < 0) return;
	if(nNowSel >= m_ChanList.GetItemCount()) return;

	int cnum = m_ChanList.GetItemData(nNowSel);
	if(cnum < 0) return;

	g_Wait = TRUE;

	GetDlgItem(IDC_BUTTON_ENTER)->EnableWindow(FALSE);

	// ä�ο� �����ϱ⸦ �䱸
	CSV_ASK_ENTERCHAN aemsg;
	aemsg.Set(cnum);
	SockMan.SendData(g_MainSrvSID, aemsg.pData, aemsg.GetTotalSize());

	// �޽���
	m_MsgDlg.MsgBox(3, "��û�� ä�ο� �������Դϴ�");
}

void CTitleDlg::OnButtonFind() 
{
	// TODO: Add your control notification handler code here
	Sound.Play(SND45);
	
	UpdateData(TRUE);
	if(m_strFindID.GetLength() <= 0) return;

	GetDlgItem(IDC_EDIT_FINDID)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(FALSE);

	CSV_ASK_FINDID afmsg;
	afmsg.Set((char*)(LPCTSTR)m_strFindID);
	SockMan.SendData(g_MainSrvSID, afmsg.pData, afmsg.GetTotalSize());
}

void CTitleDlg::OnButtonOption() 
{
	// TODO: Add your control notification handler code here
	Sound.Play(SND45);
	COptionDlg Dlg;
	Dlg.DoModal();
}

void CTitleDlg::OnButtonChangedisplay() 
{
	// TODO: Add your control notification handler code here
	g_pLobyDlg->SendMessage(WM_COMMAND, IDC_BUTTON_CHANGEDISPLAY);
}

void CTitleDlg::OnButtonExit() 
{
	// TODO: Add your control notification handler code here
	Sound.Play(SND45);
	g_pMainDlg->OnCancel();
}

// �� �Լ��� �߰�
void CTitleDlg::CreateBanner()
{
	if(m_pWebCtrl) return;

	// �� ��Ʈ�� ����
	CRect rc(0,0,450,400);
	rc.OffsetRect(329,93);
	if(!m_pWebCtrl) m_pWebCtrl = new CWebCtrl();
	m_pWebCtrl->Create(NULL, "WebControl", WS_VISIBLE|WS_CHILD, rc, this, 1321);
}

// �� �Լ��� �߰�
void CTitleDlg::DestroyBanner()
{
	if(!m_pWebCtrl) return;
	delete m_pWebCtrl;
	m_pWebCtrl = NULL;
}



void CTitleDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rc;
	rc.SetRect( 0, 0, 790, 28 );
	POINT pt;
	pt.x = point.x;
	pt.y = point.y;
	if( rc.PtInRect( pt ) )
	{
		g_pMainDlg->SendMessage( WM_LBUTTONDOWN, MK_LBUTTON, 0 );
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}


void CTitleDlg::DrawBkgnd(CDC& dc)
{
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
	
	if(Back.m_hObject) Back.DeleteObject();
	Back.LoadBitmapFile(".\\image\\Title\\TitleBack.bmp");
	CBitmap *pOldBmp = cdc.SelectObject(&Back);

	
//	dc.TextOut(30, 18, m_TitleMsg);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);



		// ��Ʈ ����
	CFont Font1;
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
		"����");


	CFont* pOldFont = dc.SelectObject(&Font1);

	//////////////////////////////////////////////
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor( RGB(255, 255, 255) );
	dc.TextOut(120, 8, m_TitleMsg);
	//////////////////////////////////////////////

	dc.SelectObject(pOldFont);
	Font1.DeleteObject();


	cdc.SelectObject(pOldBmp);
	cdc.DeleteDC();
	Back.DeleteObject();
}

BOOL CTitleDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	DrawBkgnd(*pDC);
	return TRUE;
	
	//return CDialog::OnEraseBkgnd(pDC);
}

void CTitleDlg::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CDialog::OnWindowPosChanging(lpwndpos);
	
	// TODO: Add your message handler code here
//	lpwndpos->flags &= ~SWP_SHOWWINDOW;
	
}
