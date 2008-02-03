
// 24비트 이미즈를 16비트 이미지로 변환하면서 디더링

#include "stdafx.h"
#include "dither.h"


/*
	// 마스크를 어떻게 세팅하느냐에 따라,
	// 24bit RGB -> RGB444, RGB565, RGB555 어떤 경우든 변환이 가능.
	// 단 입력소스는 한 픽셀이 3바이트로 이루어지는 RGB24로 고정.

	// 24비트 이미지를 16비트(RGB555)로 변환하는 예제
	DWORD rmask = (DWORD)0x0000001F;
	DWORD gmask = (DWORD)0x000003E0;
	DWORD bmask = (DWORD)0x00007C00;
	// 24비트 이미지를 디더링하여 16비트(5:5:5) 이미지로 변환
	BYTE* pImage16;
	setup_dither_mask(rmask, gmask, bmask, 2);
	dither_rgb24(pImage24, xsize, ysize, &pImage16);

*/


struct _PixelFormat {
	int RedShift,GreenShift,BlueShift;
	DWORD RedMask,GreenMask,BlueMask,AllMask;
	int BytesPerPixel;

	DWORD RedErrorMask,GreenErrorMask,BlueErrorMask;
};
static struct _PixelFormat TargetPixelFormat;

int get_bit_count(DWORD dw)
{
	int bitcnt = 0;
	for(int i=0;i<32;i++) {
		if(dw & 0x00000001) {
			bitcnt++;
		}
		dw >>= 1;
	}
	return bitcnt;
}


void setup_dither_mask(DWORD Red,DWORD Green,DWORD Blue,int BytesPerPixel)
{
	int i=0,j=0;
	DWORD dw=0;

	TargetPixelFormat.RedMask=Red;
	TargetPixelFormat.GreenMask=Green;
	TargetPixelFormat.BlueMask=Blue;
	TargetPixelFormat.AllMask=Red|Green|Blue;
	TargetPixelFormat.BytesPerPixel=BytesPerPixel;

 	dw=TargetPixelFormat.RedMask;
	for (i=31;i>=0;i--) {
  		if ((dw&0x80000000)!=0) break;
		dw<<=1;
	}
	TargetPixelFormat.RedShift=i-7;

	dw=TargetPixelFormat.GreenMask;
	for (i=31;i>=0;i--) {
		if ((dw&0x80000000)!=0) break;
		dw<<=1;
	}
	TargetPixelFormat.GreenShift=i-7;

	dw=TargetPixelFormat.BlueMask;
	for (i=31;i>=0;i--) {
		if ((dw&0x80000000)!=0) break;
		dw<<=1;
	}
	TargetPixelFormat.BlueShift=i-7;

	TargetPixelFormat.RedErrorMask=0;
	dw=0x00000001;
	for (i=8-get_bit_count(Red);i>0;i--) {
		TargetPixelFormat.RedErrorMask|=dw;
		dw<<=1;
	}

	TargetPixelFormat.GreenErrorMask=0;
	dw=0x00000001;
	for (i=8-get_bit_count(Green);i>0;i--) {
		TargetPixelFormat.GreenErrorMask|=dw;
		dw<<=1;
	}

	TargetPixelFormat.BlueErrorMask=0;
	dw=0x00000001;
	for (i=8-get_bit_count(Blue);i>0;i--) {
		TargetPixelFormat.BlueErrorMask|=dw;
		dw<<=1;
	}
}

// 빠른 디더링 속도를 얻기위하여 수정하였음
void dither_rgb24(BYTE *Source,int Xsize,int Ysize,BYTE **pTarget)
{
	int i=0,j=0,PalIndex=0;
	int Red=0,Green=0,Blue=0,RGBpixel=0,dw=0;
	int DitherValue=0,Offset=0;
	int Error[3]={0,};
	int PixelCounter=0;
	int xx=0,yy=0;
	BYTE *p1=NULL,*p2=NULL;
	BYTE *TempSource=NULL;

	int OffsetTable[4]={ Xsize*3-3, Xsize*3, Xsize*3+3, 3 };
	int Order[]={0,1,2,3};

	for (i=0;i<10;i++) {
		//int i1=random(4),i2=random(4),t;
		int i1=rand()%4, i2=rand()%4, t;
		t=Order[i1];
		Order[i1]=Order[i2];
		Order[i2]=t;
	}

	*pTarget=(BYTE *)malloc(Xsize*Ysize*TargetPixelFormat.BytesPerPixel);

	TempSource=(BYTE *)malloc(Xsize*Ysize*3);
	memcpy(TempSource,Source,Xsize*Ysize*3);

	p2=*pTarget;
	for (yy=0;yy<Ysize;yy++) {
		p1=TempSource+yy*Xsize*3;
		for (xx=0;xx<Xsize;xx++) {
			Red=p1[2]; Green=p1[1]; Blue=p1[0];
			Error[2]=Red&TargetPixelFormat.RedErrorMask;
			Error[1]=Green&TargetPixelFormat.GreenErrorMask;
			Error[0]=Blue&TargetPixelFormat.BlueErrorMask;

			int Error3D[] = {Error[0]/3, Error[1]/3, Error[2]/3};

			if (yy<Ysize-1) {
				if (xx==0) {
					for (i=0;i<3;i++) {
						DitherValue=0;
						Offset=3;
						if ((255-p1[Offset+i])>=Error[i]/3) {
							DitherValue+=Error[i]/3;
							p1[Offset+i]+=Error[i]/3;
						}
						else {
							DitherValue+=(255-p1[Offset+i]);
							p1[Offset+i]=255;
						}

						Offset=Xsize*3;
						if (255-p1[Offset+i]>=Error[i]/3) {
							DitherValue+=Error[i]/3;
							p1[Offset+i]+=Error[i]/3;
						}
						else {
							DitherValue+=(255-p1[Offset+i]);
							p1[Offset+i]=255;
						}

						Offset=Xsize*3+3;
						if (255-p1[Offset+i]>=(Error[i]-DitherValue)) {
							p1[Offset+i]+=(Error[i]-DitherValue);
						}
						else {
							p1[Offset+i]=255;
						}
					}
				}
				else if (xx==Xsize-1) {
					for (i=0;i<3;i++) {
						DitherValue=0;
						Offset=Xsize*3;
						if ((255-p1[Offset+i])>=Error[i]/2) {
							DitherValue+=Error[i]/2;
							p1[Offset+i]+=Error[i]/2;
						}
						else {
							DitherValue+=(255-p1[Offset+i]);
							p1[Offset+i]=255;
						}

						Offset=Xsize*3-3;
						if ((255-p1[Offset+i])>=(Error[i]-DitherValue)) {
							p1[Offset+i]+=(Error[i]-DitherValue);
						}
						else {
							p1[Offset+i]=255;
						}
					}
				}
				else {
					/*
					int OffsetTable[4]={ Xsize*3-3, Xsize*3, Xsize*3+3, 3 };
					int Order[]={0,1,2,3};

					for (i=0;i<10;i++) {
						//int i1=random(4),i2=random(4),t;
						int i1=rand()%4, i2=rand()%4, t;
						t=Order[i1];
						Order[i1]=Order[i2];
						Order[i2]=t;
					}

					for (i=0;i<3;i++) {
						DitherValue=0;

						for (j=0;j<2;j++) {						
							Offset=OffsetTable[Order[j]];
							if (255-p1[Offset+i]>=Error3D[i]) {
								DitherValue+=Error3D[i];
								p1[Offset+i]+=Error3D[i];
							}
							else {
								DitherValue+=(255-p1[Offset+i]);
								p1[Offset+i]=255;
							}
						}

						Offset=OffsetTable[Order[2]];
						if (255-p1[Offset+i]>=(Error[i]-DitherValue)) {
							p1[Offset+i]+=(Error[i]-DitherValue);
						}
						else {
							p1[Offset+i]=255;
						}
					}
					*/

					for (i=0;i<3;i++) {
						DitherValue=0;

						for (j=0;j<2;j++) {						
							//Offset=OffsetTable[Order[j]];
							Offset=OffsetTable[j];
							if (255-p1[Offset+i]>=Error3D[i]) {
								DitherValue+=Error3D[i];
								p1[Offset+i]+=Error3D[i];
							}
							else {
								DitherValue+=(255-p1[Offset+i]);
								p1[Offset+i]=255;
							}
						}

						Offset=OffsetTable[Order[2]];
						if (255-p1[Offset+i]>=(Error[i]-DitherValue)) {
							p1[Offset+i]+=(Error[i]-DitherValue);
						}
						else {
							p1[Offset+i]=255;
						}
					}
				}
			}

			if (TargetPixelFormat.RedShift>0) Red<<=TargetPixelFormat.RedShift; else Red>>=(-TargetPixelFormat.RedShift);
			if (TargetPixelFormat.GreenShift>0) Green<<=TargetPixelFormat.GreenShift; else Green>>=(-TargetPixelFormat.GreenShift);
			if (TargetPixelFormat.BlueShift>0) Blue<<=TargetPixelFormat.BlueShift; else Blue>>=(-TargetPixelFormat.BlueShift);

			Red&=TargetPixelFormat.RedMask; Green&=TargetPixelFormat.GreenMask; Blue&=TargetPixelFormat.BlueMask;
			RGBpixel=Red|Green|Blue|(~TargetPixelFormat.AllMask);

			for (i=0;i<TargetPixelFormat.BytesPerPixel;i++) {
				p2[i]=(BYTE)(RGBpixel&0xFF);  // Stored in Intel-word order
				RGBpixel>>=8;
			}

			p1+=3;
			p2+=TargetPixelFormat.BytesPerPixel;
		}
	}

	free(TempSource);
}

/*
// 원래의 초저속 디더링 함수 (-_-)
void dither_rgb24_original(BYTE *Source,int Xsize,int Ysize,BYTE **pTarget)
{
	int i=0,j=0,PalIndex=0;
	int Red=0,Green=0,Blue=0,RGBpixel=0,dw=0;
	int DitherValue=0,Offset=0;
	int Error[3]={0,};
	int PixelCounter=0;
	int xx=0,yy=0;
	BYTE *p1=NULL,*p2=NULL;
	BYTE *TempSource=NULL;

	*pTarget=(BYTE *)malloc(Xsize*Ysize*TargetPixelFormat.BytesPerPixel);

	TempSource=(BYTE *)malloc(Xsize*Ysize*3);
	memcpy(TempSource,Source,Xsize*Ysize*3);

	p2=*pTarget;
	for (yy=0;yy<Ysize;yy++) {
		p1=TempSource+yy*Xsize*3;
		for (xx=0;xx<Xsize;xx++) {
			Red=p1[2]; Green=p1[1]; Blue=p1[0];
			Error[2]=Red&TargetPixelFormat.RedErrorMask;
			Error[1]=Green&TargetPixelFormat.GreenErrorMask;
			Error[0]=Blue&TargetPixelFormat.BlueErrorMask;

			if (yy<Ysize-1) {
				if (xx==0) {
					for (i=0;i<3;i++) {
						DitherValue=0;
						Offset=3;
						if ((255-p1[Offset+i])>=Error[i]/3) {
							DitherValue+=Error[i]/3;
							p1[Offset+i]+=Error[i]/3;
						}
						else {
							DitherValue+=(255-p1[Offset+i]);
							p1[Offset+i]=255;
						}

						Offset=Xsize*3;
						if (255-p1[Offset+i]>=Error[i]/3) {
							DitherValue+=Error[i]/3;
							p1[Offset+i]+=Error[i]/3;
						}
						else {
							DitherValue+=(255-p1[Offset+i]);
							p1[Offset+i]=255;
						}

						Offset=Xsize*3+3;
						if (255-p1[Offset+i]>=(Error[i]-DitherValue)) {
							p1[Offset+i]+=(Error[i]-DitherValue);
						}
						else {
							p1[Offset+i]=255;
						}
					}
				}
				else if (xx==Xsize-1) {
					for (i=0;i<3;i++) {
						DitherValue=0;
						Offset=Xsize*3;
						if ((255-p1[Offset+i])>=Error[i]/2) {
							DitherValue+=Error[i]/2;
							p1[Offset+i]+=Error[i]/2;
						}
						else {
							DitherValue+=(255-p1[Offset+i]);
							p1[Offset+i]=255;
						}

						Offset=Xsize*3-3;
						if ((255-p1[Offset+i])>=(Error[i]-DitherValue)) {
							p1[Offset+i]+=(Error[i]-DitherValue);
						}
						else {
							p1[Offset+i]=255;
						}
					}
				}
				else {
					int OffsetTable[4]={ Xsize*3-3, Xsize*3, Xsize*3+3, 3 };
					int Order[]={0,1,2,3};

					for (i=0;i<10;i++) {
						//int i1=random(4),i2=random(4),t;
						int i1=rand()%4, i2=rand()%4, t;
						t=Order[i1];
						Order[i1]=Order[i2];
						Order[i2]=t;
					}

					for (i=0;i<3;i++) {
						DitherValue=0;

						for (j=0;j<2;j++) {						
							Offset=OffsetTable[Order[j]];
							if (255-p1[Offset+i]>=Error[i]/3) {
								DitherValue+=Error[i]/3;
								p1[Offset+i]+=Error[i]/3;
							}
							else {
								DitherValue+=(255-p1[Offset+i]);
								p1[Offset+i]=255;
							}
						}

						Offset=OffsetTable[Order[2]];
						if (255-p1[Offset+i]>=(Error[i]-DitherValue)) {
							p1[Offset+i]+=(Error[i]-DitherValue);
						}
						else {
							p1[Offset+i]=255;
						}
					}
				}
			}

			if (TargetPixelFormat.RedShift>0) Red<<=TargetPixelFormat.RedShift; else Red>>=(-TargetPixelFormat.RedShift);
			if (TargetPixelFormat.GreenShift>0) Green<<=TargetPixelFormat.GreenShift; else Green>>=(-TargetPixelFormat.GreenShift);
			if (TargetPixelFormat.BlueShift>0) Blue<<=TargetPixelFormat.BlueShift; else Blue>>=(-TargetPixelFormat.BlueShift);

			Red&=TargetPixelFormat.RedMask; Green&=TargetPixelFormat.GreenMask; Blue&=TargetPixelFormat.BlueMask;
			RGBpixel=Red|Green|Blue|(~TargetPixelFormat.AllMask);

			for (i=0;i<TargetPixelFormat.BytesPerPixel;i++) {
				p2[i]=(BYTE)(RGBpixel&0xFF);  // Stored in Intel-word order
				RGBpixel>>=8;
			}

			p1+=3;
			p2+=TargetPixelFormat.BytesPerPixel;
		}
	}

	free(TempSource);
}
*/