# Microsoft Developer Studio Project File - Name="NMServerAgent" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=NMServerAgent - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NMServerAgent.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NMServerAgent.mak" CFG="NMServerAgent - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NMServerAgent - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "NMServerAgent - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NMServerAgent - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /I ".\\Common" /I "..\CommonFile" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"../_BIN/Agent/Poker2ServerAgent.exe"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "NMServerAgent - Win32 Debug"

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
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"../_BIN/Agent/Poker2ServerAgent_debug.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "NMServerAgent - Win32 Release"
# Name "NMServerAgent - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ConncetDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ConncetDlg.h
# End Source File
# Begin Source File

SOURCE=.\KHSRichEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\KHSRichEdit.h
# End Source File
# Begin Source File

SOURCE=.\LogFile.cpp
# End Source File
# Begin Source File

SOURCE=.\LogFile.h
# End Source File
# Begin Source File

SOURCE=.\MakeSrvList.cpp
# End Source File
# Begin Source File

SOURCE=.\MakeSrvList.h
# End Source File
# Begin Source File

SOURCE=.\NMServerAgent.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\marblelogo.ico
# End Source File
# Begin Source File

SOURCE=.\res\NMServerAgent.ico
# End Source File
# Begin Source File

SOURCE=.\res\NMServerAgent.rc2
# End Source File
# End Group
# Begin Group "MAIN WORK"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AdminManager.cpp
# End Source File
# Begin Source File

SOURCE=.\AdminManager.h
# End Source File
# Begin Source File

SOURCE=.\NMServerAgent.cpp

!IF  "$(CFG)" == "NMServerAgent - Win32 Release"

!ELSEIF  "$(CFG)" == "NMServerAgent - Win32 Debug"

# ADD CPP /I ".\\Common"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NMServerAgent.h
# End Source File
# Begin Source File

SOURCE=.\NMServerAgentDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NMServerAgentDlg.h
# End Source File
# Begin Source File

SOURCE=.\SockCmnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SockCmnd.h
# End Source File
# Begin Source File

SOURCE=.\SrvInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvInfo.h
# End Source File
# Begin Source File

SOURCE=.\SrvInfoMan.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvInfoMan.h
# End Source File
# End Group
# Begin Group "SOCKET LIB"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DataSock.cpp
# End Source File
# Begin Source File

SOURCE=.\DataSock.h
# End Source File
# Begin Source File

SOURCE=.\ServerSock.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerSock.h
# End Source File
# Begin Source File

SOURCE=.\SockMan.cpp
# End Source File
# Begin Source File

SOURCE=.\SockMan.h
# End Source File
# Begin Source File

SOURCE=.\StreamBuf.cpp
# End Source File
# Begin Source File

SOURCE=.\StreamBuf.h
# End Source File
# End Group
# Begin Group "CommonFile"

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
