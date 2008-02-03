// AdminManager.cpp : implementation file
//

#include "stdafx.h"
#include "nmserveragent.h"
#include "AdminManager.h"


#include "SockCmnd.h"
#include "SrvInfoMan.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdminManager dialog


CAdminManager::CAdminManager(CWnd* pParent /*=NULL*/)
	: CDialog(CAdminManager::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdminManager)
	m_strID = _T("");
	m_nGrade = 0;
	//}}AFX_DATA_INIT
}


void CAdminManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdminManager)
	DDX_Control(pDX, IDC_IPADDRESS1, m_ctrlIP);
	DDX_Control(pDX, IDC_LIST_ADMIN, m_ctrlAdminList);
	DDX_Text(pDX, IDC_EDIT_ID, m_strID);
	DDX_Text(pDX, IDC_EDIT_GRADE, m_nGrade);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdminManager, CDialog)
	//{{AFX_MSG_MAP(CAdminManager)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ADMIN, OnDblclkListAdmin)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BTN_ADMININFOSEND, OnBtnAdmininfosend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdminManager message handlers


BOOL CAdminManager::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitList();
	
	CFile fileO;
	CFileException e;
	
//	memset(&g_AdminInfo,0,sizeof(g_AdminInfo));
	
	char *tmp = new char[sizeof(g_AdminInfo.AdminMan)];
	char *tmp1 = new char[sizeof(g_AdminInfo.AdminMan)];
	
	ZeroMemory(tmp, sizeof(g_AdminInfo.AdminMan));
	ZeroMemory(tmp1, sizeof(g_AdminInfo.AdminMan));
	
	int i=0;
	if( !fileO.Open( "AdminInfo.cfg", CFile::modeRead, &e ) ){
		AfxMessageBox("Can't find [AdminInfo.cfg] file!\nSet Admin config!");
	}
	else{
		fileO.Read(tmp,sizeof(g_AdminInfo.AdminMan));
		fileO.Close();
		for( i=0; i<sizeof(g_AdminInfo.AdminMan);i++)
		{
			tmp1[i]^= (tmp[i]^'C'^'Y');
		}
	}
		
	memcpy(&g_AdminInfo.AdminMan, tmp1, sizeof(g_AdminInfo.AdminMan));
	
	for( i=0; i<MAX_ADMIN_NUM; i++){
		InsItem(g_AdminInfo.AdminMan.admin[i].ID, g_AdminInfo.AdminMan.admin[i].Grade, g_AdminInfo.AdminMan.admin[i].UserIP);
	}
	
	delete tmp;
	delete tmp1;


	CString strPM;
	strPM.Format("%d", g_AdminInfo.NowNum);
	GetDlgItem(IDC_STATIC_COUNT)->SetWindowText(strPM.operator LPCTSTR());


	UpdateData(FALSE);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAdminManager::InitList()
{

	m_ctrlAdminList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	// Column ���Ժ�
	LV_COLUMN col;
	char szName[256]={0,};

	wsprintf(szName, "������ ID");
	
	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_WIDTH | LVCF_TEXT;
	col.fmt  = LVCFMT_CENTER;
	col.cx   = 100;
	col.pszText = szName;
	col.iSubItem = 0;
	m_ctrlAdminList.InsertColumn(0, &col);

	col.cx   = 50;
	wsprintf(szName, "���");
	col.iSubItem = 1;
	m_ctrlAdminList.InsertColumn(1, &col);

	col.cx   = 120;
	wsprintf(szName, "IP�ּ�");
	col.iSubItem = 2;
	m_ctrlAdminList.InsertColumn(2, &col);

}


void CAdminManager::InsItem(CString strID, int grade, CString strIP)
{
	if(strID == _T("")) return;
	if(strID.GetLength()>15) return;

	if(grade <= 0)
	{
		AfxMessageBox("����� Ȯ���� �ּ��� 1 ~ 5");
		return;
	}

	LV_ITEM item;
	char szName[256]={0,};

	// ���� ���̵� �����ϴ��� ���� �˻��ϰ� �̹� �����ϸ� ���
	int num=-1;
	CString szRN;
	for(;;)
	{
		num = m_ctrlAdminList.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;
		szRN = m_ctrlAdminList.GetItemText(num, 0);
		if(strcmp(strID.operator LPCTSTR(), szRN.operator LPCTSTR())==0) return;
	}

	// ���̵�
	wsprintf(szName, "%s", strID.operator LPCTSTR());
	item.mask = LVIF_TEXT;
	item.pszText = szName;
	item.iItem = m_ctrlAdminList.GetItemCount();
	item.iSubItem = 0;
	m_ctrlAdminList.InsertItem(&item);

	wsprintf(szName, "%d", grade);
	item.mask = LVIF_TEXT;
	item.pszText = szName;
	item.iSubItem = 1;
	m_ctrlAdminList.SetItem(&item);

	wsprintf(szName, "%s", strIP.operator LPCTSTR() );
	item.mask = LVIF_TEXT;
	item.pszText = szName;
	item.iSubItem = 2;
	m_ctrlAdminList.SetItem(&item);
}

void CAdminManager::DelItem(CString strID)
{
	if(strID == _T("")) return;
	if(strID.GetLength()>15) return;
	
	int num=-1;
	CString szID;
	char szName[256]={0,};
	
	for(;;)
	{
		num = m_ctrlAdminList.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;
		szID = m_ctrlAdminList.GetItemText(num, 0);
		if(strcmp(strID.operator LPCTSTR(), szID.operator LPCTSTR())==0) {m_ctrlAdminList.DeleteItem(num);	break;}
	}

}

void CAdminManager::OnDblclkListAdmin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_ctrlAdminList.GetFirstSelectedItemPosition();
	if (pos == NULL) {
		AfxMessageBox("������ �׸��� �����ϼ���");
		return;
	}

	int num = m_ctrlAdminList.GetNextSelectedItem(pos);
	if(num < 0) return;

	CString szNo;
	szNo = m_ctrlAdminList.GetItemText(num, 0);
	if(szNo.GetLength() != 0) 
	{
		m_strID = szNo;
		szNo = m_ctrlAdminList.GetItemText(num, 1);
		m_nGrade = atoi(szNo);

		szNo = m_ctrlAdminList.GetItemText(num, 2);
	
		char buf[24]={0,};
		strncpy(buf, szNo.operator LPCTSTR(), 24);
		char *pnow = buf;
		int nStart=0, nEnd=0;
		int nLine=0;
		int buflen = szNo.GetLength();
		BYTE nf0, nf1, nf2, nf3;
		for(int i=0; i<=buflen; i++)
		{			
			if(*pnow == '.' || *pnow == '\0')
			{
				nEnd = i;
				switch(nLine)
				{
					case 0:
						nf0 = atoi( szNo.Mid(nStart, nEnd-nStart) );
						break;
					case 1:
						nf1 = atoi( szNo.Mid(nStart, nEnd-nStart) );
						break;
					case 2:
						nf2 = atoi( szNo.Mid(nStart, nEnd-nStart) );
						break;
					case 3:
						nf3 = atoi( szNo.Mid(nStart, nEnd-nStart) );
						break;
				}
				nStart = nEnd+1;
				nLine++;
			}
			pnow++;
		}
		m_ctrlIP.SetAddress(nf0, nf1, nf2, nf3);
		UpdateData(FALSE);
	}

	*pResult = 0;
}

void CAdminManager::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_strID==_T("")) return;
	if(m_ctrlIP.IsBlank() )  {
		AfxMessageBox("������ ���̵�� �㰡�� ������ �ּҸ� �Է��� �ֿ�");
		return;
	}

	BYTE nf0, nf1, nf2, nf3;
	m_ctrlIP.GetAddress(nf0, nf1, nf2, nf3);
	CString str;
	str.Format("%d.%d.%d.%d", nf0, nf1, nf2, nf3);

	InsItem(m_strID, m_nGrade,  str);

	memset(&g_AdminInfo,0,sizeof(g_AdminInfo));

	g_AdminInfo.AdminMan.nTotal = m_ctrlAdminList.GetItemCount();

	for(int i=0; i<g_AdminInfo.AdminMan.nTotal; i++){
		CString strID = m_ctrlAdminList.GetItemText(i,0);
		strncpy(g_AdminInfo.AdminMan.admin[i].ID,strID.operator LPCTSTR(),15);

		strID = m_ctrlAdminList.GetItemText(i,1);
		g_AdminInfo.AdminMan.admin[i].Grade = atoi(strID);

		strID = m_ctrlAdminList.GetItemText(i,2);
		strncpy(g_AdminInfo.AdminMan.admin[i].UserIP,strID.operator LPCTSTR(), 24);
	}
	
}

void CAdminManager::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_strID==_T("")) return;

	DelItem(m_strID);	
	
	memset(&g_AdminInfo,0,sizeof(g_AdminInfo));
	
	g_AdminInfo.AdminMan.nTotal = m_ctrlAdminList.GetItemCount();
	
	for(int i=0; i<g_AdminInfo.AdminMan.nTotal; i++){
		CString strID = m_ctrlAdminList.GetItemText(i,0);
		strncpy(g_AdminInfo.AdminMan.admin[i].ID,strID.operator LPCTSTR(),15);
		strID = m_ctrlAdminList.GetItemText(i,1);
		g_AdminInfo.AdminMan.admin[i].Grade = atoi(strID);

		strID = m_ctrlAdminList.GetItemText(i,2);
		strncpy(g_AdminInfo.AdminMan.admin[i].UserIP,strID.operator LPCTSTR(), 24);
	}
	
}

void CAdminManager::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	memset(&g_AdminInfo.AdminMan,0,sizeof(g_AdminInfo.AdminMan));

	g_AdminInfo.AdminMan.nTotal = m_ctrlAdminList.GetItemCount();

	for(int i=0; i<g_AdminInfo.AdminMan.nTotal; i++){
		CString strID = m_ctrlAdminList.GetItemText(i,0);
		strncpy(g_AdminInfo.AdminMan.admin[i].ID,strID.operator LPCTSTR(),15);
		strID = m_ctrlAdminList.GetItemText(i,1);
		g_AdminInfo.AdminMan.admin[i].Grade = atoi(strID);
		
		strID = m_ctrlAdminList.GetItemText(i,2);
		strncpy(g_AdminInfo.AdminMan.admin[i].UserIP,strID.operator LPCTSTR(), 24);
	}

	CFile fileS("AdminInfo.cfg",CFile::modeCreate|CFile::modeWrite);

	char *tmp = new char[sizeof(g_AdminInfo.AdminMan)];
	char *tmp1 = new char[sizeof(g_AdminInfo.AdminMan)];

	ZeroMemory(tmp, sizeof(g_AdminInfo.AdminMan));
	ZeroMemory(tmp1, sizeof(g_AdminInfo.AdminMan));

	memcpy(tmp, &g_AdminInfo.AdminMan, sizeof(g_AdminInfo.AdminMan));

	for( i=0; i<sizeof(g_AdminInfo.AdminMan);i++)
	{
		tmp1[i]^= (tmp[i]^'C'^'Y');
	}
	fileS.Write(tmp1,sizeof(g_AdminInfo.AdminMan));
	delete tmp;
	delete tmp1;
	
	CDialog::OnOK();
}

void CAdminManager::OnBtnAdmininfosend() 
{
	// TODO: Add your control notification handler code here
	

	for(int i=0; i<MAX_CONNECT_SERVER; i++){
		if(ServerMan.m_Data[i].m_bValid && ServerMan.m_Data[i].m_bConnect){

				CNM_AGENT_ADMININFO agent;
			agent.Set(g_AdminInfo.AdminMan.nTotal, g_AdminInfo.AdminMan.admin);

			
			int nSID = ServerMan.m_Data[i].m_nSID;
			SockMan.SendData(nSID, agent.pData, agent.GetTotalSize());
		}
	}

	
}
