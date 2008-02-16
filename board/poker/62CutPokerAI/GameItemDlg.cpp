// GameItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "GameItemDlg.h"
#include "DialogCtrlReposition.h"
#include "MyBitmap.h"
#include "SysUtil.h"

#include "Global.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ��� �̹��� ���ϸ�
static const LPCTSTR BACKIMAGE_NAME = "image\\gameitem\\GameItemBack.bmp";

/////////////////////////////////////////////////////////////////////////////
// CGameItemDlg dialog


CGameItemDlg::CGameItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGameItemDlg::IDD, pParent)
{
	m_bModal = FALSE;
	m_pHelpWeb = NULL;
	//m_pShopWeb = NULL;
	m_CurrentTab = 0;
	m_DefShopItemKind = 0;

	//{{AFX_DATA_INIT(CGameItemDlg)
	a = NULL;
	//}}AFX_DATA_INIT
}


CGameItemDlg::~CGameItemDlg()
{
}

void CGameItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGameItemDlg)
	DDX_Control(pDX, IDC_BUTTON_CASH, m_CashBtn);
	DDX_Control(pDX, IDC_BUTTON_SHOP, m_ShopBtn);
	DDX_Control(pDX, IDC_BUTTON_ITEM, m_ItemBtn);
	DDX_Control(pDX, IDC_BUTTON_HELP, m_HelpBtn);
	DDX_Control(pDX, IDCANCEL, m_CancelBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGameItemDlg, CDialog)
	//{{AFX_MSG_MAP(CGameItemDlg)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_HELP, OnButtonHelp)
	ON_BN_CLICKED(IDC_BUTTON_ITEM, OnButtonItem)
	ON_BN_CLICKED(IDC_BUTTON_SHOP, OnButtonShop)
	ON_BN_CLICKED(IDC_BUTTON_CASH, OnButtonCash)
	ON_WM_PAINT()
	ON_WM_WINDOWPOSCHANGING()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameItemDlg message handlers

BOOL CGameItemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// ���̾�α� ��Ʈ�� �����ǰ� ��ġ ������
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);

	// ��� ��Ʈ�� ����� �°� ������ ������ ����
	WindowResize2Bitmap(m_hWnd, BACKIMAGE_NAME);

	// ��ư �ʱ�ȭ
	m_CancelBtn.Init(455, 5, "image\\gameitem\\btn_exit.bmp", 4);

	// �ǿ� ��ư �ʱ�ȭ
	m_HelpBtn.Init( 11, 23, "image\\gameitem\\btn_help.bmp", 4);
	m_ItemBtn.Init(111, 23, "image\\gameitem\\btn_item.bmp", 4);
	m_ShopBtn.Init(211, 23, "image\\gameitem\\btn_shop.bmp", 4);
	m_CashBtn.Init(311, 23, "image\\gameitem\\btn_cash.bmp", 4);

	// ���ϵ� ���̾�α� ����
	{
		// ������ ���� �� ��Ʈ�� ����
		CRect helprc(0,0,100,100);
		helprc.OffsetRect(0,0);
		if(!m_pHelpWeb) m_pHelpWeb = new CWebCtrl();
		m_pHelpWeb->Create(NULL, "ItemHelp", WS_VISIBLE|WS_CHILD, helprc, this, 1331);

		// ������ ����Ʈ ���̾�α� ����
		m_ItemListDlg.Create(m_ItemListDlg.IDD, this);

		// ���� �� ��Ʈ�� ����
		CRect shoprc(0,0,100,100);
		shoprc.OffsetRect(0,0);		
	}

	//CRect rc;
	//GetClientRect(&rc);
	//m_GameItemTab.MoveWindow(&rc);

	// �� ��Ʈ�� ���� �پ���� �����츦 �� Ŭ���̾�Ʈ ���� ���� ������ ����
	{
		CRect rc(0,0,463,355);
		rc.OffsetRect(9,58);
		//m_GameItemTab.GetClientRect(&rc);
		//m_GameItemTab.AdjustRect(FALSE, &rc);
		//rc.DeflateRect(5,5,5,5);

		if(m_pHelpWeb) m_pHelpWeb->MoveWindow(&rc, TRUE);
		if(m_ItemListDlg.GetSafeHwnd()) m_ItemListDlg.MoveWindow(&rc, TRUE);
	}

	///////////////

	m_bHelpWebNavigated = FALSE;
	m_bShopWebNavigated = FALSE;

	///////////////

	ChangeTab(m_FirstTab);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGameItemDlg::OnOK() 
{
	// TODO: Add extra validation here
}

void CGameItemDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	// ���� ���̾�αװ� �ڷ� ������� ��츦 ���� ����
	if(g_pMainDlg->GetSafeHwnd()) {
		g_pMainDlg->SetFocus();
	}

	if(!m_bModal) {
		DestroyWindow();
		return;
	}

	CDialog::OnCancel();
}

int CGameItemDlg::DoModal(int firsttab, BOOL bRefreshList, int iDefSelItemKind) 
{
	// TODO: Add your specialized code here and/or call the base class

	// �̹� ���̾�αװ� ������ ���¶�� �ı�	[�����̾� �۾�]
	if(GetSafeHwnd()) SendMessage(WM_COMMAND, IDCANCEL);

	m_bModal = TRUE;

	m_FirstTab = firsttab;
	m_bRefreshList = bRefreshList;

	m_DefShopItemKind = 0;
	m_ItemListDlg.m_DefSelItemKind = 0;

	// �������� �������� �� �� ����ǵ��� ����
	m_ItemListDlg.m_DefSelItemKind = iDefSelItemKind;
	// ������ �������� Ư�� �������� ���� ���� ����Ʈ�� �ǵ��� ����
	m_DefShopItemKind = iDefSelItemKind;

	return CDialog::DoModal();
}

BOOL CGameItemDlg::DoModaless(int firsttab, BOOL bRefreshList, int iDefSelItemKind)
{
	if(GetSafeHwnd()) 
		return FALSE;

	m_bModal = FALSE;

	m_FirstTab = firsttab;
	m_bRefreshList = bRefreshList;

	m_DefShopItemKind = 0;
	m_ItemListDlg.m_DefSelItemKind = 0;

	// �������� �������� �� �� ����ǵ��� ����
	m_ItemListDlg.m_DefSelItemKind = iDefSelItemKind;
	// ������ �������� Ư�� �������� ���� ���� ����Ʈ�� �ǵ��� ����
	m_DefShopItemKind = iDefSelItemKind;

	BOOL rtn = Create(IDD, g_pMainDlg);
	if(rtn) ShowWindow(SW_SHOW);

	return rtn;
}

void CGameItemDlg::OpenShopPage() 
{
	#if (0)
	{
		//
		// ���ڸ� POST ������� �����ϴ� ���� (��� ����)
		//
	
		//m_pShopWeb->Navigate("about:blank");
		
		// �ѱ� ĳ���� ������ ����
		m_pShopWeb->PutCharSet("euc-kr");

		// Html �ڵ� ���� - POST ������� ���ڸ� �����ϱ� ����
		CString strHtml;
		strHtml.Format("<Form Name='Send' Method='POST' Action='%s'>\
			<Input type=hidden Name='id' value='%s'>\
			<Input type=hidden Name='pass' value='%s'>\
			<Input type=hidden Name='gamecode' value='%d'>\
			</Form>", 
			g_ItemURL, g_LoginID, g_LoginPass, g_GameCode);

		m_pShopWeb->InsertHTML(strHtml);

		// ������ ȣ��
		char* strScript = "document.Send.submit();";
		m_pShopWeb->ExecJavascript(strScript);
	}
	#else 
	{

		CString strurl;
		strurl.Format("%s?gamecode=%d", g_ItemURL, g_GameCode);

		if(m_DefShopItemKind > 0) {
			// �⺻ �������� �����Ǿ� �ִٸ� �ֻ�ܿ� ����Ʈ �� �� �ֵ��� ���� �߰�
			CString addstr;
			addstr.Format("&default=%d", m_DefShopItemKind);
			strurl += addstr;
		}
	}
	#endif

}

void CGameItemDlg::ItemRefresh()
{
	if(m_bRefreshList) {
		// �ѹ��̶� ������ �׺���̼��� �ؾ߸� ������ ����Ʈ�� ������ ��û
		m_ItemListDlg.AskRefresh();
		// ī���� �ʱ�ȭ		
		m_bRefreshList = FALSE;
	}
}

void CGameItemDlg::ChangeTab(int tabno)
{
	// ��� �����츦 ����
	m_pHelpWeb->ShowWindow(SW_HIDE);
	m_ItemListDlg.ShowWindow(SW_HIDE);

	// ��� �ǹ�ư�� Ȱ��ȭ
	m_HelpBtn.EnableWindow(TRUE);
	m_ItemBtn.EnableWindow(TRUE);	

	if(tabno == 0) {
		m_pHelpWeb->ShowWindow(SW_SHOW);
		m_HelpBtn.EnableWindow(FALSE);
		
		if( !m_bHelpWebNavigated ) {
			// ���ʷ� ���� ���µǴ� ��츸 ������ ���� URL�� ������
			CString strurl;
			strurl.Format("%s?gamecode=%d", g_ItemHelpURL, g_GameCode);
			m_pHelpWeb->Navigate(strurl);

			m_bHelpWebNavigated = TRUE;
		}
	}
	if(tabno == 1) {
		m_ItemListDlg.ShowWindow(SW_SHOW);
		m_ItemBtn.EnableWindow(FALSE);
		
		// ������ ��������
		ItemRefresh();
	}
	if(tabno == 2) {
		m_ShopBtn.EnableWindow(FALSE);
		
		if( !m_bShopWebNavigated ) {
			
			// [PC�� �۾�] - ���� �ڵ带 �����ϰ� ���� ���� �����

			{
				/*
				// ���ʷ� ���� ���µǴ� ��츸 ������ ���� URL�� ������
				// (���� '&'�� ���� - ���ڵ��� '/'�� �پ URL�� ���µ��� �ʴ� ������ ���ֱ� ����)
				CString strurl;
				strurl.Format("%s?id=%s&pass=%s&gamecode=%d&", g_ItemURL, g_LoginID, g_LoginPass, g_GameCode);
				char openurl[256]={0,};
				g_pMainDlg->URLEncode(openurl, (char*)(LPCTSTR)strurl);
				m_pShopWeb->Navigate(openurl);

				m_pShopWeb->m_NavigatedCount = 0;
				m_bShopWebNavigated = TRUE;
				*/

				OpenShopPage();

	
				m_bShopWebNavigated = TRUE;
			}
		}
	}
	if(tabno == 3) {

		m_CashBtn.EnableWindow(FALSE);
	}

	m_CurrentTab = tabno;
}

BOOL CGameItemDlg::SafeClose()
{
	if(!GetSafeHwnd()) return FALSE;
	SendMessage(WM_COMMAND, IDCANCEL);
	return TRUE;
}

void CGameItemDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

}

void CGameItemDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here

	// ������ ��������(������ �������� �ִµ� �׳� �ݴ� ��� ����Ʈ�� ������ ��û)
	ItemRefresh();

	if(m_pHelpWeb) { delete m_pHelpWeb; m_pHelpWeb = NULL; }	
	if(m_ItemListDlg.GetSafeHwnd()) m_ItemListDlg.DestroyWindow();
}

void CGameItemDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

void CGameItemDlg::OnButtonHelp() 
{
	// TODO: Add your control notification handler code here
	ChangeTab(0);
}

void CGameItemDlg::OnButtonItem() 
{
	// TODO: Add your control notification handler code here
	ChangeTab(1);
}

void CGameItemDlg::OnButtonShop() 
{
	// TODO: Add your control notification handler code here
	ChangeTab(2);
}

void CGameItemDlg::OnButtonCash() 
{
	// TODO: Add your control notification handler code here
	ChangeTab(3);
}

void CGameItemDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
	
	CMyBitmap	Back;
	Back.LoadBitmapFile(BACKIMAGE_NAME);
	CBitmap *pOldBmp = cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	cdc.SelectObject(pOldBmp);
	cdc.DeleteDC();
	Back.DeleteObject();

	// Do not call CDialog::OnPaint() for painting messages
}

void CGameItemDlg::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CDialog::OnWindowPosChanging(lpwndpos);
	
	// TODO: Add your message handler code here
//	lpwndpos->flags &= ~SWP_SHOWWINDOW;
	
}
