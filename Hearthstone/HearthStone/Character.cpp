#include "stdafx.h"
#include "Character.h"


Character::Character()
	: atk(0), hp(0), Maxhp(0)
{
}


Character::~Character()
{
}


void Character::showdmg()
{
	if (damaged == true)
	{
		if (dmgcount < 1.5f)
		{
			dmgcount += DELTATIME;
			RENDINST.ShowDamage(x, y, dmg);
		}
		else
		{
			dmgcount = 0.0f;
			damaged = false;
		}
	}
}



void Character::TakingDamage(int _dmg)
{
	if (_dmg == 0)
	{
		return;
	}
	if (armor == 0)
	{
		hp -= _dmg;
	}
	else if (armor > 0)
	{
		if (armor >= _dmg)
		{
			armor -= _dmg;
		}
		else if (armor < _dmg)
		{
			_dmg -= armor;
			armor = 0;
			hp -= _dmg;
		}
	}
}