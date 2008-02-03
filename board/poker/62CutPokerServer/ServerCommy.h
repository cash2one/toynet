#ifndef __DEFINECOMMYDATA__
#define __DEFINECOMMYDATA__

// ������� ������ �ν��� ���� �޽��� 
extern UINT uCmmHaloMsgSC;	
extern UINT uCmmHaloMsgCS;
extern UINT uCmmGotBootMsgSC;	// ������ ���õȰ��� �ν��ߴٰ� �����⿡�� ������.

// ���� ���� �޽��� 
extern UINT uCmmGameStatusSC;
extern UINT uCmmGameStatusCS;

// ���� �޽��� ���
extern UINT uCmmServerMsgSendSC;
extern UINT uCmmServerMsgSendCS;

// ���� �ű� ���� ���� 
extern UINT uCmmStopAcceptSC;
extern UINT uCmmStopAcceptCS;


// ���� �ٿ� �޽��� ��� 
extern UINT uCmmServerDownMsgSC;
extern UINT uCmmServerDownMsgCS;

// ����� üũ �Լ� 
extern UINT uCmmServerUserSizeSC;
extern UINT uCmmServerUserSizeCS;

extern UINT uCmmDisbandUserANS;

typedef struct {
	int  gamecode;
	int  servercode;
	int  port;
	char name[64];
	char path[256];
} tag_ServerHalo;


typedef struct {
	int nCode;
	char strMsg[256];
}  tag_SvrMsg;

typedef struct {
	int  Code;
	char ID[16];
	char strMsg[128];
} tag_SingleMsg;

typedef struct {
	char ID[16];
} tag_DisbandUser;


typedef struct {
	int  Index;
	char ID[16];
} tag_MsgSendOK;
///////////////////////////////////////////////////////////
// Function
extern BOOL InitNetmarbleCommn(void);

extern void M_HaloGameServer(HWND myhwnd = HWND_BROADCAST);			// �����鿡�� �������� ���縦 �˸���.
extern void M_ImHereBridge(HWND myhwnd,int gamecode,int servercode,int port,char *name);
extern void M_GotServerBootMsg(HWND myhwnd,HWND svrhwnd);

extern void M_GetServerStatus(HWND myhwnd,HWND svrhandle);			// �����鿡�� ���¸� �����.
extern void M_SendMySetatus(HWND myhwnd,int status,int usersize);	// �ڽ��� ���¸� �����⿡�� �˸���.


extern void M_ServerDownNow(HWND myhwnd,HWND svrhandle);		// �������� �ٿ� �޽����� �ش�. 
extern void M_ImDieNow(HWND svrhandle);				// �� �ְ������ ��� �����⿡�� �˸���. 3�еڿ� �ڵ� üũ�Ͽ� ���� ����.

extern void SetBridgeHandle(HWND hwnd);
extern void SendMsgToServer(HWND svrhwnd,HWND myhwnd,int code,char *msg);
extern void SendMsgToServerSingleUser(HWND svrhwnd,HWND myhwnd,int code,char *msg,char *id);

extern void SendUserSizeToBridge(HWND svrhandle,int usersize);

// ���� ���� ��� / ����� �÷��� ���� 
extern void M_SetStopAccept(HWND myhwnd,HWND svrhandle,int bStopAccept);
extern void M_ServerAcceptStatus(HWND svrhandle,int bStopAccept);

// ����� ���� ���� 
extern void M_DisconnectUser(HWND svrhandle,HWND myhwnd,char *id);
extern void M_DisConnectAns(HWND svrhandle,BOOL tf);

extern void M_SingleMsgSecc(HWND  myhwnd,char *id,int index);
#endif