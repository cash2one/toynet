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
		// ���� ���� ��ư ����
	
// ### [���� �߰� �۾�] ###
	// ���� ���� ��ư ����
	// ### ����
	// ���
	SNDM0_0, // ���� 
	SNDM0_1, // üũ
	SNDM0_2, // ��
	SNDM0_3, // ��
	SNDM0_4, // ����
	SNDM0_5, // ����
	SNDM0_5_Q, //����
	SNDM0_6,//����

	// �ݷҺ� ����
	SNDM1_0, // ���� 
	SNDM1_1, // üũ
	SNDM1_2, // ��
	SNDM1_3, // ��
	SNDM1_4, // ����
	SNDM1_5, // ����
	SNDM1_5_Q, // ����
	SNDM1_6,//����

	// ������ ����
	SNDM2_0, // ���� 
	SNDM2_1, // üũ
	SNDM2_2, // ��
	SNDM2_3, // ��
	SNDM2_4, // ����
	SNDM2_5, // �ƽ�
	SNDM2_5_Q,//����
	SNDM2_6,//����



	// ### ����
	SNDF0_0, // ���� 
	SNDF0_1, // üũ
	SNDF0_2, // ��
	SNDF0_3, // ��
	SNDF0_4, // ����
	SNDF0_5, // �ƽ�
	SNDF0_5_Q, // �ƽ�
	SNDF0_6,//����

	// ���� ����
	SNDF1_0, // ���� 
	SNDF1_1, // üũ
	SNDF1_2, // ��
	SNDF1_3, // ��
	SNDF1_4, // ����
	SNDF1_5, // �ƽ�
	SNDF1_5_Q, // �ƽ�
	SNDF1_6,//����
	// original
	SNDF2_0, // ���� 
	SNDF2_1, // üũ
	SNDF2_2, // ��
	SNDF2_3, // ��
	SNDF2_4, // ����
	SNDF2_5, // �ƽ�
	SNDF2_5_Q, // �ƽ�
	SNDF2_6,//����

	SND16, // ��ȭ ���� ���� ī�� ����, �� ������ �Ҹ�
	SND17, // ���ӿ��� ����
	SND18, //		   �ο�	
	SND19, //		   ����
	

	// ȿ��
	SND20, // �ð� �Ҹ�
	SND21, // ī�� ������ 1
	SND22, // ī�� ������ 2
	SND23, // ī�� �ٴڿ� �ٴ� �Ҹ�
	SND24, // �ڱ���

	// Ĩ
	SND25, // ��	Ĩ1 Raise
	SND26, // ����	Ĩ2 ��
	SND27, // �ƽ�  Ĩ

	SND30, // �Ļ�
	SND31, // ���� (�����)
	SND32, // ��
	SND33, // ��
	
	// ### [ ������� ] ###
	SND34, // ���� ����
	SND35, // ����

	SND36, // ���� ���ķ�
	
	// ���� ȿ����
	SND40, // �ʴ� �˸�
	SND45, // ��ư ���� �Ҹ�
	SND50, // �Ӹ� �˸�

	SND51, // �ٲٱ��� �Ҹ�[62]

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

	// Wav ����Ʈ
	CString			SndFile[MAX_SOUNDBUFFER];
	xSoundBuffer	SBuffer[MAX_SOUNDBUFFER];
	int				SndPoly[MAX_SOUNDBUFFER];

	// Wav ����
	CString			WavMusicFile[MAX_WAVMUSICFILE];
	xSoundBuffer	MBuffer;
	
	// Mci �̵�� ����
	CString			MciMusicFile[MAX_MCIMUSICFILE];
	CMciMedia		MciMedia;

	// Wav ����
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
