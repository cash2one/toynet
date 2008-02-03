/*==========================================================================
                            Direct Draw Library
===========================================================================*/


//#define INITGUID

#include "StdAfx.h"
#include "Display.h"


CDisplay::CDisplay()
{
	winHandle = NULL;
	DDRAWINIT = FALSE;		// 다이렉트 드로우 초기화 성공 여부

	lpDD = NULL;
	lpPrimary = NULL;
	//lpBack = NULL;
}

CDisplay::~CDisplay()
{
	if(DDRAWINIT)
	{
		CloseDirectDraw();
	}
}

BOOL CDisplay::InitDirectDraw(HWND hwnd, int xl, int yl, int cBit)
{
	if(DDRAWINIT==TRUE) CloseDirectDraw();
    //Initializing Direct X
	HRESULT hr;
	winHandle = hwnd;
	
	LPDIRECTDRAW lpDDTemp;

	hr = DirectDrawCreate(NULL, &lpDDTemp, NULL);
	if(hr != DD_OK) { DDRAWINIT=FALSE; return FALSE;}

	hr = lpDDTemp->QueryInterface(IID_IDirectDraw2, (LPVOID*)&lpDD);
	if(hr != DD_OK) { DDRAWINIT=FALSE; return FALSE;}
	lpDDTemp->Release();

	hr = lpDD->SetCooperativeLevel(hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT | DDSCL_ALLOWMODEX);
	if(hr != DD_OK) { DDRAWINIT=FALSE; return FALSE; }

	hr = lpDD->SetDisplayMode(xl,yl,cBit,0,0);
	if(hr != DD_OK) { DDRAWINIT=FALSE; return FALSE; }

/*
	//Create Primary Surface
	DDSURFACEDESC ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	//ddsd.dwBackBufferCount = 2;
	ddsd.dwBackBufferCount = 0;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
	
	LPDIRECTDRAWSURFACE lpTempSurface;
	
	hr = lpDD->CreateSurface(&ddsd, &lpTempSurface, NULL);
	if(hr != DD_OK) { DDRAWINIT=FALSE; return FALSE; }
	
	hr = lpTempSurface->QueryInterface(IID_IDirectDrawSurface3,(void**)&lpPrimary);
	if(hr != DD_OK) { DDRAWINIT=FALSE; return FALSE; }
    lpTempSurface->Release();
*/

/*
	//Create Backbuffer Surface
	DDSCAPS ddscaps;
	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
	hr = lpPrimary->GetAttachedSurface(&ddscaps, &lpBack);
	if(hr != DD_OK) { DDRAWINIT=FALSE; return FALSE; }
*/
	
	DDRAWINIT=TRUE;
	return TRUE;
}


void CDisplay::CloseDirectDraw()
{
	//if(lpBack != NULL)    { lpBack->Release();    lpBack=NULL; }
	if(lpPrimary != NULL) { lpPrimary->Release(); lpPrimary=NULL; }
	if(lpDD != NULL)      { lpDD->Release();      lpDD=NULL; }
	DDRAWINIT=FALSE;
}

LPSTR CDisplay::LockPrimary(DWORD *lPitch)
{
	DDSURFACEDESC ddsd;
    ddsd.dwSize = sizeof(ddsd);
	
	while(lpPrimary->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) == DDERR_SURFACELOST)
	{
		lpPrimary->Restore();
	}

	*lPitch = ddsd.lPitch;
	return ((LPSTR) ddsd.lpSurface);
}

void CDisplay::UnlockPrimary()
{		
	lpPrimary->Unlock(NULL);
}

