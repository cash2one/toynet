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

// RGB �������� ����
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

extern	WORD					(*RGBmix  )(int r,int g,int b);	// 16��Ʈ �÷� RGB ���� �Լ�
extern	WORD					RGB565mix(int r,int g,int b);
extern	WORD					RGB555mix(int r,int g,int b);
extern	WORD					BGR565mix(int r,int g,int b);

extern	WORD					R[32][32];				// ���� ���̺�
extern	WORD					G[64][32];
extern	WORD					B[32][32];

extern	WORD					LIGHT_R[32][32];		// ������ ���� ���̺�
extern	WORD					LIGHT_G[64][64];
extern	WORD					LIGHT_B[32][32];

extern	WORD					DARK_R[32][32];			// Darken Effect�� ���� ���̺�
extern	WORD					DARK_G[64][64];
extern	WORD					DARK_B[32][32];

extern	int						_MXL, _MYL;				// ȭ���� X,Y����
extern	int						_CX1, _CY1, _CX2, _CY2;	// Ŭ���� ���� ��ǥ
extern	int						_CXL, _CYL;				// Ŭ���� ������ X,Y����
extern	DWORD					_LPITCH;				// ���ǽ�(Ȥ�� ����������)�� lPitch


/////////////////////////////////////////////////////////////////////////////////////////////////

#define GET_CSPLINEPTR(pCsp, line)  ((pCsp) + (*(long*)((pCsp)+((line)<<1))))
#define BEGINPUTSPR 1  // ���� ��������Ʈ Ŭ���ο� ���(������ ��½��ۺ� ��������� �˸�)
#define ENDPUTSPR   2  // ���� ��������Ʈ Ŭ���ο� ���(���� ��º� ó�� ������ �˸�)

/////////////////////////////////////////////////////////////////////////////////////////////////

// 4����Ʈ �޸� ���� ���� �Լ�(����: WORD���� ����, Len=WORD����)
extern void _fastcall RvrsCpy(WORD *tar, WORD *sor, int Len);

// ���������� �޸𸮸� ���� ����(����: WORD���� ����, Len=WORD����)
extern void _fastcall MemCpyHB555(WORD *tar, WORD *sor, int Len);
extern void _fastcall MemCpyHB565(WORD *tar, WORD *sor, int Len);

// ���������� �޸𸮸� ���� ����(����: WORD���� ����, Len=WORD����)
extern void _fastcall RvrsCpyHB555(WORD *tar, WORD *sor, int Len);
extern void _fastcall RvrsCpyHB565(WORD *tar, WORD *sor, int Len);

// ��Ƽ ���� �Լ�(����: WORD���� ����, Len=WORD����)
extern void _fastcall MemCpyAnti555(WORD *tar, WORD *sor, int Len);
extern void _fastcall MemCpyAnti565(WORD *tar, WORD *sor, int Len);

// �۷ο� ȿ�� ���� �Լ�(����: WORD���� ����, Len=WORD����)
extern void _fastcall MemCpyGlow555(WORD *tar, WORD *sor, int Len, WORD color, int alpha);
extern void _fastcall MemCpyGlow565(WORD *tar, WORD *sor, int Len, WORD color, int alpha);

// �۷ο� ȿ���� �޸𸮸� ���� ����(����: WORD���� ����, Len=WORD����)
extern void _fastcall RvrsCpyGlow555(WORD *tar, WORD *sor, int Len, WORD color, int alpha);
extern void _fastcall RvrsCpyGlow565(WORD *tar, WORD *sor, int Len, WORD color, int alpha);

// �׷��� ������ ȿ���� �޸𸮸� ���� ����(����: WORD���� ����, Len=WORD����)
extern void _fastcall MemCpyGrayscl555(WORD *tar, WORD *sor, int Len);
extern void _fastcall MemCpyGrayscl565(WORD *tar, WORD *sor, int Len);

// �׷��� ������ ȿ���� �޸𸮸� ���� ����(����: WORD���� ����, Len=WORD����)
extern void _fastcall RvrsCpyGrayscl555(WORD *tar, WORD *sor, int Len);
extern void _fastcall RvrsCpyGrayscl565(WORD *tar, WORD *sor, int Len);

// �ٴܰ� ���������� �޸𸮸� ���� ����(����: WORD���� ����, Len=WORD����)
extern void _fastcall MemCpyGB555(WORD *tar, WORD *sor, int Len, int alpha);
extern void _fastcall MemCpyGB565(WORD *tar, WORD *sor, int Len, int alpha);

// �ٴܰ� ���������� �޸𸮸� ���� ����(����: WORD���� ����, Len=WORD����)
extern void _fastcall RvrsCpyGB555(WORD *tar, WORD *sor, int Len, int alpha);
extern void _fastcall RvrsCpyGB565(WORD *tar, WORD *sor, int Len, int alpha);

// ���ĺ�������  �޸𸮸� ���� ����(����: WORD���� ����, Len=WORD����)
extern void _fastcall MemCpyAB555(WORD *tar, WORD *sor, int Len, int alpha);
extern void _fastcall MemCpyAB565(WORD *tar, WORD *sor, int Len, int alpha);

// ����ȿ����  �޸𸮸� ���� ����(����: WORD���� ����, Len=WORD����)
extern void _fastcall MemCpyLE555(WORD *tar, WORD *sor, int Len);
extern void _fastcall MemCpyLE565(WORD *tar, WORD *sor, int Len);

// ����ȿ����  �޸𸮸� ���� ����(����: WORD���� ����, Len=WORD����)
extern void _fastcall RvrsCpyLE555(WORD *tar, WORD *sor, int Len);
extern void _fastcall RvrsCpyLE565(WORD *tar, WORD *sor, int Len);

// Darken ȿ����  �޸𸮸� ���� ����(����: WORD���� ����, Len=WORD����)
extern void _fastcall MemCpyDE555(WORD *tar, WORD *sor, int Len);
extern void _fastcall MemCpyDE565(WORD *tar, WORD *sor, int Len);

// Darken ȿ����  �޸𸮸� ���� ����(����: WORD���� ����, Len=WORD����)
extern void _fastcall RvrsCpyDE555(WORD *tar, WORD *sor, int Len);
extern void _fastcall RvrsCpyDE565(WORD *tar, WORD *sor, int Len);

// ����� �޸𸮸� �÷�Ű �����Ͽ� �ٴܰ� ���������� ���� ����(����: WORD���� ����, Len=WORD����)
extern void _fastcall ColorKeyMemCpyGB555(WORD *tar, WORD *sor, WORD colorkey, int Len, int alpha);


/////////////////////////////////////////////////////////////////////////////////////////////////

// �Լ� ������
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

// �׷��� ���̺귯�� �ʱ�ȭ
BOOL InitGrpLib(int xl, int yl, DWORD lPitch, int PixelFormat);

// �������̺� �ʱ�ȭ
void InitAlphaTable(int PixelFormat);

// Ŭ���� ���� ����
void SetClipArea(int x1, int y1, int x2, int y2);


//============================ ������ ��� �Լ��� ==========================

// ���� �׸���(Ŭ����)
void DrawHLine( int x, int y, int len, WORD *tar, WORD color );

// ������ �׸���(Ŭ����)
void DrawVLine( int x, int y, int len, WORD *tar, WORD color );

// XOR ���� �׸���(Ŭ����)
void DrawHLineXOR( int x, int y, int len, WORD *tar);

// XOR ������ �׸���(Ŭ����)
void DrawVLineXOR( int x, int y, int len, WORD *tar);

// �� �׸���(�극���� �˰���)
void DrawBresLine( int xs, int ys, int xe, int ye, WORD *tar, WORD color );

// ���� ä�� �ڽ� �׸���(Ŭ����)
void FillBox( int x1, int y1, int x2, int y2, WORD *tar, WORD color );

// �ٴܰ� ������ �ڽ� �׸���(Ŭ����)
void FillBoxGB( int x1, int y1, int x2, int y2, WORD *tar, WORD color, int grade);

// ���� �� �ڽ� �׸���(Ŭ����)
void DrawBox( int x1, int y1, int x2, int y2, WORD *tar, WORD color );

// ���� �� XOR�ڽ� �׸���(Ŭ����)
void DrawBoxXOR( int x1, int y1, int x2, int y2, WORD *tar);

//   ���ùڽ� �׸���(���� ����)
void DrawSelectBox( int x1, int y1, int x2, int y2, WORD *tar, WORD color);

//   ���ùڽ� �׸���(XOR)
void DrawXORSelectBox( int x1, int y1, int x2, int y2, WORD *tar);

// ���ǽ��� ������ ������ ü��
void FillPage( WORD *tar, WORD fillColor );

// ���ǽ��� �� ���
void PutPixel(int x, int y, WORD *tar, WORD color );

// Ư�� ���������� �̹����� ����
void GetImage(int x, int y, int xl, int yl, WORD *page, WORD *tar );

// �̹��� ��� �Լ�
void _PutImage( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor );
void _PutImageUD( int xp, int yp, short xLen, short yLen, WORD *tar, WORD *sor );
void _PutImage_GB( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int grade);

// �Ϲ� ��������Ʈ�� ����ϴ� �Լ�
void _PutSpr( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD colorkey);
void _PutSprUD( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD colorkey );
void _PutSpr_GB( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD colorkey, int grade);

//////////////////////////// ���� ��������Ʈ ��� �Լ� ////////////////////////
void _PutCsp( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor );
void _PutCspLR( int xp, int yp, int xLen, int yLen, WORD* tar, WORD* sor );
void _PutCspUD( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor);

//////////////////////////////// ������ ��� �Լ� /////////////////////////////
void _PutCsp_HB( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor );
void _PutCspLR_HB( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor );

////////////////////////////// ��Ƽ �˶��̽� ��� �Լ� ////////////////////////
void _PutCsp_Anti( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor );

/////////////////////////////// �۷ο� ȿ�� ��� �Լ� /////////////////////////
void _PutCsp_Glow( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD color, int grade);
void _PutCspLR_Glow( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD color, int grade);

/////////////////////////////// �׷��� ������ ȿ�� ��� �Լ� /////////////////////////
void _PutCsp_Grayscl( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor);
void _PutCspLR_Grayscl( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor);

/////////////////////////////// ��� ���� ȿ�� ��� �Լ� /////////////////////////
void _PutCsp_Mono( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD color);
void _PutCspLR_Mono( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, WORD color);

////////////////////////// ��Ƽ �׷��̵� ���� ��� �Լ� //////////////////////
void _PutCsp_GB( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor ,int grade);
void _PutCspLR_GB( int xp, int yp, int xLen, int yLen, WORD* tar, WORD* sor, int grade);

////////////////////////////// ���� ���� ��� �Լ� ///////////////////////////
void _PutCsp_AB( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor, int grade);

////////////////////////////// ����ȿ�� ��� �Լ� ///////////////////////////
void _PutCsp_LE( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor);
void _PutCspLR_LE( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor);

////////////////////////////// Darken ȿ�� ��� �Լ� ///////////////////////////
void _PutCsp_DE( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor);
void _PutCspLR_DE( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor);

////////////////////////////// �ȼ����� ��ȯ ��� �Լ� ///////////////////////////
void _PutCsp_RGB555TORGB565( int xp, int yp, int xLen, int yLen, WORD *tar, WORD *sor);
void _PutCsp_RGB555TORGB24( int xp, int yp, int xLen, int yLen, BYTE *tar, int lpitch, WORD *sor);
void _PutCsp_RGB555TORGB32( int xp, int yp, int xLen, int yLen, DWORD *tar, int lpitch, WORD *sor);

//////////////////////// �Ϲ� ��������Ʈ ����,�ƿ� ��� �Լ� /////////////////
void _ZoomSpr(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, WORD colorkey);

//////////////////////// ���� ��������Ʈ ����,�ƿ� ��� �Լ� /////////////////
void _ZoomCsp( int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor);

//////////////////////// �Ϲ� ��������Ʈ ����Ʈ �浹 �˻� �Լ� /////////////////
BOOL _PointHitTestSpr( int xLen, int yLen, WORD *sor, WORD colorkey, int txp, int typ );

//////////////////////// ���� ��������Ʈ ����Ʈ �浹 �˻� �Լ� /////////////////
BOOL _PointHitTestCsp(int xLen, int yLen, WORD *sor, int txp, int typ );

#endif

