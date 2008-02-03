// SrvInfoMan.cpp: implementation of the CSrvInfoMan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NMServerAgent.h"
#include "NMServerAgentDlg.h"
#include "SrvInfoMan.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSrvInfoMan::CSrvInfoMan()
{
	Clear();
}

CSrvInfoMan::~CSrvInfoMan()
{
	Clear();
}

void CSrvInfoMan::Clear()
{
	for(int i=0; i<MAX_CONNECT_SERVER; i++) m_Data[i].Clear();
}

// 소켓번호의 데이타를 지운다!!
void CSrvInfoMan::Clear(int nSID)
{
	int nIndex = GetIndex(nSID);
	if(nIndex<0) return;
	
	m_Data[nIndex].Clear();
}

// 사용자 정보데이타를 클리어 한다
void CSrvInfoMan::ClearUserData(int nSID)
{
	int nIndex = GetIndex(nSID);
	if(nIndex<0) return;
	
	m_Data[nIndex].ClearUserData();
}

// 소켓아이디를 주어 인덱스를 구한다
int CSrvInfoMan::GetIndex(int nSID)
{
	if(nSID<0) return -1;
	int nFindIndex = -1;
	for(int i=0; i<MAX_CONNECT_SERVER; i++){
		if(m_Data[i].m_nSID==nSID){
			nFindIndex = i;
			break;
		}
	}
	if(nFindIndex>=MAX_CONNECT_SERVER) return -1;

	return nFindIndex;
}

// 접속요청을 해놓고 세팅한다
void CSrvInfoMan::SetData(CString strName, CString strIP, UINT nPort, int nSID)
{
	if( strName==_T("") || strIP==_T("") || nSID<0 ) return;
	if(IsConnect(strName))     return;
	if(IsConnect(strIP,nPort)) return;

	int nFindIndex = -1;
	for(int i=0; i<MAX_CONNECT_SERVER; i++){
		if(!m_Data[i].m_bValid && !m_Data[i].m_bConnect){
			nFindIndex = i;
			break;
		}
	}
	
	if(nFindIndex>=0 && nFindIndex<MAX_CONNECT_SERVER){
		m_Data[nFindIndex].SetSrvInfo(strName,strIP,nPort,nSID);
	}
}

// 이미 접속중인지 확인하자 (이름)
BOOL CSrvInfoMan::IsConnect(CString strName)
{
	if(strName==_T("")) return FALSE;
	for(int i=0; i<MAX_CONNECT_SERVER; i++){
		if(m_Data[i].m_strName == strName){
			return TRUE;
		}
	}
	return FALSE;
}

// 이미 접속중인지 확인하자 (아이피와 포트)
BOOL CSrvInfoMan::IsConnect(CString strIP, UINT nPort)
{
	if(strIP==_T("") || nPort==0) return FALSE;
	for(int i=0; i<MAX_CONNECT_SERVER; i++){
		if(m_Data[i].m_strIP == strIP && m_Data[i].m_nPort==nPort){
			return TRUE;
		}
	}
	return FALSE;
}

// 해당소켓의 서버정보를 구하자
CSrvInfo * CSrvInfoMan::GetSrvInfo(int nSID)
{
	int nIndex = GetIndex(nSID);
	if(nIndex<0) return NULL;
	return &(m_Data[nIndex]);
}

// 해당이름의 서버정보를 구하자
CSrvInfo * CSrvInfoMan::GetSrvInfo(CString strName)
{
	if(strName==_T("")) return NULL;
	for(int i=0; i<MAX_CONNECT_SERVER; i++){
		if(m_Data[i].m_strName == strName){
			return &(m_Data[i]);
		}
	}
	return NULL;
}

CString CSrvInfoMan::GetIP(int nSID)
{
	int nIndex = GetIndex(nSID);
	if(nIndex<0) return _T("");
	return m_Data[nIndex].m_strIP;
}

CString CSrvInfoMan::GetName(int nSID)
{
	int nIndex = GetIndex(nSID);
	if(nIndex<0) return _T("");;
	return m_Data[nIndex].m_strName;
}

UINT CSrvInfoMan::GetPort(int nSID)
{
	int nIndex = GetIndex(nSID);
	if(nIndex<0) return 0;
	return m_Data[nIndex].m_nPort;
}

// 접속상태를 표시한다
void CSrvInfoMan::Connect(int nSID)
{
	int nIndex = GetIndex(nSID);
	if(nIndex<0) return;
	m_Data[nIndex].m_bConnect = TRUE;
	m_Data[nIndex].m_nNowChan = 0;
}

// 총접속자수를 세팅한다
void CSrvInfoMan::SetTotal(int nSID, int nTotal, INT64 jackpot)
{
	int nIndex = GetIndex(nSID);
	if(nIndex<0) return;
	
	m_Data[nIndex].m_nUserNumber = nTotal;
	m_Data[nIndex].m_JackPot = jackpot;
}

// 잭팟금액만 세팅한다.
void CSrvInfoMan::SetJackPot(int nSID, INT64 jackpot)
{
	int nIndex = GetIndex(nSID);
	if(nIndex<0) return;

	m_Data[nIndex].m_JackPot = jackpot;
}

// 총접속자 수를 구한다
int CSrvInfoMan::GetTotal(int nSID)
{
	int nIndex = GetIndex(nSID);
	if(nIndex<0) return 0;
	return m_Data[nIndex].m_nUserNumber;
}

// 서버에서 받은 정보로 데이타를 세팅한다
BOOL CSrvInfoMan::SetUser(int nSID, AGENT_USER &au)
{
	int nIndex = GetIndex(nSID);
	if(nIndex<0) return FALSE;
	
	m_Data[nIndex].SetUser(au);

	return TRUE;
}

// 사용자를 제거한다
BOOL CSrvInfoMan::DelUser(int nSID, AGENT_USER &au)
{
	int nIndex = GetIndex(nSID);
	if(nIndex<0) return FALSE;

	m_Data[nIndex].DelUser(au);

	return TRUE;
}

// 이미 접속해 있는 사용자인지 확인하자 (전서버를 확인한다)
BOOL CSrvInfoMan::bFind(char *ID)
{
	if(ID==NULL) return FALSE;
	char szID[16]={0,};
	wsprintf(szID, "%s", ID);

	BOOL bFind = FALSE;
	for(int i=0; i<MAX_CONNECT_SERVER; i++){
		if(!m_Data[i].m_bValid)   continue;
		if(!m_Data[i].m_bConnect) continue;

		if( m_Data[i].bFind(szID)==TRUE ){
			bFind = TRUE;
			break;
		}
	}
	return bFind;
}

// 이미 접속해 있는 사용자인지 확인하자 (해당서버를 제외하고 확인한다)
BOOL CSrvInfoMan::bFindExcept(char *ID, int nSID)
{
	if(ID==NULL || nSID<0) return FALSE;
	char szID[16]={0,};
	wsprintf(szID, "%s", ID);

	BOOL bFind = FALSE;
	for(int i=0; i<MAX_CONNECT_SERVER; i++){
		if(!m_Data[i].m_bValid)    continue;
		if(!m_Data[i].m_bConnect)  continue;

		if(m_Data[i].m_nSID==nSID) continue; // 해당서버는 제외한다!!

		if( m_Data[i].bFind(szID)==TRUE ){
			bFind = TRUE;
			break;
		}
	}
	return bFind;
}

// 아이디를 주어 접속한 사용자의 정보를 구하자
int CSrvInfoMan::GetUser(CString strID, AGENT_USER *pau, int *sid)
{
	if(pau==NULL || sid==NULL) return 0;
	if(strID==_T("") || strID.GetLength()>15) return 0;

	char szID[16]={0,};
	wsprintf(szID, "%s", (char *)(strID.operator LPCTSTR()));

	int nTotal = 0;
	for(int i=0; i<MAX_CONNECT_SERVER; i++){
		if(!m_Data[i].m_bValid)   continue;
		if(!m_Data[i].m_bConnect) continue;

		if( m_Data[i].GetUser(szID,pau[nTotal]) ){
			sid[nTotal] = m_Data[i].m_nSID;
			nTotal++;
		}
	}
	return nTotal;
}

