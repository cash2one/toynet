//
//                       -  Layer Management Class  -
//
//                          File Name : xLayer.h
//                          Author    : Jaehoon Oh
//

#ifndef __XLAYER_H__
#define __XLAYER_H__

#include <windows.h>
//#include "xDraw.h"

#include "Graphic.h"

// Class Layer Object (Base Class)
class CLayerObj
{
public:
	CLayerObj *pNext;
	
	int xp, yp;

	CLayerObj();
	virtual void Draw(CPage* lpPage)=0;
};

// Sprite Object of Layer Class 
class CLayerSprObj : public CLayerObj
{
public:
	SPRITE30 *pSpr;
	int opt;
	int grade;
	WORD color;

	CLayerSprObj();
	void Draw(CPage* lpPage);
};

// Class Layer Plane
class CLayerPlane
{
public:
	int totObj;
	CLayerObj *pLayerObj, *pNow;

	CLayerPlane();
	void SetObject(int x, int y, SPRITE30* spr, DWORD opt, int grade, WORD color);
};

// Class Layer Management
class xLayer
{
public:
	CPage *pPage;
	CLayerPlane *pLayer;
	int totLayer;
	int zeroBase;

	xLayer();
	~xLayer();
	void Destroy();

	BOOL Init(CPage *lpPage, int layerNum, int zeroPoint);
	void Clear();
	void Draw(BOOL bDoClear = TRUE);

	// Set a Sprite to Layer
	void Set(int nLay, int x, int y, SPRITE30* spr, DWORD opt=NULL, int grade=15, WORD color=0);
	void Set(int x, int y, SPRITE30 *spr, DWORD opt=NULL, int grade=15, WORD color=0);
	void Set(int x, int y, xSprite *pSpr, int snum, DWORD opt=NULL, int grade=15, WORD color=0);
	void Set(int x, int y, int rx, int ry, SPRITE30 *spr, DWORD opt=NULL, int grade=15, WORD color=0);
	void Set(int x, int y, int rx, int ry, xSprite *pSpr, int snum, DWORD opt=NULL, int grade=15, WORD color=0);
};


#endif