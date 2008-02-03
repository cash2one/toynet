// Global.cpp: implementation of the CGlobal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "62CutPokerServer.h"
#include "Global.h"

#include "SockMan.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int __fastcall faststrcmp(LPCTSTR str1, LPCTSTR str2)
{
	for(register int i=0; ; i++) {
		if(str1[i] != str2[i]) return 1;
		if(!str1[i] && !str2[i]) return 0;
	}
	return 1;
}

int __fastcall faststrncmp(LPCTSTR str1, LPCTSTR str2, int slen)
{
	for(register int i=0; ; i++) {
		if(i >= slen) return 0;
		if(str1[i] != str2[i]) return 1;
		if(!str1[i] && !str2[i]) return 0;
	}
	return 1;
}


CString g_MakeCommaMoney(INT64 nMoney)
{
	CString strVal("0");

	if(nMoney<0) return strVal;
	strVal.Format("%I64d",nMoney);

	for(int i=18; i>0; i-=3){
		if(strVal.GetLength()>i){
			strVal.Insert(strVal.GetLength()-i,",");
		}
	}
	if(strVal=="") strVal= "0";
	return strVal;
}


void GetLevelName(int lv, char *buf)
{
	if( buf == NULL ) return;

	if( lv == 0 ) strcpy( buf, "흑클로버" );
	else if( lv == 1 ) strcpy( buf, "흑하트" );
	else if( lv == 2 ) strcpy( buf, "흑다이아" );
	else if( lv == 3 ) strcpy( buf, "흑스페이드" );
	else if( lv == 4 ) strcpy( buf, "은클로버" );
	else if( lv == 5 ) strcpy( buf, "은하트" );
	else if( lv == 6 ) strcpy( buf, "은다이아" );
	else if( lv == 7 ) strcpy( buf, "은스페이드" );
	else if( lv == 8 ) strcpy( buf, "금클로버" );
	else if( lv == 9 ) strcpy( buf, "금하트" );
	else if( lv == 10 ) strcpy( buf, "금다이아" );
	else if( lv == 11 ) strcpy( buf, "금스페이드" );
	else if( lv == 12 ) strcpy( buf, "은별" );
	else if( lv == 13 ) strcpy( buf, "금별" );
	else if( lv == 14 ) strcpy( buf, "왕관" );
	else strcpy( buf, "도신" );

}


void GetSuitableServerName(int lv, char *buf)
{
	if( buf == NULL ) return;
		
	else if( lv > 11 ) strcpy( buf, "도신 서버" );
	else if( lv > 7  ) strcpy( buf, "고수 서버" );
	else if( lv > 3  ) strcpy( buf, "중수 서버" );
	else strcpy( buf, "초보 서버" );

}

void GetSuitableChanelName(int lv, char *buf)
{
	if( buf == NULL ) return;

	else if( lv > 11 ) strcpy( buf, "도신 채널" );
	else if( lv > 7  ) strcpy( buf, "고수 채널" );
	else if( lv > 3  ) strcpy( buf, "중수 채널" );
	else strcpy( buf, "초보 채널" );

}

// 등급이름 구하기
void GetGradeName(int lv, char *buf)
{
	if( buf == NULL ) return;

	else if( lv > 11 ) strcpy( buf, "도신등급" );
	else if( lv > 7  ) strcpy( buf, "고수등급" );
	else if( lv > 3  ) strcpy( buf, "중수등급" );
	else strcpy( buf, "초보등급" );
}



////////////////////////////////////////////////////////
// 패킷 구성을 위한 임시버퍼를 구하는 함수
// 여러개의 패킷을 동시에 구성할 경우에 각 버퍼간 중복되지 않도록 
// 여러 배열을 준비한다

#define MAX_SNDBUFNUM	20

static UINT s_lastcnt = 0;
static char s_SndBuf[MAX_SNDBUFNUM][SENDQUESIZE] = {0,};

char* GetSendBuf() 
{ 
	if(s_lastcnt >= MAX_SNDBUFNUM) s_lastcnt = 0; 
	return s_SndBuf[s_lastcnt++]; 
}

///////////////////////////////////////////////////////////

