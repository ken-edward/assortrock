#include "NPC.h"
#include "Stairs.h"


void NPC::CautiousSetting()
{
	suswait = 1.0f;
}


void NPC::CautiousUpdate()
{
	if (justSuspicious)
	{
		aiming = true;
		pointer = suspoint;
		arrived = false;
		destination = suspoint;
		arrivetime = 0.0f;
	}
	else
	{
		if (arrived == false)
		{
			boost = true;
			Going(40.0f);
		}
		else
		{
			arrivetime += DT;

			if (arrivetime > 3.0f)
			{
				boost = false;
				if (pPlayer->hp > 0)
				{
					Search();
				}
				else
				{
					ceasefire += DT;

					if (ceasefire > 3.0f)
					{
						ChangeAware(Unaware);
					}
				}
			}
		}
	}
}


void NPC::Search()
{
	if (dir)
	{
		LR = 1;
	}
	else
	{
		LR = -1;
	}
	blockcool += DT;
	if (BlockedByWall() || BlockedByGlass())
	{
		if (blockcool > 1.0f)
		{
			blockcool = 0.0f;
			LR *= -1;
		}
	}
	if (BlockedByDoor())
	{
		doortry = true;
	}
	stairscol = col->Get(L"stairs");
	stairscool += DT;

	int stairschance = rand() % 5;
	if (stairscol != nullptr && stairscol != prevstairs && stairscool > 10.0f)
	{
		Stairs* stairs = (Stairs*)stairscol->pActor;
		if (aware == Hostile)
		{
			if (lastSeen < 10.0f)
			{
				if (stairs->up && stairs->down)
				{
					stairstry = true;
					stairscool = 0.0f;
					if (lastKnownPosition.y > pos.y)
					{
						UD = 1;
					}
					else
					{
						UD = -1;
					}
				}
				else if (stairs->up)
				{
					if (lastKnownPosition.y > pos.y)
					{
						stairstry = true;
						stairscool = 0.0f;
						UD = 1;
					}
				}
				else // down
				{
					if (lastKnownPosition.y < pos.y)
					{
						stairstry = true;
						stairscool = 0.0f;
						UD = -1;
					}
				}
			}
			else // hostile finding
			{
				if (stairschance < 1)
				{
					stairstry = true;
					stairscool = 0.0f;

					if (stairs->up && stairs->down)
					{
						int udhalf = rand() % 2;

						if (udhalf)
						{
							UD = 1;
						}
						else
						{
							UD = -1;
						}
					}
					else if (stairs->up)
					{
						UD = 1;
					}
					else // down
					{
						UD = -1;
					}
				}
			}
		}
		else // Cautious
		{
			if (stairschance < 1)
			{
				stairstry = true;
				stairscool = 0.0f;

				if (stairs->up && stairs->down)
				{
					int udhalf = rand() % 2;

					if (udhalf)
					{
						UD = 1;
					}
					else
					{
						UD = -1;
					}
				}
				else if (stairs->up)
				{
					UD = 1;
				}
				else // down
				{
					UD = -1;
				}
			}
		}
	}

	if (NearCliff())
	{
		ColCom* floor = col->Get(L"floor");

		if (pos.x < floor->pos.x)
		{
			LR = 1;
		}
		else
		{
			LR = -1;
		}
	}

	prevstairs = stairscol;

	aiming = true;
	pointer = armpos;
	pointer.x += 100.0f * LR;
}