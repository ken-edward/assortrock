#include "MapMgr.h"
#include "Stairs.h"
#include "Siren.h"


MapMgr::MapMgr()
{
}


MapMgr::~MapMgr()
{
}


MapResult MapMgr::ClosestStairs(VEC2 currentPos, bool upordown)
{
	int currentFloor = (int)currentPos.y / 600; // 0층부터 시작

	bool found = false;
	VEC2 targetPos;

	auto iter = stairsList.begin();
	auto end = stairsList.end();

	for (; iter != end; ++iter)
	{
		if (upordown) // 상향 계단 찾는중
		{
			if ((*iter)->up == false)
			{
				continue;
			}
		}
		else // 하향 계단 찾는중
		{
			if ((*iter)->down == false)
			{
				continue;
			}
		}

		int stairsFloor = (int)(*iter)->pos.y / 600; // 0층부터 시작

		if (currentFloor == stairsFloor)
		{
			if (found == false)
			{
				found = true;
				targetPos = (*iter)->pos;
			}
			else
			{
				float currentDis = (targetPos - currentPos).scalar();
				float newDis = ((*iter)->pos - currentPos).scalar();

				if (newDis < currentDis)
				{
					targetPos = (*iter)->pos;
				}
			}
		}
	}

	return { found, targetPos };
}


void MapMgr::RegisterStairs(Stairs* stairs)
{
	stairsList.push_back(stairs);
}


void MapMgr::RegisterSiren(Siren* stairs)
{
	sirenList.push_back(stairs);
}


MapResult MapMgr::ClosestSiren(VEC2 currentPos)
{
	int currentFloor = (int)currentPos.y / 600; // 0층부터 시작

	bool found = false;
	VEC2 targetPos;

	auto iter = sirenList.begin();
	auto end = sirenList.end();

	for (; iter != end; ++iter)
	{
		int sirenFloor = (int)currentPos.y / 600; // 0층부터 시작

		if (currentFloor == sirenFloor)
		{
			if (found == false)
			{
				found = true;
				targetPos = (*iter)->pos;
			}
			else
			{
				float currentDis = (targetPos - currentPos).scalar();
				float newDis = ((*iter)->pos - currentPos).scalar();

				if (newDis < currentDis)
				{
					targetPos = (*iter)->pos;
				}
			}
		}
	}

	return { found, targetPos };
}


void MapMgr::Clear()
{
	stairsList.clear();
	sirenList.clear();
}