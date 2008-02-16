// IME.h: interface for the CIME class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IME_H__E2A42C9A_0FBE_11D4_97A2_0050BF0FBE67__INCLUDED_)
#define AFX_IME_H__E2A42C9A_0FBE_11D4_97A2_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <imm.h>
#pragma comment(lib,"imm32.lib")

#define IME_PROCESS  128 
#define IME_END  129

/*//////////////// 클래스 사용법 //////////////
  
	CIME Ime;				// 문자열을 입력받을 윈도에 클래스 맴버를 선언한다
	Ime.Init(윈도우핸들);	// 초기화..

    메세지 루프에서 Ime.OnMessge(.....)를 실행시켜주고 리턴값이 TRUE이면...
	디폴트 윈도우 프로시져로 넘기지 않는다.

	WM_CHAR 메세지를 받아서 적당한 버퍼에 문자열을 누적시켜준다.
	(한글이 조합되어 발생하는 WM_CHAR는 2-3바이트가 될 수 있으므로 경우에따라 적절히 계산할것)

    Ime.GetNowStr()을 이용하여 현재 조합중인 문자열을 얻을 수 있다.
	(GetNowStr()이 NULL을 리턴한다면 한글 조합상태가 아님)

/////////////////////////////////////////////*/

class CIME  
{
public:
	
	HIMC hIMC;			// IME Context 핸들
	HWND hWndParent;	// WM_CHAR 메세지를 보낼 윈도우 핸들

	// IME : 한글 입력 오토마타 상태를 나타내는 변수 
	int IME_STATE; 
	
	// 완성된 문자 버퍼
	char char_key[5]; 

	CIME();
	virtual ~CIME();

	// IME 초기화(hParent : WM_CHAR를 받을 부모윈도우)
	void Init(HWND hParent);

	// 현재 조합중인가??
	BOOL IsNowComp();

	// 현재 조합중인 문자열을 얻음(조합중이지 않을때는 NULL을 리턴한다)
	char* GetNowStr();

	// 클래스가 처리해야할 메세지를 자동으로 처리함(처리했으면 TRUE를 리턴(처리했다면 디폴트 프로시져로 넘기면 안된다))
	BOOL OnMessage(UINT message, DWORD wParam, DWORD lParam);
	
	// 내부적인 메시지 처리함수들...
	void OnImeStartComposition();
	void OnImeComposition(DWORD wParam, DWORD lParam);
	void OnImeEndComposition();
};

#endif // !defined(AFX_IME_H__E2A42C9A_0FBE_11D4_97A2_0050BF0FBE67__INCLUDED_)
