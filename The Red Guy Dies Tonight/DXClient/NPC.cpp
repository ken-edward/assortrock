#include "NPC.h"


bool NPC::alarmed = false;
bool NPC::redguydied = false;


NPC::NPC(VEC2 _pos, bool _dir, bool pistol)
{
	SetPos({ _pos.x, _pos.y + 98.0f });
	firstPosition = pos;

	dir = _dir;
	firstDir = dir;

	alarmed = false;
	redguydied = false;

	if (pistol)
	{
		ChangeGun(L"pistol");
	}

	armSightLine = (ColCom*)AddCom(new ColCom);
	armSightLine->SetType(ColProjectile);
	ColMgr::Inst().Add(L"sight", armSightLine);

	posSightLine = (ColCom*)AddCom(new ColCom);
	posSightLine->SetType(ColProjectile);
	ColMgr::Inst().Add(L"sight", posSightLine);

	bodySight = (ColCom*)AddCom(new ColCom);
	bodySight->SetType(ColProjectile);
	bodySight->player = true;
	ColMgr::Inst().Add(L"deadsight", bodySight);

	col->subname = L"enemy"; // 플레이어의 근접 공격용
}


NPC::~NPC()
{
}


void NPC::Update2()
{
	if (IsJustDn(L"G"))
	{
		ChangeAware(Unaware);
		awareness = 0.0f;
		alarmed = false;
		SoundClass::Inst().Stop(L"siren");
	}

	if (alarmed)
	{
		if (aware < Hostile)
		{
			checked = true;
			arrived = true;
			HigherCaution(Hostile);
		}
	}

	crouch = false;

	if (playerInit == false)
	{
		pPlayer = (Character*)Scene::GetCurScene()->GetActor(L"Player");
	}

	boost = false;
	aiming = false;
	firing = false;
	LR = 0;
	UD = 0;

	WitnessPlayer();
	SusUpdate();
	HearSound();
	WitnessBody();
	AwareChange();

	switch (aware)
	{
	case Unaware:
		UnawareUpdate();
		break;
	case Suspicious:
		SuspicousUpdate();
		break;
	case Cautious:
		CautiousUpdate();
		break;
	case Hostile:
		HostileUpdate();
		break;
	default:
		break;
	}
}


void NPC::WitnessPlayer()
{
	armSightLine->startpos = armpos;
	posSightLine->startpos = armpos;

	armSightLine->endpos = pPlayer->armpos;
	posSightLine->endpos = pPlayer->pos;

	seeing = false;

	if (armSightLine->Find(L"character"))
	{
		VEC2 gap = armSightLine->endpos - armSightLine->startpos;

		if (gap.scalar() < 2000.0f)
		{
			if (CheckDegree(gap.theta()))
			{
				// 보임
				seeing = true;
				awareness += (2000.0f - gap.scalar()) / 2000.0f * DT * 200.0f; // 초당 0~200
			}
		}
	}

	if (posSightLine->Find(L"character"))
	{
		VEC2 gap = posSightLine->endpos - posSightLine->startpos;

		if (gap.scalar() < 2000.0f)
		{
			if (CheckDegree(gap.theta()))
			{
				// 보임
				seeing = true;
				awareness += (2000.0f - gap.scalar()) / 2000.0f * DT * 200.0f; // 초당 0~200
			}
		}
	}

	if (seeing)
	{
		if (awareness > 30.0f)
		{
			if (aware == Unaware)
			{
				NewSuspicious(pPlayer->pos);
			}
		}
		if (awareness > 60.0f)
		{
			HigherCaution(Cautious);
		}
		if (awareness > 100.0f)
		{
			HigherCaution(Hostile);
		}

		lastSeen = 0.0f;
		lastKnownPosition = pPlayer->pos;
	}
	else
	{
		lastSeen += DT;
	}
}


bool NPC::CheckDegree(float theta)
{
	float degree = 30.0f * DtoR; // 위아래로 확인할 각도

	if (dir)
	{
		float headtheta = tex->GetRotation();

		if (theta > headtheta - degree &&
			theta < headtheta + degree)
		{
			return true;
		}
	}
	else // 왼쪽
	{
		float headtheta = tex->GetRotation() + PI;
		// 좌우반전 돼 있기 때문에 실제로는 PI 더해야 함

		while (headtheta >= 2 * PI)
		{
			headtheta -= 2 * PI;
		}
		while (headtheta < 0)
		{
			headtheta += 2 * PI;
		}
		// 범위: 0 ~ 2 * PI

		while (theta >= 2 * PI)
		{
			theta -= 2 * PI;
		}
		while (theta < 0)
		{
			theta += 2 * PI;
		}
		// 범위: 0 ~ 2 * PI

		if (theta > headtheta - degree &&
			theta < headtheta + degree)
		{
			return true;
		}
	}

	return false;
}


void NPC::SetAsRedGuy()
{
	redguy = true;
	tex->display = false;
	tex = redhead;
	redhead->display = true;
}

void NPC::WitnessBody()
{
	seeingbody = false;

	curBody = bodySight->Get(L"dead");

	if (curBody != nullptr)
	{
		seeingbody = true;
	}

	if (seeingbody == true && pPlayer->hp > 0)
	{
		HigherCaution(Cautious);

		bool checkifnew = true;

		auto iter = bodySeen.begin();
		auto end = bodySeen.end();

		for (; iter != end; ++iter)
		{
			if (*iter == curBody)
			{
				checkifnew = false;
				break;
			}
		}

		if (checkifnew)
		{
			NewSuspicious(curBody->pos);
			bodySeen.push_back(curBody);
		}
		
		awareness += 30.0f * DT;

		if (awareness > 100.0f)
		{
			HigherCaution(Hostile);
		}
	}

	// 경계선

	bodySight->startpos = pos;
	bodySight->endpos = pos;

	if (dir)
	{
		bodySight->endpos.x += 2000.0f;
	}
	else
	{
		bodySight->endpos.x -= 2000.0f;
	}
}


void NPC::HearSound()
{
	ColCom* sound = col->Get(L"sound");
	if (sound != nullptr)
	{
		NewSuspicious(sound->pos);
		awareness += 10.0f * DT;
		
		if (sound->radius >= 800.0f)
		{
			HigherCaution(Cautious);
		}
		
		HigherCaution(Suspicious);
		if (awareness >= 60.0f)
		{
			HigherCaution(Cautious);
		}
		if (awareness >= 100.0f)
		{
			HigherCaution(Hostile);
		}
	}
}


void NPC::NewSuspicious(VEC2 sus)
{
	suspoint = sus;

	if (justSuspicious == false)
	{
		awareness += 10.0f;
	}

	justSuspicious = true;

	HigherCaution(Suspicious);
	if (awareness >= 60.0f)
	{
		HigherCaution(Cautious);
	}
	if (awareness >= 100.0f)
	{
		HigherCaution(Hostile);
	}
}


void NPC::SusUpdate()
{
	if (state == Stun)
	{
		NewSuspicious(pPlayer->pos);
		HigherCaution(Hostile);
		checked = false;
		lastKnownPosition = pPlayer->pos;
	}

	if (justSuspicious)
	{
		sustime += DT;

		if (sustime > suswait)
		{
			justSuspicious = false;
		}
	}
	else
	{
		sustime = 0.0f;
	}
}


void NPC::HigherCaution(NPCState caution)
{
	if (caution > nextAware)
	{
		nextAware = caution;
	}
}


void NPC::ChangeAware(NPCState state)
{
	nextAware = state;
}


void NPC::AwareChange()
{
	if (aware == nextAware)
	{
		return;
	}

	switch (nextAware)
	{
	case Unaware:
		UnawareSetting();
		break;
	case Suspicious:
		SuspicousSetting();
		break;
	case Cautious:
		CautiousSetting();
		break;
	case Hostile:
		HostileSetting();
		break;
	default:
		break;
	}

	aware = nextAware;
}