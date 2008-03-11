// Sound.cpp: implementation of the CSound class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\62CutPoker.h"
#include "Sound.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// 사운드 파일명

CSound::CSound()
{
	bInited = FALSE;
	InitErrorKind = 0;

	bDontUseSndFx = FALSE;
	bDontUseMusic = FALSE;

	memset(SndPoly, 0, sizeof(SndPoly));
}

CSound::~CSound()
{
	Destroy();
}

BOOL CSound::Init(HWND hWnd)
{
	if(xSound::Init(hWnd)==DS_OK) bInited=TRUE;
	else bInited = FALSE;


// ### 남성
	// 점젆은버젼
	SetSndFile(SNDM0_0, ".\\Sound\\man\\mk0_00.wav", 3);	// 다이
	SetSndFile(SNDM0_1, ".\\Sound\\man\\mk0_01.wav", 1);	// 체크
	SetSndFile(SNDM0_2, ".\\Sound\\man\\mk0_02.wav", 4);	// 콜
	SetSndFile(SNDM0_3, ".\\Sound\\man\\mk0_03.wav", 4);	// 삥
	SetSndFile(SNDM0_4, ".\\Sound\\man\\mk0_04.wav", 4);	// 따당
	SetSndFile(SNDM0_5, ".\\Sound\\man\\mk0_05.wav", 3);	// 맥스

	SetSndFile(SNDM0_5_Q, ".\\Sound\\man\\mk0_05_Q.wav", 3);	// 맥스
	SetSndFile(SNDM0_6, ".\\Sound\\man\\mk0_allin.wav", 3);	// 올인

	//콜롬보
	SetSndFile(SNDM1_0, ".\\Sound\\man\\mk1_00.wav", 3);	// 다이
	SetSndFile(SNDM1_1, ".\\Sound\\man\\mk1_01.wav", 1);	// 체크
	SetSndFile(SNDM1_2, ".\\Sound\\man\\mk1_02.wav", 4);	// 콜
	SetSndFile(SNDM1_3, ".\\Sound\\man\\mk1_03.wav", 4);	// 삥
	SetSndFile(SNDM1_4, ".\\Sound\\man\\mk1_04.wav", 4);	// 따당
	SetSndFile(SNDM1_5, ".\\Sound\\man\\mk1_05.wav", 3);	// 맥스

	SetSndFile(SNDM1_5_Q, ".\\Sound\\man\\mk1_05_Q.wav", 3);	// 맥스
	SetSndFile(SNDM1_6, ".\\Sound\\man\\mk1_allin.wav", 3);	// 올인

	// 사용안함
	// 오리지날
	SetSndFile(SNDM2_0, ".\\Sound\\man\\mk2_00.wav", 3);	// 다이
	SetSndFile(SNDM2_1, ".\\Sound\\man\\mk2_01.wav", 1);	// 체크
	SetSndFile(SNDM2_2, ".\\Sound\\man\\mk2_02.wav", 4);	// 콜
	SetSndFile(SNDM2_3, ".\\Sound\\man\\mk2_03.wav", 4);	// 삥
	SetSndFile(SNDM2_4, ".\\Sound\\man\\mk2_04.wav", 4);	// 따당
	SetSndFile(SNDM2_5, ".\\Sound\\man\\mk2_05.wav", 3);	// 맥스	
	SetSndFile(SNDM2_5_Q, ".\\Sound\\man\\mk2_05_Q.wav", 3);	// 맥스
	SetSndFile(SNDM2_6, ".\\Sound\\man\\mk2_allin.wav", 3);	// 올인
	




	// ### 여성
	// 노멀
	SetSndFile(SNDF0_0, ".\\Sound\\woman\\fk0_00.wav", 3);	// 다이
	SetSndFile(SNDF0_1, ".\\Sound\\woman\\fk0_01.wav", 1);	// 체크
	SetSndFile(SNDF0_2, ".\\Sound\\woman\\fk0_02.wav", 4);	// 콜
	SetSndFile(SNDF0_3, ".\\Sound\\woman\\fk0_03.wav", 4);	// 삥
	SetSndFile(SNDF0_4, ".\\Sound\\woman\\fk0_04.wav", 4);	// 따당
	SetSndFile(SNDF0_5, ".\\Sound\\woman\\fk0_05.wav", 3);	// 맥스
	
	SetSndFile(SNDF0_5_Q, ".\\Sound\\woman\\fk0_05_Q.wav", 3);	// 맥스
	SetSndFile(SNDF0_6, ".\\Sound\\woman\\fk0_allin.wav", 3);	// 올인

	// 섹시버전
	SetSndFile(SNDF1_0, ".\\Sound\\woman\\fk1_00.wav", 3);	// 다이
	SetSndFile(SNDF1_1, ".\\Sound\\woman\\fk1_01.wav", 1);	// 체크
	SetSndFile(SNDF1_2, ".\\Sound\\woman\\fk1_02.wav", 4);	// 콜
	SetSndFile(SNDF1_3, ".\\Sound\\woman\\fk1_03.wav", 4);	// 삥
	SetSndFile(SNDF1_4, ".\\Sound\\woman\\fk1_04.wav", 4);	// 따당
	SetSndFile(SNDF1_5, ".\\Sound\\woman\\fk1_05.wav", 3);	// 맥스
	
	SetSndFile(SNDF1_5_Q, ".\\Sound\\woman\\fk1_05_Q.wav", 3);	// 맥스
	SetSndFile(SNDF1_6, ".\\Sound\\woman\\fk1_allin.wav", 3);	// 올인
	
	//사용안함
	// 오리지날
	SetSndFile(SNDF2_0, ".\\Sound\\woman\\fk2_00.wav", 3);	// 다이
	SetSndFile(SNDF2_1, ".\\Sound\\woman\\fk2_01.wav", 1);	// 체크
	SetSndFile(SNDF2_2, ".\\Sound\\woman\\fk2_02.wav", 4);	// 콜
	SetSndFile(SNDF2_3, ".\\Sound\\woman\\fk2_03.wav", 4);	// 삥
	SetSndFile(SNDF2_4, ".\\Sound\\woman\\fk2_04.wav", 4);	// 따당
	SetSndFile(SNDF2_5, ".\\Sound\\woman\\fk2_05.wav", 3);	// 맥스
	
	SetSndFile(SNDF2_5_Q, ".\\Sound\\woman\\fk2_05_Q.wav", 3);	// 맥스
	SetSndFile(SNDF2_6, ".\\Sound\\woman\\fk2_allin.wav", 3);	// 올인
	
	
	SetSndFile(SND16, ".\\Sound\\16.wav", 3);	// 대화 상자 오픈 카드 선택, 패 뒤집는 소리


	// 효과
	SetSndFile(SND20, ".\\Sound\\20.wav", 4);	// 시계 소리
	SetSndFile(SND21, ".\\Sound\\21.wav", 4);	// 카드 날리기 1
	SetSndFile(SND22, ".\\Sound\\22.wav", 3);	// 카드 날리기 2
	SetSndFile(SND23, ".\\Sound\\23.wav", 3);	// 카드 바닥에 붙는 소리
	SetSndFile(SND24, ".\\Sound\\24.wav", 3);	// 자기턴

	SetSndFile(SND25, ".\\Sound\\25.wav", 3);	// 칩1 Raise
	SetSndFile(SND26, ".\\Sound\\26.wav", 3);	// 칩2 콜
	SetSndFile(SND27, ".\\Sound\\27.wav", 3);	// 칩2 콜

  
	SetSndFile(SND30, ".\\Sound\\30.wav", 3);		// 파산
	SetSndFile(SND31, ".\\Sound\\JackPot.wav", 3);	// 잭팟 (스페셜)
	SetSndFile(SND32, ".\\Sound\\Win.wav", 3);		// 승
	SetSndFile(SND33, ".\\Sound\\Fail.wav", 3);		// 패

	// ### [ 관전기능 ] ###
	SetSndFile(SND34, ".\\Sound\\game_In.wav", 3);		// 게임 참여
	SetSndFile(SND35, ".\\Sound\\game_Out.wav", 3);		// 관전

	SetSndFile(SND36, ".\\Sound\\JackPotPang.wav", 3);	// 잭팟 팡파레
	

	// 공통 효과음
	SetSndFile(SND40, ".\\Sound\\40.wav", 1);		// 초대 알림
	SetSndFile(SND45, ".\\Sound\\45.wav", 1);		// 버튼 눌림 소리
	SetSndFile(SND50, ".\\Sound\\whisper.wav", 1);	// 귓말 알림
	
	// 음악(Wav 파일)
	SetWavMusicFile(MUSIC_WAV01, "Music\\music01.wav");	// 음악 1
	SetWavMusicFile(MUSIC_WAV02, "Music\\music02.wav");	// 음악 2
	SetWavMusicFile(MUSIC_WAV03, "Music\\music03.wav");	// 음악 3

	// 음악(MP3, MIDI 파일)
	SetMciMusicFile(MUSIC_MCI01,	"Music\\music01.mp3");		// MP3 음악 1
	SetMciMusicFile(MUSIC_MCI02,	"Music\\music02.mp3");		// MP3 음악 2

	// 음악(OGG 파일)
	SetOggMusicFile(MUSIC_OGG01,	"Music\\music01.ogg");		// OGG 음악 1
	SetOggMusicFile(MUSIC_OGG02,	"Music\\music02.ogg");		// OGG 음악 2


	SetSndFile(SND51, ".\\Sound\\turn.wav");	// 음악 3

	//added.
	SetSndFile(	SND_FBONUS, ".\\Sound\\add\\fbonus-game.wav");
	SetSndFile(	SND_FLOSE, ".\\Sound\\add\\flose.wav");
	SetSndFile(	SND_FWIN, ".\\Sound\\add\\fwin.wav");
	SetSndFile(	SND_GOODBOY, ".\\Sound\\add\\goodboy7poker.wav");
	SetSndFile(	SND_MBONUS, ".\\Sound\\add\\mbonus-game.wav");
	SetSndFile(	SND_MLOSE, ".\\Sound\\add\\mlose.wav");
	SetSndFile(	SND_MWIN, ".\\Sound\\add\\mwin.wav");
	SetSndFile(	SND_ONEMORE, ".\\Sound\\add\\onemore.wav");
	SetSndFile(	SND_DRAWBONUS, ".\\Sound\\add\\DrawSevenCard.wav");				// 드로우 보너스게임
	
	
	if(bInited == FALSE) return FALSE;

	return TRUE;
}

void CSound::Destroy()
{
	for(int i=0; i<MAX_SOUNDBUFFER; i++) SBuffer[i].Destroy();
	MBuffer.Destroy();
	MciMedia.Close();
	OggStream.Destroy();
}

void CSound::SetSndFile(SND_ENUM snd, LPCTSTR fname, int nPoly)
{
	SndFile[snd] = fname;
	SndPoly[snd] = nPoly;
}

void CSound::SetWavMusicFile(WAVMUSIC_ENUM mus, LPCTSTR fname)
{
	WavMusicFile[mus] = fname;
}

void CSound::SetMciMusicFile(MCIMUSIC_ENUM mus, LPCTSTR fname)
{
	MciMusicFile[mus] = fname;
}

void CSound::SetOggMusicFile(OGGMUSIC_ENUM mus, LPCTSTR fname)
{
	OggMusicFile[mus] = fname;
}

BOOL CSound::Play(SND_ENUM snd, BOOL bIsLoop, int volume, BOOL bIgnoreBufferFull)
{
	if(bDontUseSndFx==TRUE) return TRUE;
	if(snd<0 || snd>=MAX_SOUNDBUFFER) return FALSE;

	if(bInited) {
		// 버퍼가 생성되어 있지 않으면 생성함
		if(!SBuffer[snd].m_Buffer && SndFile[snd].GetLength() > 0) {
			if(DS_OK != SBuffer[snd].Create((char*)(LPCTSTR)SndFile[snd], SndPoly[snd])) 
				return FALSE;
		}
		if(bIsLoop) SBuffer[snd].Play(PLAY_LOOP, volume, 0, bIgnoreBufferFull);
		else SBuffer[snd].Play(PLAY_ONCE, volume, 0, bIgnoreBufferFull);
	}
	else {
		PlaySound((LPCTSTR)SndFile[snd], NULL, SND_ASYNC | SND_FILENAME | SND_NODEFAULT);
	}
	return TRUE;
}


BOOL CSound::PlayWavMusic(WAVMUSIC_ENUM music)
{
	if(!bInited) return FALSE;
	if(bDontUseMusic==TRUE) return TRUE;
	if(music<0 || music>=MAX_WAVMUSICFILE) return FALSE;

	if(!bInited) return FALSE;

	StopMusic();

	if(!MBuffer.m_Buffer) {
		if(DS_OK != MBuffer.Create((char*)(LPCTSTR)WavMusicFile[music], 1))
			return FALSE;
	}

	MBuffer.Play(PLAY_LOOP, 100);

	return TRUE;
}

BOOL CSound::PlayMciMusic(MCIMUSIC_ENUM music)
{
	if(!bInited) return FALSE;
	if(bDontUseMusic==TRUE) return TRUE;
	if(music<0 || music>=MAX_MCIMUSICFILE) return FALSE;

	if(!MciMedia.Open((LPCTSTR)MciMusicFile[music]))
		return FALSE;
	if(!MciMedia.Play(0, TRUE))
		return FALSE;

	return TRUE;
}

BOOL CSound::PlayOggMusic(OGGMUSIC_ENUM music, int volume, int panning)
{
	if(!bInited) return FALSE;
	if(bDontUseMusic==TRUE) return TRUE;
	if(music<0 || music>=MAX_OGGMUSICFILE) return FALSE;

	if(!bInited) return FALSE;

	StopMusic();

	if(!OggStream.Create((LPCTSTR)OggMusicFile[music]))
		return FALSE;

	OggStream.Play(PLAY_LOOP, volume, panning);

	return TRUE;
}

void CSound::SetSndFx(BOOL bUse)
{
	if(bUse==TRUE) bDontUseSndFx = FALSE;
	if(bUse==FALSE) bDontUseSndFx = TRUE;
}

void CSound::SetMusic(BOOL bUse)
{
	if(bUse==TRUE) bDontUseMusic = FALSE;
	if(bUse==FALSE) {
		bDontUseMusic = TRUE;
		StopMusic();
	}
}

void CSound::StopSound(SND_ENUM snd)
{
	if(bInited == FALSE) return;
	if(snd < 0 || snd >= MAX_SOUNDBUFFER) return;
	if(!SBuffer[snd].m_Buffer) return;
	SBuffer[snd].Stop();
}

void CSound::StopMusic()
{
	if(bInited == FALSE) return;
	
	MBuffer.Stop();
	MciMedia.Close();
	OggStream.Destroy();
}
