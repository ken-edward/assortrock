#pragma once
#include <unordered_map>
#include "Vector.h"
#include "TexCom.h"
#include "ColCom.h"
#include "AniState.h"
#include "ColMgr.h"
using namespace std;
class Actor
{
public:
	Actor();
	virtual ~Actor();

public:
	bool bAlive = true;

public:
	virtual void Update1() {}
	virtual void Update2() {}
	virtual void Update3() {}
	virtual void Update4() {}
	virtual void Update5() {}

private:
	unordered_multimap<wstring, Com*> comMap;

public:
	Com* AddCom(wstring, Com*);
	Com* AddCom(Com*);
	Com* GetCom(wstring);
	void ComUpdate1();
	void ComUpdate2();
	void ComUpdate3();
	void ComUpdate4();
	void ComUpdate5();
	void ComRelease();
	void ComClear();

public:
	VEC2 pos;
	VEC2 acc;
	VEC2 speed;
	VEC2 size;
	ColCom* col = nullptr;
	TexCom* tex = nullptr;
	AniState* ani = nullptr;

public:
	void Move(VEC2 val);
	void SetPos(VEC2 val);
	void SetSize(VEC2 val);
	VEC2 GetPos() { return pos; }
};

