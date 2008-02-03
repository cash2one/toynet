// MakeSrvList.cpp : implementation file
//

#include "stdafx.h"
#include "NMServerAgent.h"
#include "MakeSrvList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMakeSrvList dialog


CMakeSrvList::CMakeSrvList(CWnd* pParent /*=NULL*/)
	: CDialog(CMakeSrvList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMakeSrvList)
	m_strName = _T("");
	m_strIP = _T("");
	m_nPort = 0;
	//}}AFX_DATA_INIT

	m_nSelNum = -1;
}


void CMakeSrvList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMakeSrvList)
	DDX_Control(pDX, IDC_LIST_SRV, m_ctrList);
	DDX_Text(pDX, IDC_EDIT1, m_strName);
	DDV_MaxChars(pDX, m_strName, 63);
	DDX_Text(pDX, IDC_EDIT2, m_strIP);
	DDV_MaxChars(pDX, m_strIP, 63);
	DDX_Text(pDX, IDC_EDIT3, m_nPort);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMakeSrvList, CDialog)
	//{{AFX_MSG_MAP(CMakeSrvList)
	ON_BN_CLICKED(IDC_BUTTON_MAKE, OnButtonMake)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SRV, OnRclickListSrv)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SRV, OnItemchangedListSrv)
	ON_COMMAND(IDC_MENU_DEL, OnMenuDel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SRV, OnDblclkListSrv)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMakeSrvList message handlers

void CMakeSrvList::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

// 저장한다
void CMakeSrvList::OnOK() 
{
	UpdateData();

	CFile fileS("ServerList.cfg",CFile::modeCreate|CFile::modeWrite);

	fileS.Write(&g_ServerInfoList,sizeof(SERVER_INFO_SAVE));
	
	CDialog::OnOK();
}

// 새로운 리스트를 추가한다
void CMakeSrvList::OnButtonMake() 
{
	UpdateData();

	if(m_strName=="" || m_strIP=="" || m_nPort==0){
		AfxMessageBox("서버 정보를 입력하여 주시길 바랍니다!");
		return;
	}
	if(g_ServerInfoList.nTotalList>=MAX_CONNECT_SERVER){
		AfxMessageBox("더이상 서버리스트를 추가할 수 없습니다!");
		return;
	}
	// 동일한 정보가 있는지 확인하자
	for(int i=0; i<MAX_CONNECT_SERVER; i++){
		CString strName = g_ServerInfoList.INFO[i].Name;
		CString strIP = g_ServerInfoList.INFO[i].IP;
		UINT nPort = g_ServerInfoList.INFO[i].nPort;
		if(strName==m_strName){
			AfxMessageBox("이미 같은 이름의 서버 정보가 존재합니다!");
			return;
		}

		if(m_strIP == strIP && m_nPort == nPort)
		{
			AfxMessageBox("같은 IP, PORT가 존재합니다!");
			return;
		}
	}
	// 새로운 서버데이타를 추가한다
	int nIndex = g_ServerInfoList.nTotalList;

	strncpy(g_ServerInfoList.INFO[nIndex].Name, m_strName.operator LPCTSTR(),63);
	strncpy(g_ServerInfoList.INFO[nIndex].IP,   m_strIP.operator LPCTSTR(),  63);
	g_ServerInfoList.INFO[nIndex].nPort = m_nPort;

	InsItem(g_ServerInfoList.INFO[nIndex].Name,g_ServerInfoList.INFO[nIndex].IP, g_ServerInfoList.INFO[nIndex].nPort);

	g_ServerInfoList.nTotalList++;

	m_strName = "";
	m_strIP   = "";
	m_nPort   = 0;

	UpdateData(FALSE);

}

BOOL CMakeSrvList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// 팝업 메뉴 초기화
	m_ctrMenu.LoadMenu(IDR_POPMENU);

	// 리스트 컨트롤 초기화
	InitList();

	CFile fileO;
	CFileException e;

	if( !fileO.Open( "ServerList.cfg", CFile::modeRead, &e ) ){
		AfxMessageBox("Can't find [ServerList.cfg] file!");
	}
	else{
		memset(&g_ServerInfoList,0,sizeof(SERVER_INFO_SAVE));
		fileO.Read(&g_ServerInfoList,sizeof(SERVER_INFO_SAVE));
		fileO.Close();
	}

	for(int i=0; i<g_ServerInfoList.nTotalList; i++){
		InsItem(g_ServerInfoList.INFO[i].Name,g_ServerInfoList.INFO[i].IP, g_ServerInfoList.INFO[i].nPort);
	}
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMakeSrvList::InitList()
{
	// 전체행 셀렉트 설정
	m_ctrList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	//// Column 삽입부
	LV_COLUMN col;
	char szName[256];

	wsprintf(szName, "Name");
	
	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_WIDTH | LVCF_TEXT;
	col.fmt  = LVCFMT_LEFT;
	col.cx   = 164;
	col.pszText = szName;
	col.iSubItem = 0;
	m_ctrList.InsertColumn(0, &col);

	col.fmt  = LVCFMT_LEFT;
	col.cx   = 164;
	wsprintf(szName, "IP");
	col.iSubItem = 1;
	m_ctrList.InsertColumn(1, &col);

	col.fmt  = LVCFMT_CENTER;
	col.cx   = 100;
	wsprintf(szName, "Port");
	col.iSubItem = 2;
	m_ctrList.InsertColumn(2, &col);
}

void CMakeSrvList::InsItem(char *Name, char *IP, UINT nPort)
{
	// 레코드를 리스트 뷰에 삽입
	LV_ITEM item;
	char szName[256];

	// 같은 리스트가 존재하는지 먼저 검사하고 이미 존재하면 취소
	int num=-1;
	CString szRN;
	for(;;)
	{
		num = m_ctrList.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;
		szRN = m_ctrList.GetItemText(num, 0);
		if(strcmp(Name, szRN.operator LPCTSTR())==0) return;
	}

	// 이름
	wsprintf(szName, "%s", Name);
	item.mask = LVIF_TEXT;
	item.pszText = szName;
	item.iItem = m_ctrList.GetItemCount();
	item.iSubItem = 0;
	m_ctrList.InsertItem(&item);

	// 아이피
	wsprintf(szName, "%s", IP);
	item.iSubItem = 1;
	m_ctrList.SetItem(&item);

	// 포트번호
	wsprintf(szName, "%d", nPort);
	item.iSubItem = 2;
	m_ctrList.SetItem(&item);

}

void CMakeSrvList::DelItem(char *Name)
{
	int num=-1;
	CString szID;

	for(;;)
	{
		num = m_ctrList.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;
		szID = m_ctrList.GetItemText(num, 0);
		if(strcmp(Name, szID.operator LPCTSTR())==0) {m_ctrList.DeleteItem(num);	break;}
	}
}

void CMakeSrvList::OnItemchangedListSrv(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_nSelNum = pNMListView->iItem;
	*pResult = 0;

		// 테스트용
	if(pNMHDR->code == LVN_ITEMCHANGED) 
	{
		if(pNMListView->uChanged == LVIF_STATE && pNMListView->uNewState == (LVIS_SELECTED | LVIS_FOCUSED) )
		{
			char sport[20];
			//m_ctrList.GetItemText(pNMListView->iItem, 0, (char*)m_strName.operator LPCTSTR(), 63);
			m_strName = m_ctrList.GetItemText(m_nSelNum, 0);
			m_ctrList.GetItemText(pNMListView->iItem, 1, (char*)m_strIP.operator LPCTSTR(), 63);
			m_ctrList.GetItemText(pNMListView->iItem, 2, sport, 20);

			m_nPort = atoi( sport );
			UpdateData(FALSE);

		}
	}
}

void CMakeSrvList::OnRclickListSrv(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_nSelNum = pNMListView->iItem;
	*pResult = 0;

	if(m_nSelNum < 0)
	{
		m_ctrMenu.GetSubMenu(0)->EnableMenuItem(IDC_MENU_DEL, MF_GRAYED);
//		m_ctrMenu.GetSubMenu(0)->EnableMenuItem(IDC_MENU_DEL, MF_GRAYED);
	}
	else
	{
		m_ctrMenu.GetSubMenu(0)->EnableMenuItem(IDC_MENU_DEL, MF_ENABLED);
//		m_ctrMenu.GetSubMenu(0)->EnableMenuItem(IDC_MENU_DEL, MF_ENABLED);
	}

	// 팝업 메뉴
	CPoint pnt;
	GetCursorPos(&pnt);
	m_ctrMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pnt.x, pnt.y, this);
}

// 선택한 리스트를 제거한다
void CMakeSrvList::OnMenuDel() 
{
	// 선택된 아이템중 첫번째 아이템을 얻는다
	POSITION pos = m_ctrList.GetFirstSelectedItemPosition();
	if (pos == NULL) return;
	m_nSelNum = m_ctrList.GetNextSelectedItem(pos);

	if(m_nSelNum < 0) return;
	if(m_nSelNum >= m_ctrList.GetItemCount()) return;

	CString szID;
	szID = m_ctrList.GetItemText(m_nSelNum, 0);
	if(szID.GetLength() == 0) return;

	DelItem((char*)szID.operator LPCTSTR());

	// 제거후 순서대로 새로 만든다
	memset(&g_ServerInfoList,0,sizeof(SERVER_INFO_SAVE));

	for(int i=0; i<m_ctrList.GetItemCount(); i++){
		szID = m_ctrList.GetItemText(i,0);
		strncpy(g_ServerInfoList.INFO[i].Name, szID.operator LPCTSTR(),63);
		szID = m_ctrList.GetItemText(i,1);
		strncpy(g_ServerInfoList.INFO[i].IP,   szID.operator LPCTSTR(),63);
		szID = m_ctrList.GetItemText(i,2);

		if(szID.GetLength()==0) continue;
		int num = ::atoi(szID.operator LPCTSTR());
		if(num < 0) continue;

		g_ServerInfoList.INFO[i].nPort = (UINT)num;
	}

	g_ServerInfoList.nTotalList = m_ctrList.GetItemCount();

	UpdateData(FALSE);
}

// 리스트를 셀렉트한다
void CMakeSrvList::Selection()
{

}


void CMakeSrvList::OnDblclkListSrv(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_ctrList.GetFirstSelectedItemPosition();
	if (pos == NULL) {
		AfxMessageBox("편집할 항목을 선택하세요");
		return;
	}

	int num = m_ctrList.GetNextSelectedItem(pos);
	if(num < 0) return;

	CString szNo;
	szNo = m_ctrList.GetItemText(num, 0);
	if(szNo.GetLength() != 0) 
	{
		m_strName = m_ctrList.GetItemText(num, 0);
		m_strIP = m_ctrList.GetItemText(num, 1);
		CString str = m_ctrList.GetItemText(num, 2);
		m_nPort = atoi( (char*)m_ctrList.GetItemText(num, 2).operator LPCTSTR() );

		//m_nPort = atoi( m_ctrList.GetItemText(num, 2) );
		
		UpdateData(FALSE);
	}

	
	*pResult = 0;
}


void CMakeSrvList::OnButtonEdit() 
{
	// TODO: Add your control notification handler code here

	UpdateData();



	if(m_strName=="" || m_strIP=="" || m_nPort==0){
		AfxMessageBox("서버 정보를 입력하여 주시길 바랍니다!");
		return;
	}
	if(g_ServerInfoList.nTotalList>=MAX_CONNECT_SERVER){
		AfxMessageBox("더이상 서버리스트를 추가할 수 없습니다!");
		return;
	}
/*
	// 동일한 정보가 있는지 확인하자
	for(int i=0; i<MAX_CONNECT_SERVER; i++){
		CString strName = g_ServerInfoList.INFO[i].Name;
		CString strIP = g_ServerInfoList.INFO[i].IP;
		UINT nPort = g_ServerInfoList.INFO[i].nPort;
		if(strName==m_strName){
			AfxMessageBox("이미 같은 이름의 서버 정보가 존재합니다!");
			return;
		}

		if(m_strIP == strIP && m_nPort == nPort)
		{
			AfxMessageBox("같은 IP, PORT가 존재합니다!");
			return;
		}
	}
*/
	// 새로운 서버데이타를 추가한다
	int nIndex = m_nSelNum;//g_ServerInfoList.nTotalList;
	if(m_nSelNum < 0) return;


	if(strcmp(g_ServerInfoList.INFO[nIndex].Name, m_strName)==0 &&
		strcmp(g_ServerInfoList.INFO[nIndex].IP, m_strIP)==0 &&
		g_ServerInfoList.INFO[nIndex].nPort==m_nPort)
	{
		AfxMessageBox("수정 할 값을 확인해 주십시요");
		return;
	}
	
	UINT rtn = AfxMessageBox("수정 하시겠습니까?", MB_YESNO);
	if(rtn==IDYES) {

		strncpy(g_ServerInfoList.INFO[nIndex].Name, m_strName.operator LPCTSTR(),63);
		strncpy(g_ServerInfoList.INFO[nIndex].IP,   m_strIP.operator LPCTSTR(),  63);
		g_ServerInfoList.INFO[nIndex].nPort = m_nPort;


	//	InsItem(g_ServerInfoList.INFO[nIndex].Name,g_ServerInfoList.INFO[nIndex].IP, g_ServerInfoList.INFO[nIndex].nPort);
		SetItem(g_ServerInfoList.INFO[nIndex].Name,g_ServerInfoList.INFO[nIndex].IP, g_ServerInfoList.INFO[nIndex].nPort);
	//	g_ServerInfoList.nTotalList++;
	/*
		m_strName = "";
		m_strIP   = "";
		m_nPort   = 0;
	*/
		UpdateData(FALSE);
	}

	
}


void CMakeSrvList::SetItem(char *Name, char *IP, UINT nPort)
{

		// 레코드를 리스트 뷰에 삽입
	LV_ITEM item;
	char szName[256];

	// 같은 리스트가 존재하는지 먼저 검사하고 이미 존재하면 취소
	int num=-1;
	CString szRN;
/*	
	for(;;)
	{
		num = m_ctrList.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;
		szRN = m_ctrList.GetItemText(num, 0);
		if(strcmp(Name, szRN.operator LPCTSTR())==0) return;
	}
*/
	// 이름
	wsprintf(szName, "%s", Name);
	item.mask = LVIF_TEXT;
	item.pszText = szName;
	item.iItem = m_nSelNum;
	item.iSubItem = 0;
	m_ctrList.SetItem(&item);

	// 아이피
	wsprintf(szName, "%s", IP);
	item.iSubItem = 1;
	m_ctrList.SetItem(&item);

	// 포트번호
	wsprintf(szName, "%d", nPort);
	item.iSubItem = 2;
	m_ctrList.SetItem(&item);

}
