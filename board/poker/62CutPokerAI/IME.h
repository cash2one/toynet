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

/*//////////////// Ŭ���� ���� //////////////
  
	CIME Ime;				// ���ڿ��� �Է¹��� ������ Ŭ���� �ɹ��� �����Ѵ�
	Ime.Init(�������ڵ�);	// �ʱ�ȭ..

    �޼��� �������� Ime.OnMessge(.....)�� ��������ְ� ���ϰ��� TRUE�̸�...
	����Ʈ ������ ���ν����� �ѱ��� �ʴ´�.

	WM_CHAR �޼����� �޾Ƽ� ������ ���ۿ� ���ڿ��� ���������ش�.
	(�ѱ��� ���յǾ� �߻��ϴ� WM_CHAR�� 2-3����Ʈ�� �� �� �����Ƿ� ��쿡���� ������ ����Ұ�)

    Ime.GetNowStr()�� �̿��Ͽ� ���� �������� ���ڿ��� ���� �� �ִ�.
	(GetNowStr()�� NULL�� �����Ѵٸ� �ѱ� ���ջ��°� �ƴ�)

/////////////////////////////////////////////*/

class CIME  
{
public:
	
	HIMC hIMC;			// IME Context �ڵ�
	HWND hWndParent;	// WM_CHAR �޼����� ���� ������ �ڵ�

	// IME : �ѱ� �Է� ���丶Ÿ ���¸� ��Ÿ���� ���� 
	int IME_STATE; 
	
	// �ϼ��� ���� ����
	char char_key[5]; 

	CIME();
	virtual ~CIME();

	// IME �ʱ�ȭ(hParent : WM_CHAR�� ���� �θ�������)
	void Init(HWND hParent);

	// ���� �������ΰ�??
	BOOL IsNowComp();

	// ���� �������� ���ڿ��� ����(���������� �������� NULL�� �����Ѵ�)
	char* GetNowStr();

	// Ŭ������ ó���ؾ��� �޼����� �ڵ����� ó����(ó�������� TRUE�� ����(ó���ߴٸ� ����Ʈ ���ν����� �ѱ�� �ȵȴ�))
	BOOL OnMessage(UINT message, DWORD wParam, DWORD lParam);
	
	// �������� �޽��� ó���Լ���...
	void OnImeStartComposition();
	void OnImeComposition(DWORD wParam, DWORD lParam);
	void OnImeEndComposition();
};

#endif // !defined(AFX_IME_H__E2A42C9A_0FBE_11D4_97A2_0050BF0FBE67__INCLUDED_)
