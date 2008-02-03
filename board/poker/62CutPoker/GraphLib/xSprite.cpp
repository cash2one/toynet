
#include "StdAfx.h"
#include "xSprite.h"

#include "JpegFile.h"	// JPEG 모듈
#include "dither.h"     // 디더링 모듈

///////////////////////////////////////////////////////////////////////////

/*--------------------------------------------------------------------

          <  0 번 압축 포맺 구조(헤더를 제외한 데이타 부분) >

 Byte(s)

 4 * height  - 각라인의 시작 오프셋의 거리가 담긴 배열
   2         - 패턴(pattern ) 수 ---------------------------+
   2         - 투명색 수 ---------------+                   | 세로길이 만큼
   2         - 그릴 점수                | 패턴수만큼 반복   | 반복
  size       - 데이타(점수*2)-----------+ ------------------+

  ## 데이타의 크기는 점수*2이다(16비트 이므로) 즉, 실제 바이트수를 의미한다
  ## 각라인의 거리 : 데이타의 처음에서부터 각 라인이 시작되는 곳까지의
                     거리를 나타낸다.(각라인의 길이가 아님)
                     (클리핑시 속도를 높이고 처리를 용이하게하기위함)
					 (주의: 거리1은 실제 2바이트길이임)
  ## 투명색수와 그릴데이타수는 WORD형이다.
 --------------------------------------------------------------------*/



//#define  TRANS  0

typedef struct {
    WORD skipb;
    WORD putb;
} BLKPATTERN;

/*---------------------------------------------------------
  버퍼에 이미지를 압축하여 복사하는 함수

  WORD *image   : 입력될 이미지 포인터(16비트)
  int  xl       : 입력 이미지의 가로길이
  int  yl       : 입력 이미지의 세로길이
  WORD **output : 출력될 압축 이미지를 받을 포인터
                  (반드시 NULL 이어야한다, 이중포인터임에 주의)
  WORD TRANS    : Color Key

  리턴값  -->  압축된 사이즈(실패시 0)

  주의 ) 원본 이미지는 압축이미지를 받은후
         쓸모 없어지므로 이를 직접 free 해주어야한다.
-----------------------------------------------------------*/
unsigned long
EncodeSpr0(WORD *image, int xl, int yl, WORD **output, WORD TRANS)
{
	if(!image || xl <= 0 || yl <= 0) return 0;

    WORD i,k,cnt;
    short pNum;        // 패턴의 개수
    BLKPATTERN pat[1024] = {0,};

    unsigned long bp=0;         // 버퍼 인덱스
    unsigned long dsize = 0;        // 압축된 전체 크기

	// 임시 버퍼 할당(가로크기의 4배 + LDT테이블공간) 
	// 가로길이는 최소 8픽셀 이상으로 가정(8보다 작은 경우 4배를 하더라도 버퍼가 모자랄 수 있음)
	WORD *buf = (WORD *)malloc((max(8,xl)*4)*yl+(4*yl));
	if(buf==NULL) return 0;

    bp += yl*2;   // 각 라인의 거리가 담긴 공간은 건너띔(주의: buf가 WORD형 포인터이므로... yl*2)
    for(k=0; k<yl; k++)
    {
        for(i=0,pNum=0; i<xl;)
        {
            // skip bytes
			for(cnt=0; i<xl && *(image+i)==TRANS; cnt++,i++) ;
            pat[pNum].skipb = cnt;

            // put bytes
			for(cnt=0; i<xl && *(image+i)!=TRANS; cnt++,i++) ;
            pat[pNum].putb = cnt;

            pNum++;
        }

        // Line-Distance-Table
        memcpy(buf+k*2, &bp, 4); // 주의: buf가 WORD형 포인터이므로... k*2

        // 라인당 페턴의 개수
		*(buf+bp) = pNum;
        bp++;

        // Copy pattern to temporary buffers
        for(i=0;i<pNum;i++)
        {
            memcpy(&buf[bp], &pat[i],sizeof(BLKPATTERN)); 
			bp+=2;
            if(pat[i].putb!=0)
            {
				 // Copy Image
                 memcpy(buf+bp, image+pat[i].skipb, pat[i].putb*2); // for 16bit
                 bp+=pat[i].putb;
            }

            image += pat[i].skipb + pat[i].putb;
        }
    }

    dsize=bp*2; // LDT테이블 + 이미지크기
    *output = (WORD *)malloc(dsize);
    memcpy(*output, buf, dsize); // 임시버퍼의 내용을 복사
    free(buf);
    return(dsize);
}

/*---------------------------------------------------------
  압축스프라이트를 버퍼에 디코딩하는 함수
  (버퍼의 크기는 '가로길이*세로길이' 이어야한다)
-----------------------------------------------------------*/
void DecodeSpr0( WORD *tar, int xl, int yl, WORD *sor, WORD TRANS)
{
    int  i,j;
    int  sbyte,pbyte;
    short cnt;

	for(i=0; i<yl; i++) // 우선 모두 투명색을 세팅한다
	{
		for(j=0; j<xl; j++) *(tar+(xl*i)+j) = TRANS; 
	}

    sor += yl*2; // 라인거리 데이타를 건너띔
                 // 클리핑이 없으므로 필요 없음

	//sor += *((long*)sor);    // 찍기 시작할 라인으로 이동

    for(i=0;i<yl;i++,tar+=xl) 
	{
        cnt = *sor;      // 패턴 개수 얻기
        sor++;
        sbyte=0;
        while(cnt--) {

            sbyte+=(*(WORD*)sor); // 투명색수 얻기
            sor++;

            pbyte =(*(WORD*)sor); // 점수 얻기
            sor++;

            memcpy(tar+sbyte, sor, pbyte<<1); // 이미지 찍어주기 (16비트이므로 *2)

            sor+=pbyte;
            sbyte+=pbyte;
        }
    }
}

///////////////////////////////////////////////////////////////////////////

//class xDraw;
//extern static int xDraw::PixelFormat; // 스크린의 Pixel Format을 알아야 하기 때문

WORD RGB555_TO_RGB565(WORD color) // RGB555 --> RGB565
{
	WORD r = (color & 0x7C00)<<1;
	WORD g = (color & 0x03E0)<<1;
	WORD b = (color & 0x001F);
	return (r|g|b);
}

WORD RGB565_TO_RGB555(WORD color) // RGB565 --> RGB555
{
	WORD r = (color & 0xF800)>>1;
	WORD g = (color & 0x03E0)>>1;
	WORD b = (color & 0x07E0);
	return (r|g|b);
}

WORD RGB555_TO_BGR565(WORD color) // RGB555 --> BGR565
{
	WORD r = (color & 0x7C00)>>10;
	WORD g = (color & 0x03E0)<<1;
	WORD b = (color & 0x001F)<<11;
	return (b|g|r);
}

WORD RGB565_TO_BGR565(WORD color) // RGB565 --> BGR565
{
	WORD r = (color & 0xF800)>>11;
	WORD g = (color & 0x03E0);
	WORD b = (color & 0x07E0)<<11;
	return (b|g|r);
}

void ANALYSIS_RGB555(WORD color, WORD* r, WORD* g, WORD* b) // 16비트(555)의 RGB를 분석한다
{
	*r = (color & 0x7C00)>>10;
	*g = (color & 0x03E0)>>5;
	*b = (color & 0x001F);
}

void ANALYSIS_RGB565(WORD color, WORD* r, WORD* g, WORD* b) // 16비트(565)의 RGB를 분석한다
{
	*r = (color & 0xF800)>>11;
	*g = (color & 0x03E0)>>5; 
	*b = (color & 0x07E0);
	// 참고) 565에선 Green이 두배이므로 쉬프트를 한번 더해야 
	//       RGB 조합시에 정확한 색이 나온다
	//       그러나 여기선 한번 더 하지 않았다.
}


//================================ 스프라이트 버젼 3.0 클래스 =================================//


xSprite::xSprite()
{
	ZeroMemory(&sfhead, sizeof(SPRFILEHEAD));
	spr=NULL;
}

xSprite::~xSprite()
{
	Remove();
}


/*--------------------------------------------------------------------------
  로딩된 스프라이트를 삭제
---------------------------------------------------------------------------*/
void xSprite::Remove()
{
    int i;
    if(spr != NULL) 
	{
		for(i=0; i < sfhead.TotalSprNum; i++)
		{
	        if( spr[i].hit != NULL ) { free(spr[i].hit); spr[i].hit = NULL; }
			if( spr[i].atck != NULL ) { free(spr[i].atck); spr[i].atck = NULL; }
			if( spr[i].data != NULL ) { free(spr[i].data); spr[i].data = NULL; }
	    }
		free(spr); spr=NULL;
		ZeroMemory(&sfhead, sizeof(SPRFILEHEAD));
		ani.Destroy();
	}
}

/*--------------------------------------------------------------------------
  스프라이트 파일을 로드(스크린의 픽셀 포맷에 따라 읽는다)
  name: 파일명
  (주의: xCanvas가 먼저 초기화 되어 있어야 한다)
---------------------------------------------------------------------------*/
BOOL xSprite::Load(char *name)
{
	return(Load(name, 555));
}

/*--------------------------------------------------------------------------
  스프라이트 파일을 로드
  name: 파일명
  ScreenRGBMode: 지정한 픽셀 포맷으로 읽어들인다
---------------------------------------------------------------------------*/
BOOL xSprite::Load(char *name, int ScreenRGBMode)
{
    FILE *fp;

    SPRFILEHEAD tempsfhead;     // 임시 파일 헤더
	SPRHEAD30 shead;            // 각 스프라이트의 헤더
	

	if( (fp = fopen( name, "rb")) == NULL )
	{ 
		//PutErrStr("파일이 없습니다(%s)", name); 
		return FALSE;
	}
    
	fread(&tempsfhead, sizeof(SPRFILEHEAD), 1, fp); //기본 헤더 로드

    if(strncmp("93XYZ sprite",tempsfhead.Identifier,13) != 0) //스프라이트 파일이 아니면
    {
		//PutErrStr("스프라이트 파일이 아닙니다(%s)", name);
		fclose(fp);
        return FALSE;
    }

	Remove();

	memcpy(&sfhead, &tempsfhead, sizeof(SPRFILEHEAD));

	spr = (SPRITE30*) malloc(sizeof(SPRITE30) * sfhead.TotalSprNum);
	if(spr==NULL) 
	{ 
		//PutErrStr("메모리 할당 실패(헤더)"); 
		return FALSE; 
	}
	ZeroMemory(spr, sizeof(SPRITE30) * sfhead.TotalSprNum);

    for(int i=0; i < sfhead.TotalSprNum; i++)
    {
		fread(&shead, sizeof(SPRHEAD30), 1, fp); // 각 스프라이트 마다의 헤더 로드
		
		spr[i].compress = shead.compress;
		spr[i].colorkey = shead.colorkey;
        spr[i].rx       = shead.rx;
        spr[i].ry       = shead.ry;
        spr[i].xl       = shead.xl;
        spr[i].yl       = shead.yl;
        spr[i].mx       = shead.mx;
        spr[i].my       = shead.my;
        spr[i].hitNum   = shead.hitNum;
        spr[i].atckNum  = shead.atckNum;
        spr[i].hit      = NULL;
        spr[i].atck     = NULL;
        spr[i].dsize    = shead.dsize;
		spr[i].PixelFormat = shead.PixelFormat;
		spr[i].SprFormat = shead.SprFormat;
		spr[i].Dithering = shead.Dithering;
		spr[i].byUserVal = shead.byUserVal;
		//spr[i].pJpgData = NULL;
		//spr[i].jpgsize = 0;

		///// 충돌 좌표, 공격 좌표 읽기 /////
		if(spr[i].hitNum>0) spr[i].hit = (SPRPOS*)malloc(sizeof(SPRPOS)*spr[i].hitNum);
		if(spr[i].atckNum>0) spr[i].atck = (SPRPOS*)malloc(sizeof(SPRPOS)*spr[i].atckNum);
		fread(spr[i].hit, sizeof(SPRPOS)*spr[i].hitNum, 1, fp);
		fread(spr[i].atck, sizeof(SPRPOS)*spr[i].atckNum, 1, fp);

		if(spr[i].SprFormat == 1) {
			// 디더링 옵션이 설정되어 있다면 디더링 적용
			BOOL bDither = FALSE;
			if(spr[i].Dithering == 1) bDither = TRUE;

			//// JPEG 포맷 읽기
			fpos_t pos;
			fgetpos(fp, &pos);

			int width=0, height=0;
			WORD *pNewImage = Make16bitFromJpeg(fp, &width, &height, bDither);
			if(!pNewImage) {
				//PutErrStr("JPEG 포맷 읽기 실패");
				fclose(fp);
				return FALSE;
			}

			if(width != spr[i].xl || height != spr[i].yl) {
				//PutErrStr("JPEG 포맷의 이미지 사이즈가 일치하지 않음");
				fclose(fp);
				return FALSE;
			}

			spr[i].dsize = width * height * 2;
			spr[i].data = pNewImage;

			// 압축되지 않은 상태로 일단 전환
			spr[i].compress = 0;

			if(shead.compress == 1) {
				// 압축 시킴
				Compress(i);
			}

			// JPEG 이미지 원본을 보관(게임에서는 필요 없음)
			//fsetpos(fp, &pos);
			//spr[i].jpgsize = shead.dsize;
			//spr[i].pJpgData = new BYTE[shead.dsize];
			//fread(spr[i].pJpgData, shead.dsize, 1, fp);

			// 다음 스프라이트로 파일 포인터 이동
			pos += shead.dsize;
			fsetpos(fp, &pos);
		}
		else {

			///// 스프라이트 이미지 데이타 읽기
			spr[i].data = (WORD *)malloc( shead.dsize );
			if(spr[i].data==NULL) { 
				//PutErrStr("메모리 할당 실패(이미지)"); 
				return FALSE; 
			}
			ZeroMemory(spr[i].data, shead.dsize);
			fread(spr[i].data, shead.dsize, 1, fp);
		}
    }

	//// 에니메이션 데이터 읽기
	if(sfhead.CharacterNum > 0) ani.Load(fp);

	fclose(fp);

	if(sfhead.FixelFormat != 4444) ChangeFixelFormat(ScreenRGBMode);

    return TRUE;
}

/*--------------------------------------------------------------------------
  스프라이트 압축
---------------------------------------------------------------------------*/	
BOOL xSprite::Compress(LONG snum)
{
	WORD *buf = NULL;           // 압축된 이미지가 저장될 버퍼
	long newsize=0;

	if(spr==NULL) return FALSE;
	if(snum < 0 || snum >= sfhead.TotalSprNum) return FALSE;
	if(spr[snum].data == NULL) return FALSE;
	if(spr[snum].compress == 1) return FALSE;

	newsize = EncodeSpr0(spr[snum].data, spr[snum].xl, spr[snum].yl, &buf, spr[snum].colorkey);
	if(newsize == 0) return FALSE;
	
	free(spr[snum].data);          // 기존의 이미지를 해제
	spr[snum].compress = 1;
	spr[snum].data     = buf;      // 새로 압축된 이미지
	spr[snum].dsize    = newsize;  // 압축된 사이즈

	return TRUE;
}

/*--------------------------------------------------------------------------
  스프라이트 압축 해제
---------------------------------------------------------------------------*/
BOOL xSprite::DeCompress(LONG snum)
{
	WORD *buf = NULL;           // 압축이 풀린 이미지가 저장될 버퍼
	long newsize=0;

	if(spr==NULL) return FALSE;
	if(snum < 0 || snum >= sfhead.TotalSprNum) return FALSE;
	if(spr[snum].data == NULL) return FALSE;
	if(spr[snum].compress == 0) return FALSE;

	newsize = spr[snum].xl*2 * spr[snum].yl;
	buf = (WORD *)malloc(newsize);
	if(buf==NULL) return FALSE;

	//ZeroMemory(buf, newsize);
	DecodeSpr0(buf, spr[snum].xl, spr[snum].yl, spr[snum].data, spr[snum].colorkey);

	free(spr[snum].data);          // 기존의 이미지를 해제
	spr[snum].compress = 0;
	spr[snum].data     = buf;      // 압축이 풀린 이미지 데이터 포인터
	spr[snum].dsize    = newsize;  // 압축이 풀린 사이즈

	return TRUE;
}

/*--------------------------------------------------------------------------
  스프라이트를 특정 픽셀 포맷으로 바꾼다
  (NewForm = 555, 565, -565)
---------------------------------------------------------------------------*/
BOOL xSprite::ChangeFixelFormat(int NewForm)
{
	int i,x,y;
	if(NewForm == sfhead.FixelFormat) return TRUE;

	if(NewForm!=555 && NewForm!=565 && NewForm!=-565) return FALSE;

	WORD color;
	
    for(i=0; i < sfhead.TotalSprNum; i++) // 총 스프라이트 수만큼
    {
		color = spr[i].colorkey; // 투명색을 얻는다.

		// 투명색도 픽셀 포맷을 변환한다
		if(sfhead.FixelFormat==565 && NewForm==555) // RGB 5:6:5 --> RGB 5:5:5
					spr[i].colorkey = RGB565_TO_RGB555(color);
		if(sfhead.FixelFormat==555 && NewForm==565) // RGB 5:5:5 --> RGB 5:6:5
					spr[i].colorkey = RGB555_TO_RGB565(color);
		if(sfhead.FixelFormat==555 && NewForm==-565) // RGB 5:5:5 --> BGR 5:6:5
					spr[i].colorkey = RGB555_TO_BGR565(color);
		if(sfhead.FixelFormat==565 && NewForm==-565) // RGB 5:6:5 --> BGR 5:6:5
					spr[i].colorkey = RGB565_TO_BGR565(color);
		
		// 이미지 픽셀 포맷 변환(압축된 스프라이트일 경우)
		if(spr[i].compress)
		{
			int  m,xl,yl;
			int  sbyte,pbyte;
			short cnt;

			xl = spr[i].xl;
			yl = spr[i].yl;
			WORD* tar = spr[i].data + yl*2; // 라인거리 데이타를 건너띔

			for(m=0; m<yl; m++)
			{
		        cnt = *tar++;      // 패턴 개수 얻기
				sbyte=0;
		        while(cnt--) {

				    sbyte+=(*(WORD*)tar); // 투명색수 얻기
		            tar++;

		            pbyte =(*(WORD*)tar); // 점수 얻기
					tar++;

					for(int k=0; k<pbyte; k++)
					{
						color = *tar;
						if(sfhead.FixelFormat==565 && NewForm==555) // RGB 5:6:5 --> RGB 5:5:5
							*tar = RGB565_TO_RGB555(color);
						if(sfhead.FixelFormat==555 && NewForm==565) // RGB 5:5:5 --> RGB 5:6:5
							*tar = RGB555_TO_RGB565(color);
						if(sfhead.FixelFormat==555 && NewForm==-565) // RGB 5:5:5 --> BGR 5:6:5
							*tar = RGB555_TO_BGR565(color);
						if(sfhead.FixelFormat==565 && NewForm==-565) // RGB 5:6:5 --> BGR 5:6:5
							*tar = RGB565_TO_BGR565(color);
						tar++;
					}
					//tar+=pbyte;
					sbyte+=pbyte;
				}
			}
		}
		else // 압축되지 않은 스프라이트일 경우
		{
			for(y=0; y<spr[i].yl; y++)
			{
				for(x=0; x<spr[i].xl; x++)
				{
					WORD* tar = (spr[i].data + (spr[i].xl*y + x));
					color = *tar;
					if(sfhead.FixelFormat==565 && NewForm==555) // RGB 5:6:5 --> RGB 5:5:5
						*tar = RGB565_TO_RGB555(color);
					if(sfhead.FixelFormat==555 && NewForm==565) // RGB 5:5:5 --> RGB 5:6:5
						*tar = RGB555_TO_RGB565(color);
					if(sfhead.FixelFormat==555 && NewForm==-565) // RGB 5:5:5 --> BGR 5:6:5
						*tar = RGB555_TO_BGR565(color);
					if(sfhead.FixelFormat==565 && NewForm==-565) // RGB 5:6:5 --> BGR 5:6:5
						*tar = RGB565_TO_BGR565(color);
				}
			}
		}
    }
		
	sfhead.FixelFormat = NewForm;

    return TRUE;
}

/*--------------------------------------------------------------------------
  특정 이미지를 바탕으로 스프라이트를 생성
  (이미지를 복사하지는 않는다)
---------------------------------------------------------------------------*/
BOOL xSprite::TransToSpr(int xl, int yl, WORD *pImage, WORD wColorKey)
{
	if(pImage == NULL) return FALSE;
	if(xl <= 0 || yl <= 0) return FALSE;
	Remove();
	
	spr = (SPRITE30*) malloc(sizeof(SPRITE30));
	if(spr==NULL) { 
		//PutErrStr("메모리 할당 실패(At TransToSpr)"); 
		return FALSE; 
	}

	sfhead.TotalSprNum = 1;
	strcpy(sfhead.Identifier, "93XYZ sprite");
	sfhead.Version = 30;
	sfhead.BPD = 2;
	sfhead.CharacterNum = 0;

	spr[0].compress = 0;
	spr[0].colorkey = wColorKey;
	spr[0].xl       = xl;
    spr[0].yl       = yl;
	spr[0].rx       = 0;
	spr[0].ry       = 0;
    spr[0].mx       = xl/2;
    spr[0].my       = yl/2;
    spr[0].hitNum   = 0;
    spr[0].atckNum  = 0;
    spr[0].hit      = NULL;
	spr[0].atck     = NULL;
    spr[0].dsize    = xl*2 * yl;
	spr[0].data     = pImage;
	spr[0].PixelFormat = 555;

	return TRUE;
}

/*--------------------------------------------------------------------------
  특정 이미지를 바탕으로 스프라이트를 생성
  (이미지 버퍼를 내부적으로 할당하여 복사함)
---------------------------------------------------------------------------*/
BOOL xSprite::TransToSprByCopy(int xl, int yl, WORD *pImage, WORD wColorKey)
{
	if(pImage == NULL) return FALSE;
	if(xl <= 0 || yl <= 0) return FALSE;

	WORD* pNewImage = new WORD[xl * yl];
	memcpy(pNewImage, pImage, sizeof(WORD)*(xl*yl));

	if(!TransToSpr(xl, yl, pNewImage, wColorKey)) {
		delete pNewImage;
		return FALSE;
	}

	return TRUE;
}

/*--------------------------------------------------------------------------
  JPEG 파일 포인터로 16비트 555 이미지를 생성
---------------------------------------------------------------------------*/
WORD* xSprite::Make16bitFromJpeg(LPCTSTR fname, int* width, int* height, BOOL bDither)
{
	FILE *fp = fopen(fname,"rb");
	if(!fp) return NULL;
	WORD* pRtn = Make16bitFromJpeg(fp, width, height, bDither);
	fclose(fp);
	return pRtn;
}

WORD* xSprite::Make16bitFromJpeg(FILE *fp, int* width, int* height, BOOL bDither)
{
	*width = 0;
	*height = 0;

	BYTE *pImage24 = JpegFile::JpegFileToRGB(fp, (UINT*)width, (UINT*)height);
	if(!pImage24) return NULL;

	int xsize = *width;
	int ysize = *height;
	if(xsize <= 0 || ysize <= 0) return NULL;

	WORD* pNew = NULL;

	if(bDither) {
		// 디더링 옵션이 지정된 경우
		DWORD rmask = (DWORD)0x0000001F;
		DWORD gmask = (DWORD)0x000003E0;
		DWORD bmask = (DWORD)0x00007C00;
		// 24비트 이미지를 디더링하여 16비트(5:5:5) 이미지로 변환
		setup_dither_mask(rmask, gmask, bmask, 2);
		dither_rgb24(pImage24, xsize, ysize, (BYTE**)&pNew);
	}
	else {
		pNew = new WORD[xsize * ysize];

		BYTE* bit = pImage24;
		WORD* now = (WORD*)pNew;
		WORD color16;
		BYTE r,g,b;

		for(int i=0; i<ysize; i++) {
			// 16비트로 변환
			static char k=0;
			k=0;

			for(int j=0; j<xsize; j++) {
				// 주의) 여기선 RGB순이지만 비트맵은 BGR순이다
				r = *(bit+(j*3));
				g = *(bit+(j*3)+1);
				b = *(bit+(j*3)+2);

				color16 = RGB555(r,g,b);

				*((WORD*)now+j) = color16;
			}
			now += xsize;
			bit += xsize*3;
		}
	}

	delete pImage24;

	return pNew;
}
