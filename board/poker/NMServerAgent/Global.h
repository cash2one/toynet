// �۷ι� ����, Ŭ����, �Լ�..

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

//#include "ClientDlg.h"

#include "SockMan.h"

extern CClientDlg *g_pMainDlg;
extern CTitleDlg *g_pTitleDlg;
extern CLobyDlg *g_pLobyDlg;
extern CGameDlg *g_pGameDlg;
extern CGameView *g_pGameView;

extern CSockMan SockMan;		// ���� �޴��� Ŭ����

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////

#define WH_FIRST	0			// �α����ϱ���
#define WH_LOGIN	1			// �α�����
#define WH_TITLE	2			// Ÿ��Ʋ ȭ��
#define WH_LOBY		3			// ����
#define WH_GAME		4			// ������



#endif