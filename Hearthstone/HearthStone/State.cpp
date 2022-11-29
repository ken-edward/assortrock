#include "stdafx.h"
#include "State.h"
#include "Actor.h"


SoundPlayer* State::bgm = nullptr;


State::State()
	: m_NextState(-1)
{
}


State::~State()
{
	StartIter = m_AllActorList.begin();
	EndIter = m_AllActorList.end();
	for (; StartIter != EndIter; StartIter++)
	{
		delete (*StartIter);
	}
}


void State::Update()
{
	for (int i = OnField; i < Last; i++)
	{
		StartIter = m_AllActorList.begin();
		EndIter = m_AllActorList.end();
		for (; StartIter != EndIter; StartIter++)
		{
			if ((*StartIter)->order == i)
			{
				(*StartIter)->Update();
			}
		}
	}
}


void State::ReleaseActor()
{
	StartIter = m_AllActorList.begin();
	EndIter = m_AllActorList.end();
	for (; StartIter != EndIter;)
	{
		if ((*StartIter)->IsDeath() == false)
		{
			StartIter++;
			continue;
		}
		delete *StartIter;
		StartIter = m_AllActorList.erase(StartIter);
	}
}


void State::EndState()
{
	StartIter = m_AllActorList.begin();
	EndIter = m_AllActorList.end();
	for (; StartIter != EndIter; StartIter++)
	{
		delete *StartIter;
	}
	m_AllActorList.clear();
}


