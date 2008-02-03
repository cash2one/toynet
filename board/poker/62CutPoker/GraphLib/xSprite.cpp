
#include "StdAfx.h"
#include "xSprite.h"

#include "JpegFile.h"	// JPEG ���
#include "dither.h"     // ����� ���

///////////////////////////////////////////////////////////////////////////

/*--------------------------------------------------------------------

          <  0 �� ���� ���� ����(����� ������ ����Ÿ �κ�) >

 Byte(s)

 4 * height  - �������� ���� �������� �Ÿ��� ��� �迭
   2         - ����(pattern ) �� ---------------------------+
   2         - ����� �� ---------------+                   | ���α��� ��ŭ
   2         - �׸� ����                | ���ϼ���ŭ �ݺ�   | �ݺ�
  size       - ����Ÿ(����*2)-----------+ ------------------+

  ## ����Ÿ�� ũ��� ����*2�̴�(16��Ʈ �̹Ƿ�) ��, ���� ����Ʈ���� �ǹ��Ѵ�
  ## �������� �Ÿ� : ����Ÿ�� ó���������� �� ������ ���۵Ǵ� ��������
                     �Ÿ��� ��Ÿ����.(�������� ���̰� �ƴ�)
                     (Ŭ���ν� �ӵ��� ���̰� ó���� �����ϰ��ϱ�����)
					 (����: �Ÿ�1�� ���� 2����Ʈ������)
  ## ��������� �׸�����Ÿ���� WORD���̴�.
 --------------------------------------------------------------------*/



//#define  TRANS  0

typedef struct {
    WORD skipb;
    WORD putb;
} BLKPATTERN;

/*---------------------------------------------------------
  ���ۿ� �̹����� �����Ͽ� �����ϴ� �Լ�

  WORD *image   : �Էµ� �̹��� ������(16��Ʈ)
  int  xl       : �Է� �̹����� ���α���
  int  yl       : �Է� �̹����� ���α���
  WORD **output : ��µ� ���� �̹����� ���� ������
                  (�ݵ�� NULL �̾���Ѵ�, �����������ӿ� ����)
  WORD TRANS    : Color Key

  ���ϰ�  -->  ����� ������(���н� 0)

  ���� ) ���� �̹����� �����̹����� ������
         ���� �������Ƿ� �̸� ���� free ���־���Ѵ�.
-----------------------------------------------------------*/
unsigned long
EncodeSpr0(WORD *image, int xl, int yl, WORD **output, WORD TRANS)
{
	if(!image || xl <= 0 || yl <= 0) return 0;

    WORD i,k,cnt;
    short pNum;        // ������ ����
    BLKPATTERN pat[1024] = {0,};

    unsigned long bp=0;         // ���� �ε���
    unsigned long dsize = 0;        // ����� ��ü ũ��

	// �ӽ� ���� �Ҵ�(����ũ���� 4�� + LDT���̺����) 
	// ���α��̴� �ּ� 8�ȼ� �̻����� ����(8���� ���� ��� 4�踦 �ϴ��� ���۰� ���ڶ� �� ����)
	WORD *buf = (WORD *)malloc((max(8,xl)*4)*yl+(4*yl));
	if(buf==NULL) return 0;

    bp += yl*2;   // �� ������ �Ÿ��� ��� ������ �ǳʶ�(����: buf�� WORD�� �������̹Ƿ�... yl*2)
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
        memcpy(buf+k*2, &bp, 4); // ����: buf�� WORD�� �������̹Ƿ�... k*2

        // ���δ� ������ ����
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

    dsize=bp*2; // LDT���̺� + �̹���ũ��
    *output = (WORD *)malloc(dsize);
    memcpy(*output, buf, dsize); // �ӽù����� ������ ����
    free(buf);
    return(dsize);
}

/*---------------------------------------------------------
  ���ེ������Ʈ�� ���ۿ� ���ڵ��ϴ� �Լ�
  (������ ũ��� '���α���*���α���' �̾���Ѵ�)
-----------------------------------------------------------*/
void DecodeSpr0( WORD *tar, int xl, int yl, WORD *sor, WORD TRANS)
{
    int  i,j;
    int  sbyte,pbyte;
    short cnt;

	for(i=0; i<yl; i++) // �켱 ��� ������� �����Ѵ�
	{
		for(j=0; j<xl; j++) *(tar+(xl*i)+j) = TRANS; 
	}

    sor += yl*2; // ���ΰŸ� ����Ÿ�� �ǳʶ�
                 // Ŭ������ �����Ƿ� �ʿ� ����

	//sor += *((long*)sor);    // ��� ������ �������� �̵�

    for(i=0;i<yl;i++,tar+=xl) 
	{
        cnt = *sor;      // ���� ���� ���
        sor++;
        sbyte=0;
        while(cnt--) {

            sbyte+=(*(WORD*)sor); // ������� ���
            sor++;

            pbyte =(*(WORD*)sor); // ���� ���
            sor++;

            memcpy(tar+sbyte, sor, pbyte<<1); // �̹��� ����ֱ� (16��Ʈ�̹Ƿ� *2)

            sor+=pbyte;
            sbyte+=pbyte;
        }
    }
}

///////////////////////////////////////////////////////////////////////////

//class xDraw;
//extern static int xDraw::PixelFormat; // ��ũ���� Pixel Format�� �˾ƾ� �ϱ� ����

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

void ANALYSIS_RGB555(WORD color, WORD* r, WORD* g, WORD* b) // 16��Ʈ(555)�� RGB�� �м��Ѵ�
{
	*r = (color & 0x7C00)>>10;
	*g = (color & 0x03E0)>>5;
	*b = (color & 0x001F);
}

void ANALYSIS_RGB565(WORD color, WORD* r, WORD* g, WORD* b) // 16��Ʈ(565)�� RGB�� �м��Ѵ�
{
	*r = (color & 0xF800)>>11;
	*g = (color & 0x03E0)>>5; 
	*b = (color & 0x07E0);
	// ����) 565���� Green�� �ι��̹Ƿ� ����Ʈ�� �ѹ� ���ؾ� 
	//       RGB ���սÿ� ��Ȯ�� ���� ���´�
	//       �׷��� ���⼱ �ѹ� �� ���� �ʾҴ�.
}


//================================ ��������Ʈ ���� 3.0 Ŭ���� =================================//


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
  �ε��� ��������Ʈ�� ����
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
  ��������Ʈ ������ �ε�(��ũ���� �ȼ� ���˿� ���� �д´�)
  name: ���ϸ�
  (����: xCanvas�� ���� �ʱ�ȭ �Ǿ� �־�� �Ѵ�)
---------------------------------------------------------------------------*/
BOOL xSprite::Load(char *name)
{
	return(Load(name, 555));
}

/*--------------------------------------------------------------------------
  ��������Ʈ ������ �ε�
  name: ���ϸ�
  ScreenRGBMode: ������ �ȼ� �������� �о���δ�
---------------------------------------------------------------------------*/
BOOL xSprite::Load(char *name, int ScreenRGBMode)
{
    FILE *fp;

    SPRFILEHEAD tempsfhead;     // �ӽ� ���� ���
	SPRHEAD30 shead;            // �� ��������Ʈ�� ���
	

	if( (fp = fopen( name, "rb")) == NULL )
	{ 
		//PutErrStr("������ �����ϴ�(%s)", name); 
		return FALSE;
	}
    
	fread(&tempsfhead, sizeof(SPRFILEHEAD), 1, fp); //�⺻ ��� �ε�

    if(strncmp("93XYZ sprite",tempsfhead.Identifier,13) != 0) //��������Ʈ ������ �ƴϸ�
    {
		//PutErrStr("��������Ʈ ������ �ƴմϴ�(%s)", name);
		fclose(fp);
        return FALSE;
    }

	Remove();

	memcpy(&sfhead, &tempsfhead, sizeof(SPRFILEHEAD));

	spr = (SPRITE30*) malloc(sizeof(SPRITE30) * sfhead.TotalSprNum);
	if(spr==NULL) 
	{ 
		//PutErrStr("�޸� �Ҵ� ����(���)"); 
		return FALSE; 
	}
	ZeroMemory(spr, sizeof(SPRITE30) * sfhead.TotalSprNum);

    for(int i=0; i < sfhead.TotalSprNum; i++)
    {
		fread(&shead, sizeof(SPRHEAD30), 1, fp); // �� ��������Ʈ ������ ��� �ε�
		
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

		///// �浹 ��ǥ, ���� ��ǥ �б� /////
		if(spr[i].hitNum>0) spr[i].hit = (SPRPOS*)malloc(sizeof(SPRPOS)*spr[i].hitNum);
		if(spr[i].atckNum>0) spr[i].atck = (SPRPOS*)malloc(sizeof(SPRPOS)*spr[i].atckNum);
		fread(spr[i].hit, sizeof(SPRPOS)*spr[i].hitNum, 1, fp);
		fread(spr[i].atck, sizeof(SPRPOS)*spr[i].atckNum, 1, fp);

		if(spr[i].SprFormat == 1) {
			// ����� �ɼ��� �����Ǿ� �ִٸ� ����� ����
			BOOL bDither = FALSE;
			if(spr[i].Dithering == 1) bDither = TRUE;

			//// JPEG ���� �б�
			fpos_t pos;
			fgetpos(fp, &pos);

			int width=0, height=0;
			WORD *pNewImage = Make16bitFromJpeg(fp, &width, &height, bDither);
			if(!pNewImage) {
				//PutErrStr("JPEG ���� �б� ����");
				fclose(fp);
				return FALSE;
			}

			if(width != spr[i].xl || height != spr[i].yl) {
				//PutErrStr("JPEG ������ �̹��� ����� ��ġ���� ����");
				fclose(fp);
				return FALSE;
			}

			spr[i].dsize = width * height * 2;
			spr[i].data = pNewImage;

			// ������� ���� ���·� �ϴ� ��ȯ
			spr[i].compress = 0;

			if(shead.compress == 1) {
				// ���� ��Ŵ
				Compress(i);
			}

			// JPEG �̹��� ������ ����(���ӿ����� �ʿ� ����)
			//fsetpos(fp, &pos);
			//spr[i].jpgsize = shead.dsize;
			//spr[i].pJpgData = new BYTE[shead.dsize];
			//fread(spr[i].pJpgData, shead.dsize, 1, fp);

			// ���� ��������Ʈ�� ���� ������ �̵�
			pos += shead.dsize;
			fsetpos(fp, &pos);
		}
		else {

			///// ��������Ʈ �̹��� ����Ÿ �б�
			spr[i].data = (WORD *)malloc( shead.dsize );
			if(spr[i].data==NULL) { 
				//PutErrStr("�޸� �Ҵ� ����(�̹���)"); 
				return FALSE; 
			}
			ZeroMemory(spr[i].data, shead.dsize);
			fread(spr[i].data, shead.dsize, 1, fp);
		}
    }

	//// ���ϸ��̼� ������ �б�
	if(sfhead.CharacterNum > 0) ani.Load(fp);

	fclose(fp);

	if(sfhead.FixelFormat != 4444) ChangeFixelFormat(ScreenRGBMode);

    return TRUE;
}

/*--------------------------------------------------------------------------
  ��������Ʈ ����
---------------------------------------------------------------------------*/	
BOOL xSprite::Compress(LONG snum)
{
	WORD *buf = NULL;           // ����� �̹����� ����� ����
	long newsize=0;

	if(spr==NULL) return FALSE;
	if(snum < 0 || snum >= sfhead.TotalSprNum) return FALSE;
	if(spr[snum].data == NULL) return FALSE;
	if(spr[snum].compress == 1) return FALSE;

	newsize = EncodeSpr0(spr[snum].data, spr[snum].xl, spr[snum].yl, &buf, spr[snum].colorkey);
	if(newsize == 0) return FALSE;
	
	free(spr[snum].data);          // ������ �̹����� ����
	spr[snum].compress = 1;
	spr[snum].data     = buf;      // ���� ����� �̹���
	spr[snum].dsize    = newsize;  // ����� ������

	return TRUE;
}

/*--------------------------------------------------------------------------
  ��������Ʈ ���� ����
---------------------------------------------------------------------------*/
BOOL xSprite::DeCompress(LONG snum)
{
	WORD *buf = NULL;           // ������ Ǯ�� �̹����� ����� ����
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

	free(spr[snum].data);          // ������ �̹����� ����
	spr[snum].compress = 0;
	spr[snum].data     = buf;      // ������ Ǯ�� �̹��� ������ ������
	spr[snum].dsize    = newsize;  // ������ Ǯ�� ������

	return TRUE;
}

/*--------------------------------------------------------------------------
  ��������Ʈ�� Ư�� �ȼ� �������� �ٲ۴�
  (NewForm = 555, 565, -565)
---------------------------------------------------------------------------*/
BOOL xSprite::ChangeFixelFormat(int NewForm)
{
	int i,x,y;
	if(NewForm == sfhead.FixelFormat) return TRUE;

	if(NewForm!=555 && NewForm!=565 && NewForm!=-565) return FALSE;

	WORD color;
	
    for(i=0; i < sfhead.TotalSprNum; i++) // �� ��������Ʈ ����ŭ
    {
		color = spr[i].colorkey; // ������� ��´�.

		// ������� �ȼ� ������ ��ȯ�Ѵ�
		if(sfhead.FixelFormat==565 && NewForm==555) // RGB 5:6:5 --> RGB 5:5:5
					spr[i].colorkey = RGB565_TO_RGB555(color);
		if(sfhead.FixelFormat==555 && NewForm==565) // RGB 5:5:5 --> RGB 5:6:5
					spr[i].colorkey = RGB555_TO_RGB565(color);
		if(sfhead.FixelFormat==555 && NewForm==-565) // RGB 5:5:5 --> BGR 5:6:5
					spr[i].colorkey = RGB555_TO_BGR565(color);
		if(sfhead.FixelFormat==565 && NewForm==-565) // RGB 5:6:5 --> BGR 5:6:5
					spr[i].colorkey = RGB565_TO_BGR565(color);
		
		// �̹��� �ȼ� ���� ��ȯ(����� ��������Ʈ�� ���)
		if(spr[i].compress)
		{
			int  m,xl,yl;
			int  sbyte,pbyte;
			short cnt;

			xl = spr[i].xl;
			yl = spr[i].yl;
			WORD* tar = spr[i].data + yl*2; // ���ΰŸ� ����Ÿ�� �ǳʶ�

			for(m=0; m<yl; m++)
			{
		        cnt = *tar++;      // ���� ���� ���
				sbyte=0;
		        while(cnt--) {

				    sbyte+=(*(WORD*)tar); // ������� ���
		            tar++;

		            pbyte =(*(WORD*)tar); // ���� ���
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
		else // ������� ���� ��������Ʈ�� ���
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
  Ư�� �̹����� �������� ��������Ʈ�� ����
  (�̹����� ���������� �ʴ´�)
---------------------------------------------------------------------------*/
BOOL xSprite::TransToSpr(int xl, int yl, WORD *pImage, WORD wColorKey)
{
	if(pImage == NULL) return FALSE;
	if(xl <= 0 || yl <= 0) return FALSE;
	Remove();
	
	spr = (SPRITE30*) malloc(sizeof(SPRITE30));
	if(spr==NULL) { 
		//PutErrStr("�޸� �Ҵ� ����(At TransToSpr)"); 
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
  Ư�� �̹����� �������� ��������Ʈ�� ����
  (�̹��� ���۸� ���������� �Ҵ��Ͽ� ������)
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
  JPEG ���� �����ͷ� 16��Ʈ 555 �̹����� ����
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
		// ����� �ɼ��� ������ ���
		DWORD rmask = (DWORD)0x0000001F;
		DWORD gmask = (DWORD)0x000003E0;
		DWORD bmask = (DWORD)0x00007C00;
		// 24��Ʈ �̹����� ������Ͽ� 16��Ʈ(5:5:5) �̹����� ��ȯ
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
			// 16��Ʈ�� ��ȯ
			static char k=0;
			k=0;

			for(int j=0; j<xsize; j++) {
				// ����) ���⼱ RGB�������� ��Ʈ���� BGR���̴�
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
