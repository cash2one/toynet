#ifndef _XSOUND_H_
#define _XSOUND_H_

#include <windows.h>
#include <mmsystem.h>
#include "dsound.h"
#include "windowsx.h"


//////////////////////////////////////////////////////////////////////////////////
// Definied Types
//////////////////////////////////////////////////////////////////////////////////

#define SAFE_RELEASE(x) if(x != NULL) { x->Release(); x = NULL; }
#define SAFE_DELETE(x) if(x != NULL) { delete x; x = NULL; }

typedef BOOL (*TIMERCALLBACK)(DWORD);

#define PLAY_ONCE 0
#define PLAY_LOOP 1

//////////////////////////////////////////////////////////////////////////////////
// xSound Class
//////////////////////////////////////////////////////////////////////////////////

class xSound
{
public:
	xSound(void);
	~xSound(void);
	static void Destroy();

	static HRESULT Init(HWND hwnd);

	static LPDIRECTSOUND GetDS(void) { return m_DirectSound; }
	static LPDIRECTSOUNDBUFFER GetPrimary(void) { return m_Primary; }
	static LPDIRECTSOUND3DLISTENER GetListener(void) { return m_Listener; }

	static LONG GetMasterVolume();
	static void SetMasterVolume(LONG lVolume);

public:
	static BOOL IsInitDSound;
	static LPDIRECTSOUND m_DirectSound;
	static LPDIRECTSOUNDBUFFER m_Primary;
	static LPDIRECTSOUND3DLISTENER m_Listener;
	static LONG m_lMasterVolume;
};

//////////////////////////////////////////////////////////////////////////////////
// xWaveFile Class
//////////////////////////////////////////////////////////////////////////////////

class xWaveFile
{
public:
	xWaveFile(void);
	~xWaveFile(void);

	BOOL Open(LPSTR pszFilename);
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
	HMMIO m_hmmio;
	MMRESULT m_mmr;
	MMCKINFO m_mmckiRiff;
	MMCKINFO m_mmckiFmt;
	MMCKINFO m_mmckiData;
	UINT m_nDuration;
	UINT m_nBlockAlign;
	UINT m_nAvgDataRate;
	UINT m_nDataSize;
	UINT m_nBytesPlayed;
};

//////////////////////////////////////////////////////////////////////////////////
// xSoundBuffer Class
//////////////////////////////////////////////////////////////////////////////////

class xSoundBuffer : public xSound
{
public:
	xSoundBuffer(void);
	~xSoundBuffer(void);

	virtual HRESULT		Create(char* filename, int count);
	void				Destroy();

	HRESULT				Play(BOOL bLoopFlag = PLAY_ONCE, LONG lVolume = 100, LONG lPanning = 0, BOOL bIgnoreBufferFull=TRUE);
	HRESULT				Stop(void);
	LPDIRECTSOUNDBUFFER GetFreeBuffer(void);

	BOOL				SetSoundVolume(LONG lChannelNum, LONG lVolume);
	BOOL				SetVolume(LPDIRECTSOUNDBUFFER Buffer, LONG lVolume);
	BOOL				SetSoundPanning(LONG lChannelNum, LONG lPanning);
	BOOL				SetPanning(LPDIRECTSOUNDBUFFER Buffer, LONG lPanning);

public:
	int					m_BufferCount;
	xWaveFile			*m_WaveFile;
	LPDIRECTSOUNDBUFFER *m_Buffer;
};

//////////////////////////////////////////////////////////////////////////////////
// x3DSoundBuffer Class
//////////////////////////////////////////////////////////////////////////////////

class x3DSoundBuffer : public xSoundBuffer
{
public:
	HRESULT Create(char* filename, int count);
	LPDIRECTSOUND3DBUFFER m_3dBuffer;
};

//////////////////////////////////////////////////////////////////////////////////
// xMMTimer Class
//////////////////////////////////////////////////////////////////////////////////

class xMMTimer
{
public:
	xMMTimer(void);
	~xMMTimer(void);

	BOOL Create(
		UINT nPeriod,
		UINT nRes,
		DWORD dwUser,
		TIMERCALLBACK pfnCallback);

protected:
	static void CALLBACK TimeProc(
		UINT uID,
		UINT uMsg,
		DWORD dwUser,
		DWORD dw1,
		DWORD dw2);

	TIMERCALLBACK m_pfnCallback;
	DWORD m_dwUser;
	UINT m_nPeriod;
	UINT m_nRes;
	UINT m_nIDTimer;
};

//////////////////////////////////////////////////////////////////////////////////
// xStream Class
//////////////////////////////////////////////////////////////////////////////////

class xStream : public xSound
{
public:
	xStream(void);
	~xStream(void);

	BOOL Create(LPSTR pszFilename);
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

	xWaveFile* m_pwavefile;
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
};


//////////////////////////////////////////////////////////////////////////////////
// xMidi Class
//////////////////////////////////////////////////////////////////////////////////

class xMidi
{
public:
    xMidi();

	static BOOL Init(HWND hwnd, char *fName);
	static BOOL Play(BOOL isLoop=FALSE);
	static BOOL Replay(void);
	static BOOL Stop(void);
	static BOOL Pause(void);
	static BOOL Resume(void);
	static BOOL Restart(void);
	static void OnMM_MCINOTIFY(WPARAM wFlags, LONG lDevID);

private:
	static HWND m_hWnd;
	static BOOL m_isOK;
	static char *Filename[256];
public:
	static BOOL m_isLoop;
};

//////////////////////////////////////////////////////////////////////////////////
// xMusicCD allows you to load and play audio CD files for the music in your game. 
// xMusicCD currently uses the Windows MCI functions and in terms of performance 
// is quite demanding.
//////////////////////////////////////////////////////////////////////////////////
class xMusicCD
{
public:
	xMusicCD(void);
	~xMusicCD(void);

	static short	Read(void);
	static short	GetNumberOfTracks(void)	{ return m_nNumberOfTracks; }
	static void  SetNumberOfTracks(short nTracks) { m_nNumberOfTracks = nTracks; }
	static short	GetTrackLength(short nTrack);
	static void  SetTrackLength(short nTrack, short nNewLength);
	static short	GetTotalLength(void);
	static void  Play(short nTrack);
	static void  Stop(void);

//private:
	static short	m_nNumberOfTracks;
	static short	m_nTrackLength[100];
	static MCI_STATUS_PARMS m_MCIStatus;
	static MCI_OPEN_PARMS m_MCIOpen;
};


#endif