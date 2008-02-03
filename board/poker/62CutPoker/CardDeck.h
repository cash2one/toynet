// CardDeck.h: interface for the CCardDeck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CARDDECK_H__BA644A7F_D5BD_4F61_B013_B0BCFC25AF53__INCLUDED_)
#define AFX_CARDDECK_H__BA644A7F_D5BD_4F61_B013_B0BCFC25AF53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CardEx.h"

#define SELECT_TOTALCARD 8  //6 by softpark  // ###[8��Ŀ]###
#define EMPTY -1 // ����� (62Cut)
#define _CARD_YPOS_OFFSET 5

// 7���� ī�嵦
class CCardDeck  
{
public:
	
	int PNum;			   // �÷��̾� �ѹ�
	CCardEx Card[SELECT_TOTALCARD];		   // ī������ Ŭ���� // ###[8��Ŀ]###	
	CRect CardRect[SELECT_TOTALCARD];		// ī�� �ٲٱ� rect ��

	CPage *pPage;
	
	int TotalCardNum;
	// ���� ī�嵦 ��ġ
	int TmpTotalCardNum;
	int m_ChangeCardIndex;			// �ٲ� �ε��� 
	int m_ChangeCardNum;		// �ٲ� ���� 

	int m_SelectCard;		// ���õ� ī�� �ε��� 
	BOOL bWindCardEnd;
	
	int m_returnChangeCardIndex;			// ���� ���ڿ� ���Ҷ� ����
	int m_returnChangeCardNum;		// ���� ���ڿ� ���Ҷ� ����
	int m_returnCardNum;			// �ٲٱ� ��û�� �ؼ� ���� �޾ƿ� ����

	int Xp;
	int Yp;

	int m_xp[2], m_yp[2];

public:
	CCardDeck();
	virtual ~CCardDeck();

	void carddeck_changeinitpos();

	void Init(CPage *ppage, int pnum);

	void Reset();
	void NewCardSet(int cardNum);
	int GetCardNo(int index);
	void DrawXY();
	void SetFaceUp(int index);
	void SetFaceDown(int index);
	void CardSet(int cardnum);
	void OnTimer();
	void Draw( BOOL bFold = FALSE);

	BOOL GetFace(int index);

	//62cut
	void ReSetCardRect();
	void SetCardRect();					
	BOOL SetCardPosition(int index,int pnum,int changenum,int num,int delay=0);	
	void SetChangeCard();			//�ٲ�ī�� ����
	BOOL PtInCardRect(int xp,int yp,int flg=0);//0 onclick 1 dbclick
	void SetChangeSelectCard();		//�ٲٱ� ��û��ī�� ǥ��

};

#endif // !defined(AFX_CARDDECK_H__BA644A7F_D5BD_4F61_B013_B0BCFC25AF53__INCLUDED_)
