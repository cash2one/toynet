// NoChat.cpp: implementation of the CNoChat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NoChat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNoChat::CNoChat()
{
	ZeroMemory(AddTime, sizeof(AddTime));
}

CNoChat::~CNoChat()
{

}

BOOL CNoChat::CheckNoChat(CString &str)
{
	return CheckNoChat((LPCTSTR)str);
}

BOOL CNoChat::CheckNoChat(LPCTSTR pstr)
{
	if(bNoChat == TRUE)
	{
		DWORD tick = timeGetTime() - NoChatTimeTick;
		if(tick < 0 || tick > 1000*120) // 120초간 채팅 금지
		{
			bNoChat = FALSE;
		}
		else return TRUE;
	}

	// 채팅 내용을 버퍼에 기록
	AddChat(pstr);

	int totline=0;
	NoChatTimeTick = timeGetTime();
	bNoChat = TRUE;

	// 버퍼에 기록된 총 라인수를 검사
	for(int i=0; i<10; i++)
	{
		if(History[i].GetLength()>0)
		{
			// 5줄을 검사하여 모두 같은 문자열인 경우 도배로 간주
			totline++;
			if(totline<5 && History[0].Compare(History[i])!=0) bNoChat=FALSE;
		}
		else { if(i<5) bNoChat=FALSE; break; }
	}

	// 5줄이 모두 같은 문자열이더라도 방금전의 문자열과 시간간격이 넓다면 도배가 아님
	if(bNoChat) 
	{
		DWORD interval = AddTime[0] - AddTime[4];
		// 5줄 전의 문자열과의 시간차가 5초 이상이라면
		if(interval<0 || interval > 1000*5) bNoChat = FALSE;
	}

	// 마지막으로 도배 시간을 검사
	if(!bNoChat && totline > 5)
	{
		DWORD interval = AddTime[0] - AddTime[5];
		// 5줄이 3초 이내에 씌여진 경우면 도배로 간주
		if(interval>=0 && interval < 1000*3) bNoChat = TRUE;
	}
	
	return FALSE;
}

void CNoChat::AddChat(LPCTSTR pstr)
{
	// 히스토리 갱신
	for(int i=9; i>0; i--)
	{
		AddTime[i] = AddTime[i-1];
		History[i] = History[i-1];
	}
	History[0].Format("%s",pstr);
	AddTime[0] = timeGetTime();
}