#pragma once
#include "TimeMgr.h"
#include "InputMgr.h"
#include "RenderMgr.h"
#include "ResMgr.h"

enum OrderIndex
{
	OnField,
	HeroOrder,
	AttackingOrder,
	OnHand,
	PlayingOrder,
	Last
};

class State;
class Actor
{
	friend class ColMgr;

public:
	float x;
	float y;
	float ImageX;
	float ImageY;
	float SizeX;
	float SizeY;
	float scale;

public:
	wchar_t* Name;
	State* m_State;
	bool m_bDeath;

public:
	OrderIndex order = OnField;

public:
	inline bool IsDeath() { return m_bDeath; }

public:
	virtual void Update() {}
	void State(State* _State)
	{
		m_State = _State;
	}
	virtual void Start() {}

public:
	Actor();
	~Actor();
};

