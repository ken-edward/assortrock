#pragma once
#include <unordered_map>
#include <set>
#include "Vector.h"
class ColCom;

class ColMgr
{
private:
	ColMgr();
	~ColMgr();

public:
	static ColMgr& Inst()
	{
		static ColMgr Inst;
		return Inst;
	}

private:
	std::unordered_map<std::wstring, std::set<ColCom*>> colGroupMap;
	std::unordered_map<std::wstring, std::list<std::wstring>> colLinkMap;

public:
	void Add(std::wstring groupName, ColCom* colcom);
	void ColAll();
	void Link(std::wstring, std::wstring);
	void ClearAll();
	std::set<ColCom*>* GetColGroup(std::wstring);
	void LeaveOneTarget(ColCom*);
	VEC2 GetHitPoint(ColCom*, ColCom*);
};

