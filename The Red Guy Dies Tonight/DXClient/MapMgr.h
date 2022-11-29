#pragma once
#include <unordered_map>
#include <Vector.h>

struct MapResult
{
	bool granted = false;
	VEC2 position;
};

class Stairs;
class Siren;
class MapMgr
{
private:
	MapMgr();
	~MapMgr();

public:
	static MapMgr& Inst()
	{
		static MapMgr Inst;
		return Inst;
	}

private:
	std::list<Stairs*> stairsList; // ��� ��ġ ����
	std::list<Siren*> sirenList; // ���̷�

public:
	void RegisterStairs(Stairs*);
	MapResult ClosestStairs(VEC2, bool);

public:
	void RegisterSiren(Siren*);
	MapResult ClosestSiren(VEC2);

public:
	void Clear();
};

