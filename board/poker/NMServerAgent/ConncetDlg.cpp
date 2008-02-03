// ConncetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NMServerAgent.h"
#include "ConncetDlg.h"
#include "MakeSrvList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConncetDlg dialog


CConncetDlg::CConncetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConncetDlg::IDD, pParent)
{
	m_strServerName = _T("");
	m_strIP = _T("");
	m_nPort = 0;

	//{{AFX_DATA_INIT(CConncetDlg)
	m_strSrvInfo = _T("");
	//}}AFX_DATA_INIT
}


void CConncetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConncetDlg)
	DDX_Control(pDX, IDC_COMBO_LIST, m_comboList);
	DDX_Text(pDX, IDC_EDIT_SRV_INFO, m_strSrvInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConncetDlg, CDialog)
	//{{AFX_MSG_MAP(CConncetDlg)
	ON_BN_CLICKED(IDC_BUTTON_MAKE_LIST, OnButtonMakeList)
	ON_BN_CLICKED(IDOK, OnConnect)
	ON_CBN_SELCHANGE(IDC_COMBO_LIST, OnSelchangeComboList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConncetDlg message handlers

void CConncetDlg::OnButtonMakeList() 
{
	CMakeSrvList dlg;
	dlg.DoModal();

	ReListComboData();
}

// 선택한 서버에 접속을 시도하자!!!
void CConncetDlg::OnConnect() 
{
	// TODO: Add your control notification handler code here
	if(m_comboList.GetCurSel()<0) return;

	m_comboList.GetLBText(m_comboList.GetCurSel(),m_strServerName);

	for(int i=0; i<MAX_CONNECT_SERVER; i++){
		CString strName = g_ServerInfoList.INFO[i].Name;
		if(strName==m_strServerName){
			m_strIP = g_ServerInfoList.INFO[i].IP;
			m_nPort = g_ServerInfoList.INFO[i].nPort;
			break;
		}
	}

	CDialog::OnOK();
}

void CConncetDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CConncetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	ReListComboData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConncetDlg::ReListComboData()
{
	m_comboList.ResetContent();

	BOOL bAddOk = FALSE;
	for(int i=0; i<g_ServerInfoList.nTotalList; i++){
		m_comboList.AddString(g_ServerInfoList.INFO[i].Name);
		bAddOk = TRUE;
	}
	if(bAddOk){
		m_comboList.SetCurSel(0);

		CString str;
		m_comboList.GetLBText(0,str);
		ShowInfo(str);
	}
}

void CConncetDlg::OnSelchangeComboList() 
{
	int nNum = m_comboList.GetCurSel();
	if(nNum<0) return;

	CString str;
	m_comboList.GetLBText(nNum,str);

	ShowInfo(str);
	
}

void CConncetDlg::ShowInfo(CString strName)
{
	for(int i=0; i<g_ServerInfoList.nTotalList; i++){
		CString str = g_ServerInfoList.INFO[i].Name;
		if(str==strName){
			m_strSrvInfo.Format("\r\n  * IP    : %s\r\n  * Port : %d",g_ServerInfoList.INFO[i].IP,g_ServerInfoList.INFO[i].nPort);
			UpdateData(FALSE);
			break;
		}
	}
}


