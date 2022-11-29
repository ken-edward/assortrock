#include "stdafx.h"
#include "TimeMgr.h"


TimeMgr::TimeMgr()
{
}


TimeMgr::~TimeMgr()
{
}


void TimeMgr::Init()
{
	QueryPerformanceFrequency(&m_Count);
	QueryPerformanceCounter(&m_CurTime);
	m_PrevTime = m_CurTime;
}


void TimeMgr::Update()
{
	QueryPerformanceCounter(&m_CurTime);
	LONGLONG TimeGap = m_CurTime.QuadPart - m_PrevTime.QuadPart;
	m_DeltaTime = (float)((double)TimeGap / m_Count.QuadPart);
	m_PrevTime = m_CurTime;
}


float TimeMgr::DeltaTime()
{
	float LimitTime = 0.1f; // 최대 시간. 디버깅용, 버그 방지용.

	if (m_DeltaTime > LimitTime)
	{
		m_DeltaTime = LimitTime;
	}

	return m_DeltaTime;
}