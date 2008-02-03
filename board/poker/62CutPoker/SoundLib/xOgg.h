// xOgg.h
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XOGG_H__DFE25FAC_FB9A_49F1_A092_294748F2785E__INCLUDED_)
#define AFX_XOGG_H__DFE25FAC_FB9A_49F1_A092_294748F2785E__INCLUDED_

#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "xSound.h"
#include "codec.h"
#include "vorbisfile.h"


//////////////////////////////////////////////////////////////////////////////////////


#pragma pack( push, 1 )

struct SOUND_DESC
{

	ULONG			size;
	FILE			*pt;
	ULONG			start_position;
	OggVorbis_File	vf;
	vorbis_info		*vi;
	int				ogg_current_section;

	SOUND_DESC()
	{
		Reset();
	}

	void Reset()
	{
		size = 0;
		pt = NULL;
		start_position = 0;
		vi = NULL;
		ogg_current_section = 0;
	}
};

typedef struct tag_wav_fourcc
{
	unsigned char	fourcc[4];
	unsigned int	size;
} wave_fourcc;

#pragma pack( pop )


//////////////////////////////////////////////////////////////////////////////////
// xOggFile Class
//////////////////////////////////////////////////////////////////////////////////

class xOggFile
{
public:
	xOggFile();
	~xOggFile();

	BOOL Open(LPCTSTR pszFilename);
	BOOL Cue(void);
	UINT Read(BYTE* pbDest, UINT cbSize);

	UINT GetNumBytesRemaining(void) { return (m_nDataSize - m_nBytesPlayed); }
	UINT GetAvgDataRate(void)       { return (m_nAvgDataRate); }
	UINT GetDataSize(void)          { return (m_nDataSize); }
	UINT GetNumBytesPlayed(void)    { return (m_nBytesPlayed); }
	UINT GetDuration(void)          { return (m_nDuration); }
	BYTE GetSilenceData(void);

public:
	WAVEFORMATEX* m_pFormat;
	UINT m_nDuration;
	UINT m_nBlockAlign;
	UINT m_nAvgDataRate;
	UINT m_nDataSize;
	UINT m_nBytesPlayed;

	SOUND_DESC		m_desc;

	BOOL m_bEnteredLoop;
	UINT* m_pExitLoopFlag;
};


//////////////////////////////////////////////////////////////////////////////////
// xOggStream Class
//////////////////////////////////////////////////////////////////////////////////

class xOggStream : public xSound
{
public:
	xOggStream();
	~xOggStream();

	BOOL Create(LPCTSTR pszFilename);
	BOOL Destroy(void);
	void Play(BOOL bLoopFlag = PLAY_ONCE, LONG lVolume = 100, LONG lPanning = 0);
	void Stop(void);

	BOOL SetVolume(LONG lVolume);
	BOOL SetPanning(LONG lPanning);

public:
	void Cue(void);
	BOOL WriteWaveData(UINT cbSize);
	BOOL WriteSilence(UINT cbSize);
	BOOL ServiceBuffer(void);
	DWORD GetMaxWriteSize(void);
	static BOOL TimerCallback(DWORD dwUser);

	xOggFile* m_poggfile;
	xMMTimer* m_ptimer;

	LPDIRECTSOUNDBUFFER m_pdsb;
	DSBUFFERDESC m_dsbd;
	BOOL m_fCued;
	BOOL m_fPlaying;
	BOOL m_bIsLoop;
	LONG m_lVolume;
	LONG m_lPanning;
	LONG m_lInService;
	UINT m_cbBufOffset;
	UINT m_nBufLength;
	UINT m_cbBufSize;
	UINT m_nBufService;
	UINT m_nDuration;
	UINT m_nTimeStarted;
	UINT m_nTimeElapsed;

public:

	CRITICAL_SECTION m_CriticalSection;

	inline void Lock() { EnterCriticalSection(&m_CriticalSection); }
	inline void Unlock() { LeaveCriticalSection(&m_CriticalSection); }
};


#endif // !defined(AFX_XOGG_H__DFE25FAC_FB9A_49F1_A092_294748F2785E__INCLUDED_)
