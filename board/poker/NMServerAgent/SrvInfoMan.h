// SrvInfoMan.h: interface for the CSrvInfoMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SRVINFOMAN_H__5E799A9D_EAA7_4E3B_B528_ACB54B75C128__INCLUDED_)
#define AFX_SRVINFOMAN_H__5E799A9D_EAA7_4E3B_B528_ACB54B75C128__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SrvInfo.h"
#include "UserStruct.h"	// Added by ClassView

class CSrvInfoMan  
{
public:

	int  GetUser(CString strID, AGENT_USER *pau, int *psid);
	BOOL SetUser(int nSID, AGENT_USER &au);
	BOOL DelUser(int nSID, AGENT_USER &au);
	BOOL bFindExcept(char *ID, int nSID);
	BOOL bFind(char *ID);

	int  GetIndex(int nSID);

	void Connect(int nSID);
	void ClearUserData(int nSID);

	int  GetTotal(int nSID);
	void SetTotal(int nSID, int nTotal, INT64 jackpot = 0);

	// ÀèÆÌ ±Ý¾× ¼¼ÆÃ
	void SetJackPot(int nSID, INT64 jackPot);

	void       SetData(CString strName,CString strIP, UINT nPort,int nSID);
	UINT       GetPort(int nSID);
	CString    GetName(int nSID);
	CString    GetIP(int nSID);
	CSrvInfo * GetSrvInfo(int nSID);
	CSrvInfo * GetSrvInfo(CString strName);

	BOOL IsConnect(CString strName);
	BOOL IsConnect(CString strIP, UINT nPort);

	CSrvInfo m_Data[MAX_CONNECT_SERVER];

	void Clear();
	void Clear(int nSID);

	CSrvInfoMan();
	virtual ~CSrvInfoMan();

};

#endif // !defined(AFX_SRVINFOMAN_H__5E799A9D_EAA7_4E3B_B528_ACB54B75C128__INCLUDED_)
