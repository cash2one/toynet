/*============================================================================
                        ����95/98�� �ѱ۶��̺귯��

                     ������ : �� �� �� (HiTEL : 93XYZ)


            # ���ȯ�� : 16bit Color (�ػ� Free) ���ǽ�(Ȥ�� ����������)�� ���
			# ��Ʈ ������� ��������
            # �ϼ��� �ѱ� ���
=============================================================================*/

#include "StdAfx.h"
#include "xHan.h"


char xHan::outline_temp1[128], xHan::outline_temp2[128];	// �ƿ����� ������ �ӽ� ����


xHan::xHan()
{
	p2SCREEN = NULL;
	sp_env_point = 0;
	_han_xl=640, _han_yl=480;	// ȭ���� X,Y ����
	DWORD _han_lPitch = 640;		// lPitch

	//------- �Ʒ��� 'HAN_'�� �����ϴ� �������� ����ڰ� ������ �� �ִ� ---------
	HAN_MULTI = 0;            // ��Ƽ Į�� ����  (�⺻�� : �ܻ�)     
	HAN_SHADOW = 0;           // �׸��� ����     (�⺻�� : ����)     
	HAN_BGR=0;                // ���� ��� ����(�⺻�� : ��¾���) 
	
	HAN_COLOR = 20;           // ������ ����                         
	HAN_SHADOWCOLOR = 0;      // �׸����� ���� : ����Ʈ = 0����      
	HAN_BGRCOLOR = 0;         // ��� ���� : ����Ʈ = 0����          
	HAN_GAP = 1;              // ���� ����                           
	HAN_KORFONT = 1;          // ���� ������� �ѱ� ��Ʈ ��ȣ        
	HAN_ENGFONT = 1;          // ���� ������� ���� ��Ʈ ��ȣ        
	//---------------------------------------------------------------------------
}

xHan::~xHan()
{
	Destroy();
}

/*-------------------------------------------------------------------------------
   �ѱ� ���̺귯�� �ʱ�ȭ �Լ�
        xl, yl : ����� ȭ���� ������(���� ������Ȥ�� ���ǽ��� ���� ���� ����)
		lPitch : ����� �������� lPitch(����: ���� ������)
---------------------------------------------------------------------------------*/
int xHan::Init(WORD *pscreen, int xl, int yl, DWORD lPitch)
{
    int k,i;

	_han_xl = xl;
	_han_yl = yl;
	_han_lPitch = lPitch;
	p2SCREEN = pscreen;

    for(k=0; k < KMAXFONT; k++)  // �޸� �ʱ�ȭ 
    {
		memset(&kfnt[k].han, NULL, 2350);
    }

    for(k=0; k < EMAXFONT; k++)  // �޸� �ʱ�ȭ 
    {
        for(i=0; i<128; i++) efnt[k].english[i]   = NULL;
    }

    // �ѱ� ��Ʈ�� ����
	BOOL check = TRUE;

    check = LoadKorFont( 0,".\\Font\\k8x10g0.fnt", 8, 10);
    check = LoadKorFont( 1,".\\Font\\k16x16g1.fnt", 16, 16);
    check = LoadKorFont( 2,".\\Font\\k16x16g2.fnt", 16, 16);
    check = LoadKorFont( 3,".\\Font\\k16x16g3.fnt", 16, 16);
    check = LoadKorFont( 4,".\\Font\\k16x16g4.fnt", 16, 16);
	check = LoadKorFont( 5,".\\Font\\k16x16g5.fnt", 16, 16);

    // ���� ��Ʈ�� ����
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
   ��ũ�� ��� �缳��
---------------------------------------------------------------------------------*/
void xHan::SetScreenMode(WORD *pscreen, int xl, int yl, DWORD lPitch)
{
 	_han_xl = xl;
	_han_yl = yl;
	_han_lPitch = lPitch;
	p2SCREEN = pscreen;
}

/*-------------------------------------------------------------------------------
   �ѱ� ���̺귯�� ���� �Լ�
---------------------------------------------------------------------------------*/
void xHan::Destroy()
{
    int k,i;

    for(k=0; k < KMAXFONT; k++)  // �Ҵ�� �޸𸮸� ��ȯ�Ѵ�
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

    for(k=0; k < EMAXFONT; k++)  // �Ҵ�� �޸𸮸� ��ȯ�Ѵ�
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
	case 1: (flag)? HAN_MULTI=TRUE : HAN_MULTI=FALSE; break;	// ��Ƽ Į�� ���
	case 2: (flag)? HAN_SHADOW=1 : HAN_SHADOW=0; break;			// �׸��� ���
	case 3: (flag)? HAN_SHADOW=2 : HAN_SHADOW=0; break;			// �ܰ��� ���
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
   �ѱ� ��Ʈ �б� �Լ�
        hanFile      : ���� �ѱ���Ʈ ���ϸ�
        xl, yl       : �ѱ� ��Ʈ�� X, Y����
        fnum         : ��Ʈ ��ȣ
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

    fclose(fp); // ���ҽ� ������ �ƴҰ�� 
    return TRUE;
}


/*-------------------------------------------------------------------------------
   ���� ��Ʈ �б� �Լ�

        engFile      : ���� ������Ʈ ���ϸ�
        xl, yl       : ���� ��Ʈ�� X, Y����
        fnum         : ��Ʈ ��ȣ
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
    ��Ʈ�� target ���ǽ�(Ȥ�� ����������)�� �Ѹ� --- �׽�Ʈ��

    x,y  : ����� ��ǥ                   xl,yl    : ��Ʈ�� ���� ���� ũ��
    buf  : ��Ʈ �̹���                   tar      : ����� ������
    bgr  : ���� ��� ����              bgrColor : ����� ����
	multi: 0=������� 1=��Ƽ���         color : ����� ����
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
        if(multi == 1) color++;  // ��Ƽ Į�� ���
        tar += _han_lPitch -xl;
    }
}
*/

/*----------------------------------------------------------------------------
   ������ ��Ʈ ��� �Լ�(Ŭ���� ó���� Ư�� ȿ����)

    x1, y1, x2, y2 : �ʰ�ġ
    (������ ���Ұ�, ���߿� ������)
----------------------------------------------------------------------------*/
/*
void xHan::put_font_clip( int x, int y, int xl, int yl, int x1, int y1, int x2, int y2,\
      char *buf, WORD *tar, int bgr, WORD bgrColor, int multi, WORD color)
{
    int i,j,m=0,n=0,xlim,ylim;
    unsigned char mask = 0x80;        // = 10000000b 
    tar += (y << 8) + (y << 6) + x;   //  y * 320 + x �� ����.
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
        if(multi == 1) color++;  // ��Ƽ Į�� ���
        tar += _han_lPitch-xl;
    }
}
*/

/*-----------------------------------------------------------------------------
    ��Ʈ�� target ���ǽ�(Ȥ�� ����������)�� �Ѹ�

    x,y  : ����� ��ǥ                   xl,yl    : ��Ʈ�� ���� ���� ũ��
    buf  : ��Ʈ �̹���                   tar      : ����� ������
    bgr  : ���� ��� ����              bgrColor : ����� ����
	type : 0:Normal 1:Multi 2:Outline
	color : ����� ����
-------------------------------------------------------------------------------*/
void xHan::put_font( int x, int y, int xl, int yl, char *buf,\
               WORD *tar, int bgr, WORD bgrColor, int type, WORD color)
{
    int i,j,m=0,n=0;
    unsigned char mask = 0x80;        // = 10000000b
	int xsize, size;
    
	tar += y * _han_lPitch + x;  
    
	if(type==2)  // �ܰ������
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

		for(i=0; i<size; i++)  // ���� �¿�� �þ ���ڿ��� ���� �̹����� ����
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
	else for(i = 0; i < yl; i++)  // �Ϲ����
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
        if(type == 1) color++;  // ��Ƽ Į�� ���
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
    // �׸��� ����� ���ؼ� _nx+1, _by+1 ��ǥ�� ���� ���ϻ����� ���ڸ� ����
    // _nx, _ny��ǥ�� �ٽ� ���ڸ� ������ ����Ѵ�.

    if(HAN_SHADOW == 1)  // �׸��� ���
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

	if(HAN_SHADOW == 2)  // �ܰ��� ���
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

    _nx += kfnt[HAN_KORFONT].xl + HAN_GAP;   // ���� ������ ��� 
}


void xHan::textxy(char text, WORD *tar)
{
    //memset(eng_mem, 0, efnt[HAN_ENGFONT].size);
    //memmove(eng_mem, efnt[HAN_ENGFONT].english[text], efnt[HAN_ENGFONT].size);

    if(HAN_SHADOW == 1)  // �׸��� ���
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

	if(HAN_SHADOW == 2)  // �ܰ��� ���
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

    if(HAN_BGR==1)     // ���� ON�� ��� ���� ������ ������ �������� ä���
    {
        if(HAN_GAP > 0)
        gap_fill(_nx + efnt[HAN_ENGFONT].xl, _ny,
                 _nx + efnt[HAN_ENGFONT].xl + HAN_GAP-1, 
				 _ny + efnt[HAN_ENGFONT].yl -1,
                 tar,
				 HAN_BGRCOLOR);
    }
    _nx += efnt[HAN_ENGFONT].xl + HAN_GAP;    // ���� ������ ��� 
}


/*-------------------------------------------------------------------------------
   �ϼ����ڵ带 �޾Ƽ� ��Ʈ ���̺��� �ε����� �����Ѵ�
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
        if ( data1 > 127 )     // �ѱ��̸�
		{
			i++;
			data2 = *(st+i);
            htextxy( GetKsIndex(data1, data2), tar );
		}
        else textxy( data1, tar );  // �����̸� 
	}
}

/*-----------------------------------------------------------------------
   �ѱ� ����Լ�

		x,y : ���� ��ǥ
	   (printf ��ó�� ��������� �����Ӱ� ���ش�)
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
   �ѱ� ���ȯ�� ���� Ǫ��
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
   �ѱ� ���ȯ�� ���� ��
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
