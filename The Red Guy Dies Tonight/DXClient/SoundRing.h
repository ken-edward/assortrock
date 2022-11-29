#pragma once
#include <Actor.h>
class SoundRing : public Actor
{
public:
	SoundRing(VEC2, float);
	~SoundRing();

private:
	float time = 0.0f;
	float size = 0.0f;
	float cursize = 0.0f;
	
private:
	void Update4();	
};

