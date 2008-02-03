// NMServerAgentDlg.h : header file
//

#if !defined(AFX_NMSERVERAGENTDLG_H__E5ED3A07_14BE_4736_BD79_8F7B96085AF8__INCLUDED_)
#define AFX_NMSERVERAGENTDLG_H__E5ED3A07_14BE_4736_BD79_8F7B96085AF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SockCmnd.h"
#include "KHSRichEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CNMServerAgentDlg dialog
// ### [ Queue ] ###
typedef struct NODE {
	int	 Count;
	char ID[16];
	struct NODE *next;
} QDATA, *LPQDATA;


class CNMServerAgentDlg : public CDialog
{
// Construction
protected:
	// ### [ Queue ] ###
	LPQDATA m_head, m_tail;
	int  m_TotalQueCnt;

public:
	BOOL IsAdminUser(char* id);
	// ### [ Queue ] ###
	void QueDeleteAll();
	BOOL FindQueId(char *id, int &nRcount);
	BOOL QueueProcess();
	BOOL AddQueue(char *id);


	void SaveData();
	void InsertData(AGENT_USER &au, int nSID, BOOL bCon);

	void Disconnect(int nSID);
	void ConnectFail(int nSID);
	void ShowState(CString str,CString strName,BOOL bLog = TRUE);
	CNMServerAgentDlg(CWnd* pParent = NULL);	// standard constructor

	CSockCmnd     m_SockCmnd;	// 소켓 메세지 처리 클래스
	CKHSRichEdit* m_pKRich;     // 쳇팅창.

	void GetJackPotAverage();

	CMenu m_ctrMenu;  // 팝업 메뉴
	int   m_nSelNum;
	int   m_nSelUser;

	BOOL m_bisContinue;

	void InitList();
	void InsItem(char *Name, char *IP, UINT nPort, int nNum, INT64 jackpot=0);
	void SetItem(char *Name, int nNum, INT64 jackpot = 0);
	void SetItemJackPot(char *Name, INT64 jackpot=0);
	void DelItem(char *Name);

	void InitUList();
	void InsUItem(AGENT_USER &au, char *Name);
	void DelUItem(AGENT_USER &au, char *Name);

// Dialog Data
	//{{AFX_DATA(CNMServerAgentDlg)
	enum { IDD = IDD_NMSERVERAGENT_DIALOG };
	CComboBox	m_comboUList;
	CListCtrl	m_ctrUser;
	CListCtrl	m_ctrList;
	CString	m_strUserID;
	BOOL	m_bDCon;
	BOOL	m_AverJackPot;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNMServerAgentDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CNMServerAgentDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonConnect();
	afx_msg void OnSelchangeComboUlist();
	afx_msg void OnButtonFind();
	afx_msg void OnButtonDisconnect();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMenuDisconnect();
	afx_msg void OnRclickListSrv(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedListSrv(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckDcon();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonAuto();
	afx_msg void OnButtonAlldis();
	afx_msg void OnCheckAverage();
	afx_msg void OnButtonAdmin();
	//}}AFX_MSG
	afx_msg long OnReceiveDataProc(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
};

extern CNMServerAgentDlg *g_pMainDlg;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NMSERVERAGENTDLG_H__E5ED3A07_14BE_4736_BD79_8F7B96085AF8__INCLUDED_)
