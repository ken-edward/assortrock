#pragma once
#include <Actor.h>
class Wall : public Actor
{
public:
	Wall(VEC2 leftBottom, VEC2 rectsize);
	Wall(VEC2 leftBottom, float height);
	~Wall();
};

