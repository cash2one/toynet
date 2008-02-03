#ifndef __SPRITE8_H__
#define __SPRITE8_H__

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct         // 스프라이트의 파일 헤더
{
    char  Identifier[16];      // 스프라이트 인식문자
    short Version;             // 스프라이트 버젼
    short TotalSprNum;         // 총 스프라이트 개수
	long  ActionNum;           // 액션의 개수
} SPRFILEHEAD8;
typedef SPRFILEHEAD8* LPSPRFILEHEAD8;

typedef struct         // 충돌, 공격좌표 구조체
{
	short attrib;              // 좌표의 속성
    short x1,y1;               // 좌상의 좌표
	short x2,y2;               // 우하의 좌표
	char reserved[2];
} SPRPOS8;
typedef SPRPOS8* LPSPRPOS8;

typedef struct         // 각 스프라이트의 헤더
{
    BYTE  compress;            // 압축여부
	short rx, ry;              // 고유 상대좌표
    short xl, yl;              // X, Y축 길이
    short mx, my;              // 중심좌표
	short hitNum;              // 충돌좌표의 개수
	short atckNum;             // 공격좌표의 개수
    unsigned long dsize;       // 스프라이트 데이타의 사이즈(압축시 필요)
	BYTE colorkey;			   // 컬러키
	char  reserved[2];
} SPRHEAD8;
typedef SPRHEAD8* LPSPRHEAD8;

typedef struct         // 액션 헤더
{
    BYTE  id;                  // 액션 번호
	BYTE  attrib;              // 액션의 속성
	short frameNum;            // 액션 프레임의 개수
} ACTIONHEAD8;
typedef ACTIONHEAD8* LPACTIONHEAD8;

typedef struct         // 각 액션 프레임 구조체
{
    short frame;               // 프레임 번호
	long  delay;               // 지연 프레임 카운트
	short rx, ry;              // 프레임의 상대좌표
	short flag;                // 프레임의 속성 플레그
} ACTIONFRAME8;
typedef ACTIONFRAME8* LPACTIONFRAME8;

//------------------------------------------------------------------------------

typedef struct          // 스프라이트 데이타 구조체
{
    int compress;            // 압축여부
	BYTE colorkey;           // 컬러키
	int xl, yl;              // X, Y축 길이
    int rx, ry;              // 상대좌표
    int mx, my;              // 중심좌표
	int hitNum;              // 충돌좌표의 개수
	int atckNum;             // 공격좌표의 개수
	unsigned long  dsize;    // 스프라이트 데이타의 사이즈(압축시 필요)
	SPRPOS8* hit;             // 충돌좌표 구조체의 포인터
	SPRPOS8* atck;            // 공격좌표 구조체의 포인터
    char*   data;            // 스프라이트 데이타 포인터
} SPRITE8;


class CSprite8
{
public:
	///// 공통 변수 선언
	SPRFILEHEAD8 sfhead;                  // 스프라이트 파일 헤더
	SPRITE8 *spr;                       // 스프라이트 정보 구조체

	///// 생성자, 소멸자
	CSprite8();
	~CSprite8();

	//// 스프라이트 로드,세이브,제거함수
	BOOL Load(char *name);
	void Remove();
};



#endif

