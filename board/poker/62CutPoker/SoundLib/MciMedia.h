// MciMedia.h: interface for the CMciMedia class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCIMEDIA_H__959B3FAD_6464_43AE_B3BF_458234988D3E__INCLUDED_)
#define AFX_MCIMEDIA_H__959B3FAD_6464_43AE_B3BF_458234988D3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMciMedia  
{

public:

	BOOL Open(LPCTSTR lpszFileName);	// 미디어를 오픈한다
	void  Close();						// 오픈된 미디어를 닫는다

	BOOL Play(DWORD dwStartPosition=0, BOOL bLoop=FALSE);	// 미디어 연주
	BOOL Pause();											// 미디어 일시정지
	BOOL Resume();											// 미디어 리줌
	BOOL Stop();											// 미디어 멈춤
	void SetLoop(BOOL bLoop) { m_bLoop=bLoop; }
	
	DWORD GetTotalLength();		// 전체 길이를 구한다
	DWORD GetTotalPosition();	// 전체 길이를 밀리초단위로 구한다
	DWORD GetPosition();		// 현재 포지션을 밀리초단위로 구한다

	BOOL IsReady();				// 준비상태 여부
	BOOL IsPlay();				// 재생중인지 여부
	BOOL IsPause();				// 일시머춤중인지 여부
	BOOL IsStop();				// 멈춤중인지 여부
	BOOL IsRecord();			// 기록중인지 여부

	void  SetVolume(DWORD dwVolume);	// 볼륨을 세팅한다
	DWORD GetVolume();					// 현재 볼륨값을 구한다
	DWORD GetStatus(DWORD dwFlag);		// 장치 상태를 구한다
	
	DWORD GetErrorCode() { return m_dwErrCode; }
	BOOL  GetErrorString(DWORD dwErrCode, LPCTSTR lpszError, UINT nSize);

	CMciMedia();
	virtual ~CMciMedia();

protected:

	UINT	m_nDeviceID;		// 디바이스 아이디
	HWND	m_hMsgWnd;			// 메시지 윈도우 핸들
	BOOL	m_bLoop;			// 반복여부
	DWORD	m_dwErrCode;		// 에러코드

	BOOL CreateMsgWindow();
	BOOL DestroyMsgWindow();

	static LRESULT CALLBACK MsgWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK DoMsgWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

};

#endif // !defined(AFX_MCIMEDIA_H__959B3FAD_6464_43AE_B3BF_458234988D3E__INCLUDED_)
