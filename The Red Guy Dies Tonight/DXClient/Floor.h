#pragma once
#include <Actor.h>
class Floor : public Actor
{
public:
	Floor(VEC2 leftTop, VEC2 rectsize);
	Floor(VEC2 leftTop, float length);
	~Floor();
};

