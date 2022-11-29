#include "stdafx.h"
#include "TimeClass.h"


TimeClass::TimeClass()
{
	QueryPerformanceCounter(&Count);
	QueryPerformanceFrequency(&Freq);
	Prev = Count;
}


void TimeClass::Update()
{
	QueryPerformanceCounter(&Count);

	m_deltaTime =
		(float)(Count.QuadPart - Prev.QuadPart)
		/ (float)(Freq.QuadPart) * m_slowmo;

	if (m_deltaTime > 0.1f)
	{
		m_deltaTime = 0.1f;
	}

	Prev = Count;
}


void TimeClass::SlowMo(float val)
{
	m_slowmo = val;
}