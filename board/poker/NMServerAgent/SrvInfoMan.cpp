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

// ���Ϲ�ȣ�� ����Ÿ�� �����!!
void CSrvInfoMan::Clear(int nSID)
{
	int nIndex = GetIndex(nSID);
	if(nIndex<0) return;
	
	m_Data[nIndex].Clear();
}

// ����� ��������Ÿ�� Ŭ���� �Ѵ�
void CSrvInfoMan::ClearUserData(int nSID)
{
	int nIndex = GetIndex(nSID);
	if(nIndex<0) return;
	
	m_Data[nIndex].ClearUserData();
}

// ���Ͼ��̵� �־� �ε����� ���Ѵ�
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

// ���ӿ�û�� �س��� �����Ѵ�
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

// �̹� ���������� Ȯ������ (�̸�)
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

// �̹� ���������� Ȯ������ (�����ǿ� ��Ʈ)
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

// �ش������ ���������� ������
CSrvInfo * CSrvInfoMan::GetSrvInfo(int nSID)
{
	int nIndex = GetIndex(nSID);
	if(nIndex<0) return NULL;
	return &(m_Data[nIndex]);
}

// �ش��̸��� ���������� ������
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

// ���ӻ��¸� ǥ���Ѵ�
void CSrvInfoMan::Connect(int nSID)
{
	int nIndex = GetIndex(nSID);
	if(nIndex<0) return;
	m_Data[nIndex].m_bConnect = TRUE;
	m_Data[nIndex].m_nNowChan = 0;
}

// �������ڼ��� �����Ѵ�
void CSrvInfoMan::SetTotal(int nSID, int nTotal, INT64 jackpot)
{
	int nIndex = GetIndex(nSID);
	if(nIndex<0) return;
	
	m_Data[nIndex].m_nUserNumber = nTotal;
	m_Data[nIndex].m_JackPot = jackpot;
}

// ���̱ݾ׸� �����Ѵ�.
void CSrvInfoMan::SetJackPot(int nSID, INT64 jackpot)
{
	int nIndex = GetIndex(nSID);
	if(nIndex<0) return;

	m_Data[nIndex].m_JackPot = jackpot;
}

// �������� ���� ���Ѵ�
int CSrvInfoMan::GetTotal(int nSID)
{
	int nIndex = GetIndex(nSID);
	if(nIndex<0) return 0;
	return m_Data[nIndex].m_nUserNumber;
}

// �������� ���� ������ ����Ÿ�� �����Ѵ�
BOOL CSrvInfoMan::SetUser(int nSID, AGENT_USER &au)
{
	int nIndex = GetIndex(nSID);
	if(nIndex<0) return FALSE;
	
	m_Data[nIndex].SetUser(au);

	return TRUE;
}

// ����ڸ� �����Ѵ�
BOOL CSrvInfoMan::DelUser(int nSID, AGENT_USER &au)
{
	int nIndex = GetIndex(nSID);
	if(nIndex<0) return FALSE;

	m_Data[nIndex].DelUser(au);

	return TRUE;
}

// �̹� ������ �ִ� ��������� Ȯ������ (�������� Ȯ���Ѵ�)
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

// �̹� ������ �ִ� ��������� Ȯ������ (�ش缭���� �����ϰ� Ȯ���Ѵ�)
BOOL CSrvInfoMan::bFindExcept(char *ID, int nSID)
{
	if(ID==NULL || nSID<0) return FALSE;
	char szID[16]={0,};
	wsprintf(szID, "%s", ID);

	BOOL bFind = FALSE;
	for(int i=0; i<MAX_CONNECT_SERVER; i++){
		if(!m_Data[i].m_bValid)    continue;
		if(!m_Data[i].m_bConnect)  continue;

		if(m_Data[i].m_nSID==nSID) continue; // �ش缭���� �����Ѵ�!!

		if( m_Data[i].bFind(szID)==TRUE ){
			bFind = TRUE;
			break;
		}
	}
	return bFind;
}

// ���̵� �־� ������ ������� ������ ������
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

