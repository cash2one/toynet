// SrvInfo.h: interface for the CSrvInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SRVINFO_H__F6849AAA_01E1_48E7_9E21_871E4E31D3BF__INCLUDED_)
#define AFX_SRVINFO_H__F6849AAA_01E1_48E7_9E21_871E4E31D3BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserStruct.h"

//#define MAX_USER           2211 // 서버당 최대 접속자수+10 : 해당 게임별로 조정가능해야 한다
#define MAX_USER           3010 // 서버당 최대 접속자수+10 : 해당 게임별로 조정가능해야 한다
#define MAX_CONNECT_SERVER   20 // 접속가능한 서버 갯수

// 서버정보 클래스
class CSrvInfo  
{
public:
	
	BOOL       m_bValid;
	int        m_nSID;
	BOOL       m_bConnect;
	AGENT_USER m_AU[MAX_USER];
	//제거 예약후 5초후 제거
//	int        bDelUserR[MAX_USER];
//	int        nDelUserCnt[MAX_USER];
	

	CString    m_strIP,m_strName;
	UINT       m_nPort;

	INT64      m_JackPot;

	int        m_nNowChan; // 받은 리스트의 채널번호

	int        m_nUserNumber;

	////////////////////////////
//	INT64		JackPotMoney;
	///////////////////////////

	void SetUser(AGENT_USER &au);
	void DelUser(AGENT_USER &au);
	BOOL bFind(char *ID);
	BOOL GetUser(char *ID, AGENT_USER &au);

	void SetSrvInfo(CString strName,CString strIP, UINT nPort,int nSID);

	void Clear();
	void ClearUserData();

	CSrvInfo();
	virtual ~CSrvInfo();
	CSrvInfo& operator=( CSrvInfo &assign );

};

#endif // !defined(AFX_SRVINFO_H__F6849AAA_01E1_48E7_9E21_871E4E31D3BF__INCLUDED_)
