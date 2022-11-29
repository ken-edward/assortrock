#include "BulletCase.h"
#include <ColCom.h>
#include <ColMgr.h>
#include <TexCom.h>
#include <TimeClass.h>


BulletCase::BulletCase(VEC2 _pos, float _dir)
{
	pos = _pos;
	dir = _dir;

	col = (ColCom*)AddCom(new ColCom);
	col->SetPos(pos);
	ColMgr::Inst().Add(L"bulletcase", col);

	tex = (TexCom*)AddCom(new TexCom);
	tex->Init(L"../tex/black.png");
	tex->SetPos(pos);

	speed.x = cos(dir) * 300.0f;
	speed.y = sin(dir) * 300.0f;

	SetSize({ 2.0f, 1.0f });
	SetPos(pos);
}


BulletCase::~BulletCase()
{
}


void BulletCase::Update2()
{
	speed.y -= 700.0f * DT;
}


void BulletCase::Update3()
{
	deathtime += DT;

	if (deathtime > 3.0f)
	{
		bAlive = false;
		return;
	}

	ColCom* target = col->Get(L"floor");
	if (target != nullptr)
	{
		SetPos({ pos.x, target->pos.y + target->size.y + col->size.y });
		speed.y /= -2.0f;
		speed.x /= 2.0f;
	}
	else
	{
		ColCom* wall = col->Get(L"wall");
		if (wall != nullptr)
		{
			if (wall->GetPos().x <= pos.x)
			{
				SetPos({ wall->GetPos().x + wall->GetSize().x + size.x, pos.y });
			}
			else
			{
				SetPos({ wall->GetPos().x - wall->GetSize().x - size.x, pos.y });
			}

			speed.x = speed.x * -0.6f;
		}
		else
		{
			speed.x -= speed.x * 0.9f * DT;
		}
	}

	Move(speed * DT);
}