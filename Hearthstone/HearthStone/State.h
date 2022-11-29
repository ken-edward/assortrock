#pragma once
#include <list>
#include "SoundPlayer.h"

#define FAKELOADING 0	// ��¥ �ε� ����
#define DEBUGMODE   1	// ����� ��� ����
#define STARTSTATE  0	// 0: ���� �޴�, 1: �ΰ��� ȭ��

/*
	Note

	18.05.12 ȭ: ���� ��� �߰�. ���� �� �� �ֽ��ϴ�. �׷��� �� ���� ����� ���� �� ���� �����Դϴ�. �׸��� �̰� �ø� ��αװ� ��� �����մϴ�.
*/

class Actor;
class SoundPlayer;
class State
{
	friend class APICORE;

public:
	virtual void StateUpdate() {}
	virtual void EndStateUpdate() {}
	void Update();
	virtual void Start() {}

private:
	int m_NextState;

public:
	void SetNextState(int _State)
	{
		m_NextState = _State;
	}

public:
	static SoundPlayer* bgm;

protected:
	std::list<Actor*> m_AllActorList;
	std::list<Actor*>::iterator StartIter;
	std::list<Actor*>::iterator EndIter;
	
public:
	template<typename T>
	T* CreateActor()
	{
		T* pNewActor = new T();
		m_AllActorList.push_back(pNewActor);
		pNewActor->State(this);
		return pNewActor;
	}

public:
	void ReleaseActor();
	void EndState();

public:
	State();
	virtual ~State();
};
