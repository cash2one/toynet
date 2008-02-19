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


#define MAX_ROOM_LOBY	80		//	최대방수 - 채널당
#define MAX_USER		200		//	최대유저수 - 채널당

#define MAX_ROOMCHECK	13		//	현재 영역에 그려질 방에 수
#define RLIST_SY		17		//  룸리스트 그려질 Y좌표 (X좌표 0) -> 버튼 아래

using namespace std;

// 정렬
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
		pM = pM * GetCreateRoomGrade(p.CreateRGrade) / 10; // 방만들기시 최소머니 설정등급

		qM = GetLevelToUserMoney( q.UsernIcon );
		qM = qM * GetCreateRoomGrade(q.CreateRGrade) / 10; // 방만들기시 최소머니 설정등급

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
	int				m_UseMode;			//	모드
	int				m_TimeCnt;

	CRect			m_RoomCheckRect_Max[ MAX_ROOMCHECK ];
	CRect			m_ScrollBaseRect;	//	스크롤바 영역

	CMenu           m_RoomMenuV;

	int				m_NowIndex;			//	현재 인덱스
	int				m_MouseOverIndex;	//	오버된 인덱스
	int				m_SelectIndex;		//	선택된 인덱스 - 방번호, 또는 유저번호

	CPoint			m_LButtonDownThumbPos;
//	============= 리스트 관련 ========================
	ROOMINFO	*m_pRoomInfoList[ MAX_ROOM_LOBY ];		//	방리스트

	deque<ROOMINFO> m_deque_room;
	

	//CPtrList	m_List;
	bool		InsertList( void *pList );
	bool		SetList( void *pList, BOOL bTitle = FALSE );
	void		DelList( int DelIndex, char *pID = NULL );		//	방일때는 pID 없어도 된다
	void		DelAllList( void );

	void		SetIndex( int Index );					//	인덱스 설정하기
	int			GetFindUserIndex( char *pID );			//	아이디로 유저 인덱스 찾기
	int			GetSelectIndex( void );					//	선택된 인덱스 찾아오기

	ROOMINFO	*GetRoomInfo( int RoomNum );


	void		SetFindSelectUser( SMALLUSERINFO *pUi );	//	찾기로 해서 존재하면 그유저에게 포커스 가게한다

	void		RenewalShortList( void );		//	정보 갱신
//	==================================================

//	================ 정렬하기 ========================
	void		ResetControlls( void );
//	==================================================

	void		Draw( CDC *pDC );
	void		DrawRoom( CDC *pDC );		//	룸일때
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
