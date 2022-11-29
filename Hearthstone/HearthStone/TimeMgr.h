#pragma once

#define DELTATIME TimeMgr::Inst().DeltaTime()

class TimeMgr
{
private:
	LARGE_INTEGER m_Count;
	LARGE_INTEGER m_PrevTime;
	LARGE_INTEGER m_CurTime;
	float m_DeltaTime;

public:
	static TimeMgr& Inst()
	{
		static TimeMgr TimeInst = TimeMgr();
		return TimeInst;
	}

public:
	void Init();

public:
	void Update();
	float DeltaTime();

public:
	TimeMgr();
	~TimeMgr();
};

