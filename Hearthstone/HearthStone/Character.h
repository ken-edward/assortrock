#pragma once
#include "Actor.h"
class PlayState;
class Character : public Actor
{
public:
	int atk;
	int Oriatk;
	int tempatk = 0;
	int hp;
	int Maxhp;
	int Orihp;
	wchar_t* Name;
	int armor = 0;
	int prevarmor = 0;

public:
	int prevhp;
	int dmg;
	bool damaged = false;
	float dmgcount = 0.0f;
	
public:
	bool atkstart = false;
	bool atkon = false;
	float atkcount = 0.0f;
	bool ActualDeath = false;
	float StartX = 0;
	float StartY = 0;
	float TargetX = 0;
	float TargetY = 0;
	Character* target = nullptr;
	bool attacked = false;
	bool attackable = false;

public:
	void HP(int _hp)
	{
		hp = _hp;
		Maxhp = _hp;
		Orihp = _hp;
		prevhp = _hp;
	}
	virtual void Attack() {}
	void TakingDamage(int _dmg);
	void showdmg();

public:
	Character();
	~Character();
};

