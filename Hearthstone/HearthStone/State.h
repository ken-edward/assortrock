#pragma once
#include <list>
#include "SoundPlayer.h"

#define FAKELOADING 0	// 가짜 로딩 여부
#define DEBUGMODE   1	// 디버그 모드 여부
#define STARTSTATE  0	// 0: 메인 메뉴, 1: 인게임 화면

/*
	Note

	18.05.12 화: 상점 기능 추가. 팩을 살 수 있습니다. 그러나 팩 개봉 기능은 아직 안 만든 상태입니다. 그리고 이걸 올릴 블로그가 없어서 쓸쓸합니다.
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
