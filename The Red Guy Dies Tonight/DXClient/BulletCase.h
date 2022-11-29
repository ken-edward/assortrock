#pragma once
#include <Actor.h>
class BulletCase : public Actor
{
public:
	BulletCase(VEC2 pos, float dir);
	~BulletCase();

public:
	void Update2();
	void Update3();

private:
	float deathtime = 0.0f;
	float dir;
};

