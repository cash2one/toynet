// NMServerAgent.h : main header file for the NMSERVERAGENT application
//

#if !defined(AFX_NMSERVERAGENT_H__366881EA_8978_44CF_9008_7B5C641F9BD9__INCLUDED_)
#define AFX_NMSERVERAGENT_H__366881EA_8978_44CF_9008_7B5C641F9BD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include "UserStruct.h"
#include "SockMan.h"
#include "SrvInfoMan.h"

#include <direct.h>

#define CONNECT_ID   "CTRA_CONNECT"
#define CONNECT_PASS "9669"

extern CSockMan     SockMan;  // 소켓 메니져 전역 객체
extern CSrvInfoMan  ServerMan;

extern CString   g_AppStartPath;

// 평균 잭팟 금액
extern INT64 g_AverJackPot;


extern CString g_MakeCommaMoney(INT64 nMoney);

// 서버 리스트 데이타
typedef struct{
	char Name[64];
	char IP[64];
	UINT nPort;
} SERVER_INFO;

// 서버 리스트 데이타(저장용)
typedef struct{
	int nTotalList;
	SERVER_INFO INFO[MAX_CONNECT_SERVER];
} SERVER_INFO_SAVE;

extern SERVER_INFO_SAVE g_ServerInfoList;

extern AGENT_ADMIN g_AdminInfo;

/////////////////////////////////////////////////////////////////////////////
// CNMServerAgentApp:
// See NMServerAgent.cpp for the implementation of this class
//

class CNMServerAgentApp : public CWinApp
{
public:
	CNMServerAgentApp();

	void SaveCurrentDir();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNMServerAgentApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CNMServerAgentApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NMSERVERAGENT_H__366881EA_8978_44CF_9008_7B5C641F9BD9__INCLUDED_)
