// Config3Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPokerServer.h"
#include "Config3Dlg.h"

#include "62CutPokerServerView.h"
#include "SetChanInfoDlg.h"
#include "SetColumnDlg.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfig3Dlg dialog


CConfig3Dlg::CConfig3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfig3Dlg::IDD, pParent)
{
	
	//{{AFX_DATA_INIT(CConfig3Dlg)
	m_EnterLimitMode = -1;
	m_bLoginLimit = FALSE;
	m_bChanLimit = FALSE;
	m_LoginLimit1 = 0;
	m_LoginLimit2 = 0;
	m_strFM = _T("");
	m_strGM = _T("");
	m_strHM = _T("");
	m_nHL = 0;
	m_nFL = 0;
	m_nGL = 0;
	m_bExamServer = FALSE;
	m_LoginGrade = 0;
	m_strCM = _T("");
	//}}AFX_DATA_INIT
}


void CConfig3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfig3Dlg)
	DDX_Control(pDX, IDC_LIST_CHANINFO, m_ChanInfoList);
	DDX_Radio(pDX, IDC_RADIO_EINTERLIMITMODE0, m_EnterLimitMode);
	DDX_Check(pDX, IDC_CHECK_LOGINLIMIT, m_bLoginLimit);
	DDX_Check(pDX, IDC_CHECK_CHANLIMIT, m_bChanLimit);
	DDX_Text(pDX, IDC_EDIT_LOGINLIMIT1, m_LoginLimit1);
	DDX_Text(pDX, IDC_EDIT_LOGINLIMIT2, m_LoginLimit2);
	DDX_Text(pDX, IDC_EDIT_FULL, m_strFM);
	DDX_Text(pDX, IDC_EDIT_GOD, m_strGM);
	DDX_Text(pDX, IDC_EDIT_HALF, m_strHM);
	DDX_Text(pDX, IDC_EDIT_LEVEL1, m_nHL);
	DDV_MinMaxUInt(pDX, m_nHL, 0, 100);
	DDX_Text(pDX, IDC_EDIT_LEVEL2, m_nFL);
	DDV_MinMaxUInt(pDX, m_nFL, 0, 100);
	DDX_Text(pDX, IDC_EDIT_LEVEL3, m_nGL);
	DDV_MinMaxUInt(pDX, m_nGL, 0, 100);
	DDX_Check(pDX, IDC_CHECK_EXAM, m_bExamServer);
	DDX_Text(pDX, IDC_EDIT_LOGINLIMITGRADE, m_LoginGrade);
	DDV_MinMaxInt(pDX, m_LoginGrade, 1, 4);
	DDX_Text(pDX, IDC_EDIT_CHBO, m_strCM);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfig3Dlg, CDialog)
	//{{AFX_MSG_MAP(CConfig3Dlg)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CHANINFO, OnDblclkListChaninfo)
	ON_BN_CLICKED(IDC_BUTTON_SELEDIT, OnButtonSeledit)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_CHANINFO, OnColumnclickListChaninfo)
	ON_BN_CLICKED(IDC_RADIO_EINTERLIMITMODE0, OnRadioEinterlimitmode0)
	ON_BN_CLICKED(IDC_RADIO_EINTERLIMITMODE1, OnRadioEinterlimitmode1)
	ON_BN_CLICKED(IDC_CHECK_LOGINLIMIT, OnCheckLoginlimit)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfig3Dlg message handlers

BOOL CConfig3Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	m_EnterLimitMode = g_pMainView->Cfg3.EnterLimitMode;
	m_bLoginLimit = g_pMainView->Cfg3.bLoginLimit;
	m_bChanLimit = g_pMainView->Cfg3.bChanLimit;
	m_LoginLimit1 = g_pMainView->Cfg3.LoginLimitLevel1;
	m_LoginLimit2 = g_pMainView->Cfg3.LoginLimitLevel2;

	m_LoginGrade = g_pMainView->Cfg3.LoginGrade;
	// ����ä�� ����
	m_bExamServer = g_pMainView->Cfg3.bExamServer;

	m_strCM.Format(_T("%I64d"), g_pMainView->Cfg3.nCM);
	m_strHM.Format(_T("%I64d"), g_pMainView->Cfg3.nHM);
	m_strFM.Format(_T("%I64d"), g_pMainView->Cfg3.nFM);
	m_strGM.Format(_T("%I64d"), g_pMainView->Cfg3.nGM);
	
	m_nHL = g_pMainView->Cfg3.nHL;
	m_nFL = g_pMainView->Cfg3.nFL;
	m_nGL = g_pMainView->Cfg3.nGL;


////////////////////////
	g_POKERSET.nHM = ::_atoi64(m_strHM.operator LPCTSTR());
	g_POKERSET.nFM = ::_atoi64(m_strFM.operator LPCTSTR());
	g_POKERSET.nGM = ::_atoi64(m_strGM.operator LPCTSTR());

	g_POKERSET.nHL = m_nHL;
	g_POKERSET.nFL = m_nFL;
	g_POKERSET.nGL = m_nGL;
///////////////////////



	UpdateData(FALSE);

	SetControlState();

	InitList();
	for(int i=0; i<MAX_CHAN; i++) InsertItem(i, &g_pMainView->Cfg3.Info[i]);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfig3Dlg::OnOK() 
{
	// TODO: Add extra validation here
	
	UpdateData(TRUE);

	CONFIG3 tempCfg;
	ZeroMemory(&tempCfg, sizeof(CONFIG3));

	// ����� ������ �ӽ� ����ü�� ����
	tempCfg.EnterLimitMode = m_EnterLimitMode;
	tempCfg.bLoginLimit = m_bLoginLimit;
	tempCfg.bChanLimit = m_bChanLimit;
	
	tempCfg.LoginLimitLevel1 = m_LoginLimit1;
	tempCfg.LoginLimitLevel2 = m_LoginLimit2;

	tempCfg.LoginGrade = m_LoginGrade;

	if(tempCfg.EnterLimitMode == 1 && tempCfg.bLoginLimit) {
		switch(tempCfg.LoginGrade)
		{
		case 0: // ����
			{
				g_ServerGrade = 0;
				tempCfg.LoginLimitLevel1 = 0;
				tempCfg.LoginLimitLevel2 = 14;
			}
			break;
		case 1: // �ʺ�
			{
				g_ServerGrade = 1;
				tempCfg.LoginLimitLevel1 = 0;
				tempCfg.LoginLimitLevel2 = 1;
			}
			break;
		case 2: // �߼�
			{
				g_ServerGrade = 2;
				tempCfg.LoginLimitLevel1 = 2;
				tempCfg.LoginLimitLevel2 = 7;
			}
			break;
		case 3: // ���
			{
				g_ServerGrade = 3;
				tempCfg.LoginLimitLevel1 = 8;
				tempCfg.LoginLimitLevel2 = 11;
			}
			break;
		case 4: // ����
			{
				g_ServerGrade = 4;
				tempCfg.LoginLimitLevel1 = 12;
				tempCfg.LoginLimitLevel2 = 14;
			}
			break;
	
		default: //���� 0, 1
			{
				g_ServerGrade = 0;
				tempCfg.LoginLimitLevel1 = 0;
				tempCfg.LoginLimitLevel2 = 14;
			}
			break;
		}
	}




	tempCfg.bExamServer = m_bExamServer;
		 
	tempCfg.nCM = ::_atoi64(m_strCM.operator LPCTSTR());
	tempCfg.nHM = ::_atoi64(m_strHM.operator LPCTSTR());
	tempCfg.nFM = ::_atoi64(m_strFM.operator LPCTSTR());
	tempCfg.nGM = ::_atoi64(m_strGM.operator LPCTSTR());

	tempCfg.nHL = m_nHL;
	tempCfg.nFL = m_nFL;
	tempCfg.nGL = m_nGL;

////////////////////////
	g_POKERSET.nHM = ::_atoi64(m_strHM.operator LPCTSTR());
	g_POKERSET.nFM = ::_atoi64(m_strFM.operator LPCTSTR());
	g_POKERSET.nGM = ::_atoi64(m_strGM.operator LPCTSTR());

	g_POKERSET.nHL = m_nHL;
	g_POKERSET.nFL = m_nFL;
	g_POKERSET.nGL = m_nGL;
///////////////////////


	for(int i=0; i<MAX_CHAN; i++) tempCfg.Info[i] = GetItemInfo(i);

	// ���� ���� ���
	FILE *fp = fopen( "channel.cfg", "wb");
	if( fp == NULL ) { 
		AfxMessageBox("ä�μ��� ������ ����� �� �����ϴ�.");
		return;
	}
	fwrite(&tempCfg, sizeof(CONFIG3), 1, fp);
	fclose(fp);

	if(!g_pMainView->bNowStartUpDlg) AfxMessageBox("����� ȯ�漳���� �����Ϸ��� ���α׷��� ������ؾ��մϴ�.");

	CDialog::OnOK();
}

void CConfig3Dlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CConfig3Dlg::SetControlState()
{
	UpdateData(TRUE);

	if(m_EnterLimitMode == 1) {
		GetDlgItem(IDC_CHECK_LOGINLIMIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_CHANLIMIT)->EnableWindow(TRUE);
		if(m_bLoginLimit) {
			GetDlgItem(IDC_EDIT_LOGINLIMIT1)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_LOGINLIMIT2)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_LOGINLIMITGRADE)->EnableWindow(TRUE);
			
		}
		else {
			GetDlgItem(IDC_EDIT_LOGINLIMIT1)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_LOGINLIMIT2)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_LOGINLIMITGRADE)->EnableWindow(FALSE);
		}
	}
	else {
		GetDlgItem(IDC_CHECK_LOGINLIMIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CHANLIMIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LOGINLIMIT1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LOGINLIMIT2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LOGINLIMITGRADE)->EnableWindow(FALSE);
	}
}

void CConfig3Dlg::InitList()
{
	// ��ü�� ����Ʈ ����
	m_ChanInfoList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	//// Column ���Ժ�
	LV_COLUMN col;
	char szName[256];

	wsprintf(szName, "No");
	
	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_WIDTH | LVCF_TEXT;
	col.fmt  = LVCFMT_RIGHT;
	col.cx   = 30;
	col.pszText = szName;
	col.iSubItem = 0;
	m_ChanInfoList.InsertColumn(0, &col);

	col.fmt  = LVCFMT_LEFT;
	col.cx   = 140;
	wsprintf(szName, "ä�θ�");
	col.iSubItem = 1;
	m_ChanInfoList.InsertColumn(1, &col);

	col.fmt  = LVCFMT_CENTER;
	col.cx   = 60;
	wsprintf(szName, "��������");
	col.iSubItem = 2;
	m_ChanInfoList.InsertColumn(2, &col);

	col.cx   = 50;
	wsprintf(szName, "MinLv");
	col.iSubItem = 3;
	m_ChanInfoList.InsertColumn(3, &col);

	col.cx   = 50;
	wsprintf(szName, "MaxLv");
	col.iSubItem = 4;
	m_ChanInfoList.InsertColumn(4, &col);

	col.cx   = 50;
	wsprintf(szName, "Grade");
	col.iSubItem = 5;
	m_ChanInfoList.InsertColumn(5, &col);

}

void CConfig3Dlg::InsertItem(int no, CHANINFO_CFG *pCI)
{
	// ���ڵ带 ����Ʈ �信 ����
	LV_ITEM item;
	char szName[256]={0,};

	int image = 0;

	// ��ȣ
	wsprintf(szName, "%d", no);
	item.mask = LVIF_TEXT | LVIF_IMAGE;
	item.pszText = szName;
	item.iItem = m_ChanInfoList.GetItemCount();
	item.iSubItem = 0;
	item.iImage=image;
	m_ChanInfoList.InsertItem(&item);

	// ä�θ�
	wsprintf(szName, "%s", pCI->ChanName);
	item.iSubItem = 1;
	m_ChanInfoList.SetItem(&item);

	// ��������
	wsprintf(szName, "%d", pCI->GameKind);
	item.iSubItem = 2;
	m_ChanInfoList.SetItem(&item);

	// ��� �ּ� ���
	wsprintf(szName, "%d", pCI->LimitLevel1);
	item.iSubItem = 3;
	m_ChanInfoList.SetItem(&item);

	// ��� �ְ� ���
	wsprintf(szName, "%d", pCI->LimitLevel2);
	item.iSubItem = 4;
	m_ChanInfoList.SetItem(&item);

	// ���
	wsprintf(szName, "%d", pCI->Grade);
	item.iSubItem = 5;
	m_ChanInfoList.SetItem(&item);


}

void CConfig3Dlg::SetItem(int no, CHANINFO_CFG *pCI)
{
	// �� ��ȣ�� ���ڿ��� ��ȯ
	char strNo[10]={0,};
	wsprintf(strNo, "%d", no);

	int num=-1;
	CString szNo;
	char szName[256]={0,};

	for(;;)
	{
		num = m_ChanInfoList.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;
		szNo = m_ChanInfoList.GetItemText(num, 0);
		if(strcmp(strNo, szNo.operator LPCTSTR())==0)
		{
			// ä�θ�
			wsprintf(szName, "%s", pCI->ChanName);
			m_ChanInfoList.SetItemText(num, 1, szName);
			// ��������
			wsprintf(szName, "%d", pCI->GameKind);
			m_ChanInfoList.SetItemText(num, 2, szName);
			// ��� �ּҵ��
			wsprintf(szName, "%d", pCI->LimitLevel1);
			m_ChanInfoList.SetItemText(num, 3, szName);
			// ��� �ְ���
			wsprintf(szName, "%d", pCI->LimitLevel2);
			m_ChanInfoList.SetItemText(num, 4, szName);

			// ���
			wsprintf(szName, "%d", pCI->Grade );
			m_ChanInfoList.SetItemText(num, 5, szName);

			// �̹��� ��ȣ ���
			int image = 0;
			// �̹��� ����
			m_ChanInfoList.SetItem( num, 0, LVIF_IMAGE, NULL, image, NULL, NULL, NULL);

			break;
		}
	}
}

CHANINFO_CFG CConfig3Dlg::GetItemInfo(int no)
{
	// �� ��ȣ�� ���ڿ��� ��ȯ
	char strNo[10]={0,};
	wsprintf(strNo, "%d", no);

	int num=-1;
	CString szNo;
	char szName[256]={0,};

	CHANINFO_CFG info = {0,};

	for(;;)
	{
		num = m_ChanInfoList.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;
		szNo = m_ChanInfoList.GetItemText(num, 0);
		if(strcmp(strNo, szNo.operator LPCTSTR())==0)
		{
			// ä�θ�
			m_ChanInfoList.GetItemText(num, 1, szName, 256);
			strcpy(info.ChanName, szName);

			// ��������
			m_ChanInfoList.GetItemText(num, 2, szName, 256);
			info.GameKind = ::atoi(szName);
			
			// ��� �ּҵ��
			m_ChanInfoList.GetItemText(num, 3, szName, 256);
			info.LimitLevel1 = ::atoi(szName);

			// ��� �ְ���
			m_ChanInfoList.GetItemText(num, 4, szName, 256);
			info.LimitLevel2 = ::atoi(szName);

			// ���
			m_ChanInfoList.GetItemText(num, 5, szName, 256);
			info.Grade = ::atoi(szName);


			return info;
		}
	}

	return info;
}

void CConfig3Dlg::OnDblclkListChaninfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	SendMessage(WM_COMMAND, IDC_BUTTON_SELEDIT);
	*pResult = 0;
}

void CConfig3Dlg::OnButtonSeledit() 
{
	// TODO: Add your control notification handler code here
	// ���õ� �������� ù��° �������� ��´�
	POSITION pos = m_ChanInfoList.GetFirstSelectedItemPosition();
	if (pos == NULL) {
		AfxMessageBox("������ �׸��� �����ϼ���");
		return;
	}

	int num = m_ChanInfoList.GetNextSelectedItem(pos);
	if(num < 0) return;

	CHANINFO_CFG info = GetItemInfo(num);

	CSetChanInfoDlg Dlg;
	Dlg.m_ChanName = info.ChanName;
	Dlg.m_GameKind = info.GameKind;
	Dlg.m_Limit1 = info.LimitLevel1;
	Dlg.m_Limit2 = info.LimitLevel2;
	Dlg.m_Grade  = info.Grade;

	if(Dlg.DoModal() == IDCANCEL) {
		m_ChanInfoList.SetFocus();
		return;
	}

	int totsel = m_ChanInfoList.GetSelectedCount();
	if(totsel >= 2) {
		CString str;
		str.Format("���õ� %d���� �׸��� ��� �� ������ �����մϴ�. ����Ͻðڽ��ϱ�?", totsel);
		if(MessageBox(str, "���", MB_YESNO) == IDNO) return;
	}

	memset(&info, 0, sizeof(info));
	strcpy(info.ChanName, Dlg.m_ChanName);
	info.GameKind = Dlg.m_GameKind;
	info.LimitLevel1 = Dlg.m_Limit1;
	info.LimitLevel2 = Dlg.m_Limit2;
	info.Grade		 = Dlg.m_Grade;

	pos = m_ChanInfoList.GetFirstSelectedItemPosition();
	while(pos) {
		num = m_ChanInfoList.GetNextSelectedItem(pos);
		if(num < 0) break;
		SetItem(num, &info);
	}

	m_ChanInfoList.SetFocus();
}
/*
void CConfig3Dlg::OnColumnclickListChaninfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	int col = pNMListView->iSubItem;
	if(col <= 0) return;

	// ���õ� �������� ù��° �������� ��´�
	POSITION pos = m_ChanInfoList.GetFirstSelectedItemPosition();
	if (pos == NULL) {
		AfxMessageBox("���õ� �׸��� �����ϴ�.");
		return;
	}

	// �÷� �̸� ���ϱ�
	LVCOLUMN lvColumn = {0,};
	char colname[256] = {0,};
	lvColumn.mask = LVCF_TEXT;
	lvColumn.pszText = colname;
	lvColumn.cchTextMax = 255;
	m_ChanInfoList.GetColumn(col, &lvColumn);

	static BOOL bProcessed = FALSE;
	if(bProcessed == TRUE) return;
	bProcessed = TRUE;

	CSetColumnDlg Dlg;
	Dlg.m_ColumnName = colname;
	if(Dlg.DoModal() == IDCANCEL) {
		bProcessed = FALSE;
		return;
	}

	if(Dlg.m_Value.GetLength() == 0) {
		CString str;
		str.Format("���� �������� �ʾҽ��ϴ�. ����Ͻðڽ��ϱ�?");
		if(MessageBox(str, "���", MB_YESNO) == IDNO) {
			bProcessed = FALSE;
			return;
		}
	}

	bProcessed = FALSE;

	CString value = Dlg.m_Value;

	pos = m_ChanInfoList.GetFirstSelectedItemPosition();
	while(pos) {
		int num = m_ChanInfoList.GetNextSelectedItem(pos);
		if(num < 0) break;
		m_ChanInfoList.SetItemText(num, col, value);
	}

	*pResult = 0;
}
*/


void CConfig3Dlg::OnColumnclickListChaninfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	int col = pNMListView->iSubItem;
	if(col <= 0) return;

	// ���õ� �������� ù��° �������� ��´�
	POSITION pos = m_ChanInfoList.GetFirstSelectedItemPosition();
	if (pos == NULL) {
		AfxMessageBox("���õ� �׸��� �����ϴ�.");
		return;
	}

	// �÷� �̸� ���ϱ�
	LVCOLUMN lvColumn = {0,};
	char colname[256] = {0,};
	lvColumn.mask = LVCF_TEXT;
	lvColumn.pszText = colname;
	lvColumn.cchTextMax = 255;
	m_ChanInfoList.GetColumn(col, &lvColumn);

	static BOOL bProcessed = FALSE;
	if(bProcessed == TRUE) return;
	bProcessed = TRUE;

	CSetColumnDlg Dlg;
	Dlg.m_ColumnName = colname;
	// ä�θ� �÷��� ��츸 �ڵ� �ε��� ����� Ȱ��ȭ
	if(col == 1) Dlg.m_bDontEnableAutoIndexCheck = FALSE;
	else Dlg.m_bDontEnableAutoIndexCheck = TRUE;
	if(Dlg.DoModal() == IDCANCEL) {
		bProcessed = FALSE;
		return;
	}

	if(Dlg.m_Value.GetLength() == 0) {
		CString str;
		str.Format("���� �������� �ʾҽ��ϴ�. ����Ͻðڽ��ϱ�?");
		if(MessageBox(str, "���", MB_YESNO) == IDNO) {
			bProcessed = FALSE;
			return;
		}
	}

	bProcessed = FALSE;

	int cnt = 0;
	pos = m_ChanInfoList.GetFirstSelectedItemPosition();
	while(pos) {
		int num = m_ChanInfoList.GetNextSelectedItem(pos);
		if(num < 0) break;

		if(!Dlg.m_bAutoIndex) {
			m_ChanInfoList.SetItemText(num, col, Dlg.m_Value);
		}
		else {
			// �ڵ� �ε���
			int sval = Dlg.m_StartVal + cnt;
			CString str;
			if(Dlg.m_Cipher > 1) {
				CString strdizit;
				CipherFormat(strdizit, sval, Dlg.m_Cipher);
				str = Dlg.m_Value + strdizit;
			}
			else {
				str.Format("%s%d", Dlg.m_Value, sval);
			}
			m_ChanInfoList.SetItemText(num, col, str);
			cnt++;
		}
	}

	*pResult = 0;
}



void CConfig3Dlg::OnRadioEinterlimitmode0() 
{
	// TODO: Add your control notification handler code here
	SetControlState();
}

void CConfig3Dlg::OnRadioEinterlimitmode1() 
{
	// TODO: Add your control notification handler code here
	SetControlState();
}

void CConfig3Dlg::OnCheckLoginlimit() 
{
	// TODO: Add your control notification handler code here
	SetControlState();
}

void CipherFormat(CString& strout, int val, int cipher)
{
	CString str;
	for(int i=cipher-1; i>0; i--) {
		if(val < pow(10,i)) str += "0";
		else break;
	}

	strout.Format("%s%d", str, val);
}

void CConfig3Dlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}
