// Global....


#include "StdAfx.h"
#include "Global.h"


CClientDlg *g_pMainDlg=NULL;		// ���� ���̾�α� ������(��������)
CTitleDlg *g_pTitleDlg=NULL;		// Ÿ��Ʋ ���̾�α� ������(��������)
CLobyDlg *g_pLobyDlg=NULL;			// ���� ���̾�α� ������(��������)
CGameDlg *g_pGameDlg=NULL;			// ���� ���̾�α� ������(��������)
CGameView *g_pGameView=NULL;		// ���� �� ���� ������(��������)

CSockMan SockMan;			// ���� �޴��� Ŭ����
CSound Sound;				// ���� Ŭ����
CDisplay Display;			// ���÷��� ��� ���� Ŭ����
xHan g_Han;					// 16��Ʈ��� �ѱ� Ŭ����
CNoChat NoChat;				// ä�� ���� Ŭ����

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////

CString g_ServIP("211.115.65.132");		// ���� ������ IP
UINT g_ServPort=8100;					// ���� ������ ��Ʈ��ȣ
CString g_LoginID;						// �ڵ� �α��� ID
CString g_LoginPass;					// �ڵ� �α��� �н�����

CString g_ServerName;	// ������ ���� �̸�
CString g_ChannelName;	// ������ ä�� �̸�
CString g_NoticeURL;	// �������� URL
BOOL g_bIsItemChannel;	// ������ ä���ΰ�?

CRect g_OrgMainDlgRect;	// ���� ���̾�α��� ���� ũ��
int g_MainSrvSID = 0;	// ���� ������ ���� ��ȣ
int g_Where = WH_FIRST;	// ���� ��ġ�� ��(WH_FIRST,...)
int g_MyPN = 0;			// ���� �÷��̾� ��ȣ
int g_MastPN = 0;		// ������ �÷��̾� ��ȣ

ROOMINFO g_RI={0,};		// �� ���� ����ü
CPlayer Play[10];		// �÷��̾� ����


/////////////////////////////////////////

CField Field;
CGame Game;
CWhisperDlg g_WhisperDlg;

