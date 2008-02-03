#ifndef __GRAPHMOD_H__
#define __GRAPHMOD_H__


#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <dos.h>
#include <conio.h>
#include <string.h>


#define dRGB565			565 
#define dRGB555			555
#define dBGR565			(-565)

#define dGRAY			0
#define dGREEN			1
#define dRED			2
#define dBLUE			3
#define dYELLOW			4 
#define dCYAN			5

// RGB 고유성분 추출
#define R555(r)			(((r)&0x7c00)>>10)
#define G555(g)			(((g)&0x03e0)>>5)
#define B555(b)			((b)&0x001f)
#define R565(r)			(((r)&0xf800)>>11)
#define G565(g)			(((g)&0x07e0)>>5)
#define B565(b)			((b)&0x001f)

#define	d2HM_RGB565				0xf7de
#define	d4HM_RGB565				0xf7def7de
#define	d4AM_RGB565_1			0xf81f07e0
#define	d4AM_RGB565_2			0x07e0f81f


#define	d2HM_RGB555				0x7bde
#define	d4HM_RGB555				0x7bde7bde
#define	d4AM_RGB555_1			0x7c1f03e0
#define	d4AM_RGB555_2			0x03e07c1f

extern	WORD					(*RGBmix  )(int r,int g,int b);	// 16비트 컬러 RGB 조합 함수
extern	WORD					RGB565mix(int r,int g,int b);
extern	WORD					RGB555mix(int r,int g,int b);
extern	WORD					BGR565mix(int r,int g,int b);

extern	WORD					R[32][32];				// 알파 테이블
extern	WORD					G[64][32];
extern	WORD					B[32][32];

extern	WORD					LIGHT_R[32][32];		// 광원용 알파 테이블
extern	WORD					LIGHT_G[64][64];
extern	WORD					LIGHT_B[32][32];

extern	WORD					DARK_R[32][32];			// Darken Effect용 알파 테이블
extern	WORD					DARK_G[64][64];
extern	WORD					DARK_B[32][32];

extern	int						_MXL, _MYL;				// 화면의 X,Y길이
extern	int						_CX1, _CY1, _CX2, _CY2;	// 클리핑 영역 좌표
extern	int						_CXL, _CYL;				// 클리핑 영역의 X,Y길이
extern	DWORD					_LPITCH;				// 서피스(혹은 가상페이지)의 lPitch


/////////////////////////////////////////////////////////////////////////////////////////////////

#define GET_CSPLINEPTR(pCsp, line)  ((pCsp) + (*(long*)((pCsp)+((line)<<1))))
#define BEGINPUTSPR 1  // 압축 스프라이트 클리핑에 사용(라인의 출력시작부 계산중임을 알림)
#define ENDPUTSPR   2  // 압축 스프라이트 클리핑에 사용(실제 출력부 처리 중임을 알림)

/////////////////////////////////////////////////////////////////////////////////////////////////

// 4바이트 메모리 역순 복사 함수(주의: WORD단위 복사, Len=WORD길이)
extern void _fastcall RvrsCpy(WORD *tar, WORD *sor, int Len);

// 반투명으로 메모리를 순차 복사(주의: WORD단위 복사, Len=WORD길이)
extern void _fastcall MemCpyHB555(WORD *tar, WORD *sor, int Len);
extern void _fastcall MemCpyHB565(WORD *tar, WORD *sor, int Len);

// 반투명으로 메모리를 역순 복사(주의: WORD단위 복사, Len=WORD길이)
extern void _fastcall RvrsCpyHB555(WORD *tar, WORD *sor, int Len);
extern void _fastcall RvrsCpyHB565(WORD *tar, WORD *sor, int Len);

// 안티 복사 함수(주의: WORD단위 복사, Len=WORD길이)
extern void _fastcall MemCpyAnti555(WORD *tar, WORD *sor, int Len);
extern void _fastcall MemCpyAnti565(WORD *tar, WORD *sor, int Len);

// 글로우 효과 복사 함수(주의: WORD단위 복사, Len=WORD길이)
extern void _fastcall MemCpyGlow555(WORD *tar, WORD *sor, int Len, WORD color, int alpha);
extern void _fastcall MemCpyGlow565(WORD *tar, WORD *sor, int Len, WORD color, int alpha);

// 글로우 효과로 메모리를 역순 복사(주의: WORD단위 복사, Len=WORD길이)
extern void _fastcall RvrsCpyGlow555(WORD *tar, WORD *sor, int Len, WORD color, int alpha);
extern void _fastcall RvrsCpyGlow565(WORD *tar, WORD *sor, int Len, WORD color, int alpha);

// 그레이 스케일 효과로 메모리를 순차 복사(주의: WORD단위 복사, Len=WORD길이)
extern void _fastcall MemCpyGrayscl555(WORD *tar, WORD *sor, int Len);
extern void _fastcall MemCpyGrayscl565(WORD *tar, WORD *sor, int Len);

// 그레이 스케일 효과로 메모리를 역순 복사(주의: WORD단위 복사, Len=WORD길이)
extern void _fastcall RvrsCpyGrayscl555(WORD *tar, WORD *sor, int Len);
extern void _fastcall RvrsCpyGrayscl565(WORD *tar, WORD *sor, int Len);

// 다단계 반투명으로 메모리를 순차 복사(주의: WORD단위 복사, Len=WORD길이)
extern void _fastcall MemCpyGB555(WORD *tar, WORD *sor, int Len, int alpha);
extern void _fastcall MemCpyGB565(WORD *tar, WORD *sor, int Len, int alpha);

// 다단계 반투명으로 메모리를 역순 복사(주의: WORD단위 복사, Len=WORD길이)
extern void _fastcall RvrsCpyGB555(WORD *tar, WORD *sor, int Len, int alpha);
extern void _fastcall RvrsCpyGB565(WORD *tar, WORD *sor, int Len, int alpha);

// 알파블렌딩으로  메모리를 순차 복사(주의: WORD단위 복사, Len=WORD길이)
extern void _fastcall MemCpyAB555(WORD *tar, WORD *sor, int Len, int alpha);
extern void _fastcall MemCpyAB565(WORD *tar, WORD *sor, int Len, int alpha);

// 광원효과로  메모리를 순차 복사(주의: WORD단위 복사, Len=WORD길이)
extern void _fastcall MemCpyLE555(WORD *tar, WORD *sor, int Len);
extern void _fastcall MemCpyLE565(WORD *tar, WORD *sor, int Len);

// 광원효과로  메모리를 역순 복사(주의: WORD단위 복사, Len=WORD길이)
extern void _fastcall RvrsCpyLE555(WORD *tar, WORD *sor, int Len);
extern void _fastcall RvrsCpyLE565(WORD *tar, WORD *sor, int Len);

// Darken 효과로  메모리를 순차 복사(주의: WORD단위 복사, Len=WORD길이)
extern void _fastcall MemCpyDE555(WORD *tar, WORD *sor, int Len);
extern void _fastcall MemCpyDE565(WORD *tar, WORD *sor, int Len);

// Darken 효과로  메모리를 역순 복사(주의: WORD단위 복사, Len=WORD길이)
extern void _fastcall RvrsCpyDE555(WORD *tar, WORD *sor, int Len);
extern void _fastcall RvrsCpyDE565(WORD *tar, WORD *sor, int Len);

// 비압축 메모리를 컬러키 적용하여 다단계 반투명으로 순차 복사(주의: WORD단위 복사, Len=WORD길이)
extern void _fastcall ColorKeyMemCpyGB555(WORD *tar, WORD *sor, WORD colorkey, int Len, int alpha);


/////////////////////////////////////////////////////////////////////////////////////////////////

// 함수 포인터
extern void (_fastcall *MemCpyHB) (WORD *tar, WORD *sor, int Len);
extern void (_fastcall *RvrsCpyHB) (WORD *tar, WORD *sor, int Len);
extern void (_fastcall *MemCpyAnti) (WORD *tar, WORD *sor, int Len);
extern void (_fastcall *MemCpyGlow) (WORD *tar, WORD *sor, int Len, WORD color, int alpha);
extern void (_fastcall *RvrsCpyGlow) (WORD *tar, WORD *sor, int Len, WORD color, int alpha);
extern void (_fastcall *MemCpyGrayscl) (WORD *tar, WORD *sor, int Len);
extern void (_fastcall *RvrsCpyGrayscl) (WORD *tar, WORD *sor, int Len);
extern void (_fastcall *MemCpyGB) (WORD *tar, WORD *sor, int Len, int alpha);
extern void (_fastcall *RvrsCpyGB) (WORD *tar, WORD *sor, int Len, int alpha);
extern void (_fastcall *MemCpyAB) (WORD *tar, WORD *sor, int Len, int alpha);
extern void (_fastcall *MemCpyLE) (WORD *tar, WORD *sor, int Len);
extern void (_fastcall *RvrsCpyLE) (WORD *tar, WORD *sor, int Len);
extern void (_fastcall *MemCpyDE) (WORD *tar, WORD *sor, int Len);
extern void (_fastcall *RvrsCpyDE) (WORD *tar, WORD *sor, int Len);
extern void (_fastcall *ColorKeyMemCpyGB) (WORD *tar, WORD *sor, WORD colorkey, int Len, int alpha);

/////////////////////////////////////////////////////////////////////////////////////////////////

// 그래픽 라이브러리 초기화
BOOL InitGrpLib(int xl, int yl, DWORD lPitch, int PixelFormat);

// 알파테이블 초기화
void InitAlphaTable(int PixelFormat);

// 클리핑 영역 설정
void SetClipArea(int x1, int y1, int x2, int y2);


//============================ 저수준 출력 함수부 ==========================

// 수평선 그리기(클리핑)
void DrawHLine( int x, int y, int len, WORD *tar, WORD color );

// 수직선 그리기(클리핑)
void DrawVLine( int x, int y, int len, WORD *tar, WORD color );

// XOR 수평선 그리기(클리핑)
void DrawHLineXOR( int x, int y, int len, WORD *tar);

// XOR 수직선 그리기(클리핑)
void DrawVLineXOR( int x, int y, int len, WORD *tar);

// 선 그리기(브레제남 알고리즘)
void DrawBresLine( int xs, int ys, int xe, int ye, WORD *tar, WORD color );

// 색을 채운 박스 그리기(클리핑)
void FillBox( int x1, int y1, int x2, int y2, WORD *tar, WORD color );

// 다단계 반투명 박스 그리기(클리핑)
void FillBoxGB( int x1, int y1, int x2, int y2, WORD *tar, WORD color, int grade);

// 속이 빈 박스 그리기(클리핑)
void DrawBox( int x1, int y1, int x2, int y2, WORD *tar, WORD color );

// 속이 빈 XOR박스 그리기(클리핑)
void DrawBoxXOR( int x1, int y1, int x2, int y2, WORD *tar);

//   선택박스 그리기(색상 선택)
void DrawSelectBox( int x1, int y1, int x2, int y2, WORD *tar, WORD color);

//   선택박스 그리기(XOR)
void DrawXORSelectBox( int x1, int y1, int x2, int y2, WORD *tar);

// 서피스를 지정한 색으로 체움
void FillPage( WORD *tar, WORD fillColor );

// 서피스에 점 출력
void PutPixel(int x, int y, WORD *tar, WORD color );

// 특정 페이지에서 이미지를 얻음
void GetImage(int x, int y, int xl, int yl, WORD *page, WORD *tar );

// 이미지 출력 함수
void _PutImage( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor );
void _PutImageUD( int xp, int yp, short xLen, short yLen, WORD *tar, WORD *sor );
void _PutImage_GB( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int grade);

// 일반 스프라이트를 출력하는 함수
void _PutSpr( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD colorkey);
void _PutSprUD( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD colorkey );
void _PutSpr_GB( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD colorkey, int grade);

//////////////////////////// 압축 스프라이트 출력 함수 ////////////////////////
void _PutCsp( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor );
void _PutCspLR( int xp, int yp, int xLen, int yLen, WORD* tar, WORD* sor );
void _PutCspUD( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor);

//////////////////////////////// 반투명 출력 함수 /////////////////////////////
void _PutCsp_HB( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor );
void _PutCspLR_HB( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor );

////////////////////////////// 안티 알라이싱 출력 함수 ////////////////////////
void _PutCsp_Anti( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor );

/////////////////////////////// 글로우 효과 출력 함수 /////////////////////////
void _PutCsp_Glow( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD color, int grade);
void _PutCspLR_Glow( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD color, int grade);

/////////////////////////////// 그레이 스케일 효과 출력 함수 /////////////////////////
void _PutCsp_Grayscl( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor);
void _PutCspLR_Grayscl( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor);

/////////////////////////////// 모노 필터 효과 출력 함수 /////////////////////////
void _PutCsp_Mono( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD color);
void _PutCspLR_Mono( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD color);

////////////////////////// 멀티 그레이드 블렌딩 출력 함수 //////////////////////
void _PutCsp_GB( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor ,int grade);
void _PutCspLR_GB( int xp, int yp, int xLen, int yLen, WORD* tar, WORD* sor, int grade);

////////////////////////////// 알파 블렌딩 출력 함수 ///////////////////////////
void _PutCsp_AB( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int grade);

////////////////////////////// 광원효과 출력 함수 ///////////////////////////
void _PutCsp_LE( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor);
void _PutCspLR_LE( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor);

////////////////////////////// Darken 효과 출력 함수 ///////////////////////////
void _PutCsp_DE( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor);
void _PutCspLR_DE( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor);

////////////////////////////// 픽셀포맷 변환 출력 함수 ///////////////////////////
void _PutCsp_RGB555TORGB565( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor);
void _PutCsp_RGB555TORGB24( int xp, int yp, int xLen, int yLen, BYTE *tar, int lpitch, WORD *sor);
void _PutCsp_RGB555TORGB32( int xp, int yp, int xLen, int yLen, DWORD *tar, int lpitch, WORD *sor);

//////////////////////// 일반 스프라이트 줌인,아웃 출력 함수 /////////////////
void _ZoomSpr(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, WORD colorkey);

//////////////////////// 압축 스프라이트 줌인,아웃 출력 함수 /////////////////
void _ZoomCsp( int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor);

//////////////////////// 일반 스프라이트 포인트 충돌 검사 함수 /////////////////
BOOL _PointHitTestSpr( int xLen, int yLen, WORD *sor, WORD colorkey, int txp, int typ );

//////////////////////// 압축 스프라이트 포인트 충돌 검사 함수 /////////////////
BOOL _PointHitTestCsp(int xLen, int yLen, WORD *sor, int txp, int typ );

#endif

