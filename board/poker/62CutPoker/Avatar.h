// Avatar.h: interface for the CAvatar class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __AVATAR_H__
#define __AVATAR_H__

#define APF_RGB555		555		// 타겟의 픽셀 포멧이 16bit RGB 5:5:5인 경우
#define APF_RGB565		565		// 타겟의 픽셀 포멧이 16bit RGB 5:6:5인 경우
#define APF_RGB24		24		// 타겟의 픽셀 포멧이 24bit인 경우
#define APF_RGB32		32		// 타겟의 픽셀 포멧이 32bit인 경우

//////////////////////////////////////////////////////////////////////
typedef UINT HAVATAR;

// 아바타 모듈 초기화 함수(모듈의 해제는 프로그램 종료시 자동으로 수행된다)
BOOL InitAvatarModule();

// 테스트용 초기화 함수
BOOL InitAvatarModule_ForTest(LPCSTR strBaseDir, LPCTSTR strDllPath);

// 아바타 모듈 기본 폴더를 얻는 함수
BOOL GetBaseAvatarDir(char *strBuffer, int buffersize);

//////////////////////////////////////////////////////////////////////
// 아바타 클래스

class CAvatar  
{
public:
	HAVATAR hAvatar;

	CAvatar();
	virtual ~CAvatar();

	// 아바타 초기화 및 생성
	BOOL Create();
	BOOL Create(short *pAvatarInfo);
	BOOL Create(int width, int height, short *pAvatarInfo);

	void ClearMember();
	void Destroy();											// 아바타 파괴

	BOOL SetAvatar(short *pAvatarInfo);						// 아바타 정보 설정
	BOOL SetSize(int width, int height);					// 아바타 사이즈 설정
	BOOL SetOffset(int ox, int oy);							// 아바타 오프셋 설정(디폴트 0,0)
	BOOL ResetAnimation();									// 아바타 에니메이션 리셋
	BOOL SetAnimationTime(WORD tick);						// 아바타 에니메이션 타임을 세팅

	// 아바타를 DC에 그리기
	BOOL Draw(int xp, int yp, HDC hdcDest, BOOL bMirror);	
	// 아바타를 메모리에 그리기(디폴트는 16bit RGB555)
	BOOL Draw(int xp, int yp, LONG txl, LONG tyl, void *lpbit, LONG lpitch, BOOL bMirror, int TargetPixelFormat = APF_RGB555);
};


#endif // !defined(__AVATAR_H__)
