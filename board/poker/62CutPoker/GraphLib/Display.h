#ifndef __DISPLAY_H__
#define __DISPLAY_H__


#pragma comment(lib,"ddraw.lib")
#pragma comment(lib,"dxguid.lib")

#include "ddraw.h"
#include "wchar.h"

class CDisplay
{
public:
	LPDIRECTDRAW2        lpDD;
	LPDIRECTDRAWSURFACE3 lpPrimary;
	//LPDIRECTDRAWSURFACE3 lpBack;

	///// ������ ����
	HWND winHandle;
	BOOL DDRAWINIT;		// ���̷�Ʈ ��ο� �ʱ�ȭ ���� ����

	CDisplay();
	~CDisplay();
	BOOL InitDirectDraw(HWND hwnd, int xl, int yl, int cBit);
	void CloseDirectDraw();

	LPSTR LockPrimary(DWORD *lPitch);
	void UnlockPrimary();
};

#endif