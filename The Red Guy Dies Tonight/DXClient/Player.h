#pragma once
#include "Character.h"

class Player : public Character
{
public:
	Player(VEC2);
	~Player();

private:
	void Update2();
	void OnLandUpdate();
	void RappellingUpdate();
	void FallingUpdate();
	void BreachingUpdate();
	void IntroUpdate();

public:
	static bool killmission;
	bool camchange = false;

private:
	float zoomlevel = 2.0f;
	bool godmode = 0;
	float introtime = 0.0f;
};