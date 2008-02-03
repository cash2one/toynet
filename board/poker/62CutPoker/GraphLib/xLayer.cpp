//
//                       -  Layer Management Class  -
//
//                          File Name : xLayer.cpp
//                          Author    : Jaehoon Oh
//

#include "StdAfx.h"
#include "xLayer.h"

/*-------------------------------------------------------------------------------
   Class CLayerObj
-------------------------------------------------------------------------------*/
CLayerObj::CLayerObj()
{
	pNext = NULL;
}

/*-------------------------------------------------------------------------------
   Class CLayerSprObj
-------------------------------------------------------------------------------*/
CLayerSprObj::CLayerSprObj()
{
	pNext = NULL;
	pSpr = NULL;
}

void CLayerSprObj::Draw(CPage* lpPage)
{
	lpPage->PutSprAuto(xp, yp, pSpr, opt, grade, color);
}

/*-------------------------------------------------------------------------------
   Class CLayerPlane
-------------------------------------------------------------------------------*/
CLayerPlane::CLayerPlane()
{
	pLayerObj = NULL;
	pNow = NULL;
}

void CLayerPlane::SetObject(int x, int y, SPRITE30 *spr, DWORD opt, int grade, WORD color)
{
	if(spr==NULL) return;

	CLayerObj *pNew = (CLayerObj*) new CLayerSprObj;

	if(pLayerObj==NULL)	pLayerObj = pNew;
	else pNow->pNext = pNew;

	((CLayerSprObj*)pNew)->xp = x;
	((CLayerSprObj*)pNew)->yp = y;
	((CLayerSprObj*)pNew)->pSpr = spr;
	((CLayerSprObj*)pNew)->opt = opt;
	((CLayerSprObj*)pNew)->grade = grade;
	((CLayerSprObj*)pNew)->color = color;
	//pNew->pNext = NULL;
	
	pNow = pNew;
	totObj++;
}

/*-------------------------------------------------------------------------------
   Class xLayer
-------------------------------------------------------------------------------*/
xLayer::xLayer()
{
	pLayer = NULL;
	totLayer = 0;
	zeroBase = 0;
}

xLayer::~xLayer()
{
	Destroy();
}

void xLayer::Destroy()
{
	int i;

	CLayerObj *pNow, *pNext;
	for(i=0; i<totLayer; i++)
	{
		pNext = pLayer[i].pLayerObj;
		for(;pNext != NULL;)
		{
			pNow = pNext;
			pNext = pNow->pNext;
			if(pNow) delete pNow;
		}
		pLayer[i].pLayerObj = NULL;
	}

	if(pLayer) delete []pLayer;
	pLayer = NULL;
	totLayer = 0;
	zeroBase = 0;
}

// Clear Layer Object
void xLayer::Clear()
{
	int i;

	CLayerObj *pNow, *pNext;
	for(i=0; i<totLayer; i++)
	{
		pNext = pLayer[i].pLayerObj;
		for(;pNext != NULL;)
		{
			pNow = pNext;
			pNext = pNow->pNext;
			if(pNow) delete pNow;
		}
		pLayer[i].pLayerObj = NULL;
	}
}

// Initialize xLayer
BOOL xLayer::Init(CPage* lpPage, int layerNum, int zeroPoint)
{
	Destroy();
	pLayer = new CLayerPlane[layerNum];
	totLayer = layerNum;
	zeroBase = zeroPoint;
	pPage = lpPage;
	return TRUE;
}

// Set Sprite Object to Layer
void xLayer::Set(int nLay, int x, int y, SPRITE30 *spr, DWORD opt, int grade, WORD color)
{
	if(pLayer==NULL) return;
	if(spr==NULL) return;
	if(nLay<0 || nLay>=totLayer) return;
	pLayer[nLay].SetObject(x, y, spr, opt, grade, color);
}

// Set Sprite Object to Layer(Override Function) : Don't needed Layer Number!
void xLayer::Set(int x, int y, SPRITE30 *spr, DWORD opt, int grade, WORD color)
{
	if(pLayer==NULL) return;
	if(spr==NULL) return;
	int nLay = y+spr->my;
	if(opt|RXY) nLay+=spr->ry;
	nLay+=zeroBase;
	if(nLay<0 || nLay>=totLayer) return;
	pLayer[nLay].SetObject(x, y, spr, opt, grade, color);
}

// Safe Function
// Set Sprite Object to Layer(Override Function) : Don't needed Layer Number!
void xLayer::Set(int x, int y, xSprite *pSpr, int snum, DWORD opt, int grade, WORD color)
{
	if(pLayer==NULL) return;
	if(pSpr==NULL) return;
	if(snum<0 || snum >= pSpr->sfhead.TotalSprNum) return;

	int nLay = y + pSpr->spr[snum].my;
	if(opt|RXY) nLay += pSpr->spr[snum].ry;
	nLay+=zeroBase;
	if(nLay<0 || nLay>=totLayer) return;
	pLayer[nLay].SetObject(x, y, &pSpr->spr[snum], opt, grade, color);
}

// Set Sprite Object to Layer(Override Function) : Don't needed Layer Number!
void xLayer::Set(int x, int y, int rx, int ry, SPRITE30 *spr, DWORD opt, int grade, WORD color)
{
	if(pLayer==NULL) return;
	if(spr==NULL) return;
	int nLay = y+spr->my + ry;
	nLay+=zeroBase;
	if(nLay<0 || nLay>=totLayer) return;
	pLayer[nLay].SetObject(x+rx, y+ry, spr, opt, grade, color);
}

// Safe Function
// Set Sprite Object to Layer(Override Function) : Don't needed Layer Number!
void xLayer::Set(int x, int y, int rx, int ry, xSprite *pSpr, int snum, DWORD opt, int grade, WORD color)
{
	if(pLayer==NULL) return;
	if(pSpr==NULL) return;
	if(snum<0 || snum >= pSpr->sfhead.TotalSprNum) return;

	int nLay = y + pSpr->spr[snum].my + ry;
	nLay+=zeroBase;
	if(nLay<0 || nLay>=totLayer) return;
	pLayer[nLay].SetObject(x+rx, y+ry, &pSpr->spr[snum], opt, grade, color);
}


// Draw All Layer
void xLayer::Draw(BOOL bDoClear)
{
	if(pLayer==NULL) return;

	CLayerObj *pNow, *pNext;
	for(int i=0; i<totLayer; i++)
	{
		pNext = pLayer[i].pLayerObj;
		for(;pNext != NULL;)
		{
			pNow = pNext;
			pNext = pNow->pNext;
			pNow->Draw(pPage);
		}
	}
	
	if(bDoClear) Clear();
}
