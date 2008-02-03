// CardMan.h: interface for the CCardMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CARDMAN_H__2EAE02B7_C644_4013_AB20_BF5D386E2011__INCLUDED_)
#define AFX_CARDMAN_H__2EAE02B7_C644_4013_AB20_BF5D386E2011__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "CKBMPLIST.h"

#define CARD_W (71)
#define CARD_H (96)

class CCardMan  
{
public:
	void Draw(int nX,int nY,CDC *pDC,int nCard);
	void DrawDeck(int nX,int nY,CDC *pDC,int nCard,int nStyle=2);
	
	BOOL Init();
	void Clear();

	int m_nTotalCard;           // ī�嵥ũ ��
	int m_nBCardW,m_nBCardH,m_nSCardW,m_nSCardH; // ī�嵥ũ ���μ��λ�����

//	CKBMPLIST  m_Deck; // ī�嵥ũ
//	CKBMPLIST  m_Card; // ī��

	CCardMan();
	virtual ~CCardMan();

};

#endif // !defined(AFX_CARDMAN_H__2EAE02B7_C644_4013_AB20_BF5D386E2011__INCLUDED_)
