#if !defined(AFX_LOBBYLISTWND_H__E7C3F7C6_74E5_4B93_A9A0_198D51B45396__INCLUDED_)
#define AFX_LOBBYLISTWND_H__E7C3F7C6_74E5_4B93_A9A0_198D51B45396__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LobbyListWnd.h : header file
//
#include "GraphButton.h"
#include "UserStruct.h"

/////////////////////////////////////////////////////////////////////////////
// CLobbyListWnd view


#define MAX_ROOM_LOBY	80		//	�ִ��� - ä�δ�
#define MAX_USER		200		//	�ִ������� - ä�δ�

#define MAX_ROOMCHECK	13		//	���� ������ �׷��� �濡 ��
#define RLIST_SY		17		//  �븮��Ʈ �׷��� Y��ǥ (X��ǥ 0) -> ��ư �Ʒ�

using namespace std;

// ����
class RoomInfornumSort {
public:
	bool bSort;
	RoomInfornumSort() { };
	RoomInfornumSort(bool btrue) { bSort = btrue;}
	
	bool operator() (const ROOMINFO &p, const ROOMINFO &q) const
	{
		if(bSort) {
			if(p.RoomNum < q.RoomNum ) return true;
		}
		else {
			if(p.RoomNum > q.RoomNum ) 	return true;
			
		}
/*
		if(bSort) {
			if(p.State < q.State ) {
				return true;
			} else {
				if(p.NowUserNum < q.NowUserNum) return true;
			}
		}
		else {
			if(p.State > q.State ) {
				return true;
			} else {
				if(p.NowUserNum > q.NowUserNum) return true;
			}
		}
*/
		return false;
	}
};

class RoomInformasterSort {
public:
	bool bSort;
	RoomInformasterSort() { };
	RoomInformasterSort(bool btrue) { bSort = btrue;}

	bool operator() (const ROOMINFO &p, const ROOMINFO &q) const
	{
		CString str1 = p.ID;
		CString str2 = q.ID;
		str1.MakeLower();
		str2.MakeLower();
		if(bSort)
			return str1 < str2;
		else
			return str1 > str2;
	}
};

class RoomInfortitleSort {
public:
	bool bSort;
	RoomInfortitleSort() { };
	RoomInfortitleSort(bool btrue) { bSort = btrue;}
	
	bool operator() (const ROOMINFO &p, const ROOMINFO &q) const
	{
		CString str1 = p.Title;
		CString str2 = q.Title;
		str1.MakeLower();
		str2.MakeLower();
		if(bSort)
			return str1 < str2;
		else
			return str1 > str2;
	}
};

class RoomInforkindSort {
public:
	bool bSort;
	RoomInforkindSort() { };
	RoomInforkindSort(bool btrue) { bSort = btrue;}
	
	bool operator() (const ROOMINFO &p, const ROOMINFO &q) const
	{			
		if(bSort)
			return p.FormKind < q.FormKind;
		else
			return p.FormKind > q.FormKind;	
	}
};

class RoomInforcountSort {
public:
	bool bSort;
	RoomInforcountSort() { };
	RoomInforcountSort(bool btrue) { bSort = btrue;}
	
	bool operator() (const ROOMINFO &p, const ROOMINFO &q) const
	{
		if(bSort)
			return p.NowUserNum < q.NowUserNum;
		else
			return p.NowUserNum > q.NowUserNum;
	}
};

class RoomInforstateSort {
public:
	bool bSort;
	RoomInforstateSort() { };
	RoomInforstateSort(bool btrue) { bSort = btrue;}
	
	bool operator() (const ROOMINFO &p, const ROOMINFO &q) const
	{
		INT64 pM, qM;
		pM = GetLevelToUserMoney( p.UsernIcon );
		pM = pM * GetCreateRoomGrade(p.CreateRGrade) / 10; // �游���� �ּҸӴ� �������

		qM = GetLevelToUserMoney( q.UsernIcon );
		qM = qM * GetCreateRoomGrade(q.CreateRGrade) / 10; // �游���� �ּҸӴ� �������

		if(bSort)
			return pM < qM;
		else
			return pM > qM;
	}
};



class CLobbyListWnd : public CWnd
{
public:
	CPage			m_Page;
	xSprite			*m_pBackSpr;
	xSprite			*m_pButtonSpr;

	CGraphButton	m_UpBtn;
	CGraphButton	m_DownBtn;
	CGraphButton	m_ThumbBtn;
	CGraphButton    m_RoomNunBtn;
	CGraphButton    m_RoomMasterBtn;
	CGraphButton    m_RoomTitleBtn;
	CGraphButton    m_RoomKindBtn;
	CGraphButton    m_RoomCountBtn;
	CGraphButton    m_RoomStateBtn;

	bool            bRoomNumSort;

	UINT			m_hTimer;
	int				m_UseMode;			//	���
	int				m_TimeCnt;

	CRect			m_RoomCheckRect_Max[ MAX_ROOMCHECK ];
	CRect			m_ScrollBaseRect;	//	��ũ�ѹ� ����

	CMenu           m_RoomMenuV;

	int				m_NowIndex;			//	���� �ε���
	int				m_MouseOverIndex;	//	������ �ε���
	int				m_SelectIndex;		//	���õ� �ε��� - ���ȣ, �Ǵ� ������ȣ

	CPoint			m_LButtonDownThumbPos;
//	============= ����Ʈ ���� ========================
	ROOMINFO	*m_pRoomInfoList[ MAX_ROOM_LOBY ];		//	�渮��Ʈ

	deque<ROOMINFO> m_deque_room;
	

	//CPtrList	m_List;
	bool		InsertList( void *pList );
	bool		SetList( void *pList, BOOL bTitle = FALSE );
	void		DelList( int DelIndex, char *pID = NULL );		//	���϶��� pID ��� �ȴ�
	void		DelAllList( void );

	void		SetIndex( int Index );					//	�ε��� �����ϱ�
	int			GetFindUserIndex( char *pID );			//	���̵�� ���� �ε��� ã��
	int			GetSelectIndex( void );					//	���õ� �ε��� ã�ƿ���

	ROOMINFO	*GetRoomInfo( int RoomNum );


	void		SetFindSelectUser( SMALLUSERINFO *pUi );	//	ã��� �ؼ� �����ϸ� ���������� ��Ŀ�� �����Ѵ�

	void		RenewalShortList( void );		//	���� ����
//	==================================================

//	================ �����ϱ� ========================
	void		ResetControlls( void );
//	==================================================

	void		Draw( CDC *pDC );
	void		DrawRoom( CDC *pDC );		//	���϶�
	void		DrawButton( CDC *pDC );


// Operations
public:
	BOOL GetStyleRoomInfo(int rnum, STYLEROOMINFO &sri);

	void		Init( xSprite *pBackSpr, xSprite *pButtonSpr );

	virtual void Create( CWnd* pParentWnd, const RECT& rect, UINT nID, int UseMode );


	virtual ~CLobbyListWnd();
	CLobbyListWnd();           // protected constructor used by dynamic creation

protected:
	DECLARE_DYNCREATE(CLobbyListWnd)


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLobbyListWnd)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CLobbyListWnd)
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMenuAdmInroom();
	afx_msg void OnMenuAdmMakeroom();
	afx_msg void OnMenuAdmRenameroom();
	afx_msg void OnMenuAdmRoomdel();
	afx_msg void OnMenuAdmRoominfo();
	afx_msg void OnMenuAdmRoommaster();
	afx_msg void OnButtonRoominfo();
	afx_msg void OnButtonEntergame();
	afx_msg void OnButtonCreateroom();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOBBYLISTWND_H__E7C3F7C6_74E5_4B93_A9A0_198D51B45396__INCLUDED_)
