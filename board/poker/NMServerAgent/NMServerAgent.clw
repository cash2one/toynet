; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CNMServerAgentDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "nmserveragent.h"
LastPage=0

ClassCount=11
Class1=CConncetDlg
Class2=CDataSaveThread
Class3=CDataSock
Class4=CKHSRichEdit
Class5=CMakeSrvList
Class6=CNetData
Class7=CNMServerAgentApp
Class8=CAboutDlg
Class9=CNMServerAgentDlg
Class10=CServerSock
Class11=CSockCmnd

ResourceCount=6
Resource1=IDD_CONNECT
Resource2=IDD_DLG_ADMINMAN
Resource3=IDR_POPMENU
Resource4=IDD_SRV_LIST
Resource5=IDD_NMSERVERAGENT_DIALOG
Resource6=IDD_ABOUTBOX

[CLS:CConncetDlg]
Type=0
BaseClass=CDialog
HeaderFile=ConncetDlg.h
ImplementationFile=ConncetDlg.cpp
LastObject=IDC_EDIT_SRV_INFO
Filter=D
VirtualFilter=dWC

[CLS:CDataSaveThread]
Type=0
BaseClass=CWinThread
HeaderFile=DataSaveThread.h
ImplementationFile=DataSaveThread.cpp

[CLS:CDataSock]
Type=0
BaseClass=CAsyncSocket
HeaderFile=DataSock.h
ImplementationFile=DataSock.cpp

[CLS:CKHSRichEdit]
Type=0
BaseClass=CRichEditCtrl
HeaderFile=KHSRichEdit.h
ImplementationFile=KHSRichEdit.cpp

[CLS:CMakeSrvList]
Type=0
BaseClass=CDialog
HeaderFile=MakeSrvList.h
ImplementationFile=MakeSrvList.cpp
LastObject=IDC_BUTTON_EDIT
Filter=D
VirtualFilter=dWC

[CLS:CNetData]
Type=0
BaseClass=CObject
HeaderFile=NetData.h
ImplementationFile=NetData.cpp

[CLS:CNMServerAgentApp]
Type=0
BaseClass=CWinApp
HeaderFile=NMServerAgent.h
ImplementationFile=NMServerAgent.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=NMServerAgentDlg.cpp
ImplementationFile=NMServerAgentDlg.cpp

[CLS:CNMServerAgentDlg]
Type=0
BaseClass=CDialog
HeaderFile=NMServerAgentDlg.h
ImplementationFile=NMServerAgentDlg.cpp
LastObject=CNMServerAgentDlg
Filter=D
VirtualFilter=dWC

[CLS:CServerSock]
Type=0
BaseClass=CAsyncSocket
HeaderFile=ServerSock.h
ImplementationFile=ServerSock.cpp

[CLS:CSockCmnd]
Type=0
BaseClass=CWnd
HeaderFile=SockCmnd.h
ImplementationFile=SockCmnd.cpp

[DLG:IDD_CONNECT]
Type=1
Class=CConncetDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_COMBO_LIST,combobox,1344339971
Control5=IDC_BUTTON_MAKE_LIST,button,1342242816
Control6=IDC_EDIT_SRV_INFO,edit,1342244996

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_POPMENU]
Type=1
Class=?
Command1=IDC_MENU_DEL
Command2=IDC_MENU_DISCONNECT
Command3=IDC_MENU_COPY
CommandCount=3

[DLG:IDD_SRV_LIST]
Type=1
Class=CMakeSrvList
ControlCount=13
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST_SRV,SysListView32,1350631685
Control4=IDC_STATIC,button,1342177287
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT1,edit,1350631552
Control9=IDC_EDIT2,edit,1350631552
Control10=IDC_EDIT3,edit,1350631553
Control11=IDC_BUTTON_MAKE,button,1342242816
Control12=IDC_BUTTON_EDIT,button,1342242816
Control13=IDC_STATIC,static,1342308352

[DLG:IDD_NMSERVERAGENT_DIALOG]
Type=1
Class=CNMServerAgentDlg
ControlCount=22
Control1=IDC_BUTTON_CONNECT,button,1342242816
Control2=IDC_EDIT_ID,edit,1350631552
Control3=IDC_BUTTON_FIND,button,1342242816
Control4=IDC_BUTTON_DISCONNECT,button,1342242816
Control5=IDC_COMBO_ULIST,combobox,1344339971
Control6=IDC_CHECK_DCON,button,1476460547
Control7=IDC_LIST_SRV,SysListView32,1350631689
Control8=IDC_LIST_USER,SysListView32,1350631429
Control9=IDC_RICHEDITLIST,RICHEDIT,1082196096
Control10=IDC_BUTTON_SAVE,button,1342242816
Control11=IDCANCEL,button,1342242816
Control12=IDC_STATIC,static,1342177283
Control13=IDC_STATIC,button,1342177287
Control14=IDC_BUTTON_AUTO,button,1342242816
Control15=IDC_BUTTON_ALLDIS,button,1342242816
Control16=IDC_STATIC_JACKPOT,static,1350696962
Control17=IDC_CHECK_AVERAGE,button,1342242819
Control18=IDC_STATIC_AUTO_MSG,static,1342308352
Control19=IDC_STATIC_QUEUE,static,1342308354
Control20=IDC_BUTTON_ADMIN,button,1342242816
Control21=IDC_STATIC,button,1342178055
Control22=IDC_STATIC,button,1342178055

[DLG:IDD_DLG_ADMINMAN]
Type=1
Class=?
ControlCount=14
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_ID,edit,1350631552
Control5=IDC_LIST_ADMIN,SysListView32,1350631429
Control6=IDC_EDIT_GRADE,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_BUTTON_ADD,button,1342242816
Control9=IDC_BUTTON_DEL,button,1342242816
Control10=IDC_BTN_ADMININFOSEND,button,1342242816
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC_COUNT,static,1342308354
Control13=IDC_IPADDRESS1,SysIPAddress32,1342242816
Control14=IDC_STATIC,static,1342308352

