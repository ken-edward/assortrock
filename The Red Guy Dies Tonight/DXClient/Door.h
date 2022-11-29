#pragma once
#include <Actor.h>
class Door :
	public Actor
{
public:
	Door(VEC2, VEC2, bool);
	~Door();

private:
	TexCom* dooropen = nullptr;

public:
	bool open = false;

public:
	void Toggle();
};

