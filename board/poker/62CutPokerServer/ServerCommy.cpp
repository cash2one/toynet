#include"stdafx.h"
#include<stdio.h>

#include "servercommy.h"

// 서버들과 서로의 인식을 위한 메시지 
UINT uCmmHaloMsgSC = 0;	
UINT uCmmHaloMsgCS = 0;
UINT uCmmGotBootMsgSC = 0;

// 서버 상태 메시지 
UINT uCmmGameStatusSC = 0;
UINT uCmmGameStatusCS = 0;

// 서버 메시지 출력
UINT uCmmServerMsgSendSC = 0;
UINT uCmmServerMsgSendCS = 0;

// 서버 신규 접속 막음 
UINT uCmmStopAcceptSC = 0;
UINT uCmmStopAcceptCS = 0;


// 서버 다운 메시지 출력 
UINT uCmmServerDownMsgSC = 0;
UINT uCmmServerDownMsgCS = 0;

UINT uCmmServerUserSizeSC = 0;
UINT uCmmServerUserSizeCS = 0;

UINT uCmmDisbandUserANS = 0;

HWND _BridgeServerHandle = HWND_BROADCAST;



///////////////////////////////////////////////////////////
// Function
BOOL InitNetmarbleCommn(void)
{

	uCmmHaloMsgSC		= RegisterWindowMessage("NetmarbleHaloSC1");
	uCmmHaloMsgCS	    = RegisterWindowMessage("NetmarbleHaloCS2");
	uCmmGotBootMsgSC	= RegisterWindowMessage("NetmarbleGotBootMsg11");
	
	uCmmGameStatusSC	= RegisterWindowMessage("NetmarbleGameStatusSC3");
	uCmmGameStatusCS	= RegisterWindowMessage("NetmarbleGameStatusSC4");

	uCmmServerMsgSendSC	= RegisterWindowMessage("NetmarbleMsgSendSC5");
	uCmmServerMsgSendCS	= RegisterWindowMessage("NetmarbleMsgSendCS6");

	uCmmStopAcceptSC	= RegisterWindowMessage("NetmarbleAcceptSC7");
	uCmmStopAcceptCS	= RegisterWindowMessage("NetmarbleAcceptCS8");

	uCmmServerDownMsgSC	= RegisterWindowMessage("NetmarbleServerDownNow9");
	uCmmServerDownMsgCS	= RegisterWindowMessage("NetmarbleImDownNow10");

	uCmmServerUserSizeSC = RegisterWindowMessage("NetmarbleServerUserSizeSc12");
	uCmmServerUserSizeCS = RegisterWindowMessage("NetmarbleServerUserSizeCs13");

	uCmmDisbandUserANS = RegisterWindowMessage("Netmarble.Server.User.Disconnect");

	return 1;
}

void M_HaloGameServer(HWND myhwnd)
{
	::SendMessage(HWND_BROADCAST,uCmmHaloMsgSC,(WPARAM)myhwnd,0);
}

void M_GotServerBootMsg(HWND myhwnd,HWND svrhwnd)
{
	::SendMessage(svrhwnd,uCmmGotBootMsgSC,(WPARAM)myhwnd,0);
}

void M_ImHereBridge(HWND myhwnd,int gamecode,int servercode,int port,char *name)
{
	tag_ServerHalo cData;

	sprintf(cData.name,"%s",name);
	GetModuleFileName(AfxGetInstanceHandle(),cData.path,256);
	cData.gamecode = gamecode;
	cData.port = port;
	cData.servercode = servercode;

	COPYDATASTRUCT SendDataStruct;
	SendDataStruct.cbData = sizeof(tag_ServerHalo);
	SendDataStruct.dwData = 0;
	SendDataStruct.lpData =(void*)&cData;
	if(_BridgeServerHandle != NULL)
	::SendMessage(_BridgeServerHandle,WM_COPYDATA,(WPARAM)myhwnd,(LPARAM)&SendDataStruct);
}


void M_GetServerStatus(HWND myhwnd,HWND svrhandle)		// 서버들에게 상태를 물어본다.
{
	::PostMessage(svrhandle,uCmmGameStatusSC,(WPARAM)myhwnd,0);
}

void M_SendMySetatus(HWND myhwnd,int status,int usersize) // 자신의 상태를 관리기에게 알린다.
{
	int sz = (status << 16) + usersize;
	::PostMessage(_BridgeServerHandle,uCmmGameStatusCS,(WPARAM)myhwnd,sz);
}


void M_ServerDownNow(HWND myhwnd,HWND svrhandle)		// 서버에게 다운 메시지를 준다. 
{
	::PostMessage(svrhandle,uCmmServerDownMsgSC,(WPARAM)myhwnd,0);
}


void M_ImDieNow(HWND svrhandle)				// 나 주거유우우 라고 관리기에게 알린다. 3분뒤에 핸들 체크하여 강제 종료.
{
	::PostMessage(_BridgeServerHandle,uCmmServerDownMsgCS,(WPARAM)svrhandle,0);
}


void SetBridgeHandle(HWND hwnd)
{
	_BridgeServerHandle = hwnd;
}


void SendMsgToServer(HWND svrhwnd,HWND myhwnd,int code,char *msg)
{
	tag_SvrMsg cData;

	sprintf(cData.strMsg,"%s",msg);
	cData.nCode = code;

	COPYDATASTRUCT SendDataStruct;
	SendDataStruct.cbData = sizeof(tag_SvrMsg);
	SendDataStruct.dwData = 0;
	SendDataStruct.lpData =(void*)&cData;
	if(svrhwnd != NULL)	::SendMessage(svrhwnd,WM_COPYDATA,(WPARAM)myhwnd,(LPARAM)&SendDataStruct);
}

void SendMsgToServerSingleUser(HWND svrhwnd,HWND myhwnd,int code,char *msg,char *id)
{
	tag_SingleMsg cData;
	sprintf(cData.ID,"%s",id);
	sprintf(cData.strMsg,"%s",msg);
	cData.Code = code;

	COPYDATASTRUCT SendDataStruct;
	SendDataStruct.cbData = sizeof(tag_SingleMsg);
	SendDataStruct.dwData = 0;
	SendDataStruct.lpData =(void*)&cData;
	if(svrhwnd != NULL)	::SendMessage(svrhwnd,WM_COPYDATA,(WPARAM)myhwnd,(LPARAM)&SendDataStruct);

}


void SendUserSizeToBridge(HWND svrhandle,int usersize)
{
	if(_BridgeServerHandle != HWND_BROADCAST && _BridgeServerHandle)
	{
		::PostMessage(_BridgeServerHandle,uCmmServerUserSizeCS,(WPARAM)svrhandle,(LPARAM)usersize);		
	}
}

void M_SetStopAccept(HWND myhwnd,HWND svrhandle,int bStopAccept)
{
	::PostMessage(svrhandle,uCmmStopAcceptSC,(WPARAM)myhwnd,(LPARAM)bStopAccept);
}


void M_ServerAcceptStatus(HWND svrhandle,int bStopAccept)
{
	::PostMessage(_BridgeServerHandle,uCmmStopAcceptCS,(WPARAM)svrhandle,(LPARAM)bStopAccept);
}


void M_DisconnectUser(HWND svrhandle,HWND  myhwnd,char *id)
{
	tag_DisbandUser cData;
	sprintf(cData.ID,"%s",id);
	COPYDATASTRUCT SendDataStruct;
	SendDataStruct.cbData = sizeof(tag_DisbandUser);
	SendDataStruct.dwData = 0;
	SendDataStruct.lpData =(void*)&cData;
	if(svrhandle != NULL)	::SendMessage(svrhandle,WM_COPYDATA,(WPARAM)myhwnd,(LPARAM)&SendDataStruct);
}

void M_DisConnectAns(HWND svrhandle,BOOL tf)
{
	::PostMessage(_BridgeServerHandle,uCmmDisbandUserANS,(WPARAM)svrhandle,(LPARAM)tf);
}

void M_SingleMsgSecc(HWND  myhwnd,char *id,int index)
{
	/*
		index :  0 : 짜르기 성공,  1 : 메시지보내기 성공 

	*/
	tag_MsgSendOK cData;
	sprintf(cData.ID,"%s",id);
	cData.Index = index;
	COPYDATASTRUCT SendDataStruct;
	SendDataStruct.cbData = sizeof(tag_MsgSendOK);
	SendDataStruct.dwData = 0;
	SendDataStruct.lpData =(void*)&cData;
	if(myhwnd != NULL)	::SendMessage(_BridgeServerHandle,WM_COPYDATA,(WPARAM)myhwnd,(LPARAM)&SendDataStruct);
}