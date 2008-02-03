// CaptureTalk.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPokerServer.h"
#include "62CutPokerServerView.h"
#include "CaptureTalk.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCaptureTalk dialog


CCaptureTalk::CCaptureTalk(CWnd* pParent /*=NULL*/)
	: CDialog(CCaptureTalk::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCaptureTalk)
	m_strID = _T("");
	m_bUse = FALSE;
	//}}AFX_DATA_INIT
}


void CCaptureTalk::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCaptureTalk)
	DDX_Control(pDX, IDC_LIST_USER, m_ctrlUserID);
	DDX_Text(pDX, IDC_EDIT_ID, m_strID);
	DDV_MaxChars(pDX, m_strID, 15);
	DDX_Check(pDX, IDC_CHECK_CAP, m_bUse);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCaptureTalk, CDialog)
	//{{AFX_MSG_MAP(CCaptureTalk)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_USER, OnDblclkListUser)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_NOTIFY(NM_CLICK, IDC_LIST_USER, OnClickListUser)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCaptureTalk message handlers

void CCaptureTalk::OnDblclkListUser(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_ctrlUserID.GetFirstSelectedItemPosition();
	if (pos == NULL) {
		AfxMessageBox("편집할 항목을 선택하세요");
		return;
	}

	int num = m_ctrlUserID.GetNextSelectedItem(pos);
	if(num < 0) return;

	CString szNo;
	szNo = m_ctrlUserID.GetItemText(num, 0);
	if(szNo.GetLength() != 0) 
	{
		m_strID = szNo;
		UpdateData(FALSE);
	}
	
/*
	// 방 번호를 문자열로 변환
	char strNo[10]={0,};
	wsprintf(strNo, "%d", num);

	num = -1;
	CString szNo;
	char szName[15]={0,};

	for(;;)
	{
		num = m_ctrlAdminList.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;
		szNo = m_ctrlAdminList.GetItemText(num, 0);
		if(strcmp(strNo, szNo.operator LPCTSTR())==0)
		{
			// 채널명
			m_ctrlAdminList.GetItemText(num, 1, szName, 256);
			m_strID = szName;
			UpdateData();
			break;
		}
	}
	*/
	
	*pResult = 0;
}

void CCaptureTalk::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_strID==_T("")) return;

	InsItem(m_strID);
	
}

void CCaptureTalk::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_strID==_T("")) return;

	DelItem(m_strID);
}

void CCaptureTalk::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	memset(&g_CaptureID,0,sizeof(g_CaptureID));

	g_CaptureID.bUse= m_bUse;
	g_CaptureID.nTotal = m_ctrlUserID.GetItemCount();

	for(int i=0; i<g_CaptureID.nTotal; i++){
		CString strID = m_ctrlUserID.GetItemText(i,0);
		strncpy(g_CaptureID.ID[i],strID.operator LPCTSTR(),15);
	}

	CFile fileS("CaptureID.cfg",CFile::modeCreate|CFile::modeWrite);

	fileS.Write(&g_CaptureID,sizeof(g_CaptureID));
	
		
	CDialog::OnOK();
}

void CCaptureTalk::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CCaptureTalk::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	// TODO: Add extra initialization here
	InitList();

	CFile fileO;
	CFileException e;

	memset(&g_CaptureID,0,sizeof(g_CaptureID));

	

	if( !fileO.Open( "CaptureID.cfg", CFile::modeRead, &e ) ){
		AfxMessageBox("Can't find [CaptureID.cfg] file!\nSet CaptureID config!");
	}
	else{
		fileO.Read(&g_CaptureID,sizeof(g_CaptureID));
		fileO.Close();
	}

	m_bUse = g_CaptureID.bUse;

	for(int i=0; i<MAX_CAPTURE_NUM; i++){
		InsItem(g_CaptureID.ID[i]);
	}
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




void CCaptureTalk::InitList()
{

	m_ctrlUserID.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	// Column 삽입부
	LV_COLUMN col;
	char szName[256]={0,};

	wsprintf(szName, "ID");
	
	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_WIDTH | LVCF_TEXT;
	col.fmt  = LVCFMT_LEFT;
	col.cx   = 140;
	col.pszText = szName;
	col.iSubItem = 0;
	m_ctrlUserID.InsertColumn(0, &col);
/*
	col.cx   = 50;
	wsprintf(szName, "배팅");
	col.iSubItem = 1;
	m_ctrList.InsertColumn(1, &col);
*/

}

void CCaptureTalk::InsItem(CString strID)
{
	if(strID == _T("")) return;
	if(strID.GetLength()>15) return;

	LV_ITEM item;
	char szName[256]={0,};

	// 같은 아이디가 존재하는지 먼저 검사하고 이미 존재하면 취소
	int num=-1;
	CString szRN;
	for(;;)
	{
		num = m_ctrlUserID.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;
		szRN = m_ctrlUserID.GetItemText(num, 0);
		if(strcmp(strID.operator LPCTSTR(), szRN.operator LPCTSTR())==0) return;
	}

	// 아이디
	wsprintf(szName, "%s", strID.operator LPCTSTR());
	item.mask = LVIF_TEXT;
	item.pszText = szName;
	item.iItem = m_ctrlUserID.GetItemCount();
	item.iSubItem = 0;
	m_ctrlUserID.InsertItem(&item);

	int cnt = m_ctrlUserID.GetItemCount()-1;
	
	m_ctrlUserID.SetItemState(cnt, LVIS_SELECTED, LVIS_SELECTED);
	m_ctrlUserID.EnsureVisible(cnt, FALSE);

	CString str;
	str.Format("%d", m_ctrlUserID.GetItemCount());
	GetDlgItem(IDC_STATIC_COUNT)->SetWindowText(str);

/*
	// 성별
	if(sex==TRUE) wsprintf(szName, "♂");
	else wsprintf(szName, "♀");
	item.iSubItem = 1;
	m_ctrList.SetItem(&item);
*/
}

void CCaptureTalk::DelItem(CString strID)
{
	if(strID == _T("")) return;
	if(strID.GetLength()>15) return;

	int num=-1;
	CString szID;
	char szName[256]={0,};

	for(;;)
	{
		num = m_ctrlUserID.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;
		szID = m_ctrlUserID.GetItemText(num, 0);
		if(strcmp(strID.operator LPCTSTR(), szID.operator LPCTSTR())==0) {m_ctrlUserID.DeleteItem(num);	break;}
	}

	CString str;
	str.Format("%d", m_ctrlUserID.GetItemCount());
	GetDlgItem(IDC_STATIC_COUNT)->SetWindowText(str);
}

void CCaptureTalk::OnClickListUser(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_ctrlUserID.GetFirstSelectedItemPosition();
	if (pos == NULL) {
		AfxMessageBox("편집할 항목을 선택하세요");
		return;
	}

	int num = m_ctrlUserID.GetNextSelectedItem(pos);
	if(num < 0) return;

	CString szNo;
	szNo = m_ctrlUserID.GetItemText(num, 0);
	if(szNo.GetLength() != 0) 
	{
		m_strID = szNo;
		UpdateData(FALSE);
	}
	
	*pResult = 0;
}
