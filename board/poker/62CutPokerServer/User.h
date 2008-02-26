// User.h: interface for the CUser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USER_H__1494A741_1F9D_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_USER_H__1494A741_1F9D_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserStruct.h"
#include "DBThread.h"
#include "CommMsg.h"

#define WH_CHAN	0
#define WH_LOBY	1
#define WH_SHOP	2
#define WH_GAME	3

class CUser  
{

private:
	INT64			m_LostMoney; // [��ȣõ��] 2004.07.08
	INT64			m_PrevMoney; // [��ȣõ��] 2004.07.08
public:
	inline void  Set_init_premoney()  { m_PrevMoney = UI.PMoney; }  // [��ȣõ��] 2004.07.08 ~~
	inline void  Set_init_lost_money(INT64 lmoney) { m_LostMoney = lmoney; }
	inline INT64 Get_lost_money() const { return m_LostMoney; }
	inline void  Set_refresh_lost_money() { m_LostMoney = m_PrevMoney - UI.PMoney; } //call:  DoGameOver, FoldUser

public:
	BOOL bValid;			// ��ȿ �÷���
	USERINFO UI;			// ����� ����
	SMALLUSERINFO SUI;		// ���� ����� ����
	ORGINFO OI;			// �α��� ���� �������� ����
	int Sid;				// ���� �迭 ID
	int ChNum;				// ���� ä�� ��ȣ(-1�ΰ�� ���� ä�ο� �������� �ʾ���)
	int Cid;				// ä�� ���� ��ȣ(ä�ο� ������ ��츸 ��ȿ)
	int PNum;				// �÷��̾� ��ȣ(�濡 ������ ��츸 ��ȿ)	

	char			RegID[16];			// �ֹε�� ��ȣ [���� ������ �۾�]
	char			UniqNo[16];			// ����ũ �ѹ�
	char			UserIP[24];			// ������� ���� IP����	### [�α� ��Ͽ�] ###	

	// ����� ����(���� ������ Ŭ���� ������ ���� �Ұ�)
	// ### [����� ����] ###
	// ### [ �ߺ��� IP�� ] ###
	in_addr			PeerIP;

	BOOL bSendAcceptLogin;	// �α��� ��� �޽����� ���´°�?		[���� ������ �۾�]
	BOOL bLogOutDBSaved;	// �α׾ƿ� DB�� ó����û�Ǿ��°�?
	BOOL bLobyOpen;			// ���� �ʴ�â�� �ѵ� �����ΰ�?
	BOOL bExitSubscript;	// ������ ���� ���ΰ�?
	BOOL bNoWhisper;		// �Ӹ� �ź����ΰ�?
	UINT ConnectSec;		// ���� �ð�(��)
	
	INT64 nBet;             // ��Ŀ�Ӵ� ���ӹ��þ�
	UINT  UniqueTick;        // ���ӱ���� �Ϸ�� �������� ���ϱ����� ���̵�
	BOOL  bIsUpdateData;     // ���ӵ���Ÿ���̽��� ������̴�  

	GAMEITEM_LIST GameItem; // [���� ������ �۾�]
	EVENT_PRIZE EventPrize; // ### [�̺�Ʈ Ƽ��] ###

	BOOL bAdminUser;        //### [������ ��� �۾�] ###
	BOOL bObserver;			// ### [ ������� ] ###

	// [ ����� ]
	int		m_UserLevel;		// �α��ν� ����
	COleDateTime	m_StartTime;		// ���� �ð�

	INT64	nBMoney;			// ���� ��

public:
	CUser();
	virtual ~CUser();
	void Clear();
	
	// ### [�̺�Ʈ Ƽ��] ###
	void SetNewUser(USERINFO *pUI, GAMEITEM_LIST *pItemList, int sid, EVENT_PRIZE *pEvent);	// [���� ������ �۾�]
	BOOL SendAcceptLoginMsg();											// [���� ������ �۾�]

	BOOL MakeDBWork(int workkind, DBWORKS *pWork);
	BOOL IsSetDBWork();

	BOOL SendMsg(CCommMsg *pMsg);

};

#endif // !defined(AFX_USER_H__1494A741_1F9D_11D4_97A5_0050BF0FBE67__INCLUDED_)
