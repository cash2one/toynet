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
		if(tick < 0 || tick > 1000*120) // 120�ʰ� ä�� ����
		{
			bNoChat = FALSE;
		}
		else return TRUE;
	}

	// ä�� ������ ���ۿ� ���
	AddChat(pstr);

	int totline=0;
	NoChatTimeTick = timeGetTime();
	bNoChat = TRUE;

	// ���ۿ� ��ϵ� �� ���μ��� �˻�
	for(int i=0; i<10; i++)
	{
		if(History[i].GetLength()>0)
		{
			// 5���� �˻��Ͽ� ��� ���� ���ڿ��� ��� ����� ����
			totline++;
			if(totline<5 && History[0].Compare(History[i])!=0) bNoChat=FALSE;
		}
		else { if(i<5) bNoChat=FALSE; break; }
	}

	// 5���� ��� ���� ���ڿ��̴��� ������� ���ڿ��� �ð������� �дٸ� ���谡 �ƴ�
	if(bNoChat) 
	{
		DWORD interval = AddTime[0] - AddTime[4];
		// 5�� ���� ���ڿ����� �ð����� 5�� �̻��̶��
		if(interval<0 || interval > 1000*5) bNoChat = FALSE;
	}

	// ���������� ���� �ð��� �˻�
	if(!bNoChat && totline > 5)
	{
		DWORD interval = AddTime[0] - AddTime[5];
		// 5���� 3�� �̳��� ������ ���� ����� ����
		if(interval>=0 && interval < 1000*3) bNoChat = TRUE;
	}
	
	return FALSE;
}

void CNoChat::AddChat(LPCTSTR pstr)
{
	// �����丮 ����
	for(int i=9; i>0; i--)
	{
		AddTime[i] = AddTime[i-1];
		History[i] = History[i-1];
	}
	History[0].Format("%s",pstr);
	AddTime[0] = timeGetTime();
}