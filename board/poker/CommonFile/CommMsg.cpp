#include "StdAfx.h"
#include "CommMsg.h"


////////////////////////////////////////////////////////////////////////////
//                              �޼��� �⺻ Ŭ����
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
//                              �޼��� �⺻ Ŭ����
////////////////////////////////////////////////////////////////////////////
// ��ȣȭ/��ȣȭ�� ���̺�
static char g_GHashTable[256] = 
{
  48,-106,  34,  50,  22, -94,   5, -68,  37, -84,-128,  16,
 107,   1,  59, -53, -43,  56, 124,-100, -29, -56, -33, 117,
-108,  67,-127,  52,  85,  53,  -3, -34,  63,  -2,  73,  64,
-121,  30, -98, -18, -93, -61,  25,  80,-101, -74,  66,-126,
-118, -49, -44, 119, -21,  32, -65,  91,-116,-104,-122, -69,
-111,  90,  17, -46, 109,-105,  84, -35, -75, -62,  14,  62,
 -78,  39, 115, 105, 110,  20, -54, -30,  31,  99,  19, -72,
 -79,  35, -32, -17,  12, 114,  97,  26, -26,  61,  65, 123,
   9,  11, -11,-109,  23,  15, 116,  58,-107, -83,  41, -13,
   2, 100, -22,  10,  87, -91,  79,   3, 127,  49, -15, -77,
  -4,  -6,  33, 104, -27, -82, -10,-110,  92,   8,  13,  38,
  77,  54,  78, -64, -87,  44, -57, -97,-123,  96,  24,  69,
  47, -47, -76,  70,  27,  95,  81, -70, -52,-120, -38,-119,
  -7, 122,   4, -16, 118, 111,-114, -85,  -5,-124, -58,  -1,
  74,  82, -80, 113,  28, -89, 103, -51,  72, -95, -31,  42,
   6,-117, -28,  86, -19, -25,  88,  94, -42,  -9,-112,  57,
 120, -86,  29, -99, -60, -59,  27, -88, -36,  43, -90, -45,
 -63, -12, -96,   7, -73,  93,  40, -66, -41, 102, 125,-103,
 -14, -50,  83, 106, -55,  36,  68, 126,  71, -71,  46, -39,
  76,  60, 101, 112, -24,  75,  45,-113,-102, 108, -81, -20,
  -8,  98, -40, -67,  21,-115,  89, 121,-125, -92, -37, -23,
 -48,  51,  55,  18
};


////////////// CommMsg Ŭ������ �Ʒ��Ͱ��� �����Ѵ�. �Լ��� ���� ���켼��
CCommMsg::CCommMsg()
{
	bReferedData = FALSE;
	TotalSize = 0;
	pData=NULL;
}

CCommMsg::CCommMsg(char *pRefBuf)
{
	bReferedData = TRUE;
	TotalSize = 0;
	pData = pRefBuf;
}

CCommMsg::~CCommMsg()
{
	Clear();
}

void CCommMsg::Clear()
{
	if(!bReferedData) {
		if(pData) free(pData);
	}
	pData=NULL;
	bReferedData = FALSE;
	TotalSize = 0;
}

int CCommMsg::GetTotalSize()
{
	if(pData==NULL) return 0;
	return TotalSize;
}

//---- �Լ� �߰�
void CCommMsg::BuildPattern(char *pdata, UINT packcnt, int size)
{
	int i;
	switch((packcnt>>4)&4) {
		case 0:
			{
			} break;

		case 1:
			{
				char temp;
				int szi = (size >> 1);
				if(szi <2) return;
				for(i = 0; i < szi;i++) {
					temp = pdata[i];
					pdata[i] = pdata[szi+i];
					pdata[szi+i] = temp;
				}
			} break;

		case 2:
			{
				int szi;
				WORD tempWD;
				WORD *pWdata = (WORD *)pdata;
				szi = (size >> 1) -1;
				if(szi <2) return;
				for(i = 0; i < szi; i+=2) {
					tempWD = pWdata[i];
					pWdata[i] = pWdata[i+1];
					pWdata[i+1] = tempWD;
				}
			} break;

		case 3:
			{
				char temp;
				int szi = size-1;
				if(szi <2) return;
				for(i = 0; i < szi;i++) {
					temp = pdata[i];
					pdata[i] = pdata[i+1];
					pdata[i+1] = temp;
				}
			} break;
	}
}

//---- �Լ� �߰�
void CCommMsg::EncodeAll(char *pdata)
{
	char *pUserData = (char *)(pdata + sizeof(PACKETHEADER));
	PACKETHEADER *phead = (PACKETHEADER*)pdata;
	if(phead->Dummy[1]==2) return;
	phead->PackCRC = g_GHashTable[phead->PackCnt & 255] & 0x7F;
	
	for(int i=0; i<phead->MsgLen; i++) 
	{
		phead->PackCRC += (pUserData[i]&0x5F);
		pUserData[i] += g_GHashTable[(phead->PackCnt+51) & 255];
		pUserData[i] ^= g_GHashTable[(phead->PackCnt+27) & 255];
	}
	phead->PackCRC += g_GHashTable[(31+phead->PackCnt) & 255] & 0x7F;
	phead->PackCRC += phead->Signal;
	phead->PackCRC = (phead->PackCRC & 0x0FFFFFFF);
	
	if(phead->PackCRC<0x0FFFF) {
		phead->PackCRC += ((g_GHashTable[phead->PackCnt & 255] & 0xF) << 24) +
					((g_GHashTable[(phead->PackCnt+1) & 255] & 0xF) << 20) +
				    ((g_GHashTable[(phead->PackCnt+3) & 255] & 0xF) << 16);
		phead->PackCRC = (phead->PackCRC << 4) + 0xF;	
	}
	else
	if(phead->PackCRC<0x0FFFFF) {
		phead->PackCRC += ((g_GHashTable[phead->PackCnt & 255] & 0xF) << 24) +
					((g_GHashTable[(phead->PackCnt+1) & 255] & 0xF) << 20);
		phead->PackCRC = (phead->PackCRC << 4) + (g_GHashTable[(phead->PackCnt+5) & 255] & 0x7);
					
	}else {
		phead->PackCRC = (phead->PackCRC << 4) + 0x0A;
	}
	phead->Signal += g_GHashTable[phead->PackCnt & 255];
	phead->Dummy[1] = 2;
	phead->Dummy[0] = phead->PackCRC & 0x1C;
	BuildPattern(pUserData, phead->PackCRC, phead->MsgLen);
}


//---- �Լ� �߰�
BOOL CCommMsg::DecodeAll(char *pdata)
{

	int signal =0;
	char *pUserData = (char *)(pdata + sizeof(PACKETHEADER));
	PACKETHEADER *phead = (PACKETHEADER*)pdata;

	if(phead->Dummy[1] != 2) return FALSE;

	BuildPattern(pUserData, phead->PackCRC, phead->MsgLen);

	phead->Signal -= g_GHashTable[phead->PackCnt & 255];
	UINT tempCRC = g_GHashTable[phead->PackCnt & 255] & 0x7F;
	for(int i=0; i<phead->MsgLen; i++) {
		pUserData[i] ^= g_GHashTable[(phead->PackCnt+27) & 255];
		pUserData[i] -= g_GHashTable[(phead->PackCnt+51) & 255];

		tempCRC += (pUserData[i]&0x5F);
	}
	phead->Dummy[0] = 0;

	tempCRC+=g_GHashTable[(31+phead->PackCnt) & 255] & 0x7F;


	tempCRC = tempCRC & 0x0FFFFFFF;

	int flag = phead->PackCRC & 0x0F;
	phead->PackCRC = phead->PackCRC >> 4;
	if(flag==0x0F) phead->PackCRC &= 0x0FFFF;
	else 
	if(flag==(g_GHashTable[(phead->PackCnt+5) & 255] & 0x7)) phead->PackCRC &= 0x0FFFFF;
	else 
	if(flag!=0x0A)
	{
		phead->Dummy[0] = -1;
		return FALSE;
	}
	signal = phead->PackCRC - tempCRC;

	if(signal != phead->Signal) {
		phead->Dummy[0] = -1;
		return FALSE;
	}

	phead->Dummy[1] = 1;
	if(phead->PackCRC != tempCRC+signal) {
		phead->Dummy[0] = -1;
		return FALSE;
	}
	phead->Dummy[1] = 1;

	return TRUE;
}



//---- ���� �Լ� ��ü
BOOL CCommMsg::CheckInfo(char *lpdata, int *psignal, int *pmsglen)
{
	PACKETHEADER *pH = (PACKETHEADER*)lpdata;
	if(*((short*)pH->MsgID) != *((short*)MSG_ID)) return FALSE;
	if(pH->Dummy[1] != 2) return FALSE;
	*psignal = (int)(pH->Signal-g_GHashTable[pH->PackCnt & 255]);
	*pmsglen = (int)pH->MsgLen;
	return TRUE;
}

//---- ���� �Լ� ��ü
char* CCommMsg::SetHeader(int signal, int msglen)
{
	// ���۰� ������ �����Ͱ� �ƴ� ��츸 ���ο� �޸𸮸� �Ҵ�
	if(!bReferedData) {
		if(pData) free(pData);
		pData = (char*)malloc(PACKET_HEADERSIZE + msglen);
		if(pData==NULL) return NULL;
	}

	// �޽��� ������ ���
	TotalSize = PACKET_HEADERSIZE + msglen;

	pHead = (PACKETHEADER*)pData;
	*((short*)pHead->MsgID) = *((short*)MSG_ID);
	pHead->Signal = (short)signal;
	pHead->MsgLen = (short)msglen;
	pHead->Dummy[1] = 1;

	return (pData + PACKET_HEADERSIZE);
}

//---- ���� �Լ� ��ü
char* CCommMsg::GetHeader(char *lpdata, int size)
{
	if(!lpdata) return NULL;
	if(!bReferedData && pData) free(pData);
	pData = lpdata;
	pHead = (PACKETHEADER*)lpdata;
	
	TotalSize = size;
	// ������ ���������� ����(�ı��� ȣ��� �޸� ������ ���� �ʰ���)
	bReferedData = TRUE;
	// ��ȣȭ �Ǿ��ִٸ� ���ڵ���
	if(pHead->Dummy[1] == 2) 
		if(!DecodeAll(pData)) return NULL;
	return (pData + PACKET_HEADERSIZE);
}


char* CCommMsg::SmartCpy(char **tar, const void *src, int size)
{
	char *org = (*tar);
	memcpy((*tar), src, size);
	*tar += size;
	return org;
}

char* CCommMsg::Jump(char **tar, int size)
{
	char *org = (*tar);
	*tar += size;
	return org;
}


////////////////////////////////////////////////////////////////////////////
//                      ������ �޼��� Ŭ����
////////////////////////////////////////////////////////////////////////////

////////// ������ üũ��
BOOL CSV_CHECKVERSION::Set(int ver, int prversion)
{
	int msglen = sizeof(*Ver)+sizeof(*PreVersion);
	char* ptemp = SetHeader(SV_CHECKVERSION, msglen);
	if(ptemp==NULL) {TRACE("CSV_CHECKVERSION::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	Ver = (int*)SmartCpy(&ptemp, &ver, sizeof(*Ver));
	PreVersion = (int*)SmartCpy(&ptemp, &prversion, sizeof(*PreVersion));
	return TRUE;
}
BOOL CSV_CHECKVERSION::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_CHECKVERSION::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	Ver = (int*)Jump(&ptemp, sizeof(*Ver));
	PreVersion = (int*)Jump(&ptemp, sizeof(*PreVersion));
	return TRUE;
}


////////// ���� ����	[���� ������ �۾�]
BOOL CSV_SERVERINFO::Set(SERVERINFO *pSI)
{
	int msglen = sizeof(SERVERINFO);
	char* ptemp = SetHeader(SV_SERVERINFO, msglen);
	if(ptemp==NULL) {TRACE("CSV_SERVERINFO::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	SI = (SERVERINFO*)SmartCpy(&ptemp, pSI, sizeof(SERVERINFO));
	//Encode();
	return TRUE;
}
BOOL CSV_SERVERINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_SERVERINFO::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	SI = (SERVERINFO*)Jump(&ptemp, sizeof(SERVERINFO));
	return TRUE;
}



/*
//////// �α����� ��û��
BOOL CSV_LOGIN::Set(char *id, char *pass, BOOL bOver, char *socknameip, char *hostnameip, int port)
{
	char strid[16]={0,};
	char strpass[10]={0,};
	char strSockNameIp[20]={0,};
	char strHostNameIp[20]={0,};

	int  msglen = 16 + 10 + sizeof(*Over) + 20 + 20 + sizeof(*Port);
	char* ptemp = SetHeader(SV_LOGIN, msglen);
	if(ptemp==NULL) {TRACE("CSV_LOGIN::Set() �޸� �Ҵ� ����\n"); return FALSE;}

	if(strlen(id)<16) strcpy(strid, id);
	if(strlen(pass)<10) strcpy(strpass, pass);
	strncpy(strSockNameIp, socknameip, 19);
	strncpy(strHostNameIp, hostnameip, 19);

	ID   = SmartCpy(&ptemp, strid, 16);
	Pass = SmartCpy(&ptemp, strpass, 10);
	Over  = (BOOL*)SmartCpy(&ptemp, &bOver, sizeof(*Over));
	SockNameIP = SmartCpy(&ptemp, strSockNameIp, 20);
	HostNameIP = SmartCpy(&ptemp, strHostNameIp, 20);
	Port = (int*)SmartCpy(&ptemp, &port, sizeof(*Port));
	Encode();
	return TRUE;
}
BOOL CSV_LOGIN::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_LOGIN::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	ID = Jump(&ptemp, 16);
	Pass = Jump(&ptemp, 10);
	Over = (BOOL*)Jump(&ptemp, sizeof(*Over));
	SockNameIP = Jump(&ptemp, 20);
	HostNameIP = Jump(&ptemp, 20);
	Port = (int*)Jump(&ptemp, sizeof(*Port));
	return TRUE;
}
*/

//2004.05.07 �������Ѿ� ����
BOOL CSV_LEADERSJOIN::Set(LEADERS_JOIN *pLJ)
{
	int msglen = sizeof(LEADERS_JOIN);
	char* ptemp = SetHeader(SV_LEADERSJOIN, msglen);
	if(ptemp==NULL) {TRACE("CSV_LEADERSJOIN::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	memcpy(ptemp, pLJ, msglen);
	LJ = (LEADERS_JOIN*)ptemp;
	return TRUE;
}
BOOL CSV_LEADERSJOIN::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_LEADERSJOIN::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	LJ = (LEADERS_JOIN*)ptemp;
	return TRUE;
}


// [SSO �۾�] - �߰�
////////// �α����� ��û��
BOOL CSV_ASK_LOGIN::Set(ASK_LOGININFO *pal, char* szAuthCookie, char* szDataCookie)
{
	int msglen = sizeof(ASK_LOGININFO) + pal->AuthCookieLen +  pal->DataCookieLen;
	char* ptemp = SetHeader(SV_ASK_LOGIN, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_LOGIN::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	pAL = (ASK_LOGININFO*)SmartCpy(&ptemp, pal, sizeof(ASK_LOGININFO));
	if(pAL->AuthCookieLen>0) AuthCookie = (char*)SmartCpy(&ptemp, szAuthCookie, pal->AuthCookieLen);
	if(pAL->DataCookieLen>0) DataCookie = (char*)SmartCpy(&ptemp, szDataCookie, pal->DataCookieLen);
	//Encode();
	return TRUE;
}
BOOL CSV_ASK_LOGIN::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_LOGIN::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	pAL = (ASK_LOGININFO*)Jump(&ptemp, sizeof(ASK_LOGININFO));

	if(pAL->AuthCookieLen>0) AuthCookie = (char*)Jump(&ptemp, pAL->AuthCookieLen);
	else AuthCookie = NULL;
	if(pAL->DataCookieLen>0) DataCookie = (char*)Jump(&ptemp, pAL->DataCookieLen);
	else DataCookie = NULL;

	return TRUE;
}

////////// �α����� �ź���	[SSO �۾�] - ��ü
BOOL CSV_REFUSE_LOGIN::Set(int code, char* strmsg)
{
	int l_strmsg = 0;
	if(strmsg) l_strmsg = strlen(strmsg);

	int msglen = sizeof(*Code) + sizeof(*l_StrMsg) + l_strmsg;
	char* ptemp = SetHeader(SV_REFUSE_LOGIN, msglen);
	if(ptemp==NULL) {TRACE("CSV_REFUSE_LOGIN::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	Code = (int*)SmartCpy(&ptemp, &code, sizeof(*Code));
	l_StrMsg = (int*)SmartCpy(&ptemp, &l_strmsg, sizeof(*l_StrMsg));
	if(l_strmsg > 0) StrMsg = (char*)SmartCpy(&ptemp, strmsg, l_strmsg);
	return TRUE;
}
BOOL CSV_REFUSE_LOGIN::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_REFUSE_LOGIN::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	Code = (int*)Jump(&ptemp, sizeof(*Code));
	l_StrMsg = (int*)Jump(&ptemp, sizeof(*l_StrMsg));
	if(*l_StrMsg > 0) StrMsg = (char*)Jump(&ptemp, *l_StrMsg);
	else StrMsg = NULL;
	return TRUE;
}

////////// �α����� �����
BOOL CSV_ACCEPT_LOGIN::Set(LOGININFO *pLI)
{
	int msglen = sizeof(LOGININFO);
	char* ptemp = SetHeader(SV_ACCEPT_LOGIN, msglen);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_LOGIN::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	LI = (LOGININFO*)SmartCpy(&ptemp, pLI, sizeof(LOGININFO));
	//Encode();
	return TRUE;
}
BOOL CSV_ACCEPT_LOGIN::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_LOGIN::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	LI = (LOGININFO*)Jump(&ptemp, sizeof(LOGININFO));
	return TRUE;
}
/*
////////// �α����� �ź���
BOOL CSV_REFUSE_LOGIN::Set(int code)
{
	int msglen = sizeof(*Code);
	char* ptemp = SetHeader(SV_REFUSE_LOGIN, msglen);
	if(ptemp==NULL) {TRACE("CSV_REFUSE_LOGIN::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	memcpy(ptemp, &code, sizeof(*Code));
	Code = (int*)ptemp;
	return TRUE;
}
BOOL CSV_REFUSE_LOGIN::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_REFUSE_LOGIN::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	Code = (int*)ptemp;
	return TRUE;
}
*/
////////// �α׾ƿ��� ��û
BOOL CSV_ASK_LOGOUT::Set(int unum, char *id)
{
	char strId[16]={0,};
	if(strlen(id) < 16) strcpy(strId, id);
	int msglen = sizeof(*UNum) + 16;
	char* ptemp = SetHeader(SV_ASK_LOGOUT, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_LOGOUT::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ID = SmartCpy(&ptemp, strId, 16);
	return TRUE;
}
BOOL CSV_ASK_LOGOUT::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_LOGOUT::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ID = ptemp;
	return TRUE;
}

////////// �α׾ƿ��� ���
BOOL CSV_ACCEPT_LOGOUT::Set()
{
	int msglen = 0;
	char* ptemp = SetHeader(SV_ACCEPT_LOGOUT, msglen);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_LOGOUT::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	return TRUE;
}
BOOL CSV_ACCEPT_LOGOUT::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_LOGOUT::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	return TRUE;
}

////////// ��Ʈ��Ʈ
BOOL CSV_HEARTBEAT::Set()
{
	int msglen = 0;
	char* ptemp = SetHeader(SV_HEARTBEAT, msglen);
	if(ptemp==NULL) {TRACE("CSV_HEARTBEAT::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	return TRUE;
}
BOOL CSV_HEARTBEAT::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_HEARTBEAT::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	return TRUE;
}

////////// ä�� ������ ��û
BOOL CSV_ASK_ENTERCHAN::Set(int cnum)
{
	int msglen = sizeof(*ChNum);
	char* ptemp = SetHeader(SV_ASK_ENTERCHAN, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_ENTERCHAN::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	ChNum = (int*)SmartCpy(&ptemp, &cnum, sizeof(*ChNum));
	return TRUE;
}
BOOL CSV_ASK_ENTERCHAN::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_ENTERCHAN::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	ChNum = (int*)Jump(&ptemp, sizeof(*ChNum));
	return TRUE;
}

////////// ä�� ������ �㰡
BOOL CSV_ACCEPT_ENTERCHAN::Set(ENTERCHANINFO *pEC)
{
	int msglen = sizeof(ENTERCHANINFO);
	char* ptemp = SetHeader(SV_ACCEPT_ENTERCHAN, msglen);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_ENTERCHAN::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	EC = (ENTERCHANINFO*)SmartCpy(&ptemp, pEC, sizeof(ENTERCHANINFO));
	return TRUE;
}
BOOL CSV_ACCEPT_ENTERCHAN::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_ENTERCHAN::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	EC = (ENTERCHANINFO*)Jump(&ptemp, sizeof(ENTERCHANINFO));
	return TRUE;
}

////////// ä�� ������ ��û
BOOL CSV_ASK_OUTCHAN::Set()
{
	int msglen = 0;
	char* ptemp = SetHeader(SV_ASK_OUTCHAN, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_OUTCHAN::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	return TRUE;
}
BOOL CSV_ASK_OUTCHAN::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_OUTCHAN::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	return TRUE;
}

////////// ä�� ������ �㰡
BOOL CSV_ACCEPT_OUTCHAN::Set()
{
	int msglen = 0;
	char* ptemp = SetHeader(SV_ACCEPT_OUTCHAN, msglen);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_OUTCHAN::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	return TRUE;
}
BOOL CSV_ACCEPT_OUTCHAN::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_OUTCHAN::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	return TRUE;
}

////////// ���̵� ã�� ��û
BOOL CSV_ASK_FINDID::Set(char *id)
{
	char strId[16]={0,};
	if(strlen(id) < 16) strcpy(strId, id);
	int msglen = 16;
	char* ptemp = SetHeader(SV_ASK_FINDID, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_FINDID::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	ID = SmartCpy(&ptemp, strId, 16);
	return TRUE;
}
BOOL CSV_ASK_FINDID::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_FINDID::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	ID = Jump(&ptemp, 16);
	return TRUE;
}

////////// ���̵� ã�� ���
BOOL CSV_RESULT_FINDID::Set(RESULTFINDID *pRF)
{
	int msglen = sizeof(RESULTFINDID);
	char* ptemp = SetHeader(SV_RESULT_FINDID, msglen);
	if(ptemp==NULL) {TRACE("CSV_RESULT_FINDID::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RF = (RESULTFINDID*)SmartCpy(&ptemp, pRF, sizeof(RESULTFINDID));
	return TRUE;
}
BOOL CSV_RESULT_FINDID::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_RESULT_FINDID::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RF = (RESULTFINDID*)Jump(&ptemp, sizeof(RESULTFINDID));
	return TRUE;
}

////////// ��ȭ���� �Ű� ��û
BOOL CSV_ASK_BADUSER::Set(BOOL announce, int rnum, int unum, char* id, char *tarid, char *reason, char *watchid, char *chat)
{
	char strId[16]={0,};
	char strTargetId[16]={0,};
	char strReason[64]={0,};
	CString szWatchID, szChat;
	szWatchID.Format("%s", watchid);
	szChat.Format("%s", chat);
	int l_strwatchid = szWatchID.GetLength();
	int l_strchat = szChat.GetLength();
	int msglen = sizeof(*bAnnounce) + sizeof(*RNum) + sizeof(*UNum) + 16 + 16 + 64 + sizeof(l_strwatchid) + l_strwatchid + sizeof(l_strchat) + l_strchat;

	char* ptemp = SetHeader(SV_ASK_BADUSER, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_BADUSER::Set() �޸� �Ҵ� ����\n"); return FALSE;}

	if(strlen(id) < 16) strcpy(strId, id);
	if(strlen(tarid) < 16) strcpy(strTargetId, tarid);
	if(strlen(reason) < 64) strcpy(strReason, reason);
	
	bAnnounce = (BOOL*)SmartCpy(&ptemp, &announce, sizeof(*bAnnounce));
	RNum      = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RNum));
	UNum      = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ID        = SmartCpy(&ptemp, strId, 16);
	TargetID  = SmartCpy(&ptemp, strTargetId, 16);
	SubmitReason = SmartCpy(&ptemp, strReason, 64);
	l_StrWatchID = (int*)SmartCpy(&ptemp, &l_strwatchid, sizeof(l_strwatchid));
	WatchID   = SmartCpy(&ptemp, (void*)szWatchID.operator LPCTSTR(), l_strwatchid);
	l_StrChat = (int*)SmartCpy(&ptemp, &l_strchat, sizeof(l_strchat));
	StrChat   = SmartCpy(&ptemp, (void*)szChat.operator LPCTSTR(), l_strchat);
	return TRUE;
}
BOOL CSV_ASK_BADUSER::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_BADUSER::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	bAnnounce = (int*)Jump(&ptemp, sizeof(*bAnnounce));
	RNum      = (int*)Jump(&ptemp, sizeof(*RNum));
	UNum      = (int*)Jump(&ptemp, sizeof(*UNum));
	ID        = Jump(&ptemp, 16);
	TargetID  = Jump(&ptemp, 16);
	SubmitReason = Jump(&ptemp, 64);
	l_StrWatchID = (int*)Jump(&ptemp, sizeof(*l_StrWatchID));
	WatchID = Jump(&ptemp, *l_StrWatchID);
	l_StrChat = (int*)Jump(&ptemp, sizeof(*l_StrChat));
	StrChat = ptemp;
	return TRUE;
}

////////// ��ü ä�� ����� �䱸
BOOL CSV_ASK_ALLCHANINFO::Set()
{
	char* ptemp = SetHeader(SV_ASK_ALLCHANINFO, 0);
	if(ptemp==NULL) {TRACE("CSV_ASK_ALLCHANINFO::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	return TRUE;
}
BOOL CSV_ASK_ALLCHANINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_ALLCHANINFO::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	return TRUE;
}

////////// ��ü ä�� ����� ������
BOOL CSV_ALLCHANINFO::Set(int totnum, CHANNELINFO *ppCI)
{
	int msglen = sizeof(*TotNum) + sizeof(CHANNELINFO)*totnum;
	char* ptemp = SetHeader(SV_ALLCHANINFO, msglen);
	if(ptemp==NULL) {TRACE("CSV_ALLCHANINFO::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	TotNum = (int*)SmartCpy(&ptemp, &totnum, sizeof(*TotNum));
	ArrayCI = (CHANNELINFO*)SmartCpy(&ptemp, ppCI, sizeof(CHANNELINFO)*totnum);
	return TRUE;
}
BOOL CSV_ALLCHANINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ALLCHANINFO::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	TotNum   = (int*)Jump(&ptemp, sizeof(*TotNum));
	ArrayCI = (CHANNELINFO*)ptemp;
	return TRUE;
}

////////// ��ü ����� ����� �䱸
BOOL CSV_ASK_ALLUSERINFO::Set()
{
	char* ptemp = SetHeader(SV_ASK_ALLUSERINFO, 0);
	if(ptemp==NULL) {TRACE("CSV_ASK_ALLUSERINFO::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	return TRUE;
}
BOOL CSV_ASK_ALLUSERINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_ALLUSERINFO::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	return TRUE;
}

////////// ��ü ����� ����� ������
BOOL CSV_ALLUSERINFO::Set(int totnum, SMALLUSERINFO *ppSUI)
{
	int msglen = sizeof(*TotNum) + sizeof(SMALLUSERINFO)*totnum;
	char* ptemp = SetHeader(SV_ALLUSERINFO, msglen);
	if(ptemp==NULL) {TRACE("CSV_ALLUSERINFO::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	TotNum = (int*)SmartCpy(&ptemp, &totnum, sizeof(*TotNum));
	ArraySUI = (SMALLUSERINFO*)SmartCpy(&ptemp, ppSUI, sizeof(SMALLUSERINFO)*totnum);
	return TRUE;
}
BOOL CSV_ALLUSERINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ALLUSERINFO::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	TotNum   = (int*)Jump(&ptemp, sizeof(*TotNum));
	ArraySUI = (SMALLUSERINFO*)ptemp;
	return TRUE;
}

////////// Ư�� ������� �� ������ �䱸
BOOL CSV_ASK_DETAILUSERINFO::Set(int unum, char *id)
{
	char strId[16]={0,};
	if(strlen(id) < 16) strcpy(strId, id);
	int msglen = sizeof(*UNum) + 16;
	char* ptemp = SetHeader(SV_ASK_DETAILUSERINFO, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_DETAILUSERINFO::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ID = SmartCpy(&ptemp, strId, 16);
	return TRUE;
}
BOOL CSV_ASK_DETAILUSERINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_DETAILUSERINFO::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ID = ptemp;
	return TRUE;
}

////////// Ư�� ������� �� ������ ������
BOOL CSV_DETAILUSERINFO::Set(USERINFO *pUI)
{
	int msglen = sizeof(USERINFO);
	char* ptemp = SetHeader(SV_DETAILUSERINFO, msglen);
	if(ptemp==NULL) {TRACE("CSV_DETAILUSERINFO::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	memcpy(ptemp, pUI, msglen);
	UI = (USERINFO*)ptemp;
	//Encode();
	return TRUE;
}
BOOL CSV_DETAILUSERINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_DETAILUSERINFO::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	UI = (USERINFO*)ptemp;
	return TRUE;
}

////////// Ư�� ������� ������ �䱸
BOOL CSV_ASK_USERINFO::Set(int unum, char *id)
{
	char strId[16]={0,};
	if(strlen(id) < 16) strcpy(strId, id);
	int msglen = sizeof(*UNum) + 16;
	char* ptemp = SetHeader(SV_ASK_USERINFO, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_USERINFO::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ID = SmartCpy(&ptemp, strId, 16);
	return TRUE;
}
BOOL CSV_ASK_USERINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_USERINFO::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ID = ptemp;
	return TRUE;
}

////////// Ư�� ������� ������ ������
BOOL CSV_USERINFO::Set(USERINFO *pUI)
{
	int msglen = sizeof(USERINFO);
	char* ptemp = SetHeader(SV_USERINFO, msglen);
	if(ptemp==NULL) {TRACE("CSV_USERINFO::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	memcpy(ptemp, pUI, msglen);
	UI = (USERINFO*)ptemp;
	//Encode();
	return TRUE;
}
BOOL CSV_USERINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_USERINFO::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	UI = (USERINFO*)ptemp;
	return TRUE;
}

////////// ���ο� ����ڰ� �����Ͽ���
BOOL CSV_NEWUSER::Set(SMALLUSERINFO *pSUI)
{
	int msglen = sizeof(SMALLUSERINFO);
	char* ptemp = SetHeader(SV_NEWUSER, msglen);
	if(ptemp==NULL) {TRACE("CSV_NEWUSER::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	SUI = (SMALLUSERINFO*)SmartCpy(&ptemp, pSUI, sizeof(SMALLUSERINFO));
	return TRUE;
}
BOOL CSV_NEWUSER::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_NEWUSER::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	SUI = (SMALLUSERINFO*)ptemp;
	return TRUE;
}

////////// ����ڰ� ������ �����Ͽ���
BOOL CSV_OUTUSER::Set(SMALLUSERINFO *pSUI)
{
	int msglen = sizeof(SMALLUSERINFO);
	char* ptemp = SetHeader(SV_OUTUSER, msglen);
	if(ptemp==NULL) {TRACE("CSV_OUTUSER::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	SUI = (SMALLUSERINFO*)SmartCpy(&ptemp, pSUI, sizeof(SMALLUSERINFO));
	return TRUE;
}
BOOL CSV_OUTUSER::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_OUTUSER::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	SUI = (SMALLUSERINFO*)ptemp;
	return TRUE;
}

////////// �ٸ� ������� ���������� ������
BOOL CSV_CHANGEUSER::Set(SMALLUSERINFO *pSUI)
{
	int msglen = sizeof(SMALLUSERINFO);
	char* ptemp = SetHeader(SV_CHANGEUSER, msglen);
	if(ptemp==NULL) {TRACE("CSV_CHANGEUSER::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	SUI = (SMALLUSERINFO*)SmartCpy(&ptemp, pSUI, sizeof(SMALLUSERINFO));
	return TRUE;
}
BOOL CSV_CHANGEUSER::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_CHANGEUSER::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	SUI = (SMALLUSERINFO*)ptemp;
	return TRUE;
}


////////// ��ü �� ����� �䱸
BOOL CSV_ASK_ALLROOMINFO::Set()
{
	char* ptemp = SetHeader(SV_ASK_ALLROOMINFO, 0);
	if(ptemp==NULL) {TRACE("CSV_ASK_ALLROOMINFO::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	return TRUE;
}
BOOL CSV_ASK_ALLROOMINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_ALLROOMINFO::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	return TRUE;
}

////////// ��ü �� ����� ������
BOOL CSV_ALLROOMINFO::Set(int totnum, ROOMINFO *ppRI)
{
	int msglen = sizeof(*TotNum) + sizeof(ROOMINFO)*totnum;
	char* ptemp = SetHeader(SV_ALLROOMINFO, msglen);
	if(ptemp==NULL) {TRACE("CSV_ALLROOMINFO::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	TotNum = (int*)SmartCpy(&ptemp, &totnum, sizeof(*TotNum));
	ArrayRI = (ROOMINFO*)SmartCpy(&ptemp, ppRI, sizeof(ROOMINFO)*totnum);
	return TRUE;
}
BOOL CSV_ALLROOMINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ALLROOMINFO::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	TotNum   = (int*)Jump(&ptemp, sizeof(*TotNum));
	ArrayRI = (ROOMINFO*)ptemp;
	return TRUE;
}

////////// �� ������ �䱸
BOOL CSV_ASK_ROOMINFO::Set(int roomnum)
{
	int msglen = sizeof(*RoomNum);
	char* ptemp = SetHeader(SV_ASK_ROOMINFO, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_ROOMINFO::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &roomnum, sizeof(*RoomNum));
	return TRUE;
}
BOOL CSV_ASK_ROOMINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_ROOMINFO::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)ptemp;
	return TRUE;
}

////////// ������
BOOL CSV_ROOMINFO::Set(int errcode, GAMEINFO *pGI)
{
	int msglen = sizeof(*ErrCode) + sizeof(GAMEINFO);
	char* ptemp = SetHeader(SV_ROOMINFO, msglen);
	if(ptemp==NULL) {TRACE("CSV_ROOMINFO::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	ErrCode = (int*)SmartCpy(&ptemp, &errcode, sizeof(*ErrCode));
	GI = (GAMEINFO*)SmartCpy(&ptemp, pGI, sizeof(GAMEINFO));
	//Encode();
	return TRUE;
}
BOOL CSV_ROOMINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ROOMINFO::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	ErrCode = (int*)Jump(&ptemp, sizeof(*ErrCode));
	GI = (GAMEINFO*)ptemp;
	return TRUE;
}

////////// ĳ���� �ٲٱ⸦ �䱸
BOOL CSV_ASK_CHANGECHAR::Set(int unum, char *id, int charnum)
{
	char strID[16];
	strncpy(strID, id, 15);
	int msglen = sizeof(*UNum) + 16 + sizeof(*CharNum);
	char* ptemp = SetHeader(SV_ASK_CHANGECHAR, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_CHANGECHAR::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	UNum = (int*) SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ID = SmartCpy(&ptemp, strID, 16);
	CharNum = (int*) SmartCpy(&ptemp, &charnum, sizeof(*CharNum));
	return TRUE;
}
BOOL CSV_ASK_CHANGECHAR::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_CHANGECHAR::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ID = Jump(&ptemp, 16);
	CharNum = (int*)Jump(&ptemp, sizeof(*CharNum));
	return TRUE;
}

////////// ĳ���� �ٲٱ⸦ ���
BOOL CSV_ACCEPT_CHANGECHAR::Set(int charnum)
{
	int msglen = sizeof(*CharNum);
	char* ptemp = SetHeader(SV_ACCEPT_CHANGECHAR, msglen);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_CHANGECHAR::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	CharNum = (int*) SmartCpy(&ptemp, &charnum, sizeof(*CharNum));
	return TRUE;
}
BOOL CSV_ACCEPT_CHANGECHAR::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_CHANGECHAR::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	CharNum = (int*)Jump(&ptemp, sizeof(*CharNum));
	return TRUE;
}

////////// �� ���� �䱸
BOOL CSV_ASK_CREATEROOM::Set(ROOMINFO *pRI, UINT port)
{
	int msglen = sizeof(ROOMINFO) + sizeof(*Port);
	char* ptemp = SetHeader(SV_ASK_CREATEROOM, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_CREATEROOM::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RI = (ROOMINFO*)SmartCpy(&ptemp, pRI, sizeof(ROOMINFO));
	Port = (UINT*)SmartCpy(&ptemp, &port, sizeof(*Port));
	return TRUE;
}
BOOL CSV_ASK_CREATEROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_CREATEROOM::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RI = (ROOMINFO*)Jump(&ptemp, sizeof(ROOMINFO));
	Port = (UINT*)Jump(&ptemp, sizeof(*Port));
	return TRUE;
}

////////// �� ���� �ź�
BOOL CSV_REFUSE_CREATEROOM::Set(int code)
{
	int msglen = sizeof(*Code);
	char* ptemp = SetHeader(SV_REFUSE_CREATEROOM, msglen);
	if(ptemp==NULL) {TRACE("CSV_REFUSE_CREATEROOM::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	memcpy(ptemp, &code, sizeof(*Code));
	return TRUE;
}
BOOL CSV_REFUSE_CREATEROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_REFUSE_CREATEROOM::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	Code = (int*)ptemp;
	return TRUE;
}

////////// �� ���� ���
BOOL CSV_ACCEPT_CREATEROOM::Set(ROOMINFO *pRI)
{
	int msglen = sizeof(ROOMINFO);
	char* ptemp = SetHeader(SV_ACCEPT_CREATEROOM, msglen);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_CREATEROOM::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	memcpy(ptemp, pRI, msglen);
	RI = (ROOMINFO*)ptemp;
	return TRUE;
}
BOOL CSV_ACCEPT_CREATEROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_CREATEROOM::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RI = (ROOMINFO*)ptemp;
	return TRUE;
}

////////// ���ο� ���� �����Ǿ���
BOOL CSV_NEWROOM::Set(ROOMINFO *pRI)
{
	int msglen = sizeof(ROOMINFO);
	char* ptemp = SetHeader(SV_NEWROOM, msglen);
	if(ptemp==NULL) {TRACE("CSV_NEWROOM::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	memcpy(ptemp, pRI, msglen);
	RI = (ROOMINFO*)ptemp;
	return TRUE;
}
BOOL CSV_NEWROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_NEWROOM::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RI = (ROOMINFO*)ptemp;
	return TRUE;
}

////////// ���� �������
BOOL CSV_OUTROOM::Set(SMALLROOMINFO *pSRI)
{
	int msglen = sizeof(SMALLROOMINFO);
	char* ptemp = SetHeader(SV_OUTROOM, msglen);
	if(ptemp==NULL) {TRACE("CSV_OUTROOM::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	memcpy(ptemp, pSRI, msglen);
	SRI = (SMALLROOMINFO*)ptemp;
	return TRUE;
}
BOOL CSV_OUTROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_OUTROOM::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	SRI = (SMALLROOMINFO*)ptemp;
	return TRUE;
}

////////// �� ������ ������
BOOL CSV_CHANGEROOM::Set(SMALLROOMINFO *pSRI)
{
	int msglen = sizeof(SMALLROOMINFO);
	char* ptemp = SetHeader(SV_CHANGEROOM, msglen);
	if(ptemp==NULL) {TRACE("CSV_CHANGEROOM::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	memcpy(ptemp, pSRI, msglen);
	SRI = (SMALLROOMINFO*)ptemp;
	return TRUE;
}
BOOL CSV_CHANGEROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_CHANGEROOM::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	SRI = (SMALLROOMINFO*)ptemp;
	return TRUE;
}

////////// ������ �ٲ����
BOOL CSV_CHANGEROOMMASTER::Set(int rnum, int unum, char *id)
{
	char strid[16]={0,};
	if(id != NULL) strncpy(strid, id, 15);
	int msglen = sizeof(*RoomNum) + sizeof(*UNum) + 16;
	char* ptemp = SetHeader(SV_CHANGEROOMMASTER, msglen);
	if(ptemp==NULL) {TRACE("CSV_CHANGEROOMMASTER::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ID = SmartCpy(&ptemp, strid, 16);
	return TRUE;
}
BOOL CSV_CHANGEROOMMASTER::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_CHANGEROOMMASTER::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ID = Jump(&ptemp, 16);
	return TRUE;
}

////////// Ư�� �濡 ������ �䱸
BOOL CSV_ASK_ENTERROOM::Set(int rnum, int unum, int sndKind, char *id, char *pass)// ### [���� �߰� �۾�] ###
{
	int msglen = sizeof(*RoomNum) + sizeof(*UNum) + sizeof(*SndFxKind)+ 16 + 5;
	char strid[16]={0,};
	char strpass[5]={0,};
	if(id != NULL) strncpy(strid, id, 15);
	if(pass != NULL) strncpy(strpass, pass, 4);
	char* ptemp = SetHeader(SV_ASK_ENTERROOM, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_ENTERROOM::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	SndFxKind = (int*)SmartCpy(&ptemp, &sndKind, sizeof(*SndFxKind)); // �߰�
	ID = SmartCpy(&ptemp, strid, 16);
	Pass = SmartCpy(&ptemp, strpass, 5);
	return TRUE;
}
BOOL CSV_ASK_ENTERROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_ENTERROOM::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	SndFxKind = (int*)Jump(&ptemp, sizeof(*SndFxKind));// ### [���� �߰� �۾�] ###
	ID = Jump(&ptemp, 16);
	Pass = Jump(&ptemp, 5);
	return TRUE;
}

////////// �濡 �����ϱ⸦ �㰡��
BOOL CSV_ACCEPT_ENTERROOM::Set(ENTERROOMINFO *pEI)//, int fieldnum, BLOCKFIELDINFO *pbf) 
{
	int msglen = sizeof(ENTERROOMINFO);// + sizeof(*FieldNum) + sizeof(BLOCKFIELDINFO)*fieldnum;
	char* ptemp = SetHeader(SV_ACCEPT_ENTERROOM, msglen);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_ENTERROOM::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	EI = (ENTERROOMINFO*)SmartCpy(&ptemp, pEI, sizeof(ENTERROOMINFO));
	//FieldNum = (int*)SmartCpy(&ptemp, &fieldnum, sizeof(*FieldNum));
	//pBF = (BLOCKFIELDINFO*)SmartCpy(&ptemp, pbf, sizeof(BLOCKFIELDINFO)*fieldnum);
	//Encode();
	return TRUE;
}
BOOL CSV_ACCEPT_ENTERROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_ENTERROOM::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	EI = (ENTERROOMINFO*)Jump(&ptemp, sizeof(ENTERROOMINFO));
	//FieldNum = (int*)Jump(&ptemp, sizeof(*FieldNum));
	//if(*FieldNum > 0)
	//	pBF = (BLOCKFIELDINFO*)Jump(&ptemp, sizeof(BLOCKFIELDINFO)*(*FieldNum));
	//else 
	//	pBF = NULL;
	return TRUE;
}

////////// �����û�� �ź���
BOOL CSV_REFUSE_ENTERROOM::Set(int code) 
{
	int msglen = sizeof(*Code);
	char* ptemp = SetHeader(SV_REFUSE_ENTERROOM, msglen);
	if(ptemp==NULL) {TRACE("CSV_REFUSE_ENTERROOM::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	Code = (int*)SmartCpy(&ptemp, &code, sizeof(*Code));
	return TRUE;
}
BOOL CSV_REFUSE_ENTERROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_REFUSE_ENTERROOM::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	Code = (int*)ptemp;
	return TRUE;
}

// ### [ �ߺ��� IP�� ] ###
////////// �濡 ����ڰ� �����Ͽ���
BOOL CSV_USERENTERROOM::Set(int rnum, USERINFO *pUI, IPARRINDEX *pIPA, int sndKind)// ### [���� �߰� �۾�] ###)
{
	int msglen = sizeof(*RoomNum) + sizeof(USERINFO) + sizeof(IPARRINDEX) + sizeof(*SndFxKind); // �߰�
	char* ptemp = SetHeader(SV_USERENTERROOM, msglen);
	if(ptemp==NULL) {TRACE("CSV_USERENTERROOM::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	UI = (USERINFO*)SmartCpy(&ptemp, pUI, sizeof(USERINFO));
	IPA = (IPARRINDEX*)SmartCpy(&ptemp, pIPA, sizeof(IPARRINDEX));
	SndFxKind = (int*)SmartCpy(&ptemp, &sndKind, sizeof(*SndFxKind)); // �߰�
	//Encode();
	return TRUE;
}
BOOL CSV_USERENTERROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_USERENTERROOM::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UI = (USERINFO*)Jump(&ptemp, sizeof(USERINFO));
	IPA = (IPARRINDEX*)Jump(&ptemp, sizeof(IPARRINDEX));
	SndFxKind = (int*)Jump(&ptemp, sizeof(*SndFxKind));// ### [���� �߰� �۾�] ###
	return TRUE;
}

// ### [ �ߺ��� IP�� ] ###
////////// ����ڰ� ����Ͽ���
BOOL CSV_USEROUTROOM::Set(int rnum, int unum, char *id, IPARRINDEX *pIPA)
{
	char strid[16]={0,};
	if(id != NULL) strncpy(strid, id, 15);
	int msglen = sizeof(*RoomNum) + sizeof(*UNum) + 16 +sizeof(IPARRINDEX);
	char* ptemp = SetHeader(SV_USEROUTROOM, msglen);
	if(ptemp==NULL) {TRACE("CSV_USEROUTROOM::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ID = SmartCpy(&ptemp, strid, 16);
	IPA = (IPARRINDEX*)SmartCpy(&ptemp, pIPA, sizeof(IPARRINDEX));
	return TRUE;
}
BOOL CSV_USEROUTROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_USEROUTROOM::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ID = Jump(&ptemp, 16);
	IPA = (IPARRINDEX*)Jump(&ptemp, sizeof(IPARRINDEX));
	return TRUE;
}

////////// ��ǿ�û
BOOL CSV_ASK_OUTROOM::Set(int rnum, int unum, char *id)
{
	char strid[16]={0,};
	if(id != NULL) strncpy(strid, id, 15);
	int msglen = sizeof(*RoomNum) + sizeof(*UNum) + 16;
	char* ptemp = SetHeader(SV_ASK_OUTROOM, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_OUTROOM::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ID = SmartCpy(&ptemp, strid, 16);
	return TRUE;
}
BOOL CSV_ASK_OUTROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_OUTROOM::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ID = Jump(&ptemp, 16);
	return TRUE;
}

////////// ��ǿ�û �㰡
BOOL CSV_ACCEPT_OUTROOM::Set()
{
	int msglen = 0;
	char* ptemp = SetHeader(SV_ACCEPT_OUTROOM, msglen);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_OUTROOM::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	return TRUE;
}
BOOL CSV_ACCEPT_OUTROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_OUTROOM::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	return TRUE;
}

////////// ���� ����� ������ �����
BOOL CSV_CHANGEMYINFO::Set(CHANGEMYINFO *pcmi)
{
	int msglen = sizeof(CHANGEMYINFO);
	char* ptemp = SetHeader(SV_CHANGEMYINFO, msglen);
	if(ptemp==NULL) {TRACE("CSV_CHANGEMYINFO::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	pCMI = (CHANGEMYINFO*)SmartCpy(&ptemp, pcmi, sizeof(CHANGEMYINFO));
	return TRUE;
}
BOOL CSV_CHANGEMYINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_CHANGEMYINFO::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	pCMI = (CHANGEMYINFO*)Jump(&ptemp, sizeof(CHANGEMYINFO));
	return TRUE;
}

////////// ���� ���� �ʴ�â ���� ���� �˸�
BOOL CSV_LOBYOPEN::Set(BOOL blobyopen)
{
	int msglen = sizeof(*bLobyOpen);
	char* ptemp = SetHeader(SV_LOBYOPEN, msglen);
	if(ptemp==NULL) {TRACE("CSV_LOBYOPEN::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	bLobyOpen = (BOOL*)SmartCpy(&ptemp, &blobyopen, sizeof(*bLobyOpen));
	return TRUE;
}
BOOL CSV_LOBYOPEN::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_LOBYOPEN::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	bLobyOpen = (BOOL*)Jump(&ptemp, sizeof(*bLobyOpen));
	return TRUE;
}

////////// ��û �޼����� ����
BOOL CSV_ASK_INVITE::Set(int targetunum, char* targetid, int rnum, char *pass, USERINFO *pMyUI, char *strmsg)
{
	char strTarId[16]={0,};
	if(strlen(targetid) < 16) strcpy(strTarId, targetid);
	char strPass[5]={0,};
	strncpy(strPass, pass, 4);
	CString szMsg;
	szMsg.Format("%s", strmsg);
	int l_strmsg = szMsg.GetLength();

	int msglen = sizeof(*TarUNum) + 16 + sizeof(*RoomNum) + 5 + sizeof(USERINFO) + sizeof(*l_StrMsg) + l_strmsg;
	char* ptemp = SetHeader(SV_ASK_INVITE, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_INVITE::Set() �޸� �Ҵ� ����\n"); return FALSE;}

	TarUNum = (int*)SmartCpy(&ptemp, &targetunum, sizeof(*TarUNum));
	TarID = SmartCpy(&ptemp, strTarId, 16);
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	RoomPass = SmartCpy(&ptemp, strPass, 5);
	MyUI = (USERINFO*)SmartCpy(&ptemp, pMyUI, sizeof(USERINFO));
	l_StrMsg = (int*)SmartCpy(&ptemp, &l_strmsg, sizeof(*l_StrMsg));
	StrMsg = SmartCpy(&ptemp, (void*)szMsg.operator LPCTSTR(), l_strmsg);
	return TRUE;
}
BOOL CSV_ASK_INVITE::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_INVITE::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	TarUNum = (int*)Jump(&ptemp, sizeof(*TarUNum));
	TarID = Jump(&ptemp, 16);
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	RoomPass = Jump(&ptemp, 5);
	MyUI = (USERINFO*)Jump(&ptemp, sizeof(USERINFO));
	l_StrMsg = (int*)Jump(&ptemp, sizeof(*l_StrMsg));
	StrMsg = ptemp;
	return TRUE;
}

////////// ��û �޼����� �ź�
BOOL CSV_REFUSE_INVITE::Set(int targetunum, char* targetid, char *myid, char *strmsg)
{
	char strTarId[16]={0,};
	if(strlen(targetid) < 16) strcpy(strTarId, targetid);
	char strMyId[16]={0,};
	if(strlen(myid) < 16) strcpy(strMyId, myid);
	CString szMsg;
	szMsg.Format("%s", strmsg);
	int l_strmsg = szMsg.GetLength();

	int msglen = sizeof(*TarUNum) + 16 + 16 + sizeof(*l_StrMsg) + l_strmsg;
	char* ptemp = SetHeader(SV_REFUSE_INVITE, msglen);
	if(ptemp==NULL) {TRACE("CSV_REFUSE_INVITE::Set() �޸� �Ҵ� ����\n"); return FALSE;}

	TarUNum = (int*)SmartCpy(&ptemp, &targetunum, sizeof(*TarUNum));
	TarID = SmartCpy(&ptemp, strTarId, 16);
	MyID = SmartCpy(&ptemp, strMyId, 16);
	l_StrMsg = (int*)SmartCpy(&ptemp, &l_strmsg, sizeof(*l_StrMsg));
	StrMsg = SmartCpy(&ptemp, (void*)szMsg.operator LPCTSTR(), l_strmsg);
	return TRUE;
}
BOOL CSV_REFUSE_INVITE::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_REFUSE_INVITE::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	TarUNum = (int*)Jump(&ptemp, sizeof(*TarUNum));
	TarID = Jump(&ptemp, 16);
	MyID = Jump(&ptemp, 16);
	l_StrMsg = (int*)Jump(&ptemp, sizeof(*l_StrMsg));
	StrMsg = ptemp;
	return TRUE;
}

////////// �߹� ��ǥ ��û
BOOL CSV_ASK_BANISHVOTE::Set(BANISHVOTEINFO *pbv)
{
	int msglen = sizeof(BANISHVOTEINFO);
	char* ptemp = SetHeader(SV_ASK_BANISHVOTE, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_BANISHVOTE::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	pBV = (BANISHVOTEINFO*)SmartCpy(&ptemp, pbv, sizeof(BANISHVOTEINFO));
	return TRUE;
}
BOOL CSV_ASK_BANISHVOTE::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_BANISHVOTE::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	pBV = (BANISHVOTEINFO*)Jump(&ptemp, sizeof(BANISHVOTEINFO));
	return TRUE;
}

////////// ���� �߹� ��ǥ ���
BOOL CSV_MYBANISHVOTE::Set(BOOL bagree)
{
	int msglen = sizeof(*bAgree);
	char* ptemp = SetHeader(SV_MYBANISHVOTE, msglen);
	if(ptemp==NULL) {TRACE("CSV_MYBANISHVOTE::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	bAgree = (BOOL*)SmartCpy(&ptemp, &bagree, sizeof(*bAgree));
	return TRUE;
}
BOOL CSV_MYBANISHVOTE::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_MYBANISHVOTE::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	bAgree = (BOOL*)Jump(&ptemp, sizeof(*bAgree));
	return TRUE;
}

////////// ���������� ������ ��û
BOOL CSV_ABNORMALOUTROOM::Set()
{
	int msglen = 0;
	char* ptemp = SetHeader(SV_ABNORMALOUTROOM, msglen);
	if(ptemp==NULL) {TRACE("CSV_ABNORMALOUTROOM::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	return TRUE;
}
BOOL CSV_ABNORMALOUTROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ABNORMALOUTROOM::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	return TRUE;
}



////////// ���� �߹� ��û	[���� ������ �۾�]
BOOL CSV_ASK_FORCEBANISH::Set(char *id)
{
	char strId[16]={0,};
	if(strlen(id) < 16) strcpy(strId, id);
	int msglen = 16;
	char* ptemp = SetHeader(SV_ASK_FORCEBANISH, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_FORCEBANISH::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	ID = SmartCpy(&ptemp, strId, 16);
	return TRUE;
}
BOOL CSV_ASK_FORCEBANISH::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_FORCEBANISH::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	ID = Jump(&ptemp, 16);
	return TRUE;
}

///////////////////////////////////////// [���� ������ �۾�] /////////////////////////////////////////

////////// ���� ������ ����� ��û
BOOL CSV_ASK_GAMEITEMLIST::Set()
{
	int msglen = 0;
	char* ptemp = SetHeader(SV_ASK_GAMEITEMLIST, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_GAMEITEMLIST::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	return TRUE;
}
BOOL CSV_ASK_GAMEITEMLIST::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_GAMEITEMLIST::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	return TRUE;
}

////////// ���� ������ ����� ������
BOOL CSV_GAMEITEMLIST::Set(GAMEITEM_INFO *pin, GAMEITEM *ppGI, LPCTSTR strmsg1, LPCTSTR strmsg2)
{
	int msglen = sizeof(GAMEITEM_INFO) + (sizeof(GAMEITEM) * pin->TotNum) + pin->StrMsgLen1 + pin->StrMsgLen2;
	char* ptemp = SetHeader(SV_GAMEITEMLIST, msglen);
	if(ptemp==NULL) {TRACE("CSV_GAMEITEMLIST::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	pIN = (GAMEITEM_INFO*)SmartCpy(&ptemp, pin, sizeof(GAMEITEM_INFO));
	if(pin->TotNum > 0) ArrayGI = (GAMEITEM*)SmartCpy(&ptemp, ppGI, sizeof(GAMEITEM) * pin->TotNum);
	else ArrayGI = NULL;
	if(pin->StrMsgLen1 > 0) StrMsg1 = (char*)SmartCpy(&ptemp, strmsg1, pin->StrMsgLen1);
	else StrMsg1 = NULL;
	if(pin->StrMsgLen2 > 0) StrMsg2 = (char*)SmartCpy(&ptemp, strmsg2, pin->StrMsgLen2);
	else StrMsg2 = NULL;
	//Encode();
	return TRUE;
}
BOOL CSV_GAMEITEMLIST::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_GAMEITEMLIST::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	pIN  = (GAMEITEM_INFO*)Jump(&ptemp, sizeof(GAMEITEM_INFO));
	if(pIN->TotNum > 0) ArrayGI = (GAMEITEM*)Jump(&ptemp, sizeof(GAMEITEM) * pIN->TotNum);
	else ArrayGI = NULL;
	if(pIN->StrMsgLen1 > 0) StrMsg1 = Jump(&ptemp, pIN->StrMsgLen1);
	else StrMsg1 = NULL;
	if(pIN->StrMsgLen2 > 0) StrMsg2 = Jump(&ptemp, pIN->StrMsgLen2);
	else StrMsg2 = NULL;
	return TRUE;
}

////////// ���� ������ ��� ����
BOOL CSV_USEGAMEITEM::Set(USEGAMEITEMINFO *pug)
{
	int msglen = sizeof(USEGAMEITEMINFO);
	char* ptemp = SetHeader(SV_USEGAMEITEM, msglen);
	if(ptemp==NULL) {TRACE("CSV_USEGAMEITEM::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	pUG = (USEGAMEITEMINFO*)SmartCpy(&ptemp, pug, sizeof(USEGAMEITEMINFO));
	//Encode();
	return TRUE;
}
BOOL CSV_USEGAMEITEM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_USEGAMEITEM::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	pUG = (USEGAMEITEMINFO*)ptemp;
	return TRUE;
}

////////// ���� ������ ������ ���
BOOL CSV_BUYGAMEITEM::Set(BUYGAMEITEMINFO *pbi, LPCTSTR strmsg)
{
	int msglen = sizeof(BUYGAMEITEMINFO) + pbi->StrMsgLen;
	char* ptemp = SetHeader(SV_BUYGAMEITEM, msglen);
	if(ptemp==NULL) {TRACE("CSV_BUYGAMEITEM::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	pBI = (BUYGAMEITEMINFO*)SmartCpy(&ptemp, pbi, sizeof(BUYGAMEITEMINFO));
	if(pbi->StrMsgLen > 0) StrMsg = (char*)SmartCpy(&ptemp, strmsg, pbi->StrMsgLen);
	else StrMsg = NULL;
	return TRUE;
}
BOOL CSV_BUYGAMEITEM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_BUYGAMEITEM::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	pBI = (BUYGAMEITEMINFO*)Jump(&ptemp, sizeof(BUYGAMEITEMINFO));
	if(pBI->StrMsgLen > 0) StrMsg = ptemp;
	else StrMsg = NULL;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////








////////// ������ ���۵Ǿ���
BOOL CSV_STARTGAME::Set(int roomnum)
{
	int msglen = sizeof(*RoomNum);
	char* ptemp = SetHeader(SV_STARTGAME, msglen);
	if(ptemp==NULL) {TRACE("CSV_STARTGAME::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &roomnum, sizeof(*RoomNum));
	return TRUE;
}
BOOL CSV_STARTGAME::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_STARTGAME::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	return TRUE;
}

////////// �������� ��� �뺸
BOOL CSV_GAMEOVERRESULT::Set(GAMEOVERRESULT *pGOR)
{
	int msglen =  sizeof(GAMEOVERRESULT);
	char* ptemp = SetHeader(SV_GAMEOVERRESULT, msglen);
	if(ptemp==NULL) {TRACE("CSV_GAMEOVERRESULT::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	//TotNum = (int*)SmartCpy(&ptemp, &totnum, sizeof(*TotNum));
	GOR = (GAMEOVERRESULT*)SmartCpy(&ptemp, pGOR, sizeof(GAMEOVERRESULT));
	//Encode();
	return TRUE;
}
BOOL CSV_GAMEOVERRESULT::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_GAMEOVERRESULT::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	//TotNum = (int*)Jump(&ptemp, sizeof(*TotNum));
	GOR = (GAMEOVERRESULT*)Jump(&ptemp, sizeof(GAMEOVERRESULT) );
	return TRUE;
}

////////// ���� �˸� �޼���
BOOL CSV_SERVERMSG::Set(int kind, char *chat, BOOL bcolorassigned, COLORREF color)
{
	CString szChat;
	szChat.Format("%s", chat);
	short l_strchat = (short) szChat.GetLength();
	int msglen = sizeof(*Kind) + sizeof(l_strchat) + l_strchat + sizeof(*bColorAssigned) + sizeof(*Color);

	char* ptemp = SetHeader(SV_SERVERMSG, msglen);
	if(ptemp==NULL) {TRACE("CSV_SERVERMSG::Set() �޸� �Ҵ� ����\n"); return FALSE;}

	Kind      = (int*)SmartCpy(&ptemp, &kind, sizeof(*Kind));
	l_StrChat = (short*)SmartCpy(&ptemp, &l_strchat, sizeof(l_strchat));
	StrChat   = SmartCpy(&ptemp, (void*)szChat.operator LPCTSTR(), l_strchat);
	bColorAssigned = (BOOL*)SmartCpy(&ptemp, &bcolorassigned, sizeof(*bColorAssigned));
	Color      = (COLORREF*)SmartCpy(&ptemp, &color, sizeof(*Color));
	return TRUE;
}
BOOL CSV_SERVERMSG::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_SERVERMSG::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	Kind      = (int*)Jump(&ptemp, sizeof(*Kind));
	l_StrChat = (short*)Jump(&ptemp, sizeof(*l_StrChat));
	StrChat = Jump(&ptemp, *l_StrChat);
	bColorAssigned = (BOOL*)Jump(&ptemp, sizeof(*bColorAssigned));
	Color = (COLORREF*)Jump(&ptemp, sizeof(*Color));
	return TRUE;
}

////////// �Ӹ� ��� ���� ����
BOOL CSV_NOWHISPER::Set(BOOL bnowhisper)
{
	int msglen = sizeof(*bNoWhisper);
	char* ptemp = SetHeader(SV_NOWHISPER, msglen);
	if(ptemp==NULL) {TRACE("CSV_NOWHISPER::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	bNoWhisper = (BOOL*)SmartCpy(&ptemp, &bnowhisper, sizeof(*bNoWhisper));
	return TRUE;
}
BOOL CSV_NOWHISPER::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_NOWHISPER::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	bNoWhisper = (BOOL*)Jump(&ptemp, sizeof(*bNoWhisper));
	return TRUE;
}

////////// ���� ä�� ����Ÿ
BOOL CSV_CHATDATA::Set(char *id, char *chat)
{
	char strId[16]={0,};
	CString szChat;
	szChat.Format("%s", chat);
	short l_strchat = (short) szChat.GetLength();
	int msglen = sizeof(strId) + sizeof(l_strchat) + l_strchat;

	char* ptemp = SetHeader(SV_CHATDATA, msglen);
	if(ptemp==NULL) {TRACE("CSV_CHATDATA::Set() �޸� �Ҵ� ����\n"); return FALSE;}

	if(strlen(id) < 16) strcpy(strId, id);
	
	ID        = SmartCpy(&ptemp, strId, 16);
	l_StrChat = (short*)SmartCpy(&ptemp, &l_strchat, sizeof(l_strchat));
	StrChat   = SmartCpy(&ptemp, (void*)szChat.operator LPCTSTR(), l_strchat);
	return TRUE;
}
BOOL CSV_CHATDATA::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_CHATDATA::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	ID        = Jump(&ptemp, 16);
	l_StrChat =(short*)Jump(&ptemp, sizeof(*l_StrChat));
	StrChat = ptemp;
	return TRUE;
}

////////// �ӼӸ�
BOOL CSV_WHISPERCHAT::Set(char *id, int destunum, char *destid, char *chat)
{
	char strId[16]={0,};
	char strDestId[16]={0,};
	CString szChat;
	szChat.Format("%s", chat);
	short l_strchat = (short) szChat.GetLength();
	int msglen = 16 + sizeof(*DestUNum) + 16 + sizeof(l_strchat) + l_strchat;

	char* ptemp = SetHeader(SV_WHISPERCHAT, msglen);
	if(ptemp==NULL) {TRACE("CSV_WHISPERCHAT::Set() �޸� �Ҵ� ����\n"); return FALSE;}

	if(strlen(id) < 16) strcpy(strId, id);
	if(strlen(destid) < 16) strcpy(strDestId, destid);
	
	ID        = SmartCpy(&ptemp, strId, 16);
	DestUNum  = (int*)SmartCpy(&ptemp, &destunum, sizeof(*DestUNum));
	DestID    = SmartCpy(&ptemp, strDestId, 16);
	l_StrChat = (short*)SmartCpy(&ptemp, &l_strchat, sizeof(l_strchat));
	StrChat   = SmartCpy(&ptemp, (void*)szChat.operator LPCTSTR(), l_strchat);
	return TRUE;
}
BOOL CSV_WHISPERCHAT::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_WHISPERCHAT::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	ID        = Jump(&ptemp, 16);
	DestUNum  = (int*)Jump(&ptemp, sizeof(*DestUNum));
	DestID    = Jump(&ptemp, 16);
	l_StrChat =(short*)Jump(&ptemp, sizeof(*l_StrChat));
	StrChat   = ptemp;
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////
//                      Ŭ���̾�Ʈ�� �޼��� Ŭ����
////////////////////////////////////////////////////////////////////////////


////////// �÷��̾� ������ ���� ����
BOOL CCL_EXITSUBSCRIPT::Set(int rnum, int unum, char *id, int bexit)
{
	char strId[16]={0,};
	int msglen = sizeof(*RoomNum)+ sizeof(*UNum) + 16 + sizeof(*bExit);
	char* ptemp = SetHeader(CL_EXITSUBSCRIPT, msglen);
	if(strlen(id) < 16) strcpy(strId, id);
	if(ptemp==NULL) {TRACE("CCL_EXITSUBSCRIPT::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ID = SmartCpy(&ptemp, strId, 16);
	bExit = (int*)SmartCpy(&ptemp, &bexit, sizeof(*bExit));
	return TRUE;
}
BOOL CCL_EXITSUBSCRIPT::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_EXITSUBSCRIPT::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ID = Jump(&ptemp, 16);
	bExit = (int*)Jump(&ptemp, sizeof(*bExit));
	return TRUE;
}

////////// ä�� ����Ÿ
BOOL CCL_CHATDATA::Set(int rnum, char *id, char kind, char* chat)
{
	char strId[16]={0,};
	short l_strchat = (short)strlen(chat);

	int msglen = sizeof(*RoomNum) + 16 + sizeof(*Kind) + sizeof(*l_StrChat) + l_strchat;

	if(strlen(id) < 16) strcpy(strId, id);

	char* ptemp = SetHeader(CL_CHATDATA, msglen);
	if(ptemp==NULL) {TRACE("CCL_CHATDATA::Set() �޸� �Ҵ� ����\n"); return FALSE;}

	RoomNum   = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	ID        = SmartCpy(&ptemp, strId, 16);
	Kind      = SmartCpy(&ptemp, &kind, sizeof(*Kind));
	l_StrChat = (short*)SmartCpy(&ptemp, &l_strchat, sizeof(*l_StrChat));
	StrChat   = SmartCpy(&ptemp, chat, l_strchat);
	return TRUE;
}
BOOL CCL_CHATDATA::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_CHATDATA::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum   = (int*)Jump(&ptemp, sizeof(*RoomNum));
	ID        = Jump(&ptemp, 16);
	Kind      = Jump(&ptemp, sizeof(*Kind));
	l_StrChat =(short*)Jump(&ptemp, sizeof(*l_StrChat));
	StrChat = ptemp;
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////
//                      ���� ����� �޼��� Ŭ����
////////////////////////////////////////////////////////////////////////////

////////// ������ ������ �غ� ������(������Ҷ� ���)
BOOL CCL_READYTOSTART::Set(char *id)
{
	char strId[16]={0,};
	int msglen = 16;
	char* ptemp = SetHeader(CL_READYTOSTART, msglen);

	if(strlen(id) < 16) strcpy(strId, id);

	if(ptemp==NULL) {TRACE("CCL_READYTOSTART::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	Id = SmartCpy(&ptemp, strId, 16);
	return TRUE;
}
BOOL CCL_READYTOSTART::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_READYTOSTART::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	Id = ptemp;
	return TRUE;
}

////////// ���� ��� �ٲٱ� ��ûint 
	
BOOL CCL_ASK_CHANGEGAMEKIND::Set(int rnum, int servpnum, char *id, int gamekind, int bwhis, int ncard)
{
	char strId[16]={0,};
	int msglen = sizeof(*RoomNum) + sizeof(*ServPNum) + 16 + sizeof(*GameKind) + sizeof(*bWhis) + sizeof(*nCard);
	char* ptemp = SetHeader(CL_ASK_CHANGEGAMEKIND, msglen);
	if(strlen(id) < 16) strcpy(strId, id);

	if(ptemp==NULL) {TRACE("CCL_ASK_CHANGEGAMEKIND::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	ServPNum = (int*)SmartCpy(&ptemp, &servpnum, sizeof(*ServPNum));
	ID = SmartCpy(&ptemp, strId, 16);
	GameKind = (int*)SmartCpy(&ptemp, &gamekind, sizeof(*GameKind));
	bWhis = (int*)SmartCpy(&ptemp, &bwhis, sizeof(*bWhis));
	nCard = (int*)SmartCpy(&ptemp, &ncard, sizeof(*nCard));
	return TRUE;
}
BOOL CCL_ASK_CHANGEGAMEKIND::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_ASK_CHANGEGAMEKIND::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	ServPNum = (int*)Jump(&ptemp, sizeof(*ServPNum));
	ID = Jump(&ptemp, 16);
	GameKind = (int*)Jump(&ptemp, sizeof(*GameKind));
	bWhis = (int*)Jump(&ptemp, sizeof(*bWhis));
	nCard = (int*)Jump(&ptemp, sizeof(*nCard));
	return TRUE;
}

////////// ���� ��� �ٲٱ� ���
BOOL CCL_CHANGEGAMEKIND::Set( ROOMINFO *pri)
{
	int msglen = sizeof(ROOMINFO);
	char* ptemp = SetHeader(CL_CHANGEGAMEKIND, msglen);
	if(ptemp==NULL) {TRACE("CCL_CHANGEGAMEKIND::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	pRI = (ROOMINFO*)SmartCpy(&ptemp, pri, sizeof(ROOMINFO));
	return TRUE;
}
BOOL CCL_CHANGEGAMEKIND::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_CHANGEGAMEKIND::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	pRI = (ROOMINFO*)Jump(&ptemp, sizeof(ROOMINFO));
	return TRUE;
}

////////// ���� ������ ��û
BOOL CCL_ASK_STARTGAME::Set(int rnum, int servpnum, char *id)
{
	char strId[16]={0,};
	int msglen = sizeof(*RoomNum) + sizeof(*ServPNum) + 16;
	char* ptemp = SetHeader(CL_ASK_STARTGAME, msglen);
	if(strlen(id) < 16) strcpy(strId, id);

	if(ptemp==NULL) {TRACE("CCL_ASK_STARTGAME::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	ServPNum = (int*)SmartCpy(&ptemp, &servpnum, sizeof(*ServPNum));
	ID = SmartCpy(&ptemp, strId, 16);
	return TRUE;
}
BOOL CCL_ASK_STARTGAME::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_ASK_STARTGAME::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	ServPNum = (int*)Jump(&ptemp, sizeof(*ServPNum));
	ID = Jump(&ptemp, 16);
	return TRUE;
}

////////// ���� ���� ����
BOOL CCL_STARTINFO::Set(STARTINFO *pSI)
{
	int msglen = sizeof(STARTINFO);
	char* ptemp = SetHeader(CL_STARTINFO, msglen);
	if(ptemp==NULL) {TRACE("CCL_STARTINFO::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	SI = (STARTINFO*)SmartCpy(&ptemp, pSI, sizeof(STARTINFO));
	//Encode();
	return TRUE;
}
BOOL CCL_STARTINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_STARTINFO::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	SI = (STARTINFO*)ptemp;
	return TRUE;
}


////////// ������ ���� 
BOOL CCL_ASK_CHSTYLEROOMINFO::Set(STYLEROOMINFO *pCHRI)
{
	int msglen = sizeof(STYLEROOMINFO);
	char* ptemp = SetHeader(CL_ASK_CHSTYLEROOMINFO, msglen);
	if(ptemp==NULL) {TRACE("CCL_ASK_CHSTYLEROOMINFO::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	CHRI = (STYLEROOMINFO*)SmartCpy(&ptemp, pCHRI, sizeof(STYLEROOMINFO));
	//Encode();
	return TRUE;
}
BOOL CCL_ASK_CHSTYLEROOMINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_ASK_CHSTYLEROOMINFO::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	CHRI = (STYLEROOMINFO*)ptemp;
	return TRUE;
}





//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

////////// ó�� ī�带 ���� 3��
BOOL CSV_STARTCARD::Set(int rnum, int unum, char *id, STARTINFO *psc)
{
	char strId[16]={0,};
	int msglen = sizeof(*RoomNum)+ sizeof(*UNum) + 16 + sizeof(STARTINFO);
	char* ptemp = SetHeader(SV_STARTCARD, msglen);
	if(strlen(id) < 16) strcpy(strId, id);
	if(ptemp==NULL) {TRACE("CSV_STARTCARD::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ID = SmartCpy(&ptemp, strId, 16);
	pSC = (STARTINFO*)SmartCpy(&ptemp, psc, sizeof(STARTINFO));
	return TRUE;
}

BOOL CSV_STARTCARD::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_STARTCARD::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ID = Jump(&ptemp, 16);
	pSC = (STARTINFO*)Jump(&ptemp, sizeof(STARTINFO));
	return TRUE;
}

////////// ī�� ���� ��û
BOOL CSV_CHANGECARD::Set(CHANGECARD *pcc)
{
	int msglen = sizeof(CHANGECARD);
	char* ptemp = SetHeader(SV_CHANGECARD, msglen);
	if(ptemp==NULL) {TRACE("CSV_CHANGECARD::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	pCC = (CHANGECARD*)SmartCpy(&ptemp, pcc, sizeof(CHANGECARD));
	return TRUE;
}

BOOL CSV_CHANGECARD::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_CHANGECARD::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	pCC = (CHANGECARD*)Jump(&ptemp, sizeof(CHANGECARD));
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
// 62cut ī�� ���� ��û
BOOL CSV_ASK_CHANGECARD_62CUT::Set(CHANGECARD_62CUT *pcc)
{
	int msglen = sizeof(CHANGECARD_62CUT);
	char* ptemp = SetHeader(SV_ASK_CHANGECARD_62CUT, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_CHANGECARD_62CUT::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	pCC = (CHANGECARD_62CUT*)SmartCpy(&ptemp, pcc, sizeof(CHANGECARD_62CUT));
	return TRUE;
}

BOOL CSV_ASK_CHANGECARD_62CUT::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("SV_ASK_CHANGECARD_62CUT::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	pCC = (CHANGECARD_62CUT*)Jump(&ptemp, sizeof(CHANGECARD_62CUT));
	return TRUE;
}

// 62cut �ٲ� ī�� �ѱ� (���ÿ� �Ѹ���
BOOL CSV_ASK_CHANGECARD_62CUT_EX::Set(CHANGECARD_62CUT_EX *pcc)
{
	int msglen = sizeof(CHANGECARD_62CUT_EX);
	char* ptemp = SetHeader(SV_ASK_CHANGECARD_62CUT_EX, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_CHANGECARD_62CUT_EX::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	pCC = (CHANGECARD_62CUT_EX*)SmartCpy(&ptemp, pcc, sizeof(CHANGECARD_62CUT_EX));
	return TRUE;
}

BOOL CSV_ASK_CHANGECARD_62CUT_EX::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("SV_ASK_CHANGECARD_62CUT_EX::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	pCC = (CHANGECARD_62CUT_EX*)Jump(&ptemp, sizeof(CHANGECARD_62CUT_EX));
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 62cut ����Ǵ� ī�带 ����
BOOL CSV_CHANGECARD_62CUT_RESULT::Set(CHANGECARD_62CUT *pcc)
{
	int msglen = sizeof(CHANGECARD_62CUT);
	char* ptemp = SetHeader(SV_CHANGECARD_62CUT_RESULT, msglen);
	if(ptemp==NULL) {TRACE("CSV_CHANGECARD_62CUT_RESULT::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	pCC = (CHANGECARD_62CUT*)SmartCpy(&ptemp, pcc, sizeof(CHANGECARD_62CUT));
	return TRUE;
}

BOOL CSV_CHANGECARD_62CUT_RESULT::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_CHANGECARD_62CUT_RESULT::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	pCC = (CHANGECARD_62CUT*)Jump(&ptemp, sizeof(CHANGECARD_62CUT));
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// �ٲٱ� ��� �˸�
BOOL CCL_CARDCHANGE_END_62CUT::Set(int rnum, int servpnum, char *id)
{
	char strId[16]={0,};
	int msglen = sizeof(*RoomNum) + sizeof(*ServPNum) + 16;
	char* ptemp = SetHeader(CL_CARDCHANGE_END_62CUT, msglen);
	if(strlen(id) < 16) strcpy(strId, id);

	if(ptemp==NULL) {TRACE("CCL_ASK_STARTGAME::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	ServPNum = (int*)SmartCpy(&ptemp, &servpnum, sizeof(*ServPNum));
	ID = SmartCpy(&ptemp, strId, 16);
	return TRUE;
}

BOOL CCL_CARDCHANGE_END_62CUT::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_CARDCHANGE_END_62CUT::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	ServPNum = (int*)Jump(&ptemp, sizeof(*ServPNum));
	ID = Jump(&ptemp, 16);
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////

// ���۹�ư Ȱ��ȭ �˸�
//���۹�ư Ȱ��ȭ 
BOOL CSV_ACTIVE_STARTBUTTON::Set(int rnum, int servpnum, char *id,int bactive)
{
	char strId[16]={0,};
	int msglen = sizeof(*RoomNum) + sizeof(*WinnerServPNum) + 16 + sizeof(*bActive);
	char* ptemp = SetHeader(SV_ACTIVE_STARTBUTTON, msglen);
	if(strlen(id) < 16) strcpy(strId, id);

	if(ptemp==NULL) {TRACE("CSV_ACTIVE_STARTBUTTON::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	WinnerServPNum = (int*)SmartCpy(&ptemp, &servpnum, sizeof(*WinnerServPNum));
	WinnerID = SmartCpy(&ptemp, strId, 16);
	bActive = (int*)SmartCpy(&ptemp, &bactive, sizeof(*bActive));
	return TRUE;
}

BOOL CSV_ACTIVE_STARTBUTTON::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ACTIVE_STARTBUTTON::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	WinnerServPNum = (int*)Jump(&ptemp, sizeof(*WinnerServPNum));
	WinnerID = Jump(&ptemp, 16);
	bActive = (int*)Jump(&ptemp, sizeof(*bActive));
	return TRUE;
}

////////// ī�� ��⸦ ��û
BOOL CSV_BUYCARD::Set(BUYCARD *pcc)
{
	
	int msglen = sizeof(BUYCARD);
	char* ptemp = SetHeader(SV_BUYCARD, msglen);

	if(ptemp==NULL) {TRACE("CSV_BUYCARD::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	pBC = (BUYCARD*)SmartCpy(&ptemp, pcc, sizeof(BUYCARD));
	return TRUE;
}

BOOL CSV_BUYCARD::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_BUYCARD::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	pBC = (BUYCARD*)Jump(&ptemp, sizeof(BUYCARD));
	return TRUE;
}


////////// ���� ���� FOLD
BOOL CCL_ASK_FOLD::Set(int rnum, int unum, int pnum, char *id, POKERCLNT *pfd)
{
	char strId[16]={0,};
	int msglen = sizeof(*RoomNum)+ sizeof(*UNum) + sizeof(*PNum) + 16 + sizeof(POKERCLNT);
	char* ptemp = SetHeader(CL_ASK_FOLD, msglen);
	if(strlen(id) < 16) strcpy(strId, id);
	if(ptemp==NULL) {TRACE("CCL_ASK_FOLD::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	PNum = (int*)SmartCpy(&ptemp, &pnum, sizeof(*PNum));
	ID = SmartCpy(&ptemp, strId, 16);
	pFD = (POKERCLNT*)SmartCpy(&ptemp, pfd, sizeof(POKERCLNT));
	return TRUE;
}

BOOL CCL_ASK_FOLD::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_ASK_FOLD::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	PNum = (int*)Jump(&ptemp, sizeof(*PNum));
	ID = Jump(&ptemp, 16);
	pFD = (POKERCLNT*)Jump(&ptemp, sizeof(POKERCLNT));
	return TRUE;
}


////////// ���� ���� ī�� ����
BOOL CCL_ASK_SELECTCARD::Set(int rnum, int unum, int pnum, char *id, POKERCLNT *psd, int discard)
{
	char strId[16]={0,};
	int msglen = sizeof(*RoomNum)+ sizeof(*UNum) + sizeof(*PNum) + 16 + sizeof(POKERCLNT)+ sizeof(*DisCard);
	char* ptemp = SetHeader(CL_ASK_SELECTCARD, msglen);
	if(strlen(id) < 16) strcpy(strId, id);
	if(ptemp==NULL) {TRACE("CCL_ASK_SELECTCARD::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	PNum = (int*)SmartCpy(&ptemp, &pnum, sizeof(*PNum));
	ID = SmartCpy(&ptemp, strId, 16);
	pSD = (POKERCLNT*)SmartCpy(&ptemp, psd, sizeof(POKERCLNT));
	DisCard = (int*)SmartCpy(&ptemp, &discard, sizeof(*DisCard));
	return TRUE;
}

BOOL CCL_ASK_SELECTCARD::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_ASK_SELECTCARD::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	PNum = (int*)Jump(&ptemp, sizeof(*PNum));
	ID = Jump(&ptemp, 16);
	pSD = (POKERCLNT*)Jump(&ptemp, sizeof(POKERCLNT));
	DisCard = (int*)Jump(&ptemp, sizeof(*DisCard));
	return TRUE;
}


////////// Raise
BOOL CCL_ASK_RAISE::Set(int rnum, int unum, int pnum, char *id, POKERCLNT *pra)
{
	char strId[16]={0,};
	int msglen = sizeof(*RoomNum)+ sizeof(*UNum) + sizeof(*PNum) + 16 + sizeof(POKERCLNT);
	char* ptemp = SetHeader(CL_ASK_RAISE, msglen);
	if(strlen(id) < 16) strcpy(strId, id);
	if(ptemp==NULL) {TRACE("CCL_ASK_RAISE::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	PNum = (int*)SmartCpy(&ptemp, &pnum, sizeof(*PNum));
	ID = SmartCpy(&ptemp, strId, 16);
	pRA = (POKERCLNT*)SmartCpy(&ptemp, pra, sizeof(POKERCLNT));
	return TRUE;
}

BOOL CCL_ASK_RAISE::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_ASK_RAISE::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	PNum = (int*)Jump(&ptemp, sizeof(*PNum));
	ID = Jump(&ptemp, 16);
	pRA = (POKERCLNT*)Jump(&ptemp, sizeof(POKERCLNT));
	return TRUE;
}

//////////  ����� ī�� ������ ����
BOOL CCL_WINDCARDEND::Set(int rnum, int pnum, int unum, char *id)//, POKERCLNT *pwd)
{
	char strId[16]={0,};
	int msglen = sizeof(*RoomNum) + sizeof(*PNum) + sizeof(*UNum) + 16; // + sizeof(POKERCLNT);
	char* ptemp = SetHeader(CL_WINDCARDEND, msglen);
	if(strlen(id) < 16) strcpy(strId, id);
	if(ptemp==NULL) {TRACE("CCL_WINDCARDEND::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	PNum = (int*)SmartCpy(&ptemp, &pnum, sizeof(*PNum));
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ID = SmartCpy(&ptemp, strId, 16);
//	pWD = (POKERCLNT*)SmartCpy(&ptemp, pwd, sizeof(POKERCLNT));
	return TRUE;
}

BOOL CCL_WINDCARDEND::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_WINDCARDEND::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	PNum = (int*)Jump(&ptemp, sizeof(*PNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ID = Jump(&ptemp, 16);
//	pWD = (POKERCLNT*)Jump(&ptemp, sizeof(POKERCLNT));
	return TRUE;
}


////////// �� �Һ�� ���� �޼���
BOOL CCL_ROOM_TIMEOUT::Set(int rnum, int unum, char *id, int lstrlen, char *msg)
{
	char strId[16]={0,};
	int msglen = sizeof(*RoomNum)+ sizeof(*UNum) + 16 + sizeof(l_StrMsg) + lstrlen;
	char* ptemp = SetHeader(CL_ROOM_TIMEOUT, msglen);
	if(strlen(id) < 16) strcpy(strId, id);
	if(ptemp==NULL) {TRACE("CCL_ROOM_TIMEOUT::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ID = SmartCpy(&ptemp, strId, 16);
	l_StrMsg = (int*)SmartCpy(&ptemp, &lstrlen, sizeof(*l_StrMsg));
	StrMsg = SmartCpy(&ptemp, msg, lstrlen);
	return TRUE;
}

BOOL CCL_ROOM_TIMEOUT::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_ROOM_TIMEOUT::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ID = Jump(&ptemp, 16);
	l_StrMsg = (int*)Jump(&ptemp, sizeof(*l_StrMsg));
	StrMsg = Jump(&ptemp, *l_StrMsg);
	return TRUE;
}


////////// ���伣 ī��� �̰��� ��� ��� �����ڿ��� �˸�
BOOL CSV_SPECIALCARD::Set(SPECIALCARD *psc)
{
	int msglen =  sizeof(SPECIALCARD);
	char* ptemp = SetHeader(SV_SPECIALCARD, msglen);
	if(ptemp==NULL) {TRACE("CSV_SPECIALCARD::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	pSC = (SPECIALCARD*)SmartCpy(&ptemp, psc, sizeof(SPECIALCARD));
	return TRUE;
}
BOOL CSV_SPECIALCARD::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_SPECIALCARD::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	pSC = (SPECIALCARD*)Jump(&ptemp, sizeof(SPECIALCARD) );
	return TRUE;
}


/////////// ������ ����� ���� ����
BOOL CCL_USERRESULT::Set(USERRESULT *prs)
{
	int msglen =  sizeof(USERRESULT);
	char* ptemp = SetHeader(CL_USERRESULT, msglen);
	if(ptemp==NULL) {TRACE("CCL_USERRESULT::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	pRS = (USERRESULT*)SmartCpy(&ptemp, prs, sizeof(USERRESULT));
	return TRUE;
}
BOOL CCL_USERRESULT::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_USERRESULT::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	pRS = (USERRESULT*)Jump(&ptemp, sizeof(USERRESULT) );
	return TRUE;
}


/////////// ���� �˸� ������
BOOL CSV_RAISE::Set(POKERGAME *ppg)
{
	int msglen =  sizeof(POKERGAME);
	char* ptemp = SetHeader(SV_RAISE, msglen);
	if(ptemp==NULL) {TRACE("CSV_RAISE::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	pPG = (POKERGAME*)SmartCpy(&ptemp, ppg, sizeof(POKERGAME));
	return TRUE;
}
BOOL CSV_RAISE::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_RAISE::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	pPG = (POKERGAME*)Jump(&ptemp, sizeof(POKERGAME) );
	return TRUE;
}

////////// ������ ���� ī�� 
BOOL CSV_HIDDENCARD::Set(int rnum, int unum, char *id, int card)
{
	char strId[16]={0,};
	int msglen = sizeof(*RoomNum)+ sizeof(*UNum) + 16 + sizeof(*nCard);
	char* ptemp = SetHeader(SV_HIDDENCARD, msglen);
	if(strlen(id) < 16) strcpy(strId, id);
	if(ptemp==NULL) {TRACE("CSV_HIDDENCARD::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ID = SmartCpy(&ptemp, strId, 16);
	nCard = (int*)SmartCpy(&ptemp, &card, sizeof(*nCard));
	return TRUE;
}

BOOL CSV_HIDDENCARD::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_HIDDENCARD::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ID = Jump(&ptemp, 16);
	nCard = (int*)Jump(&ptemp, sizeof(*nCard));
	return TRUE;
}


//ù 3���� ������ ������ �ϳ��� ī�带 ���� => ��� ���ý� ���ӽ���
BOOL CSV_WINDCARDEND::Set(int reserve )
{
	
	int msglen = sizeof(*Reserve);
	char* ptemp = SetHeader(SV_WINDCARDEND, msglen);
	if(ptemp==NULL) {TRACE("CSV_WINDCARDEND::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	Reserve = (int*)SmartCpy(&ptemp, &reserve, sizeof(*Reserve));
	
	return TRUE;
}

BOOL CSV_WINDCARDEND::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_WINDCARDEND::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	Reserve = (int*)Jump(&ptemp, sizeof(*Reserve));
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// 62cut client �� server �� �ٲ� ī�带 ���� 
BOOL CCL_CARDCHANGE_62CUT_RESULT::Set(CHANGECARD_62CUT *pcc)
{
	int msglen = sizeof(CHANGECARD_62CUT);
	char* ptemp = SetHeader(CL_CARDCHANGE_62CUT_RESULT, msglen);
	if(ptemp==NULL) {TRACE("CCL_CARDCHAGE_62CUT_RESULT::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	pCC = (CHANGECARD_62CUT*)SmartCpy(&ptemp, pcc, sizeof(CHANGECARD_62CUT));
	return TRUE;
}

BOOL CCL_CARDCHANGE_62CUT_RESULT::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_CARDCHAGE_62CUT_RESULT::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	pCC = (CHANGECARD_62CUT*)Jump(&ptemp, sizeof(CHANGECARD_62CUT));
	return TRUE;
}


// �� ���� �˸�
BOOL CSV_ENDGAME::Set(int reserve )
{
	
	int msglen = sizeof(*Reserve);
	char* ptemp = SetHeader(SV_ENDGAME, msglen);
	if(ptemp==NULL) {TRACE("CSV_ENDGAME::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	Reserve = (int*)SmartCpy(&ptemp, &reserve, sizeof(*Reserve));
	
	return TRUE;
}

BOOL CSV_ENDGAME::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ENDGAME::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	Reserve = (int*)Jump(&ptemp, sizeof(*Reserve));
	return TRUE;
}


/////////// ���� �˸� ������ ��  ���� �ο� ���� ����
BOOL CSV_ENDRAISE::Set(int flag, POKERGAME *ppg)
{
	int msglen =  sizeof(*Flag) + sizeof(POKERGAME);
	char* ptemp = SetHeader(SV_ENDRAISE, msglen);
	if(ptemp==NULL) {TRACE("CSV_ENDRAISE::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	Flag = (int*)SmartCpy(&ptemp, &flag, sizeof(*Flag));
	pPG = (POKERGAME*)SmartCpy(&ptemp, ppg, sizeof(POKERGAME));
	return TRUE;
}
BOOL CSV_ENDRAISE::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ENDRAISE::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	Flag = (int*)Jump(&ptemp, sizeof(*Flag) );
	pPG = (POKERGAME*)Jump(&ptemp, sizeof(POKERGAME) );
	return TRUE;
}

////////// � �ɷ� �ºθ� �ܷ�ڴ°�?
BOOL CCL_SELECTWINCASE::Set(int rnum, int servepnum, int unum, char *id, int ncase)
{
	char strId[16]={0,};
	int msglen = sizeof(*RoomNum)+ sizeof(*ServPNum) + sizeof(*UNum) + 16 + sizeof(*nCase);
	char* ptemp = SetHeader(CL_SELECTWINCASE, msglen);
	if(strlen(id) < 16) strcpy(strId, id);
	if(ptemp==NULL) {TRACE("CCL_SELECTWINCASE::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	ServPNum = (int*)SmartCpy(&ptemp, &servepnum, sizeof(*ServPNum));
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ID = SmartCpy(&ptemp, strId, 16);
	nCase = (int*)SmartCpy(&ptemp, &ncase, sizeof(*nCase));
	return TRUE;
}

BOOL CCL_SELECTWINCASE::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_SELECTWINCASE::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	ServPNum = (int*)Jump(&ptemp, sizeof(*ServPNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ID = Jump(&ptemp, 16);
	nCase = (int*)Jump(&ptemp, sizeof(*nCase));
	return TRUE;
}




/////////////////////////////////////////////////////////////////////////////////////////
// [ ���� ������ ]
// ���� �޼���~~~
BOOL CCL_ITEMRESULT::Set( int recv_code )
{
	int msglen = sizeof( *RetCode );
	char* ptemp = SetHeader( CL_ITEMRESULT, msglen );
	if( ptemp == NULL ) { TRACE( "CCL_ITEMRESULT::Set() �޸� �Ҵ� ����\n" ); return FALSE; }
	RetCode = (int*)SmartCpy( &ptemp, &recv_code, sizeof(*RetCode) );
	
	return TRUE;
}

BOOL CCL_ITEMRESULT::Get( char* lpdata, int size )
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_ITEMRESULT::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RetCode = (int*)Jump(&ptemp, sizeof(*RetCode));

	return TRUE;
}

// ������ ������ �ֱ� ����Ѱ� ���ٸ� �����Ҽ� �ִ�.
BOOL CCL_ITEMLIST::Set( GAMEITEM* recv_item, int recv_itemcount, int recv_use )
{
	int msglen = sizeof( GAMEITEM ) * recv_itemcount + sizeof(*Itemcount) + sizeof(*Use);
	char* ptemp = SetHeader( CL_ITEMLIST, msglen );
	if( ptemp == NULL ) { TRACE( "CCL_ITEMLIST::Set() �޸� �Ҵ� ����\n" ); return FALSE; }
	Itemcount = (int*)SmartCpy( &ptemp, &recv_itemcount, sizeof(*Itemcount) );
	Item = (GAMEITEM*)SmartCpy( &ptemp, recv_item, sizeof(GAMEITEM) * recv_itemcount );
	Use = (int*)SmartCpy( &ptemp, &recv_use, sizeof(*Use) );

	return TRUE;
}

BOOL CCL_ITEMLIST::Get( char* lpdata, int size )
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_ITEMLIST::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	Itemcount = (int*)Jump(&ptemp, sizeof(*Itemcount) );
	Item = (GAMEITEM*)Jump(&ptemp, sizeof(GAMEITEM) * (*Itemcount) );
	Use = (int*)Jump( &ptemp, sizeof(*Use) );

	return TRUE;
}


// ������ ���� ��û
BOOL CCL_ASK_ITEMLOGIN::Set( GAMEITEM* recv_item, int recv_itemcount )
{
	int msglen = sizeof( GAMEITEM ) * recv_itemcount + sizeof(*Itemcount);
	char* ptemp = SetHeader( CL_ASK_ITEMLOGIN, msglen );
	if( ptemp == NULL ) { TRACE( "CCL_ASK_ITEMLOGIN::Set() �޸� �Ҵ� ����\n" ); return FALSE; }
	Itemcount = (int*)SmartCpy( &ptemp, &recv_itemcount, sizeof(*Itemcount) );
	Item = (GAMEITEM*)SmartCpy( &ptemp, recv_item, sizeof(GAMEITEM) * recv_itemcount );

	return TRUE;
}

BOOL CCL_ASK_ITEMLOGIN::Get( char* lpdata, int size )
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_ASK_ITEMLOGIN::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	Itemcount = (int*)Jump(&ptemp, sizeof(*Itemcount) );
	Item = (GAMEITEM*)Jump(&ptemp, sizeof(GAMEITEM) * (*Itemcount) );

	return TRUE;
}

// ������ ���� 
BOOL CCL_ACCEPT_ITEMLOGIN::Set( GAMEITEM* recv_item, int recv_itemcount )
{
	int msglen = sizeof( GAMEITEM ) * recv_itemcount + sizeof(*Itemcount);
	char* ptemp = SetHeader( CL_ACCEPT_ITEMLOGIN, msglen );
	if( ptemp == NULL ) { TRACE( "CCL_ACCEPT_ITEMLOGIN::Set() �޸� �Ҵ� ����\n" ); return FALSE; }
	Itemcount = (int*)SmartCpy( &ptemp, &recv_itemcount, sizeof(*Itemcount) );
	Item = (GAMEITEM*)SmartCpy( &ptemp, recv_item, sizeof(GAMEITEM) * recv_itemcount );

	return TRUE;
}

BOOL CCL_ACCEPT_ITEMLOGIN::Get( char* lpdata, int size )
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_ACCEPT_ITEMLOGIN::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	Itemcount = (int*)Jump(&ptemp, sizeof(*Itemcount) );
	Item = (GAMEITEM*)Jump(&ptemp, sizeof(GAMEITEM) * (*Itemcount) );

	return TRUE;
}

// ������ ����ؼ� ����
BOOL CCL_ITEMLOGIN::Set( int rc_index )
{
	int msglen = sizeof(*Index);
	char* ptemp = SetHeader( CL_ITEMLOGIN, msglen );
	if( ptemp == NULL ) { TRACE( "CCL_ITEMLOGIN::Set() �޸� �Ҵ� ����\n" ); return FALSE; }
	Index = (int*)SmartCpy( &ptemp, &rc_index, sizeof(*Index) );

	return TRUE;
}

BOOL CCL_ITEMLOGIN::Get( char* lpdata, int size )
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_ITEMLOGIN::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	if( ptemp == NULL ) { TRACE( "" ); return FALSE; }
	Index = (int*)Jump( &ptemp, sizeof(*Index) );
	
	return TRUE;
}


////////// ������ ����
BOOL CSV_FOLDUSER::Set(int rnum, int unum, int pnum, char *id, int kind, INT64 jackpot)
{
	char strId[16]={0,};
	int msglen = sizeof(*RoomNum)+ sizeof(*UNum) + sizeof(*PNum) + 16 + sizeof(*Kind) + sizeof(*JackPotMoney);
	char* ptemp = SetHeader(SV_FOLDUSER, msglen);
	if(strlen(id) < 16) strcpy(strId, id);
	if(ptemp==NULL) {TRACE("CSV_FOLDUSER::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	PNum = (int*)SmartCpy(&ptemp, &pnum, sizeof(*PNum));
	ID = SmartCpy(&ptemp, strId, 16);
	Kind = (int*)SmartCpy(&ptemp, &kind, sizeof(*Kind));
	JackPotMoney = (INT64*)SmartCpy(&ptemp, &jackpot, sizeof(*JackPotMoney));
	return TRUE;
}

BOOL CSV_FOLDUSER::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_FOLDUSER::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	PNum = (int*)Jump(&ptemp, sizeof(*PNum));
	ID = Jump(&ptemp, 16);
	Kind = (int*)Jump(&ptemp, sizeof(*Kind));
	JackPotMoney = (INT64*)Jump(&ptemp, sizeof(*JackPotMoney));
	return TRUE;
}



/////////// �ٷ� ���� �� ������ ��û
BOOL CSV_ASK_QUICKSTART::Set(int unum, char *id, int sndKind, int gamekind)// ### [���� �߰� �۾�] ###
{
	int msglen = sizeof(*UNum) + 16 + sizeof(*SndFxKind) + sizeof(*GameKind); // �߰�
	char strId[16]={0,};
	if(strlen(id) < 16) strcpy(strId, id);
	char* ptemp = SetHeader(SV_ASK_QUICKSTART, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_QUICKSTART::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ID = SmartCpy(&ptemp, strId, 16);
	SndFxKind = (int*)SmartCpy(&ptemp, &sndKind, sizeof(*SndFxKind)); // �߰�
	GameKind = (int*)SmartCpy(&ptemp, &gamekind, sizeof(*GameKind)); 
	return TRUE;
}
BOOL CSV_ASK_QUICKSTART::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_QUICKSTART::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ID = Jump(&ptemp, 16);
	SndFxKind = (int*)Jump(&ptemp, sizeof(*SndFxKind));// ### [���� �߰� �۾�] ###
	GameKind = (int*)Jump(&ptemp, sizeof(*GameKind));
	return TRUE;
}



/////////// �ٷ� ���� �Ұ�(�����)
BOOL CSV_REFUSE_QUICKSTART::Set(int code) 
{
	int msglen = sizeof(*Code);
	char* ptemp = SetHeader(SV_REFUSE_QUICKSTART, msglen);
	if(ptemp==NULL) {TRACE("CSV_REFUSE_QUICKSTART::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	Code = (int*)SmartCpy(&ptemp, &code, sizeof(*Code));
	return TRUE;
}
BOOL CSV_REFUSE_QUICKSTART::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_REFUSE_QUICKSTART::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	Code = (int*)ptemp;
	return TRUE;
}




///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// ����������Ʈ �۾�	

BOOL CNM_AGENT_CONNECT::Set(int tot, INT64 jackpot) 
{
	int msglen = sizeof(*TotalNum)+sizeof(*JackPot);
	char* ptemp = SetHeader(NM_AGENT_CONNECT, msglen);
	if(ptemp==NULL) {TRACE("CNM_AGENT_CONNECT::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	TotalNum = (int*)SmartCpy(&ptemp, &tot, sizeof(*TotalNum));
	JackPot = (INT64*)SmartCpy(&ptemp, &jackpot, sizeof(*JackPot));
	return TRUE;
}
BOOL CNM_AGENT_CONNECT::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CNM_AGENT_CONNECT::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	TotalNum = (int*)Jump(&ptemp, sizeof(*TotalNum));
	JackPot = (INT64*)Jump(&ptemp, sizeof(*JackPot));
	return TRUE;
}

// ���� ��� ������
BOOL CNM_AGENT_USERLIST::Set(int reserve) 
{
	int msglen = sizeof(*Reserve);
	char* ptemp = SetHeader(NM_AGENT_USERLIST, msglen);
	if(ptemp==NULL) {TRACE("CNM_AGENT_USERLIST::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	Reserve = (int*)SmartCpy(&ptemp, &reserve, sizeof(*Reserve));
	return TRUE;
}
BOOL CNM_AGENT_USERLIST::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CNM_AGENT_USERLIST::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	Reserve = (int*)ptemp;
	return TRUE;
}


////////// 1�� 400��
BOOL CNM_AGENT_USERLIST_R1::Set(int totnum, AGENT_USER *ppSUI, int count)
{
	int msglen = sizeof(*TotNum) + sizeof(*Count) + sizeof(AGENT_USER)*totnum; 
	char* ptemp = SetHeader(NM_AGENT_USERLIST_R1, msglen);
	if(ptemp==NULL) {TRACE("CNM_AGENT_USERLIST_R1::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	TotNum = (int*)SmartCpy(&ptemp, &totnum, sizeof(*TotNum));
	Count = (int*)SmartCpy(&ptemp, &count, sizeof(*Count));
	ArraySUI = (AGENT_USER*)SmartCpy(&ptemp, ppSUI, sizeof(AGENT_USER)*totnum);
	return TRUE;
}
BOOL CNM_AGENT_USERLIST_R1::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CNM_AGENT_USERLIST_R1::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	TotNum   = (int*)Jump(&ptemp, sizeof(*TotNum));
	Count = (int*)Jump(&ptemp, sizeof(*Count));
	ArraySUI = (AGENT_USER*)ptemp;
	return TRUE;
}

// ���ο� ���� ����
BOOL CNM_AGENT_NEWUSER::Set(int totnum, AGENT_USER *ppSUI, INT64 jackpot) 
{
	int msglen = sizeof(*TotNum) + sizeof(AGENT_USER) + sizeof(*JackPot);
	char* ptemp = SetHeader(NM_AGENT_NEWUSER, msglen);
	if(ptemp==NULL) {TRACE("CNM_AGENT_NEWUSER::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	TotNum = (int*)SmartCpy(&ptemp, &totnum, sizeof(*TotNum));
	ArraySUI = (AGENT_USER*)SmartCpy(&ptemp, ppSUI, sizeof(AGENT_USER));
	JackPot = (INT64*)SmartCpy(&ptemp, &jackpot, sizeof(*JackPot));
	return TRUE;
}
BOOL CNM_AGENT_NEWUSER::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CNM_AGENT_NEWUSER::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	TotNum   = (int*)Jump(&ptemp, sizeof(*TotNum));
	ArraySUI = (AGENT_USER*)Jump(&ptemp, sizeof(AGENT_USER));
	JackPot = (INT64*)Jump(&ptemp, sizeof(*JackPot));
	return TRUE;
}

// ���� �ƿ�
BOOL CNM_AGENT_OUTUSER::Set(int totnum, AGENT_USER *ppSUI, INT64 jackpot) 
{
	int msglen = sizeof(*TotNum) + sizeof(AGENT_USER) + sizeof(*JackPot);
	char* ptemp = SetHeader(NM_AGENT_OUTUSER, msglen);
	if(ptemp==NULL) {TRACE("CNM_AGENT_OUTUSER::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	TotNum = (int*)SmartCpy(&ptemp, &totnum, sizeof(*TotNum));
	ArraySUI = (AGENT_USER*)SmartCpy(&ptemp, ppSUI, sizeof(AGENT_USER));
	JackPot = (INT64*)SmartCpy(&ptemp, &jackpot, sizeof(*JackPot));
	return TRUE;
}
BOOL CNM_AGENT_OUTUSER::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CNM_AGENT_OUTUSER::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	TotNum   = (int*)Jump(&ptemp, sizeof(*TotNum));
	ArraySUI = (AGENT_USER*)Jump(&ptemp, sizeof(AGENT_USER));
	JackPot = (INT64*)Jump(&ptemp, sizeof(*JackPot));
	return TRUE;
}

// �ߺ� ���� Ȯ��
BOOL CNM_AGENT_DUBLECON::Set(int totnum, AGENT_USER *ppSUI)
{
	int msglen = sizeof(*TotNum) + sizeof(AGENT_USER);
	char* ptemp = SetHeader(NM_AGENT_DUBLECON, msglen);
	if(ptemp==NULL) {TRACE("CNM_AGENT_DUBLECON::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	TotNum = (int*)SmartCpy(&ptemp, &totnum, sizeof(*TotNum));
	ArraySUI = (AGENT_USER*)SmartCpy(&ptemp, ppSUI, sizeof(AGENT_USER));
	return TRUE;
}
BOOL CNM_AGENT_DUBLECON::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CNM_AGENT_DUBLECON::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	TotNum   = (int*)Jump(&ptemp, sizeof(*TotNum));
	ArraySUI = (AGENT_USER*)ptemp;
	return TRUE;
}


// �ߺ� ������
BOOL CNM_AGENT_NEWUSERERROR::Set(int totnum, AGENT_USER *ppSUI)
{
	int msglen = sizeof(*TotNum) + sizeof(AGENT_USER);
	char* ptemp = SetHeader(NM_AGENT_NEWUSERERROR, msglen);
	if(ptemp==NULL) {TRACE("CNM_AGENT_NEWUSERERROR::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	TotNum = (int*)SmartCpy(&ptemp, &totnum, sizeof(*TotNum));
	ArraySUI = (AGENT_USER*)SmartCpy(&ptemp, ppSUI, sizeof(AGENT_USER));
	return TRUE;
}
BOOL CNM_AGENT_NEWUSERERROR::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CNM_AGENT_NEWUSERERROR::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	TotNum   = (int*)Jump(&ptemp, sizeof(*TotNum));
	ArraySUI = (AGENT_USER*)ptemp;
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// ���� ������ �۾�


// ���� ������ ���� ��û
BOOL CCL_ASK_JUMPITEM::Set( GAMEITEM* recv_item, int recv_itemcount )
{
	int msglen = sizeof( GAMEITEM ) * recv_itemcount + sizeof(*Itemcount);
	char* ptemp = SetHeader( CL_ASK_JUMPITEM, msglen );
	if( ptemp == NULL ) { TRACE( "CCL_ASK_JUMPITEM::Set() �޸� �Ҵ� ����\n" ); return FALSE; }
	Itemcount = (int*)SmartCpy( &ptemp, &recv_itemcount, sizeof(*Itemcount) );
	Item = (GAMEITEM*)SmartCpy( &ptemp, recv_item, sizeof(GAMEITEM) * recv_itemcount );

	return TRUE;
}

BOOL CCL_ASK_JUMPITEM::Get( char* lpdata, int size )
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_ASK_JUMPITEM::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	Itemcount = (int*)Jump(&ptemp, sizeof(*Itemcount) );
	Item = (GAMEITEM*)Jump(&ptemp, sizeof(GAMEITEM) * (*Itemcount) );

	return TRUE;
}


// ���� ������ ä�� ���� ���
BOOL CCL_ACCEPT_JUMPITEM::Set( GAMEITEM* recv_item, int recv_itemcount, ENTERCHANINFO *pEC )
{
	int msglen = sizeof( GAMEITEM ) * recv_itemcount + sizeof(*Itemcount) + sizeof(ENTERCHANINFO);
	char* ptemp = SetHeader( CL_ACCEPT_JUMPITEM, msglen );
	if( ptemp == NULL ) { TRACE( "CCL_ACCEPT_JUMPITEM::Set() �޸� �Ҵ� ����\n" ); return FALSE; }
	Itemcount = (int*)SmartCpy( &ptemp, &recv_itemcount, sizeof(*Itemcount) );
	Item = (GAMEITEM*)SmartCpy( &ptemp, recv_item, sizeof(GAMEITEM) * recv_itemcount );
	EC = (ENTERCHANINFO*)SmartCpy(&ptemp, pEC, sizeof(ENTERCHANINFO));
	return TRUE;
}

BOOL CCL_ACCEPT_JUMPITEM::Get( char* lpdata, int size )
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_ACCEPT_JUMPITEM::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	Itemcount = (int*)Jump(&ptemp, sizeof(*Itemcount) );
	Item = (GAMEITEM*)Jump(&ptemp, sizeof(GAMEITEM) * (*Itemcount) );
	EC = (ENTERCHANINFO*)Jump(&ptemp, sizeof(ENTERCHANINFO));

	return TRUE;
}


// ���������� ����ؼ� ����
BOOL CCL_JUMPITEM_LOGIN::Set( int rc_index )
{
	int msglen = sizeof(*Index);
	char* ptemp = SetHeader( CL_JUMPITEM_LOGIN, msglen );
	if( ptemp == NULL ) { TRACE( "CCL_JUMPITEM_LOGIN::Set() �޸� �Ҵ� ����\n" ); return FALSE; }
	Index = (int*)SmartCpy( &ptemp, &rc_index, sizeof(*Index) );

	return TRUE;
}

BOOL CCL_JUMPITEM_LOGIN::Get( char* lpdata, int size )
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_JUMPITEM_LOGIN::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	if( ptemp == NULL ) { TRACE( "" ); return FALSE; }
	Index = (int*)Jump( &ptemp, sizeof(*Index) );
	
	return TRUE;
}

//  DB �� ��� ���� ������Ʈ �� ����� �˷��ش�.
BOOL CCL_ACCEPT_JUMPITEM_CH::Set( GAMEITEM* recv_item, int recv_itemcount )
{
	int msglen = sizeof( GAMEITEM ) * recv_itemcount + sizeof(*Itemcount);
	char* ptemp = SetHeader( CL_ACCEPT_JUMPITEM_CH, msglen );
	if( ptemp == NULL ) { TRACE( "CCL_ACCEPT_JUMPITEM_CH::Set() �޸� �Ҵ� ����\n" ); return FALSE; }
	Itemcount = (int*)SmartCpy( &ptemp, &recv_itemcount, sizeof(*Itemcount) );
	Item = (GAMEITEM*)SmartCpy( &ptemp, recv_item, sizeof(GAMEITEM) * recv_itemcount );

	return TRUE;
}

BOOL CCL_ACCEPT_JUMPITEM_CH::Get( char* lpdata, int size )
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_ACCEPT_JUMPITEM_CH::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	Itemcount = (int*)Jump(&ptemp, sizeof(*Itemcount) );
	Item = (GAMEITEM*)Jump(&ptemp, sizeof(GAMEITEM) * (*Itemcount) );

	return TRUE;
}

////////// ���� ��� 
BOOL CNM_AGENT_AVERAGE::Set(INT64 jackpot)
{

	int msglen = sizeof(*JackPotMoney);
	char* ptemp = SetHeader(NM_AGENT_AVERAGE, msglen);
	if(ptemp==NULL) {TRACE("CNM_AGENT_AVERAGE::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	JackPotMoney = (INT64*)SmartCpy(&ptemp, &jackpot, sizeof(*JackPotMoney));
	return TRUE;
}

BOOL CNM_AGENT_AVERAGE::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CNM_AGENT_AVERAGE::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	JackPotMoney = (INT64*)Jump(&ptemp, sizeof(*JackPotMoney));
	return TRUE;
}


// ### [���� �߰� �۾�] ###     ���� ��û
BOOL CSV_ASK_CHANGESNDFX::Set(int unum, int spnum, int rnum, char *id, int sndfx)
{
	char strID[16];
	strncpy(strID, id, 15);
	int msglen = sizeof(*UNum) + sizeof(*ServPNum) + sizeof(*RoomNum) + 16 + sizeof(*SndFX);
	char* ptemp = SetHeader(SV_ASK_CHANGESNDFX, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_CHANGESNDFX::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	UNum = (int*) SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ServPNum = (int*) SmartCpy(&ptemp, &spnum, sizeof(*ServPNum));
	RoomNum= (int*) SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	ID = SmartCpy(&ptemp, strID, 16);
	SndFX = (int*) SmartCpy(&ptemp, &sndfx, sizeof(*SndFX));
	return TRUE;
}

BOOL CSV_ASK_CHANGESNDFX::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_CHANGESNDFX::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ServPNum =(int*)Jump(&ptemp, sizeof(*ServPNum));
	RoomNum =(int*)Jump(&ptemp, sizeof(*RoomNum));
	ID = Jump(&ptemp, 16);
	SndFX = (int*)Jump(&ptemp, sizeof(*SndFX));
	return TRUE;
}


// ### [���� �߰� �۾�] ###     ���� ���
BOOL CSV_ACCEPT_CHANGESNDFX::Set(int sndfx)
{
	int msglen = sizeof(*SndFx);
	char* ptemp = SetHeader(SV_ACCEPT_CHANGESNDFX, msglen);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_CHANGESNDFX::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	SndFx = (int*) SmartCpy(&ptemp, &sndfx, sizeof(*SndFx));
	return TRUE;
}
BOOL CSV_ACCEPT_CHANGESNDFX::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_CHANGESNDFX::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	SndFx = (int*)Jump(&ptemp, sizeof(*SndFx));
	return TRUE;
}


////////// �̺�Ʈ ��÷�� �˸�
BOOL CSV_EVENTPRIZE::Set(EVENTPRIZEINFO *pei)
{
	int msglen = sizeof(EVENTPRIZEINFO);
	char* ptemp = SetHeader(SV_EVENTPRIZE, msglen);
	if(ptemp==NULL) {TRACE("CSV_EVENTPRIZE::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	pEI = (EVENTPRIZEINFO*)SmartCpy(&ptemp, pei, sizeof(EVENTPRIZEINFO));
	return TRUE;
}
BOOL CSV_EVENTPRIZE::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_EVENTPRIZE::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	pEI = (EVENTPRIZEINFO*)Jump(&ptemp, sizeof(EVENTPRIZEINFO));
	return TRUE;
}


////////// �̺�Ʈ ����
BOOL CSV_EVENTNOTICE::Set(short kind, char* chat, COLORREF color)
{
	short l_strchat = (short)strlen(chat);
	int msglen = sizeof(*Kind) + sizeof(*l_StrChat) + l_strchat + sizeof(*Color);

	char* ptemp = SetHeader(SV_EVENTNOTICE, msglen);
	if(ptemp==NULL) {TRACE("CSV_EVENTNOTICE::Set() �޸� �Ҵ� ����\n"); return FALSE;}

	Kind      = (short*)SmartCpy(&ptemp, &kind, sizeof(*Kind));
	l_StrChat = (short*)SmartCpy(&ptemp, &l_strchat, sizeof(*l_StrChat));
	StrChat   = SmartCpy(&ptemp, chat, l_strchat);
	Color     = (COLORREF*)SmartCpy(&ptemp, &color, sizeof(*Color));
	return TRUE;
}
BOOL CSV_EVENTNOTICE::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_CHATDATA::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	Kind      = (short*)Jump(&ptemp, sizeof(*Kind));
	l_StrChat = (short*)Jump(&ptemp, sizeof(*l_StrChat));
	StrChat = Jump(&ptemp, *l_StrChat);
	Color = (COLORREF*)Jump(&ptemp, sizeof(*Color));
	return TRUE;
}


///////////////////////////////////////////////////////////////////
/////////////////// ### [������ ��� �۾�] ###  ///////////////////
///////////////////////////////////////////////////////////////////

/////////// ������ �Ƶ� �����Ѵ� ������Ʈ
BOOL CNM_AGENT_ADMININFO::Set(int totnum, ADMIN_INFO *ppCI)
{
	int msglen = sizeof(*TotNum) + sizeof(ADMIN_INFO)*totnum;
	char* ptemp = SetHeader(NM_AGENT_ADMININFO, msglen);
	if(ptemp==NULL) {TRACE("CNM_AGENT_ADMININFO::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	TotNum = (int*)SmartCpy(&ptemp, &totnum, sizeof(*TotNum));
	AdminInfo = (ADMIN_INFO*)SmartCpy(&ptemp, ppCI, sizeof(ADMIN_INFO)*totnum);
	return TRUE;
}
BOOL CNM_AGENT_ADMININFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CNM_AGENT_ADMININFO::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	TotNum   = (int*)Jump(&ptemp, sizeof(*TotNum));
	AdminInfo = (ADMIN_INFO*)ptemp;
	return TRUE;
}


//���� �����ϱ�
BOOL CNM_ADMIN_OUTUSER::Set(ADMINMESSAGE *pmsg)
{
	int msglen =  sizeof(ADMINMESSAGE);
	char* ptemp = SetHeader(NM_ADMIN_OUTUSER, msglen);
	if(ptemp==NULL) {TRACE("CNM_ADMIN_OUTUSER::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	pAMESG = (ADMINMESSAGE*)SmartCpy(&ptemp, pmsg, sizeof(ADMINMESSAGE));
	return TRUE;
}
BOOL CNM_ADMIN_OUTUSER::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CNM_ADMIN_OUTUSER::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	pAMESG = (ADMINMESSAGE*)Jump(&ptemp, sizeof(ADMINMESSAGE) );
	return TRUE;
}

// �������ϱ�
BOOL CNM_ADMIN_DELROOM::Set(ADMINMESSAGE *pmsg)
{
	int msglen =  sizeof(ADMINMESSAGE);
	char* ptemp = SetHeader(NM_ADMIN_DELROOM, msglen);
	if(ptemp==NULL) {TRACE("CNM_ADMIN_DELROOM::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	pAMESG = (ADMINMESSAGE*)SmartCpy(&ptemp, pmsg, sizeof(ADMINMESSAGE));
	return TRUE;
}
BOOL CNM_ADMIN_DELROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CNM_ADMIN_DELROOM::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	pAMESG = (ADMINMESSAGE*)Jump(&ptemp, sizeof(ADMINMESSAGE) );
	return TRUE;
}

////////// �ӼӸ�
BOOL CNM_ADMIN_WHISPERCHAT::Set(char *id, int destunum, char *destid, char *chat)
{
	char strId[16]={0,};
	char strDestId[16]={0,};
	CString szChat;
	szChat.Format("%s", chat);
	short l_strchat = (short) szChat.GetLength();
	int msglen = 16 + sizeof(*DestUNum) + 16 + sizeof(l_strchat) + l_strchat;

	char* ptemp = SetHeader(NM_ADMIN_WHISPERCHAT, msglen);
	if(ptemp==NULL) {TRACE("CNM_ADMIN_WHISPERCHAT::Set() �޸� �Ҵ� ����\n"); return FALSE;}

	if(strlen(id) < 16) strcpy(strId, id);
	if(strlen(destid) < 16) strcpy(strDestId, destid);
	
	ID        = SmartCpy(&ptemp, strId, 16);
	DestUNum  = (int*)SmartCpy(&ptemp, &destunum, sizeof(*DestUNum));
	DestID    = SmartCpy(&ptemp, strDestId, 16);
	l_StrChat = (short*)SmartCpy(&ptemp, &l_strchat, sizeof(l_strchat));
	StrChat   = SmartCpy(&ptemp, (void*)szChat.operator LPCTSTR(), l_strchat);
	return TRUE;
}

BOOL CNM_ADMIN_WHISPERCHAT::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CNM_ADMIN_WHISPERCHAT::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	ID        = Jump(&ptemp, 16);
	DestUNum  = (int*)Jump(&ptemp, sizeof(*DestUNum));
	DestID    = Jump(&ptemp, 16);
	l_StrChat =(short*)Jump(&ptemp, sizeof(*l_StrChat));
	StrChat   = ptemp;
	return TRUE;
}


// ���� �����ϱ�
BOOL CNM_ADMIN_CHANGEROOM::Set(ADMINMESSAGE *padmsg, STYLEROOMINFO *pmsg)
{
	int msglen =  sizeof(STYLEROOMINFO) + sizeof(ADMINMESSAGE);
	char* ptemp = SetHeader(NM_ADMIN_CHANGEROOM, msglen);
	if(ptemp==NULL) {TRACE("CNM_ADMIN_CHANGEROOM::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	pAMESG = (ADMINMESSAGE*)SmartCpy(&ptemp, padmsg, sizeof(ADMINMESSAGE));
	STYLE = (STYLEROOMINFO*)SmartCpy(&ptemp, pmsg, sizeof(STYLEROOMINFO));
	return TRUE;
}
BOOL CNM_ADMIN_CHANGEROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CNM_ADMIN_CHANGEROOM::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	pAMESG = (ADMINMESSAGE*)Jump(&ptemp, sizeof(ADMINMESSAGE) );
	STYLE = (STYLEROOMINFO*)Jump(&ptemp, sizeof(STYLEROOMINFO) );
	return TRUE;
}


/////////// �� ���� ���� ��û �˸�
BOOL CSV_CHANGESTYLEROOMINFO::Set(CHANGESTYLEROOMINFO *pSRI)
{
	int msglen = sizeof(CHANGESTYLEROOMINFO);
	char* ptemp = SetHeader(SV_CHANGESTYLEROOMINFO, msglen);
	if(ptemp==NULL) {TRACE("CSV_CHANGESTYLEROOMINFO::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	SRI = (CHANGESTYLEROOMINFO*)SmartCpy(&ptemp, pSRI, sizeof(CHANGESTYLEROOMINFO));
	return TRUE;
}
BOOL CSV_CHANGESTYLEROOMINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_CHANGESTYLEROOMINFO::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	SRI = (CHANGESTYLEROOMINFO*)Jump(&ptemp, sizeof(CHANGESTYLEROOMINFO) );
	return TRUE;
}

// ����� ����ֱ� 
BOOL CNM_SPECIALMESSAGE::Set(ADMINMESSAGE *pmsg)
{
	int msglen =  sizeof(ADMINMESSAGE);
	char* ptemp = SetHeader(NM_SPECIALMESSAGE, msglen);
	if(ptemp==NULL) {TRACE("CNM_SPECIALMESSAGE::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	pAMESG = (ADMINMESSAGE*)SmartCpy(&ptemp, pmsg, sizeof(ADMINMESSAGE));
	return TRUE;
}
BOOL CNM_SPECIALMESSAGE::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CNM_SPECIALMESSAGE::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	pAMESG = (ADMINMESSAGE*)Jump(&ptemp, sizeof(ADMINMESSAGE) );
	return TRUE;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////
/////////////////// ### [ ������� ] ###  ///////////////////
/////////////////////////////////////////////////////////////

////////// ������ ��û
BOOL CSV_ASK_PARTICIPATION::Set(int rnum, int unum, int servpnum, char *id, int sndfx)
{
	char strid[16]={0,};
	if(id != NULL) strncpy(strid, id, 15);
	int msglen = sizeof(*RoomNum) + sizeof(*UNum) + sizeof(*ServPNum) + 16 + sizeof(*SndFX); 
	char* ptemp = SetHeader(SV_ASK_PARTICIPATION, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_PARTICIPATION::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ServPNum = (int*)SmartCpy(&ptemp, &servpnum, sizeof(*ServPNum));
	ID = SmartCpy(&ptemp, strid, 16);
	SndFX = (int*)SmartCpy(&ptemp, &sndfx, sizeof(*SndFX));
	return TRUE;
}
BOOL CSV_ASK_PARTICIPATION::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_PARTICIPATION::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ServPNum = (int*)Jump(&ptemp, sizeof(*ServPNum));
	ID = Jump(&ptemp, 16);
	SndFX = (int*)Jump(&ptemp, sizeof(*SndFX));
	return TRUE;
}



////////// ����������
BOOL CSV_ACCEPT_PARTICIPATION::Set(int rnum, int nowpnum, int newpnum, int sndKind, char *id, IPARRINDEX *pIPA)// �߰�
{
	int msglen = sizeof(*RoomNum) + sizeof(*NowPNum) + sizeof(*NewPNum) + sizeof(*SndFxKind)+ 16 + sizeof(IPARRINDEX);
	char strid[16]={0,};
	if(id != NULL) strncpy(strid, id, 15);
	char* ptemp = SetHeader(SV_ACCEPT_PARTICIPATION, msglen);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_PARTICIPATION::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	NowPNum = (int*)SmartCpy(&ptemp, &nowpnum, sizeof(*NowPNum));
	NewPNum = (int*)SmartCpy(&ptemp, &newpnum, sizeof(*NewPNum));
	SndFxKind = (int*)SmartCpy(&ptemp, &sndKind, sizeof(*SndFxKind)); // �߰�
	ID = SmartCpy(&ptemp, strid, 16);
	IPA = (IPARRINDEX*)SmartCpy(&ptemp, pIPA, sizeof(IPARRINDEX));
	return TRUE;
}
BOOL CSV_ACCEPT_PARTICIPATION::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_PARTICIPATION::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	NowPNum = (int*)Jump(&ptemp, sizeof(*NowPNum));
	NewPNum = (int*)Jump(&ptemp, sizeof(*NewPNum));
	SndFxKind = (int*)Jump(&ptemp, sizeof(*SndFxKind));// ### [���� �߰� �۾�] ###
	ID = Jump(&ptemp, 16);
	IPA = (IPARRINDEX*)Jump(&ptemp, sizeof(IPARRINDEX));
	return TRUE;
}

////////// �ٸ��������� �˸�
BOOL CSV_ENTER_PARTICIPATION::Set(int rnum, int nowpnum, int newpnum, int sndKind, char *id, IPARRINDEX *pIPA)// �߰�
{
	int msglen = sizeof(*RoomNum) + sizeof(*NowPNum) + sizeof(*NewPNum) + sizeof(*SndFxKind)+ 16 +sizeof(IPARRINDEX);
	char strid[16]={0,};
	if(id != NULL) strncpy(strid, id, 15);
	char* ptemp = SetHeader(SV_ENTER_PARTICIPATION, msglen);
	if(ptemp==NULL) {TRACE("CSV_ENTER_PARTICIPATION::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	NowPNum = (int*)SmartCpy(&ptemp, &nowpnum, sizeof(*NowPNum));
	NewPNum = (int*)SmartCpy(&ptemp, &newpnum, sizeof(*NewPNum));
	SndFxKind = (int*)SmartCpy(&ptemp, &sndKind, sizeof(*SndFxKind)); // �߰�
	ID = SmartCpy(&ptemp, strid, 16);
	IPA = (IPARRINDEX*)SmartCpy(&ptemp, pIPA, sizeof(IPARRINDEX));
	return TRUE;
}
BOOL CSV_ENTER_PARTICIPATION::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ENTER_PARTICIPATION::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	NowPNum = (int*)Jump(&ptemp, sizeof(*NowPNum));
	NewPNum = (int*)Jump(&ptemp, sizeof(*NewPNum));
	SndFxKind = (int*)Jump(&ptemp, sizeof(*SndFxKind));// ### [���� �߰� �۾�] ###
	ID = Jump(&ptemp, 16);
	IPA = (IPARRINDEX*)Jump(&ptemp, sizeof(IPARRINDEX));
	return TRUE;
}


////////// ������ -> �������� ��ȯ
BOOL CSV_ASK_EMERAGE::Set(int rnum, int unum, int servpnum, char *id)
{
	char strid[16]={0,};
	if(id != NULL) strncpy(strid, id, 15);
	int msglen = sizeof(*RoomNum) + sizeof(*UNum) + sizeof(*ServPNum) + 16; 
	char* ptemp = SetHeader(SV_ASK_EMERAGE, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_EMERAGE::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ServPNum = (int*)SmartCpy(&ptemp, &servpnum, sizeof(*ServPNum));
	ID = SmartCpy(&ptemp, strid, 16);
	return TRUE;
}
BOOL CSV_ASK_EMERAGE::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_EMERAGE::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ServPNum = (int*)Jump(&ptemp, sizeof(*ServPNum));
	ID = Jump(&ptemp, 16);
	return TRUE;
}


////////// ����
BOOL CSV_ACCEPT_EMERAGE::Set(int rnum, int nowpnum, int newpnum,  char *id, IPARRINDEX *pIPA)// �߰�
{
	int msglen = sizeof(*RoomNum) + sizeof(*NowPNum) + sizeof(*NewPNum) + 16 + sizeof(IPARRINDEX);
	char strid[16]={0,};
	if(id != NULL) strncpy(strid, id, 15);
	char* ptemp = SetHeader(SV_ACCEPT_EMERAGE, msglen);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_EMERAGE::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	NowPNum = (int*)SmartCpy(&ptemp, &nowpnum, sizeof(*NowPNum));
	NewPNum = (int*)SmartCpy(&ptemp, &newpnum, sizeof(*NewPNum));
	ID = SmartCpy(&ptemp, strid, 16);
	IPA = (IPARRINDEX*)SmartCpy(&ptemp, pIPA, sizeof(IPARRINDEX));
	return TRUE;
}
BOOL CSV_ACCEPT_EMERAGE::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_EMERAGE::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	NowPNum = (int*)Jump(&ptemp, sizeof(*NowPNum));
	NewPNum = (int*)Jump(&ptemp, sizeof(*NewPNum));
	ID = Jump(&ptemp, 16);
	IPA = (IPARRINDEX*)Jump(&ptemp, sizeof(IPARRINDEX));
	return TRUE;
}


//////////// ���� �������� �ٸ��������� �˸���
BOOL CSV_ENTER_EMERAGE::Set(int rnum, int nowpnum, int newpnum,  char *id, IPARRINDEX *pIPA)// �߰�
{
	int msglen = sizeof(*RoomNum) + sizeof(*NowPNum) + sizeof(*NewPNum) + 16+sizeof(IPARRINDEX);
	char strid[16]={0,};
	if(id != NULL) strncpy(strid, id, 15);
	char* ptemp = SetHeader(SV_ENTER_EMERAGE, msglen);
	if(ptemp==NULL) {TRACE("CSV_ENTER_EMERAGE::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	NowPNum = (int*)SmartCpy(&ptemp, &nowpnum, sizeof(*NowPNum));
	NewPNum = (int*)SmartCpy(&ptemp, &newpnum, sizeof(*NewPNum));
	ID = SmartCpy(&ptemp, strid, 16);
	IPA = (IPARRINDEX*)SmartCpy(&ptemp, pIPA, sizeof(IPARRINDEX));
	return TRUE;
}
BOOL CSV_ENTER_EMERAGE::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ENTER_EMERAGE::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	NowPNum = (int*)Jump(&ptemp, sizeof(*NowPNum));
	NewPNum = (int*)Jump(&ptemp, sizeof(*NewPNum));
	ID = Jump(&ptemp, 16);
	IPA = (IPARRINDEX*)Jump(&ptemp, sizeof(IPARRINDEX));
	return TRUE;
}

/////////// ������ ���� ����
BOOL CSV_RESERVATIONINFO::Set(int totnum, RESERVATIONINFO *reservation)
{
	int msglen = sizeof(*TotNum) + sizeof(RESERVATIONINFO)*totnum;
	char* ptemp = SetHeader(SV_RESERVATIONINFO, msglen);
	if(ptemp==NULL) {TRACE("CSV_RESERVATIONINFO::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	TotNum = (int*)SmartCpy(&ptemp, &totnum, sizeof(*TotNum));
	ReservedInfo = (RESERVATIONINFO*)SmartCpy(&ptemp, reservation, sizeof(RESERVATIONINFO)*totnum);
	return TRUE;
}
BOOL CSV_RESERVATIONINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_RESERVATIONINFO::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	TotNum   = (int*)Jump(&ptemp, sizeof(*TotNum));
	ReservedInfo = (RESERVATIONINFO*)ptemp;
	return TRUE;
}


/////////// �������� �˸� �޼���
BOOL CSV_OBSERVER_MESSAGE::Set(int kind, char *chat)
{
	CString szChat;
	szChat.Format("%s", chat);
	short l_strchat = (short) szChat.GetLength();
	int msglen = sizeof(*Kind) + sizeof(l_strchat) + l_strchat;// + 4;

	char* ptemp = SetHeader(SV_OBSERVER_MESSAGE, msglen);
	if(ptemp==NULL) {TRACE("CSV_OBSERVER_MESSAGE::Set() �޸� �Ҵ� ����\n"); return FALSE;}

	Kind      = (int*)SmartCpy(&ptemp, &kind, sizeof(*Kind));
	l_StrChat = (short*)SmartCpy(&ptemp, &l_strchat, sizeof(l_strchat));
	StrChat   = SmartCpy(&ptemp, (void*)szChat.operator LPCTSTR(), l_strchat);
	return TRUE;
}
BOOL CSV_OBSERVER_MESSAGE::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_OBSERVER_MESSAGE::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	Kind      =(int*)Jump(&ptemp, sizeof(*Kind));
	l_StrChat =(short*)Jump(&ptemp, sizeof(*l_StrChat));
	StrChat = ptemp;
	return TRUE;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


////////// ���� ���� �޼���
BOOL CSV_SERVERNOTIC::Set(int kind, char *chat, BOOL bcolorassigned, COLORREF color)
{
	CString szChat;
	szChat.Format("%s", chat);
	short l_strchat = (short) szChat.GetLength();
	int msglen = sizeof(*Kind) + sizeof(l_strchat) + l_strchat + sizeof(*bColorAssigned) + sizeof(*Color);

	char* ptemp = SetHeader(SV_SERVERNOTIC, msglen);
	if(ptemp==NULL) {TRACE("CSV_SERVERNOTIC::Set() �޸� �Ҵ� ����\n"); return FALSE;}

	Kind      = (int*)SmartCpy(&ptemp, &kind, sizeof(*Kind));
	l_StrChat = (short*)SmartCpy(&ptemp, &l_strchat, sizeof(l_strchat));
	StrChat   = SmartCpy(&ptemp, (void*)szChat.operator LPCTSTR(), l_strchat);
	bColorAssigned = (BOOL*)SmartCpy(&ptemp, &bcolorassigned, sizeof(*bColorAssigned));
	Color      = (COLORREF*)SmartCpy(&ptemp, &color, sizeof(*Color));
	return TRUE;
}
BOOL CSV_SERVERNOTIC::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_SERVERNOTIC::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	Kind      = (int*)Jump(&ptemp, sizeof(*Kind));
	l_StrChat = (short*)Jump(&ptemp, sizeof(*l_StrChat));
	StrChat = Jump(&ptemp, *l_StrChat);
	bColorAssigned = (BOOL*)Jump(&ptemp, sizeof(*bColorAssigned));
	Color = (COLORREF*)Jump(&ptemp, sizeof(*Color));
	return TRUE;
}


// ī�� ���� Ÿ�̸� �˸�
BOOL CSV_SELECTCARD::Set(int rnum, int unum, int servpnum, char *id)
{
	char strid[16]={0,};
	if(id != NULL) strncpy(strid, id, 15);
	int msglen = sizeof(*RoomNum) + sizeof(*UNum) + sizeof(*ServPNum) + 16; 
	char* ptemp = SetHeader(SV_SELECTCARD, msglen);
	if(ptemp==NULL) {TRACE("CSV_SELECTCARD::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ServPNum = (int*)SmartCpy(&ptemp, &servpnum, sizeof(*ServPNum));
	ID = SmartCpy(&ptemp, strid, 16);
	return TRUE;
}
BOOL CSV_SELECTCARD::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_SELECTCARD::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ServPNum = (int*)Jump(&ptemp, sizeof(*ServPNum));
	ID = Jump(&ptemp, 16);
	return TRUE;
}

// [��ȣõ��] 2004.07.08 ����->������� Ȯ��
BOOL CSV_ASK_SAFEANGEL::Set()
{
	int msglen = 0; 
	char* ptemp = SetHeader(SV_ASK_SAFEANGEL, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_SAFEANGEL::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	return TRUE;
}

BOOL CSV_ASK_SAFEANGEL::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_SAFEANGEL::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	return TRUE;
}

// [��ȣõ��] 2004.07.08 �Ӵ� ���� �޽���
BOOL CCL_CHARGE_SAFEANGEL::Set( char *id, int Kind, INT64 Money )
{
	char strid[16]={0,};
	if(id != NULL) strncpy(strid, id, 15);
	int msglen = sizeof(*kind) + sizeof(*money) + 16;
	char* ptemp = SetHeader(CL_CHARGE_SAFEANGEL, msglen);
	if(ptemp==NULL) {TRACE("CCL_CHARGE_SAFEANGEL::Set() �޸� �Ҵ� ����\n"); return FALSE;}
	kind = (int*)SmartCpy( &ptemp, &Kind, sizeof(*kind) ); 
	money = (INT64*)SmartCpy( &ptemp, &Money, sizeof(*money) );
	ID = SmartCpy(&ptemp, strid, 16);
	return TRUE;
}

BOOL CCL_CHARGE_SAFEANGEL::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_CHARGE_SAFEANGEL::Get() �޸� �Ҵ� ����\n"); return FALSE;}
	kind = (int*)Jump( &ptemp, sizeof(*kind) );
	money = (INT64*)Jump( &ptemp, sizeof(*money) );
	ID = Jump(&ptemp, 16);
	return TRUE;
}