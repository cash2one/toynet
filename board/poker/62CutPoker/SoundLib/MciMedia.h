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

	BOOL Open(LPCTSTR lpszFileName);	// �̵� �����Ѵ�
	void  Close();						// ���µ� �̵� �ݴ´�

	BOOL Play(DWORD dwStartPosition=0, BOOL bLoop=FALSE);	// �̵�� ����
	BOOL Pause();											// �̵�� �Ͻ�����
	BOOL Resume();											// �̵�� ����
	BOOL Stop();											// �̵�� ����
	void SetLoop(BOOL bLoop) { m_bLoop=bLoop; }
	
	DWORD GetTotalLength();		// ��ü ���̸� ���Ѵ�
	DWORD GetTotalPosition();	// ��ü ���̸� �и��ʴ����� ���Ѵ�
	DWORD GetPosition();		// ���� �������� �и��ʴ����� ���Ѵ�

	BOOL IsReady();				// �غ���� ����
	BOOL IsPlay();				// ��������� ����
	BOOL IsPause();				// �Ͻø��������� ����
	BOOL IsStop();				// ���������� ����
	BOOL IsRecord();			// ��������� ����

	void  SetVolume(DWORD dwVolume);	// ������ �����Ѵ�
	DWORD GetVolume();					// ���� �������� ���Ѵ�
	DWORD GetStatus(DWORD dwFlag);		// ��ġ ���¸� ���Ѵ�
	
	DWORD GetErrorCode() { return m_dwErrCode; }
	BOOL  GetErrorString(DWORD dwErrCode, LPCTSTR lpszError, UINT nSize);

	CMciMedia();
	virtual ~CMciMedia();

protected:

	UINT	m_nDeviceID;		// ����̽� ���̵�
	HWND	m_hMsgWnd;			// �޽��� ������ �ڵ�
	BOOL	m_bLoop;			// �ݺ�����
	DWORD	m_dwErrCode;		// �����ڵ�

	BOOL CreateMsgWindow();
	BOOL DestroyMsgWindow();

	static LRESULT CALLBACK MsgWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK DoMsgWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

};

#endif // !defined(AFX_MCIMEDIA_H__959B3FAD_6464_43AE_B3BF_458234988D3E__INCLUDED_)
