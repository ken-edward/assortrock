#include "NPC.h"
#include "MapMgr.h"


void NPC::UnawareSetting()
{
	suswait = 3.0f;
}


void NPC::UnawareUpdate()
{
	if (patrol)
	{
		Patrol();
	}
	else
	{
		KeepPosition();
	}
}


void NPC::SetPatrolRoute(float left, float right, float height)
{
	patrol = true;

	leftPatrolPoint.x = left;
	rightPatrolPoint.x = right;
	leftPatrolPoint.y = height;
	rightPatrolPoint.y = height;
}


void NPC::KeepPosition()
{
	boost = false;

	if ((firstPosition - pos).scalar() > 30.0f)
	{
		destination = firstPosition;
		Going();
	}
	else
	{
		dir = firstDir;
		LR = 0;
	}
}


void NPC::Patrol()
{
	boost = false;

	int currentFloor = (int)pos.y / 600;

	int targetFloor = (int)leftPatrolPoint.y / 600;

	if (currentFloor != targetFloor)
	{
		destination = leftPatrolPoint;
		Going();
	}
	else // 같은 층에 있음
	{
		justleft += DT;

		if (pos.x > leftPatrolPoint.x && pos.x < rightPatrolPoint.x) // 정찰 범위 내에 있음
		{
			if (fabsf(rightPatrolPoint.x - pos.x) < 30.0f) // 우측 끝
			{
				if (staytime < 10.0f && justleft > 5.0f)
				{
					staytime += DT;
					LR = 0;
				}
				else // 10초 이상
				{
					staytime = 0.0f;
					LR = -1;
					justleft = 0.0f;
				}
			}
			else if (fabsf(leftPatrolPoint.x - pos.x) < 30.0f) // 좌측 끝
			{
				if (staytime < 5.0f && justleft > 5.0f)
				{
					staytime += DT;
					LR = 0;
				}
				else // 10초 이상
				{
					staytime = 0.0f;
					
					LR = 1;
					justleft = 0.0f;
				}
			}
			else // 정찰 범위 중간에 있음
			{
				if (dir)
				{
					LR = 1;
				}
				else
				{
					LR = -1;
				}
			}
		}
		else // 층이 같으나 정찰 범위 내에 있지 않음
		{
			destination.x = (rightPatrolPoint.x + leftPatrolPoint.x) * 0.5f;
			destination.y = rightPatrolPoint.y;
			Going();
		}
	}
}



bool NPC::Going(float dis)
{
	int currentFloor = (int)pos.y / 600;
	int targetFloor = (int)destination.y / 600;

	UD = 0;
	stairstry = false;
	arrived = false;

	if (currentFloor == targetFloor) // 같은 층임
	{
		if (fabsf(destination.x - pos.x) < dis) // 목적지에 도착했음. 접근 거리는 지정.
		{
			arrived = true;
			LR = 0;
		}
		else if (pos.x < destination.x) // 우측에 있음
		{
			LR = 1;
		}
		else // 좌측에 있음
		{
			LR = -1;
		}
	}
	else // 다른 층임
	{
		if (currentFloor < targetFloor) // 더 높은 층에 있음
		{
			MapResult result = MapMgr::Inst().ClosestStairs(pos, 1);

			if (result.granted == false)
			{
				arrived = true;
				return false; // 같은 층에 있는 계단이 없어서 갈 수 없음
				LR = 0;
			}

			if (fabsf(result.position.x - pos.x) < 10.0f) // 계단에 도착함
			{
				stairstry = true;
				UD = 1;
			}
			else if (result.position.x > pos.x)
			{
				LR = 1;
			}
			else // 좌측에 있음
			{
				LR = -1;
			}
		}
		else // 더 낮은 층에 있음
		{
			MapResult result = MapMgr::Inst().ClosestStairs(pos, 0);

			if (result.granted == false)
			{
				arrived = true;
				return false; // 같은 층에 있는 계단이 없어서 갈 수 없음
				LR = 0;
			}

			if (fabsf(result.position.x - pos.x) < 10.0f) // 계단에 도착함
			{
				stairstry = true;
				UD = -1;
			}
			else if (result.position.x > pos.x)
			{
				LR = 1;
			}
			else // 좌측에 있음
			{
				LR = -1;
			}
		}
	}

	if (BlockedByDoor())
	{
		doortry = true;
	}
	if (BlockedByGlass())
	{
		arrived = true;
		return false; // 갈 수 없음
	}
	if (BlockedByWall())
	{
		arrived = true;
		return false; // 갈 수 없음
	}

	return true; // 갈 수 있음
}

