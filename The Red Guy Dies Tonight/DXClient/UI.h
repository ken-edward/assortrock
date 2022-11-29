#pragma once
#include <Actor.h>
class UI :
	public Actor
{
public:
	UI();
	~UI();

private:
	TexCom* ak47;
	TexCom* spistol;
	TexCom* machete;
	float time = 0.0f;

private:
	void Update5();
};

