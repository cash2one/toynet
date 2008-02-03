
#ifndef __GRAPHIC_H__
#define __GRAPHIC_H__


#include <windows.h>
#include <stdio.h>
#include <memory.h>

#include "GraphMod.h"	// 16비트 스프라이트 출력 모듈
#include "xSprite.h"	// 16 비트 스프라이트 클래스
#include "GraphMod8.h"	// 8비트 스프라이트 출력 모듈
#include "Sprite8.h"	// 8 비트 스프라이트 클래스


// 스프라이트 출력효과 설정 플래그
#define LR		((DWORD)1)		// 0000000000000001(b) (좌우반전)
#define UD		((DWORD)2)		// 0000000000000010(b) (상하반전)
#define HB		((DWORD)4)		// 0000000000000100(b) (반투명효과)
#define GB		((DWORD)8)		// 0000000000001000(b) (다단계 반투명효과)
#define GLOW	((DWORD)16)		// 0000000000010000(b) (글로우 필터효과 - 색톤 변경)
#define GRAYSCL	((DWORD)32)		// 0000000000100000(b) (그레이스케일 필터효과 - 흑백 출력)
#define MONO	((DWORD)64)		// 0000000001000000(b) (모노 필터효과 - 단색 출력)
#define ANTI	((DWORD)128)	// 0000000010000000(b) (안티효과)
#define LE		((DWORD)256)	// 0000000100000000(b) (광원효과)
#define DE		((DWORD)512)	// 0000001000000000(b) (Darken Effect)
#define RXY		((DWORD)1024)	// 0000010000000000(b) (상대좌표 적용)
#define CENTER	((DWORD)2048)	// 0000100000000000(b) (확대시 캐릭터를 좌표의 중심에 위치시킨다)
#define PERCENT	((DWORD)4096)	// 0001000000000000(b) (확대시 비율에 따른 확대)
#define MIRROR  ((DWORD)8192)	// 0010000000000000(b) (좌우 뒤집기-LR과 달리 상대좌표도 함께 반전)
#define TEMPCLIP ((DWORD)16384)	// 0100000000000000(b) (임시 클리핑 영역을 사용함)


BOOL PointHitTest(xSprite *pSpr, int sprnum, int x, int y, DWORD opt=0);


class CPage
{
public:
	LPBITMAPINFO lpBmpInfo;     // 비트맵 정보
	LPLOGPALETTE lpLogPal;      // 로그 파레트 포인터
	HPALETTE	 hPal;	        // DIB형식의 팔레트 핸들
	HBITMAP      hBmp;			// 비트맵의 핸들(CreateDIBSection()으로 생성된 핸들)
	DWORD nAllocSize;

	void* lpBit;                // 페이지 포인터
	LONG  lPitch;               // PITCH(4의배수) - 실제의 페이지의 가로 사이즈
	LONG  lPitchHalf;           // lPitch/2
	LONG  width, height;        // 페이지의 유효 범위 길이(최대 클리핑 지역)
	WORD  nBitNum;              // 비트수
	LONG  nBPD;                 // 도트당 바이트 수
	WORD  nColorNum;            // 칼라수
	
	CPage();
	~CPage();

	// 8비트 알파 테이블을 생성하였는가?
	BOOL bCreateAlphaTable8;

	// 임시 클리핑 영역
	int TClipX1, TClipY1, TClipX2, TClipY2;
	void SetTempClip(int x1, int y1, int x2, int y2);		// 스프라이트를 그릴때 임시 클리핑 영역을 지정(그리기 옵션에서 TMPCLIP을 적용해야함)

	// 미러 찍기 정보
	int MirrorX;
	void SetMirrorX(int x);									// 스프라이트 그리기 옵션중 미러(MIRROR)를 적용하는 경우 뒤집는 중심 위치

	// Attach, Detach
	BOOL bAttached;
	BOOL Attach(LONG xl, LONG yl, void *lpbit, LONG lpitch);
	BOOL Detach();

	
	///////// 페이지 관련

	BOOL Init(LONG xl, LONG yl, INT bit, RGBQUAD *lpPal=NULL, LPCTSTR palfname=NULL);				// 페이지 초기화
	void Destroy();																					// 페이지 파괴
	BOOL SaveToBitmap(LPSTR fname);																	// 페이지의 내용을 비트맵으로 저장
	BOOL SaveToJpeg(LPSTR fname, int quality);	// quality = 0-100									// 페이지의 내용을 JPG로 저장
	BOOL ReCreateDIBSection();
	BOOL OpenPal(LPCTSTR filename);
	BOOL OpenAlphaTable8(LPCTSTR filename);
	BOOL SetPal(RGBQUAD *lpPal);
	BOOL SetLogPal(LPLOGPALETTE lpPal);
	BOOL SetPalHandle(HPALETTE h);
	BOOL PutFromPage(LONG xp, LONG yp, CPage *pPage, int blendgrade=31);							// 현재의 페이지에 다른 페이지의 이미지를 다단계 반투명으로 그림
	BOOL PutFromPageWithColorKey(LONG xp, LONG yp, CPage *pPage, WORD colorkey, int blendgrade=31);	// 현재의 페이지에 다른 페이지의 이미지를 다단계 반투명으로 그림(컬러키 적용)
	BOOL FillGDI(HDC hTarDC, int nXStart, int nYStart, COLORREF crColor);							// DC의 특정영역을 특정 컬러로 체워줌
	BOOL FlipToGDI(HWND hwnd, LONG xp=0, LONG yp=0, int ZoomX=1);									// 페이지의 내용을 윈도우 클라이언트 영역에 그려줌
	BOOL FlipToGDI(HDC hdc, LONG xp=0, LONG yp=0, int ZoomX=1);										// 페이지의 내용을 GDI에 그려줌
	void SetFlipPos(LONG xp, LONG yp, int zoom);													// Flip()함수 사용시 페이지가 그려질 위치 지정
	BOOL Flip(HWND hwnd);																			// 특정 윈도우의 클라이언트 영역에 페이지 내용을 그려줌
	BOOL Flip(HDC hdc);																				// 특정 DC에 페이지 내용을 그려줌
	BOOL StretchFlip(HWND hwnd, RECT* rcDest, RECT* rcSrc);											// 특정 윈도우의 클라이언트 영역에 페이지 내용을 타겟 영역에 맞게 늘려서 그려줌

	///////// 페이지 정보관련

	int  xpFlip, ypFlip;
	int  iZoomX;

	void ToPagePos(LONG xp, LONG yp, LONG* X, LONG* Y);

	///////// 드로잉 관련
	void HLine(int x, int y, int len, WORD color, BOOL flag);						// 수평선을 그림
	void VLine(int x, int y, int len, WORD color, BOOL flag);						// 수직선을 그림
	void DrawLine(int x1, int y1, int x2, int y2, WORD color);						// 선을 그림(브레제남 알고리즘)
	void DrawEmptyBox(int x1, int y1, int x2, int y2, WORD color, BOOL flag=0);		// 속이 빈 박스를 그림
	void DrawFillBox(int x1, int y1, int x2, int y2, WORD color);					// 속을 채운 박스를 그림
	void DrawFillBoxGB(int x1, int y1, int x2, int y2, WORD color, int grade);		// 속을 채운 박스를 그림(다단게 반투명)
	void DrawSelectBox(int x1, int y1, int x2, int y2, WORD color, BOOL flag=0);	// 선택 상자를 그림

	///////// 스프라이트 관련

	// 16비트 스프라이트
	BOOL PutSprAuto(int x, int y, xSprite *pXSpr, int sprnum, int opt=NULL, int grade=NULL , WORD color=NULL);
	BOOL PutSprAuto(int x, int y, SPRITE30 *pSpr, int opt=NULL, int grade=NULL , WORD color=NULL);
	BOOL ZoomSprAuto(int x, int y, xSprite *pXSpr, int sprnum, int tx, int ty, int opt=NULL, int grade=NULL, WORD color=NULL);
	BOOL ZoomSprAuto(int x, int y, SPRITE30 *pSpr, int tx, int ty, int opt=NULL, int grade=NULL, WORD color=NULL);
	BOOL PutImageAuto(int x, int y, xSprite *pXSpr, int sprnum, int opt=NULL, int grade=NULL, WORD color=NULL);
	BOOL PutImageAuto(int x, int y, SPRITE30 *pSpr, int opt=NULL, int grade=NULL, WORD color=NULL);

	// 8비트 스프라이트
	BOOL PutSprAuto(int x, int y, CSprite8 *pSpr8, int sprnum, int opt=NULL, int grade=NULL , BYTE color=NULL);
	BOOL PutSprAuto(int x, int y, SPRITE8 *pSpr, int opt=NULL, int grade=NULL , BYTE color=NULL);
	BOOL ZoomSprAuto(int x, int y, CSprite8 *pSpr8, int sprnum, int tx, int ty, int opt=NULL, int grade=NULL, BYTE color=NULL);
	BOOL ZoomSprAuto(int x, int y, SPRITE8 *pSpr, int tx, int ty, int opt=NULL, int grade=NULL, BYTE color=NULL);
	BOOL PutImageAuto(int x, int y, CSprite8 *pSpr8, int sprnum, int opt=NULL, int grade=NULL, BYTE color=NULL);
	BOOL PutImageAuto(int x, int y, SPRITE8 *pSpr, int opt=NULL, int grade=NULL, BYTE color=NULL);

};


#endif