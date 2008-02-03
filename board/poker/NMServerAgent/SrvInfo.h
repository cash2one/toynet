// SrvInfo.h: interface for the CSrvInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SRVINFO_H__F6849AAA_01E1_48E7_9E21_871E4E31D3BF__INCLUDED_)
#define AFX_SRVINFO_H__F6849AAA_01E1_48E7_9E21_871E4E31D3BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserStruct.h"

//#define MAX_USER           2211 // ������ �ִ� �����ڼ�+10 : �ش� ���Ӻ��� ���������ؾ� �Ѵ�
#define MAX_USER           3010 // ������ �ִ� �����ڼ�+10 : �ش� ���Ӻ��� ���������ؾ� �Ѵ�
#define MAX_CONNECT_SERVER   20 // ���Ӱ����� ���� ����

// �������� Ŭ����
class CSrvInfo  
{
public:
	
	BOOL       m_bValid;
	int        m_nSID;
	BOOL       m_bConnect;
	AGENT_USER m_AU[MAX_USER];
	//���� ������ 5���� ����
//	int        bDelUserR[MAX_USER];
//	int        nDelUserCnt[MAX_USER];
	

	CString    m_strIP,m_strName;
	UINT       m_nPort;

	INT64      m_JackPot;

	int        m_nNowChan; // ���� ����Ʈ�� ä�ι�ȣ

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
