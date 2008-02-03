#ifndef __GRAPHMOD8_H__
#define __GRAPHMOD8_H__


#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <dos.h>
#include <conio.h>
#include <string.h>


extern int _MXL, _MYL;              // 화면의 X,Y길이
extern int _CX1, _CY1, _CX2, _CY2;  // 클리핑 영역 좌표
extern int _CXL, _CYL;              // 클리핑 영역의 X,Y길이
extern DWORD _LPITCH;               // 서피스(혹은 가상페이지)의 lPitch


void CreateAlphaTable8();
void ClearAlphaTable8();

int SaveAlphaTable8(char* fname);
int LoadAlphaTable8(char* fname);

void PutSprGlass8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, int grade);
void FillBox8( int x1, int y1, int x2, int y2, char *tar, char color );
void FillBoxGlass8( int xp, int yp, int xl, int yl, char *tar, char color, int grade);
void DrawHLine8( int x, int y, int len, char *tar, char color );
void DrawVLine8( int x, int y, int len, char *tar, char color );
void DrawBox8( int x1, int y1, int x2, int y2, char *tar, char color );
void FillPage8( char *tar, char fillColor );
void PutPixel8(int x, int y, char *tar, char color );
void GetImage8(int x, int y, int xl, int yl, char *page, char *tar );
void PutImage8( int xp, int yp, int xLen, int yLen, char *tar, char *sor );
void PutImageLR8( int xp, int yp, short xLen, short yLen, char *tar, char *sor );
void PutImageUD8( int xp, int yp, short xLen, short yLen, char *tar, char *sor );
void PutImageUDLR8( int xp, int yp, short xLen, short yLen, char *tar, char *sor );
void PutImageAngle8( int xp, int yp, short xLen, short yLen, char *tar, char *sor, int ang );
void PutImage90d8( int xp, int yp, short xLen, short yLen, char *tar, char *sor );
void PutImage270d8( int xp, int yp, short xLen, short yLen, char *tar, char *sor );

void PutSpr8( int xp, int yp, int xLen, int yLen, char *tar, char *sor );
void PutSprLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor );
void PutSprUD8( int xp, int yp, int xLen, int yLen, char *tar, char *sor );
void PutSprUDLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor );
void PutSpr90d8( int xp, int yp, short xLen, short yLen, char *tar, char *sor );
void PutSpr270d8( int xp, int yp, short xLen, short yLen, char *tar, char *sor );
void PutSprAngle8( int xp, int yp, short xLen, short yLen, char *tar, char *sor, int ang );
void PutSprMono8( int xp, int yp, short xLen, short yLen, char *tar, char *sor, char color );
void PutSprMonoLR8( int xp, int yp, short xLen, short yLen, char *tar, char *sor, char color );
void PutSprMonoUD8( int xp, int yp, short xLen, short yLen, char *tar, char *sor, char color);
void PutSprMonoUDLR8( int xp, int yp, short xLen, short yLen, char *tar, char *sor, char color );
void PutSprMono90d8( int xp, int yp, short xLen, short yLen, char *tar, char *sor, char color );
void PutSprMono270d8( int xp, int yp, short xLen, short yLen, char *tar, char *sor, char color );
void PutSprMonoAngle8( int xp, int yp, short xLen, short yLen, char *tar, char *sor, char color, int ang );
void PutSprGlassLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, int grade);
void PutSprGlassUD8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, int grade);
void PutSprGlassUDLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, int grade);
void PutSprMonoGlass8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color, int grade);
void PutSprMonoGlassLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color, int grade);
void PutSprMonoGlassUD8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color, int grade);
void PutSprMonoGlassUDLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color, int grade);

// 압축 스프라이트
void PutCspLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor );
void PutCspUD8( int xp, int yp, int xLen, int yLen, char *tar, char *sor);
void PutCspUDLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor );
void PutCsp8( int xp, int yp, int xLen, int yLen, char *tar, char *sor );
void PutCspMono8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color );
void PutCspMonoLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color );
void PutCspMonoUD8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color);
void PutCspMonoUDLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color);
void PutCspGlass8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, int grade);
void PutCspGlassLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, int grade);
void PutCspGlassUD8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, int grade);
void PutCspGlassUDLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, int grade);
void PutCspMonoGlass8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color, int grade);
void PutCspMonoGlassLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color, int grade);
void PutCspMonoGlassUD8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color, int grade);
void PutCspMonoGlassUDLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color, int grade);

// 확대축소 출력
void LowZoomSpr8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor);
void LowZoomSprLR8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor);
void LowZoomSprUD8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor);
void LowZoomSprUDLR8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor);
void LowZoomSprMono8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color);
void LowZoomSprMonoLR8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color);
void LowZoomSprMonoUD8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color);
void LowZoomSprMonoUDLR8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color);
void LowZoomSprGlass8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, int grade);
void LowZoomSprGlassLR8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, int grade);
void LowZoomSprGlassUD8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, int grade);
void LowZoomSprGlassUDLR8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, int grade);
void LowZoomSprMonoGlass8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color, int grade);
void LowZoomSprMonoGlassLR8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color, int grade);
void LowZoomSprMonoGlassUD8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color, int grade);
void LowZoomSprMonoGlassUDLR8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color, int grade);
void LowZoomCsp8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor);
void LowZoomCspLR8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor);
void LowZoomCspUD8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor);
void LowZoomCspUDLR8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor);
void LowZoomCspMono8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color);
void LowZoomCspMonoLR8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color);
void LowZoomCspMonoUD8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color);
void LowZoomCspMonoUDLR8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color);
void LowZoomCspGlass8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, int grade);
void LowZoomCspGlassLR8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, int grade);
void LowZoomCspGlassUD8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, int grade);
void LowZoomCspGlassUDLR8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, int grade);
void LowZoomCspMonoGlass8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color, int grade);
void LowZoomCspMonoGlassLR8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color, int grade);
void LowZoomCspMonoGlassUD8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color, int grade);
void LowZoomCspMonoGlassUDLR8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color, int grade);



#endif

