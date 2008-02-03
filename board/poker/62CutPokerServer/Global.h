// Global.h: interface for the CGlobal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBAL_H__088242D5_7C74_485C_A366_CD968D5055EA__INCLUDED_)
#define AFX_GLOBAL_H__088242D5_7C74_485C_A366_CD968D5055EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



// ### [�α� ��Ͽ�] ###
#define LOG_RECORD_


typedef struct
{
	char ChanName[20];
	int  GameKind;
	int  LimitLevel1;
	int  LimitLevel2;
	int  Grade;
} CHANINFO_CFG;

CString g_MakeCommaMoney(INT64 nMoney);

void GetLevelName(int lv, char *buf);
void GetSuitableServerName(int lv, char *buf);

void GetSuitableChanelName(int lv, char *buf);

void GetGradeName(int lv, char *buf);

//int GetMyLeveltoGrade(int lv);

////////////////////////////////// ������� //////////////////////////////

// ��ȯ
#define Swap(a,b) {(a)^=(b); (b)^=(a); (a)^=(b);}

// �ּҰ�
template <class A, class B> inline A Min(A a, B b) { return a>b ? b : a; }
// �ִ밪
template <class A, class B> inline A Max(A a, B b) { return a>b ? a : b; }
// �ΰ��� �Ÿ����ϱ�
template <class A, class B> inline A Dist(A a, B b) { return ( Max(a,b)-Min(a,b) ); }

////////////////////////////////// �⺻���� //////////////////////////////

int __fastcall faststrcmp(LPCTSTR str1, LPCTSTR str2);
int __fastcall faststrncmp(LPCTSTR str1, LPCTSTR str2, int slen);


//// Socket Send Packet ������ ����
char* GetSendBuf();

// ������ ����ϴ� ���� �̸��� �״�� ����ϱ� ���Ͽ� Define
#define SndBuf	(GetSendBuf())


#endif // !defined(AFX_GLOBAL_H__088242D5_7C74_485C_A366_CD968D5055EA__INCLUDED_)
