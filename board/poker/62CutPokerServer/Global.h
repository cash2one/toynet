// Global.h: interface for the CGlobal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBAL_H__088242D5_7C74_485C_A366_CD968D5055EA__INCLUDED_)
#define AFX_GLOBAL_H__088242D5_7C74_485C_A366_CD968D5055EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



// ### [로그 기록용] ###
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

////////////////////////////////// 산술연산 //////////////////////////////

// 교환
#define Swap(a,b) {(a)^=(b); (b)^=(a); (a)^=(b);}

// 최소값
template <class A, class B> inline A Min(A a, B b) { return a>b ? b : a; }
// 최대값
template <class A, class B> inline A Max(A a, B b) { return a>b ? a : b; }
// 두값의 거리구하기
template <class A, class B> inline A Dist(A a, B b) { return ( Max(a,b)-Min(a,b) ); }

////////////////////////////////// 기본연산 //////////////////////////////

int __fastcall faststrcmp(LPCTSTR str1, LPCTSTR str2);
int __fastcall faststrncmp(LPCTSTR str1, LPCTSTR str2, int slen);


//// Socket Send Packet 구성용 버퍼
char* GetSendBuf();

// 기존에 사용하던 버퍼 이름을 그대로 사용하기 위하여 Define
#define SndBuf	(GetSendBuf())


#endif // !defined(AFX_GLOBAL_H__088242D5_7C74_485C_A366_CD968D5055EA__INCLUDED_)
