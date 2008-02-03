/*============================================================================
                        윈도95/98용 한글라이브러리

                     제작자 : 오 재 훈 (HiTEL : 93XYZ)


            # 사용환경 : 16bit Color (해상도 Free) 서피스(혹은 가상페이지)에 출력
			# 폰트 사이즈는 자유자재
            # 완성형 한글 출력
=============================================================================*/
#ifndef __XHAN_H__
#define __XHAN_H__


#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <malloc.h>
#include <string.h>
#include <stdarg.h>
#include <conio.h>

//#include "xCanvas.h"
#include "Graphic.h"

#define HAN_OK 1
#define HAN_FILENOTFOUND -1
#define HAN_FONTNOTFOUND -2
#define HAN_RSFNOTOPENED -3


/*------------------ 내부적으로 쓰이는 변수와 함수들 -------------*/
#define KMAXFONT  10    //  한글 폰트의 최대 개수 
#define EMAXFONT  10    //  영문 폰트의 최대 개수 

// 출력 환경저장 스택 구조체
typedef struct 
{
    int multi;
    int shadow;
    int bgr;
    int color;
    int shadowcolor;
    int bgrcolor;
    int gap;
    int hanfont;
    int engfont;
} sHAN_ENVIRENT;

// 한글 폰트 데이터 구조체
typedef struct {
    int xl, yl;            // 한글 폰트의 가로, 세로길이
    int size;              // 한글 폰트 하나의 사이즈(바이트) 
    char *han[2350];       // 한글 폰트 버퍼
} sKFNTDATA;

// 영문 폰트 데이터구조체
typedef struct {
    int xl, yl;            // 영문 폰트의 가로, 세로길이
    int size;              // 폰트 하나의 사이즈(바이트)
    char *english[128];    // 영문 폰트 버퍼                  
} sEFNTDATA;



class xHan
{
public:
	WORD *p2SCREEN;
	int _nx, _ny;			// 현재 글자를 찍을 좌표 
	int _han_xl, _han_yl;	// 화면의 X,Y 길이
	DWORD _han_lPitch;		// lPitch

	char sp_env_point;
	
	
	//------- 아래의 'HAN_'로 시작하는 변수들은 사용자가 조작할 수 있다 ---------
	int   HAN_MULTI;            // 멀티 칼라 여부  (기본값 : 단색)     
	int   HAN_SHADOW;           // 그림자 여부     (기본값 : 없음)     
	int   HAN_BGR;              // 배경색 출력 여부(기본값 : 출력안함) 
	int   HAN_COLOR;            // 글자의 색상                         
	int   HAN_SHADOWCOLOR;      // 그림자의 색상 : 디폴트 = 0번색      
	int   HAN_BGRCOLOR;         // 배경 색상 : 디폴트 = 0번색          
	int   HAN_GAP;              // 글자 간격                           
	int   HAN_KORFONT;          // 현재 사용중인 한글 폰트 번호        
	int   HAN_ENGFONT;          // 현재 사용중인 영문 폰트 번호        
	//---------------------------------------------------------------------------


	sHAN_ENVIRENT env[5];

	sKFNTDATA kfnt[KMAXFONT];			// 한글 폰트 데이타
	sEFNTDATA efnt[EMAXFONT];			// 영문 폰트 데이타

	static char outline_temp1[128], outline_temp2[128];	// 아웃라인 생성용 임시 버퍼

	xHan();
	~xHan();
	
	int  Init(WORD *pscreen, int xl, int yl, DWORD lPitch);
	void SetScreenMode(WORD *pscreen, int xl, int yl, DWORD lPitch);
	void Destroy();
	void Print(int x, int y, char *fmt, ...);

	// 출력 환경 설정
	void SetEffect(int effect, BOOL flag); //(effect: 1=멀티, 2=그림자, 3=외곽선)
	void ClearEffect();
	void SetFont(int kor, int eng);
	void SetTextColor(WORD color);
	void SetBkColor(WORD color);
	void SetTrans(BOOL trans);
	void SetGap(int gap);
	
	BOOL LoadKorFont(int fnum, char *hanFile, int xl, int yl);
	BOOL LoadEngFont(int fnum, char *engFile, int xl, int yl);
	void Push();
	void Pop();


	// 저수준 출력 함수부
	void put_font( int x, int y, int xl, int yl, char *buf,
               WORD *tar, int bgr, WORD bgrColor, int type, WORD color);
	void gap_fill(int x1, int y1, int x2, int y2, WORD *tar, WORD color);
	void htextxy(int hindex, WORD *tar);
	void textxy(char text, WORD *tar);
	int  GetKsIndex(BYTE fstByte,BYTE sndByte);
	void gputs( int x, int y, WORD *tar, char *st);
};

#endif
