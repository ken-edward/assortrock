#pragma once
class Script
{
public:
	Script();
	virtual ~Script();

public:
	bool bAlive = true;

public:
	virtual void Update1() {}
	virtual void Update2() {}
	virtual void Update3() {}
	virtual void Update4() {}
	virtual void Update5() {}
};

