# Microsoft Developer Studio Project File - Name="62CutPokerServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=62CutPokerServer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "62CutPokerServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "62CutPokerServer.mak" CFG="62CutPokerServer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "62CutPokerServer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "62CutPokerServer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "62CutPokerServer - Win32 ReleaseDebug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "62CutPokerServer - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "..\CommonFile" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /debugtype:both /machine:I386 /out:"../_BIN/Server/NewPokerServer.exe" /libpath:"..\CommonFile"

!ELSEIF  "$(CFG)" == "62CutPokerServer - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\CommonFile" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"../_BIN/Server/62CutPokerServer_debug.exe" /pdbtype:sept /libpath:"..\CommonFile"

!ELSEIF  "$(CFG)" == "62CutPokerServer - Win32 ReleaseDebug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "62CutPokerServer___Win32_ReleaseDebug"
# PROP BASE Intermediate_Dir "62CutPokerServer___Win32_ReleaseDebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "62CutPokerServer___Win32_ReleaseDebug"
# PROP Intermediate_Dir "62CutPokerServer___Win32_ReleaseDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /I "..\CommonFile" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386 /out:"Release/7CardHiLowServer.exe"
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"Release/62CutPokerServer.exe"

!ENDIF 

# Begin Target

# Name "62CutPokerServer - Win32 Release"
# Name "62CutPokerServer - Win32 Debug"
# Name "62CutPokerServer - Win32 ReleaseDebug"
# Begin Group "SocketLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FastSocket.h
# End Source File
# Begin Source File

SOURCE=.\FirstInclude.h
# End Source File
# Begin Source File

SOURCE=.\IocpThread.h
# End Source File
# Begin Source File

SOURCE=.\MySocket.cpp
# End Source File
# Begin Source File

SOURCE=.\MySocket.h
# End Source File
# Begin Source File

SOURCE=.\RecvQue.h
# End Source File
# Begin Source File

SOURCE=.\SendQue.h
# End Source File
# Begin Source File

SOURCE=.\SockMan.cpp
# End Source File
# Begin Source File

SOURCE=.\SockMan.h
# End Source File
# Begin Source File

SOURCE=.\SockEngine.lib
# End Source File
# End Group
# Begin Group "DB Files"

# PROP Default_Filter ""
# Begin Group "GameWork"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DBThread.cpp
# End Source File
# Begin Source File

SOURCE=.\DBThread.h
# End Source File
# Begin Source File

SOURCE=.\GameDB.cpp
# End Source File
# Begin Source File

SOURCE=.\GameDB.h
# End Source File
# Begin Source File

SOURCE=.\ItemDB.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemDB.h
# End Source File
# Begin Source File

SOURCE=.\Pre_UserLimitGMLogDB.cpp
# End Source File
# Begin Source File

SOURCE=.\Pre_UserLimitGMLogDB.h
# End Source File
# Begin Source File

SOURCE=.\PremIPDB.cpp
# End Source File
# Begin Source File

SOURCE=.\PremIPDB.h
# End Source File
# Begin Source File

SOURCE=.\PremiumDB.cpp
# End Source File
# Begin Source File

SOURCE=.\PremiumDB.h
# End Source File
# End Group
# Begin Group "BasicWork"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AvatarDB.cpp
# End Source File
# Begin Source File

SOURCE=.\AvatarDB.h
# End Source File
# Begin Source File

SOURCE=.\BadUserDB.cpp
# End Source File
# Begin Source File

SOURCE=.\BadUserDB.h
# End Source File
# Begin Source File

SOURCE=.\BadUserDBThread.cpp
# End Source File
# Begin Source File

SOURCE=.\BadUserDBThread.h
# End Source File
# Begin Source File

SOURCE=.\DBMan.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMan.h
# End Source File
# Begin Source File

SOURCE=.\StatusDB.cpp
# End Source File
# Begin Source File

SOURCE=.\StatusDB.h
# End Source File
# Begin Source File

SOURCE=.\UserDB.cpp
# End Source File
# Begin Source File

SOURCE=.\UserDB.h
# End Source File
# End Group
# Begin Group "DBLogManager"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AllinCharge.cpp
# End Source File
# Begin Source File

SOURCE=.\AllinCharge.h
# End Source File
# Begin Source File

SOURCE=.\LogManDBMan.cpp
# End Source File
# Begin Source File

SOURCE=.\LogManDBMan.h
# End Source File
# Begin Source File

SOURCE=.\LogManDBThread.cpp
# End Source File
# Begin Source File

SOURCE=.\LogManDBThread.h
# End Source File
# Begin Source File

SOURCE=.\LogRecordDB.cpp
# End Source File
# Begin Source File

SOURCE=.\LogRecordDB.h
# End Source File
# End Group
# Begin Group "EventWork"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EventPrizeDB.cpp
# End Source File
# Begin Source File

SOURCE=.\EventPrizeDB.h
# End Source File
# End Group
# Begin Group "StatisticsLog"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\StatisticsLog.cpp
# End Source File
# Begin Source File

SOURCE=.\StatisticsLog.h
# End Source File
# Begin Source File

SOURCE=.\StatisticsLogDB.cpp
# End Source File
# Begin Source File

SOURCE=.\StatisticsLogDB.h
# End Source File
# End Group
# End Group
# Begin Group "StrFile"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\NetData.cpp
# End Source File
# Begin Source File

SOURCE=.\NetData.h
# End Source File
# Begin Source File

SOURCE=.\NMSTR.cpp
# End Source File
# Begin Source File

SOURCE=.\NMSTR.h
# End Source File
# Begin Source File

SOURCE=.\NMSTRMAN.cpp
# End Source File
# Begin Source File

SOURCE=.\NMSTRMAN.h
# End Source File
# End Group
# Begin Group "SevenCard"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Raise.cpp
# End Source File
# Begin Source File

SOURCE=.\Raise.h
# End Source File
# Begin Source File

SOURCE=.\SevenCard.cpp
# End Source File
# Begin Source File

SOURCE=.\SevenCard.h
# End Source File
# End Group
# Begin Group "BasicClass"

# PROP Default_Filter ""
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\62CutPoker.ico
# End Source File
# Begin Source File

SOURCE=.\res\62CutPokerServer.ico
# End Source File
# Begin Source File

SOURCE=.\res\62CutPokerServer.rc2
# End Source File
# Begin Source File

SOURCE=.\res\62CutPokerServerDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Nsevenpoker.ico
# End Source File
# Begin Source File

SOURCE=.\res\SPoker2ServerDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\TetrisServerDoc.ico
# End Source File
# Begin Source File

SOURCE=".\res\æ∆¿Ãƒ‹.ico"
# End Source File
# End Group
# Begin Group "Dialog Files"

# PROP Default_Filter ""
# Begin Group "Basic"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Config2Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Config2Dlg.h
# End Source File
# Begin Source File

SOURCE=.\Config3Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Config3Dlg.h
# End Source File
# Begin Source File

SOURCE=.\ConfigDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigDlg.h
# End Source File
# Begin Source File

SOURCE=.\ConfigSADlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigSADlg.h
# End Source File
# Begin Source File

SOURCE=.\MsgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetChanInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetChanInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetColumnDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetColumnDlg.h
# End Source File
# Begin Source File

SOURCE=.\StartUpDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StartUpDlg.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AdminManager.cpp
# End Source File
# Begin Source File

SOURCE=.\AdminManager.h
# End Source File
# Begin Source File

SOURCE=.\CaptureTalk.cpp
# End Source File
# Begin Source File

SOURCE=.\CaptureTalk.h
# End Source File
# Begin Source File

SOURCE=.\ConfigEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigEvent.h
# End Source File
# Begin Source File

SOURCE=.\ConfigJackPotDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigJackPotDlg.h
# End Source File
# Begin Source File

SOURCE=.\SearchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SearchDlg.h
# End Source File
# End Group
# Begin Group "Source File"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\62CutPokerServer.cpp
# End Source File
# Begin Source File

SOURCE=.\62CutPokerServer.rc
# End Source File
# Begin Source File

SOURCE=.\62CutPokerServerDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\LogFile.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header File"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\62CutPokerServer.h
# End Source File
# Begin Source File

SOURCE=.\62CutPokerServerDoc.h
# End Source File
# Begin Source File

SOURCE=.\LogFile.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\62CutPokerServerView.cpp
# End Source File
# Begin Source File

SOURCE=.\Channel.cpp
# End Source File
# Begin Source File

SOURCE=.\Global.cpp
# End Source File
# Begin Source File

SOURCE=.\Room.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerCommy.cpp
# End Source File
# Begin Source File

SOURCE=.\User.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\62CutPokerServerView.h
# End Source File
# Begin Source File

SOURCE=.\Channel.h
# End Source File
# Begin Source File

SOURCE=.\Global.h
# End Source File
# Begin Source File

SOURCE=.\Room.h
# End Source File
# Begin Source File

SOURCE=.\ServerCommy.h
# End Source File
# Begin Source File

SOURCE=.\User.h
# End Source File
# End Group
# Begin Group "Common File"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\CommonFile\CommMsg.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonFile\CommMsg.h
# End Source File
# Begin Source File

SOURCE=..\CommonFile\UserStruct.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonFile\UserStruct.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
