#pragma once
#include <Actor.h>
class TitleActor : public Actor
{
public:
	TitleActor();
	~TitleActor();

public:
	bool selected = false;
	float delay = 0.0f;
	int task = 0;
	void Update2();
	bool ButtonTest(POINT pos, POINT size);
};

