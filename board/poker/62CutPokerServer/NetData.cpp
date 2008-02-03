// NetData.cpp: implementation of the CNetData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NetData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CNetData,CObject,50)

// �⺻���� �������̴�.
CNetData::CNetData()
{
	m_chCode[ 0] = 'V';
	m_chCode[ 1] = 'a';
	m_chCode[ 2] = 's';
	m_chCode[ 3] = '!';
	m_chCode[ 4] = '*';
	m_chCode[ 5] = 'Q';
	m_chCode[ 6] = 'w';
	m_chCode[ 7] = 'o';
	m_chCode[ 8] = 'P';
	m_chCode[ 9] = '&';
	m_chCode[10] = '>';
	m_chCode[11] = '?';
	m_ch1 = 'H';
	m_ch2 = 'S';

	m_lpData  = NULL;
	m_NetData = NULL;
	InitClass();
}

// ��Ʈ����.
CNetData::~CNetData()
{
	if(m_lpData)
		delete m_lpData;
	if(m_NetData)
		delete m_NetData;
	m_lpData = NULL;
	m_NetData = NULL;

	memset(&m_NetHeader,0,sizeof(NETHEADER));
	m_Size = 0;
}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CNetData, CObject)
	//{{AFX_MSG_MAP(CNetData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

// Ŭ������ �ʱ�ȭ��ƾ. �⺻���� ������ ���⼭ ����.
int CNetData::InitClass()
{
	m_Size = 0;
	if(m_lpData)  delete m_lpData;
	if(m_NetData) delete m_NetData;
	m_lpData  = NULL;
	m_NetData = NULL;
	ZeroMemory(&m_NetHeader,sizeof(NETHEADER));
	strcpy(m_NetHeader.Header,NETMARBLEHEADER); // ����ʱ�ȭ.
	m_NetHeader.Signal   = NM_NOSIGN; // �ñ׳��� ����.
	m_NetHeader.Type     = NM_DATA; //����Ʈ�� ����Ÿ��..

	return 1;
}

// ����Ÿ�� ��Ʈ��ũ������ �����Ѵ�.
BOOL CNetData::SetData(LPVOID lpdata, UINT nsize, DATATYPE type, SIGNAL signal,int userval/*=0*/)
{
	if(lpdata == NULL)
		return FALSE;
	InitClass();

	int nRand = 0;
	m_Size    = nsize;

	if(m_Size>0){
		m_lpData = new char[nsize];
		if(m_lpData == NULL){
			InitClass();
			return FALSE;
		}
		memcpy(m_lpData,lpdata,nsize);
		if(m_lpData == NULL){
			InitClass();
			return FALSE;
		}

		// ���⼭ ���ڵ��Ѵ�.
		int nS = (int)nsize;
		char *nOrgStart = (char *)m_lpData;
		nRand = (int)rand()*NETCODENUM/(RAND_MAX+1);

		if(nRand<0 || nRand>=NETCODENUM) nRand = 0;

		for(int i=0; i<nS; i++){
			TCHAR ch = *nOrgStart^m_ch1^m_chCode[nRand]^m_ch2;//(**����)
			*nOrgStart=ch;
			nOrgStart++;
		}
	}

	m_NetHeader.Type    = type;
	m_NetHeader.Signal  = signal;
	m_NetHeader.Size    = m_Size;
	m_NetHeader.Code    = (UINT)nRand;
	m_NetHeader.UserVal = userval;

	return TRUE;
}

// ����Ÿ ��ȣ�� ���Ѵ�.
SIGNAL CNetData::GetSign()
{
	return m_NetHeader.Signal;
}

// ����Ÿ ���¸� ���Ѵ�.
DATATYPE CNetData::GetType()
{
	return m_NetHeader.Type;
}

// ����� ���� ���� ���Ѵ�
int  CNetData::GetUserVal()
{
	return m_NetHeader.UserVal;
}

// ��ſ� ����Ÿ�� ���Ѵ�. ��ȣ ����Ÿ��� ����� ���Ѵ�.
char * CNetData::GetData()
{
	int size = m_Size + sizeof(NETHEADER); // ���� �������̴�.

	if(m_NetData!=NULL){
		delete m_NetData;
		m_NetData = NULL;
	}
	m_NetData = new char[size];
	memcpy(m_NetData,&m_NetHeader,sizeof(NETHEADER));
	if(m_Size>0) memcpy((char *)m_NetData+sizeof(NETHEADER),m_lpData,m_Size);

	return  (char *)m_NetData;
}

// ��ſ� ����Ÿ ����� ���Ѵ�. ��ȣ ����Ÿ��� ����ü ����� ���Ѵ�.
UINT CNetData::GetSize()
{
	return ((UINT)sizeof(NETHEADER) + m_Size);
}

// �ùٸ� ��Ʈ��ũ ����Ÿ���� Ȯ���Ѵ�. ������ �� ����Ÿ�� �Ҵ��Ѵ�.
BOOL CNetData::IsData(LPVOID lpData, int size)
{
	if(lpData == NULL || size == 0 || size < sizeof(NETHEADER))
		return FALSE;

	char  head[8] = {0,};
	memcpy(head,lpData,8);

	if(memcmp(head,NETMARBLEHEADER,8)==0){
		InitClass();
			
		memcpy(&m_NetHeader,lpData,sizeof(NETHEADER));
		if(m_NetHeader.Size < 0){ // �������� ����Ÿ�� ����. ��ȣ����Ÿ������ �𸥴�.
			ZeroMemory(&m_NetHeader,sizeof(NETHEADER));
			return FALSE;
		}

		// �߸��� �ڵ� ��ȣ��. ó������ �ʴ´�!!!
		if(m_NetHeader.Code<0 || m_NetHeader.Code>=NETCODENUM){
			ZeroMemory(&m_NetHeader,sizeof(NETHEADER));
			return FALSE;
		}

		m_Size = m_NetHeader.Size;

		if((int)(m_Size+sizeof(NETHEADER))>size) {
			m_Size = 0;
			ZeroMemory(&m_NetHeader,sizeof(NETHEADER));
			return FALSE;
		}

		if(m_Size>0){
			m_lpData = new char[m_Size];
			if(m_lpData==NULL){
				InitClass();
				return FALSE;
			}
			memcpy(m_lpData,(char *)lpData+sizeof(NETHEADER),m_Size);
			if(m_lpData==NULL){
				InitClass();
				return FALSE;
			}

			// ���⼭ ���ڵ��Ѵ�.
			int nS = (int)m_Size;
			char *nOrgStart = (char *)m_lpData;
			int nRand = (int)m_NetHeader.Code;

			if(nRand<0 || nRand>=NETCODENUM) return FALSE;

			for(int i=0; i<nS; i++){
				TCHAR ch = *nOrgStart^m_ch1^m_chCode[nRand]^m_ch2;//(**����)
				*nOrgStart=ch;
				nOrgStart++;
			}
		}
	}
	else // �ùٸ� ����Ÿ�� �ƴϴ�.
		return FALSE;
	
	return TRUE;
}


CNetData& CNetData::operator=( CNetData &assign )
{
	InitClass(); // �ϴ� �ѹ� �ٽ� �ʱ�ȭ ����.
	memcpy(&m_NetHeader,&assign.m_NetHeader,sizeof(NETHEADER));
	m_Size   = m_NetHeader.Size;
	if(m_Size>0){
		m_lpData = new char[m_Size];
		if(m_lpData==NULL){
			InitClass();
			return *this;
		}
		memcpy(m_lpData,assign.m_lpData,m_Size);
		if(m_lpData == NULL){
			InitClass();
			return *this;
		}
	}
	return *this;
}

BOOL CNetData::CheckHeader(char *lpdata, int *psignal, int *pmsglen)
{
	if(strncmp(lpdata, NETMARBLEHEADER, 8) != 0) return FALSE;
	*pmsglen = *((UINT*)(lpdata + 8));
	*psignal = (int)(*((SIGNAL*)(lpdata + 12)));
	return TRUE;
}

#ifdef _DEBUG
void CNetData::AssertValid() const
{
	CObject::AssertValid();
}
void CNetData::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}
#endif
