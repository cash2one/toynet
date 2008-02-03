// SrvInfo.cpp: implementation of the CSrvInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NMServerAgent.h"
#include "NMServerAgentDlg.h"
#include "SrvInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CSrvInfo Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSrvInfo::CSrvInfo()
{
	Clear();
}

CSrvInfo::~CSrvInfo()
{
	Clear();
}

CSrvInfo& CSrvInfo::operator=( CSrvInfo &assign )
{
	Clear();

	m_bValid      = assign.m_bValid;
	m_nSID        = assign.m_nSID;
	m_bConnect    = assign.m_bConnect;

	m_strName     = assign.m_strName;
	m_strIP       = assign.m_strIP;
	m_nPort       = assign.m_nPort;
	m_nUserNumber = assign.m_nUserNumber;

	m_nNowChan    = assign.m_nNowChan;

	m_JackPot     = assign.m_JackPot;

	memcpy(m_AU,assign.m_AU,sizeof(AGENT_USER)*MAX_USER);

//	ZeroMemory(bDelUserR, sizeof(bDelUserR));
//	ZeroMemory(nDelUserCnt, sizeof(nDelUserCnt));

	return *this;
}


void CSrvInfo::Clear()
{
	m_bValid   = FALSE;
	m_nSID     = -1; // 소켓 아이디
	m_bConnect =  FALSE;

	m_strName  = _T("");
	m_strIP    = _T("");
	m_nPort    = 0;
	
	m_nNowChan = 0;

	m_JackPot = 0;

	ClearUserData();
}

void CSrvInfo::ClearUserData()
{
	m_nUserNumber = 0;
	memset(m_AU,0,sizeof(AGENT_USER)*MAX_USER);
}

// 서버의 접속정보를 세팅한다
void CSrvInfo::SetSrvInfo(CString strName, CString strIP, UINT nPort, int nSID)
{
	m_bValid  = TRUE;
	m_strName = strName;
	m_strIP   = strIP;
	m_nPort   = nPort;
	m_nSID    = nSID;
}

void CSrvInfo::SetUser(AGENT_USER &au)
{
	int unum = au.nUNum;
	if(unum<0 || unum>=MAX_USER || strlen(au.ID)<2) return;
	
	memset( &m_AU[unum],   0, sizeof(AGENT_USER) );
	memcpy( &m_AU[unum], &au, sizeof(AGENT_USER) );
}

void CSrvInfo::DelUser(AGENT_USER &au)
{
	int unum = au.nUNum;
	if(unum<0 || unum>=MAX_USER) return;
	
	memset( &m_AU[unum],   0, sizeof(AGENT_USER) );
//	bDelUserR[unum]   = 1;
//	nDelUserCnt[unum] = 6;

}

// 사용자를 검색한다
BOOL CSrvInfo::bFind(char *ID)
{
	if(ID==NULL) return FALSE;

	BOOL bFind = FALSE;
	for(int i=0; i<MAX_USER; i++){
// [대소문자 구분 제거]		if(strncmp(m_AU[i].ID,ID,15)==0){
		if(_strnicmp(m_AU[i].ID,ID,15)==0){
			bFind = TRUE;
			break;
		}
	}
	return bFind;
}

// 사용자 정보를 구한다
BOOL CSrvInfo::GetUser(char *ID, AGENT_USER &au)
{
	if(ID==NULL) return FALSE;

	BOOL bFind = FALSE;
	for(int i=0; i<MAX_USER; i++){
// [대소문자 구분 제거]		if(strncmp(m_AU[i].ID,ID,15)==0){
		if(_strnicmp(m_AU[i].ID,ID,15)==0){
			memcpy(&au,&(m_AU[i]),sizeof(AGENT_USER));
			bFind = TRUE;
			break;
		}
	}
	return bFind;
}

