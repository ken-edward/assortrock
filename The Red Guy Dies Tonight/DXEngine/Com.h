#pragma once
#include "Vector.h"
class Actor;
class Com
{
public:
	Com();
	virtual ~Com();

public:
	bool bAlive = true;
	bool bUpdate = true;
	Actor* pActor = nullptr;

public:
	virtual void Update1() {}
	virtual void Update2() {}
	virtual void Update3() {}
	virtual void Update4() {}
	virtual void Update5() {}

public:
	virtual void Move(VEC2 value) {}
	virtual void SetPos(VEC2 value) {}
	virtual void SetSize(VEC2) {}
};

