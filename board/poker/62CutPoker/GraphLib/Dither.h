
#ifndef	__DITHER_H__
#define	__DITHER_H__

#ifndef __AFXWIN_H__
#include <windows.h>
#endif

#include <stdlib.h>

void setup_dither_mask(DWORD Red,DWORD Green,DWORD Blue,int BytesPerPixel);
void dither_rgb24(BYTE *Source,int Xsize,int Ysize,BYTE **pTarget);

#endif