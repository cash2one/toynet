// DialogCtrlReposition.cpp: implementation of the CDialogCtrlReposition class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DialogCtrlReposition.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDialogCtrlReposition::CDialogCtrlReposition()
{
	// Default ���̽� Rect�� ����ü ������9 Ȥ�� MS Sans Sefif ������9�� ���������Ѵ�
	rcBase1.SetRect(0, 0, 1000, 1000);
	rcBase2.SetRect(0, 0, 1750, 1500);

	SetRate(rcBase1, rcBase2);
}

CDialogCtrlReposition::~CDialogCtrlReposition()
{

}

void CDialogCtrlReposition::SetBaseRect(CRect &rc1, CRect &rc2)
{
	rcBase1 = rc1;
	rcBase2 = rc2;
}

void CDialogCtrlReposition::SetRate(CRect &rc1, CRect &rc2)
{
	float bw = rcBase2.Width() / (float)max(1, rcBase1.Width());
	float bh = rcBase2.Height() / (float)max(1, rcBase1.Height());

	float rw = rc2.Width() / (float)max(1, rc1.Width());
	float rh = rc2.Height() / (float)max(1, rc1.Height());

	if(rw == 0) rw = 1.0f;
	if(rh == 0) rh = 1.0f;

	fWRate = bw / rw;
	fHRate = bh / rh;
}

CRect CDialogCtrlReposition::GetRect(CRect &rc)
{
	CRect rcRtn;
	rcRtn.left = (int)(rc.left * fWRate);
	rcRtn.top = (int)(rc.top * fHRate);
	rcRtn.right = (int)(rc.right * fWRate);
	rcRtn.bottom = (int)(rc.bottom * fHRate);
	return rcRtn;
}

void CDialogCtrlReposition::Reposition(HWND hDlg, BOOL bReposDlg)
{
	if(!hDlg) return;

	CRect rc1(0, 0, 1000, 1000);
	CRect rc2(0, 0, 1000, 1000);

	if(!::MapDialogRect(hDlg, (LPRECT)rc2)) return;

	SetRate(rc1, rc2);

	// ������ �ʿ䰡 ������ ����
	if(fWRate == 1.0f && fHRate == 1.0f) return;

	// ���̾�α��� ����� �����ؾ� �Ѵٸ�
	if(bReposDlg) {
		CRect rc, rcClient;
		if(::GetWindowRect(hDlg, (LPRECT)rc) && ::GetClientRect(hDlg, (LPRECT)rcClient)) {
			// �����Ǵ� ������ Ŭ���̾�Ʈ �����̴�(Ÿ��Ʋ�ٿ� �׵θ� �������� ������)
			CRect rcNew = GetRect(rcClient);
			rcNew.right += (rc.Width() - rcClient.Width());
			rcNew.bottom += (rc.Height() - rcClient.Height());
			// �������� ����� �����Ų��
			::SetWindowPos(hDlg, NULL, 0, 0, rcNew.Width(), rcNew.Height(), SWP_NOMOVE|SWP_NOZORDER);
		}
	}

	// �ڽ� �����츦 �����ϸ鼭 ����� ������
	::EnumChildWindows(hDlg, EnumChildProc, (LONG)this);
}

void CDialogCtrlReposition::CtrlPos(HWND hWnd, CRect &rc)
{
	CRect rcNew = GetRect(rc);
	// �������� ��ġ�� ����� �����Ų��
	::SetWindowPos(hWnd, NULL, rcNew.left, rcNew.top, rcNew.Width(), rcNew.Height(), SWP_NOZORDER);
}

BOOL CALLBACK CDialogCtrlReposition::EnumChildProc(HWND hwnd, LPARAM lParam)
{
	if(lParam==NULL) return FALSE;
	CDialogCtrlReposition *pThis = (CDialogCtrlReposition*)lParam;

	if(hwnd) {
		HWND hWndParent = ::GetParent(hwnd);
		CRect rc;
		POINT pnt1, pnt2;
		if(hWndParent && ::GetWindowRect(hwnd, (LPRECT)rc)) {
			pnt1.x = rc.left;
			pnt1.y = rc.top;
			pnt2.x = rc.right;
			pnt2.y = rc.bottom;
			::ScreenToClient(hWndParent, &pnt1);
			::ScreenToClient(hWndParent, &pnt2);
			rc.SetRect(pnt1, pnt2);
			pThis->CtrlPos(hwnd, rc);
		}
	}
	else {
		return FALSE;
	}

	// ã�⸦ ����ϱ� ���Ͽ� TRUE�� ����
	return TRUE;
}