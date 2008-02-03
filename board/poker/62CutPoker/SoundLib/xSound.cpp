//////////////////////////////////////////////////////////////////////////////////
// Project Name:   [ xSound Class Library ]
// Author:         [ Jaehoon. Oh - 93XYZ@hitel.net ]
// Source File:    [ x3DSoundBuffer Implementation ]
// Revision:       [ 1.0 ]
//////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xSound.h"

#pragma comment(lib,"dsound.lib")  // Direct Sound Library
#pragma comment(lib,"winmm.lib")   // Windows Multi Media Library


// Melongs~! (Destroy가 귀찮아서 넣어놓은 거돠!)
xSound cSound;

// for static variable
BOOL					xSound::IsInitDSound = FALSE;
LPDIRECTSOUND			xSound::m_DirectSound = NULL;
LPDIRECTSOUNDBUFFER		xSound::m_Primary = NULL;
LPDIRECTSOUND3DLISTENER xSound::m_Listener = NULL;
LONG					xSound::m_lMasterVolume = 100;

// xSound Constructor
xSound::xSound()
{
	IsInitDSound = FALSE;
	m_DirectSound = NULL;
	m_Primary = NULL;
	m_Listener = NULL;
	m_lMasterVolume = 100;
}

// xSound Destructor
xSound::~xSound()
{
	Destroy();
}

// xSound Destructor
void xSound::Destroy()
{
	SAFE_RELEASE(m_Listener);
	SAFE_RELEASE(m_Primary);
	SAFE_RELEASE(m_DirectSound);
	IsInitDSound = FALSE;
}

// xSound Initialize
HRESULT xSound::Init(HWND hwnd)
{
	HRESULT rval;
	DSBUFFERDESC desc;

	rval = DirectSoundCreate(NULL, &m_DirectSound, NULL);
	if(rval != DS_OK) return rval;

	// MCI로 동영상 플레이시 DDSCL_EXCLUSIVE 모드로는 사운드 재생이 안됨
	//rval = m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_EXCLUSIVE);
	//rval = m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	rval = m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_NORMAL);
	if(rval != DS_OK) return rval;

	ZeroMemory(&desc, sizeof(DSBUFFERDESC));
	desc.dwSize = sizeof(DSBUFFERDESC);
	desc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;

	// Create the primary buffer
	rval = m_DirectSound->CreateSoundBuffer(&desc, &m_Primary, NULL);
	if(rval != DS_OK) return rval;

	// Obtain the listener interface
	rval = m_Primary->QueryInterface(IID_IDirectSound3DListener, (LPVOID*)&m_Listener);
	if(rval != DS_OK) return rval;

	IsInitDSound = TRUE;

	return rval;
}

LONG xSound::GetMasterVolume()
{
	return m_lMasterVolume;
}

void xSound::SetMasterVolume(LONG lVolume)
{
	if (lVolume > 100) lVolume = 100;
	if (lVolume <   0) lVolume =   0;

	m_lMasterVolume = lVolume;
}

//////////////////////////////////////////////////////////////////////////////////////////

// xSoundBuffer Constructor
xSoundBuffer::xSoundBuffer()
{
	m_WaveFile = NULL;
	m_Buffer = NULL;
	m_BufferCount = 1;
}

// xSoundBuffer Destructor
xSoundBuffer::~xSoundBuffer()
{
	Destroy();
}

// Destroy
void xSoundBuffer::Destroy()
{
	if(m_WaveFile)
	{
		delete m_WaveFile;
		m_WaveFile = NULL;
	}

	if(m_Buffer)
	{
		for(int i = 0; i < m_BufferCount; i++)
		{
			//if(m_Buffer[i]) m_Buffer[i]->Stop();
			SAFE_RELEASE(m_Buffer[i]);
		}
		delete m_Buffer;
		m_Buffer = NULL;
	}

}

// xSoundBuffer Create
HRESULT xSoundBuffer::Create(char* filename, int count)
{
	HRESULT rval = -1;
	LPVOID Audio = NULL;
	DWORD Bytes = 0;
	UINT BytesRead = 0;
	DSBUFFERDESC desc = {0,};
	int i = 0;

	Destroy();

	if(!GetDS()) return -1;

	// Create enough space for the buffers
	m_BufferCount = max(1, count);
	m_Buffer = new LPDIRECTSOUNDBUFFER[m_BufferCount];
	if(m_Buffer == NULL) return -1;
	for(i=0; i<m_BufferCount; i++) m_Buffer[i]=NULL;

	// Open the file and get ready to read
	m_WaveFile = new xWaveFile;
	if(m_WaveFile->Open(filename)==FALSE) 
		goto CREATE_ERROR;

	memset(&desc, 0, sizeof(DSBUFFERDESC));
	desc.dwSize = sizeof(DSBUFFERDESC);
	//desc.dwFlags = DSBCAPS_STATIC;
	desc.dwFlags = DSBCAPS_CTRLPAN|DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLFREQUENCY|DSBCAPS_LOCSOFTWARE;
	desc.dwBufferBytes = m_WaveFile->m_nDataSize;
	desc.lpwfxFormat = m_WaveFile->m_pFormat;

	rval = GetDS()->CreateSoundBuffer(&desc, &m_Buffer[0], NULL);
	if(rval != DS_OK) goto CREATE_ERROR;

	//rval = m_Buffer[0]->Lock(0, 0, &Audio, &Bytes, NULL, NULL, DSBLOCK_ENTIREBUFFER);
	rval = m_Buffer[0]->Lock(0, desc.dwBufferBytes, &Audio, &Bytes, NULL, NULL, DSBLOCK_ENTIREBUFFER);
	if(rval != DS_OK) goto CREATE_ERROR;
	if(Audio == NULL || Bytes == 0) {
		m_Buffer[0]->Unlock(Audio, Bytes, NULL, 0);
		goto CREATE_ERROR;
	}

	// Read the data from the file into the buffer
	BytesRead = m_WaveFile->Read((BYTE*)Audio, Bytes);

	rval = m_Buffer[0]->Unlock(Audio, Bytes, NULL, 0);
	if(rval != DS_OK) goto CREATE_ERROR;

	for(i = 1; i < m_BufferCount; i++)
		rval = m_DirectSound->DuplicateSoundBuffer(m_Buffer[0], &m_Buffer[i]);

	return rval;

CREATE_ERROR:

	Destroy();
	return rval;
}

// xSoundBuffer GetFreeBuffer
LPDIRECTSOUNDBUFFER xSoundBuffer::GetFreeBuffer(void)
{
	LPDIRECTSOUNDBUFFER Buffer = NULL;
	DWORD Status;
	int Current = 0;

	if(!m_Buffer) return NULL;

	while(Current < m_BufferCount)
	{
		Buffer = m_Buffer[Current];
		if(Buffer==NULL) break;
		if(Buffer->GetStatus(&Status) < 0) Status = 0;

		if((Status & DSBSTATUS_PLAYING) == DSBSTATUS_PLAYING)
		{
			Buffer = NULL;
			Current++;
		}
		else break;
	}

	if(Buffer && (Status & DSBSTATUS_BUFFERLOST))
	{
		if((Buffer->Restore() < 0)) Buffer = NULL;
	}

	return Buffer;
}

//#include "xMsg.h"
// xSoundBuffer Play
HRESULT xSoundBuffer::Play(BOOL bLoopFlag, LONG lVolume, LONG lPanning, BOOL bIgnoreBufferFull)
{
	DWORD flags = (bLoopFlag == PLAY_LOOP) ? DSBPLAY_LOOPING  : NULL;
	HRESULT rval;
	LPDIRECTSOUNDBUFFER Buffer = NULL;

	if(!IsInitDSound) return -1;
	if(!m_Buffer) return -1;

	Buffer = GetFreeBuffer();
	if(Buffer == NULL) {
		if(!m_Buffer || m_Buffer[0] == NULL) return -1;
		if(bIgnoreBufferFull == FALSE) return DS_OK;

		Buffer = m_Buffer[0];
		if(Buffer) {
			DWORD Status = 0;
			if(Buffer->GetStatus(&Status) < 0) 
				Status = 0;
			if((Status & DSBSTATUS_PLAYING) == DSBSTATUS_PLAYING) {
				Buffer->Stop();
				Buffer->SetCurrentPosition(0);
			}
		}
	}

	rval = SetVolume(Buffer, lVolume) ;
	//if (rval != TRUE) return -1;

	rval = SetPanning(Buffer, lPanning) ;
	//if (rval != TRUE) return -1;

	rval = Buffer->Play(0, 0, flags);
	if(rval != DS_OK) return rval;

	return rval;
}

// xSoundBuffer Stop
HRESULT xSoundBuffer::Stop()
{
	HRESULT rval;

	if(!IsInitDSound || !m_Buffer) return -1;

	for(int i = 0; i < m_BufferCount; i++)
	{
		if(m_Buffer[i]==NULL) continue;
		rval = m_Buffer[i]->Stop();
		if(rval != DS_OK) return rval;

		rval = m_Buffer[i]->SetCurrentPosition(0);
		if(rval != DS_OK) return rval;
	}

	return TRUE;
}

// Set Volume of Sound channel
BOOL xSoundBuffer::SetSoundVolume(LONG lChannelNum, LONG lVolume)
{
	if (lVolume > 100) lVolume = 100 ;
	if (lVolume <   0) lVolume =   0 ;

	//lVolume = (lVolume * GetMasterVolume()) - 10000 ;		// -10000 ~ 0
	lVolume = (LONG)((lVolume * 100) * (GetMasterVolume()/100.0f)) - 10000 ;		// -10000 ~ 0

	if(m_Buffer == NULL) return FALSE;
	if(lChannelNum >= m_BufferCount) return FALSE;
	if(m_Buffer[lChannelNum] == NULL) return FALSE;
	return (m_Buffer[lChannelNum]->SetVolume(lVolume) != DS_OK) ? FALSE : TRUE ;
}

BOOL xSoundBuffer::SetVolume(LPDIRECTSOUNDBUFFER Buffer, LONG lVolume)
{
	if (lVolume > 100) lVolume = 100 ;
	if (lVolume <   0) lVolume =   0 ;

	//lVolume = (lVolume * GetMasterVolume()) - 10000 ;		// -10000 ~ 0
	lVolume = (LONG)((lVolume * 100) * (GetMasterVolume()/100.0f)) - 10000 ;		// -10000 ~ 0

	//long vol = lVolume >> 2; 

	if(Buffer==NULL) return FALSE;
	return (Buffer->SetVolume(lVolume) != DS_OK) ? FALSE : TRUE ;
}

// Set panning of Sound channel
BOOL xSoundBuffer::SetSoundPanning(LONG lChannelNum, LONG lPanning)
{
	if (lPanning > 100) lPanning = 100 ;
	if (lPanning < -100) lPanning = -100 ;

	lPanning *= 100;  // lPanning = -10000 ~ 10000

	if(m_Buffer == NULL) return FALSE;
	if(lChannelNum >= m_BufferCount) return FALSE;
	if(m_Buffer[lChannelNum] == NULL) return FALSE;
	return (m_Buffer[lChannelNum]->SetPan(lPanning) != DS_OK) ? FALSE : TRUE ;
}

BOOL xSoundBuffer::SetPanning(LPDIRECTSOUNDBUFFER Buffer, LONG lPanning)
{
	if (lPanning > 100) lPanning = 100 ;
	if (lPanning < -100) lPanning = -100 ;

	lPanning *= 100;  // lPanning = -10000 ~ 10000

	if(Buffer==NULL) return FALSE;
	return (Buffer->SetPan(lPanning) != DS_OK) ? FALSE : TRUE ;
}

//////////////////////////////////////////////////////////////////////////////////


// x3DSoundBuffer Create
HRESULT x3DSoundBuffer::Create(char* filename, int count)
{
	if(!IsInitDSound) return -1;

	HRESULT rval = -1;
	LPVOID Audio = NULL;
	DWORD Bytes = 0;
	UINT BytesRead = 0;
	DSBUFFERDESC desc = {0,};
	int i = 0;

	Destroy();

	// Create enough space for the buffers
	m_BufferCount = count;
	m_Buffer = new LPDIRECTSOUNDBUFFER[((m_BufferCount-1) * sizeof(LPDIRECTSOUNDBUFFER))];
	if(m_Buffer == NULL) return -1;

	// Open the file and get ready to read
	m_WaveFile = new xWaveFile;
	m_WaveFile->Open(filename);

	memset(&desc, 0, sizeof(DSBUFFERDESC));
	desc.dwSize = sizeof(DSBUFFERDESC);
	//desc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_STATIC;
	desc.dwFlags = DSBCAPS_CTRL3D|DSBCAPS_CTRLPAN|DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLFREQUENCY|DSBCAPS_LOCSOFTWARE;
	desc.dwBufferBytes = m_WaveFile->m_mmckiData.cksize;
	desc.lpwfxFormat = m_WaveFile->m_pFormat;

	rval = GetDS()->CreateSoundBuffer(&desc, &m_Buffer[0], NULL);
	if(rval != DS_OK) goto CREATE_ERROR;

	rval = m_Buffer[0]->Lock(0, desc.dwBufferBytes, &Audio, &Bytes, NULL, NULL, DSBLOCK_ENTIREBUFFER);
	if(rval != DS_OK) goto CREATE_ERROR;
	if(Audio == NULL || Bytes == 0) {
		m_Buffer[0]->Unlock(Audio, Bytes, NULL, 0);
		goto CREATE_ERROR;
	}

	// Read the data from the file into the buffer
	BytesRead = m_WaveFile->Read((BYTE*)Audio, Bytes);

	rval = m_Buffer[0]->Unlock(Audio, Bytes, NULL, 0);
	if(rval != DS_OK) goto CREATE_ERROR;

	for(i = 1; i < m_BufferCount; i++)
		rval = m_DirectSound->DuplicateSoundBuffer(m_Buffer[0], &m_Buffer[i]);

	// Obtain the 3D interface
	rval = m_Buffer[0]->QueryInterface(IID_IDirectSound3DBuffer, (LPVOID*)&m_3dBuffer);
	if(rval != S_OK) goto CREATE_ERROR;

	return rval;

CREATE_ERROR:
	
	Destroy();
	return rval;
}


//////////////////////////////////////////////////////////////////////////////////////////

// Constructor
xStream::xStream(void)
{
	// Initialize data members
	m_pwavefile = NULL;
	m_pdsb = NULL;
	m_ptimer = NULL;
	m_fPlaying = m_fCued = FALSE;
	m_bIsLoop = FALSE;
	m_lVolume = 100;
	m_lPanning = 0;
	m_lInService = FALSE;
	m_cbBufOffset = 0;
	m_nBufLength = 2000;
	m_cbBufSize = 0;
	m_nBufService = 250;
	m_nDuration = 0;
	m_nTimeStarted = 0;
	m_nTimeElapsed = 0;
}

// Destructor
xStream::~xStream(void)
{
	Destroy();
}

// Create
BOOL xStream::Create(LPSTR pszFilename)
{
	BOOL fRtn = TRUE;

	if(!IsInitDSound) return FALSE;

	if(pszFilename)
	{
		// Create a new CWaveFile object
		if(m_pwavefile = new xWaveFile)
		{
			// Open given file
			if(m_pwavefile->Open(pszFilename))
			{
				// Calculate sound buffer size in bytes
				m_cbBufSize = (m_pwavefile->GetAvgDataRate() * m_nBufLength) / 1000;
				m_cbBufSize = (m_cbBufSize > m_pwavefile->GetDataSize()) ? m_pwavefile->GetDataSize() : m_cbBufSize;

				// Get duration of sound
				m_nDuration = m_pwavefile->GetDuration();

				// Create sound buffer
				HRESULT hr;
				memset(&m_dsbd, 0, sizeof (DSBUFFERDESC));
				m_dsbd.dwSize = sizeof (DSBUFFERDESC);
				m_dsbd.dwFlags = DSBCAPS_GETCURRENTPOSITION2|DSBCAPS_CTRLPAN|DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLFREQUENCY|DSBCAPS_LOCSOFTWARE;
				//m_dsbd.dwFlags = DSBCAPS_GETCURRENTPOSITION2|DSBCAPS_CTRLDEFAULT;
				m_dsbd.dwBufferBytes = m_cbBufSize;
				m_dsbd.lpwfxFormat = m_pwavefile->m_pFormat;

				hr = GetDS()->CreateSoundBuffer(&m_dsbd, &m_pdsb, NULL);
				if(hr == DS_OK)
				{
					// Cue for playback
					Cue();
				}
				else fRtn = FALSE;
			}
			else
			{
				// Error opening file
				delete m_pwavefile;
				m_pwavefile = NULL;
				fRtn = FALSE;
			}
		}
		else fRtn = FALSE;
	}
	else fRtn = FALSE;

	return fRtn;
}

// Destroy
BOOL xStream::Destroy(void)
{
	BOOL fRtn = TRUE;

	// Stop playback
	Stop();

	// Release DirectSound buffer
	SAFE_RELEASE(m_pdsb);

	// Delete CWaveFile object
	if(m_pwavefile)
	{
		delete(m_pwavefile);
		m_pwavefile = NULL;
	}

	return fRtn;
}

// WriteWaveData
BOOL xStream::WriteWaveData(UINT size)
{
	HRESULT hr;
	LPBYTE lpbuf1 = NULL;
	LPBYTE lpbuf2 = NULL;
	DWORD dwsize1 = 0;
	DWORD dwsize2 = 0;
	DWORD dwbyteswritten1 = 0;
	DWORD dwbyteswritten2 = 0;
	BOOL fRtn = TRUE;

	// Lock the sound buffer
	hr = m_pdsb->Lock(m_cbBufOffset, size, (LPVOID*)&lpbuf1, &dwsize1, (LPVOID*)&lpbuf2, &dwsize2, 0);
	if(hr == DS_OK)
	{
		// Write data to sound buffer
		if((dwbyteswritten1 = m_pwavefile->Read(lpbuf1, dwsize1)) == dwsize1)
		{
			// Second write required?
			if(lpbuf2)
			{
				if((dwbyteswritten2 = m_pwavefile->Read(lpbuf2, dwsize2)) == dwsize2)
				{
					// Both write operations successful!
				}
				else fRtn = FALSE;
			}
		}
		else fRtn = FALSE;

		// Update our buffer offset and unlock sound buffer
		m_cbBufOffset = (m_cbBufOffset + dwbyteswritten1 + dwbyteswritten2) % m_cbBufSize;
		m_pdsb->Unlock (lpbuf1, dwbyteswritten1, lpbuf2, dwbyteswritten2);
	}
	else fRtn = FALSE;

	return fRtn;
}

// WriteSilence
BOOL xStream::WriteSilence(UINT size)
{
	HRESULT hr;
	LPBYTE lpbuf1 = NULL;
	LPBYTE lpbuf2 = NULL;
	DWORD dwsize1 = 0;
	DWORD dwsize2 = 0;
	DWORD dwbyteswritten1 = 0;
	DWORD dwbyteswritten2 = 0;
	BOOL fRtn = TRUE;

	// Lock the sound buffer
	hr = m_pdsb->Lock(m_cbBufOffset, size, (LPVOID*)&lpbuf1, &dwsize1, (LPVOID*)&lpbuf2, &dwsize2, 0);
	if(hr == DS_OK)
	{
		BYTE bSilence = m_pwavefile->GetSilenceData();

		// Write silence to sound buffer
		memset (lpbuf1, bSilence, dwsize1);
		dwbyteswritten1 = dwsize1;

		// Second write required?
		if(lpbuf2)
		{
			memset(lpbuf2, bSilence, dwsize1);
			dwbyteswritten2 = dwsize2;
		}

		// Update our buffer offset and unlock sound buffer
		m_cbBufOffset = (m_cbBufOffset + dwbyteswritten1 + dwbyteswritten2) % m_cbBufSize;
		m_pdsb->Unlock(lpbuf1, dwbyteswritten1, lpbuf2, dwbyteswritten2);
	}
	else fRtn = FALSE;

	return fRtn;
}

// GetMaxWriteSize
DWORD xStream::GetMaxWriteSize(void)
{
	DWORD dwWriteCursor, dwPlayCursor, dwMaxSize;

	// Get current play position
	if(m_pdsb->GetCurrentPosition(&dwPlayCursor, &dwWriteCursor) == DS_OK)
	{
		if(m_cbBufOffset <= dwPlayCursor)
		{
			// Our write position trails play cursor
			dwMaxSize = dwPlayCursor - m_cbBufOffset;
		}
		else
		{
			// Play cursor has wrapped
			dwMaxSize = m_cbBufSize - m_cbBufOffset + dwPlayCursor;
		}
	}
	else
	{
		// GetCurrentPosition call failed
		dwMaxSize = 0;
	}

	return dwMaxSize;
}

// Service Buffer
BOOL xStream::ServiceBuffer(void)
{
	BOOL fRtn = TRUE;

	// Check for reentrance
	if(InterlockedExchange(&m_lInService, TRUE) == FALSE)
	{
		// Maintain elapsed time count
		m_nTimeElapsed = timeGetTime() - m_nTimeStarted;

		// Stop if all of sound has played
		if(m_nTimeElapsed < m_nDuration)
		{
			// All of sound not played yet, send more data to buffer
			DWORD dwFreeSpace = GetMaxWriteSize();

			// Determine free space in sound buffer
			if(dwFreeSpace)
			{
				// See how much wave data remains to be sent to buffer
				DWORD dwDataRemaining = m_pwavefile->GetNumBytesRemaining();
				if(dwDataRemaining == 0)
				{
					// All wave data has been sent to buffer
					if(WriteSilence(dwFreeSpace) == FALSE)
					{
						// Error writing silence data
						fRtn = FALSE;
					}
				}
				else if(dwDataRemaining >= dwFreeSpace)
				{
					// Enough wave data remains to fill free space in buffer
					if(WriteWaveData(dwFreeSpace) == FALSE)
					{
						// Error writing wave data
						fRtn = FALSE;
					}
				}
				else
				{
					// Some wave data remains, but not enough to fill free space
					if(WriteWaveData(dwDataRemaining) == TRUE)
					{
						if(WriteSilence(dwFreeSpace - dwDataRemaining) == FALSE)
						{
							// Error writing silence data
							fRtn = FALSE;
						}
					}
					else
					{
						// Error writing wave data
						fRtn = FALSE;
					}
				}
			}
			else
			{
				// No free space in buffer for some reason
				fRtn = FALSE;
			}
		}
		else
		{
			// All of sound has played
			Stop();
			if(m_bIsLoop) Play(PLAY_LOOP, m_lVolume, m_lPanning);
		}

		// Reset reentrancy semaphore
		InterlockedExchange(&m_lInService, FALSE);
	}
	else
	{
		// Service routine reentered. Do nothing
		fRtn = FALSE;
	}

	return fRtn;
}

// Cue
void xStream::Cue(void)
{
	if(!m_fCued)
	{
		// Reset buffer ptr
		m_cbBufOffset = 0;

		// Reset file ptr
		m_pwavefile->Cue();

		// Reset DirectSound buffer
		m_pdsb->SetCurrentPosition(0);

		// Fill buffer with wave data
		WriteWaveData(m_cbBufSize);

		m_fCued = TRUE;
	}
}

// Play
void xStream::Play(BOOL bLoopFlag, LONG lVolume, LONG lPanning)
{
	m_bIsLoop = bLoopFlag;

	if(!IsInitDSound) return;

	if(m_pdsb)
	{
		// Stop if playing
		if(m_fPlaying) Stop ();

		// Cue for playback if necessary
		if(!m_fCued) Cue ();
		
		SetVolume(lVolume);
		SetPanning(lPanning);
		
		// Begin DirectSound playback
		HRESULT hr = m_pdsb->Play(0, 0, DSBPLAY_LOOPING);
		if(hr == DS_OK)
		{
			m_nTimeStarted = timeGetTime();

			// Kick off timer to service buffer
			m_ptimer = new xMMTimer;
			if(m_ptimer)
			{
				m_ptimer->Create(m_nBufService, m_nBufService, (DWORD)(this), TimerCallback);
			}

			// Playback begun
			m_fPlaying = TRUE;
			m_fCued = FALSE;
		}
	}
}

// Stop
void xStream::Stop(void)
{
	if(m_fPlaying)
	{
		// Stop DirectSound playback
		if(IsInitDSound) m_pdsb->Stop();

		// Delete Timer object
		delete(m_ptimer);
		m_ptimer = NULL;
		m_fPlaying = FALSE;
	}
}

// Set Volume of Sound Stream
BOOL xStream::SetVolume(LONG lVolume)
{
	if (lVolume > 100) lVolume = 100;
	if (lVolume <   0) lVolume =   0;

	m_lVolume = lVolume;

	lVolume = (lVolume * GetMasterVolume()) - 10000;		// -10000 ~ 0

	return (m_pdsb->SetVolume(lVolume) != DS_OK) ? FALSE : TRUE;
}

// Set panning of Sound Stream
BOOL xStream::SetPanning(LONG lPanning)
{
	if (lPanning > 100) lPanning = 100;
	if (lPanning < -100) lPanning = -100;

	m_lPanning = lPanning;

	lPanning *= 100;  // lPanning = -10000 ~ 10000

	return (m_pdsb->SetPan(lPanning) != DS_OK) ? FALSE : TRUE;
}


// TimerCallback
BOOL xStream::TimerCallback(DWORD dwUser)
{
	// dwUser contains ptr to xStream object
	xStream* pas = (xStream*) dwUser;

	return pas->ServiceBuffer();
}


//////////////////////////////////////////////////////////////////////////////////


// Constructor
xMMTimer::xMMTimer(void)
{
	m_nIDTimer = NULL;
	m_pfnCallback = NULL;
}

// Destructor
xMMTimer::~xMMTimer(void)
{
	if(m_nIDTimer) timeKillEvent(m_nIDTimer);
	m_pfnCallback = NULL;
}

// Create
BOOL xMMTimer::Create(UINT nPeriod, UINT nRes, DWORD dwUser, TIMERCALLBACK pfnCallback)
{
	BOOL bRtn = TRUE;

	m_nPeriod = nPeriod;
	m_nRes = nRes;
	m_dwUser = dwUser;
	m_pfnCallback = pfnCallback;

	m_nIDTimer = timeSetEvent(m_nPeriod, m_nRes, TimeProc, (DWORD)this, TIME_PERIODIC);
	if(m_nIDTimer == NULL)
	{
		bRtn = FALSE;
	}

	return bRtn;
}

// TimeProc
void CALLBACK xMMTimer::TimeProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	// dwUser contains ptr to xMMTimer object
	xMMTimer* ptimer = (xMMTimer*) dwUser;

	if(IsBadReadPtr(ptimer, 4))
		return;

	// Call user-specified callback and pass back user specified data
	if(ptimer->m_pfnCallback)
		(ptimer->m_pfnCallback) (ptimer->m_dwUser);
}


//////////////////////////////////////////////////////////////////////////////////


// Constructor
xWaveFile::xWaveFile(void)
{
	// Initialise data members
	m_pFormat = NULL;
	m_hmmio = NULL;
	m_nBlockAlign= 0;
	m_nAvgDataRate = 0;
	m_nDataSize = 0;
	m_nBytesPlayed = 0;
	m_nDuration = 0;
	memset(&m_mmckiRiff, 0, sizeof (MMCKINFO));
	memset(&m_mmckiFmt, 0, sizeof (MMCKINFO));
	memset(&m_mmckiData, 0, sizeof (MMCKINFO));
}

// Destructor
xWaveFile::~xWaveFile(void)
{
	// Free memory
	if(m_pFormat) GlobalFree(m_pFormat);

	// Close file
	if(m_hmmio) mmioClose(m_hmmio, 0);
}

// Open
BOOL xWaveFile::Open(LPSTR pszFilename)
{
	WORD cbExtra = 0;
	BOOL fRtn = TRUE;

	// Open the requested file
	if((m_hmmio = mmioOpen(pszFilename, NULL, MMIO_ALLOCBUF | MMIO_READ)) == NULL)
	{
		m_mmr = MMIOERR_CANNOTOPEN;
		goto OPEN_ERROR;
	}

	// Descend into initial chunk ('RIFF')
	if(m_mmr = mmioDescend(m_hmmio, &m_mmckiRiff, NULL, 0))
	{
		goto OPEN_ERROR;
	}

	// Validate that it's a WAVE file
	if((m_mmckiRiff.ckid != FOURCC_RIFF) || (m_mmckiRiff.fccType != mmioFOURCC('W', 'A', 'V', 'E')))
	{
		m_mmr = MMIOERR_INVALIDFILE;
		goto OPEN_ERROR;
	}

	// Find format chunk ('fmt '), allocate and fill WAVEFORMATEX structure
	m_mmckiFmt.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if(m_mmr = mmioDescend(m_hmmio, &m_mmckiFmt, &m_mmckiRiff, MMIO_FINDCHUNK))
	{
		goto OPEN_ERROR;
	}

	// Read the format chunk into temporary structure
	PCMWAVEFORMAT pcmwf;
	if(mmioRead (m_hmmio, (CHAR *) &pcmwf, sizeof(PCMWAVEFORMAT)) != sizeof(PCMWAVEFORMAT))
	{
		m_mmr = MMIOERR_CANNOTREAD;
		goto OPEN_ERROR;
	}

	// If format is not PCM then there are extra bytes appended to WAVEFORMATEX
	if(pcmwf.wf.wFormatTag != WAVE_FORMAT_PCM)
	{
		// Read WORD specifying number of extra bytes
		if(mmioRead(m_hmmio, (LPSTR) &cbExtra, sizeof (cbExtra)) != sizeof(cbExtra))
		{
			m_mmr = MMIOERR_CANNOTREAD;
			goto OPEN_ERROR;
		}
	}

	// Allocate memory for WAVEFORMATEX structure + extra bytes
	if(m_pFormat = (WAVEFORMATEX *) GlobalAlloc(GMEM_FIXED, sizeof(WAVEFORMATEX)+cbExtra))
	{
		// Copy bytes from temporary format structure
		memcpy(m_pFormat, &pcmwf, sizeof(pcmwf));
		m_pFormat->cbSize = cbExtra;

		// Read those extra bytes and append to WAVEFORMATEX structure
		if(cbExtra != 0)
		{
			if((m_mmr = mmioRead(m_hmmio, (LPSTR) ((BYTE *)(m_pFormat) + sizeof (WAVEFORMATEX)), cbExtra)) != cbExtra)
			{
				// Error reading extra bytes
				m_mmr = MMIOERR_CANNOTREAD;
				goto OPEN_ERROR;
			}
		}
	}
  else
	{
		// Error allocating memory
		m_mmr = MMIOERR_OUTOFMEMORY;
		goto OPEN_ERROR;
	}

	// Init some member data from format chunk
	m_nBlockAlign = m_pFormat->nBlockAlign;
	m_nAvgDataRate = m_pFormat->nAvgBytesPerSec;

	// Ascend out of format chunk
	if(m_mmr = mmioAscend(m_hmmio, &m_mmckiFmt, 0))
	{
		goto OPEN_ERROR;
	}

	// Cue for streaming
	Cue();

	// Init some member data from data chunk
	m_nDataSize = m_mmckiData.cksize;
	m_nDuration = (UINT)(((__int64) m_nDataSize * 1000) / m_nAvgDataRate);

	// Successful open!
	goto OPEN_DONE;
	
OPEN_ERROR:

	fRtn = FALSE;
	if(m_hmmio)
	{
		// Close file
		mmioClose(m_hmmio, 0);
		m_hmmio = NULL;
	}
	if(m_pFormat)
	{
		// Free memory
		GlobalFree (m_pFormat);
		m_pFormat = NULL;
	}

OPEN_DONE:

	return fRtn;
}

// Cue
BOOL xWaveFile::Cue(void)
{
	BOOL fRtn = TRUE;

	// Seek to 'data' chunk from beginning of file
	if(mmioSeek (m_hmmio, m_mmckiRiff.dwDataOffset + sizeof(FOURCC), SEEK_SET) != -1)
	{
		// Descend into 'data' chunk
		m_mmckiData.ckid = mmioFOURCC('d', 'a', 't', 'a');
		if((m_mmr = mmioDescend(m_hmmio, &m_mmckiData, &m_mmckiRiff, MMIO_FINDCHUNK)) == MMSYSERR_NOERROR)
		{
			// Reset byte counter
			m_nBytesPlayed = 0;
		}
		else
		{
			fRtn = FALSE;
		}
	}
	else
	{
		// mmioSeek error
		m_mmr = MMIOERR_CANNOTSEEK;
		fRtn = FALSE;
	}

	return fRtn;
}

// Read
UINT xWaveFile::Read(BYTE * pbDest, UINT cbSize)
{
	MMIOINFO mmioinfo;
	UINT cb;

	// Use direct buffer access for reads to maximize performance
	if(m_mmr = mmioGetInfo(m_hmmio, &mmioinfo, 0))
	{
		goto READ_ERROR;
	}

	// Limit read size to chunk size
	cbSize = (cbSize > m_mmckiData.cksize) ? m_mmckiData.cksize : cbSize;

	// Adjust chunk size
	m_mmckiData.cksize -= cbSize;

	// Copy bytes from MMIO buffer
	for(cb = 0; cb < cbSize; cb++)
	{
		// Advance buffer if necessary
		if(mmioinfo.pchNext == mmioinfo.pchEndRead)
		{
			if(m_mmr = mmioAdvance(m_hmmio, &mmioinfo, MMIO_READ))
			{
				goto READ_ERROR;
			}

			if(mmioinfo.pchNext == mmioinfo.pchEndRead)
			{
				m_mmr = MMIOERR_CANNOTREAD;
				goto READ_ERROR;
			}
		}

		// Actual copy
		*(pbDest+cb) = *(mmioinfo.pchNext)++;
	}

	// End direct buffer access
	if(m_mmr = mmioSetInfo(m_hmmio, &mmioinfo, 0))
	{
		goto READ_ERROR;
	}

	// Successful read keep running total of number of data bytes read
	m_nBytesPlayed += cbSize;
	goto READ_DONE;

READ_ERROR:
	cbSize = 0;

READ_DONE:
	return cbSize;
}

// GetSilenceData
BYTE xWaveFile::GetSilenceData(void)
{
	BYTE bSilenceData = 0;

	// Silence data depends on format of Wave file
	if(m_pFormat)
	{
		if(m_pFormat->wBitsPerSample == 8)
		{
			// For 8-bit formats (unsigned, 0 to 255)
			// Packed DWORD = 0x80808080;
			bSilenceData = 0x80;
		}
		else if(m_pFormat->wBitsPerSample == 16)
		{
			// For 16-bit formats (signed, -32768 to 32767)
			// Packed DWORD = 0x00000000;
			bSilenceData = 0x00;
		}
	}

	return bSilenceData;
}

//-----------------------------------  xMidi Class ------------------------------

HWND xMidi::m_hWnd = NULL;
BOOL xMidi::m_isOK = FALSE;
char *xMidi::Filename[256];
BOOL xMidi::m_isLoop;


xMidi::xMidi()
{
	//m_isOK = FALSE;
}

BOOL xMidi::Init(HWND hwnd, char *fName)
{
	if(fName == NULL)
	{
		m_isOK = FALSE;
		return FALSE;
	}

	m_hWnd = hwnd;
	strcpy((char *)Filename, fName);
	m_isOK = TRUE;
	m_isLoop = FALSE;
	return TRUE;
}

BOOL xMidi::Play(BOOL isLoop)
{
	char buffer[256];
	
	if(m_isOK == FALSE) return FALSE;

	wsprintf(buffer, "open %s type sequencer alias MUSIC", Filename);

	if (mciSendString("close all", NULL, 0, NULL) != 0)
	{
		return(FALSE);
	}

	if (mciSendString(buffer, NULL, 0, NULL) != 0)
	{
		return(FALSE);
	}

	if (mciSendString("play MUSIC from 0 notify", NULL, 0, m_hWnd) != 0)
	{
		return(FALSE);
	}

	if(isLoop == PLAY_LOOP) m_isLoop = TRUE;
	else m_isLoop = FALSE;

	return TRUE;
}

BOOL xMidi::Replay()
{
	if(m_isOK == FALSE) return FALSE;

	if (mciSendString("play MUSIC from 0 notify", NULL, 0, m_hWnd) != 0)
	{
		return(FALSE);
	}
	return TRUE;
}



xMidi::Stop()
{
	if (mciSendString("close all", NULL, 0, NULL) != 0)
	{
		return(FALSE);
	}   

	return TRUE;
}

BOOL xMidi::Pause(void)
{
	// Pause if we're not already paused
	if (mciSendString("stop MUSIC", NULL, 0, NULL) != 0)
	{
		return(FALSE);
	}

	return TRUE;
}

BOOL xMidi::Resume(void)
{
	// Resume midi
	if (mciSendString("play MUSIC notify", NULL, 0, m_hWnd) != 0)
	{
		return(FALSE);
	}

	return TRUE;
}

BOOL xMidi::Restart(void)
{
	// Replay midi
	if (mciSendString("play MUSIC from 0 notify", NULL, 0, m_hWnd) != 0)
	{
		return(FALSE);
	}

	return TRUE;
}

// MM_MCINOTIFY 메세지를 받을 경우 리플레이 시킬지를 결정한다
void xMidi::OnMM_MCINOTIFY(WPARAM wFlags, LONG lDevID)
{
	if(wFlags==MCI_NOTIFY_SUCCESSFUL && m_isLoop==TRUE) Replay();
}

//-----------------------------------  xMusicCD Class ------------------------------

#define MAX_CD_TRACKS 50

short				xMusicCD::m_nNumberOfTracks;
short				xMusicCD::m_nTrackLength[100];
MCI_STATUS_PARMS	xMusicCD::m_MCIStatus;
MCI_OPEN_PARMS		xMusicCD::m_MCIOpen;

xMusicCD cMusicCD; // Destroy가 귀찮다.


// Default constructor.
xMusicCD::xMusicCD(void)
{
	m_MCIOpen.wDeviceID = 0;
	m_nNumberOfTracks = 0;
}

// Default constructor.
xMusicCD::~xMusicCD(void)
{
}

// Reads an audio CD.
short xMusicCD::Read(void)
{
	int i;
	short nTrackLength;

	m_nNumberOfTracks = 0;
	m_MCIOpen.lpstrDeviceType = (LPCTSTR)MCI_DEVTYPE_CD_AUDIO;

	if(mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE|MCI_OPEN_TYPE_ID, (DWORD)(LPVOID)&m_MCIOpen))
	{
		return 0;
	}	
		
	m_MCIStatus.dwItem = MCI_STATUS_NUMBER_OF_TRACKS;
	if(mciSendCommand(m_MCIOpen.wDeviceID, MCI_STATUS, MCI_STATUS_ITEM|MCI_WAIT, (DWORD)(LPVOID)&m_MCIStatus))
	{
		mciSendCommand(m_MCIOpen.wDeviceID, MCI_CLOSE, NULL, NULL);
		return 0;
	}

	m_nNumberOfTracks = (short)m_MCIStatus.dwReturn;
	if(m_nNumberOfTracks > MAX_CD_TRACKS) m_nNumberOfTracks = MAX_CD_TRACKS;
	m_MCIStatus.dwItem = MCI_STATUS_LENGTH;

	for(i = 0; i < m_nNumberOfTracks; i++)
	{
		m_MCIStatus.dwTrack = i + 1;
		mciSendCommand(m_MCIOpen.wDeviceID, MCI_STATUS, MCI_TRACK|MCI_STATUS_ITEM|MCI_WAIT,	(DWORD)(LPVOID)&m_MCIStatus);
		nTrackLength = (short)(MCI_MSF_MINUTE(m_MCIStatus.dwReturn)*60 + MCI_MSF_SECOND(m_MCIStatus.dwReturn));
		m_nTrackLength[i] = nTrackLength;
	}

	mciSendCommand(m_MCIOpen.wDeviceID, MCI_CLOSE, NULL, NULL);

	return m_nNumberOfTracks;
}

// Gets the current track length for a given track.
short	xMusicCD::GetTrackLength(short nTrack)
{
	if(nTrack > 0 && nTrack <= m_nNumberOfTracks) return m_nTrackLength[nTrack-1];
	else return 0;
}

// Sets the track length for each track on a CD.
void xMusicCD::SetTrackLength(short nTrack, short nNewLength)
{
	if(nTrack > 0 && nTrack <= m_nNumberOfTracks)
		m_nTrackLength[nTrack-1] = nNewLength;
}

// Gets the total length of the CD contents.
short xMusicCD::GetTotalLength(void)
{
	short nTotalLength = 0;
	short nTrack;

	for(nTrack = 0; nTrack < m_nNumberOfTracks; nTrack++)
		nTotalLength = (short)(nTotalLength + m_nTrackLength[nTrack]);

	return nTotalLength;
}

// Plays a track from a CD.
void xMusicCD::Play(short nTrack)
{
	MCI_SET_PARMS	mciSet;
	MCI_PLAY_PARMS mciPlay;

	m_MCIOpen.lpstrDeviceType = (LPCTSTR)MCI_DEVTYPE_CD_AUDIO;
	if(mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE|MCI_OPEN_TYPE_ID, (DWORD)&m_MCIOpen))
	{
		return;
	}

	// Set the time format to track/minute/second/frame (TMSF)
	mciSet.dwTimeFormat = MCI_FORMAT_TMSF;
	if(mciSendCommand(m_MCIOpen.wDeviceID, MCI_SET, MCI_SET_TIME_FORMAT, (DWORD)&mciSet))
	{
		mciSendCommand(m_MCIOpen.wDeviceID, MCI_CLOSE, 0, NULL);
		return;
	}

	mciPlay.dwCallback = 0;
	mciPlay.dwFrom = MCI_MAKE_TMSF(nTrack, 0, 0, 0);
	if(mciSendCommand(m_MCIOpen.wDeviceID, MCI_PLAY, MCI_FROM, (DWORD)&mciPlay))
	{
		return;
	}

	mciSendCommand(m_MCIOpen.wDeviceID, MCI_CLOSE, 0, NULL);
}

// Stops playing CD music.
void xMusicCD::Stop(void)
{
	if (m_MCIOpen.wDeviceID != -1)
	{
		mciSendCommand(m_MCIOpen.wDeviceID, MCI_CLOSE, NULL, NULL);
	}
}

