#pragma once
#include <vector>
#include "State.h"

enum PlayPhase
{
	IntroPhase,
	MulliganPhase,
	EndPhase
};

class MyHero;
class OpponentHero;
class Character;
class PlayState :
	public State
{
public:
	MyHero* Player;
	OpponentHero* Opponent;

private:
	bool myturnstart = false;
	float turnstartcount = 0.0f;
	bool surrender = false;

public:
	void Intro();
	bool introstart = true;
	float anicount = 0;
	bool Phase[5] = {};

public:
	void StateUpdate() override;
	void EndStateUpdate() override;
	void Start() override;
	void CheckTurnStart();

public:
	void HitGround();

public:
	PlayState();
	~PlayState();
};

