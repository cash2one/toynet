//
//                                   그래픽 처리 모듈
//
#include "StdAfx.h"
#include "Graphic.h"
#include "Jpegfile.h"

#pragma message("linking with GraphEngine library")
#pragma comment(lib, "GraphEngine.lib")


// 스프라이트 히트 테스트
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

// 임시클리핑 영역을 설정한다
void CPage::SetTempClip(int x1, int y1, int x2, int y2)
{
	TClipX1 = x1;
	TClipY1 = y1;
	TClipX2 = x2;
	TClipY2 = y2;
}

// 미러찍기 기준 좌표를 설정한다
void CPage::SetMirrorX(int x)
{
	MirrorX = x;
}

/*-----------------------------------------------------------------------------------------
   페이지를 초기화 한다
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
	while(lPitch%4) lPitch++; // Pitch가 4의 배수가 되게 한다
	//lPitch/=2;                // 16비트 칼라여서 편법을 썼다.
	                          // 여기서의 lPitch란 바이트수이다(점의 개수는 아니다)
	lPitchHalf = lPitch/nBPD; // lPitch는 바이트수이므로 8,16,23비트 모드의 점의 개수를 얻는다.

	// 비트맵 인포 구조체를 위한 메모리 할당
	nAllocSize = sizeof(BITMAPINFO) + sizeof(RGBQUAD) * nColorNum;
	//if(nColorNum != 0) nAllocSize -= sizeof(RGBQUAD);
	nAllocSize -= sizeof(RGBQUAD);
	lpBmpInfo = (LPBITMAPINFO)new char[nAllocSize + sizeof(RGBQUAD)];
	if(lpBmpInfo == NULL) return FALSE;

	// 가상 페이지 생성(비트맵 이미지 포인터)
	//lpBit = (WORD*)new char[lPitch * yl];
	//if(lpBit == NULL) return FALSE;

	// 비트맵 인포 해더 생성
	lpBmpInfo->bmiHeader.biSize	         = sizeof (BITMAPINFOHEADER);
	lpBmpInfo->bmiHeader.biWidth         = width;      //width;
	lpBmpInfo->bmiHeader.biHeight        = -height;    // 음수면 그림이 뒤집혀 출력된다
	lpBmpInfo->bmiHeader.biPlanes        = 1;          // 항상1
	lpBmpInfo->bmiHeader.biBitCount	     = nBitNum;    // 도트당 비트수(16비트는 5:6:5)
	lpBmpInfo->bmiHeader.biCompression	 = 0;          // BI_RGB와 같다(비압축)
	lpBmpInfo->bmiHeader.biSizeImage     = lPitch*yl;  // lPitch; // 항상 4의배수
	lpBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	lpBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	lpBmpInfo->bmiHeader.biClrUsed	     = 0;          // 사용된 총 칼라수(항상 0으로 고정)
	lpBmpInfo->bmiHeader.biClrImportant	 = 0;          // 중요한 색상수(항상 0으로 고정)

	// 파레트 세팅
	if( nColorNum != 0) {
		if(palfname) OpenPal(palfname);
		else SetPal(lpPal);
	}

	// DIB를 생성
	ReCreateDIBSection();

	// 그래픽 라입 초기화
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
	//while(lPitch%4) lPitch++; // Pitch가 4의 배수가 되게 한다
	lPitchHalf = lPitch/nBPD; // lPitch는 바이트수이므로 8,16,23비트 모드의 점의 개수를 얻는다.
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

	// DIB를 생성
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
   페이지를 비트맵 파일로 저장한다
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

	// 16비트 이미지를 24비트로 변환(이미지를 뒤집음)
	for(int i=height-1; i>=0; i--) {
		for(int j=0; j<width; j++) {
			WORD color = *((WORD*)lpBit + (i * lPitchHalf + j));
			// 주의) BGR순이다
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

	bmfHeader.bfType = ((WORD)('M'<<8 | 'B'));	// bmp포맷 표시자;
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
   페이지를 JPG 파일로 저장한다(quality = 0-100)
------------------------------------------------------------------------------------------*/
BOOL CPage::SaveToJpeg(LPSTR fname, int quality)
{
	if(!fname) return FALSE;
	if(!lpBit || !lpBmpInfo) return FALSE;
	if(nAllocSize <= 0) return FALSE;
	if(nBitNum != 16) return FALSE;

	int npitch = width * 3;
	//while(npitch%4) npitch++;	// 4바이트 정렬코드
	int allocsize = npitch * height;
	BYTE *pimage = new BYTE[allocsize];
	memset(pimage, 0, allocsize);

	BYTE *bit = pimage;

	// 16비트 이미지를 24비트로 변환(이미지를 뒤집지 않아야한다)
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
   페이지의 파레트를 설정한다

   - 인수 : RGBQUAD의 포인터
------------------------------------------------------------------------------------------*/
BOOL CPage::SetPal(RGBQUAD *lpPal)
{
	if( nColorNum == 0)	return TRUE;        //파레트가 필요없다면

	if(lpLogPal) { free( lpLogPal );  lpLogPal=NULL; } // 메모리 해제
	lpLogPal = (LPLOGPALETTE)malloc( sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * nColorNum );

	lpLogPal->palVersion = 0x300;	        // 팔레트 버젼 입력
	lpLogPal->palNumEntries = nColorNum;    // 팔레트 색상수 입력
	
	int i;
	if(lpPal==NULL){             // 파레트 포인터가 NULL이면 기본 파레트를 생성한다
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
	if( nColorNum == 0)	//파레트가 필요없다면
		return TRUE;
	
	if(lpLogPal!=NULL) { free( lpLogPal );  lpLogPal=NULL; } // 메모리 해제

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
   GDI 표면을 특정 칼라로 채운다
------------------------------------------------------------------------------------------*/
BOOL CPage::FillGDI(HDC hTarDC, int nXStart, int nYStart, COLORREF crColor)
{
	BOOL check;
	check = FloodFill(hTarDC, nXStart, nYStart, crColor);
	if(check != 0)return TRUE;
	else return FALSE;
}

/*-----------------------------------------------------------------------------------------
   다른 페이지의 내용을 현재 페이지 표면에 뿌린다
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
   다른 페이지의 내용을 컬러키를 적용하여 현재 페이지 표면에 뿌린다
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
   페이지가 뿌려질 클라이언트의 특정 좌표를 페이지의 좌표로 변환시킨다
------------------------------------------------------------------------------------------*/
void CPage::ToPagePos(LONG xp, LONG yp, LONG* X, LONG* Y)
{
	*X = (xp-xpFlip)/iZoomX;
	*Y = (yp-ypFlip)/iZoomX;
}

/*-----------------------------------------------------------------------------------------
    페이지를 플립할 GDI표면 위치(클라이언트 영역)을 설정한다
------------------------------------------------------------------------------------------*/
void CPage::SetFlipPos(LONG xp, LONG yp, int zoom)
{
	xpFlip = xp;
	ypFlip = yp;
	iZoomX = zoom;
}

/*-----------------------------------------------------------------------------------------
   페이지의 내용을 GDI 표면에 정수배로 뿌린다(SetPagePos()로 위치를 바꿀수 있다)
------------------------------------------------------------------------------------------*/
BOOL CPage::Flip(HWND hwnd)
{
	// 페이지가 초기화 되지 않았다면
	if(  lpBit == NULL ) return FALSE;
	HDC hDC = GetDC(hwnd);
	BOOL rtn = Flip(hDC);
	ReleaseDC(hwnd, hDC);
	return rtn;
}

BOOL CPage::Flip(HDC hdc)
{
	int rtn;

	// 페이지가 초기화 되지 않았다면
	if(  lpBit == NULL ) return FALSE;

	HDC hDC = hdc;

	if( nColorNum != 0 && hPal != NULL)	//파레트가 필요하다면
	{
		HPALETTE hOldPal;
		hOldPal = SelectPalette( hDC, hPal, TRUE ); //팔레트 선택
		if(hOldPal == NULL) return FALSE;
		rtn = RealizePalette( hDC );				 //팔레트 활성화
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
   페이지의 내용을 GDI 표면에 정수배 확대로 뿌린다(SetFlipPos()를 미리실행)
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
   페이지의 내용을 GDI 표면에 Stretch로 뿌린다
   
   - 소스나 타겟의 RECT가 NULL이면 전체 영역으로 설정된다
------------------------------------------------------------------------------------------*/
BOOL CPage::StretchFlip(HWND hwnd, RECT* rcDest, RECT* rcSrc )
{
	int rtn;

	// 페이지가 초기화 되지 않았다면
	if(  lpBit == NULL ) return FALSE;

	HDC hDC = GetDC(hwnd);

	if( nColorNum != 0 && hPal != NULL)	//파레트가 필요하다면
	{
		HPALETTE hOldPal;
		hOldPal = SelectPalette( hDC, hPal, TRUE ); //팔레트 선택
		if(hOldPal == NULL) return FALSE;
		rtn = RealizePalette( hDC );				 //팔레트 활성화
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

	sy = height-sy-syl; // 그림이 뒤집혀 있으므로 좌표를 뒤집는다

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
//  드로잉 관련
/////////////////////

/*-----------------------------------------------------------------------------------------
   페이지에 수평선을 그린다(flag가 TRUE이면 XOR로 그린다)
------------------------------------------------------------------------------------------*/
void CPage::HLine(int x, int y, int len, WORD color, BOOL flag)
{
	if(lpBit==NULL) return;
	InitGrpLib(width, height, lPitchHalf, 555);

	if(nBitNum == 16) {
		if(flag==FALSE) DrawHLine( x, y, len, (WORD*)lpBit, color );    // 단색 칼라 드로잉
		else if(flag==TRUE) DrawHLineXOR( x, y, len, (WORD*)lpBit);           // XOR 드로잉
	}
	else if(nBitNum == 8) {
		DrawHLine8(x, y, len, (char*)lpBit, (char)color);
	}
}

/*-----------------------------------------------------------------------------------------
   페이지에 수직선을 그린다(flag가 TRUE이면 XOR로 그린다)
------------------------------------------------------------------------------------------*/
void CPage::VLine(int x, int y, int len, WORD color, BOOL flag)
{
	if(lpBit==NULL) return;
	InitGrpLib(width, height, lPitchHalf, 555);

	if(nBitNum == 16) {
		if(flag==FALSE) DrawVLine( x, y, len, (WORD*)lpBit, color );    // 단색 칼라 드로잉
		else if(flag==TRUE) DrawVLineXOR( x, y, len, (WORD*)lpBit);           // XOR 드로잉
	}
	else if(nBitNum == 8) {
		DrawVLine8(x, y, len, (char*)lpBit, (char)color);
	}
}

/*-----------------------------------------------------------------------------------------
   페이지에 선을 그린다(브레제남 알고리즘)
------------------------------------------------------------------------------------------*/
void CPage::DrawLine(int x1, int y1, int x2, int y2, WORD color)
{
	if(lpBit==NULL) return;
	InitGrpLib(width, height, lPitchHalf, 555);

	if(nBitNum == 16) {
		DrawBresLine( x1, y1, x2, y2, (WORD*)lpBit, color );    // 선 그리기(브레제남 알고리즘)
	}
}

/*-----------------------------------------------------------------------------------------
   페이지에 속이 빈 박스를 그린다(flag가 TRUE이면 XOR로 그린다)
------------------------------------------------------------------------------------------*/
void CPage::DrawEmptyBox(int x1, int y1, int x2, int y2, WORD color, BOOL flag)
{
	if(lpBit==NULL) return;
	InitGrpLib(width, height, lPitchHalf, 555);

	if(x1>x2) swap(x1,x2);
	if(y1>y2) swap(y1,y2);

	if(nBitNum == 16) {
		if(flag==FALSE) DrawBox( x1, y1, x2, y2, (WORD*)lpBit, color );    // 단색 칼라 드로잉
		else if(flag==TRUE) DrawBoxXOR( x1, y1, x2, y2, (WORD*)lpBit);           // XOR 드로잉
	}
	else if(nBitNum == 8) {
		DrawBox8(x1, y1, x2, y2, (char*)lpBit, (char)color);
	}
}

/*-----------------------------------------------------------------------------------------
   페이지에 속이 찬 박스를 그린다
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
   페이지에 속이 찬 반투명 박스를 그린다
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
   선택 박스를 그린다
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
//  스프라이트 관련
/////////////////////

/*--------------------------------------------------------------------------------------
   자동 스프라이트 출력 함수(압축/비압축 자동구분, 반투명/플립/단색출력 가능)
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
   자동 스프라이트 출력 함수(압축/비압축 자동구분, 반투명/플립/단색출력 가능)
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

	// 뒤집기 모드이면(상대좌표 포함)
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
   Zoom 스프라이트 출력 함수(압축/비압축 모두 사용가능, 좌우/상하플립 가능)
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
   Zoom 스프라이트 출력 함수(압축/비압축 모두 사용가능, 좌우/상하플립 가능)
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

	if(opt & PERCENT) { tx = pSpr->xl*tx/100; ty = pSpr->yl*ty/100; }	// 100% 기준으로 확대,축소
	if(opt & RXY) { x+=pSpr->rx*tx/pSpr->xl; y+=pSpr->ry*ty/pSpr->yl; }	// 상대좌표를 적용
	if(opt & CENTER) { x -= (tx>>1); y -= (ty>>1); }					// 캐릭터를 좌표의 중심에 위치시킨다
	
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
   자동 이미지 출력 함수(압축/비압축 자동구분, 반투명/플립/단색출력 가능)
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
      자동 이미지 출력 함수(압축/비압축 자동구분, 반투명/플립/단색출력 가능)
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



////////////////////// 여기서부터 8비트 이미지 출력 함수 ///////////////////




/*--------------------------------------------------------------------------------------
   자동 스프라이트 출력 함수(압축/비압축 자동구분, 반투명/플립/단색출력 가능)
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
   자동 스프라이트 출력 함수(압축/비압축 자동구분, 반투명/플립/단색출력 가능)
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

	// 뒤집기 모드이면(상대좌표 포함)
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


	if(!(opt&GB) && !(opt&GLOW))  // 일반출력인가?
	{
		if(pSpr->compress) // 압축인가?
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
	else if(!(opt&GB) && (opt&GLOW))  // 일반 단색 출력인가?
	{
		if(pSpr->compress) // 압축인가?
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
	else if((opt&GB) && !(opt&GLOW)) // 반투명출력인가?
	{
		if(pSpr->compress) // 압축인가?
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
	else if((opt&GB) && (opt&GLOW)) // 단색 반투명출력인가?
	{
		if(pSpr->compress) // 압축인가?
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
   Zoom 스프라이트 출력 함수(압축/비압축 모두 사용가능, 좌우/상하플립 가능)
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
   Zoom 스프라이트 출력 함수(압축/비압축 모두 사용가능, 좌우/상하플립 가능)
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

	if(opt & PERCENT) { tx = pSpr->xl*tx/100; ty = pSpr->yl*ty/100; }	// 100% 기준으로 확대,축소
	if(opt & RXY) { x+=pSpr->rx*tx/pSpr->xl; y+=pSpr->ry*ty/pSpr->yl; }	// 상대좌표를 적용
	if(opt & CENTER) { x -= (tx>>1); y -= (ty>>1); }					// 캐릭터를 좌표의 중심에 위치시킨다
	
	// 1x Zoom
	if(tx == pSpr->xl && ty == pSpr->yl) { PutSprAuto(x, y, pSpr, opt, grade, color); return TRUE; }

	if(!(opt&GB) && !(opt&GLOW))  // 일반출력인가?
	{
		if(pSpr->compress) // 압축인가?
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
	else if(!(opt&GB) && (opt&GLOW))  // 일반 단색 출력인가?
	{
		if(pSpr->compress) // 압축인가?
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
	else if((opt&GB) && !(opt&GLOW)) // 반투명출력인가?
	{
		if(pSpr->compress) // 압축인가?
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
	else if((opt&GB) && (opt&GLOW)) // 단색 반투명출력인가?
	{
		if(pSpr->compress) // 압축인가?
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

