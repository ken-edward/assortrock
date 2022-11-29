#include "NPC.h"


void NPC::SuspicousSetting()
{
	suswait = 3.0f;
}


void NPC::SuspicousUpdate()
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
			boost = false;
			Going(40.0f);
		}
		else
		{
			arrivetime += DT;

			if (arrivetime > 3.0f)
			{
				ChangeAware(Unaware);
			}
		}
	}
}