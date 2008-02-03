// Channel.h: interface for the CChannel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHANNEL_H__52EB9D79_BBF5_4ECE_910A_2438BE9AAB71__INCLUDED_)
#define AFX_CHANNEL_H__52EB9D79_BBF5_4ECE_910A_2438BE9AAB71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SockMan.h"
#include "Room.h"

#define MAX_SUPERROOM	30		// �ִ� ���۹� ����	[���� ������ �۾�]
#define MAX_ROOM		70		// �ִ� ���ӷ��� ����(default: 60��)
#define MAX_CHUSER 200	// ä�� �ִ� ���Ӱ��� ������(������ 62CutPokerServerView.h�� MAX_USER�� �����ؾ���)

// [������Ŀ] ��ƾ
#define M_100     (1000000) //  �鸸��
#define M_500     (5000000) // 5�鸸��
#define M_1000    (10000000) //  õ����
#define M_3000    (30000000) // 3õ����
#define M_5000    (50000000) // 5õ����   // �ʱ�ġ
#define M_1Y      (100000000) //   1���
#define M_3Y      (300000000) //   3���
#define M_5Y      (500000000) //   5���
#define M_10Y     (1000000000) //  10���
#define M_100Y    (10000000000) //  100���				�ݾ����� �ʺ�
#define M_1000Y   (100000000000) //  1000���			�ݾ����� �ʺ�
#define M_5000Y   (500000000000) //  5000���			�ݾ����� �߼�
#define M_1B	  (1000000000000) //  1�� (billion) �ݾ����� ���
#define M_10B	  (10000000000000) //  10�� (billion) �ݾ����� ���
#define M_100B	  (100000000000000) //  100�� (billion) �ݾ����� ���
#define M_1000B	  (1000000000000000) //  1000�� (billion) �ݾ����� ���
#define M_1H      (10000000000000000)   //   1��	(hard money)  �ݾ����� ����
#define M_10H 	  (100000000000000000) // 10��	�ݾ����� �����÷ο� ���� ���ѱݾ�



class CChannel  
{
public:
	BOOL CheckGameMoney(INT64 Money);
	int ChNum;				// ä�� ��ȣ
	int TotUser;			// ä�� �̿��� ��
	
	int ChannelStyle; // ä�� ��Ÿ��
	int m_channel_usernum;

	// ������ ����� �ӽ� ����
	// ��Ƽ ������ ä�η� ������ ��� static�� ������� ����
//	static char SndBuf[SENDQUESIZE];

	CRoom Room[MAX_ROOM];	// �� Ŭ����
	int Cid[MAX_CHUSER];	// ä�� ���� ID �ε���
	
	CString strChanName;		// ä�� �̸�
	INT64	DefaultJackPot;		// ������ �⺻ ���� �Ӵ�
	int     JackPotSaveRatio;	// ���� ����
//	int     JackPotPrizeRatio;		// ä�κ� ���� ��� ����(��Ƽ��)
		
//	INT64   ChannelMoney; // �ѵ��ݾ�
	int m_betmoney_5user;
	int m_betmoney_6user;


	CChannel();
	virtual ~CChannel();

	void Init(const int cnum);

	const CHANNELINFO GetChannelInfo();
	void SendMsgToAll(char *pMsgData, int nMsgSize);
	void SendMsgToLoby(char *pMsgData, int nMsgSize);
	void SendMsgToLobyOpen(char *pMsgData, int nMsgSize);

	int OnNewUser(int unum, int &nUseItem); // [���� ������# �۾�]
	BOOL OnOutUser(int unum);
	void OnGlobalGameTimer();

	BOOL ProcessPacket(int sid, USERVAL *pUserVal, char* lpdata, int totsize, int signal);
	
	// [������Ŀ]��ƾ 
	BOOL CheckGameLevel(INT64 PMoney,int nIcon, int nStyle,int nLevel,int &nError);

	char GetBetStyle(int bPlayer);
	char GetRaiseNum(int bPlayer);
};


#endif // !defined(AFX_CHANNEL_H__52EB9D79_BBF5_4ECE_910A_2438BE9AAB71__INCLUDED_)
