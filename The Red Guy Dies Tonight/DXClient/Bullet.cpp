#include "Bullet.h"
#include <ColCom.h>
#include <ColMgr.h>
#include <TexCom.h>
#include <TimeClass.h>
#include <SoundClass.h>
#include "Character.h"
#include "Window.h"
#include "SoundRing.h"


Bullet::Bullet(VEC2 _pos, float _rot, ColCom* shooter)
{
	startpos = _pos;
	rot = _rot;

	col = (ColCom*)AddCom(new ColCom);
	col->SetType(ColProjectile);
	col->startpos = { -1.0f, -1.0f };
	col->endpos = { 0.0f, 0.0f };
	col->shooter = shooter;
	col->player = shooter->player;
	ColMgr::Inst().Add(L"bullet", col);

	tex = (TexCom*)AddCom(new TexCom);
	tex->Init(L"../tex/yellow.png");
	tex->SetZ(5.0f);
}


Bullet::~Bullet()
{
}


void Bullet::Update2()
{
	float speed = 16000.0f;

	endpos.x = startpos.x + cosf(rot) * speed * DT;
	endpos.y = startpos.y + sinf(rot) * speed * DT;

	col->startpos = startpos;
	col->endpos = endpos;
}


void Bullet::Update3()
{
	time += DT;

	if (time > 3.0f)
	{
		bAlive = false;
	}

	if (col->GetColSet()->empty() == false) // 맞은 상대가 있다면
	{
		// 피격한 상대에게 총알이 맞았음을 전달해야 합니다.
		// 이는 Update4에서 반영됩니다.

		bAlive = false;
		// 이번 프레임에 출력만 하고 나서 삭제됩니다.

		endpos = col->hitpoint;

		if (col->player)
		{
			if (col->Find(L"character") == false)
			{
				Scene::GetCurScene()->AddActor(new SoundRing(endpos, 300.0f));
			}
		}

		if (col->Find(L"character"))
		{
			Character* pCharacter = (Character*)col->Get(L"character")->pActor;
			pCharacter->hp -= dmg;
		}
		else if (col->Find(L"window"))
		{
			((Window*)col->Get(L"window")->pActor)->Hit(col->hitpoint);
		}
	}

	float len = (endpos - startpos).scalar() / 2.0f;

	VEC2 pos = (endpos - startpos) / 2.0f + startpos;

	tex->SetSize({ len, 2.0f });
	tex->SetRotation(rot);
	tex->SetPos(pos);

	startpos = endpos;
}