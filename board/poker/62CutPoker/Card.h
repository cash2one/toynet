// Card.h: interface for the CCard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CARD_H__34924AA1_21CA_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_CARD_H__34924AA1_21CA_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graphic.h"
#include "Sound.h"
#include "CardDefine.h"
#include "MoveMent.h"

class CCard  
{
public:
	BOOL PtInCard( int x, int y );

	CPage *pPage;

	POINT m_size;		// ��������Ʈ ũ�� (ī�� ũ��)

	CARDEVENT Event;
	CARDEVENT Event1;
//	BOOL bShowShadow;
	// ī�� �Ӽ�
	int CardNo;			// ī���ȣ(0 ~ 51, 0 ~ 12: Ŭ�ι�  13 ~ 25:���̾Ƹ�� 26~38 :��Ʈ  39 ~ 51:�����̵�)
	int PNum;			// ī���� ������(0-7:�÷��̾�)
	int Where;			// ī�尡 ���� �ִ� ��ġ (  ī�嵦 0, ���� 1)
	int Kind;			// ���� �з� (1-12��... (13=������))
	int Attrib;			// ī���� �Ӽ�(1:�� 2:�� 3:�� 4:Ȭ�� )
	
	int ShowMode;		// �����ֱ� ���(0:���� ���� 1:������ �� �̵��� ������)

	// ����
//	SND_ENUM MoveStartSnd;	// �̵��� ���۵ɶ��� ����
//	SND_ENUM MoveEndSnd;	// �̵��� ���������� ����

	// �˸� �̺�Ʈ
	BOOL bNotifyEvent;	// �˸� �̺�Ʈ�� �ִ°�?
	COLORREF NotifyStringColor;	// �˸� �̺�Ʈ ��Ʈ�� ����
	CString NotifyString;// �˸� �̺�Ʈ ��Ʈ��


	// ī�� ����
	BOOL bShow;			// ȭ�鿡 �������°�?
	BOOL bFront;		// �ո��� ���̴� �����ΰ�?
	BOOL bMove;			// �̵����ΰ�?
	BOOL bAccelMove;	// ���� �̵����ΰ�?
	// �̺�Ʈ�� ���õ� ����.
	BOOL bEvent;		// ī�� �̺�Ʈ�� �ִ°�?
	BOOL bEvent1;		// ī�� �̺�Ʈ�� �ִ°�?

	// ī�� �̵� ���� ����
	int Xp, Yp;			// ī�尡 �׷��� ��ǥ
	int SXp, SYp;		// ī�� �̵��� ��� ��ǥ
	int TXp, TYp;		// ī�� �̵��� ��ǥ ��ǥ
	int MoveDelay;		// �̵� ���� ����ġ
	int EndMoveDelay;	// �̵� �Ϸ� ����ġ
	int MoveSpeed;		// �̵� �ӵ�
	int AccelSpeed;		// ���� �̵� �ӵ�

	// ��¦�� ���� ����
	BOOL bTwincle;
	int TwincleCnt;		// ��¦�� ī��Ʈ

	// ������ ��� ����
	BOOL bGlass;	

	int  GetCardNum()	{ return CardNo; }



	// �̵��� ���õ� ����.
	CMovement	m_move;
	void StraightMoveTo( POINT dest, float spd_gamma, float acc_gamma, int delay = 0 );


	
	CCard();
	virtual ~CCard();

	void Init(CPage *ppage, int cardnum, int pnum);	// ī�� ����
	void Clear();			// ��� Ŭ����
	void Reset();			// ���� ������ 
	void OnTimer();
	
	void SetEvent(int eventcode, int option=0, int option1 = 0, int option2 = 0);
	void SetEvent1(int eventcode, int option=0, int option1 = 0, int option2 = 0);
	
	void ClearEvent();
	void ClearEvent1();
	void SetNotifyEvent(char *pstr, COLORREF color=RGB(0,0,0));
	void ClearNotifyEvent();
	void SetMoveStartSnd(SND_ENUM sound);
	void SetMoveEndSnd(SND_ENUM sound);
	void SetPos(int tx, int ty);
	void SetMove(int tx, int ty, int sdelay=0, int edelay=0, int speed=-1);
	void SetAccelMove();


	// ��ǥ �� 
	void DrawXY(int x, int y);
	void Draw( BOOL bFold = FALSE );


};

#endif // !defined(AFX_CARD_H__34924AA1_21CA_11D4_97A5_0050BF0FBE67__INCLUDED_)
