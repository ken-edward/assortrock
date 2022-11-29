#pragma once
#include "Hero.h"

class OpponentHero :
	public Hero
{
	friend class PlayState;

public:
	void Update() override;
	void ShowMana();
	void OppCardDisplay();
	void OppFieldDisplay();
	float turncount = 0.0f;
	bool CardDisplayOn = true;
	float playterm = 0.0f;

public:
	bool AIPlaying();
	void changecard() override;
	bool heropowerstart = false;
	bool heropoweron = false;
	void usingheropower();
	float anicount = 0;
	bool phase[5] = {};

	void ShowEmote();
	

public:
	OpponentHero();
	~OpponentHero();
};

