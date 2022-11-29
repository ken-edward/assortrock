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
	else // ���� ���� ����
	{
		justleft += DT;

		if (pos.x > leftPatrolPoint.x && pos.x < rightPatrolPoint.x) // ���� ���� ���� ����
		{
			if (fabsf(rightPatrolPoint.x - pos.x) < 30.0f) // ���� ��
			{
				if (staytime < 10.0f && justleft > 5.0f)
				{
					staytime += DT;
					LR = 0;
				}
				else // 10�� �̻�
				{
					staytime = 0.0f;
					LR = -1;
					justleft = 0.0f;
				}
			}
			else if (fabsf(leftPatrolPoint.x - pos.x) < 30.0f) // ���� ��
			{
				if (staytime < 5.0f && justleft > 5.0f)
				{
					staytime += DT;
					LR = 0;
				}
				else // 10�� �̻�
				{
					staytime = 0.0f;
					
					LR = 1;
					justleft = 0.0f;
				}
			}
			else // ���� ���� �߰��� ����
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
		else // ���� ������ ���� ���� ���� ���� ����
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

	if (currentFloor == targetFloor) // ���� ����
	{
		if (fabsf(destination.x - pos.x) < dis) // �������� ��������. ���� �Ÿ��� ����.
		{
			arrived = true;
			LR = 0;
		}
		else if (pos.x < destination.x) // ������ ����
		{
			LR = 1;
		}
		else // ������ ����
		{
			LR = -1;
		}
	}
	else // �ٸ� ����
	{
		if (currentFloor < targetFloor) // �� ���� ���� ����
		{
			MapResult result = MapMgr::Inst().ClosestStairs(pos, 1);

			if (result.granted == false)
			{
				arrived = true;
				return false; // ���� ���� �ִ� ����� ��� �� �� ����
				LR = 0;
			}

			if (fabsf(result.position.x - pos.x) < 10.0f) // ��ܿ� ������
			{
				stairstry = true;
				UD = 1;
			}
			else if (result.position.x > pos.x)
			{
				LR = 1;
			}
			else // ������ ����
			{
				LR = -1;
			}
		}
		else // �� ���� ���� ����
		{
			MapResult result = MapMgr::Inst().ClosestStairs(pos, 0);

			if (result.granted == false)
			{
				arrived = true;
				return false; // ���� ���� �ִ� ����� ��� �� �� ����
				LR = 0;
			}

			if (fabsf(result.position.x - pos.x) < 10.0f) // ��ܿ� ������
			{
				stairstry = true;
				UD = -1;
			}
			else if (result.position.x > pos.x)
			{
				LR = 1;
			}
			else // ������ ����
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
		return false; // �� �� ����
	}
	if (BlockedByWall())
	{
		arrived = true;
		return false; // �� �� ����
	}

	return true; // �� �� ����
}

