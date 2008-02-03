//
//                                   �׷��� ó�� ���
//
#include "StdAfx.h"
#include "Graphic.h"
#include "Jpegfile.h"

#pragma message("linking with GraphEngine library")
#pragma comment(lib, "GraphEngine.lib")


// ��������Ʈ ��Ʈ �׽�Ʈ
BOOL PointHitTest(xSprite *pSpr, int sprnum, int x, int y, DWORD opt)
{
	if(pSpr==NULL) return FALSE;
	if(pSpr->spr==NULL) return FALSE;
	if(sprnum<0 || sprnum >= pSpr->sfhead.TotalSprNum) return FALSE;

	int tx = x;
	int ty = y;

	if((opt & RXY) == RXY) {
		tx -= pSpr->spr[sprnum].rx;
		ty -= pSpr->spr[sprnum].ry;
	}

	if(pSpr->spr[sprnum].compress) 
		return _PointHitTestCsp(pSpr->spr[sprnum].xl, pSpr->spr[sprnum].yl, pSpr->spr[sprnum].data, tx, ty);
	else 
		return _PointHitTestSpr(pSpr->spr[sprnum].xl, pSpr->spr[sprnum].yl, pSpr->spr[sprnum].data, pSpr->spr[sprnum].colorkey, tx, ty);
}


//////////////////////////////////////////////////


CPage::CPage()
{
	lpBmpInfo = NULL;
	lpBit     = NULL;
	lPitch    = 0;
	lPitchHalf= 0;
	nBitNum   = 8;
	nBPD      = 1;
	nColorNum = 0;
	lpLogPal  = NULL;
	hPal      = NULL;
	hBmp      = NULL;

	nAllocSize = 0;

	////////////

	bCreateAlphaTable8 = FALSE;

	////////////

	TClipX1 = 0;
	TClipY1 = 0;
	TClipX2 = 0;
	TClipY2 = 0;

	MirrorX = 0;

	////////////

	bAttached = FALSE;
	
	////////////

	xpFlip = 0;
	ypFlip = 0;
	iZoomX = 1;
}


CPage::~CPage()
{
	Destroy();
}

void CPage::Destroy()
{
	if(bAttached) Detach();

	nColorNum = 0;
	nAllocSize = 0;

	if(lpBmpInfo != NULL){ delete lpBmpInfo; lpBmpInfo=NULL;}
	//if(lpBit != NULL) { delete lpBit; lpBit=NULL; }
	if(lpLogPal) { free( lpLogPal );  lpLogPal=NULL; }
	if(hPal) { DeleteObject(hPal); hPal=NULL;}
	if(hBmp) { DeleteObject(hBmp); hBmp=NULL;}

	if(bCreateAlphaTable8) ClearAlphaTable8();
	bCreateAlphaTable8 = FALSE;
}

// �ӽ�Ŭ���� ������ �����Ѵ�
void CPage::SetTempClip(int x1, int y1, int x2, int y2)
{
	TClipX1 = x1;
	TClipY1 = y1;
	TClipX2 = x2;
	TClipY2 = y2;
}

// �̷���� ���� ��ǥ�� �����Ѵ�
void CPage::SetMirrorX(int x)
{
	MirrorX = x;
}

/*-----------------------------------------------------------------------------------------
   �������� �ʱ�ȭ �Ѵ�
------------------------------------------------------------------------------------------*/
BOOL CPage::Init(LONG xl, LONG yl, INT bit, RGBQUAD *lpPal, LPCTSTR palfname)
{
	if(bAttached) Detach();

	//if(lpBit != NULL) { delete lpBit; lpBit=NULL; }
	if(lpBmpInfo != NULL){ delete lpBmpInfo; lpBmpInfo=NULL;}
	if(hBmp) { DeleteObject(hBmp); hBmp=NULL;}
	
	switch(bit)
	{
	case 4  : nBPD=1; nColorNum=16;  break;
	case 8  : nBPD=1; nColorNum=256; break;
	case 16 : nBPD=2; nColorNum=0;   break;
	case 24 : nBPD=3; nColorNum=0;   break;
	default : return FALSE;
	}

	nBitNum = bit;
	width = xl;
	lPitch = xl*nBPD;
	height = yl;
	while(lPitch%4) lPitch++; // Pitch�� 4�� ����� �ǰ� �Ѵ�
	//lPitch/=2;                // 16��Ʈ Į�󿩼� ����� ���.
	                          // ���⼭�� lPitch�� ����Ʈ���̴�(���� ������ �ƴϴ�)
	lPitchHalf = lPitch/nBPD; // lPitch�� ����Ʈ���̹Ƿ� 8,16,23��Ʈ ����� ���� ������ ��´�.

	// ��Ʈ�� ���� ����ü�� ���� �޸� �Ҵ�
	nAllocSize = sizeof(BITMAPINFO) + sizeof(RGBQUAD) * nColorNum;
	//if(nColorNum != 0) nAllocSize -= sizeof(RGBQUAD);
	nAllocSize -= sizeof(RGBQUAD);
	lpBmpInfo = (LPBITMAPINFO)new char[nAllocSize + sizeof(RGBQUAD)];
	if(lpBmpInfo == NULL) return FALSE;

	// ���� ������ ����(��Ʈ�� �̹��� ������)
	//lpBit = (WORD*)new char[lPitch * yl];
	//if(lpBit == NULL) return FALSE;

	// ��Ʈ�� ���� �ش� ����
	lpBmpInfo->bmiHeader.biSize	         = sizeof (BITMAPINFOHEADER);
	lpBmpInfo->bmiHeader.biWidth         = width;      //width;
	lpBmpInfo->bmiHeader.biHeight        = -height;    // ������ �׸��� ������ ��µȴ�
	lpBmpInfo->bmiHeader.biPlanes        = 1;          // �׻�1
	lpBmpInfo->bmiHeader.biBitCount	     = nBitNum;    // ��Ʈ�� ��Ʈ��(16��Ʈ�� 5:6:5)
	lpBmpInfo->bmiHeader.biCompression	 = 0;          // BI_RGB�� ����(�����)
	lpBmpInfo->bmiHeader.biSizeImage     = lPitch*yl;  // lPitch; // �׻� 4�ǹ��
	lpBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	lpBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	lpBmpInfo->bmiHeader.biClrUsed	     = 0;          // ���� �� Į���(�׻� 0���� ����)
	lpBmpInfo->bmiHeader.biClrImportant	 = 0;          // �߿��� �����(�׻� 0���� ����)

	// �ķ�Ʈ ����
	if( nColorNum != 0) {
		if(palfname) OpenPal(palfname);
		else SetPal(lpPal);
	}

	// DIB�� ����
	ReCreateDIBSection();

	// �׷��� ���� �ʱ�ȭ
	InitGrpLib(xl, yl, lPitchHalf, 555);
	InitAlphaTable(555);

	return TRUE;
}

BOOL CPage::Attach(LONG xl, LONG yl, void *lpbit, LONG lpitch)
{
	if(lpBmpInfo) return FALSE;
	if(hBmp) return FALSE;
	if(bAttached) return FALSE;
	
	nBPD=2; nColorNum=0;
	nBitNum = 16;
	width = xl;
	lPitch = lpitch;
	height = yl;
	//while(lPitch%4) lPitch++; // Pitch�� 4�� ����� �ǰ� �Ѵ�
	lPitchHalf = lPitch/nBPD; // lPitch�� ����Ʈ���̹Ƿ� 8,16,23��Ʈ ����� ���� ������ ��´�.
	lpBmpInfo = NULL;
	lpBit = (WORD*)lpbit;

	InitGrpLib(xl, yl, lPitch/2, 555);

	bAttached = TRUE;
	return TRUE;
}

BOOL CPage::Detach()
{
	if(!bAttached) return FALSE;

	lpBmpInfo = NULL;
	hBmp = NULL;
	lpBit = NULL;
	width = height = 0;
	lPitch = lPitchHalf = 0;
	bAttached = FALSE;

	return TRUE;
}

BOOL CPage::ReCreateDIBSection()
{
	if(hBmp) DeleteObject(hBmp);

	// DIB�� ����
	HWND ActiveWindow = GetActiveWindow();
	HDC ScreenDC = GetDC(ActiveWindow);
	hBmp = CreateDIBSection(ScreenDC, lpBmpInfo, DIB_RGB_COLORS, (VOID**)&lpBit, NULL, 0);
	ReleaseDC(ActiveWindow, ScreenDC);

	return (hBmp != NULL);
}

BOOL CPage::OpenPal(LPCTSTR filename)
{
	if(nColorNum<=0) return FALSE;

	FILE *fp;
	RGBQUAD rgbpal[256];
	if( (fp = fopen( filename, "rb")) == NULL ) return FALSE;

	fread(rgbpal, nColorNum*sizeof(RGBQUAD), 1, fp);
	fclose(fp);

	return SetPal(rgbpal);
}

BOOL CPage::OpenAlphaTable8(LPCTSTR filename)
{
	if(LoadAlphaTable8((char*)filename)==TRUE) { 
		bCreateAlphaTable8 = TRUE; 
		return TRUE; 
	}
	return FALSE;
}

/*-----------------------------------------------------------------------------------------
   �������� ��Ʈ�� ���Ϸ� �����Ѵ�
------------------------------------------------------------------------------------------*/
BOOL CPage::SaveToBitmap(LPSTR fname)
{
	if(!fname) return FALSE;
	if(!lpBit || !lpBmpInfo) return FALSE;
	if(nAllocSize <= 0) return FALSE;
	if(nBitNum != 16) return FALSE;

	FILE *fp = fopen(fname, "wb");
    if(!fp) return FALSE;

	int npitch = width * 3;
	while(npitch%4) npitch++;
	int allocsize = npitch * height;
	BYTE *pimage = new BYTE[allocsize];
	memset(pimage, 0, allocsize);
	BYTE *bit = pimage;

	// 16��Ʈ �̹����� 24��Ʈ�� ��ȯ(�̹����� ������)
	for(int i=height-1; i>=0; i--) {
		for(int j=0; j<width; j++) {
			WORD color = *((WORD*)lpBit + (i * lPitchHalf + j));
			// ����) BGR���̴�
			*(bit+(j*3)) = ((BYTE)B555(color)) << 3;	// B
			*(bit+(j*3)+1) = ((BYTE)G555(color)) << 3;	// G
			*(bit+(j*3)+2) = ((BYTE)R555(color)) << 3;	// R
		}
		bit += npitch;
	}
     
	BITMAPFILEHEADER	bmfHeader = {0,};
	BITMAPINFOHEADER    bmiHeader = {0,};
	memcpy(&bmiHeader, &lpBmpInfo->bmiHeader, sizeof(BITMAPINFOHEADER));

	bmiHeader.biBitCount = 24;
	bmiHeader.biHeight = height;

	bmfHeader.bfType = ((WORD)('M'<<8 | 'B'));	// bmp���� ǥ����;
	bmfHeader.bfSize = 14 + sizeof(BITMAPINFOHEADER) + allocsize;
	bmfHeader.bfOffBits = 14 + sizeof(BITMAPINFOHEADER);
	fwrite(&bmfHeader, 14, 1, fp);
	fwrite(&bmiHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(pimage, allocsize, 1, fp);

	fclose(fp);

	delete pimage;

	return TRUE;
}

/*-----------------------------------------------------------------------------------------
   �������� JPG ���Ϸ� �����Ѵ�(quality = 0-100)
------------------------------------------------------------------------------------------*/
BOOL CPage::SaveToJpeg(LPSTR fname, int quality)
{
	if(!fname) return FALSE;
	if(!lpBit || !lpBmpInfo) return FALSE;
	if(nAllocSize <= 0) return FALSE;
	if(nBitNum != 16) return FALSE;

	int npitch = width * 3;
	//while(npitch%4) npitch++;	// 4����Ʈ �����ڵ�
	int allocsize = npitch * height;
	BYTE *pimage = new BYTE[allocsize];
	memset(pimage, 0, allocsize);

	BYTE *bit = pimage;

	// 16��Ʈ �̹����� 24��Ʈ�� ��ȯ(�̹����� ������ �ʾƾ��Ѵ�)
	for(int i=0; i<height; i++) {
		for(int j=0; j<width; j++) {
			WORD color = *((WORD*)lpBit + (i * lPitchHalf + j));
			// RGB
			*(bit+(j*3)) = ((BYTE)R555(color)) << 3;	// R
			*(bit+(j*3)+1) = ((BYTE)G555(color)) << 3;	// G
			*(bit+(j*3)+2) = ((BYTE)B555(color)) << 3;	// B
		}
		bit += npitch;
	}

	BOOL bSuccess = JpegFile::RGBToJpegFile(fname,		// path
											pimage,		// RGB buffer
											width,		// pixels
											height,		// rows
											TRUE,		// TRUE = RGB, FALSE = Grayscale
											quality);	// 0 - 100

	delete pimage;

	return bSuccess;
}

/*-----------------------------------------------------------------------------------------
   �������� �ķ�Ʈ�� �����Ѵ�

   - �μ� : RGBQUAD�� ������
------------------------------------------------------------------------------------------*/
BOOL CPage::SetPal(RGBQUAD *lpPal)
{
	if( nColorNum == 0)	return TRUE;        //�ķ�Ʈ�� �ʿ���ٸ�

	if(lpLogPal) { free( lpLogPal );  lpLogPal=NULL; } // �޸� ����
	lpLogPal = (LPLOGPALETTE)malloc( sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * nColorNum );

	lpLogPal->palVersion = 0x300;	        // �ȷ�Ʈ ���� �Է�
	lpLogPal->palNumEntries = nColorNum;    // �ȷ�Ʈ ����� �Է�
	
	int i;
	if(lpPal==NULL){             // �ķ�Ʈ �����Ͱ� NULL�̸� �⺻ �ķ�Ʈ�� �����Ѵ�
	for(i=0; i<nColorNum; i++)
	{
		lpLogPal->palPalEntry[i].peRed   = lpBmpInfo->bmiColors[i].rgbRed      = (BYTE)(((i>>5) & 0x07) * 255/7);
		lpLogPal->palPalEntry[i].peGreen = lpBmpInfo->bmiColors[i].rgbGreen    = (BYTE)(((i>>2) & 0x07) * 255/7);
		lpLogPal->palPalEntry[i].peBlue  = lpBmpInfo->bmiColors[i].rgbBlue     = (BYTE)(((i>>0) & 0x03) * 255/3);
		lpLogPal->palPalEntry[i].peFlags = lpBmpInfo->bmiColors[i].rgbReserved = (BYTE)0;
	}}
	else{
	for(i=0; i<nColorNum; i++)
	{
		lpLogPal->palPalEntry[i].peRed   = lpBmpInfo->bmiColors[i].rgbRed   = lpPal[i].rgbRed;
		lpLogPal->palPalEntry[i].peGreen = lpBmpInfo->bmiColors[i].rgbGreen = lpPal[i].rgbGreen;
		lpLogPal->palPalEntry[i].peBlue  = lpBmpInfo->bmiColors[i].rgbBlue  = lpPal[i].rgbBlue;
		lpLogPal->palPalEntry[i].peFlags = lpBmpInfo->bmiColors[i].rgbReserved = 0;
	}}

	if(hPal) { DeleteObject(hPal); hPal=NULL;}
	hPal = CreatePalette( lpLogPal );		
	
	if(hPal == NULL) return FALSE;
	return TRUE;
}

BOOL CPage::SetLogPal(LPLOGPALETTE lpPal)
{
	if( nColorNum == 0)	//�ķ�Ʈ�� �ʿ���ٸ�
		return TRUE;
	
	if(lpLogPal!=NULL) { free( lpLogPal );  lpLogPal=NULL; } // �޸� ����

	LONG size = sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * nColorNum;
	lpLogPal = (LPLOGPALETTE)malloc( size );

	memcpy(lpLogPal, lpPal, size);

	if(hPal) { DeleteObject(hPal); hPal=NULL;}
	hPal = CreatePalette( lpLogPal );		
	
	if(hPal == NULL) return FALSE;
	return TRUE;
}


BOOL CPage::SetPalHandle(HPALETTE h)
{
	if(h==NULL) return FALSE;
	if(hPal) { DeleteObject(hPal); hPal=NULL;}
	hPal = h;
	return TRUE;
}

/*-----------------------------------------------------------------------------------------
   GDI ǥ���� Ư�� Į��� ä���
------------------------------------------------------------------------------------------*/
BOOL CPage::FillGDI(HDC hTarDC, int nXStart, int nYStart, COLORREF crColor)
{
	BOOL check;
	check = FloodFill(hTarDC, nXStart, nYStart, crColor);
	if(check != 0)return TRUE;
	else return FALSE;
}

/*-----------------------------------------------------------------------------------------
   �ٸ� �������� ������ ���� ������ ǥ�鿡 �Ѹ���
------------------------------------------------------------------------------------------*/
BOOL CPage::PutFromPage(LONG xp, LONG yp, CPage *pPage, int blendgrade)
{
	int w = pPage->width;
	int h = pPage->height;
	if(lpBit == NULL) return FALSE;
	if(pPage->lpBit == NULL) return FALSE;
	if(w==0 || h==0) return FALSE;
	
	InitGrpLib(width, height, lPitchHalf, 555);
	if(blendgrade >= 31) _PutImage(xp, yp, w, h, (WORD*)lpBit, (WORD*)pPage->lpBit);
	else _PutImage_GB(xp, yp, w, h, (WORD*)lpBit, (WORD*)pPage->lpBit, blendgrade);

	return TRUE;
}

/*-----------------------------------------------------------------------------------------
   �ٸ� �������� ������ �÷�Ű�� �����Ͽ� ���� ������ ǥ�鿡 �Ѹ���
------------------------------------------------------------------------------------------*/
BOOL CPage::PutFromPageWithColorKey(LONG xp, LONG yp, CPage *pPage, WORD colorkey, int blendgrade)
{
	int w = pPage->width;
	int h = pPage->height;
	if(lpBit == NULL) return FALSE;
	if(pPage->lpBit == NULL) return FALSE;
	if(w==0 || h==0) return FALSE;
	
	InitGrpLib(width, height, lPitchHalf, 555);
	if(blendgrade >= 31) _PutSpr(xp, yp, w, h, (WORD*)lpBit, (WORD*)pPage->lpBit, colorkey);
	else _PutSpr_GB(xp, yp, w, h, (WORD*)lpBit, (WORD*)pPage->lpBit, colorkey, blendgrade);

	return TRUE;
}

/*-----------------------------------------------------------------------------------------
   �������� �ѷ��� Ŭ���̾�Ʈ�� Ư�� ��ǥ�� �������� ��ǥ�� ��ȯ��Ų��
------------------------------------------------------------------------------------------*/
void CPage::ToPagePos(LONG xp, LONG yp, LONG* X, LONG* Y)
{
	*X = (xp-xpFlip)/iZoomX;
	*Y = (yp-ypFlip)/iZoomX;
}

/*-----------------------------------------------------------------------------------------
    �������� �ø��� GDIǥ�� ��ġ(Ŭ���̾�Ʈ ����)�� �����Ѵ�
------------------------------------------------------------------------------------------*/
void CPage::SetFlipPos(LONG xp, LONG yp, int zoom)
{
	xpFlip = xp;
	ypFlip = yp;
	iZoomX = zoom;
}

/*-----------------------------------------------------------------------------------------
   �������� ������ GDI ǥ�鿡 ������� �Ѹ���(SetPagePos()�� ��ġ�� �ٲܼ� �ִ�)
------------------------------------------------------------------------------------------*/
BOOL CPage::Flip(HWND hwnd)
{
	// �������� �ʱ�ȭ ���� �ʾҴٸ�
	if(  lpBit == NULL ) return FALSE;
	HDC hDC = GetDC(hwnd);
	BOOL rtn = Flip(hDC);
	ReleaseDC(hwnd, hDC);
	return rtn;
}

BOOL CPage::Flip(HDC hdc)
{
	int rtn;

	// �������� �ʱ�ȭ ���� �ʾҴٸ�
	if(  lpBit == NULL ) return FALSE;

	HDC hDC = hdc;

	if( nColorNum != 0 && hPal != NULL)	//�ķ�Ʈ�� �ʿ��ϴٸ�
	{
		HPALETTE hOldPal;
		hOldPal = SelectPalette( hDC, hPal, TRUE ); //�ȷ�Ʈ ����
		if(hOldPal == NULL) return FALSE;
		rtn = RealizePalette( hDC );				 //�ȷ�Ʈ Ȱ��ȭ
		if(rtn==GDI_ERROR) return FALSE;             // GDI_ERROR = -1
	}

	int xl = width * iZoomX;
	int yl = height * iZoomX;
	
	rtn = StretchDIBits( hDC,
				   xpFlip,
				   ypFlip,
				   xl,
				   yl,
				   0,
				   0,
				   width,
				   height,
				   lpBit,
				   lpBmpInfo,
				   DIB_RGB_COLORS,
				   SRCCOPY );

	if(rtn == GDI_ERROR) return FALSE;
	return TRUE;
}

/*-----------------------------------------------------------------------------------------
   �������� ������ GDI ǥ�鿡 ������ Ȯ��� �Ѹ���(SetFlipPos()�� �̸�����)
------------------------------------------------------------------------------------------*/
BOOL CPage::FlipToGDI( HDC hdc, LONG xp, LONG yp, int zoom)
{
	SetFlipPos(xp, yp, zoom);
	return Flip(hdc);
}

BOOL CPage::FlipToGDI( HWND hwnd, LONG xp, LONG yp, int zoom)
{
	SetFlipPos(xp, yp, zoom);
	return Flip(hwnd);
}

/*-----------------------------------------------------------------------------------------
   �������� ������ GDI ǥ�鿡 Stretch�� �Ѹ���
   
   - �ҽ��� Ÿ���� RECT�� NULL�̸� ��ü �������� �����ȴ�
------------------------------------------------------------------------------------------*/
BOOL CPage::StretchFlip(HWND hwnd, RECT* rcDest, RECT* rcSrc )
{
	int rtn;

	// �������� �ʱ�ȭ ���� �ʾҴٸ�
	if(  lpBit == NULL ) return FALSE;

	HDC hDC = GetDC(hwnd);

	if( nColorNum != 0 && hPal != NULL)	//�ķ�Ʈ�� �ʿ��ϴٸ�
	{
		HPALETTE hOldPal;
		hOldPal = SelectPalette( hDC, hPal, TRUE ); //�ȷ�Ʈ ����
		if(hOldPal == NULL) return FALSE;
		rtn = RealizePalette( hDC );				 //�ȷ�Ʈ Ȱ��ȭ
		if(rtn==GDI_ERROR) return FALSE;             // GDI_ERROR = -1
	}

	//int xl = width * iZoomX;
	//int yl = height * iZoomX;
	
	int sx, sy, sxl, syl;
	int dx, dy, dxl, dyl;
	
	if(rcDest==NULL) 
	{ 
		RECT rect;
		GetClientRect(hwnd, &rect);
		dx=0; dy=0; dxl=rect.right-rect.left+1; dyl=rect.bottom-rect.top+1;
	}
	else
	{
		dx = rcDest->left; dy = rcDest->top;
		dxl = rcDest->right - rcDest->left + 1; dyl = rcDest->bottom - rcDest->top + 1;
	}

	if(rcSrc==NULL) 
	{ 
		sx=0; sy=0; sxl=width; syl=height;
	}
	else
	{
		sx = rcSrc->left; sy = rcSrc->top;
		sxl = rcSrc->right - rcSrc->left + 1; syl = rcSrc->bottom - rcSrc->top + 1;
	}

	sy = height-sy-syl; // �׸��� ������ �����Ƿ� ��ǥ�� �����´�

	rtn = StretchDIBits( hDC,
				   dx, dy, dxl, dyl,
				   sx, sy, sxl, syl,
				   lpBit,
				   lpBmpInfo,
				   DIB_RGB_COLORS,
				   SRCCOPY );

	ReleaseDC(hwnd, hDC);

	if(rtn == GDI_ERROR) return FALSE;
	return TRUE;
}


/////////////////////
//  ����� ����
/////////////////////

/*-----------------------------------------------------------------------------------------
   �������� ������ �׸���(flag�� TRUE�̸� XOR�� �׸���)
------------------------------------------------------------------------------------------*/
void CPage::HLine(int x, int y, int len, WORD color, BOOL flag)
{
	if(lpBit==NULL) return;
	InitGrpLib(width, height, lPitchHalf, 555);

	if(nBitNum == 16) {
		if(flag==FALSE) DrawHLine( x, y, len, (WORD*)lpBit, color );    // �ܻ� Į�� �����
		else if(flag==TRUE) DrawHLineXOR( x, y, len, (WORD*)lpBit);           // XOR �����
	}
	else if(nBitNum == 8) {
		DrawHLine8(x, y, len, (char*)lpBit, (char)color);
	}
}

/*-----------------------------------------------------------------------------------------
   �������� �������� �׸���(flag�� TRUE�̸� XOR�� �׸���)
------------------------------------------------------------------------------------------*/
void CPage::VLine(int x, int y, int len, WORD color, BOOL flag)
{
	if(lpBit==NULL) return;
	InitGrpLib(width, height, lPitchHalf, 555);

	if(nBitNum == 16) {
		if(flag==FALSE) DrawVLine( x, y, len, (WORD*)lpBit, color );    // �ܻ� Į�� �����
		else if(flag==TRUE) DrawVLineXOR( x, y, len, (WORD*)lpBit);           // XOR �����
	}
	else if(nBitNum == 8) {
		DrawVLine8(x, y, len, (char*)lpBit, (char)color);
	}
}

/*-----------------------------------------------------------------------------------------
   �������� ���� �׸���(�극���� �˰���)
------------------------------------------------------------------------------------------*/
void CPage::DrawLine(int x1, int y1, int x2, int y2, WORD color)
{
	if(lpBit==NULL) return;
	InitGrpLib(width, height, lPitchHalf, 555);

	if(nBitNum == 16) {
		DrawBresLine( x1, y1, x2, y2, (WORD*)lpBit, color );    // �� �׸���(�극���� �˰���)
	}
}

/*-----------------------------------------------------------------------------------------
   �������� ���� �� �ڽ��� �׸���(flag�� TRUE�̸� XOR�� �׸���)
------------------------------------------------------------------------------------------*/
void CPage::DrawEmptyBox(int x1, int y1, int x2, int y2, WORD color, BOOL flag)
{
	if(lpBit==NULL) return;
	InitGrpLib(width, height, lPitchHalf, 555);

	if(x1>x2) swap(x1,x2);
	if(y1>y2) swap(y1,y2);

	if(nBitNum == 16) {
		if(flag==FALSE) DrawBox( x1, y1, x2, y2, (WORD*)lpBit, color );    // �ܻ� Į�� �����
		else if(flag==TRUE) DrawBoxXOR( x1, y1, x2, y2, (WORD*)lpBit);           // XOR �����
	}
	else if(nBitNum == 8) {
		DrawBox8(x1, y1, x2, y2, (char*)lpBit, (char)color);
	}
}

/*-----------------------------------------------------------------------------------------
   �������� ���� �� �ڽ��� �׸���
------------------------------------------------------------------------------------------*/
void CPage::DrawFillBox(int x1, int y1, int x2, int y2, WORD color)
{
	if(lpBit==NULL) return;
	InitGrpLib(width, height, lPitchHalf, 555);

	if(x1>x2) swap(x1,x2);
	if(y1>y2) swap(y1,y2);

	if(nBitNum == 16) {
		FillBox( x1, y1, x2, y2, (WORD*)lpBit, color );
	}
	else if(nBitNum == 8) {
		FillBox8( x1, y1, x2, y2, (char*)lpBit, (char)color );
	}
}

/*-----------------------------------------------------------------------------------------
   �������� ���� �� ������ �ڽ��� �׸���
------------------------------------------------------------------------------------------*/
void CPage::DrawFillBoxGB(int x1, int y1, int x2, int y2, WORD color, int grade)
{
	if(lpBit==NULL) return;
	InitGrpLib(width, height, lPitchHalf, 555);

	if(x1>x2) swap(x1,x2);
	if(y1>y2) swap(y1,y2);

	if(nBitNum == 16) {
		FillBoxGB( x1, y1, x2, y2, (WORD*)lpBit, color, grade);
	}
	else if(nBitNum == 8) {
		FillBoxGlass8(x1, y1, x2, y2, (char*)lpBit, (char)color, grade);
	}
}

/*-----------------------------------------------------------------------------------------
   ���� �ڽ��� �׸���
------------------------------------------------------------------------------------------*/
void CPage::DrawSelectBox(int x1, int y1, int x2, int y2, WORD color, BOOL flag)
{
	if(lpBit==NULL) return;
	InitGrpLib(width, height, lPitchHalf, 555);

	if(x1>x2) swap(x1,x2);
	if(y1>y2) swap(y1,y2);

	if(nBitNum == 16) {
		if(flag) ::DrawXORSelectBox(x1, y1, x2, y2, (WORD*)lpBit);
		else ::DrawSelectBox(x1, y1, x2, y2, (WORD*)lpBit, color);
	}
	else if(nBitNum == 8) {
	}
}




/////////////////////
//  ��������Ʈ ����
/////////////////////

/*--------------------------------------------------------------------------------------
   �ڵ� ��������Ʈ ��� �Լ�(����/����� �ڵ�����, ������/�ø�/�ܻ���� ����)
----------------------------------------------------------------------------------------*/
BOOL CPage::PutSprAuto(int x, int y, xSprite *pXSpr, int sprnum, int opt, int grade, WORD color)
{
	if(pXSpr==NULL) return FALSE;
	if(pXSpr->spr==NULL) return FALSE;
	if(sprnum<0 || sprnum >= pXSpr->sfhead.TotalSprNum) return FALSE;
	PutSprAuto(x, y, &pXSpr->spr[sprnum], opt, grade, color);
	return TRUE;
}

/*--------------------------------------------------------------------------------------
   �ڵ� ��������Ʈ ��� �Լ�(����/����� �ڵ�����, ������/�ø�/�ܻ���� ����)
----------------------------------------------------------------------------------------*/
BOOL CPage::PutSprAuto(int x, int y, SPRITE30 *pSpr, int opt, int grade, WORD color)
{
	if(pSpr==NULL) return FALSE;
	if(lpBit==NULL) return FALSE;
	if(nBitNum != 16) return FALSE;

	if((int)width != _MXL || (int)height != _MYL || (DWORD)lPitchHalf != _LPITCH) 
		InitGrpLib(width, height, lPitchHalf, 555);
	
	if(opt & TEMPCLIP) {
		opt = (~(opt&TEMPCLIP))&opt;
		SetClipArea(TClipX1, TClipY1, TClipX2, TClipY2);
	}
	else {
		SetClipArea(0, 0, width-1, height-1);
	}

	// ������ ����̸�(�����ǥ ����)
	if(opt & MIRROR)
	{
		opt = (~(opt&MIRROR))&opt;
		opt |= LR;
		if(opt & RXY)
		{
			x = MirrorX + (MirrorX - (x + pSpr->rx + pSpr->xl));
			opt = (~(opt&RXY))&opt;
		}
		else x = MirrorX + (MirrorX - (x + pSpr->xl));
		y+=pSpr->ry;
	}
	else if(opt & RXY) { x+=pSpr->rx; y+=pSpr->ry; opt = (~(opt&RXY))&opt; }

	if(pSpr->compress)
	{
		if(opt&LR)
		{
			opt = (~(opt&LR))&opt;
			if(opt==NULL) _PutCspLR(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
			else if(opt&HB) _PutCspLR_HB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
			else if(opt&GB) _PutCspLR_GB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, grade);
			else if(opt&LE) _PutCspLR_LE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
			else if(opt&DE) _PutCspLR_DE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
			else if(opt&GLOW) _PutCspLR_Glow(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, color, grade);
			else if(opt&GRAYSCL) _PutCspLR_Grayscl(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
			else if(opt&MONO) _PutCspLR_Mono(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, color);
			else _PutCspLR(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
		}
		else
		{
			if(opt==NULL) _PutCsp(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
			else if(opt&ANTI) _PutCsp_Anti(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
			else if(opt&HB) _PutCsp_HB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
			else if(opt&GB) _PutCsp_GB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, grade);
			else if(opt&LE) _PutCsp_LE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
			else if(opt&DE) _PutCsp_DE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
			else if(opt&GLOW) _PutCsp_Glow(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, color, grade);
			else if(opt&GRAYSCL) _PutCsp_Grayscl(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
			else if(opt&MONO) _PutCsp_Mono(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, color);
			else _PutCsp(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
		}
	}
	else
	{
		if(opt==NULL) _PutSpr(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->colorkey);
		else if(opt&GB) _PutSpr_GB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->colorkey, grade);
		else _PutSpr(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->colorkey);
	}

	return TRUE;
}

/*--------------------------------------------------------------------------------------
   Zoom ��������Ʈ ��� �Լ�(����/����� ��� ��밡��, �¿�/�����ø� ����)
----------------------------------------------------------------------------------------*/
BOOL CPage::ZoomSprAuto(int x, int y, xSprite *pXSpr, int sprnum, int tx, int ty, int opt, int grade, WORD color)
{
	if(pXSpr==NULL) return FALSE;
	if(pXSpr->spr==NULL) return FALSE;
	if(sprnum<0 || sprnum >= pXSpr->sfhead.TotalSprNum) return FALSE;
	ZoomSprAuto(x, y, &pXSpr->spr[sprnum], tx, ty, opt, grade, color);
	return TRUE;
}

/*--------------------------------------------------------------------------------------
   Zoom ��������Ʈ ��� �Լ�(����/����� ��� ��밡��, �¿�/�����ø� ����)
----------------------------------------------------------------------------------------*/
BOOL CPage::ZoomSprAuto(int x, int y, SPRITE30 *pSpr, int tx, int ty, int opt, int grade, WORD color)
{
	if(pSpr==NULL) return FALSE;
	if(lpBit==NULL) return FALSE;
	if(nBitNum != 16) return FALSE;

	if((int)width != _MXL || (int)height != _MYL || (DWORD)lPitchHalf != _LPITCH) 
		InitGrpLib(width, height, lPitchHalf, 555);

	if(opt & TEMPCLIP) {
		opt = (~(opt&TEMPCLIP))&opt;
		SetClipArea(TClipX1, TClipY1, TClipX2, TClipY2);
	}
	else {
		SetClipArea(0, 0, width-1, height-1);
	}

	if(opt & PERCENT) { tx = pSpr->xl*tx/100; ty = pSpr->yl*ty/100; }	// 100% �������� Ȯ��,���
	if(opt & RXY) { x+=pSpr->rx*tx/pSpr->xl; y+=pSpr->ry*ty/pSpr->yl; }	// �����ǥ�� ����
	if(opt & CENTER) { x -= (tx>>1); y -= (ty>>1); }					// ĳ���͸� ��ǥ�� �߽ɿ� ��ġ��Ų��
	
	// 1x Zoom
	if(tx == pSpr->xl && ty == pSpr->yl) { PutSprAuto(x, y, pSpr, opt, grade, color); return TRUE; }
	
	if(pSpr->compress)
	{
		//if(opt==NULL) _ZoomCsp(x, y, pSpr->xl, pSpr->yl, lpBit, tx, ty, pSpr->data);
		_ZoomCsp(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data);
	}
	else
	{
		//if(opt==NULL) _ZoomSpr(x, y, pSpr->xl, pSpr->yl, lpBit, tx, ty, pSpr->data, pSpr->colorkey);
		//_PutSpr(x, y, pSpr->xl, pSpr->yl, lpBit, pSpr->data, pSpr->colorkey);
	}
	return TRUE;
}

/*--------------------------------------------------------------------------------------
   �ڵ� �̹��� ��� �Լ�(����/����� �ڵ�����, ������/�ø�/�ܻ���� ����)
----------------------------------------------------------------------------------------*/
BOOL CPage::PutImageAuto(int x, int y, xSprite *pXSpr, int sprnum, int opt, int grade, WORD color)
{
	if(pXSpr==NULL) return FALSE;
	if(pXSpr->spr==NULL) return FALSE;
	if(sprnum<0 || sprnum >= pXSpr->sfhead.TotalSprNum) return FALSE;
	PutImageAuto(x, y, &pXSpr->spr[sprnum], opt, grade, color);
	return TRUE;
}

/*--------------------------------------------------------------------------------------
      �ڵ� �̹��� ��� �Լ�(����/����� �ڵ�����, ������/�ø�/�ܻ���� ����)
----------------------------------------------------------------------------------------*/
BOOL CPage::PutImageAuto(int x, int y, SPRITE30 *pSpr, int opt, int grade, WORD color)
{
	if(pSpr==NULL) return FALSE;
	if(lpBit==NULL) return FALSE;
	if(nBitNum != 16) return FALSE;

	if((int)width != _MXL || (int)height != _MYL || (DWORD)lPitchHalf != _LPITCH) 
		InitGrpLib(width, height, lPitchHalf, 555);

	if(opt & TEMPCLIP) {
		opt = (~(opt&TEMPCLIP))&opt;
		SetClipArea(TClipX1, TClipY1, TClipX2, TClipY2);
	}
	else {
		SetClipArea(0, 0, width-1, height-1);
	}

	if(opt & RXY) { x+=pSpr->rx; y+=pSpr->ry; opt = (~(opt&RXY))&opt; }

	if(pSpr->compress)
	{
		//if(opt==NULL) _PutCsp(x, y, pSpr->xl, pSpr->yl, lpBit, pSpr->data);
	}
	else
	{
		_PutImage(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
	}

	
	return TRUE;
}



////////////////////// ���⼭���� 8��Ʈ �̹��� ��� �Լ� ///////////////////




/*--------------------------------------------------------------------------------------
   �ڵ� ��������Ʈ ��� �Լ�(����/����� �ڵ�����, ������/�ø�/�ܻ���� ����)
----------------------------------------------------------------------------------------*/
BOOL CPage::PutSprAuto(int x, int y, CSprite8 *pSpr8, int sprnum, int opt, int grade, BYTE color)
{
	if(pSpr8==NULL) return FALSE;
	if(pSpr8->spr==NULL) return FALSE;
	if(sprnum<0 || sprnum >= pSpr8->sfhead.TotalSprNum) return FALSE;
	PutSprAuto(x, y, &pSpr8->spr[sprnum], opt, grade, color);
	return TRUE;
}

/*--------------------------------------------------------------------------------------
   �ڵ� ��������Ʈ ��� �Լ�(����/����� �ڵ�����, ������/�ø�/�ܻ���� ����)
----------------------------------------------------------------------------------------*/
BOOL CPage::PutSprAuto(int x, int y, SPRITE8 *pSpr, int opt, int grade, BYTE color)
{
	if(pSpr==NULL) return FALSE;
	if(lpBit==NULL) return FALSE;
	if(nBitNum != 8) return FALSE;

	if((int)width != _MXL || (int)height != _MYL || (DWORD)lPitchHalf != _LPITCH) 
		InitGrpLib(width, height, lPitchHalf, 555);
	
	if(opt & TEMPCLIP) {
		opt = (~(opt&TEMPCLIP))&opt;
		SetClipArea(TClipX1, TClipY1, TClipX2, TClipY2);
	}
	else {
		SetClipArea(0, 0, width-1, height-1);
	}

	// ������ ����̸�(�����ǥ ����)
	if(opt & MIRROR)
	{
		opt = (~(opt&MIRROR))&opt;
		opt |= LR;
		if(opt & RXY)
		{
			x = MirrorX + (MirrorX - (x + pSpr->rx + pSpr->xl));
			opt = (~(opt&RXY))&opt;
		}
		else x = MirrorX + (MirrorX - (x + pSpr->xl));
		y+=pSpr->ry;
	}
	else if(opt & RXY) { x+=pSpr->rx; y+=pSpr->ry; opt = (~(opt&RXY))&opt; }


	if(!(opt&GB) && !(opt&GLOW))  // �Ϲ�����ΰ�?
	{
		if(pSpr->compress) // �����ΰ�?
		{
			if(!(opt&LR) && !(opt&UD)) PutCsp8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data);
			else if((opt&LR) && !(opt&UD)) PutCspLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data);
			else if(!(opt&LR) && (opt&UD)) PutCspUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data);
			else PutCspUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data);
		}
		else
		{
			if(!(opt&LR) && !(opt&UD)) PutSpr8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data);
			else if((opt&LR) && !(opt&UD)) PutSprLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data);
			else if(!(opt&LR) && (opt&UD)) PutSprUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data);
			else PutSprUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data);
		}
	}
	else if(!(opt&GB) && (opt&GLOW))  // �Ϲ� �ܻ� ����ΰ�?
	{
		if(pSpr->compress) // �����ΰ�?
		{
			if(!(opt&LR) && !(opt&UD)) PutCspMono8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color);
			else if((opt&LR) && !(opt&UD)) PutCspMonoLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color);
			else if(!(opt&LR) && (opt&UD)) PutCspMonoUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color);
			else PutCspMonoUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color);
		}
		else
		{
			if(!(opt&LR) && !(opt&UD)) PutSprMono8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color);
			else if((opt&LR) && !(opt&UD)) PutSprMonoLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color);
			else if(!(opt&LR) && (opt&UD)) PutSprMonoUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color);
			else PutSprMonoUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color);
		}
	}
	else if((opt&GB) && !(opt&GLOW)) // ����������ΰ�?
	{
		if(pSpr->compress) // �����ΰ�?
		{
			if(!(opt&LR) && !(opt&UD)) PutCspGlass8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, grade);
			else if((opt&LR) && !(opt&UD)) PutCspGlassLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, grade);
			else if(!(opt&LR) && (opt&UD)) PutCspGlassUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, grade);
			else PutCspGlassUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, grade);
		}
		else
		{
			if(!(opt&LR) && !(opt&UD)) PutSprGlass8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, grade);
			else if((opt&LR) && !(opt&UD)) PutSprGlassLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, grade);
			else if(!(opt&LR) && (opt&UD)) PutSprGlassUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, grade);
			else PutSprGlassUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, grade);
		}
	}
	else if((opt&GB) && (opt&GLOW)) // �ܻ� ����������ΰ�?
	{
		if(pSpr->compress) // �����ΰ�?
		{
			if(!(opt&LR) && !(opt&UD)) PutCspMonoGlass8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color, grade);
			else if((opt&LR) && !(opt&UD)) PutCspMonoGlassLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color, grade);
			else if(!(opt&LR) && (opt&UD)) PutCspMonoGlassUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color, grade);
			else PutCspMonoGlassUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color, grade);
		}
		else
		{
			if(!(opt&LR) && !(opt&UD)) PutSprMonoGlass8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color, grade);
			else if((opt&LR) && !(opt&UD)) PutSprMonoGlassLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color, grade);
			else if(!(opt&LR) && (opt&UD)) PutSprMonoGlassUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color, grade);
			else PutSprMonoGlassUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color, grade);
		}
	}

	return TRUE;
}

/*--------------------------------------------------------------------------------------
   Zoom ��������Ʈ ��� �Լ�(����/����� ��� ��밡��, �¿�/�����ø� ����)
----------------------------------------------------------------------------------------*/
BOOL CPage::ZoomSprAuto(int x, int y, CSprite8 *pSpr8, int sprnum, int tx, int ty, int opt, int grade, BYTE color)
{
	if(pSpr8==NULL) return FALSE;
	if(pSpr8->spr==NULL) return FALSE;
	if(sprnum<0 || sprnum >= pSpr8->sfhead.TotalSprNum) return FALSE;
	ZoomSprAuto(x, y, &pSpr8->spr[sprnum], tx, ty, opt, grade, color);
	return TRUE;
}

/*--------------------------------------------------------------------------------------
   Zoom ��������Ʈ ��� �Լ�(����/����� ��� ��밡��, �¿�/�����ø� ����)
----------------------------------------------------------------------------------------*/
BOOL CPage::ZoomSprAuto(int x, int y, SPRITE8 *pSpr, int tx, int ty, int opt, int grade, BYTE color)
{
	if(pSpr==NULL) return FALSE;
	if(lpBit==NULL) return FALSE;
	if(nBitNum != 8) return FALSE;

	if((int)width != _MXL || (int)height != _MYL || (DWORD)lPitchHalf != _LPITCH) 
		InitGrpLib(width, height, lPitchHalf, 555);

	if(opt & TEMPCLIP) {
		opt = (~(opt&TEMPCLIP))&opt;
		SetClipArea(TClipX1, TClipY1, TClipX2, TClipY2);
	}
	else {
		SetClipArea(0, 0, width-1, height-1);
	}

	if(opt & PERCENT) { tx = pSpr->xl*tx/100; ty = pSpr->yl*ty/100; }	// 100% �������� Ȯ��,���
	if(opt & RXY) { x+=pSpr->rx*tx/pSpr->xl; y+=pSpr->ry*ty/pSpr->yl; }	// �����ǥ�� ����
	if(opt & CENTER) { x -= (tx>>1); y -= (ty>>1); }					// ĳ���͸� ��ǥ�� �߽ɿ� ��ġ��Ų��
	
	// 1x Zoom
	if(tx == pSpr->xl && ty == pSpr->yl) { PutSprAuto(x, y, pSpr, opt, grade, color); return TRUE; }

	if(!(opt&GB) && !(opt&GLOW))  // �Ϲ�����ΰ�?
	{
		if(pSpr->compress) // �����ΰ�?
		{
			if(!(opt&LR) && !(opt&UD)) LowZoomCsp8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data);
			else if((opt&LR) && !(opt&UD)) LowZoomCspLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data);
			else if(!(opt&LR) && (opt&UD)) LowZoomCspUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data);
			else LowZoomCspUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data);
		}
		else
		{
			if(!(opt&LR) && !(opt&UD)) LowZoomSpr8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data);
			else if((opt&LR) && !(opt&UD)) LowZoomSprLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data);
			else if(!(opt&LR) && (opt&UD)) LowZoomSprUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data);
			else LowZoomSprUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data);
		}
	}
	else if(!(opt&GB) && (opt&GLOW))  // �Ϲ� �ܻ� ����ΰ�?
	{
		if(pSpr->compress) // �����ΰ�?
		{
			if(!(opt&LR) && !(opt&UD)) LowZoomCspMono8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color);
			else if((opt&LR) && !(opt&UD)) LowZoomCspMonoLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color);
			else if(!(opt&LR) && (opt&UD)) LowZoomCspMonoUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color);
			else LowZoomCspMonoUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color);
		}
		else
		{
			if(!(opt&LR) && !(opt&UD)) LowZoomSprMono8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color);
			else if((opt&LR) && !(opt&UD)) LowZoomSprMonoLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color);
			else if(!(opt&LR) && (opt&UD)) LowZoomSprMonoUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color);
			else LowZoomSprMonoUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color);
		}
	}
	else if((opt&GB) && !(opt&GLOW)) // ����������ΰ�?
	{
		if(pSpr->compress) // �����ΰ�?
		{
			if(!(opt&LR) && !(opt&UD)) LowZoomCspGlass8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, grade);
			else if((opt&LR) && !(opt&UD)) LowZoomCspGlassLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, grade);
			else if(!(opt&LR) && (opt&UD)) LowZoomCspGlassUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, grade);
			else LowZoomCspGlassUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, grade);
		}
		else
		{
			if(!(opt&LR) && !(opt&UD)) LowZoomSprGlass8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, grade);
			else if((opt&LR) && !(opt&UD)) LowZoomSprGlassLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, grade);
			else if(!(opt&LR) && (opt&UD)) LowZoomSprGlassUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, grade);
			else LowZoomSprGlassUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, grade);
		}
	}
	else if((opt&GB) && (opt&GLOW)) // �ܻ� ����������ΰ�?
	{
		if(pSpr->compress) // �����ΰ�?
		{
			if(!(opt&LR) && !(opt&UD)) LowZoomCspMonoGlass8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color, grade);
			else if((opt&LR) && !(opt&UD)) LowZoomCspMonoGlassLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color, grade);
			else if(!(opt&LR) && (opt&UD)) LowZoomCspMonoGlassUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color, grade);
			else LowZoomCspMonoGlassUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color, grade);
		}
		else
		{
			if(!(opt&LR) && !(opt&UD)) LowZoomSprMonoGlass8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color, grade);
			else if((opt&LR) && !(opt&UD)) LowZoomSprMonoGlassLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color, grade);
			else if(!(opt&LR) && (opt&UD)) LowZoomSprMonoGlassUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color, grade);
			else LowZoomSprMonoGlassUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color, grade);
		}
	}

	return TRUE;
}

