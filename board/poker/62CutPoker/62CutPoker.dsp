# Microsoft Developer Studio Project File - Name="62CutPoker" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=62CutPoker - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "62CutPoker.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "62CutPoker.mak" CFG="62CutPoker - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "62CutPoker - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "62CutPoker - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "62CutPoker - Win32 ReleaseDebug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "62CutPoker - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "..\CommonFile" /I ".\GraphLib" /I ".\SoundLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /debugtype:both /machine:I386 /out:"../_BIN/Client/NewPoker.exe" /libpath:".\GraphLib" /libpath:"..\CommonFile" /libpath:".\SoundLib"

!ELSEIF  "$(CFG)" == "62CutPoker - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\CommonFile" /I ".\GraphLib" /I ".\SoundLib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Odbccp32.lib RxCom232.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"../_BIN/Client/NewPoker_Debug.exe" /pdbtype:sept /libpath:".\GraphLib" /libpath:"..\CommonFile" /libpath:".\FolderLib" /libpath:".\SoundLib"

!ELSEIF  "$(CFG)" == "62CutPoker - Win32 ReleaseDebug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "62CutPoker___Win32_ReleaseDebug"
# PROP BASE Intermediate_Dir "62CutPoker___Win32_ReleaseDebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "62CutPoker___Win32_ReleaseDebug"
# PROP Intermediate_Dir "62CutPoker___Win32_ReleaseDebug"
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
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386 /out:"Release/7CardHiLow.exe"
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"Release/62CutPoker.exe"

!ENDIF 

# Begin Target

# Name "62CutPoker - Win32 Release"
# Name "62CutPoker - Win32 Debug"
# Name "62CutPoker - Win32 ReleaseDebug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\62CutPokerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CharView.cpp
# End Source File
# Begin Source File

SOURCE=.\CoverCard.cpp
# End Source File
# Begin Source File

SOURCE=.\GameDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Global.cpp
# End Source File
# Begin Source File

SOURCE=.\LobyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProfileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RoomInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TitleDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\62CutPokerDlg.h
# End Source File
# Begin Source File

SOURCE=.\CharView.h
# End Source File
# Begin Source File

SOURCE=.\CoverCard.h
# End Source File
# Begin Source File

SOURCE=.\GameDlg.h
# End Source File
# Begin Source File

SOURCE=.\Global.h
# End Source File
# Begin Source File

SOURCE=.\LobyDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProfileDlg.h
# End Source File
# Begin Source File

SOURCE=.\RoomInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\TitleDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\62CutPoker.ico
# End Source File
# Begin Source File

SOURCE=.\res\62CutPoker.rc2
# End Source File
# Begin Source File

SOURCE=.\res\62CutPoker_GameItem.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BadUserBack.bmp
# End Source File
# Begin Source File

SOURCE=.\res\banishvote.BMP
# End Source File
# Begin Source File

SOURCE=.\res\BigMsgBack.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\croomback.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00002.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor_i.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor_m.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor_s.cur
# End Source File
# Begin Source File

SOURCE=.\res\image_gameitem.bmp
# End Source File
# Begin Source File

SOURCE=.\res\image_itemtab.bmp
# End Source File
# Begin Source File

SOURCE=.\res\image_ro.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ImageChan.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ImageCheck.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ImageLevel.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Invite.bmp
# End Source File
# Begin Source File

SOURCE=.\res\inviteback.bmp
# End Source File
# Begin Source File

SOURCE=.\res\InviteSendMsg.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LobyBack.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Nsevenpoker.ico
# End Source File
# Begin Source File

SOURCE=.\res\ObserverBack.bmp
# End Source File
# Begin Source File

SOURCE=.\res\OptionBack.bmp
# End Source File
# Begin Source File

SOURCE=.\res\RefuseInvite.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SPoker2_GameItem.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TitleBack.bmp
# End Source File
# Begin Source File

SOURCE=.\res\voteback.BMP
# End Source File
# Begin Source File

SOURCE=".\res\세븐포카2.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\아이콘.ico"
# End Source File
# End Group
# Begin Group "GraphLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GraphLib\Display.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphLib\Display.h
# End Source File
# Begin Source File

SOURCE=.\GraphLib\Dither.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphLib\Dither.h
# End Source File
# Begin Source File

SOURCE=.\GraphLib\Graphic.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphLib\Graphic.h
# End Source File
# Begin Source File

SOURCE=.\GraphLib\GraphMod.h
# End Source File
# Begin Source File

SOURCE=.\GraphLib\GraphMod8.h
# End Source File
# Begin Source File

SOURCE=.\GraphLib\Jpegfile.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphLib\Jpegfile.h
# End Source File
# Begin Source File

SOURCE=.\GraphLib\Sprite8.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphLib\Sprite8.h
# End Source File
# Begin Source File

SOURCE=.\GraphLib\xAniFile.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphLib\xAniFile.h
# End Source File
# Begin Source File

SOURCE=.\GraphLib\xAniMan.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphLib\xAniMan.h
# End Source File
# Begin Source File

SOURCE=.\GraphLib\xHan.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphLib\xHan.h
# End Source File
# Begin Source File

SOURCE=.\GraphLib\xLayer.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphLib\xLayer.h
# End Source File
# Begin Source File

SOURCE=.\GraphLib\xSprite.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphLib\xSprite.h
# End Source File
# End Group
# Begin Group "SockLib"

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
# Begin Group "Etc"

# PROP Default_Filter ""
# Begin Group "GraphicButton"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CheckButton.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckButton.h
# End Source File
# Begin Source File

SOURCE=.\GraphButton.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphButton.h
# End Source File
# Begin Source File

SOURCE=.\MyBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\MyBitmap.h
# End Source File
# Begin Source File

SOURCE=.\MyButton.cpp
# End Source File
# Begin Source File

SOURCE=.\MyButton.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Avatar.h
# End Source File
# Begin Source File

SOURCE=.\BadChat.cpp
# End Source File
# Begin Source File

SOURCE=.\BadChat.h
# End Source File
# Begin Source File

SOURCE=.\BadChatMan.cpp
# End Source File
# Begin Source File

SOURCE=.\BadChatMan.h
# End Source File
# Begin Source File

SOURCE=.\BmpCheckButton.cpp
# End Source File
# Begin Source File

SOURCE=.\BmpCheckButton.h
# End Source File
# Begin Source File

SOURCE=.\BmpRadioBtn.cpp
# End Source File
# Begin Source File

SOURCE=.\BmpRadioBtn.h
# End Source File
# Begin Source File

SOURCE=.\BmpToRegion.cpp
# End Source File
# Begin Source File

SOURCE=.\BmpToRegion.h
# End Source File
# Begin Source File

SOURCE=.\DesktopIconMan.cpp
# End Source File
# Begin Source File

SOURCE=.\DesktopIconMan.h
# End Source File
# Begin Source File

SOURCE=.\DialogCtrlReposition.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogCtrlReposition.h
# End Source File
# Begin Source File

SOURCE=.\IME.cpp
# End Source File
# Begin Source File

SOURCE=.\IME.h
# End Source File
# Begin Source File

SOURCE=.\KHSToolTip.cpp
# End Source File
# Begin Source File

SOURCE=.\KHSToolTip.h
# End Source File
# Begin Source File

SOURCE=.\ListCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ListCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\NMCrypt.h
# End Source File
# Begin Source File

SOURCE=.\NoChat.cpp
# End Source File
# Begin Source File

SOURCE=.\NoChat.h
# End Source File
# Begin Source File

SOURCE=.\SysUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\SysUtil.h
# End Source File
# Begin Source File

SOURCE=.\TitleTip.cpp
# End Source File
# Begin Source File

SOURCE=.\TitleTip.h
# End Source File
# Begin Source File

SOURCE=.\WebCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\WebCtrl.h
# End Source File
# Begin Source File

SOURCE=.\NMCrypt.lib
# End Source File
# End Group
# Begin Group "Game Files"

# PROP Default_Filter ""
# Begin Group "ChattingBox"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CharCutInArray.cpp
# End Source File
# Begin Source File

SOURCE=.\CharCutInArray.h
# End Source File
# Begin Source File

SOURCE=.\ChatBox.cpp
# End Source File
# Begin Source File

SOURCE=.\ChatBox.h
# End Source File
# Begin Source File

SOURCE=.\ChatViewEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\ChatViewEdit.h
# End Source File
# Begin Source File

SOURCE=.\SendEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\SendEdit.h
# End Source File
# End Group
# Begin Group "minigame"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MiniDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MiniDlg.h
# End Source File
# Begin Source File

SOURCE=.\MiniGame.cpp
# End Source File
# Begin Source File

SOURCE=.\MiniGame.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\CharBox.cpp
# End Source File
# Begin Source File

SOURCE=.\CharBox.h
# End Source File
# Begin Source File

SOURCE=.\Game.cpp
# End Source File
# Begin Source File

SOURCE=.\Game.h
# End Source File
# Begin Source File

SOURCE=.\GameView.cpp
# End Source File
# Begin Source File

SOURCE=.\GameView.h
# End Source File
# Begin Source File

SOURCE=.\InfoBox.cpp
# End Source File
# Begin Source File

SOURCE=.\InfoBox.h
# End Source File
# Begin Source File

SOURCE=.\LevelView.cpp
# End Source File
# Begin Source File

SOURCE=.\LevelView.h
# End Source File
# Begin Source File

SOURCE=.\Oring.cpp
# End Source File
# Begin Source File

SOURCE=.\Oring.h
# End Source File
# Begin Source File

SOURCE=.\Player.cpp
# End Source File
# Begin Source File

SOURCE=.\Player.h
# End Source File
# Begin Source File

SOURCE=.\PlayScr.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayScr.h
# End Source File
# Begin Source File

SOURCE=.\SyncFPS.cpp
# End Source File
# Begin Source File

SOURCE=.\SyncFPS.h
# End Source File
# Begin Source File

SOURCE=.\ThreadTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\ThreadTimer.h
# End Source File
# End Group
# Begin Group "SoundLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SoundLib\codec.h
# End Source File
# Begin Source File

SOURCE=.\SoundLib\MciMedia.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundLib\MciMedia.h
# End Source File
# Begin Source File

SOURCE=.\SoundLib\ogg.h
# End Source File
# Begin Source File

SOURCE=.\SoundLib\os_types.h
# End Source File
# Begin Source File

SOURCE=.\SoundLib\Sound.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundLib\Sound.h
# End Source File
# Begin Source File

SOURCE=.\SoundLib\vorbisenc.h
# End Source File
# Begin Source File

SOURCE=.\SoundLib\vorbisfile.h
# End Source File
# Begin Source File

SOURCE=.\SoundLib\xOgg.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundLib\xOgg.h
# End Source File
# Begin Source File

SOURCE=.\SoundLib\xSound.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundLib\xSound.h
# End Source File
# End Group
# Begin Group "NMStr"

# PROP Default_Filter ""
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
# Begin Group "62CutPoker"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Card.cpp
# End Source File
# Begin Source File

SOURCE=.\Card.h
# End Source File
# Begin Source File

SOURCE=.\CardDeck.cpp
# End Source File
# Begin Source File

SOURCE=.\CardDeck.h
# End Source File
# Begin Source File

SOURCE=.\CardDefine.h
# End Source File
# Begin Source File

SOURCE=.\CardEx.cpp
# End Source File
# Begin Source File

SOURCE=.\CardEx.h
# End Source File
# Begin Source File

SOURCE=.\Chip.cpp
# End Source File
# Begin Source File

SOURCE=.\Chip.h
# End Source File
# Begin Source File

SOURCE=.\Movement.cpp
# End Source File
# Begin Source File

SOURCE=.\Movement.h
# End Source File
# Begin Source File

SOURCE=.\SevenCard.cpp
# End Source File
# Begin Source File

SOURCE=.\SevenCard.h
# End Source File
# Begin Source File

SOURCE=.\ShortCut.cpp
# End Source File
# Begin Source File

SOURCE=.\ShortCut.h
# End Source File
# End Group
# Begin Group "BasicClass"

# PROP Default_Filter ""
# Begin Group "Source Files No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\62CutPoker.cpp
# End Source File
# Begin Source File

SOURCE=.\62CutPoker.rc
# End Source File
# Begin Source File

SOURCE=.\BadUserDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BadUserWarningDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BanishVoteDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BannerWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ChangeCharDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LoginDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgBox.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MyVoteDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\RoomPassDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SockCmnd.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\WebProfileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WhisperDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\62CutPoker.h
# End Source File
# Begin Source File

SOURCE=.\BadUserDlg.h
# End Source File
# Begin Source File

SOURCE=.\BadUserWarningDlg.h
# End Source File
# Begin Source File

SOURCE=.\BanishVoteDlg.h
# End Source File
# Begin Source File

SOURCE=.\BannerWnd.h
# End Source File
# Begin Source File

SOURCE=.\ChangeCharDlg.h
# End Source File
# Begin Source File

SOURCE=.\ConfigDlg.h
# End Source File
# Begin Source File

SOURCE=.\LoginDlg.h
# End Source File
# Begin Source File

SOURCE=.\MsgBox.h
# End Source File
# Begin Source File

SOURCE=.\MsgDlg.h
# End Source File
# Begin Source File

SOURCE=.\MyVoteDlg.h
# End Source File
# Begin Source File

SOURCE=.\OptionDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProgressCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RoomPassDlg.h
# End Source File
# Begin Source File

SOURCE=.\SockCmnd.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\WebProfileDlg.h
# End Source File
# Begin Source File

SOURCE=.\WhisperDlg.h
# End Source File
# End Group
# Begin Group "Invite"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\InviteDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\InviteDlg.h
# End Source File
# Begin Source File

SOURCE=.\RecvInviteDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RecvInviteDlg.h
# End Source File
# Begin Source File

SOURCE=.\RefuseInviteDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RefuseInviteDlg.h
# End Source File
# Begin Source File

SOURCE=.\ResultFindIdDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ResultFindIdDlg.h
# End Source File
# Begin Source File

SOURCE=.\SendInviteDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SendInviteDlg.h
# End Source File
# End Group
# End Group
# Begin Group "Game Item Work"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AskBuyItemDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AskBuyItemDlg.h
# End Source File
# Begin Source File

SOURCE=.\GameItemDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GameItemDlg.h
# End Source File
# Begin Source File

SOURCE=.\ItemListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemListDlg.h
# End Source File
# Begin Source File

SOURCE=.\PremServiceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PremServiceDlg.h
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

SOURCE=..\CommonFile\ExHandle.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonFile\ExHandle.h
# End Source File
# Begin Source File

SOURCE=..\CommonFile\UserStruct.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonFile\UserStruct.h
# End Source File
# End Group
# Begin Group "Dialog Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AdminBtnTestDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AdminBtnTestDlg.h
# End Source File
# Begin Source File

SOURCE=.\AdminMessageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AdminMessageDlg.h
# End Source File
# Begin Source File

SOURCE=.\AdminTitleDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AdminTitleDlg.h
# End Source File
# Begin Source File

SOURCE=.\CreateRoomDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateRoomDlg.h
# End Source File
# Begin Source File

SOURCE=.\LeadersMoneyLimit.cpp
# End Source File
# Begin Source File

SOURCE=.\LeadersMoneyLimit.h
# End Source File
# Begin Source File

SOURCE=.\LimitMoneyConfirmDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LimitMoneyConfirmDlg.h
# End Source File
# Begin Source File

SOURCE=.\ObserverDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ObserverDlg.h
# End Source File
# Begin Source File

SOURCE=.\ResultHi.cpp
# End Source File
# Begin Source File

SOURCE=.\ResultHi.h
# End Source File
# Begin Source File

SOURCE=.\RoomOptionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RoomOptionDlg.h
# End Source File
# Begin Source File

SOURCE=.\SelCardChoiceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelCardChoiceDlg.h
# End Source File
# Begin Source File

SOURCE=.\SelCardDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelCardDlg.h
# End Source File
# Begin Source File

SOURCE=.\StartDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StartDlg.h
# End Source File
# End Group
# Begin Group "LobyListView"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\LobbyListWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\LobbyListWnd.h
# End Source File
# End Group
# Begin Group "Server Notice"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MsgBoxDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgBoxDlg.h
# End Source File
# Begin Source File

SOURCE=.\NoticeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NoticeDlg.h
# End Source File
# End Group
# Begin Group "SafeAngelItem"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\NewAskChargeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NewAskChargeDlg.h
# End Source File
# Begin Source File

SOURCE=.\WebControl4Charge\WebCtrl4New.cpp
# End Source File
# Begin Source File

SOURCE=.\WebControl4Charge\WebCtrl4New.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
