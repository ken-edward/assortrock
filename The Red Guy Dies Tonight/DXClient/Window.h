#pragma once
#include <Actor.h>
class Window : public Actor
{
public:
	Window(VEC2, VEC2, bool);
	~Window();

public:
	int hp = 150;
	TexCom* glass = nullptr;
	ColCom* frame = nullptr;

public:
	void Hit(VEC2 hitpoint);
	void Break();
};

