#pragma once
#include <Actor.h>
class Bullet :
	public Actor
{
public:
	Bullet(VEC2 pos, float rot, ColCom* shooter = nullptr);
	~Bullet();

private:
	float rot;
	VEC2 startpos;
	VEC2 endpos;
	ColCom* col;
	TexCom* tex = nullptr;
	Actor* target = nullptr;
	float time = 0.0f;

public:
	int dmg = 30;

private:
	void Update2() override;
	void Update3() override;
};

