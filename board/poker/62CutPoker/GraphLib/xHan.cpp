/*============================================================================
                        윈도95/98용 한글라이브러리

                     제작자 : 오 재 훈 (HiTEL : 93XYZ)


            # 사용환경 : 16bit Color (해상도 Free) 서피스(혹은 가상페이지)에 출력
			# 폰트 사이즈는 자유자재
            # 완성형 한글 출력
=============================================================================*/

#include "StdAfx.h"
#include "xHan.h"


char xHan::outline_temp1[128], xHan::outline_temp2[128];	// 아웃라인 생성용 임시 버퍼


xHan::xHan()
{
	p2SCREEN = NULL;
	sp_env_point = 0;
	_han_xl=640, _han_yl=480;	// 화면의 X,Y 길이
	DWORD _han_lPitch = 640;		// lPitch

	//------- 아래의 'HAN_'로 시작하는 변수들은 사용자가 조작할 수 있다 ---------
	HAN_MULTI = 0;            // 멀티 칼라 여부  (기본값 : 단색)     
	HAN_SHADOW = 0;           // 그림자 여부     (기본값 : 없음)     
	HAN_BGR=0;                // 배경색 출력 여부(기본값 : 출력안함) 
	
	HAN_COLOR = 20;           // 글자의 색상                         
	HAN_SHADOWCOLOR = 0;      // 그림자의 색상 : 디폴트 = 0번색      
	HAN_BGRCOLOR = 0;         // 배경 색상 : 디폴트 = 0번색          
	HAN_GAP = 1;              // 글자 간격                           
	HAN_KORFONT = 1;          // 현재 사용중인 한글 폰트 번호        
	HAN_ENGFONT = 1;          // 현재 사용중인 영문 폰트 번호        
	//---------------------------------------------------------------------------
}

xHan::~xHan()
{
	Destroy();
}

/*-------------------------------------------------------------------------------
   한글 라이브러리 초기화 함수
        xl, yl : 출력할 화면의 사이즈(가상 페이지혹은 서피스의 가로 세로 길이)
		lPitch : 출력할 페이지의 lPitch(주의: 워드 단위임)
---------------------------------------------------------------------------------*/
int xHan::Init(WORD *pscreen, int xl, int yl, DWORD lPitch)
{
    int k,i;

	_han_xl = xl;
	_han_yl = yl;
	_han_lPitch = lPitch;
	p2SCREEN = pscreen;

    for(k=0; k < KMAXFONT; k++)  // 메모리 초기화 
    {
		memset(&kfnt[k].han, NULL, 2350);
    }

    for(k=0; k < EMAXFONT; k++)  // 메모리 초기화 
    {
        for(i=0; i<128; i++) efnt[k].english[i]   = NULL;
    }

    // 한글 폰트를 읽음
	BOOL check = TRUE;

    check = LoadKorFont( 0,".\\Font\\k8x10g0.fnt", 8, 10);
    check = LoadKorFont( 1,".\\Font\\k16x16g1.fnt", 16, 16);
    check = LoadKorFont( 2,".\\Font\\k16x16g2.fnt", 16, 16);
    check = LoadKorFont( 3,".\\Font\\k16x16g3.fnt", 16, 16);
    check = LoadKorFont( 4,".\\Font\\k16x16g4.fnt", 16, 16);
	check = LoadKorFont( 5,".\\Font\\k16x16g5.fnt", 16, 16);

    // 영문 폰트를 읽음
    check = LoadEngFont( 0,".\\Font\\e6x6g0.fnt", 6, 6);
    check = LoadEngFont( 1,".\\Font\\e5x9g1.fnt", 5, 9);
    check = LoadEngFont( 2,".\\Font\\e8x16g2.fnt", 8, 16);
    check = LoadEngFont( 3,".\\Font\\e8x16g3.fnt", 8, 16);
    check = LoadEngFont( 4,".\\Font\\e8x16g4.fnt", 8, 16);
    check = LoadEngFont( 5,".\\Font\\e8x16g5.fnt", 8, 16);

    //close_resfile();
	if(!check) return HAN_FONTNOTFOUND;
	return HAN_OK;
}

/*-------------------------------------------------------------------------------
   스크린 모드 재설정
---------------------------------------------------------------------------------*/
void xHan::SetScreenMode(WORD *pscreen, int xl, int yl, DWORD lPitch)
{
 	_han_xl = xl;
	_han_yl = yl;
	_han_lPitch = lPitch;
	p2SCREEN = pscreen;
}

/*-------------------------------------------------------------------------------
   한글 라이브러리 종료 함수
---------------------------------------------------------------------------------*/
void xHan::Destroy()
{
    int k,i;

    for(k=0; k < KMAXFONT; k++)  // 할당된 메모리를 반환한다
    {
        for(i=0; i<2350; i++)
        {
            if(kfnt[k].han[i] != NULL)
            {
                free(kfnt[k].han[i]);
                kfnt[k].han[i] = NULL;
            }
        }
    }

    for(k=0; k < EMAXFONT; k++)  // 할당된 메모리를 반환한다
    {
        for(i=0; i<128; i++)
        {
            if(efnt[k].english[i] != NULL)
            {
                free(efnt[k].english[i]);
                efnt[k].english[i] = NULL;
            }
        }
    }
}

void xHan::SetFont(int kor, int eng)
{
	if(kfnt[kor].han[0] != NULL) HAN_KORFONT = kor;
	if(efnt[eng].english[0] != NULL) HAN_ENGFONT = kor;
}

void xHan::SetTextColor(WORD color)
{
	HAN_COLOR = color;
}

void xHan::SetBkColor(WORD color)
{
	HAN_BGRCOLOR = color;
}

void xHan::SetTrans(BOOL trans)
{
	HAN_BGR = trans;
}

void xHan::SetEffect(int effect, BOOL flag)
{
	switch(effect)
	{
	case 1: (flag)? HAN_MULTI=TRUE : HAN_MULTI=FALSE; break;	// 멀티 칼라 출력
	case 2: (flag)? HAN_SHADOW=1 : HAN_SHADOW=0; break;			// 그림자 출력
	case 3: (flag)? HAN_SHADOW=2 : HAN_SHADOW=0; break;			// 외곽선 출력
	}
}

void xHan::ClearEffect()
{
	HAN_MULTI=FALSE;
	HAN_SHADOW=0;
}

void xHan::SetGap(int gap)
{
	HAN_GAP = gap;
}


/*-------------------------------------------------------------------------------
   한글 폰트 읽기 함수
        hanFile      : 읽을 한글폰트 파일명
        xl, yl       : 한글 폰트의 X, Y길이
        fnum         : 폰트 번호
---------------------------------------------------------------------------------*/
BOOL xHan::LoadKorFont( int fnum, char *hanFile, int xl, int yl)
{
    FILE *fp;

	int i;

	if ( (fp = fopen( hanFile, "rb" )) == NULL ) return FALSE;

    kfnt[fnum].xl = xl;
    kfnt[fnum].yl = yl;

    kfnt[fnum].size = ( (xl/8 * 8) != xl ? (xl/8+1)*8 : xl ) /8 * yl;

    for(i=0; i<2350; i++)
        kfnt[fnum].han[i] = (char *) malloc(kfnt[fnum].size);
    /*----------------*/

    for(i=0; i<2350; i++)
	{
			fread((void *)(kfnt[fnum].han[i]) , kfnt[fnum].size, 1, fp);
	}

    fclose(fp); // 리소스 파일이 아닐경우 
    return TRUE;
}


/*-------------------------------------------------------------------------------
   영문 폰트 읽기 함수

        engFile      : 읽을 영문폰트 파일명
        xl, yl       : 영문 폰트의 X, Y길이
        fnum         : 폰트 번호
---------------------------------------------------------------------------------*/
BOOL xHan::LoadEngFont(int fnum, char *engFile, int xl, int yl)
{
    FILE *fp=NULL;
    int i;
	
	if ( (fp = fopen( engFile, "rb" )) == NULL ) return FALSE;
	
    efnt[fnum].xl = xl;
    efnt[fnum].yl = yl;

    efnt[fnum].size = ( (xl/8 * 8) != xl ? (xl/8+1)*8 : xl ) /8 * yl;

    for(i=0; i<128; i++)
		efnt[fnum].english[i]   = (char *) malloc(efnt[fnum].size);

    for(i=0; i<128; i++)
	{
	        fread((void *)(efnt[fnum].english[i]), efnt[fnum].size, 1, fp);
	}

    fclose(fp); 
    return TRUE;
}



/*-----------------------------------------------------------------------------
    폰트를 target 서피스(혹은 가상페이지)에 뿌림 --- 테스트용

    x,y  : 출력할 좌표                   xl,yl    : 폰트의 가로 세로 크기
    buf  : 폰트 이미지                   tar      : 출력할 페이지
    bgr  : 배경색 출력 여부              bgrColor : 출력할 배경색
	multi: 0=정상출력 1=멀티출력         color : 출력할 색상
-------------------------------------------------------------------------------*/
/*
void xHan::put_font_test( int x, int y, int xl, int yl, char *buf,\
               WORD *tar, int bgr, WORD bgrColor, int multi, WORD color)
{
    int i,j,m=0,n=0;
    unsigned char mask = 0x80;        // = 10000000b
    
	tar += y * _han_lPitch + x;  
    
	for(i = 0; i < yl; i++)
    {
        n=0;

        for(j=0; j<xl; j++)
        {
            if((buf[m] & (mask >> n)) == (mask >> n)) *tar = color;
            else if(bgr) *tar=bgrColor;
            n++;
            if(n == 8) if(j+1 != xl) { n = 0; m++; }
            tar++;
        }
        m++;
        if(multi == 1) color++;  // 멀티 칼라 출력
        tar += _han_lPitch -xl;
    }
}
*/

/*----------------------------------------------------------------------------
   저수준 폰트 출력 함수(클리핑 처리및 특수 효과용)

    x1, y1, x2, y2 : 초과치
    (아직은 사용불가, 나중에 쓸것임)
----------------------------------------------------------------------------*/
/*
void xHan::put_font_clip( int x, int y, int xl, int yl, int x1, int y1, int x2, int y2,\
      char *buf, WORD *tar, int bgr, WORD bgrColor, int multi, WORD color)
{
    int i,j,m=0,n=0,xlim,ylim;
    unsigned char mask = 0x80;        // = 10000000b 
    tar += (y << 8) + (y << 6) + x;   //  y * 320 + x 와 같다.
    xlim=xl-x2;
    ylim=yl-y2;
    for(i = 0; i < yl; i++)
    {

        for(j=0; j<xl; j++)
        {
            if((buf[m] & (mask >> n)) == (mask >> n))
            {
                if(j>=x1 && j<xlim && i>=y1 && i<ylim) *tar = color;
            }
            else if(bgr) if(j>=x1 && j<xlim && i>=y1 && i<ylim) *tar=bgrColor;
            n++;
            if(n == 8 || n == xl) { n = 0; m++; }
            tar++;
        }
        if(multi == 1) color++;  // 멀티 칼라 출력
        tar += _han_lPitch-xl;
    }
}
*/

/*-----------------------------------------------------------------------------
    폰트를 target 서피스(혹은 가상페이지)에 뿌림

    x,y  : 출력할 좌표                   xl,yl    : 폰트의 가로 세로 크기
    buf  : 폰트 이미지                   tar      : 출력할 페이지
    bgr  : 배경색 출력 여부              bgrColor : 출력할 배경색
	type : 0:Normal 1:Multi 2:Outline
	color : 출력할 색상
-------------------------------------------------------------------------------*/
void xHan::put_font( int x, int y, int xl, int yl, char *buf,\
               WORD *tar, int bgr, WORD bgrColor, int type, WORD color)
{
    int i,j,m=0,n=0;
    unsigned char mask = 0x80;        // = 10000000b
	int xsize, size;
    
	tar += y * _han_lPitch + x;  
    
	if(type==2)  // 외곽선출력
	{
		xsize = xl/8;
		if(xl%8) xsize++;
		size = xsize*yl;

		memcpy(outline_temp1, buf, size);
		memcpy(outline_temp2, buf, size);

		for(i=0; i<size; i++)
		{
			outline_temp1[i] |= outline_temp2[i]>>1;
			outline_temp1[i] |= outline_temp2[i]<<1;
		}

		for(i=0; i<size-xsize; i++)
		{
			outline_temp1[i+xsize] |= outline_temp2[i];
		}

		for(i=xsize; i<size; i++)
		{
			outline_temp1[i-xsize] |= outline_temp2[i];
		}

		for(i=0; i<size; i++)  // 상하 좌우로 늘어난 글자에서 원래 이미지를 지움
		{
			outline_temp1[i] ^= outline_temp2[i];
		}

		for(i = 0; i < yl; i++)
		{
			n=0;

			for(j=0; j<xl; j++)
			{
				if((outline_temp1[m] & (mask >> n)) == (mask >> n)) *tar = color;
				else if(bgr) *tar=bgrColor;
				n++;
				if(n == 8) if(j+1 != xl) { n = 0; m++; }
				tar++;
			}
			m++;
			tar += _han_lPitch -xl;
		}
	}
	else for(i = 0; i < yl; i++)  // 일반출력
    {
        n=0;

        for(j=0; j<xl; j++)
        {
            if((buf[m] & (mask >> n)) == (mask >> n)) *tar = color;
            else if(bgr) *tar=bgrColor;
            n++;
            if(n == 8) if(j+1 != xl) { n = 0; m++; }
            tar++;
        }
        m++;
        if(type == 1) color++;  // 멀티 칼라 출력
        tar += _han_lPitch -xl;
    }
}



void xHan::gap_fill(int x1, int y1, int x2, int y2, WORD *tar, WORD color)
{
    int i;
    int xl = x2-x1+1;

    tar += y1 * _han_lPitch + x1;

    for(i=0; i<=y2-y1; i++)
    {
        memset( tar , color, xl);
        tar += _han_lPitch;
    }
}

void xHan::htextxy(int hindex, WORD *tar)
{
    // 그림자 출력을 위해서 _nx+1, _by+1 좌표에 먼저 단일색으로 글자를 쓰고
    // _nx, _ny좌표에 다시 글자를 덧씌워 출력한다.

    if(HAN_SHADOW == 1)  // 그림자 출력
	{
	put_font( _nx+1, 
			  _ny+1, 
			  kfnt[HAN_KORFONT].xl,
			  kfnt[HAN_KORFONT].yl,
			  kfnt[HAN_KORFONT].han[hindex],
			  tar, 
			  HAN_BGR, 
			  HAN_BGRCOLOR, 
			  0, 
			  HAN_SHADOWCOLOR);
	}

	if(HAN_SHADOW == 2)  // 외곽선 출력
	{
	put_font( _nx, 
			  _ny, 
			  kfnt[HAN_KORFONT].xl,
			  kfnt[HAN_KORFONT].yl,
			  kfnt[HAN_KORFONT].han[hindex],
			  tar, 
			  HAN_BGR, 
			  HAN_BGRCOLOR, 
			  2, 
			  HAN_SHADOWCOLOR);
	}

	
    put_font( _nx, 
		      _ny, 
			  kfnt[HAN_KORFONT].xl,
              kfnt[HAN_KORFONT].yl, 
			  kfnt[HAN_KORFONT].han[hindex], 
			  tar, 
			  HAN_BGR, 
			  HAN_BGRCOLOR, 
			  HAN_MULTI, 
			  HAN_COLOR);
	

    if(HAN_BGR==1)
    {
        if(HAN_GAP > 0)
        gap_fill(_nx + kfnt[HAN_KORFONT].xl, 
		         _ny,
                 _nx + kfnt[HAN_KORFONT].xl + HAN_GAP-1, 
				 _ny + kfnt[HAN_KORFONT].yl -1,
                 tar,
				 HAN_BGRCOLOR);
    }

    _nx += kfnt[HAN_KORFONT].xl + HAN_GAP;   // 글자 간격을 띄움 
}


void xHan::textxy(char text, WORD *tar)
{
    //memset(eng_mem, 0, efnt[HAN_ENGFONT].size);
    //memmove(eng_mem, efnt[HAN_ENGFONT].english[text], efnt[HAN_ENGFONT].size);

    if(HAN_SHADOW == 1)  // 그림자 출력
	{
	put_font( _nx+1, 
	   		  _ny+1, 
			  efnt[HAN_ENGFONT].xl,
              efnt[HAN_ENGFONT].yl, 
			  efnt[HAN_ENGFONT].english[text], 
			  tar, 
			  HAN_BGR, 
			  HAN_BGRCOLOR, 
			  0, 
			  HAN_SHADOWCOLOR); 
	}

	if(HAN_SHADOW == 2)  // 외곽선 출력
	{
	put_font( _nx, 
	   		  _ny, 
			  efnt[HAN_ENGFONT].xl,
              efnt[HAN_ENGFONT].yl, 
			  efnt[HAN_ENGFONT].english[text], 
			  tar, 
			  HAN_BGR, 
			  HAN_BGRCOLOR, 
			  2, 
			  HAN_SHADOWCOLOR); 
	}

    put_font( _nx, 
		      _ny, 
			  efnt[HAN_ENGFONT].xl,
              efnt[HAN_ENGFONT].yl, 
			  efnt[HAN_ENGFONT].english[text], 
			  tar, 
			  HAN_BGR, 
			  HAN_BGRCOLOR, 
			  HAN_MULTI, 
			  HAN_COLOR);

    if(HAN_BGR==1)     // 배경색 ON일 경우 글자 사이의 공백을 배경색으로 채운다
    {
        if(HAN_GAP > 0)
        gap_fill(_nx + efnt[HAN_ENGFONT].xl, _ny,
                 _nx + efnt[HAN_ENGFONT].xl + HAN_GAP-1, 
				 _ny + efnt[HAN_ENGFONT].yl -1,
                 tar,
				 HAN_BGRCOLOR);
    }
    _nx += efnt[HAN_ENGFONT].xl + HAN_GAP;    // 글자 간격을 띄움 
}


/*-------------------------------------------------------------------------------
   완성형코드를 받아서 폰트 테이블의 인덱스를 리턴한다
---------------------------------------------------------------------------------*/
int xHan::GetKsIndex(BYTE fstByte,BYTE sndByte)
{
    return ((int)(fstByte  - 0xb0) * 94 + (int)(sndByte - 0xa1));
}


void xHan::gputs( int x, int y, WORD *tar, char *st)
{
	unsigned char data1, data2;
	unsigned int i;

    _nx = x;
    _ny = y;

	for( i=0; i < strlen(st) ; i++ )
	{
		data1 = *(st+i); 
        if ( data1 > 127 )     // 한글이면
		{
			i++;
			data2 = *(st+i);
            htextxy( GetKsIndex(data1, data2), tar );
		}
        else textxy( data1, tar );  // 영문이면 
	}
}

/*-----------------------------------------------------------------------
   한글 출력함수

		x,y : 찍을 좌표
	   (printf 문처럼 출력포맷을 자유롭게 해준다)
-----------------------------------------------------------------------*/
void xHan::Print( int x, int y, char *fmt, ...)
{
	va_list  argptr;
	char  str[255];

	va_start(argptr, fmt);
	vsprintf(str, fmt, argptr);

    gputs(x, y, p2SCREEN, str);
}


/*-----------------------------------------------------------------------
   한글 출력환경 스텍 푸쉬
-----------------------------------------------------------------------*/
void xHan::Push()
{
    if(sp_env_point > 3) return;

    env[sp_env_point].multi       = HAN_MULTI;
    env[sp_env_point].shadow      = HAN_SHADOW;
    env[sp_env_point].bgr         = HAN_BGR;
    env[sp_env_point].color       = HAN_COLOR;
    env[sp_env_point].shadowcolor = HAN_SHADOWCOLOR;
    env[sp_env_point].bgrcolor    = HAN_BGRCOLOR;
    env[sp_env_point].gap         = HAN_GAP;
    env[sp_env_point].hanfont     = HAN_KORFONT;
    env[sp_env_point].engfont     = HAN_ENGFONT;

    sp_env_point++;
}


/*-----------------------------------------------------------------------
   한글 출력환경 스텍 팝
-----------------------------------------------------------------------*/
void xHan::Pop()
{
    if(sp_env_point < 1) return;

    sp_env_point--;

    HAN_MULTI       = env[sp_env_point].multi;
    HAN_SHADOW      = env[sp_env_point].shadow;
    HAN_BGR         = env[sp_env_point].bgr;
    HAN_COLOR       = env[sp_env_point].color;
    HAN_SHADOWCOLOR = env[sp_env_point].shadowcolor;
    HAN_BGRCOLOR    = env[sp_env_point].bgrcolor;
    HAN_GAP         = env[sp_env_point].gap;
    HAN_KORFONT     = env[sp_env_point].hanfont;
    HAN_ENGFONT     = env[sp_env_point].engfont;
}
