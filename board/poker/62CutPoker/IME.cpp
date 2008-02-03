// IME.cpp: implementation of the CIME class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IME.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIME::CIME()
{
	hIMC = NULL;
	hWndParent = NULL;
	IME_STATE = IME_END;
	memset(char_key, 0, sizeof(char_key));
}

CIME::~CIME()
{

}

void CIME::Init(HWND hParent)
{
	hWndParent = hParent;
	hIMC = ImmGetContext(hWndParent);
	IME_STATE = IME_END;
	memset(char_key, 0, sizeof(char_key));
}

BOOL CIME::IsNowComp()
{
	return(IME_STATE == IME_PROCESS);
}

char* CIME::GetNowStr()
{ 
	if(IME_STATE == IME_END) return NULL;
	return char_key;
} 

BOOL CIME::OnMessage(UINT message, DWORD wParam, DWORD lParam)
{ 
	switch(message)
	{
	case WM_IME_STARTCOMPOSITION : OnImeStartComposition(); return TRUE;
	case WM_IME_COMPOSITION : OnImeComposition(wParam, lParam); return TRUE;
	case WM_IME_ENDCOMPOSITION : OnImeEndComposition(); return TRUE;
	}

	return FALSE;
} 

void CIME::OnImeStartComposition()
{ 
	IME_STATE = IME_PROCESS; 
} 

void CIME::OnImeEndComposition()
{ 
	IME_STATE = IME_END;
} 

void CIME::OnImeComposition(DWORD wParam, DWORD lParam)
{ 
	int rep_cnt; 
	char code_1, code_2; 
	char key_buf[5]={0,}; 

	if ( lParam == GCS_RESULTSTR )
	{ 
		memset(char_key, 0, sizeof(char_key));
		ImmGetCompositionString(hIMC, GCS_RESULTSTR, char_key, 5);

		UINT uStr = 0; //*((WORD*)char_key);
		memcpy(&uStr, char_key, strlen(char_key));
		::SendMessage(hWndParent, WM_CHAR, uStr, 0);
	}
	else // 글자가 아직 미완성인경우 
	{ 
		// 입력된 완성형 한글을 메세지로 얻어낸다. 
		rep_cnt = (UINT )lParam; 
		code_1 = (char )(wParam >> 32); 
		code_2 = (char )(wParam >> 40); 
		key_buf[0] = code_2; 
		key_buf[1] = code_1; 
		key_buf[2] = '\0'; 

		strcpy(char_key, key_buf);   
	}
} 
