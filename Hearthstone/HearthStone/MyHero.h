#pragma once
#include "Hero.h"

class MyHero :
	public Hero
{
	friend class PlayState;

public:
	void Update() override;
	void ShowMana();

private:
	void MyCardDisplay();
	void SelectCard();
	void MyFieldDisplay();
	void SelectField();
	void SelectHero();
	bool HeroAttackSelecting = false;

private:
	void domulligan();
	bool MulCard[3] = {};
	void changecard() override;

public:
	Character* TargetTest();
	Character* EnemyTargetTest();
	Character* FriendlyTargetTest();

private:

	void ShowEmote();
	void SelectEmote();
	bool SelectingEmote = false;

public:
	MyHero();
	~MyHero();
};

