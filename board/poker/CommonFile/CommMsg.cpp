#include "StdAfx.h"
#include "CommMsg.h"


////////////////////////////////////////////////////////////////////////////
//                              메세지 기본 클래스
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
//                              메세지 기본 클래스
////////////////////////////////////////////////////////////////////////////
// 암호화/복호화용 테이블
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


////////////// CommMsg 클래스를 아래와같이 변경한다. 함수에 덮어 씌우세요
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

//---- 함수 추가
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

//---- 함수 추가
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


//---- 함수 추가
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



//---- 기존 함수 교체
BOOL CCommMsg::CheckInfo(char *lpdata, int *psignal, int *pmsglen)
{
	PACKETHEADER *pH = (PACKETHEADER*)lpdata;
	if(*((short*)pH->MsgID) != *((short*)MSG_ID)) return FALSE;
	if(pH->Dummy[1] != 2) return FALSE;
	*psignal = (int)(pH->Signal-g_GHashTable[pH->PackCnt & 255]);
	*pmsglen = (int)pH->MsgLen;
	return TRUE;
}

//---- 기존 함수 교체
char* CCommMsg::SetHeader(int signal, int msglen)
{
	// 버퍼가 참조된 데이터가 아닌 경우만 새로운 메모리를 할당
	if(!bReferedData) {
		if(pData) free(pData);
		pData = (char*)malloc(PACKET_HEADERSIZE + msglen);
		if(pData==NULL) return NULL;
	}

	// 메시지 사이즈 계산
	TotalSize = PACKET_HEADERSIZE + msglen;

	pHead = (PACKETHEADER*)pData;
	*((short*)pHead->MsgID) = *((short*)MSG_ID);
	pHead->Signal = (short)signal;
	pHead->MsgLen = (short)msglen;
	pHead->Dummy[1] = 1;

	return (pData + PACKET_HEADERSIZE);
}

//---- 기존 함수 교체
char* CCommMsg::GetHeader(char *lpdata, int size)
{
	if(!lpdata) return NULL;
	if(!bReferedData && pData) free(pData);
	pData = lpdata;
	pHead = (PACKETHEADER*)lpdata;
	
	TotalSize = size;
	// 참조된 데이터임을 설정(파괴자 호출시 메모리 해제를 하지 않게함)
	bReferedData = TRUE;
	// 암호화 되어있다면 디코드함
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
//                      서버용 메세지 클래스
////////////////////////////////////////////////////////////////////////////

////////// 버젼을 체크함
BOOL CSV_CHECKVERSION::Set(int ver, int prversion)
{
	int msglen = sizeof(*Ver)+sizeof(*PreVersion);
	char* ptemp = SetHeader(SV_CHECKVERSION, msglen);
	if(ptemp==NULL) {TRACE("CSV_CHECKVERSION::Set() 메모리 할당 에러\n"); return FALSE;}
	Ver = (int*)SmartCpy(&ptemp, &ver, sizeof(*Ver));
	PreVersion = (int*)SmartCpy(&ptemp, &prversion, sizeof(*PreVersion));
	return TRUE;
}
BOOL CSV_CHECKVERSION::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_CHECKVERSION::Get() 메모리 할당 에러\n"); return FALSE;}
	Ver = (int*)Jump(&ptemp, sizeof(*Ver));
	PreVersion = (int*)Jump(&ptemp, sizeof(*PreVersion));
	return TRUE;
}


////////// 서버 정보	[게임 아이템 작업]
BOOL CSV_SERVERINFO::Set(SERVERINFO *pSI)
{
	int msglen = sizeof(SERVERINFO);
	char* ptemp = SetHeader(SV_SERVERINFO, msglen);
	if(ptemp==NULL) {TRACE("CSV_SERVERINFO::Set() 메모리 할당 에러\n"); return FALSE;}
	SI = (SERVERINFO*)SmartCpy(&ptemp, pSI, sizeof(SERVERINFO));
	//Encode();
	return TRUE;
}
BOOL CSV_SERVERINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_SERVERINFO::Get() 메모리 할당 에러\n"); return FALSE;}
	SI = (SERVERINFO*)Jump(&ptemp, sizeof(SERVERINFO));
	return TRUE;
}



/*
//////// 로그인을 요청함
BOOL CSV_LOGIN::Set(char *id, char *pass, BOOL bOver, char *socknameip, char *hostnameip, int port)
{
	char strid[16]={0,};
	char strpass[10]={0,};
	char strSockNameIp[20]={0,};
	char strHostNameIp[20]={0,};

	int  msglen = 16 + 10 + sizeof(*Over) + 20 + 20 + sizeof(*Port);
	char* ptemp = SetHeader(SV_LOGIN, msglen);
	if(ptemp==NULL) {TRACE("CSV_LOGIN::Set() 메모리 할당 에러\n"); return FALSE;}

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
	if(ptemp==NULL) {TRACE("CSV_LOGIN::Get() 메모리 할당 에러\n"); return FALSE;}
	ID = Jump(&ptemp, 16);
	Pass = Jump(&ptemp, 10);
	Over = (BOOL*)Jump(&ptemp, sizeof(*Over));
	SockNameIP = Jump(&ptemp, 20);
	HostNameIP = Jump(&ptemp, 20);
	Port = (int*)Jump(&ptemp, sizeof(*Port));
	return TRUE;
}
*/

//2004.05.07 리더스총액 제한
BOOL CSV_LEADERSJOIN::Set(LEADERS_JOIN *pLJ)
{
	int msglen = sizeof(LEADERS_JOIN);
	char* ptemp = SetHeader(SV_LEADERSJOIN, msglen);
	if(ptemp==NULL) {TRACE("CSV_LEADERSJOIN::Set() 메모리 할당 에러\n"); return FALSE;}
	memcpy(ptemp, pLJ, msglen);
	LJ = (LEADERS_JOIN*)ptemp;
	return TRUE;
}
BOOL CSV_LEADERSJOIN::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_LEADERSJOIN::Get() 메모리 할당 에러\n"); return FALSE;}
	LJ = (LEADERS_JOIN*)ptemp;
	return TRUE;
}


// [SSO 작업] - 추가
////////// 로그인을 요청함
BOOL CSV_ASK_LOGIN::Set(ASK_LOGININFO *pal, char* szAuthCookie, char* szDataCookie)
{
	int msglen = sizeof(ASK_LOGININFO) + pal->AuthCookieLen +  pal->DataCookieLen;
	char* ptemp = SetHeader(SV_ASK_LOGIN, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_LOGIN::Set() 메모리 할당 에러\n"); return FALSE;}
	pAL = (ASK_LOGININFO*)SmartCpy(&ptemp, pal, sizeof(ASK_LOGININFO));
	if(pAL->AuthCookieLen>0) AuthCookie = (char*)SmartCpy(&ptemp, szAuthCookie, pal->AuthCookieLen);
	if(pAL->DataCookieLen>0) DataCookie = (char*)SmartCpy(&ptemp, szDataCookie, pal->DataCookieLen);
	//Encode();
	return TRUE;
}
BOOL CSV_ASK_LOGIN::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_LOGIN::Get() 메모리 할당 에러\n"); return FALSE;}
	pAL = (ASK_LOGININFO*)Jump(&ptemp, sizeof(ASK_LOGININFO));

	if(pAL->AuthCookieLen>0) AuthCookie = (char*)Jump(&ptemp, pAL->AuthCookieLen);
	else AuthCookie = NULL;
	if(pAL->DataCookieLen>0) DataCookie = (char*)Jump(&ptemp, pAL->DataCookieLen);
	else DataCookie = NULL;

	return TRUE;
}

////////// 로그인을 거부함	[SSO 작업] - 교체
BOOL CSV_REFUSE_LOGIN::Set(int code, char* strmsg)
{
	int l_strmsg = 0;
	if(strmsg) l_strmsg = strlen(strmsg);

	int msglen = sizeof(*Code) + sizeof(*l_StrMsg) + l_strmsg;
	char* ptemp = SetHeader(SV_REFUSE_LOGIN, msglen);
	if(ptemp==NULL) {TRACE("CSV_REFUSE_LOGIN::Set() 메모리 할당 에러\n"); return FALSE;}
	Code = (int*)SmartCpy(&ptemp, &code, sizeof(*Code));
	l_StrMsg = (int*)SmartCpy(&ptemp, &l_strmsg, sizeof(*l_StrMsg));
	if(l_strmsg > 0) StrMsg = (char*)SmartCpy(&ptemp, strmsg, l_strmsg);
	return TRUE;
}
BOOL CSV_REFUSE_LOGIN::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_REFUSE_LOGIN::Get() 메모리 할당 에러\n"); return FALSE;}
	Code = (int*)Jump(&ptemp, sizeof(*Code));
	l_StrMsg = (int*)Jump(&ptemp, sizeof(*l_StrMsg));
	if(*l_StrMsg > 0) StrMsg = (char*)Jump(&ptemp, *l_StrMsg);
	else StrMsg = NULL;
	return TRUE;
}

////////// 로그인을 허용함
BOOL CSV_ACCEPT_LOGIN::Set(LOGININFO *pLI)
{
	int msglen = sizeof(LOGININFO);
	char* ptemp = SetHeader(SV_ACCEPT_LOGIN, msglen);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_LOGIN::Set() 메모리 할당 에러\n"); return FALSE;}
	LI = (LOGININFO*)SmartCpy(&ptemp, pLI, sizeof(LOGININFO));
	//Encode();
	return TRUE;
}
BOOL CSV_ACCEPT_LOGIN::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_LOGIN::Get() 메모리 할당 에러\n"); return FALSE;}
	LI = (LOGININFO*)Jump(&ptemp, sizeof(LOGININFO));
	return TRUE;
}
/*
////////// 로그인을 거부함
BOOL CSV_REFUSE_LOGIN::Set(int code)
{
	int msglen = sizeof(*Code);
	char* ptemp = SetHeader(SV_REFUSE_LOGIN, msglen);
	if(ptemp==NULL) {TRACE("CSV_REFUSE_LOGIN::Set() 메모리 할당 에러\n"); return FALSE;}
	memcpy(ptemp, &code, sizeof(*Code));
	Code = (int*)ptemp;
	return TRUE;
}
BOOL CSV_REFUSE_LOGIN::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_REFUSE_LOGIN::Get() 메모리 할당 에러\n"); return FALSE;}
	Code = (int*)ptemp;
	return TRUE;
}
*/
////////// 로그아웃을 요청
BOOL CSV_ASK_LOGOUT::Set(int unum, char *id)
{
	char strId[16]={0,};
	if(strlen(id) < 16) strcpy(strId, id);
	int msglen = sizeof(*UNum) + 16;
	char* ptemp = SetHeader(SV_ASK_LOGOUT, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_LOGOUT::Set() 메모리 할당 에러\n"); return FALSE;}
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ID = SmartCpy(&ptemp, strId, 16);
	return TRUE;
}
BOOL CSV_ASK_LOGOUT::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_LOGOUT::Get() 메모리 할당 에러\n"); return FALSE;}
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ID = ptemp;
	return TRUE;
}

////////// 로그아웃을 허용
BOOL CSV_ACCEPT_LOGOUT::Set()
{
	int msglen = 0;
	char* ptemp = SetHeader(SV_ACCEPT_LOGOUT, msglen);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_LOGOUT::Set() 메모리 할당 에러\n"); return FALSE;}
	return TRUE;
}
BOOL CSV_ACCEPT_LOGOUT::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_LOGOUT::Get() 메모리 할당 에러\n"); return FALSE;}
	return TRUE;
}

////////// 허트비트
BOOL CSV_HEARTBEAT::Set()
{
	int msglen = 0;
	char* ptemp = SetHeader(SV_HEARTBEAT, msglen);
	if(ptemp==NULL) {TRACE("CSV_HEARTBEAT::Set() 메모리 할당 에러\n"); return FALSE;}
	return TRUE;
}
BOOL CSV_HEARTBEAT::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_HEARTBEAT::Get() 메모리 할당 에러\n"); return FALSE;}
	return TRUE;
}

////////// 채널 입장을 요청
BOOL CSV_ASK_ENTERCHAN::Set(int cnum)
{
	int msglen = sizeof(*ChNum);
	char* ptemp = SetHeader(SV_ASK_ENTERCHAN, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_ENTERCHAN::Set() 메모리 할당 에러\n"); return FALSE;}
	ChNum = (int*)SmartCpy(&ptemp, &cnum, sizeof(*ChNum));
	return TRUE;
}
BOOL CSV_ASK_ENTERCHAN::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_ENTERCHAN::Get() 메모리 할당 에러\n"); return FALSE;}
	ChNum = (int*)Jump(&ptemp, sizeof(*ChNum));
	return TRUE;
}

////////// 채널 입장을 허가
BOOL CSV_ACCEPT_ENTERCHAN::Set(ENTERCHANINFO *pEC)
{
	int msglen = sizeof(ENTERCHANINFO);
	char* ptemp = SetHeader(SV_ACCEPT_ENTERCHAN, msglen);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_ENTERCHAN::Set() 메모리 할당 에러\n"); return FALSE;}
	EC = (ENTERCHANINFO*)SmartCpy(&ptemp, pEC, sizeof(ENTERCHANINFO));
	return TRUE;
}
BOOL CSV_ACCEPT_ENTERCHAN::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_ENTERCHAN::Get() 메모리 할당 에러\n"); return FALSE;}
	EC = (ENTERCHANINFO*)Jump(&ptemp, sizeof(ENTERCHANINFO));
	return TRUE;
}

////////// 채널 퇴장을 요청
BOOL CSV_ASK_OUTCHAN::Set()
{
	int msglen = 0;
	char* ptemp = SetHeader(SV_ASK_OUTCHAN, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_OUTCHAN::Set() 메모리 할당 에러\n"); return FALSE;}
	return TRUE;
}
BOOL CSV_ASK_OUTCHAN::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_OUTCHAN::Get() 메모리 할당 에러\n"); return FALSE;}
	return TRUE;
}

////////// 채널 퇴장을 허가
BOOL CSV_ACCEPT_OUTCHAN::Set()
{
	int msglen = 0;
	char* ptemp = SetHeader(SV_ACCEPT_OUTCHAN, msglen);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_OUTCHAN::Set() 메모리 할당 에러\n"); return FALSE;}
	return TRUE;
}
BOOL CSV_ACCEPT_OUTCHAN::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_OUTCHAN::Get() 메모리 할당 에러\n"); return FALSE;}
	return TRUE;
}

////////// 아이디 찾기 요청
BOOL CSV_ASK_FINDID::Set(char *id)
{
	char strId[16]={0,};
	if(strlen(id) < 16) strcpy(strId, id);
	int msglen = 16;
	char* ptemp = SetHeader(SV_ASK_FINDID, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_FINDID::Set() 메모리 할당 에러\n"); return FALSE;}
	ID = SmartCpy(&ptemp, strId, 16);
	return TRUE;
}
BOOL CSV_ASK_FINDID::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_FINDID::Get() 메모리 할당 에러\n"); return FALSE;}
	ID = Jump(&ptemp, 16);
	return TRUE;
}

////////// 아이디 찾기 결과
BOOL CSV_RESULT_FINDID::Set(RESULTFINDID *pRF)
{
	int msglen = sizeof(RESULTFINDID);
	char* ptemp = SetHeader(SV_RESULT_FINDID, msglen);
	if(ptemp==NULL) {TRACE("CSV_RESULT_FINDID::Set() 메모리 할당 에러\n"); return FALSE;}
	RF = (RESULTFINDID*)SmartCpy(&ptemp, pRF, sizeof(RESULTFINDID));
	return TRUE;
}
BOOL CSV_RESULT_FINDID::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_RESULT_FINDID::Get() 메모리 할당 에러\n"); return FALSE;}
	RF = (RESULTFINDID*)Jump(&ptemp, sizeof(RESULTFINDID));
	return TRUE;
}

////////// 대화내용 신고 요청
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
	if(ptemp==NULL) {TRACE("CSV_ASK_BADUSER::Set() 메모리 할당 에러\n"); return FALSE;}

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
	if(ptemp==NULL) {TRACE("CSV_ASK_BADUSER::Get() 메모리 할당 에러\n"); return FALSE;}
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

////////// 전체 채널 목록을 요구
BOOL CSV_ASK_ALLCHANINFO::Set()
{
	char* ptemp = SetHeader(SV_ASK_ALLCHANINFO, 0);
	if(ptemp==NULL) {TRACE("CSV_ASK_ALLCHANINFO::Set() 메모리 할당 에러\n"); return FALSE;}
	return TRUE;
}
BOOL CSV_ASK_ALLCHANINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_ALLCHANINFO::Get() 메모리 할당 에러\n"); return FALSE;}
	return TRUE;
}

////////// 전체 채널 목록을 보내줌
BOOL CSV_ALLCHANINFO::Set(int totnum, CHANNELINFO *ppCI)
{
	int msglen = sizeof(*TotNum) + sizeof(CHANNELINFO)*totnum;
	char* ptemp = SetHeader(SV_ALLCHANINFO, msglen);
	if(ptemp==NULL) {TRACE("CSV_ALLCHANINFO::Set() 메모리 할당 에러\n"); return FALSE;}
	TotNum = (int*)SmartCpy(&ptemp, &totnum, sizeof(*TotNum));
	ArrayCI = (CHANNELINFO*)SmartCpy(&ptemp, ppCI, sizeof(CHANNELINFO)*totnum);
	return TRUE;
}
BOOL CSV_ALLCHANINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ALLCHANINFO::Get() 메모리 할당 에러\n"); return FALSE;}
	TotNum   = (int*)Jump(&ptemp, sizeof(*TotNum));
	ArrayCI = (CHANNELINFO*)ptemp;
	return TRUE;
}

////////// 전체 사용자 목록을 요구
BOOL CSV_ASK_ALLUSERINFO::Set()
{
	char* ptemp = SetHeader(SV_ASK_ALLUSERINFO, 0);
	if(ptemp==NULL) {TRACE("CSV_ASK_ALLUSERINFO::Set() 메모리 할당 에러\n"); return FALSE;}
	return TRUE;
}
BOOL CSV_ASK_ALLUSERINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_ALLUSERINFO::Get() 메모리 할당 에러\n"); return FALSE;}
	return TRUE;
}

////////// 전체 사용자 목록을 보내줌
BOOL CSV_ALLUSERINFO::Set(int totnum, SMALLUSERINFO *ppSUI)
{
	int msglen = sizeof(*TotNum) + sizeof(SMALLUSERINFO)*totnum;
	char* ptemp = SetHeader(SV_ALLUSERINFO, msglen);
	if(ptemp==NULL) {TRACE("CSV_ALLUSERINFO::Set() 메모리 할당 에러\n"); return FALSE;}
	TotNum = (int*)SmartCpy(&ptemp, &totnum, sizeof(*TotNum));
	ArraySUI = (SMALLUSERINFO*)SmartCpy(&ptemp, ppSUI, sizeof(SMALLUSERINFO)*totnum);
	return TRUE;
}
BOOL CSV_ALLUSERINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ALLUSERINFO::Get() 메모리 할당 에러\n"); return FALSE;}
	TotNum   = (int*)Jump(&ptemp, sizeof(*TotNum));
	ArraySUI = (SMALLUSERINFO*)ptemp;
	return TRUE;
}

////////// 특정 사용자의 상세 정보를 요구
BOOL CSV_ASK_DETAILUSERINFO::Set(int unum, char *id)
{
	char strId[16]={0,};
	if(strlen(id) < 16) strcpy(strId, id);
	int msglen = sizeof(*UNum) + 16;
	char* ptemp = SetHeader(SV_ASK_DETAILUSERINFO, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_DETAILUSERINFO::Set() 메모리 할당 에러\n"); return FALSE;}
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ID = SmartCpy(&ptemp, strId, 16);
	return TRUE;
}
BOOL CSV_ASK_DETAILUSERINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_DETAILUSERINFO::Get() 메모리 할당 에러\n"); return FALSE;}
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ID = ptemp;
	return TRUE;
}

////////// 특정 사용자의 상세 정보를 보내줌
BOOL CSV_DETAILUSERINFO::Set(USERINFO *pUI)
{
	int msglen = sizeof(USERINFO);
	char* ptemp = SetHeader(SV_DETAILUSERINFO, msglen);
	if(ptemp==NULL) {TRACE("CSV_DETAILUSERINFO::Set() 메모리 할당 에러\n"); return FALSE;}
	memcpy(ptemp, pUI, msglen);
	UI = (USERINFO*)ptemp;
	//Encode();
	return TRUE;
}
BOOL CSV_DETAILUSERINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_DETAILUSERINFO::Get() 메모리 할당 에러\n"); return FALSE;}
	UI = (USERINFO*)ptemp;
	return TRUE;
}

////////// 특정 사용자의 정보를 요구
BOOL CSV_ASK_USERINFO::Set(int unum, char *id)
{
	char strId[16]={0,};
	if(strlen(id) < 16) strcpy(strId, id);
	int msglen = sizeof(*UNum) + 16;
	char* ptemp = SetHeader(SV_ASK_USERINFO, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_USERINFO::Set() 메모리 할당 에러\n"); return FALSE;}
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ID = SmartCpy(&ptemp, strId, 16);
	return TRUE;
}
BOOL CSV_ASK_USERINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_USERINFO::Get() 메모리 할당 에러\n"); return FALSE;}
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ID = ptemp;
	return TRUE;
}

////////// 특정 사용자의 정보를 보내줌
BOOL CSV_USERINFO::Set(USERINFO *pUI)
{
	int msglen = sizeof(USERINFO);
	char* ptemp = SetHeader(SV_USERINFO, msglen);
	if(ptemp==NULL) {TRACE("CSV_USERINFO::Set() 메모리 할당 에러\n"); return FALSE;}
	memcpy(ptemp, pUI, msglen);
	UI = (USERINFO*)ptemp;
	//Encode();
	return TRUE;
}
BOOL CSV_USERINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_USERINFO::Get() 메모리 할당 에러\n"); return FALSE;}
	UI = (USERINFO*)ptemp;
	return TRUE;
}

////////// 새로운 사용자가 접속하였음
BOOL CSV_NEWUSER::Set(SMALLUSERINFO *pSUI)
{
	int msglen = sizeof(SMALLUSERINFO);
	char* ptemp = SetHeader(SV_NEWUSER, msglen);
	if(ptemp==NULL) {TRACE("CSV_NEWUSER::Set() 메모리 할당 에러\n"); return FALSE;}
	SUI = (SMALLUSERINFO*)SmartCpy(&ptemp, pSUI, sizeof(SMALLUSERINFO));
	return TRUE;
}
BOOL CSV_NEWUSER::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_NEWUSER::Get() 메모리 할당 에러\n"); return FALSE;}
	SUI = (SMALLUSERINFO*)ptemp;
	return TRUE;
}

////////// 사용자가 접속을 종료하였음
BOOL CSV_OUTUSER::Set(SMALLUSERINFO *pSUI)
{
	int msglen = sizeof(SMALLUSERINFO);
	char* ptemp = SetHeader(SV_OUTUSER, msglen);
	if(ptemp==NULL) {TRACE("CSV_OUTUSER::Set() 메모리 할당 에러\n"); return FALSE;}
	SUI = (SMALLUSERINFO*)SmartCpy(&ptemp, pSUI, sizeof(SMALLUSERINFO));
	return TRUE;
}
BOOL CSV_OUTUSER::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_OUTUSER::Get() 메모리 할당 에러\n"); return FALSE;}
	SUI = (SMALLUSERINFO*)ptemp;
	return TRUE;
}

////////// 다른 사용자의 접속정보가 변했음
BOOL CSV_CHANGEUSER::Set(SMALLUSERINFO *pSUI)
{
	int msglen = sizeof(SMALLUSERINFO);
	char* ptemp = SetHeader(SV_CHANGEUSER, msglen);
	if(ptemp==NULL) {TRACE("CSV_CHANGEUSER::Set() 메모리 할당 에러\n"); return FALSE;}
	SUI = (SMALLUSERINFO*)SmartCpy(&ptemp, pSUI, sizeof(SMALLUSERINFO));
	return TRUE;
}
BOOL CSV_CHANGEUSER::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_CHANGEUSER::Get() 메모리 할당 에러\n"); return FALSE;}
	SUI = (SMALLUSERINFO*)ptemp;
	return TRUE;
}


////////// 전체 방 목록을 요구
BOOL CSV_ASK_ALLROOMINFO::Set()
{
	char* ptemp = SetHeader(SV_ASK_ALLROOMINFO, 0);
	if(ptemp==NULL) {TRACE("CSV_ASK_ALLROOMINFO::Set() 메모리 할당 에러\n"); return FALSE;}
	return TRUE;
}
BOOL CSV_ASK_ALLROOMINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_ALLROOMINFO::Get() 메모리 할당 에러\n"); return FALSE;}
	return TRUE;
}

////////// 전체 방 목록을 보내줌
BOOL CSV_ALLROOMINFO::Set(int totnum, ROOMINFO *ppRI)
{
	int msglen = sizeof(*TotNum) + sizeof(ROOMINFO)*totnum;
	char* ptemp = SetHeader(SV_ALLROOMINFO, msglen);
	if(ptemp==NULL) {TRACE("CSV_ALLROOMINFO::Set() 메모리 할당 에러\n"); return FALSE;}
	TotNum = (int*)SmartCpy(&ptemp, &totnum, sizeof(*TotNum));
	ArrayRI = (ROOMINFO*)SmartCpy(&ptemp, ppRI, sizeof(ROOMINFO)*totnum);
	return TRUE;
}
BOOL CSV_ALLROOMINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ALLROOMINFO::Get() 메모리 할당 에러\n"); return FALSE;}
	TotNum   = (int*)Jump(&ptemp, sizeof(*TotNum));
	ArrayRI = (ROOMINFO*)ptemp;
	return TRUE;
}

////////// 방 정보를 요구
BOOL CSV_ASK_ROOMINFO::Set(int roomnum)
{
	int msglen = sizeof(*RoomNum);
	char* ptemp = SetHeader(SV_ASK_ROOMINFO, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_ROOMINFO::Set() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &roomnum, sizeof(*RoomNum));
	return TRUE;
}
BOOL CSV_ASK_ROOMINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_ROOMINFO::Get() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)ptemp;
	return TRUE;
}

////////// 방정보
BOOL CSV_ROOMINFO::Set(int errcode, GAMEINFO *pGI)
{
	int msglen = sizeof(*ErrCode) + sizeof(GAMEINFO);
	char* ptemp = SetHeader(SV_ROOMINFO, msglen);
	if(ptemp==NULL) {TRACE("CSV_ROOMINFO::Set() 메모리 할당 에러\n"); return FALSE;}
	ErrCode = (int*)SmartCpy(&ptemp, &errcode, sizeof(*ErrCode));
	GI = (GAMEINFO*)SmartCpy(&ptemp, pGI, sizeof(GAMEINFO));
	//Encode();
	return TRUE;
}
BOOL CSV_ROOMINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ROOMINFO::Get() 메모리 할당 에러\n"); return FALSE;}
	ErrCode = (int*)Jump(&ptemp, sizeof(*ErrCode));
	GI = (GAMEINFO*)ptemp;
	return TRUE;
}

////////// 캐릭터 바꾸기를 요구
BOOL CSV_ASK_CHANGECHAR::Set(int unum, char *id, int charnum)
{
	char strID[16];
	strncpy(strID, id, 15);
	int msglen = sizeof(*UNum) + 16 + sizeof(*CharNum);
	char* ptemp = SetHeader(SV_ASK_CHANGECHAR, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_CHANGECHAR::Set() 메모리 할당 에러\n"); return FALSE;}
	UNum = (int*) SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ID = SmartCpy(&ptemp, strID, 16);
	CharNum = (int*) SmartCpy(&ptemp, &charnum, sizeof(*CharNum));
	return TRUE;
}
BOOL CSV_ASK_CHANGECHAR::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_CHANGECHAR::Get() 메모리 할당 에러\n"); return FALSE;}
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ID = Jump(&ptemp, 16);
	CharNum = (int*)Jump(&ptemp, sizeof(*CharNum));
	return TRUE;
}

////////// 캐릭터 바꾸기를 허락
BOOL CSV_ACCEPT_CHANGECHAR::Set(int charnum)
{
	int msglen = sizeof(*CharNum);
	char* ptemp = SetHeader(SV_ACCEPT_CHANGECHAR, msglen);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_CHANGECHAR::Set() 메모리 할당 에러\n"); return FALSE;}
	CharNum = (int*) SmartCpy(&ptemp, &charnum, sizeof(*CharNum));
	return TRUE;
}
BOOL CSV_ACCEPT_CHANGECHAR::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_CHANGECHAR::Get() 메모리 할당 에러\n"); return FALSE;}
	CharNum = (int*)Jump(&ptemp, sizeof(*CharNum));
	return TRUE;
}

////////// 방 개설 요구
BOOL CSV_ASK_CREATEROOM::Set(ROOMINFO *pRI, UINT port)
{
	int msglen = sizeof(ROOMINFO) + sizeof(*Port);
	char* ptemp = SetHeader(SV_ASK_CREATEROOM, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_CREATEROOM::Set() 메모리 할당 에러\n"); return FALSE;}
	RI = (ROOMINFO*)SmartCpy(&ptemp, pRI, sizeof(ROOMINFO));
	Port = (UINT*)SmartCpy(&ptemp, &port, sizeof(*Port));
	return TRUE;
}
BOOL CSV_ASK_CREATEROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_CREATEROOM::Get() 메모리 할당 에러\n"); return FALSE;}
	RI = (ROOMINFO*)Jump(&ptemp, sizeof(ROOMINFO));
	Port = (UINT*)Jump(&ptemp, sizeof(*Port));
	return TRUE;
}

////////// 방 개설 거부
BOOL CSV_REFUSE_CREATEROOM::Set(int code)
{
	int msglen = sizeof(*Code);
	char* ptemp = SetHeader(SV_REFUSE_CREATEROOM, msglen);
	if(ptemp==NULL) {TRACE("CSV_REFUSE_CREATEROOM::Set() 메모리 할당 에러\n"); return FALSE;}
	memcpy(ptemp, &code, sizeof(*Code));
	return TRUE;
}
BOOL CSV_REFUSE_CREATEROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_REFUSE_CREATEROOM::Get() 메모리 할당 에러\n"); return FALSE;}
	Code = (int*)ptemp;
	return TRUE;
}

////////// 방 개설 허용
BOOL CSV_ACCEPT_CREATEROOM::Set(ROOMINFO *pRI)
{
	int msglen = sizeof(ROOMINFO);
	char* ptemp = SetHeader(SV_ACCEPT_CREATEROOM, msglen);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_CREATEROOM::Set() 메모리 할당 에러\n"); return FALSE;}
	memcpy(ptemp, pRI, msglen);
	RI = (ROOMINFO*)ptemp;
	return TRUE;
}
BOOL CSV_ACCEPT_CREATEROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_CREATEROOM::Get() 메모리 할당 에러\n"); return FALSE;}
	RI = (ROOMINFO*)ptemp;
	return TRUE;
}

////////// 새로운 방이 개설되었음
BOOL CSV_NEWROOM::Set(ROOMINFO *pRI)
{
	int msglen = sizeof(ROOMINFO);
	char* ptemp = SetHeader(SV_NEWROOM, msglen);
	if(ptemp==NULL) {TRACE("CSV_NEWROOM::Set() 메모리 할당 에러\n"); return FALSE;}
	memcpy(ptemp, pRI, msglen);
	RI = (ROOMINFO*)ptemp;
	return TRUE;
}
BOOL CSV_NEWROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_NEWROOM::Get() 메모리 할당 에러\n"); return FALSE;}
	RI = (ROOMINFO*)ptemp;
	return TRUE;
}

////////// 방이 사라졌음
BOOL CSV_OUTROOM::Set(SMALLROOMINFO *pSRI)
{
	int msglen = sizeof(SMALLROOMINFO);
	char* ptemp = SetHeader(SV_OUTROOM, msglen);
	if(ptemp==NULL) {TRACE("CSV_OUTROOM::Set() 메모리 할당 에러\n"); return FALSE;}
	memcpy(ptemp, pSRI, msglen);
	SRI = (SMALLROOMINFO*)ptemp;
	return TRUE;
}
BOOL CSV_OUTROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_OUTROOM::Get() 메모리 할당 에러\n"); return FALSE;}
	SRI = (SMALLROOMINFO*)ptemp;
	return TRUE;
}

////////// 방 정보가 변했음
BOOL CSV_CHANGEROOM::Set(SMALLROOMINFO *pSRI)
{
	int msglen = sizeof(SMALLROOMINFO);
	char* ptemp = SetHeader(SV_CHANGEROOM, msglen);
	if(ptemp==NULL) {TRACE("CSV_CHANGEROOM::Set() 메모리 할당 에러\n"); return FALSE;}
	memcpy(ptemp, pSRI, msglen);
	SRI = (SMALLROOMINFO*)ptemp;
	return TRUE;
}
BOOL CSV_CHANGEROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_CHANGEROOM::Get() 메모리 할당 에러\n"); return FALSE;}
	SRI = (SMALLROOMINFO*)ptemp;
	return TRUE;
}

////////// 방장이 바뀌었음
BOOL CSV_CHANGEROOMMASTER::Set(int rnum, int unum, char *id)
{
	char strid[16]={0,};
	if(id != NULL) strncpy(strid, id, 15);
	int msglen = sizeof(*RoomNum) + sizeof(*UNum) + 16;
	char* ptemp = SetHeader(SV_CHANGEROOMMASTER, msglen);
	if(ptemp==NULL) {TRACE("CSV_CHANGEROOMMASTER::Set() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ID = SmartCpy(&ptemp, strid, 16);
	return TRUE;
}
BOOL CSV_CHANGEROOMMASTER::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_CHANGEROOMMASTER::Get() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ID = Jump(&ptemp, 16);
	return TRUE;
}

////////// 특정 방에 입장을 요구
BOOL CSV_ASK_ENTERROOM::Set(int rnum, int unum, int sndKind, char *id, char *pass)// ### [사운드 추가 작업] ###
{
	int msglen = sizeof(*RoomNum) + sizeof(*UNum) + sizeof(*SndFxKind)+ 16 + 5;
	char strid[16]={0,};
	char strpass[5]={0,};
	if(id != NULL) strncpy(strid, id, 15);
	if(pass != NULL) strncpy(strpass, pass, 4);
	char* ptemp = SetHeader(SV_ASK_ENTERROOM, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_ENTERROOM::Set() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	SndFxKind = (int*)SmartCpy(&ptemp, &sndKind, sizeof(*SndFxKind)); // 추가
	ID = SmartCpy(&ptemp, strid, 16);
	Pass = SmartCpy(&ptemp, strpass, 5);
	return TRUE;
}
BOOL CSV_ASK_ENTERROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_ENTERROOM::Get() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	SndFxKind = (int*)Jump(&ptemp, sizeof(*SndFxKind));// ### [사운드 추가 작업] ###
	ID = Jump(&ptemp, 16);
	Pass = Jump(&ptemp, 5);
	return TRUE;
}

////////// 방에 입장하기를 허가함
BOOL CSV_ACCEPT_ENTERROOM::Set(ENTERROOMINFO *pEI)//, int fieldnum, BLOCKFIELDINFO *pbf) 
{
	int msglen = sizeof(ENTERROOMINFO);// + sizeof(*FieldNum) + sizeof(BLOCKFIELDINFO)*fieldnum;
	char* ptemp = SetHeader(SV_ACCEPT_ENTERROOM, msglen);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_ENTERROOM::Set() 메모리 할당 에러\n"); return FALSE;}
	EI = (ENTERROOMINFO*)SmartCpy(&ptemp, pEI, sizeof(ENTERROOMINFO));
	//FieldNum = (int*)SmartCpy(&ptemp, &fieldnum, sizeof(*FieldNum));
	//pBF = (BLOCKFIELDINFO*)SmartCpy(&ptemp, pbf, sizeof(BLOCKFIELDINFO)*fieldnum);
	//Encode();
	return TRUE;
}
BOOL CSV_ACCEPT_ENTERROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_ENTERROOM::Get() 메모리 할당 에러\n"); return FALSE;}
	EI = (ENTERROOMINFO*)Jump(&ptemp, sizeof(ENTERROOMINFO));
	//FieldNum = (int*)Jump(&ptemp, sizeof(*FieldNum));
	//if(*FieldNum > 0)
	//	pBF = (BLOCKFIELDINFO*)Jump(&ptemp, sizeof(BLOCKFIELDINFO)*(*FieldNum));
	//else 
	//	pBF = NULL;
	return TRUE;
}

////////// 입장요청을 거부함
BOOL CSV_REFUSE_ENTERROOM::Set(int code) 
{
	int msglen = sizeof(*Code);
	char* ptemp = SetHeader(SV_REFUSE_ENTERROOM, msglen);
	if(ptemp==NULL) {TRACE("CSV_REFUSE_ENTERROOM::Set() 메모리 할당 에러\n"); return FALSE;}
	Code = (int*)SmartCpy(&ptemp, &code, sizeof(*Code));
	return TRUE;
}
BOOL CSV_REFUSE_ENTERROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_REFUSE_ENTERROOM::Get() 메모리 할당 에러\n"); return FALSE;}
	Code = (int*)ptemp;
	return TRUE;
}

// ### [ 중복된 IP가 ] ###
////////// 방에 사용자가 입장하였음
BOOL CSV_USERENTERROOM::Set(int rnum, USERINFO *pUI, IPARRINDEX *pIPA, int sndKind)// ### [사운드 추가 작업] ###)
{
	int msglen = sizeof(*RoomNum) + sizeof(USERINFO) + sizeof(IPARRINDEX) + sizeof(*SndFxKind); // 추가
	char* ptemp = SetHeader(SV_USERENTERROOM, msglen);
	if(ptemp==NULL) {TRACE("CSV_USERENTERROOM::Set() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	UI = (USERINFO*)SmartCpy(&ptemp, pUI, sizeof(USERINFO));
	IPA = (IPARRINDEX*)SmartCpy(&ptemp, pIPA, sizeof(IPARRINDEX));
	SndFxKind = (int*)SmartCpy(&ptemp, &sndKind, sizeof(*SndFxKind)); // 추가
	//Encode();
	return TRUE;
}
BOOL CSV_USERENTERROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_USERENTERROOM::Get() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UI = (USERINFO*)Jump(&ptemp, sizeof(USERINFO));
	IPA = (IPARRINDEX*)Jump(&ptemp, sizeof(IPARRINDEX));
	SndFxKind = (int*)Jump(&ptemp, sizeof(*SndFxKind));// ### [사운드 추가 작업] ###
	return TRUE;
}

// ### [ 중복된 IP가 ] ###
////////// 사용자가 퇴실하였음
BOOL CSV_USEROUTROOM::Set(int rnum, int unum, char *id, IPARRINDEX *pIPA)
{
	char strid[16]={0,};
	if(id != NULL) strncpy(strid, id, 15);
	int msglen = sizeof(*RoomNum) + sizeof(*UNum) + 16 +sizeof(IPARRINDEX);
	char* ptemp = SetHeader(SV_USEROUTROOM, msglen);
	if(ptemp==NULL) {TRACE("CSV_USEROUTROOM::Set() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ID = SmartCpy(&ptemp, strid, 16);
	IPA = (IPARRINDEX*)SmartCpy(&ptemp, pIPA, sizeof(IPARRINDEX));
	return TRUE;
}
BOOL CSV_USEROUTROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_USEROUTROOM::Get() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ID = Jump(&ptemp, 16);
	IPA = (IPARRINDEX*)Jump(&ptemp, sizeof(IPARRINDEX));
	return TRUE;
}

////////// 퇴실요청
BOOL CSV_ASK_OUTROOM::Set(int rnum, int unum, char *id)
{
	char strid[16]={0,};
	if(id != NULL) strncpy(strid, id, 15);
	int msglen = sizeof(*RoomNum) + sizeof(*UNum) + 16;
	char* ptemp = SetHeader(SV_ASK_OUTROOM, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_OUTROOM::Set() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ID = SmartCpy(&ptemp, strid, 16);
	return TRUE;
}
BOOL CSV_ASK_OUTROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_OUTROOM::Get() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ID = Jump(&ptemp, 16);
	return TRUE;
}

////////// 퇴실요청 허가
BOOL CSV_ACCEPT_OUTROOM::Set()
{
	int msglen = 0;
	char* ptemp = SetHeader(SV_ACCEPT_OUTROOM, msglen);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_OUTROOM::Set() 메모리 할당 에러\n"); return FALSE;}
	return TRUE;
}
BOOL CSV_ACCEPT_OUTROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_OUTROOM::Get() 메모리 할당 에러\n"); return FALSE;}
	return TRUE;
}

////////// 나의 사용자 정보가 변경됨
BOOL CSV_CHANGEMYINFO::Set(CHANGEMYINFO *pcmi)
{
	int msglen = sizeof(CHANGEMYINFO);
	char* ptemp = SetHeader(SV_CHANGEMYINFO, msglen);
	if(ptemp==NULL) {TRACE("CSV_CHANGEMYINFO::Set() 메모리 할당 에러\n"); return FALSE;}
	pCMI = (CHANGEMYINFO*)SmartCpy(&ptemp, pcmi, sizeof(CHANGEMYINFO));
	return TRUE;
}
BOOL CSV_CHANGEMYINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_CHANGEMYINFO::Get() 메모리 할당 에러\n"); return FALSE;}
	pCMI = (CHANGEMYINFO*)Jump(&ptemp, sizeof(CHANGEMYINFO));
	return TRUE;
}

////////// 대기실 보기 초대창 오픈 상태 알림
BOOL CSV_LOBYOPEN::Set(BOOL blobyopen)
{
	int msglen = sizeof(*bLobyOpen);
	char* ptemp = SetHeader(SV_LOBYOPEN, msglen);
	if(ptemp==NULL) {TRACE("CSV_LOBYOPEN::Set() 메모리 할당 에러\n"); return FALSE;}
	bLobyOpen = (BOOL*)SmartCpy(&ptemp, &blobyopen, sizeof(*bLobyOpen));
	return TRUE;
}
BOOL CSV_LOBYOPEN::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_LOBYOPEN::Get() 메모리 할당 에러\n"); return FALSE;}
	bLobyOpen = (BOOL*)Jump(&ptemp, sizeof(*bLobyOpen));
	return TRUE;
}

////////// 초청 메세지를 보냄
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
	if(ptemp==NULL) {TRACE("CSV_ASK_INVITE::Set() 메모리 할당 에러\n"); return FALSE;}

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
	if(ptemp==NULL) {TRACE("CSV_ASK_INVITE::Get() 메모리 할당 에러\n"); return FALSE;}
	TarUNum = (int*)Jump(&ptemp, sizeof(*TarUNum));
	TarID = Jump(&ptemp, 16);
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	RoomPass = Jump(&ptemp, 5);
	MyUI = (USERINFO*)Jump(&ptemp, sizeof(USERINFO));
	l_StrMsg = (int*)Jump(&ptemp, sizeof(*l_StrMsg));
	StrMsg = ptemp;
	return TRUE;
}

////////// 초청 메세지를 거부
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
	if(ptemp==NULL) {TRACE("CSV_REFUSE_INVITE::Set() 메모리 할당 에러\n"); return FALSE;}

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
	if(ptemp==NULL) {TRACE("CSV_REFUSE_INVITE::Get() 메모리 할당 에러\n"); return FALSE;}
	TarUNum = (int*)Jump(&ptemp, sizeof(*TarUNum));
	TarID = Jump(&ptemp, 16);
	MyID = Jump(&ptemp, 16);
	l_StrMsg = (int*)Jump(&ptemp, sizeof(*l_StrMsg));
	StrMsg = ptemp;
	return TRUE;
}

////////// 추방 투표 요청
BOOL CSV_ASK_BANISHVOTE::Set(BANISHVOTEINFO *pbv)
{
	int msglen = sizeof(BANISHVOTEINFO);
	char* ptemp = SetHeader(SV_ASK_BANISHVOTE, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_BANISHVOTE::Set() 메모리 할당 에러\n"); return FALSE;}
	pBV = (BANISHVOTEINFO*)SmartCpy(&ptemp, pbv, sizeof(BANISHVOTEINFO));
	return TRUE;
}
BOOL CSV_ASK_BANISHVOTE::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_BANISHVOTE::Get() 메모리 할당 에러\n"); return FALSE;}
	pBV = (BANISHVOTEINFO*)Jump(&ptemp, sizeof(BANISHVOTEINFO));
	return TRUE;
}

////////// 나의 추방 투표 결과
BOOL CSV_MYBANISHVOTE::Set(BOOL bagree)
{
	int msglen = sizeof(*bAgree);
	char* ptemp = SetHeader(SV_MYBANISHVOTE, msglen);
	if(ptemp==NULL) {TRACE("CSV_MYBANISHVOTE::Set() 메모리 할당 에러\n"); return FALSE;}
	bAgree = (BOOL*)SmartCpy(&ptemp, &bagree, sizeof(*bAgree));
	return TRUE;
}
BOOL CSV_MYBANISHVOTE::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_MYBANISHVOTE::Get() 메모리 할당 에러\n"); return FALSE;}
	bAgree = (BOOL*)Jump(&ptemp, sizeof(*bAgree));
	return TRUE;
}

////////// 비정상적인 퇴장을 요청
BOOL CSV_ABNORMALOUTROOM::Set()
{
	int msglen = 0;
	char* ptemp = SetHeader(SV_ABNORMALOUTROOM, msglen);
	if(ptemp==NULL) {TRACE("CSV_ABNORMALOUTROOM::Set() 메모리 할당 에러\n"); return FALSE;}
	return TRUE;
}
BOOL CSV_ABNORMALOUTROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ABNORMALOUTROOM::Get() 메모리 할당 에러\n"); return FALSE;}
	return TRUE;
}



////////// 강제 추방 요청	[게임 아이템 작업]
BOOL CSV_ASK_FORCEBANISH::Set(char *id)
{
	char strId[16]={0,};
	if(strlen(id) < 16) strcpy(strId, id);
	int msglen = 16;
	char* ptemp = SetHeader(SV_ASK_FORCEBANISH, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_FORCEBANISH::Set() 메모리 할당 에러\n"); return FALSE;}
	ID = SmartCpy(&ptemp, strId, 16);
	return TRUE;
}
BOOL CSV_ASK_FORCEBANISH::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_FORCEBANISH::Get() 메모리 할당 에러\n"); return FALSE;}
	ID = Jump(&ptemp, 16);
	return TRUE;
}

///////////////////////////////////////// [게임 아이템 작업] /////////////////////////////////////////

////////// 게임 아이템 목록을 요청
BOOL CSV_ASK_GAMEITEMLIST::Set()
{
	int msglen = 0;
	char* ptemp = SetHeader(SV_ASK_GAMEITEMLIST, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_GAMEITEMLIST::Set() 메모리 할당 에러\n"); return FALSE;}
	return TRUE;
}
BOOL CSV_ASK_GAMEITEMLIST::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_GAMEITEMLIST::Get() 메모리 할당 에러\n"); return FALSE;}
	return TRUE;
}

////////// 게임 아이템 목록을 보내줌
BOOL CSV_GAMEITEMLIST::Set(GAMEITEM_INFO *pin, GAMEITEM *ppGI, LPCTSTR strmsg1, LPCTSTR strmsg2)
{
	int msglen = sizeof(GAMEITEM_INFO) + (sizeof(GAMEITEM) * pin->TotNum) + pin->StrMsgLen1 + pin->StrMsgLen2;
	char* ptemp = SetHeader(SV_GAMEITEMLIST, msglen);
	if(ptemp==NULL) {TRACE("CSV_GAMEITEMLIST::Set() 메모리 할당 에러\n"); return FALSE;}
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
	if(ptemp==NULL) {TRACE("CSV_GAMEITEMLIST::Get() 메모리 할당 에러\n"); return FALSE;}
	pIN  = (GAMEITEM_INFO*)Jump(&ptemp, sizeof(GAMEITEM_INFO));
	if(pIN->TotNum > 0) ArrayGI = (GAMEITEM*)Jump(&ptemp, sizeof(GAMEITEM) * pIN->TotNum);
	else ArrayGI = NULL;
	if(pIN->StrMsgLen1 > 0) StrMsg1 = Jump(&ptemp, pIN->StrMsgLen1);
	else StrMsg1 = NULL;
	if(pIN->StrMsgLen2 > 0) StrMsg2 = Jump(&ptemp, pIN->StrMsgLen2);
	else StrMsg2 = NULL;
	return TRUE;
}

////////// 게임 아이템 사용 정보
BOOL CSV_USEGAMEITEM::Set(USEGAMEITEMINFO *pug)
{
	int msglen = sizeof(USEGAMEITEMINFO);
	char* ptemp = SetHeader(SV_USEGAMEITEM, msglen);
	if(ptemp==NULL) {TRACE("CSV_USEGAMEITEM::Set() 메모리 할당 에러\n"); return FALSE;}
	pUG = (USEGAMEITEMINFO*)SmartCpy(&ptemp, pug, sizeof(USEGAMEITEMINFO));
	//Encode();
	return TRUE;
}
BOOL CSV_USEGAMEITEM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_USEGAMEITEM::Get() 메모리 할당 에러\n"); return FALSE;}
	pUG = (USEGAMEITEMINFO*)ptemp;
	return TRUE;
}

////////// 게임 아이템 구입을 물어봄
BOOL CSV_BUYGAMEITEM::Set(BUYGAMEITEMINFO *pbi, LPCTSTR strmsg)
{
	int msglen = sizeof(BUYGAMEITEMINFO) + pbi->StrMsgLen;
	char* ptemp = SetHeader(SV_BUYGAMEITEM, msglen);
	if(ptemp==NULL) {TRACE("CSV_BUYGAMEITEM::Set() 메모리 할당 에러\n"); return FALSE;}
	pBI = (BUYGAMEITEMINFO*)SmartCpy(&ptemp, pbi, sizeof(BUYGAMEITEMINFO));
	if(pbi->StrMsgLen > 0) StrMsg = (char*)SmartCpy(&ptemp, strmsg, pbi->StrMsgLen);
	else StrMsg = NULL;
	return TRUE;
}
BOOL CSV_BUYGAMEITEM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_BUYGAMEITEM::Get() 메모리 할당 에러\n"); return FALSE;}
	pBI = (BUYGAMEITEMINFO*)Jump(&ptemp, sizeof(BUYGAMEITEMINFO));
	if(pBI->StrMsgLen > 0) StrMsg = ptemp;
	else StrMsg = NULL;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////








////////// 게임이 시작되었음
BOOL CSV_STARTGAME::Set(int roomnum)
{
	int msglen = sizeof(*RoomNum);
	char* ptemp = SetHeader(SV_STARTGAME, msglen);
	if(ptemp==NULL) {TRACE("CSV_STARTGAME::Set() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &roomnum, sizeof(*RoomNum));
	return TRUE;
}
BOOL CSV_STARTGAME::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_STARTGAME::Get() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	return TRUE;
}

////////// 게임종료 결과 통보
BOOL CSV_GAMEOVERRESULT::Set(GAMEOVERRESULT *pGOR)
{
	int msglen =  sizeof(GAMEOVERRESULT);
	char* ptemp = SetHeader(SV_GAMEOVERRESULT, msglen);
	if(ptemp==NULL) {TRACE("CSV_GAMEOVERRESULT::Set() 메모리 할당 에러\n"); return FALSE;}
	//TotNum = (int*)SmartCpy(&ptemp, &totnum, sizeof(*TotNum));
	GOR = (GAMEOVERRESULT*)SmartCpy(&ptemp, pGOR, sizeof(GAMEOVERRESULT));
	//Encode();
	return TRUE;
}
BOOL CSV_GAMEOVERRESULT::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_GAMEOVERRESULT::Get() 메모리 할당 에러\n"); return FALSE;}
	//TotNum = (int*)Jump(&ptemp, sizeof(*TotNum));
	GOR = (GAMEOVERRESULT*)Jump(&ptemp, sizeof(GAMEOVERRESULT) );
	return TRUE;
}

////////// 서버 알림 메세지
BOOL CSV_SERVERMSG::Set(int kind, char *chat, BOOL bcolorassigned, COLORREF color)
{
	CString szChat;
	szChat.Format("%s", chat);
	short l_strchat = (short) szChat.GetLength();
	int msglen = sizeof(*Kind) + sizeof(l_strchat) + l_strchat + sizeof(*bColorAssigned) + sizeof(*Color);

	char* ptemp = SetHeader(SV_SERVERMSG, msglen);
	if(ptemp==NULL) {TRACE("CSV_SERVERMSG::Set() 메모리 할당 에러\n"); return FALSE;}

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
	if(ptemp==NULL) {TRACE("CSV_SERVERMSG::Get() 메모리 할당 에러\n"); return FALSE;}
	Kind      = (int*)Jump(&ptemp, sizeof(*Kind));
	l_StrChat = (short*)Jump(&ptemp, sizeof(*l_StrChat));
	StrChat = Jump(&ptemp, *l_StrChat);
	bColorAssigned = (BOOL*)Jump(&ptemp, sizeof(*bColorAssigned));
	Color = (COLORREF*)Jump(&ptemp, sizeof(*Color));
	return TRUE;
}

////////// 귓말 허용 상태 변경
BOOL CSV_NOWHISPER::Set(BOOL bnowhisper)
{
	int msglen = sizeof(*bNoWhisper);
	char* ptemp = SetHeader(SV_NOWHISPER, msglen);
	if(ptemp==NULL) {TRACE("CSV_NOWHISPER::Set() 메모리 할당 에러\n"); return FALSE;}
	bNoWhisper = (BOOL*)SmartCpy(&ptemp, &bnowhisper, sizeof(*bNoWhisper));
	return TRUE;
}
BOOL CSV_NOWHISPER::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_NOWHISPER::Get() 메모리 할당 에러\n"); return FALSE;}
	bNoWhisper = (BOOL*)Jump(&ptemp, sizeof(*bNoWhisper));
	return TRUE;
}

////////// 대기실 채팅 데이타
BOOL CSV_CHATDATA::Set(char *id, char *chat)
{
	char strId[16]={0,};
	CString szChat;
	szChat.Format("%s", chat);
	short l_strchat = (short) szChat.GetLength();
	int msglen = sizeof(strId) + sizeof(l_strchat) + l_strchat;

	char* ptemp = SetHeader(SV_CHATDATA, msglen);
	if(ptemp==NULL) {TRACE("CSV_CHATDATA::Set() 메모리 할당 에러\n"); return FALSE;}

	if(strlen(id) < 16) strcpy(strId, id);
	
	ID        = SmartCpy(&ptemp, strId, 16);
	l_StrChat = (short*)SmartCpy(&ptemp, &l_strchat, sizeof(l_strchat));
	StrChat   = SmartCpy(&ptemp, (void*)szChat.operator LPCTSTR(), l_strchat);
	return TRUE;
}
BOOL CSV_CHATDATA::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_CHATDATA::Get() 메모리 할당 에러\n"); return FALSE;}
	ID        = Jump(&ptemp, 16);
	l_StrChat =(short*)Jump(&ptemp, sizeof(*l_StrChat));
	StrChat = ptemp;
	return TRUE;
}

////////// 귓속말
BOOL CSV_WHISPERCHAT::Set(char *id, int destunum, char *destid, char *chat)
{
	char strId[16]={0,};
	char strDestId[16]={0,};
	CString szChat;
	szChat.Format("%s", chat);
	short l_strchat = (short) szChat.GetLength();
	int msglen = 16 + sizeof(*DestUNum) + 16 + sizeof(l_strchat) + l_strchat;

	char* ptemp = SetHeader(SV_WHISPERCHAT, msglen);
	if(ptemp==NULL) {TRACE("CSV_WHISPERCHAT::Set() 메모리 할당 에러\n"); return FALSE;}

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
	if(ptemp==NULL) {TRACE("CSV_WHISPERCHAT::Get() 메모리 할당 에러\n"); return FALSE;}
	ID        = Jump(&ptemp, 16);
	DestUNum  = (int*)Jump(&ptemp, sizeof(*DestUNum));
	DestID    = Jump(&ptemp, 16);
	l_StrChat =(short*)Jump(&ptemp, sizeof(*l_StrChat));
	StrChat   = ptemp;
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////
//                      클라이언트용 메세지 클래스
////////////////////////////////////////////////////////////////////////////


////////// 플레이어 나가기 예약 상태
BOOL CCL_EXITSUBSCRIPT::Set(int rnum, int unum, char *id, int bexit)
{
	char strId[16]={0,};
	int msglen = sizeof(*RoomNum)+ sizeof(*UNum) + 16 + sizeof(*bExit);
	char* ptemp = SetHeader(CL_EXITSUBSCRIPT, msglen);
	if(strlen(id) < 16) strcpy(strId, id);
	if(ptemp==NULL) {TRACE("CCL_EXITSUBSCRIPT::Set() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ID = SmartCpy(&ptemp, strId, 16);
	bExit = (int*)SmartCpy(&ptemp, &bexit, sizeof(*bExit));
	return TRUE;
}
BOOL CCL_EXITSUBSCRIPT::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_EXITSUBSCRIPT::Get() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ID = Jump(&ptemp, 16);
	bExit = (int*)Jump(&ptemp, sizeof(*bExit));
	return TRUE;
}

////////// 채팅 데이타
BOOL CCL_CHATDATA::Set(int rnum, char *id, char kind, char* chat)
{
	char strId[16]={0,};
	short l_strchat = (short)strlen(chat);

	int msglen = sizeof(*RoomNum) + 16 + sizeof(*Kind) + sizeof(*l_StrChat) + l_strchat;

	if(strlen(id) < 16) strcpy(strId, id);

	char* ptemp = SetHeader(CL_CHATDATA, msglen);
	if(ptemp==NULL) {TRACE("CCL_CHATDATA::Set() 메모리 할당 에러\n"); return FALSE;}

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
	if(ptemp==NULL) {TRACE("CCL_CHATDATA::Get() 메모리 할당 에러\n"); return FALSE;}
	RoomNum   = (int*)Jump(&ptemp, sizeof(*RoomNum));
	ID        = Jump(&ptemp, 16);
	Kind      = Jump(&ptemp, sizeof(*Kind));
	l_StrChat =(short*)Jump(&ptemp, sizeof(*l_StrChat));
	StrChat = ptemp;
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////
//                      게임 진행용 메세지 클래스
////////////////////////////////////////////////////////////////////////////

////////// 게임을 시작할 준비를 마쳤음(재시작할때 사용)
BOOL CCL_READYTOSTART::Set(char *id)
{
	char strId[16]={0,};
	int msglen = 16;
	char* ptemp = SetHeader(CL_READYTOSTART, msglen);

	if(strlen(id) < 16) strcpy(strId, id);

	if(ptemp==NULL) {TRACE("CCL_READYTOSTART::Set() 메모리 할당 에러\n"); return FALSE;}
	Id = SmartCpy(&ptemp, strId, 16);
	return TRUE;
}
BOOL CCL_READYTOSTART::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_READYTOSTART::Get() 메모리 할당 에러\n"); return FALSE;}
	Id = ptemp;
	return TRUE;
}

////////// 게임 방식 바꾸기 요청int 
	
BOOL CCL_ASK_CHANGEGAMEKIND::Set(int rnum, int servpnum, char *id, int gamekind, int bwhis, int ncard)
{
	char strId[16]={0,};
	int msglen = sizeof(*RoomNum) + sizeof(*ServPNum) + 16 + sizeof(*GameKind) + sizeof(*bWhis) + sizeof(*nCard);
	char* ptemp = SetHeader(CL_ASK_CHANGEGAMEKIND, msglen);
	if(strlen(id) < 16) strcpy(strId, id);

	if(ptemp==NULL) {TRACE("CCL_ASK_CHANGEGAMEKIND::Set() 메모리 할당 에러\n"); return FALSE;}
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
	if(ptemp==NULL) {TRACE("CCL_ASK_CHANGEGAMEKIND::Get() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	ServPNum = (int*)Jump(&ptemp, sizeof(*ServPNum));
	ID = Jump(&ptemp, 16);
	GameKind = (int*)Jump(&ptemp, sizeof(*GameKind));
	bWhis = (int*)Jump(&ptemp, sizeof(*bWhis));
	nCard = (int*)Jump(&ptemp, sizeof(*nCard));
	return TRUE;
}

////////// 게임 방식 바꾸기 결과
BOOL CCL_CHANGEGAMEKIND::Set( ROOMINFO *pri)
{
	int msglen = sizeof(ROOMINFO);
	char* ptemp = SetHeader(CL_CHANGEGAMEKIND, msglen);
	if(ptemp==NULL) {TRACE("CCL_CHANGEGAMEKIND::Set() 메모리 할당 에러\n"); return FALSE;}
	pRI = (ROOMINFO*)SmartCpy(&ptemp, pri, sizeof(ROOMINFO));
	return TRUE;
}
BOOL CCL_CHANGEGAMEKIND::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_CHANGEGAMEKIND::Get() 메모리 할당 에러\n"); return FALSE;}
	pRI = (ROOMINFO*)Jump(&ptemp, sizeof(ROOMINFO));
	return TRUE;
}

////////// 게임 시작을 요청
BOOL CCL_ASK_STARTGAME::Set(int rnum, int servpnum, char *id)
{
	char strId[16]={0,};
	int msglen = sizeof(*RoomNum) + sizeof(*ServPNum) + 16;
	char* ptemp = SetHeader(CL_ASK_STARTGAME, msglen);
	if(strlen(id) < 16) strcpy(strId, id);

	if(ptemp==NULL) {TRACE("CCL_ASK_STARTGAME::Set() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	ServPNum = (int*)SmartCpy(&ptemp, &servpnum, sizeof(*ServPNum));
	ID = SmartCpy(&ptemp, strId, 16);
	return TRUE;
}
BOOL CCL_ASK_STARTGAME::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_ASK_STARTGAME::Get() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	ServPNum = (int*)Jump(&ptemp, sizeof(*ServPNum));
	ID = Jump(&ptemp, 16);
	return TRUE;
}

////////// 게임 시작 정보
BOOL CCL_STARTINFO::Set(STARTINFO *pSI)
{
	int msglen = sizeof(STARTINFO);
	char* ptemp = SetHeader(CL_STARTINFO, msglen);
	if(ptemp==NULL) {TRACE("CCL_STARTINFO::Set() 메모리 할당 에러\n"); return FALSE;}
	SI = (STARTINFO*)SmartCpy(&ptemp, pSI, sizeof(STARTINFO));
	//Encode();
	return TRUE;
}
BOOL CCL_STARTINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_STARTINFO::Get() 메모리 할당 에러\n"); return FALSE;}
	SI = (STARTINFO*)ptemp;
	return TRUE;
}


////////// 방정보 변경 
BOOL CCL_ASK_CHSTYLEROOMINFO::Set(STYLEROOMINFO *pCHRI)
{
	int msglen = sizeof(STYLEROOMINFO);
	char* ptemp = SetHeader(CL_ASK_CHSTYLEROOMINFO, msglen);
	if(ptemp==NULL) {TRACE("CCL_ASK_CHSTYLEROOMINFO::Set() 메모리 할당 에러\n"); return FALSE;}
	CHRI = (STYLEROOMINFO*)SmartCpy(&ptemp, pCHRI, sizeof(STYLEROOMINFO));
	//Encode();
	return TRUE;
}
BOOL CCL_ASK_CHSTYLEROOMINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_ASK_CHSTYLEROOMINFO::Get() 메모리 할당 에러\n"); return FALSE;}
	CHRI = (STYLEROOMINFO*)ptemp;
	return TRUE;
}





//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

////////// 처음 카드를 날림 3장
BOOL CSV_STARTCARD::Set(int rnum, int unum, char *id, STARTINFO *psc)
{
	char strId[16]={0,};
	int msglen = sizeof(*RoomNum)+ sizeof(*UNum) + 16 + sizeof(STARTINFO);
	char* ptemp = SetHeader(SV_STARTCARD, msglen);
	if(strlen(id) < 16) strcpy(strId, id);
	if(ptemp==NULL) {TRACE("CSV_STARTCARD::Set() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ID = SmartCpy(&ptemp, strId, 16);
	pSC = (STARTINFO*)SmartCpy(&ptemp, psc, sizeof(STARTINFO));
	return TRUE;
}

BOOL CSV_STARTCARD::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_STARTCARD::Get() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ID = Jump(&ptemp, 16);
	pSC = (STARTINFO*)Jump(&ptemp, sizeof(STARTINFO));
	return TRUE;
}

////////// 카드 변경 요청
BOOL CSV_CHANGECARD::Set(CHANGECARD *pcc)
{
	int msglen = sizeof(CHANGECARD);
	char* ptemp = SetHeader(SV_CHANGECARD, msglen);
	if(ptemp==NULL) {TRACE("CSV_CHANGECARD::Set() 메모리 할당 에러\n"); return FALSE;}
	pCC = (CHANGECARD*)SmartCpy(&ptemp, pcc, sizeof(CHANGECARD));
	return TRUE;
}

BOOL CSV_CHANGECARD::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_CHANGECARD::Get() 메모리 할당 에러\n"); return FALSE;}
	pCC = (CHANGECARD*)Jump(&ptemp, sizeof(CHANGECARD));
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
// 62cut 카드 변경 요청
BOOL CSV_ASK_CHANGECARD_62CUT::Set(CHANGECARD_62CUT *pcc)
{
	int msglen = sizeof(CHANGECARD_62CUT);
	char* ptemp = SetHeader(SV_ASK_CHANGECARD_62CUT, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_CHANGECARD_62CUT::Set() 메모리 할당 에러\n"); return FALSE;}
	pCC = (CHANGECARD_62CUT*)SmartCpy(&ptemp, pcc, sizeof(CHANGECARD_62CUT));
	return TRUE;
}

BOOL CSV_ASK_CHANGECARD_62CUT::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("SV_ASK_CHANGECARD_62CUT::Get() 메모리 할당 에러\n"); return FALSE;}
	pCC = (CHANGECARD_62CUT*)Jump(&ptemp, sizeof(CHANGECARD_62CUT));
	return TRUE;
}

// 62cut 바꿀 카드 넘김 (동시에 뿌리기
BOOL CSV_ASK_CHANGECARD_62CUT_EX::Set(CHANGECARD_62CUT_EX *pcc)
{
	int msglen = sizeof(CHANGECARD_62CUT_EX);
	char* ptemp = SetHeader(SV_ASK_CHANGECARD_62CUT_EX, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_CHANGECARD_62CUT_EX::Set() 메모리 할당 에러\n"); return FALSE;}
	pCC = (CHANGECARD_62CUT_EX*)SmartCpy(&ptemp, pcc, sizeof(CHANGECARD_62CUT_EX));
	return TRUE;
}

BOOL CSV_ASK_CHANGECARD_62CUT_EX::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("SV_ASK_CHANGECARD_62CUT_EX::Get() 메모리 할당 에러\n"); return FALSE;}
	pCC = (CHANGECARD_62CUT_EX*)Jump(&ptemp, sizeof(CHANGECARD_62CUT_EX));
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 62cut 변경되는 카드를 보냄
BOOL CSV_CHANGECARD_62CUT_RESULT::Set(CHANGECARD_62CUT *pcc)
{
	int msglen = sizeof(CHANGECARD_62CUT);
	char* ptemp = SetHeader(SV_CHANGECARD_62CUT_RESULT, msglen);
	if(ptemp==NULL) {TRACE("CSV_CHANGECARD_62CUT_RESULT::Set() 메모리 할당 에러\n"); return FALSE;}
	pCC = (CHANGECARD_62CUT*)SmartCpy(&ptemp, pcc, sizeof(CHANGECARD_62CUT));
	return TRUE;
}

BOOL CSV_CHANGECARD_62CUT_RESULT::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_CHANGECARD_62CUT_RESULT::Get() 메모리 할당 에러\n"); return FALSE;}
	pCC = (CHANGECARD_62CUT*)Jump(&ptemp, sizeof(CHANGECARD_62CUT));
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 바꾸기 결과 알림
BOOL CCL_CARDCHANGE_END_62CUT::Set(int rnum, int servpnum, char *id)
{
	char strId[16]={0,};
	int msglen = sizeof(*RoomNum) + sizeof(*ServPNum) + 16;
	char* ptemp = SetHeader(CL_CARDCHANGE_END_62CUT, msglen);
	if(strlen(id) < 16) strcpy(strId, id);

	if(ptemp==NULL) {TRACE("CCL_ASK_STARTGAME::Set() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	ServPNum = (int*)SmartCpy(&ptemp, &servpnum, sizeof(*ServPNum));
	ID = SmartCpy(&ptemp, strId, 16);
	return TRUE;
}

BOOL CCL_CARDCHANGE_END_62CUT::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_CARDCHANGE_END_62CUT::Get() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	ServPNum = (int*)Jump(&ptemp, sizeof(*ServPNum));
	ID = Jump(&ptemp, 16);
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////

// 시작버튼 활성화 알림
//시작버튼 활성화 
BOOL CSV_ACTIVE_STARTBUTTON::Set(int rnum, int servpnum, char *id,int bactive)
{
	char strId[16]={0,};
	int msglen = sizeof(*RoomNum) + sizeof(*WinnerServPNum) + 16 + sizeof(*bActive);
	char* ptemp = SetHeader(SV_ACTIVE_STARTBUTTON, msglen);
	if(strlen(id) < 16) strcpy(strId, id);

	if(ptemp==NULL) {TRACE("CSV_ACTIVE_STARTBUTTON::Set() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	WinnerServPNum = (int*)SmartCpy(&ptemp, &servpnum, sizeof(*WinnerServPNum));
	WinnerID = SmartCpy(&ptemp, strId, 16);
	bActive = (int*)SmartCpy(&ptemp, &bactive, sizeof(*bActive));
	return TRUE;
}

BOOL CSV_ACTIVE_STARTBUTTON::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ACTIVE_STARTBUTTON::Get() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	WinnerServPNum = (int*)Jump(&ptemp, sizeof(*WinnerServPNum));
	WinnerID = Jump(&ptemp, 16);
	bActive = (int*)Jump(&ptemp, sizeof(*bActive));
	return TRUE;
}

////////// 카드 사기를 요청
BOOL CSV_BUYCARD::Set(BUYCARD *pcc)
{
	
	int msglen = sizeof(BUYCARD);
	char* ptemp = SetHeader(SV_BUYCARD, msglen);

	if(ptemp==NULL) {TRACE("CSV_BUYCARD::Set() 메모리 할당 에러\n"); return FALSE;}
	pBC = (BUYCARD*)SmartCpy(&ptemp, pcc, sizeof(BUYCARD));
	return TRUE;
}

BOOL CSV_BUYCARD::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_BUYCARD::Get() 메모리 할당 에러\n"); return FALSE;}
	pBC = (BUYCARD*)Jump(&ptemp, sizeof(BUYCARD));
	return TRUE;
}


////////// 게임 유저 FOLD
BOOL CCL_ASK_FOLD::Set(int rnum, int unum, int pnum, char *id, POKERCLNT *pfd)
{
	char strId[16]={0,};
	int msglen = sizeof(*RoomNum)+ sizeof(*UNum) + sizeof(*PNum) + 16 + sizeof(POKERCLNT);
	char* ptemp = SetHeader(CL_ASK_FOLD, msglen);
	if(strlen(id) < 16) strcpy(strId, id);
	if(ptemp==NULL) {TRACE("CCL_ASK_FOLD::Set() 메모리 할당 에러\n"); return FALSE;}
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
	if(ptemp==NULL) {TRACE("CCL_ASK_FOLD::Get() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	PNum = (int*)Jump(&ptemp, sizeof(*PNum));
	ID = Jump(&ptemp, 16);
	pFD = (POKERCLNT*)Jump(&ptemp, sizeof(POKERCLNT));
	return TRUE;
}


////////// 게임 유저 카드 선택
BOOL CCL_ASK_SELECTCARD::Set(int rnum, int unum, int pnum, char *id, POKERCLNT *psd, int discard)
{
	char strId[16]={0,};
	int msglen = sizeof(*RoomNum)+ sizeof(*UNum) + sizeof(*PNum) + 16 + sizeof(POKERCLNT)+ sizeof(*DisCard);
	char* ptemp = SetHeader(CL_ASK_SELECTCARD, msglen);
	if(strlen(id) < 16) strcpy(strId, id);
	if(ptemp==NULL) {TRACE("CCL_ASK_SELECTCARD::Set() 메모리 할당 에러\n"); return FALSE;}
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
	if(ptemp==NULL) {TRACE("CCL_ASK_SELECTCARD::Get() 메모리 할당 에러\n"); return FALSE;}
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
	if(ptemp==NULL) {TRACE("CCL_ASK_RAISE::Set() 메모리 할당 에러\n"); return FALSE;}
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
	if(ptemp==NULL) {TRACE("CCL_ASK_RAISE::Get() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	PNum = (int*)Jump(&ptemp, sizeof(*PNum));
	ID = Jump(&ptemp, 16);
	pRA = (POKERCLNT*)Jump(&ptemp, sizeof(POKERCLNT));
	return TRUE;
}

//////////  사용자 카드 날리기 종료
BOOL CCL_WINDCARDEND::Set(int rnum, int pnum, int unum, char *id)//, POKERCLNT *pwd)
{
	char strId[16]={0,};
	int msglen = sizeof(*RoomNum) + sizeof(*PNum) + sizeof(*UNum) + 16; // + sizeof(POKERCLNT);
	char* ptemp = SetHeader(CL_WINDCARDEND, msglen);
	if(strlen(id) < 16) strcpy(strId, id);
	if(ptemp==NULL) {TRACE("CCL_WINDCARDEND::Set() 메모리 할당 에러\n"); return FALSE;}
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
	if(ptemp==NULL) {TRACE("CCL_WINDCARDEND::Get() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	PNum = (int*)Jump(&ptemp, sizeof(*PNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ID = Jump(&ptemp, 16);
//	pWD = (POKERCLNT*)Jump(&ptemp, sizeof(POKERCLNT));
	return TRUE;
}


////////// 턴 소비로 인한 메세지
BOOL CCL_ROOM_TIMEOUT::Set(int rnum, int unum, char *id, int lstrlen, char *msg)
{
	char strId[16]={0,};
	int msglen = sizeof(*RoomNum)+ sizeof(*UNum) + 16 + sizeof(l_StrMsg) + lstrlen;
	char* ptemp = SetHeader(CL_ROOM_TIMEOUT, msglen);
	if(strlen(id) < 16) strcpy(strId, id);
	if(ptemp==NULL) {TRACE("CCL_ROOM_TIMEOUT::Set() 메모리 할당 에러\n"); return FALSE;}
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
	if(ptemp==NULL) {TRACE("CCL_ROOM_TIMEOUT::Get() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ID = Jump(&ptemp, 16);
	l_StrMsg = (int*)Jump(&ptemp, sizeof(*l_StrMsg));
	StrMsg = Jump(&ptemp, *l_StrMsg);
	return TRUE;
}


////////// 스페샬 카드로 이겼을 경우 모든 접속자에게 알림
BOOL CSV_SPECIALCARD::Set(SPECIALCARD *psc)
{
	int msglen =  sizeof(SPECIALCARD);
	char* ptemp = SetHeader(SV_SPECIALCARD, msglen);
	if(ptemp==NULL) {TRACE("CSV_SPECIALCARD::Set() 메모리 할당 에러\n"); return FALSE;}
	pSC = (SPECIALCARD*)SmartCpy(&ptemp, psc, sizeof(SPECIALCARD));
	return TRUE;
}
BOOL CSV_SPECIALCARD::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_SPECIALCARD::Get() 메모리 할당 에러\n"); return FALSE;}
	pSC = (SPECIALCARD*)Jump(&ptemp, sizeof(SPECIALCARD) );
	return TRUE;
}


/////////// 폴드한 사용자 유저 정보
BOOL CCL_USERRESULT::Set(USERRESULT *prs)
{
	int msglen =  sizeof(USERRESULT);
	char* ptemp = SetHeader(CL_USERRESULT, msglen);
	if(ptemp==NULL) {TRACE("CCL_USERRESULT::Set() 메모리 할당 에러\n"); return FALSE;}
	pRS = (USERRESULT*)SmartCpy(&ptemp, prs, sizeof(USERRESULT));
	return TRUE;
}
BOOL CCL_USERRESULT::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_USERRESULT::Get() 메모리 할당 에러\n"); return FALSE;}
	pRS = (USERRESULT*)Jump(&ptemp, sizeof(USERRESULT) );
	return TRUE;
}


/////////// 서버 알림 레이즈
BOOL CSV_RAISE::Set(POKERGAME *ppg)
{
	int msglen =  sizeof(POKERGAME);
	char* ptemp = SetHeader(SV_RAISE, msglen);
	if(ptemp==NULL) {TRACE("CSV_RAISE::Set() 메모리 할당 에러\n"); return FALSE;}
	pPG = (POKERGAME*)SmartCpy(&ptemp, ppg, sizeof(POKERGAME));
	return TRUE;
}
BOOL CSV_RAISE::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_RAISE::Get() 메모리 할당 에러\n"); return FALSE;}
	pPG = (POKERGAME*)Jump(&ptemp, sizeof(POKERGAME) );
	return TRUE;
}

////////// 마지막 히든 카드 
BOOL CSV_HIDDENCARD::Set(int rnum, int unum, char *id, int card)
{
	char strId[16]={0,};
	int msglen = sizeof(*RoomNum)+ sizeof(*UNum) + 16 + sizeof(*nCard);
	char* ptemp = SetHeader(SV_HIDDENCARD, msglen);
	if(strlen(id) < 16) strcpy(strId, id);
	if(ptemp==NULL) {TRACE("CSV_HIDDENCARD::Set() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ID = SmartCpy(&ptemp, strId, 16);
	nCard = (int*)SmartCpy(&ptemp, &card, sizeof(*nCard));
	return TRUE;
}

BOOL CSV_HIDDENCARD::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_HIDDENCARD::Get() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ID = Jump(&ptemp, 16);
	nCard = (int*)Jump(&ptemp, sizeof(*nCard));
	return TRUE;
}


//첫 3장을 보낸후 유저가 하나의 카드를 선택 => 모두 선택시 게임시작
BOOL CSV_WINDCARDEND::Set(int reserve )
{
	
	int msglen = sizeof(*Reserve);
	char* ptemp = SetHeader(SV_WINDCARDEND, msglen);
	if(ptemp==NULL) {TRACE("CSV_WINDCARDEND::Set() 메모리 할당 에러\n"); return FALSE;}
	Reserve = (int*)SmartCpy(&ptemp, &reserve, sizeof(*Reserve));
	
	return TRUE;
}

BOOL CSV_WINDCARDEND::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_WINDCARDEND::Get() 메모리 할당 에러\n"); return FALSE;}
	Reserve = (int*)Jump(&ptemp, sizeof(*Reserve));
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// 62cut client 가 server 로 바꿀 카드를 보냄 
BOOL CCL_CARDCHANGE_62CUT_RESULT::Set(CHANGECARD_62CUT *pcc)
{
	int msglen = sizeof(CHANGECARD_62CUT);
	char* ptemp = SetHeader(CL_CARDCHANGE_62CUT_RESULT, msglen);
	if(ptemp==NULL) {TRACE("CCL_CARDCHAGE_62CUT_RESULT::Set() 메모리 할당 에러\n"); return FALSE;}
	pCC = (CHANGECARD_62CUT*)SmartCpy(&ptemp, pcc, sizeof(CHANGECARD_62CUT));
	return TRUE;
}

BOOL CCL_CARDCHANGE_62CUT_RESULT::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_CARDCHAGE_62CUT_RESULT::Get() 메모리 할당 에러\n"); return FALSE;}
	pCC = (CHANGECARD_62CUT*)Jump(&ptemp, sizeof(CHANGECARD_62CUT));
	return TRUE;
}


// 판 종료 알림
BOOL CSV_ENDGAME::Set(int reserve )
{
	
	int msglen = sizeof(*Reserve);
	char* ptemp = SetHeader(SV_ENDGAME, msglen);
	if(ptemp==NULL) {TRACE("CSV_ENDGAME::Set() 메모리 할당 에러\n"); return FALSE;}
	Reserve = (int*)SmartCpy(&ptemp, &reserve, sizeof(*Reserve));
	
	return TRUE;
}

BOOL CSV_ENDGAME::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ENDGAME::Get() 메모리 할당 에러\n"); return FALSE;}
	Reserve = (int*)Jump(&ptemp, sizeof(*Reserve));
	return TRUE;
}


/////////// 서버 알림 레이즈 끝  하이 로우 스윙 결정
BOOL CSV_ENDRAISE::Set(int flag, POKERGAME *ppg)
{
	int msglen =  sizeof(*Flag) + sizeof(POKERGAME);
	char* ptemp = SetHeader(SV_ENDRAISE, msglen);
	if(ptemp==NULL) {TRACE("CSV_ENDRAISE::Set() 메모리 할당 에러\n"); return FALSE;}
	Flag = (int*)SmartCpy(&ptemp, &flag, sizeof(*Flag));
	pPG = (POKERGAME*)SmartCpy(&ptemp, ppg, sizeof(POKERGAME));
	return TRUE;
}
BOOL CSV_ENDRAISE::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ENDRAISE::Get() 메모리 할당 에러\n"); return FALSE;}
	Flag = (int*)Jump(&ptemp, sizeof(*Flag) );
	pPG = (POKERGAME*)Jump(&ptemp, sizeof(POKERGAME) );
	return TRUE;
}

////////// 어떤 걸로 승부를 겨루겠는가?
BOOL CCL_SELECTWINCASE::Set(int rnum, int servepnum, int unum, char *id, int ncase)
{
	char strId[16]={0,};
	int msglen = sizeof(*RoomNum)+ sizeof(*ServPNum) + sizeof(*UNum) + 16 + sizeof(*nCase);
	char* ptemp = SetHeader(CL_SELECTWINCASE, msglen);
	if(strlen(id) < 16) strcpy(strId, id);
	if(ptemp==NULL) {TRACE("CCL_SELECTWINCASE::Set() 메모리 할당 에러\n"); return FALSE;}
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
	if(ptemp==NULL) {TRACE("CCL_SELECTWINCASE::Get() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	ServPNum = (int*)Jump(&ptemp, sizeof(*ServPNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ID = Jump(&ptemp, 16);
	nCase = (int*)Jump(&ptemp, sizeof(*nCase));
	return TRUE;
}




/////////////////////////////////////////////////////////////////////////////////////////
// [ 게임 아이템 ]
// 에러 메세지~~~
BOOL CCL_ITEMRESULT::Set( int recv_code )
{
	int msglen = sizeof( *RetCode );
	char* ptemp = SetHeader( CL_ITEMRESULT, msglen );
	if( ptemp == NULL ) { TRACE( "CCL_ITEMRESULT::Set() 메모리 할당 에러\n" ); return FALSE; }
	RetCode = (int*)SmartCpy( &ptemp, &recv_code, sizeof(*RetCode) );
	
	return TRUE;
}

BOOL CCL_ITEMRESULT::Get( char* lpdata, int size )
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_ITEMRESULT::Get() 메모리 할당 에러\n"); return FALSE;}
	RetCode = (int*)Jump(&ptemp, sizeof(*RetCode));

	return TRUE;
}

// 아템이 여러개 있구 사용한게 없다면 선택할수 있다.
BOOL CCL_ITEMLIST::Set( GAMEITEM* recv_item, int recv_itemcount, int recv_use )
{
	int msglen = sizeof( GAMEITEM ) * recv_itemcount + sizeof(*Itemcount) + sizeof(*Use);
	char* ptemp = SetHeader( CL_ITEMLIST, msglen );
	if( ptemp == NULL ) { TRACE( "CCL_ITEMLIST::Set() 메모리 할당 에러\n" ); return FALSE; }
	Itemcount = (int*)SmartCpy( &ptemp, &recv_itemcount, sizeof(*Itemcount) );
	Item = (GAMEITEM*)SmartCpy( &ptemp, recv_item, sizeof(GAMEITEM) * recv_itemcount );
	Use = (int*)SmartCpy( &ptemp, &recv_use, sizeof(*Use) );

	return TRUE;
}

BOOL CCL_ITEMLIST::Get( char* lpdata, int size )
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_ITEMLIST::Get() 메모리 할당 에러\n"); return FALSE;}
	Itemcount = (int*)Jump(&ptemp, sizeof(*Itemcount) );
	Item = (GAMEITEM*)Jump(&ptemp, sizeof(GAMEITEM) * (*Itemcount) );
	Use = (int*)Jump( &ptemp, sizeof(*Use) );

	return TRUE;
}


// 아이템 선택 요청
BOOL CCL_ASK_ITEMLOGIN::Set( GAMEITEM* recv_item, int recv_itemcount )
{
	int msglen = sizeof( GAMEITEM ) * recv_itemcount + sizeof(*Itemcount);
	char* ptemp = SetHeader( CL_ASK_ITEMLOGIN, msglen );
	if( ptemp == NULL ) { TRACE( "CCL_ASK_ITEMLOGIN::Set() 메모리 할당 에러\n" ); return FALSE; }
	Itemcount = (int*)SmartCpy( &ptemp, &recv_itemcount, sizeof(*Itemcount) );
	Item = (GAMEITEM*)SmartCpy( &ptemp, recv_item, sizeof(GAMEITEM) * recv_itemcount );

	return TRUE;
}

BOOL CCL_ASK_ITEMLOGIN::Get( char* lpdata, int size )
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_ASK_ITEMLOGIN::Get() 메모리 할당 에러\n"); return FALSE;}
	Itemcount = (int*)Jump(&ptemp, sizeof(*Itemcount) );
	Item = (GAMEITEM*)Jump(&ptemp, sizeof(GAMEITEM) * (*Itemcount) );

	return TRUE;
}

// 아이템 선택 
BOOL CCL_ACCEPT_ITEMLOGIN::Set( GAMEITEM* recv_item, int recv_itemcount )
{
	int msglen = sizeof( GAMEITEM ) * recv_itemcount + sizeof(*Itemcount);
	char* ptemp = SetHeader( CL_ACCEPT_ITEMLOGIN, msglen );
	if( ptemp == NULL ) { TRACE( "CCL_ACCEPT_ITEMLOGIN::Set() 메모리 할당 에러\n" ); return FALSE; }
	Itemcount = (int*)SmartCpy( &ptemp, &recv_itemcount, sizeof(*Itemcount) );
	Item = (GAMEITEM*)SmartCpy( &ptemp, recv_item, sizeof(GAMEITEM) * recv_itemcount );

	return TRUE;
}

BOOL CCL_ACCEPT_ITEMLOGIN::Get( char* lpdata, int size )
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_ACCEPT_ITEMLOGIN::Get() 메모리 할당 에러\n"); return FALSE;}
	Itemcount = (int*)Jump(&ptemp, sizeof(*Itemcount) );
	Item = (GAMEITEM*)Jump(&ptemp, sizeof(GAMEITEM) * (*Itemcount) );

	return TRUE;
}

// 아이템 사용해서 입장
BOOL CCL_ITEMLOGIN::Set( int rc_index )
{
	int msglen = sizeof(*Index);
	char* ptemp = SetHeader( CL_ITEMLOGIN, msglen );
	if( ptemp == NULL ) { TRACE( "CCL_ITEMLOGIN::Set() 메모리 할당 에러\n" ); return FALSE; }
	Index = (int*)SmartCpy( &ptemp, &rc_index, sizeof(*Index) );

	return TRUE;
}

BOOL CCL_ITEMLOGIN::Get( char* lpdata, int size )
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_ITEMLOGIN::Get() 메모리 할당 에러\n"); return FALSE;}
	if( ptemp == NULL ) { TRACE( "" ); return FALSE; }
	Index = (int*)Jump( &ptemp, sizeof(*Index) );
	
	return TRUE;
}


////////// 유저가 폴드
BOOL CSV_FOLDUSER::Set(int rnum, int unum, int pnum, char *id, int kind, INT64 jackpot)
{
	char strId[16]={0,};
	int msglen = sizeof(*RoomNum)+ sizeof(*UNum) + sizeof(*PNum) + 16 + sizeof(*Kind) + sizeof(*JackPotMoney);
	char* ptemp = SetHeader(SV_FOLDUSER, msglen);
	if(strlen(id) < 16) strcpy(strId, id);
	if(ptemp==NULL) {TRACE("CSV_FOLDUSER::Set() 메모리 할당 에러\n"); return FALSE;}
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
	if(ptemp==NULL) {TRACE("CSV_FOLDUSER::Get() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	PNum = (int*)Jump(&ptemp, sizeof(*PNum));
	ID = Jump(&ptemp, 16);
	Kind = (int*)Jump(&ptemp, sizeof(*Kind));
	JackPotMoney = (INT64*)Jump(&ptemp, sizeof(*JackPotMoney));
	return TRUE;
}



/////////// 바로 시작 방 입장을 요청
BOOL CSV_ASK_QUICKSTART::Set(int unum, char *id, int sndKind, int gamekind)// ### [사운드 추가 작업] ###
{
	int msglen = sizeof(*UNum) + 16 + sizeof(*SndFxKind) + sizeof(*GameKind); // 추가
	char strId[16]={0,};
	if(strlen(id) < 16) strcpy(strId, id);
	char* ptemp = SetHeader(SV_ASK_QUICKSTART, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_QUICKSTART::Set() 메모리 할당 에러\n"); return FALSE;}
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ID = SmartCpy(&ptemp, strId, 16);
	SndFxKind = (int*)SmartCpy(&ptemp, &sndKind, sizeof(*SndFxKind)); // 추가
	GameKind = (int*)SmartCpy(&ptemp, &gamekind, sizeof(*GameKind)); 
	return TRUE;
}
BOOL CSV_ASK_QUICKSTART::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_QUICKSTART::Get() 메모리 할당 에러\n"); return FALSE;}
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ID = Jump(&ptemp, 16);
	SndFxKind = (int*)Jump(&ptemp, sizeof(*SndFxKind));// ### [사운드 추가 작업] ###
	GameKind = (int*)Jump(&ptemp, sizeof(*GameKind));
	return TRUE;
}



/////////// 바로 입장 불가(방생성)
BOOL CSV_REFUSE_QUICKSTART::Set(int code) 
{
	int msglen = sizeof(*Code);
	char* ptemp = SetHeader(SV_REFUSE_QUICKSTART, msglen);
	if(ptemp==NULL) {TRACE("CSV_REFUSE_QUICKSTART::Set() 메모리 할당 에러\n"); return FALSE;}
	Code = (int*)SmartCpy(&ptemp, &code, sizeof(*Code));
	return TRUE;
}
BOOL CSV_REFUSE_QUICKSTART::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_REFUSE_QUICKSTART::Get() 메모리 할당 에러\n"); return FALSE;}
	Code = (int*)ptemp;
	return TRUE;
}




///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// 서버에이전트 작업	

BOOL CNM_AGENT_CONNECT::Set(int tot, INT64 jackpot) 
{
	int msglen = sizeof(*TotalNum)+sizeof(*JackPot);
	char* ptemp = SetHeader(NM_AGENT_CONNECT, msglen);
	if(ptemp==NULL) {TRACE("CNM_AGENT_CONNECT::Set() 메모리 할당 에러\n"); return FALSE;}
	TotalNum = (int*)SmartCpy(&ptemp, &tot, sizeof(*TotalNum));
	JackPot = (INT64*)SmartCpy(&ptemp, &jackpot, sizeof(*JackPot));
	return TRUE;
}
BOOL CNM_AGENT_CONNECT::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CNM_AGENT_CONNECT::Get() 메모리 할당 에러\n"); return FALSE;}
	TotalNum = (int*)Jump(&ptemp, sizeof(*TotalNum));
	JackPot = (INT64*)Jump(&ptemp, sizeof(*JackPot));
	return TRUE;
}

// 접속 목록 보내기
BOOL CNM_AGENT_USERLIST::Set(int reserve) 
{
	int msglen = sizeof(*Reserve);
	char* ptemp = SetHeader(NM_AGENT_USERLIST, msglen);
	if(ptemp==NULL) {TRACE("CNM_AGENT_USERLIST::Set() 메모리 할당 에러\n"); return FALSE;}
	Reserve = (int*)SmartCpy(&ptemp, &reserve, sizeof(*Reserve));
	return TRUE;
}
BOOL CNM_AGENT_USERLIST::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CNM_AGENT_USERLIST::Get() 메모리 할당 에러\n"); return FALSE;}
	Reserve = (int*)ptemp;
	return TRUE;
}


////////// 1차 400명
BOOL CNM_AGENT_USERLIST_R1::Set(int totnum, AGENT_USER *ppSUI, int count)
{
	int msglen = sizeof(*TotNum) + sizeof(*Count) + sizeof(AGENT_USER)*totnum; 
	char* ptemp = SetHeader(NM_AGENT_USERLIST_R1, msglen);
	if(ptemp==NULL) {TRACE("CNM_AGENT_USERLIST_R1::Set() 메모리 할당 에러\n"); return FALSE;}
	TotNum = (int*)SmartCpy(&ptemp, &totnum, sizeof(*TotNum));
	Count = (int*)SmartCpy(&ptemp, &count, sizeof(*Count));
	ArraySUI = (AGENT_USER*)SmartCpy(&ptemp, ppSUI, sizeof(AGENT_USER)*totnum);
	return TRUE;
}
BOOL CNM_AGENT_USERLIST_R1::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CNM_AGENT_USERLIST_R1::Get() 메모리 할당 에러\n"); return FALSE;}
	TotNum   = (int*)Jump(&ptemp, sizeof(*TotNum));
	Count = (int*)Jump(&ptemp, sizeof(*Count));
	ArraySUI = (AGENT_USER*)ptemp;
	return TRUE;
}

// 새로운 유저 입장
BOOL CNM_AGENT_NEWUSER::Set(int totnum, AGENT_USER *ppSUI, INT64 jackpot) 
{
	int msglen = sizeof(*TotNum) + sizeof(AGENT_USER) + sizeof(*JackPot);
	char* ptemp = SetHeader(NM_AGENT_NEWUSER, msglen);
	if(ptemp==NULL) {TRACE("CNM_AGENT_NEWUSER::Set() 메모리 할당 에러\n"); return FALSE;}
	TotNum = (int*)SmartCpy(&ptemp, &totnum, sizeof(*TotNum));
	ArraySUI = (AGENT_USER*)SmartCpy(&ptemp, ppSUI, sizeof(AGENT_USER));
	JackPot = (INT64*)SmartCpy(&ptemp, &jackpot, sizeof(*JackPot));
	return TRUE;
}
BOOL CNM_AGENT_NEWUSER::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CNM_AGENT_NEWUSER::Get() 메모리 할당 에러\n"); return FALSE;}
	TotNum   = (int*)Jump(&ptemp, sizeof(*TotNum));
	ArraySUI = (AGENT_USER*)Jump(&ptemp, sizeof(AGENT_USER));
	JackPot = (INT64*)Jump(&ptemp, sizeof(*JackPot));
	return TRUE;
}

// 유저 아웃
BOOL CNM_AGENT_OUTUSER::Set(int totnum, AGENT_USER *ppSUI, INT64 jackpot) 
{
	int msglen = sizeof(*TotNum) + sizeof(AGENT_USER) + sizeof(*JackPot);
	char* ptemp = SetHeader(NM_AGENT_OUTUSER, msglen);
	if(ptemp==NULL) {TRACE("CNM_AGENT_OUTUSER::Set() 메모리 할당 에러\n"); return FALSE;}
	TotNum = (int*)SmartCpy(&ptemp, &totnum, sizeof(*TotNum));
	ArraySUI = (AGENT_USER*)SmartCpy(&ptemp, ppSUI, sizeof(AGENT_USER));
	JackPot = (INT64*)SmartCpy(&ptemp, &jackpot, sizeof(*JackPot));
	return TRUE;
}
BOOL CNM_AGENT_OUTUSER::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CNM_AGENT_OUTUSER::Get() 메모리 할당 에러\n"); return FALSE;}
	TotNum   = (int*)Jump(&ptemp, sizeof(*TotNum));
	ArraySUI = (AGENT_USER*)Jump(&ptemp, sizeof(AGENT_USER));
	JackPot = (INT64*)Jump(&ptemp, sizeof(*JackPot));
	return TRUE;
}

// 중복 접속 확인
BOOL CNM_AGENT_DUBLECON::Set(int totnum, AGENT_USER *ppSUI)
{
	int msglen = sizeof(*TotNum) + sizeof(AGENT_USER);
	char* ptemp = SetHeader(NM_AGENT_DUBLECON, msglen);
	if(ptemp==NULL) {TRACE("CNM_AGENT_DUBLECON::Set() 메모리 할당 에러\n"); return FALSE;}
	TotNum = (int*)SmartCpy(&ptemp, &totnum, sizeof(*TotNum));
	ArraySUI = (AGENT_USER*)SmartCpy(&ptemp, ppSUI, sizeof(AGENT_USER));
	return TRUE;
}
BOOL CNM_AGENT_DUBLECON::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CNM_AGENT_DUBLECON::Get() 메모리 할당 에러\n"); return FALSE;}
	TotNum   = (int*)Jump(&ptemp, sizeof(*TotNum));
	ArraySUI = (AGENT_USER*)ptemp;
	return TRUE;
}


// 중복 접속자
BOOL CNM_AGENT_NEWUSERERROR::Set(int totnum, AGENT_USER *ppSUI)
{
	int msglen = sizeof(*TotNum) + sizeof(AGENT_USER);
	char* ptemp = SetHeader(NM_AGENT_NEWUSERERROR, msglen);
	if(ptemp==NULL) {TRACE("CNM_AGENT_NEWUSERERROR::Set() 메모리 할당 에러\n"); return FALSE;}
	TotNum = (int*)SmartCpy(&ptemp, &totnum, sizeof(*TotNum));
	ArraySUI = (AGENT_USER*)SmartCpy(&ptemp, ppSUI, sizeof(AGENT_USER));
	return TRUE;
}
BOOL CNM_AGENT_NEWUSERERROR::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CNM_AGENT_NEWUSERERROR::Get() 메모리 할당 에러\n"); return FALSE;}
	TotNum   = (int*)Jump(&ptemp, sizeof(*TotNum));
	ArraySUI = (AGENT_USER*)ptemp;
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// 점프 아이템 작업


// 점프 아이템 선택 요청
BOOL CCL_ASK_JUMPITEM::Set( GAMEITEM* recv_item, int recv_itemcount )
{
	int msglen = sizeof( GAMEITEM ) * recv_itemcount + sizeof(*Itemcount);
	char* ptemp = SetHeader( CL_ASK_JUMPITEM, msglen );
	if( ptemp == NULL ) { TRACE( "CCL_ASK_JUMPITEM::Set() 메모리 할당 에러\n" ); return FALSE; }
	Itemcount = (int*)SmartCpy( &ptemp, &recv_itemcount, sizeof(*Itemcount) );
	Item = (GAMEITEM*)SmartCpy( &ptemp, recv_item, sizeof(GAMEITEM) * recv_itemcount );

	return TRUE;
}

BOOL CCL_ASK_JUMPITEM::Get( char* lpdata, int size )
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_ASK_JUMPITEM::Get() 메모리 할당 에러\n"); return FALSE;}
	Itemcount = (int*)Jump(&ptemp, sizeof(*Itemcount) );
	Item = (GAMEITEM*)Jump(&ptemp, sizeof(GAMEITEM) * (*Itemcount) );

	return TRUE;
}


// 점프 아이템 채널 입장 허용
BOOL CCL_ACCEPT_JUMPITEM::Set( GAMEITEM* recv_item, int recv_itemcount, ENTERCHANINFO *pEC )
{
	int msglen = sizeof( GAMEITEM ) * recv_itemcount + sizeof(*Itemcount) + sizeof(ENTERCHANINFO);
	char* ptemp = SetHeader( CL_ACCEPT_JUMPITEM, msglen );
	if( ptemp == NULL ) { TRACE( "CCL_ACCEPT_JUMPITEM::Set() 메모리 할당 에러\n" ); return FALSE; }
	Itemcount = (int*)SmartCpy( &ptemp, &recv_itemcount, sizeof(*Itemcount) );
	Item = (GAMEITEM*)SmartCpy( &ptemp, recv_item, sizeof(GAMEITEM) * recv_itemcount );
	EC = (ENTERCHANINFO*)SmartCpy(&ptemp, pEC, sizeof(ENTERCHANINFO));
	return TRUE;
}

BOOL CCL_ACCEPT_JUMPITEM::Get( char* lpdata, int size )
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_ACCEPT_JUMPITEM::Get() 메모리 할당 에러\n"); return FALSE;}
	Itemcount = (int*)Jump(&ptemp, sizeof(*Itemcount) );
	Item = (GAMEITEM*)Jump(&ptemp, sizeof(GAMEITEM) * (*Itemcount) );
	EC = (ENTERCHANINFO*)Jump(&ptemp, sizeof(ENTERCHANINFO));

	return TRUE;
}


// 점프아이템 사용해서 입장
BOOL CCL_JUMPITEM_LOGIN::Set( int rc_index )
{
	int msglen = sizeof(*Index);
	char* ptemp = SetHeader( CL_JUMPITEM_LOGIN, msglen );
	if( ptemp == NULL ) { TRACE( "CCL_JUMPITEM_LOGIN::Set() 메모리 할당 에러\n" ); return FALSE; }
	Index = (int*)SmartCpy( &ptemp, &rc_index, sizeof(*Index) );

	return TRUE;
}

BOOL CCL_JUMPITEM_LOGIN::Get( char* lpdata, int size )
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_JUMPITEM_LOGIN::Get() 메모리 할당 에러\n"); return FALSE;}
	if( ptemp == NULL ) { TRACE( "" ); return FALSE; }
	Index = (int*)Jump( &ptemp, sizeof(*Index) );
	
	return TRUE;
}

//  DB 에 사용 유무 업데이트 후 결과를 알려준다.
BOOL CCL_ACCEPT_JUMPITEM_CH::Set( GAMEITEM* recv_item, int recv_itemcount )
{
	int msglen = sizeof( GAMEITEM ) * recv_itemcount + sizeof(*Itemcount);
	char* ptemp = SetHeader( CL_ACCEPT_JUMPITEM_CH, msglen );
	if( ptemp == NULL ) { TRACE( "CCL_ACCEPT_JUMPITEM_CH::Set() 메모리 할당 에러\n" ); return FALSE; }
	Itemcount = (int*)SmartCpy( &ptemp, &recv_itemcount, sizeof(*Itemcount) );
	Item = (GAMEITEM*)SmartCpy( &ptemp, recv_item, sizeof(GAMEITEM) * recv_itemcount );

	return TRUE;
}

BOOL CCL_ACCEPT_JUMPITEM_CH::Get( char* lpdata, int size )
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_ACCEPT_JUMPITEM_CH::Get() 메모리 할당 에러\n"); return FALSE;}
	Itemcount = (int*)Jump(&ptemp, sizeof(*Itemcount) );
	Item = (GAMEITEM*)Jump(&ptemp, sizeof(GAMEITEM) * (*Itemcount) );

	return TRUE;
}

////////// 잭팟 평균 
BOOL CNM_AGENT_AVERAGE::Set(INT64 jackpot)
{

	int msglen = sizeof(*JackPotMoney);
	char* ptemp = SetHeader(NM_AGENT_AVERAGE, msglen);
	if(ptemp==NULL) {TRACE("CNM_AGENT_AVERAGE::Set() 메모리 할당 에러\n"); return FALSE;}
	JackPotMoney = (INT64*)SmartCpy(&ptemp, &jackpot, sizeof(*JackPotMoney));
	return TRUE;
}

BOOL CNM_AGENT_AVERAGE::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CNM_AGENT_AVERAGE::Get() 메모리 할당 에러\n"); return FALSE;}
	JackPotMoney = (INT64*)Jump(&ptemp, sizeof(*JackPotMoney));
	return TRUE;
}


// ### [사운드 추가 작업] ###     변경 요청
BOOL CSV_ASK_CHANGESNDFX::Set(int unum, int spnum, int rnum, char *id, int sndfx)
{
	char strID[16];
	strncpy(strID, id, 15);
	int msglen = sizeof(*UNum) + sizeof(*ServPNum) + sizeof(*RoomNum) + 16 + sizeof(*SndFX);
	char* ptemp = SetHeader(SV_ASK_CHANGESNDFX, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_CHANGESNDFX::Set() 메모리 할당 에러\n"); return FALSE;}
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
	if(ptemp==NULL) {TRACE("CSV_ASK_CHANGESNDFX::Get() 메모리 할당 에러\n"); return FALSE;}
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ServPNum =(int*)Jump(&ptemp, sizeof(*ServPNum));
	RoomNum =(int*)Jump(&ptemp, sizeof(*RoomNum));
	ID = Jump(&ptemp, 16);
	SndFX = (int*)Jump(&ptemp, sizeof(*SndFX));
	return TRUE;
}


// ### [사운드 추가 작업] ###     변경 허락
BOOL CSV_ACCEPT_CHANGESNDFX::Set(int sndfx)
{
	int msglen = sizeof(*SndFx);
	char* ptemp = SetHeader(SV_ACCEPT_CHANGESNDFX, msglen);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_CHANGESNDFX::Set() 메모리 할당 에러\n"); return FALSE;}
	SndFx = (int*) SmartCpy(&ptemp, &sndfx, sizeof(*SndFx));
	return TRUE;
}
BOOL CSV_ACCEPT_CHANGESNDFX::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_CHANGESNDFX::Get() 메모리 할당 에러\n"); return FALSE;}
	SndFx = (int*)Jump(&ptemp, sizeof(*SndFx));
	return TRUE;
}


////////// 이벤트 당첨자 알림
BOOL CSV_EVENTPRIZE::Set(EVENTPRIZEINFO *pei)
{
	int msglen = sizeof(EVENTPRIZEINFO);
	char* ptemp = SetHeader(SV_EVENTPRIZE, msglen);
	if(ptemp==NULL) {TRACE("CSV_EVENTPRIZE::Set() 메모리 할당 에러\n"); return FALSE;}
	pEI = (EVENTPRIZEINFO*)SmartCpy(&ptemp, pei, sizeof(EVENTPRIZEINFO));
	return TRUE;
}
BOOL CSV_EVENTPRIZE::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_EVENTPRIZE::Get() 메모리 할당 에러\n"); return FALSE;}
	pEI = (EVENTPRIZEINFO*)Jump(&ptemp, sizeof(EVENTPRIZEINFO));
	return TRUE;
}


////////// 이벤트 공지
BOOL CSV_EVENTNOTICE::Set(short kind, char* chat, COLORREF color)
{
	short l_strchat = (short)strlen(chat);
	int msglen = sizeof(*Kind) + sizeof(*l_StrChat) + l_strchat + sizeof(*Color);

	char* ptemp = SetHeader(SV_EVENTNOTICE, msglen);
	if(ptemp==NULL) {TRACE("CSV_EVENTNOTICE::Set() 메모리 할당 에러\n"); return FALSE;}

	Kind      = (short*)SmartCpy(&ptemp, &kind, sizeof(*Kind));
	l_StrChat = (short*)SmartCpy(&ptemp, &l_strchat, sizeof(*l_StrChat));
	StrChat   = SmartCpy(&ptemp, chat, l_strchat);
	Color     = (COLORREF*)SmartCpy(&ptemp, &color, sizeof(*Color));
	return TRUE;
}
BOOL CSV_EVENTNOTICE::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_CHATDATA::Get() 메모리 할당 에러\n"); return FALSE;}
	Kind      = (short*)Jump(&ptemp, sizeof(*Kind));
	l_StrChat = (short*)Jump(&ptemp, sizeof(*l_StrChat));
	StrChat = Jump(&ptemp, *l_StrChat);
	Color = (COLORREF*)Jump(&ptemp, sizeof(*Color));
	return TRUE;
}


///////////////////////////////////////////////////////////////////
/////////////////// ### [관리자 모드 작업] ###  ///////////////////
///////////////////////////////////////////////////////////////////

/////////// 관리자 아디를 전송한다 에이전트
BOOL CNM_AGENT_ADMININFO::Set(int totnum, ADMIN_INFO *ppCI)
{
	int msglen = sizeof(*TotNum) + sizeof(ADMIN_INFO)*totnum;
	char* ptemp = SetHeader(NM_AGENT_ADMININFO, msglen);
	if(ptemp==NULL) {TRACE("CNM_AGENT_ADMININFO::Set() 메모리 할당 에러\n"); return FALSE;}
	TotNum = (int*)SmartCpy(&ptemp, &totnum, sizeof(*TotNum));
	AdminInfo = (ADMIN_INFO*)SmartCpy(&ptemp, ppCI, sizeof(ADMIN_INFO)*totnum);
	return TRUE;
}
BOOL CNM_AGENT_ADMININFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CNM_AGENT_ADMININFO::Get() 메모리 할당 에러\n"); return FALSE;}
	TotNum   = (int*)Jump(&ptemp, sizeof(*TotNum));
	AdminInfo = (ADMIN_INFO*)ptemp;
	return TRUE;
}


//유저 종료하기
BOOL CNM_ADMIN_OUTUSER::Set(ADMINMESSAGE *pmsg)
{
	int msglen =  sizeof(ADMINMESSAGE);
	char* ptemp = SetHeader(NM_ADMIN_OUTUSER, msglen);
	if(ptemp==NULL) {TRACE("CNM_ADMIN_OUTUSER::Set() 메모리 할당 에러\n"); return FALSE;}
	pAMESG = (ADMINMESSAGE*)SmartCpy(&ptemp, pmsg, sizeof(ADMINMESSAGE));
	return TRUE;
}
BOOL CNM_ADMIN_OUTUSER::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CNM_ADMIN_OUTUSER::Get() 메모리 할당 에러\n"); return FALSE;}
	pAMESG = (ADMINMESSAGE*)Jump(&ptemp, sizeof(ADMINMESSAGE) );
	return TRUE;
}

// 방제거하기
BOOL CNM_ADMIN_DELROOM::Set(ADMINMESSAGE *pmsg)
{
	int msglen =  sizeof(ADMINMESSAGE);
	char* ptemp = SetHeader(NM_ADMIN_DELROOM, msglen);
	if(ptemp==NULL) {TRACE("CNM_ADMIN_DELROOM::Set() 메모리 할당 에러\n"); return FALSE;}
	pAMESG = (ADMINMESSAGE*)SmartCpy(&ptemp, pmsg, sizeof(ADMINMESSAGE));
	return TRUE;
}
BOOL CNM_ADMIN_DELROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CNM_ADMIN_DELROOM::Get() 메모리 할당 에러\n"); return FALSE;}
	pAMESG = (ADMINMESSAGE*)Jump(&ptemp, sizeof(ADMINMESSAGE) );
	return TRUE;
}

////////// 귓속말
BOOL CNM_ADMIN_WHISPERCHAT::Set(char *id, int destunum, char *destid, char *chat)
{
	char strId[16]={0,};
	char strDestId[16]={0,};
	CString szChat;
	szChat.Format("%s", chat);
	short l_strchat = (short) szChat.GetLength();
	int msglen = 16 + sizeof(*DestUNum) + 16 + sizeof(l_strchat) + l_strchat;

	char* ptemp = SetHeader(NM_ADMIN_WHISPERCHAT, msglen);
	if(ptemp==NULL) {TRACE("CNM_ADMIN_WHISPERCHAT::Set() 메모리 할당 에러\n"); return FALSE;}

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
	if(ptemp==NULL) {TRACE("CNM_ADMIN_WHISPERCHAT::Get() 메모리 할당 에러\n"); return FALSE;}
	ID        = Jump(&ptemp, 16);
	DestUNum  = (int*)Jump(&ptemp, sizeof(*DestUNum));
	DestID    = Jump(&ptemp, 16);
	l_StrChat =(short*)Jump(&ptemp, sizeof(*l_StrChat));
	StrChat   = ptemp;
	return TRUE;
}


// 방제 변경하기
BOOL CNM_ADMIN_CHANGEROOM::Set(ADMINMESSAGE *padmsg, STYLEROOMINFO *pmsg)
{
	int msglen =  sizeof(STYLEROOMINFO) + sizeof(ADMINMESSAGE);
	char* ptemp = SetHeader(NM_ADMIN_CHANGEROOM, msglen);
	if(ptemp==NULL) {TRACE("CNM_ADMIN_CHANGEROOM::Set() 메모리 할당 에러\n"); return FALSE;}
	pAMESG = (ADMINMESSAGE*)SmartCpy(&ptemp, padmsg, sizeof(ADMINMESSAGE));
	STYLE = (STYLEROOMINFO*)SmartCpy(&ptemp, pmsg, sizeof(STYLEROOMINFO));
	return TRUE;
}
BOOL CNM_ADMIN_CHANGEROOM::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CNM_ADMIN_CHANGEROOM::Get() 메모리 할당 에러\n"); return FALSE;}
	pAMESG = (ADMINMESSAGE*)Jump(&ptemp, sizeof(ADMINMESSAGE) );
	STYLE = (STYLEROOMINFO*)Jump(&ptemp, sizeof(STYLEROOMINFO) );
	return TRUE;
}


/////////// 방 정보 변경 요청 알림
BOOL CSV_CHANGESTYLEROOMINFO::Set(CHANGESTYLEROOMINFO *pSRI)
{
	int msglen = sizeof(CHANGESTYLEROOMINFO);
	char* ptemp = SetHeader(SV_CHANGESTYLEROOMINFO, msglen);
	if(ptemp==NULL) {TRACE("CSV_CHANGESTYLEROOMINFO::Set() 메모리 할당 에러\n"); return FALSE;}
	SRI = (CHANGESTYLEROOMINFO*)SmartCpy(&ptemp, pSRI, sizeof(CHANGESTYLEROOMINFO));
	return TRUE;
}
BOOL CSV_CHANGESTYLEROOMINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_CHANGESTYLEROOMINFO::Get() 메모리 할당 에러\n"); return FALSE;}
	SRI = (CHANGESTYLEROOMINFO*)Jump(&ptemp, sizeof(CHANGESTYLEROOMINFO) );
	return TRUE;
}

// 어뷰져 놀려주기 
BOOL CNM_SPECIALMESSAGE::Set(ADMINMESSAGE *pmsg)
{
	int msglen =  sizeof(ADMINMESSAGE);
	char* ptemp = SetHeader(NM_SPECIALMESSAGE, msglen);
	if(ptemp==NULL) {TRACE("CNM_SPECIALMESSAGE::Set() 메모리 할당 에러\n"); return FALSE;}
	pAMESG = (ADMINMESSAGE*)SmartCpy(&ptemp, pmsg, sizeof(ADMINMESSAGE));
	return TRUE;
}
BOOL CNM_SPECIALMESSAGE::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CNM_SPECIALMESSAGE::Get() 메모리 할당 에러\n"); return FALSE;}
	pAMESG = (ADMINMESSAGE*)Jump(&ptemp, sizeof(ADMINMESSAGE) );
	return TRUE;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////
/////////////////// ### [ 관전기능 ] ###  ///////////////////
/////////////////////////////////////////////////////////////

////////// 겜참여 요청
BOOL CSV_ASK_PARTICIPATION::Set(int rnum, int unum, int servpnum, char *id, int sndfx)
{
	char strid[16]={0,};
	if(id != NULL) strncpy(strid, id, 15);
	int msglen = sizeof(*RoomNum) + sizeof(*UNum) + sizeof(*ServPNum) + 16 + sizeof(*SndFX); 
	char* ptemp = SetHeader(SV_ASK_PARTICIPATION, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_PARTICIPATION::Set() 메모리 할당 에러\n"); return FALSE;}
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
	if(ptemp==NULL) {TRACE("CSV_ASK_PARTICIPATION::Get() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ServPNum = (int*)Jump(&ptemp, sizeof(*ServPNum));
	ID = Jump(&ptemp, 16);
	SndFX = (int*)Jump(&ptemp, sizeof(*SndFX));
	return TRUE;
}



////////// 겜참여가능
BOOL CSV_ACCEPT_PARTICIPATION::Set(int rnum, int nowpnum, int newpnum, int sndKind, char *id, IPARRINDEX *pIPA)// 추가
{
	int msglen = sizeof(*RoomNum) + sizeof(*NowPNum) + sizeof(*NewPNum) + sizeof(*SndFxKind)+ 16 + sizeof(IPARRINDEX);
	char strid[16]={0,};
	if(id != NULL) strncpy(strid, id, 15);
	char* ptemp = SetHeader(SV_ACCEPT_PARTICIPATION, msglen);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_PARTICIPATION::Set() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	NowPNum = (int*)SmartCpy(&ptemp, &nowpnum, sizeof(*NowPNum));
	NewPNum = (int*)SmartCpy(&ptemp, &newpnum, sizeof(*NewPNum));
	SndFxKind = (int*)SmartCpy(&ptemp, &sndKind, sizeof(*SndFxKind)); // 추가
	ID = SmartCpy(&ptemp, strid, 16);
	IPA = (IPARRINDEX*)SmartCpy(&ptemp, pIPA, sizeof(IPARRINDEX));
	return TRUE;
}
BOOL CSV_ACCEPT_PARTICIPATION::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_PARTICIPATION::Get() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	NowPNum = (int*)Jump(&ptemp, sizeof(*NowPNum));
	NewPNum = (int*)Jump(&ptemp, sizeof(*NewPNum));
	SndFxKind = (int*)Jump(&ptemp, sizeof(*SndFxKind));// ### [사운드 추가 작업] ###
	ID = Jump(&ptemp, 16);
	IPA = (IPARRINDEX*)Jump(&ptemp, sizeof(IPARRINDEX));
	return TRUE;
}

////////// 다른유저에게 알림
BOOL CSV_ENTER_PARTICIPATION::Set(int rnum, int nowpnum, int newpnum, int sndKind, char *id, IPARRINDEX *pIPA)// 추가
{
	int msglen = sizeof(*RoomNum) + sizeof(*NowPNum) + sizeof(*NewPNum) + sizeof(*SndFxKind)+ 16 +sizeof(IPARRINDEX);
	char strid[16]={0,};
	if(id != NULL) strncpy(strid, id, 15);
	char* ptemp = SetHeader(SV_ENTER_PARTICIPATION, msglen);
	if(ptemp==NULL) {TRACE("CSV_ENTER_PARTICIPATION::Set() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	NowPNum = (int*)SmartCpy(&ptemp, &nowpnum, sizeof(*NowPNum));
	NewPNum = (int*)SmartCpy(&ptemp, &newpnum, sizeof(*NewPNum));
	SndFxKind = (int*)SmartCpy(&ptemp, &sndKind, sizeof(*SndFxKind)); // 추가
	ID = SmartCpy(&ptemp, strid, 16);
	IPA = (IPARRINDEX*)SmartCpy(&ptemp, pIPA, sizeof(IPARRINDEX));
	return TRUE;
}
BOOL CSV_ENTER_PARTICIPATION::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ENTER_PARTICIPATION::Get() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	NowPNum = (int*)Jump(&ptemp, sizeof(*NowPNum));
	NewPNum = (int*)Jump(&ptemp, sizeof(*NewPNum));
	SndFxKind = (int*)Jump(&ptemp, sizeof(*SndFxKind));// ### [사운드 추가 작업] ###
	ID = Jump(&ptemp, 16);
	IPA = (IPARRINDEX*)Jump(&ptemp, sizeof(IPARRINDEX));
	return TRUE;
}


////////// 겜참여 -> 관전으로 전환
BOOL CSV_ASK_EMERAGE::Set(int rnum, int unum, int servpnum, char *id)
{
	char strid[16]={0,};
	if(id != NULL) strncpy(strid, id, 15);
	int msglen = sizeof(*RoomNum) + sizeof(*UNum) + sizeof(*ServPNum) + 16; 
	char* ptemp = SetHeader(SV_ASK_EMERAGE, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_EMERAGE::Set() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ServPNum = (int*)SmartCpy(&ptemp, &servpnum, sizeof(*ServPNum));
	ID = SmartCpy(&ptemp, strid, 16);
	return TRUE;
}
BOOL CSV_ASK_EMERAGE::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_EMERAGE::Get() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ServPNum = (int*)Jump(&ptemp, sizeof(*ServPNum));
	ID = Jump(&ptemp, 16);
	return TRUE;
}


////////// 관전
BOOL CSV_ACCEPT_EMERAGE::Set(int rnum, int nowpnum, int newpnum,  char *id, IPARRINDEX *pIPA)// 추가
{
	int msglen = sizeof(*RoomNum) + sizeof(*NowPNum) + sizeof(*NewPNum) + 16 + sizeof(IPARRINDEX);
	char strid[16]={0,};
	if(id != NULL) strncpy(strid, id, 15);
	char* ptemp = SetHeader(SV_ACCEPT_EMERAGE, msglen);
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_EMERAGE::Set() 메모리 할당 에러\n"); return FALSE;}
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
	if(ptemp==NULL) {TRACE("CSV_ACCEPT_EMERAGE::Get() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	NowPNum = (int*)Jump(&ptemp, sizeof(*NowPNum));
	NewPNum = (int*)Jump(&ptemp, sizeof(*NewPNum));
	ID = Jump(&ptemp, 16);
	IPA = (IPARRINDEX*)Jump(&ptemp, sizeof(IPARRINDEX));
	return TRUE;
}


//////////// 관전 같은방의 다른유저에게 알린다
BOOL CSV_ENTER_EMERAGE::Set(int rnum, int nowpnum, int newpnum,  char *id, IPARRINDEX *pIPA)// 추가
{
	int msglen = sizeof(*RoomNum) + sizeof(*NowPNum) + sizeof(*NewPNum) + 16+sizeof(IPARRINDEX);
	char strid[16]={0,};
	if(id != NULL) strncpy(strid, id, 15);
	char* ptemp = SetHeader(SV_ENTER_EMERAGE, msglen);
	if(ptemp==NULL) {TRACE("CSV_ENTER_EMERAGE::Set() 메모리 할당 에러\n"); return FALSE;}
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
	if(ptemp==NULL) {TRACE("CSV_ENTER_EMERAGE::Get() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	NowPNum = (int*)Jump(&ptemp, sizeof(*NowPNum));
	NewPNum = (int*)Jump(&ptemp, sizeof(*NewPNum));
	ID = Jump(&ptemp, 16);
	IPA = (IPARRINDEX*)Jump(&ptemp, sizeof(IPARRINDEX));
	return TRUE;
}

/////////// 예약자 정보 전송
BOOL CSV_RESERVATIONINFO::Set(int totnum, RESERVATIONINFO *reservation)
{
	int msglen = sizeof(*TotNum) + sizeof(RESERVATIONINFO)*totnum;
	char* ptemp = SetHeader(SV_RESERVATIONINFO, msglen);
	if(ptemp==NULL) {TRACE("CSV_RESERVATIONINFO::Set() 메모리 할당 에러\n"); return FALSE;}
	TotNum = (int*)SmartCpy(&ptemp, &totnum, sizeof(*TotNum));
	ReservedInfo = (RESERVATIONINFO*)SmartCpy(&ptemp, reservation, sizeof(RESERVATIONINFO)*totnum);
	return TRUE;
}
BOOL CSV_RESERVATIONINFO::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_RESERVATIONINFO::Get() 메모리 할당 에러\n"); return FALSE;}
	TotNum   = (int*)Jump(&ptemp, sizeof(*TotNum));
	ReservedInfo = (RESERVATIONINFO*)ptemp;
	return TRUE;
}


/////////// 관전관련 알림 메세지
BOOL CSV_OBSERVER_MESSAGE::Set(int kind, char *chat)
{
	CString szChat;
	szChat.Format("%s", chat);
	short l_strchat = (short) szChat.GetLength();
	int msglen = sizeof(*Kind) + sizeof(l_strchat) + l_strchat;// + 4;

	char* ptemp = SetHeader(SV_OBSERVER_MESSAGE, msglen);
	if(ptemp==NULL) {TRACE("CSV_OBSERVER_MESSAGE::Set() 메모리 할당 에러\n"); return FALSE;}

	Kind      = (int*)SmartCpy(&ptemp, &kind, sizeof(*Kind));
	l_StrChat = (short*)SmartCpy(&ptemp, &l_strchat, sizeof(l_strchat));
	StrChat   = SmartCpy(&ptemp, (void*)szChat.operator LPCTSTR(), l_strchat);
	return TRUE;
}
BOOL CSV_OBSERVER_MESSAGE::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_OBSERVER_MESSAGE::Get() 메모리 할당 에러\n"); return FALSE;}
	Kind      =(int*)Jump(&ptemp, sizeof(*Kind));
	l_StrChat =(short*)Jump(&ptemp, sizeof(*l_StrChat));
	StrChat = ptemp;
	return TRUE;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


////////// 서버 공지 메세지
BOOL CSV_SERVERNOTIC::Set(int kind, char *chat, BOOL bcolorassigned, COLORREF color)
{
	CString szChat;
	szChat.Format("%s", chat);
	short l_strchat = (short) szChat.GetLength();
	int msglen = sizeof(*Kind) + sizeof(l_strchat) + l_strchat + sizeof(*bColorAssigned) + sizeof(*Color);

	char* ptemp = SetHeader(SV_SERVERNOTIC, msglen);
	if(ptemp==NULL) {TRACE("CSV_SERVERNOTIC::Set() 메모리 할당 에러\n"); return FALSE;}

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
	if(ptemp==NULL) {TRACE("CSV_SERVERNOTIC::Get() 메모리 할당 에러\n"); return FALSE;}
	Kind      = (int*)Jump(&ptemp, sizeof(*Kind));
	l_StrChat = (short*)Jump(&ptemp, sizeof(*l_StrChat));
	StrChat = Jump(&ptemp, *l_StrChat);
	bColorAssigned = (BOOL*)Jump(&ptemp, sizeof(*bColorAssigned));
	Color = (COLORREF*)Jump(&ptemp, sizeof(*Color));
	return TRUE;
}


// 카드 선택 타이머 알림
BOOL CSV_SELECTCARD::Set(int rnum, int unum, int servpnum, char *id)
{
	char strid[16]={0,};
	if(id != NULL) strncpy(strid, id, 15);
	int msglen = sizeof(*RoomNum) + sizeof(*UNum) + sizeof(*ServPNum) + 16; 
	char* ptemp = SetHeader(SV_SELECTCARD, msglen);
	if(ptemp==NULL) {TRACE("CSV_SELECTCARD::Set() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)SmartCpy(&ptemp, &rnum, sizeof(*RoomNum));
	UNum = (int*)SmartCpy(&ptemp, &unum, sizeof(*UNum));
	ServPNum = (int*)SmartCpy(&ptemp, &servpnum, sizeof(*ServPNum));
	ID = SmartCpy(&ptemp, strid, 16);
	return TRUE;
}
BOOL CSV_SELECTCARD::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_SELECTCARD::Get() 메모리 할당 에러\n"); return FALSE;}
	RoomNum = (int*)Jump(&ptemp, sizeof(*RoomNum));
	UNum = (int*)Jump(&ptemp, sizeof(*UNum));
	ServPNum = (int*)Jump(&ptemp, sizeof(*ServPNum));
	ID = Jump(&ptemp, 16);
	return TRUE;
}

// [수호천사] 2004.07.08 유저->사용유무 확인
BOOL CSV_ASK_SAFEANGEL::Set()
{
	int msglen = 0; 
	char* ptemp = SetHeader(SV_ASK_SAFEANGEL, msglen);
	if(ptemp==NULL) {TRACE("CSV_ASK_SAFEANGEL::Set() 메모리 할당 에러\n"); return FALSE;}
	return TRUE;
}

BOOL CSV_ASK_SAFEANGEL::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CSV_ASK_SAFEANGEL::Get() 메모리 할당 에러\n"); return FALSE;}
	return TRUE;
}

// [수호천사] 2004.07.08 머니 충전 메시지
BOOL CCL_CHARGE_SAFEANGEL::Set( char *id, int Kind, INT64 Money )
{
	char strid[16]={0,};
	if(id != NULL) strncpy(strid, id, 15);
	int msglen = sizeof(*kind) + sizeof(*money) + 16;
	char* ptemp = SetHeader(CL_CHARGE_SAFEANGEL, msglen);
	if(ptemp==NULL) {TRACE("CCL_CHARGE_SAFEANGEL::Set() 메모리 할당 에러\n"); return FALSE;}
	kind = (int*)SmartCpy( &ptemp, &Kind, sizeof(*kind) ); 
	money = (INT64*)SmartCpy( &ptemp, &Money, sizeof(*money) );
	ID = SmartCpy(&ptemp, strid, 16);
	return TRUE;
}

BOOL CCL_CHARGE_SAFEANGEL::Get(char* lpdata, int size)
{
	char* ptemp = GetHeader(lpdata, size);
	if(ptemp==NULL) {TRACE("CCL_CHARGE_SAFEANGEL::Get() 메모리 할당 에러\n"); return FALSE;}
	kind = (int*)Jump( &ptemp, sizeof(*kind) );
	money = (INT64*)Jump( &ptemp, sizeof(*money) );
	ID = Jump(&ptemp, 16);
	return TRUE;
}