#pragma once
#include <Actor.h>
class Stairs :
	public Actor
{
public:
	Stairs(VEC2 leftBot, bool up, bool down);
	~Stairs();

public:
	bool up = false;
	bool down = false;
};

