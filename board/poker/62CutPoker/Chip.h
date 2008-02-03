// Chip.h: interface for the CChip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHIP_H__A37227E1_8E37_11D4_97A4_0050BF0FBE67__INCLUDED_)
#define AFX_CHIP_H__A37227E1_8E37_11D4_97A4_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graphic.h"

class CChip  
{
public:
	CPage *pPage, *pBatPage;

	// Ĩ �Ӽ�
	int ChipNo;			// Ĩ��ȣ(0:�Ⱦ��� 1=1Ĩ 2=5Ĩ  3=10Ĩ 4=50Ĩ 5=100Ĩ 6=500Ĩ 7=1000Ĩ ## �߰� 8=5000 9=10000)
	int Price;			// Ĩ�� �ݾ�
	int Kind;			// Ĩ�� ������ ����
	int SprNo;			// ��������Ʈ ��ȣ

	int ShowMode;		// �����ֱ� ���(0:���� ���� 1:������ �� �̵��� ������)

	// ����
	char *MoveStartSnd;	// �̵��� ���۵ɶ��� ����
	char *MoveEndSnd;	// �̵��� ���������� ����

	// Ĩ ����
	BOOL bLive;			// Ĩ�� ����ִ°�?
	BOOL bMove;			// �̵����ΰ�?
	BOOL bEvent;		// �̺�Ʈ�� �ִ°�?

	// ī�� �̵� ���� ����
	int Xp, Yp;			// �׷��� ��ǥ
	int MoveLen;		// �̵��� �Ÿ�
	int SXp, SYp;		// �̵��� ��� ��ǥ
	int TXp, TYp;		// �̵��� ��ǥ ��ǥ
	int MoveDelay;		// �̵� ���� ����ġ
	int EndMoveDelay;	// �̵� �Ϸ� ����ġ
	int MoveSpeed;		// �̵� �ӵ�

	// ������ ���� ����
	BOOL bTwincle;
	int TwincleCnt;		// ������ ī��Ʈ

	CChip();
	virtual ~CChip();

	void Init(CPage *ppage, CPage *pbatpage);
	void SetChip(int chipno, int kind);
	void Reset();
	void OnTimer();
	void SetMoveStartSnd(char *sndfilename);
	void SetMoveEndSnd(char *sndfilename);
	void SetPos(int tx, int ty);
	void SetMove(int tx, int ty, int sdelay=0, int edelay=0, int speed=-1);
	void Draw();
};

#endif // !defined(AFX_CHIP_H__A37227E1_8E37_11D4_97A4_0050BF0FBE67__INCLUDED_)
