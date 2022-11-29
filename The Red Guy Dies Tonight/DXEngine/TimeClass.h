#pragma once
#define DT TimeClass::Inst().GetDeltaTime()
#include <Windows.h>
class TimeClass
{
private:
	TimeClass();

public:
	static TimeClass& Inst()
	{
		static TimeClass Inst;
		return Inst;
	}

public:
	void Update();
	float GetDeltaTime() { return m_deltaTime; }
	void SlowMo(float val);

private:
	float m_deltaTime = 0.0f;
	float m_slowmo = 1.0f;
	LARGE_INTEGER Count;
	LARGE_INTEGER Prev;
	LARGE_INTEGER Freq;
};