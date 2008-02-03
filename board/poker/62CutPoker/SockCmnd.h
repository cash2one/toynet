#if !defined(AFX_SOCKCMND_H__0D551C01_EDC8_11D3_97A2_0050BF0FBE67__INCLUDED_)
#define AFX_SOCKCMND_H__0D551C01_EDC8_11D3_97A2_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SockCmnd.h : header file
//

#include "SockMan.h"
#include "CommMsg.h"

const int MAINSERVER_PORT = 8219;
const int CLIENTSERVER_PORT = 5824;

#define UM_PACKET_NOTIFY (WM_USER+500)

struct GAMENOTIFY
{
	int Sid;
	int Signal;
	int TotSize;
	char *lpData;
};

/////////////////////////////////////////////////////////////////////////////
// CSockCmnd window

class CSockCmnd : public CWnd
{
// Construction
public:
	public:
	CWnd *pParentWnd;		// 메세지를 보낼 부모윈도우
	CString szMainServerIP;	// 메인서버의 IP

	CSockCmnd();
	BOOL Init(CWnd *parent);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSockCmnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSockCmnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSockCmnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg LONG OnServAccept(UINT, LONG);
	afx_msg LONG OnServClose(UINT, LONG);
	afx_msg LONG OnDataReceive(UINT, LONG);
	afx_msg LONG OnDataClose(UINT, LONG);
	afx_msg LONG OnDataConnect(UINT, LONG);
	afx_msg LONG OnDataConnectFailure(UINT, LONG);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////




//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKCMND_H__0D551C01_EDC8_11D3_97A2_0050BF0FBE67__INCLUDED_)

