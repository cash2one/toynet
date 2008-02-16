// Avatar.h: interface for the CAvatar class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __AVATAR_H__
#define __AVATAR_H__

#define APF_RGB555		555		// Ÿ���� �ȼ� ������ 16bit RGB 5:5:5�� ���
#define APF_RGB565		565		// Ÿ���� �ȼ� ������ 16bit RGB 5:6:5�� ���
#define APF_RGB24		24		// Ÿ���� �ȼ� ������ 24bit�� ���
#define APF_RGB32		32		// Ÿ���� �ȼ� ������ 32bit�� ���

//////////////////////////////////////////////////////////////////////
typedef UINT HAVATAR;

// �ƹ�Ÿ ��� �ʱ�ȭ �Լ�(����� ������ ���α׷� ����� �ڵ����� ����ȴ�)
BOOL InitAvatarModule();

// �׽�Ʈ�� �ʱ�ȭ �Լ�
BOOL InitAvatarModule_ForTest(LPCSTR strBaseDir, LPCTSTR strDllPath);

// �ƹ�Ÿ ��� �⺻ ������ ��� �Լ�
BOOL GetBaseAvatarDir(char *strBuffer, int buffersize);

//////////////////////////////////////////////////////////////////////
// �ƹ�Ÿ Ŭ����

class CAvatar  
{
public:
	HAVATAR hAvatar;

	CAvatar();
	virtual ~CAvatar();

	// �ƹ�Ÿ �ʱ�ȭ �� ����
	BOOL Create();
	BOOL Create(short *pAvatarInfo);
	BOOL Create(int width, int height, short *pAvatarInfo);

	void ClearMember();
	void Destroy();											// �ƹ�Ÿ �ı�

	BOOL SetAvatar(short *pAvatarInfo);						// �ƹ�Ÿ ���� ����
	BOOL SetSize(int width, int height);					// �ƹ�Ÿ ������ ����
	BOOL SetOffset(int ox, int oy);							// �ƹ�Ÿ ������ ����(����Ʈ 0,0)
	BOOL ResetAnimation();									// �ƹ�Ÿ ���ϸ��̼� ����
	BOOL SetAnimationTime(WORD tick);						// �ƹ�Ÿ ���ϸ��̼� Ÿ���� ����

	// �ƹ�Ÿ�� DC�� �׸���
	BOOL Draw(int xp, int yp, HDC hdcDest, BOOL bMirror);	
	// �ƹ�Ÿ�� �޸𸮿� �׸���(����Ʈ�� 16bit RGB555)
	BOOL Draw(int xp, int yp, LONG txl, LONG tyl, void *lpbit, LONG lpitch, BOOL bMirror, int TargetPixelFormat = APF_RGB555);
};


#endif // !defined(__AVATAR_H__)
