// Movement.h: interface for the CMovement class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOVEMENT_H__0117225E_AECE_4C1C_83E4_CFEDDBDF0E68__INCLUDED_)
#define AFX_MOVEMENT_H__0117225E_AECE_4C1C_83E4_CFEDDBDF0E68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Sound.h"

extern CSound Sound;

class FPOINT
{

public:

	FPOINT(){ x = y = 0.0f; }
	~FPOINT(){};

	float x;
	float y;

	FPOINT& operator = ( POINT a )
	{
		x = (float)a.x;
		y = (float)a.y;

		return (*this);
	}

	FPOINT& operator = ( FPOINT a )
	{
		x = a.x;
		y = a.y;

		return (*this);
	}

	POINT GetPOINT()
	{
		POINT ret;
		ret.x = (long)x;
		ret.y = (long)y;

		return ret;
	}
};

class CMovement  
{

public:

	CMovement();
	virtual ~CMovement();

	void Reset();
	void PlayMoveSound( int sndno = 0 )
	{ 
		m_bSound = TRUE; 
		m_SndIndex = static_cast<SND_ENUM>(sndno);
	}

	void StraightMoveTo( POINT dest, FPOINT spd, FPOINT acc, int delay = 0 )
	{
		m_dest.x = (float)dest.x;
		m_dest.y = (float)dest.y;

		m_vel = spd;
		m_acc = acc;
//		m_vel.x = (dest.x - m_cur.x) / 40.0f;
//		m_vel.y = (dest.y - m_cur.y) / 40.0f;

//		m_acc.x = (dest.x - m_cur.x) / 32.0f;
//		m_acc.y = (dest.y - m_cur.y) / 32.0f;
//		m_acc.x = 0;
//		m_acc.y = 0;

		m_delay = delay;
		m_bStop = FALSE;
	}

	void Stop()
	{ 
		FPOINT dest;
		dest.x = m_dest.x;
		dest.y = m_dest.y;

		if( m_bSound && m_SndIndex != 0 )		
		{
			Sound.Play( m_SndIndex );
			//m_SndIndex = 0;
		}
		m_bSound = FALSE;
		Reset();
		m_dest = m_cur = dest;
		m_dist.x = 0;
		m_dist.y = 0;
	}

	BOOL IsMoving(){ return !m_bStop; }
	BOOL IsStopped(){ return m_bStop; }

	void SetCurPos( POINT pt){ m_cur = pt; }
	void SetDestPos( POINT pt ){ m_dest = pt; }
	void SetAcc( POINT acc ){ m_acc = acc; }
	void SetVel( POINT vel ){ m_vel = vel; }

	POINT GetCurPos()
	{ 
		POINT temp;
		temp.x = (long)m_cur.x;
		temp.y = (long)m_cur.y;
		return temp;
	}

	POINT GetDest()
	{
		POINT temp;
		temp.x = (long)m_dest.x;
		temp.y = (long)m_dest.y;
		return temp;
	}
	
	void OnTimer( FLOAT time );

	void SetDelay( int delay ){ if( delay > 0 ) m_delay = delay; }

	int GetDelay() { return m_delay; }

protected:

	BOOL m_bStop;		// 현재 정지상태인가..
	int  m_delay;		// 얼마나 기다린 후에 운동을 시작할 것인가.

	FPOINT m_cur;		// 현재 위치값.
	FPOINT m_acc;		// 가속도 벡터
	FPOINT m_vel;		// 속도 벡터


	FPOINT m_dest;			// 끝점
	FPOINT m_dist;
	BOOL m_bSound;
	SND_ENUM  m_SndIndex;
//	CPath m_path;		// 시작점과 끝점. 조절점 2개를 결정했을때의 경로..
};

#endif // !defined(AFX_MOVEMENT_H__0117225E_AECE_4C1C_83E4_CFEDDBDF0E68__INCLUDED_)
