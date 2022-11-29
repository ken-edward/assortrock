#pragma once
#include <Actor.h>
class Siren :
	public Actor
{
public:
	Siren(VEC2 midBot);
	~Siren();

private:
	void Update2();
	
private:
	TexCom* triggered = nullptr;
};

