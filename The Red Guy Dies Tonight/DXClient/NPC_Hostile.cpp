#include "NPC.h"
#include "MapMgr.h"


void NPC::HostileSetting()
{
	suswait = 1.0f;
	checked = true;
}


void NPC::HostileUpdate()
{
	boost = true;

	if (seeing)
	{
		aiming = true;
		if (burstfire < 8)
		{
			firing = true;
		}
		else
		{
			burstcool += DT;

			if (burstcool > 1.0f)
			{
				burstcool = 0.0f;
				burstfire = 0;
			}
		}
		if (armSightLine->Find(L"character"))
		{
			pointer = pPlayer->armpos;
		}
		else
		{
			pointer = pPlayer->pos;
		}
		checked = false;
		arrivetime = 0.0f;
	}
	else
	{
		burstfire = 0;
		if (checked == false)
		{
			arrived = false;
			destination = lastKnownPosition;
			boost = true;
			aiming = true;
			pointer = lastKnownPosition;
			if (Going())
			{
				checked = true;
			}
			
			if (arrived)
			{
				arrivetime += DT;
				if (arrivetime > 2.0f)
				{
					checked = true;
				}
			}
			else
			{
				arrivetime = 0.0f;
			}
		}
		else
		{
			if (justSuspicious)
			{
				aiming = true;
				pointer = suspoint;
				arrived = false;
				destination = suspoint;
				arrivetime = 0.0f;
				findingsiren = false;
			}

			if (arrived == false)
			{
				boost = true;
				if (findingsiren == false)
				{
					aiming = true;
				}
				Going(40.0f);
			}
			else
			{
				boost = true;

				if (pPlayer->hp > 0)
				{
					if (alarmed == false)
					{
						aiming = false;
						findingsiren = true;
						FindSiren();
					}
					else
					{
						Search();
					}
				}
				else
				{
					ceasefire += DT;

					if (ceasefire > 3.0f)
					{
						ChangeAware(Unaware);
						alarmed = false;
						SoundClass::Inst().Stop(L"siren");
					}
				}
			}
		
		}
	}
}


void NPC::FindSiren()
{
	MapResult result = MapMgr::Inst().ClosestSiren(pos);
	
	if (result.granted)
	{
		destination = result.position;
		Going(10.0f);
		if (arrived)
		{
			if (col->Find(L"siren"))
			{
				alarmed = true;
				SoundClass::Inst().Play(L"siren");
			}
			else
			{
				Search();
			}
		}
	}
	else
	{
		Search();
	}
}