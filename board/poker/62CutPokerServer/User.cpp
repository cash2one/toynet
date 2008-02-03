// User.cpp: implementation of the CUser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "62CutPokerServer.h"
#include "62CutPokerServerView.h"
#include "User.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUser::CUser()
{
	Clear();
}

CUser::~CUser()
{

}

void CUser::Clear()
{
	bValid = FALSE;
	ZeroMemory(&UI, sizeof(USERINFO));
	ZeroMemory(&SUI, sizeof(SMALLUSERINFO));
	ZeroMemory(&OI, sizeof(ORGINFO));
	ZeroMemory(&GameItem, sizeof(GAMEITEM_LIST)); // [���� ������ �۾�]
	ZeroMemory(UserIP, sizeof(UserIP)); // ������� ���� IP���� ### [�α� ��Ͽ�] ###
	memset(RegID, 0, sizeof(RegID));
	memset(UniqNo, 0, sizeof(UniqNo));
	
	ZeroMemory(&EventPrize, sizeof(EVENT_PRIZE)); // ### [�̺�Ʈ Ƽ��] ###

	// ### [����� ����] ###
	// ### [ �ߺ��� IP�� ] ###
	memset(&PeerIP, 0, sizeof(in_addr));

	// [ ����� ]
	m_UserLevel = 0;		// �α��� ����
	m_StartTime.SetDateTime(0,0,0,0,0,0);

	
	Sid = 0;
	ChNum = -1;
	Cid = -1;
	PNum = -1;

	m_LostMoney = 0; // [��ȣõ��] 2004.07.08
	m_PrevMoney = 0; // [��ȣõ��] 2004.07.08

	bSendAcceptLogin = FALSE;						// [���� ������ �۾�]

	bLogOutDBSaved = FALSE;
	bLobyOpen = FALSE;
	bExitSubscript = FALSE;
	bNoWhisper = FALSE;

	ConnectSec = 0;
	nBet          = 0;
	UniqueTick    = 0;
	bIsUpdateData = FALSE;

	//### [������ ��� �۾�] ###
	bAdminUser        = FALSE; 
	// ### [ ������� ] ###
	bObserver = FALSE;

	// [ ����� ]
	m_UserLevel = 0;		// �α��� ����
	m_StartTime.SetDateTime(0,0,0,0,0,0);

}

// [���� ������ �۾�]
// ### [�̺�Ʈ Ƽ��] ###
void CUser::SetNewUser(USERINFO *pUI, GAMEITEM_LIST *pItemList, int sid, EVENT_PRIZE *pEvent)
{
	Clear();

	bValid = TRUE;
	memcpy(&UI, pUI, sizeof(USERINFO));						// ���� ���� ����
	memcpy(&GameItem, pItemList, sizeof(GAMEITEM_LIST));	// ���� ������ ���� ����	[���� ������ �۾�]


	// �ʱ� ���� ���� ����
	OI.WinNum = UI.WinNum;
	OI.LooseNum = UI.LooseNum;
	OI.DrawNum = UI.DrawNum;
	OI.pBackup = UI.PMoney;

	Sid = sid;							// ���� ��ȣ ����
					
	// ���ο� ������ �޼��� ���� �غ�
	SUI.UNum = UI.UNum;					// ������ȣ
	strncpy(SUI.ID, UI.ID, 15);			// ���̵�
	SUI.Sex = UI.Sex;					// ����
	SUI.nIcon    = UI.nIcon;			// ������
	SUI.Position = WH_CHAN;				// ���� ��ġ(ä��)
	SUI.RoomNum = 0;					// ���� ��ġ�� ���ȣ(�濡 �������� ��ȿ)

	// ### [�̺�Ʈ Ƽ��] ###
	memcpy(&EventPrize, pEvent, sizeof(EVENT_PRIZE));

	//### [������ ��� �۾�] ###
	if(UI.cAdminUser  == 1)	{
		bAdminUser = TRUE;
		SUI.cAdminUser = 1;
	}


		// [ ����� ]
	m_UserLevel = UI.nIcon;				// �α��� ����
	m_StartTime  = COleDateTime::GetCurrentTime();  // �α��ν� �ð�
	if(g_nAgentIndex != -1)				// ������Ʈ�� ���� (��Ŀ�� ���) ��Ŀ�� �ٸ��ٸ� if���� �ּ�ó�� �Ұ�
		g_pMainView->m_cSALog.Make_Log(LEVEL_CONCURRENT_USER, UI.UNum,  TRUE);// ����,..


//	// [ ����� ]
//	m_UserLevel = UI.nIcon;				// �α��� ����
//	m_StartTime  = COleDateTime::GetCurrentTime();  // �α��ν� �ð�
//	if(g_nAgentIndex != -1)				// ������Ʈ�� ���� (��Ŀ�� ���)
//		g_pMainView->Make_StatisticsLog(LEVEL_CONCURRENT_USER, UI.UNum,  TRUE);// ����,..

	UniqueTick = ::timeGetTime();
}

// [���� ������ �۾�]
BOOL CUser::SendAcceptLoginMsg()
{
	if(!bValid) return FALSE;

	// �޽����� �������� �ִٸ� ���
	if(bSendAcceptLogin) return FALSE;

	// �α��� �㰡 �޽��� ����
	LOGININFO Li;
	ZeroMemory(&Li, sizeof(Li));
	memcpy(&Li.UI, &UI, sizeof(USERINFO));

	// [���� ������ �۾�]
//	strncpy(Li.RegID, RegID, 15);	// �ֹε�� ��ȣ
	strncpy(Li.UniqNo, UniqNo, 15);	// ����ũ �ѹ�
	
	// �α��� ��� �޽����� ����
	CSV_ACCEPT_LOGIN msg(SndBuf);
	msg.Set(&Li);
	SendMsg(&msg);

	// �α��� ������ �ִٸ� ������(�޽��� �ڽ�)
	if(g_pMainView->m_ServerLoginMsg.GetLength() > 2) {
		CSV_SERVERMSG smsg(SndBuf);
		smsg.Set(1, (char*)((LPCTSTR)g_pMainView->m_ServerLoginMsg));
		SendMsg(&smsg);
	}

	if(strlen(g_pMainView->Cfg2.TitleNotice) > 2) {
		CSV_SERVERNOTIC nmsg(SndBuf); // Ÿ��Ʋ ����
		nmsg.Set(0, (char*)((LPCTSTR)g_pMainView->Cfg2.TitleNotice) );
		SendMsg(&nmsg);
	}

	bSendAcceptLogin = TRUE;

	return TRUE;
}


BOOL CUser::MakeDBWork(int workkind, DBWORKS *pWork)
{
	if(!pWork) return FALSE;
	ZeroMemory(pWork, sizeof(DBWORKS));

	switch(workkind)
	{
	case 3: // �߰��� ����� ���� ����Ÿ���̽� ��ũ�� ����
		{
			pWork->bValid    = TRUE;					// ��ȿ �÷���
			pWork->WorkKind = 3;						// ���� ����
			pWork->SID      = Sid;						// ���� SID
			pWork->StartTick   = ::timeGetTime();       // Time Tick
			pWork->ConnectTime = max(0, ConnectSec);	// ���� ���� �ð�
			memcpy(&pWork->UI, &UI, sizeof(USERINFO));
			bIsUpdateData = TRUE;
		} break;
		
	case 4: // �α׾ƿ��� ����� ����Ÿ ���̿� ����Ÿ
		{
			pWork->bValid = TRUE;						// ��ȿ �÷���
			pWork->WorkKind = 4;						// ���� ����
			pWork->SID = Sid;							// ���� SID
			pWork->StartTick = ::timeGetTime();			// Time Tick
			pWork->ConnectTime = max(0, ConnectSec);	// ���� ���� �ð�
			memcpy(&pWork->UI, &UI, sizeof(USERINFO));
			memcpy(&pWork->OI, &OI, sizeof(ORGINFO));
			// ### [�̺�Ʈ Ƽ��] ###
			memcpy(&pWork->EventPrize, &EventPrize, sizeof(EVENT_PRIZE));
		// [ ����� ]
		//	g_pMainView->Make_StatisticsLog(LEVEL_CONCURRENT_USER, UI.UNum,  FALSE); // ����, ���λ����, �̿�ð�
			// [ ����� ]
			g_pMainView->m_cSALog.Make_Log(LEVEL_CONCURRENT_USER, UI.UNum,  FALSE); // ����, ���λ����, �̿�ð�

			pWork->LostMoney  = m_LostMoney;			// [��ȣõ��] 2004.07.08

		} break;

	case 5:
		{	// ### [�̺�Ʈ Ƽ��] ###
			pWork->bValid = TRUE;						// ��ȿ �÷���
			pWork->WorkKind = 5;						// ���� ����
			pWork->SID = Sid;							// ���� SID
			pWork->StartTick = ::timeGetTime();			// Time Tick
			pWork->ConnectTime = max(0, ConnectSec);	// ���� ���� �ð�
			memcpy(&pWork->UI, &UI, sizeof(USERINFO));
			memcpy(&pWork->EventPrize, &EventPrize, sizeof(EVENT_PRIZE));
		} break;

	case 9: // ���� ������ ����Ʈ ���� DB �۾�	[���� ������ �۾�]
	case 10: // ���� ������ ��� DB �۾�	[���� ������ �۾�]
		{
			pWork->bValid    = TRUE;					// ��ȿ �÷���
			pWork->WorkKind  = workkind;				// ���� ����
			pWork->SID       = Sid;						// ���� SID
			pWork->StartTick   = ::timeGetTime();		// Time Tick
			memcpy(&pWork->UI, &UI, sizeof(USERINFO));
		}
		break;
		/*	
		
//	case 5: // �α׾ƿ��� ����� ����Ÿ ���̿� ����Ÿ(������������� �����ʴ´�. ���� �ڵ常 �ʱ�ȭ)
//	case 6: // ���÷� �ߺ����ӿ��� Ȯ��(����� 1�п� �ѹ�)
	case 10: // [���� ������ �۾�] �߰��� ���Ӿ����� �۾�
	case 11: // [������� ������] �߰��� ���Ӿ����� �۾�
	case 12: // [���� ������]
		pWork->bValid = TRUE;						// ��ȿ �÷���
		pWork->WorkKind = workkind;						// ���� ����
		pWork->SID = Sid;							// ���� SID
		pWork->StartTick = ::timeGetTime();			// Time Tick
		pWork->ConnectTime = max(0, ConnectSec);	// ���� ���� �ð�
//		pWork->PlusMarble = max(0, PlusMarble);		// ȹ�� ����
//		pWork->MinusMarble = max(0, MinusMarble);	// ���� ����
		memcpy(&pWork->UI, &UI, sizeof(USERINFO));
		memcpy(&pWork->OI, &OI, sizeof(USERINFO));
		break;
*/
	default : 
		return FALSE;
	}
	return TRUE;
}

// ���� ����ġ�� �����ϴٸ� ���� ����Ÿ���̽��� ���Ӱ������ ����� �ʿ� ����
BOOL CUser::IsSetDBWork()
{
	return FALSE;
	if(bValid==FALSE) return FALSE;
	if(ConnectSec<30) return FALSE; // 30�ʹ̸��ڴ� ó������ �ʴ´�
	if(memcmp(&UI,&OI,sizeof(USERINFO))==0) return FALSE;
	return TRUE;
}

// [���� ������ �۾�]
BOOL CUser::SendMsg(CCommMsg *pMsg)
{
	if(Sid < 0 || Sid >= MAX_DATA_SOCKET) return FALSE;
	return SockMan.SendData(Sid, pMsg->pData, pMsg->GetTotalSize());
}