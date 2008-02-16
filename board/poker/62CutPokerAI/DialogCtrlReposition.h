// DialogCtrlReposition.h: interface for the CDialogCtrlReposition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOGCTRLREPOSITION_H__F84575AC_EA17_4846_B9FF_1FF87A6F4CBF__INCLUDED_)
#define AFX_DIALOGCTRLREPOSITION_H__F84575AC_EA17_4846_B9FF_1FF87A6F4CBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDialogCtrlReposition  
{
public:
	CRect rcBase1, rcBase2;

	float fWRate;
	float fHRate;

	CDialogCtrlReposition();
	virtual ~CDialogCtrlReposition();

	void SetBaseRect(CRect &rc1, CRect &rc2);
	void SetRate(CRect &rc1, CRect &rc2);
	CRect GetRect(CRect &rc);
	void Reposition(HWND hDlg, BOOL bReposDlg=TRUE);
	void CtrlPos(HWND hWnd, CRect &rc);

	// Enum Child Window 콜백 프로시져
	static BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam);
};

// for static function
static BOOL CALLBACK CDialogCtrlReposition::EnumChildProc(HWND hwnd, LPARAM lParam);

#endif // !defined(AFX_DIALOGCTRLREPOSITION_H__F84575AC_EA17_4846_B9FF_1FF87A6F4CBF__INCLUDED_)
