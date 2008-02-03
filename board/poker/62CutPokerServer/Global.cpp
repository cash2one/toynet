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

	if( lv == 0 ) strcpy( buf, "��Ŭ�ι�" );
	else if( lv == 1 ) strcpy( buf, "����Ʈ" );
	else if( lv == 2 ) strcpy( buf, "����̾�" );
	else if( lv == 3 ) strcpy( buf, "�潺���̵�" );
	else if( lv == 4 ) strcpy( buf, "��Ŭ�ι�" );
	else if( lv == 5 ) strcpy( buf, "����Ʈ" );
	else if( lv == 6 ) strcpy( buf, "�����̾�" );
	else if( lv == 7 ) strcpy( buf, "�������̵�" );
	else if( lv == 8 ) strcpy( buf, "��Ŭ�ι�" );
	else if( lv == 9 ) strcpy( buf, "����Ʈ" );
	else if( lv == 10 ) strcpy( buf, "�ݴ��̾�" );
	else if( lv == 11 ) strcpy( buf, "�ݽ����̵�" );
	else if( lv == 12 ) strcpy( buf, "����" );
	else if( lv == 13 ) strcpy( buf, "�ݺ�" );
	else if( lv == 14 ) strcpy( buf, "�հ�" );
	else strcpy( buf, "����" );

}


void GetSuitableServerName(int lv, char *buf)
{
	if( buf == NULL ) return;
		
	else if( lv > 11 ) strcpy( buf, "���� ����" );
	else if( lv > 7  ) strcpy( buf, "��� ����" );
	else if( lv > 3  ) strcpy( buf, "�߼� ����" );
	else strcpy( buf, "�ʺ� ����" );

}

void GetSuitableChanelName(int lv, char *buf)
{
	if( buf == NULL ) return;

	else if( lv > 11 ) strcpy( buf, "���� ä��" );
	else if( lv > 7  ) strcpy( buf, "��� ä��" );
	else if( lv > 3  ) strcpy( buf, "�߼� ä��" );
	else strcpy( buf, "�ʺ� ä��" );

}

// ����̸� ���ϱ�
void GetGradeName(int lv, char *buf)
{
	if( buf == NULL ) return;

	else if( lv > 11 ) strcpy( buf, "���ŵ��" );
	else if( lv > 7  ) strcpy( buf, "������" );
	else if( lv > 3  ) strcpy( buf, "�߼����" );
	else strcpy( buf, "�ʺ����" );
}



////////////////////////////////////////////////////////
// ��Ŷ ������ ���� �ӽù��۸� ���ϴ� �Լ�
// �������� ��Ŷ�� ���ÿ� ������ ��쿡 �� ���۰� �ߺ����� �ʵ��� 
// ���� �迭�� �غ��Ѵ�

#define MAX_SNDBUFNUM	20

static UINT s_lastcnt = 0;
static char s_SndBuf[MAX_SNDBUFNUM][SENDQUESIZE] = {0,};

char* GetSendBuf() 
{ 
	if(s_lastcnt >= MAX_SNDBUFNUM) s_lastcnt = 0; 
	return s_SndBuf[s_lastcnt++]; 
}

///////////////////////////////////////////////////////////

