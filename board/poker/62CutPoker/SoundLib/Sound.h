// Sound.h: interface for the CSound class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUND_H__EF4CEDCE_DF40_49DC_BF81_995C5AC5DBF3__INCLUDED_)
#define AFX_SOUND_H__EF4CEDCE_DF40_49DC_BF81_995C5AC5DBF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "xSound.h"
#include "MciMedia.h"
#include "xOgg.h"

enum SND_ENUM
{
		// 게임 진행 버튼 관련
	
// ### [사운드 추가 작업] ###
	// 게임 진행 버튼 관련
	// ### 남성
	// 노멀
	SNDM0_0, // 다이 
	SNDM0_1, // 체크
	SNDM0_2, // 콜
	SNDM0_3, // 삥
	SNDM0_4, // 따당
	SNDM0_5, // 하프
	SNDM0_5_Q, //쿼터
	SNDM0_6,//올인

	// 콜롬보 버젼
	SNDM1_0, // 다이 
	SNDM1_1, // 체크
	SNDM1_2, // 콜
	SNDM1_3, // 삥
	SNDM1_4, // 따당
	SNDM1_5, // 하프
	SNDM1_5_Q, // 쿼터
	SNDM1_6,//올인

	// 점잖은 버젼
	SNDM2_0, // 다이 
	SNDM2_1, // 체크
	SNDM2_2, // 콜
	SNDM2_3, // 삥
	SNDM2_4, // 따당
	SNDM2_5, // 맥스
	SNDM2_5_Q,//쿼터
	SNDM2_6,//올인



	// ### 여성
	SNDF0_0, // 다이 
	SNDF0_1, // 체크
	SNDF0_2, // 콜
	SNDF0_3, // 삥
	SNDF0_4, // 따당
	SNDF0_5, // 맥스
	SNDF0_5_Q, // 맥스
	SNDF0_6,//올인

	// 섹시 버젼
	SNDF1_0, // 다이 
	SNDF1_1, // 체크
	SNDF1_2, // 콜
	SNDF1_3, // 삥
	SNDF1_4, // 따당
	SNDF1_5, // 맥스
	SNDF1_5_Q, // 맥스
	SNDF1_6,//올인
	// original
	SNDF2_0, // 다이 
	SNDF2_1, // 체크
	SNDF2_2, // 콜
	SNDF2_3, // 삥
	SNDF2_4, // 따당
	SNDF2_5, // 맥스
	SNDF2_5_Q, // 맥스
	SNDF2_6,//올인

	SND16, // 대화 상자 오픈 카드 선택, 패 뒤집는 소리
	SND17, // 게임오버 하이
	SND18, //		   로우	
	SND19, //		   스윙
	

	// 효과
	SND20, // 시계 소리
	SND21, // 카드 날리기 1
	SND22, // 카드 날리기 2
	SND23, // 카드 바닥에 붙는 소리
	SND24, // 자기턴

	// 칩
	SND25, // 삥	칩1 Raise
	SND26, // 따당	칩2 콜
	SND27, // 맥스  칩

	SND30, // 파산
	SND31, // 잭팟 (스페셜)
	SND32, // 승
	SND33, // 패
	
	// ### [ 관전기능 ] ###
	SND34, // 게임 참여
	SND35, // 관전

	SND36, // 잭팟 팡파레
	
	// 공통 효과음
	SND40, // 초대 알림
	SND45, // 버튼 눌림 소리
	SND50, // 귓말 알림

	SND51, // 바꾸기턴 소리[62]

	SND_FBONUS,
	SND_FLOSE,
	SND_FWIN,
	SND_GOODBOY,
	SND_MBONUS,
	SND_MLOSE,
	SND_MWIN,
	SND_ONEMORE,
	
	MAX_SOUNDBUFFER
};

enum WAVMUSIC_ENUM
{
	MUSIC_WAV01,
	MUSIC_WAV02,
	MUSIC_WAV03,
	MAX_WAVMUSICFILE
};

enum MCIMUSIC_ENUM
{
	MUSIC_MCI01,
	MUSIC_MCI02,
	MUSIC_MCI03,
	MAX_MCIMUSICFILE
};

enum OGGMUSIC_ENUM
{
	MUSIC_OGG01,
	MUSIC_OGG02,
	MUSIC_OGG03,
	MAX_OGGMUSICFILE
};
class CSound  
{
public:

	BOOL			bInited;
	int				InitErrorKind;

	BOOL			bDontUseSndFx;
	BOOL			bDontUseMusic;

	// Wav 이펙트
	CString			SndFile[MAX_SOUNDBUFFER];
	xSoundBuffer	SBuffer[MAX_SOUNDBUFFER];
	int				SndPoly[MAX_SOUNDBUFFER];

	// Wav 음악
	CString			WavMusicFile[MAX_WAVMUSICFILE];
	xSoundBuffer	MBuffer;
	
	// Mci 미디어 음악
	CString			MciMusicFile[MAX_MCIMUSICFILE];
	CMciMedia		MciMedia;

	// Wav 음악
	CString			OggMusicFile[MAX_OGGMUSICFILE];
	xOggStream		OggStream;
	

	CSound();
	virtual ~CSound();

	BOOL Init(HWND hWnd);
	void Destroy();
	void SetSndFile(SND_ENUM snd, LPCTSTR fname, int nPoly = 1);
	void SetWavMusicFile(WAVMUSIC_ENUM mus, LPCTSTR fname);
	void SetMciMusicFile(MCIMUSIC_ENUM mus, LPCTSTR fname);
	void SetOggMusicFile(OGGMUSIC_ENUM mus, LPCTSTR fname);

	BOOL Play(SND_ENUM snd, BOOL bIsLoop=FALSE, int volume=100, BOOL bIgnoreBufferFull=TRUE);
	BOOL PlayWavMusic(WAVMUSIC_ENUM music);
	BOOL PlayMciMusic(MCIMUSIC_ENUM music);
	BOOL PlayOggMusic(OGGMUSIC_ENUM music, int volume=100, int panning = 0);

	void SetSndFx(BOOL bUse);
	void SetMusic(BOOL bUse);

	void StopSound(SND_ENUM snd);
	void StopMusic();
};


#endif // !defined(AFX_SOUND_H__EF4CEDCE_DF40_49DC_BF81_995C5AC5DBF3__INCLUDED_)
