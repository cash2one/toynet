#if !defined(AFX_CONFIGDLG_H__702E7D41_16DF_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_CONFIGDLG_H__702E7D41_16DF_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog

class CConfigDlg : public CDialog
{
// Construction
public:
	CConfigDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigDlg)
	enum { IDD = IDD_CONFIG_DIALOG };
	int		m_GameCode;
	int		m_ServerCode;
	CString	m_ServerName;
	UINT	m_ServerPort;
	CString	m_BadUserDSN;
	CString	m_GameDSN;
	CString	m_StatusDSN;
	CString	m_UserDSN;
	CString	m_BadUserPass;
	CString	m_GamePass;
	CString	m_StatusPass;
	CString	m_UserPass;
	CString	m_BadUserTable;
	CString	m_GameTable;
	CString	m_StatusTable;
	CString	m_UserTable;
	CString	m_BadUserUID;
	CString	m_GameUID;
	CString	m_StatusUID;
	CString	m_UserUID;
	CString	m_AvatarDSN;
	CString	m_AvatarPass;
	CString	m_AvatarTable;
	CString	m_AvatarUID;
	int		m_nMSCode;
	CString	m_ItemDSN;
	CString	m_ItemPass;
	CString	m_ItemTable;
	CString	m_ItemUID;
	CString	m_PremiumDSN;		// [프리미엄 작업]
	CString	m_PremiumPass;
	CString	m_PremiumTable;
	CString	m_PremiumUID;
	CString	m_PremIPDSN;		// [PC방 작업]
	CString	m_PremIPPass;
	CString	m_PremIPTable;
	CString	m_PremIPUID;
	CString	m_strMaxMoney;
	BOOL	m_bItem;
	BOOL	m_bScret;
	BOOL	m_bJump;
	CString	m_LogRecDSN;        // ### [로그 기록용] ###
	CString	m_LogRecPass;
	CString	m_LogRecTable;
	CString	m_LogRecUID;
	BOOL	m_bLogDB;
	CString	m_EventDSN;        // ### [이벤트 티켓] ###
	CString	m_EventPass;
	CString	m_EventTable;
	CString	m_EventUID;
	BOOL	m_bEventDB;
	CString	m_StatisticsDSN;    // [ 통계기록 ]
	CString	m_StatisticsPass;   // [ 통계기록 ]
	CString	m_StatisticsTable;  // [ 통계기록 ]
	CString	m_StatisticsUID;    // [ 통계기록 ]
	BOOL	m_bStatisticsDB;      // [ 통계기록 ]
	CString	m_PreUserLGMLogDSN;
	CString	m_PreUserLGMLogPass;
	CString	m_PreUserLGMLogTable;
	CString	m_PreUserLGMLogUID;
	BOOL	m_bPreUserLGMLogDB;
	CString	m_AllinChargeDSN;		// [수호천사]
	CString	m_AllinChargePass;		// [수호천사]
	CString	m_AllinChargeTable;		// [수호천사]
	CString	m_AllinChargeUID;		// [수호천사]
	BOOL	m_bAllinDB;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGDLG_H__702E7D41_16DF_11D4_97A5_0050BF0FBE67__INCLUDED_)
