// Movement.cpp: implementation of the CMovement class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "62CutPoker.h"
#include "Movement.h"
#include "Sound.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CSound Sound;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMovement::CMovement()
{
	Reset();
}

CMovement::~CMovement()
{

}

void CMovement::Reset()
{
	m_delay = 0;		// 얼마나 기다린 후에 운동을 시작할 것인가.
	m_bStop = TRUE;

	m_cur.x = 0;
	m_cur.y = 0;
	m_dist = m_dest = m_acc = m_vel = m_cur;
	m_bSound = FALSE;
}

void CMovement::OnTimer( FLOAT time )
{
	// Delay가 있다면 그 수치가 0이 되었을 경우 이동..
	if( m_delay > 0 ) 
	{
		m_delay--;
		if( m_delay == 0  ) Sound.Play( SND21 );
	}
	// 아니라면..
	else if( m_bStop == FALSE )
	{
		// 이번에 이동할 거리를 구한다.
		// 현재의 속도를 v = v0 + at로 구한다.
		m_vel.x += m_acc.x;// * time;
		m_vel.y += m_acc.y;// * time;

		// 이전 위치를 저장해놓고..
		FPOINT prev = m_cur;

		// 다음에 위치할 곳을 찾은 후에..( 이동할 거리는 s = v * t이다. )
		float xvel = m_vel.x * time;
		float yvel = m_vel.y * time;
		m_cur.x += xvel;
		m_cur.y += yvel;

		// 이동 사각형 안에 도착지점이 있다면 도착한 것이므로 일단 이동 사각형을 설정..
		const int EPSILON = 5;
		CRect rc;
		if( prev.x < m_cur.x ) 
		{
			rc.left = (long)(prev.x - EPSILON);
			rc.right = (long)(m_cur.x + EPSILON);
		}
		else
		{
			rc.left = (long)(m_cur.x - EPSILON);
			rc.right = (long)(prev.x + EPSILON);
		}

		if( prev.y < m_cur.y )
		{
			rc.top = (long)(prev.y - EPSILON);
			rc.bottom = (long)(m_cur.y + EPSILON);
		}
		else
		{
			rc.top = (long)(m_cur.y - EPSILON);
			rc.bottom = (long)(prev.y + EPSILON);
		}
/*
		CString str;
		str.Format( "dest : %d %d, RC : %d  %d  %d  %d ", m_dest.x, m_dest.y, rc.left, rc.top, rc.right, rc.bottom );
		AfxMessageBox( str );
*/
		if( rc.PtInRect( m_dest.GetPOINT() ) )	Stop();
/*
		POINT dis = { 0, 0 };
		dis.x = m_dest.x - m_cur.x;
		dis.y = m_dest.y - m_cur.y;

		int len = dis.x * dis.x + dis.y + dis.y;

		int prev_len = m_dist.x * m_dist.x + m_dist.y + m_dist.y;
		if( prev_len <= len ) 
		{
			Stop();
		}
		else m_dist = dis;
*/
	}
}