#include "Character.h"
#include <Cam.h>
#include "Bullet.h"
#include "BulletCase.h"
#include "RappelWall.h"
#include "SoundRing.h"
#include "Door.h"
#include <list>
#include "NPC.h"

Character::Character()
{
	// 머리
	tex = (TexCom*)AddCom(new TexCom);
	tex->Init(L"../tex/head.png");
	tex->name = L"head";
	tex->SetZ(3.0f);
	
	redhead = (TexCom*)AddCom(new TexCom);
	redhead->Init(L"../tex/red/head.png");
	redhead->name = L"redhead";
	redhead->display = false;

	rope = (TexCom*)AddCom(new TexCom);
	rope->Init(L"../tex/black.png");
	rope->display = false;

	// 충돌체 세팅
	col = (ColCom*)AddCom(new ColCom);
	col->SetSize({ 30.0f, 98.0f });
	col->character = true;
	ColMgr::Inst().Add(L"character", col);

	nearchara = (ColCom*)AddCom(new ColCom);
	nearchara->SetSize({ 120.0f, 98.0f });
	ColMgr::Inst().Add(L"nearchara", nearchara);

	dead = (ColCom*)AddCom(new ColCom);
	dead->pass = true;
	dead->SetSize({ 30.0f, 98.0f });
	ColMgr::Inst().Add(L"dead", dead);

	// 모션 세팅 (몸통)
	ani = (AniState*)AddCom(new AniState);
	ani->SetZ(3.0f);

	AniData* walk = new AniData;
	walk->Load(L"../tex/walk/walk", 4);
	ani->Add(L"walk", walk);
	
	AniData* walkback = new AniData;
	walkback->Load(L"../tex/walkback/walkback", 4);
	ani->Add(L"walkback", walkback);

	AniData* stand = new AniData;
	stand->Load(L"../tex/stand");
	ani->Add(L"stand", stand);

	AniData* fall = new AniData;
	fall->Load(L"../tex/fall");
	ani->Add(L"fall", fall);

	AniData* crouch = new AniData;
	crouch->Load(L"../tex/crouch");
	ani->Add(L"crouch", crouch);

	AniData* crouchwalk = new AniData;
	crouchwalk->Load(L"../tex/crouchwalk/crouchwalk", 4, 0.13f);
	ani->Add(L"crouchwalk", crouchwalk);

	AniData* crouchback = new AniData;
	crouchback->Load(L"../tex/crouchback/crouchback", 4, 0.13f);
	ani->Add(L"crouchback", crouchback);

	AniData* rappel = new AniData;
	rappel->Load(L"../tex/rappel");
	ani->Add(L"rappel", rappel);

	AniData* rappelup = new AniData;
	rappelup->Load(L"../tex/rappelup/rappelup", 4);
	ani->Add(L"rappelup", rappelup);

	AniData* rappeldown = new AniData;
	rappeldown->Load(L"../tex/rappeldown/rappeldown", 4);
	ani->Add(L"rappeldown", rappeldown);

	AniData* death = new AniData(L"../tex/death/death", 7, 0.13f, false);
	ani->Add(L"death", death);

	AniData* deathfall = new AniData(L"../tex/deathfall_faceup", 1, 0.4f, false);
	ani->Add(L"deathfall", deathfall);

	ani->Add(L"death_faceup_fall", new AniData(L"../tex/death_faceup_fall/death_faceup_fall", 4, 0.13f, false));

	ani->Add(L"run", new AniData(L"../tex/run/run", 4, 0.1f));

	ani->Add(L"run_back", new AniData(L"../tex/run_back/run_back", 4, 0.1f));

	AniData* stab = new AniData(L"../tex/stab/stab", 8, 0.13f);
	machete.stab = L"stab";
	ani->Add(machete.stab, stab);

	AniData* stabfront = new AniData(L"../tex/stab_front/stab_front", 8, 0.13f);
	machete.stabfront = L"stab_front";
	ani->Add(machete.stabfront, stabfront);

	ani->Add(L"rappelcharge", new AniData(L"../tex/rappelcharge/rappelcharge", 5, 0.1f, false));

	ani->Add(L"rappelbreachin", new AniData(L"../tex/rappelbreachin"));

	ani->Add(L"rappelbreach", new AniData(L"../tex/rappelbreach"));
	ani->Add(L"rappelkick", new AniData(L"../tex/rappelkick"));

	ani->Change(L"stand");


	// 모션 세팅 (총기)
	arm = (AniState*)AddCom(new AniState);
	arm->SetZ(3.0f);

	ak47.SetInfo(L"ak47");
	ak47.firelen = 100.0f;
	ak47.firerate = 0.1f;
	ak47.caselen = 44.0f;
	ak47.noise = 1000.0f;
	ak47.aimtex = new AniData;
	ak47.aimtex->Load(L"../tex/ak47");
	arm->Add(L"ak47", ak47.aimtex);
	ak47.holdtex = new AniData;
	ak47.holdtex->Load(L"../tex/ak47hold");
	arm->Add(L"ak47hold", ak47.holdtex);
	ak47.falltex = new AniData;
	ak47.falltex->Load(L"../tex/ak47fall");
	arm->Add(L"ak47fall", ak47.falltex);

	AniData* stun_ak47 = new AniData(L"../tex/stun_ak47/stun_ak47", 4, 0.1f, false);
	ak47.stun = L"stun_ak47";
	ani->Add(ak47.stun, stun_ak47);

	AniData* stun_forward_ak47 = new AniData(L"../tex/stun_forward_ak47/stun_forward_ak47", 4, 0.1f, false);
	ak47.stun_forward = L"stun_forward_ak47";
	ani->Add(ak47.stun_forward, stun_forward_ak47);


	spistol.SetInfo(L"spistol");
	spistol.firelen = 80.0f;
	spistol.firerate = 0.3f;
	spistol.caselen = 60.0f;
	spistol.noise = 200.0f;
	spistol.aimtex = new AniData;
	spistol.aimtex->Load(L"../tex/spistol");
	arm->Add(L"spistol", spistol.aimtex);
	spistol.holdtex = new AniData;
	spistol.holdtex->Load(L"../tex/spistolhold");
	arm->Add(L"spistolhold", spistol.holdtex);
	spistol.falltex = new AniData;
	spistol.falltex->Load(L"../tex/spistolfall");
	arm->Add(L"spistolfall", spistol.falltex);

	spistol.stun = ak47.stun;
	spistol.stun_forward = ak47.stun_forward;



	pistol.SetInfo(L"pistol");
	pistol.firelen = 60.0f;
	pistol.firerate = 0.3f;
	pistol.caselen = 60.0f;
	pistol.noise = 800.0f;
	pistol.aimtex = new AniData;
	pistol.aimtex->Load(L"../tex/pistol");
	arm->Add(L"pistol", pistol.aimtex);
	pistol.holdtex = new AniData;
	pistol.holdtex->Load(L"../tex/pistolhold");
	arm->Add(L"pistolhold", pistol.holdtex);
	pistol.falltex = new AniData;
	pistol.falltex->Load(L"../tex/pistolfall");
	arm->Add(L"pistolfall", pistol.falltex);

	pistol.stun = ak47.stun;
	pistol.stun_forward = ak47.stun_forward;

	pistol.stun = L"stun_pistol";
	ani->Add(pistol.stun, new AniData(L"../tex/stun_pistol/stun_pistol", 4, 0.1f, false));

	pistol.stun_forward = L"stun_forward_pistol";
	ani->Add(pistol.stun_forward, new AniData(L"../tex/stun_forward_pistol/stun_forward_pistol", 4, 0.1f, false));


	machete.SetInfo(L"machete");
	machete.melee = true;
	machete.noise = 200.0f; // 사살 시 소음
	machete.holdtex = new AniData(L"../tex/machete");
	arm->Add(L"machetehold", machete.holdtex);
	machete.falltex = new AniData(L"../tex/machetefall");
	arm->Add(L"machetefall", machete.falltex);
	machete.swing = new AniData(L"../tex/swing/swing", 3, 0.13f, false);
	arm->Add(L"swing", machete.swing);
	machete.swing2 = new AniData(L"../tex/swing2/swing2", 3, 0.13f, false);
	arm->Add(L"swing2", machete.swing2);

	machete.stun = ak47.stun;
	machete.stun_forward = ak47.stun_forward;

	// red 버전

	ani->Add(L"red_death", new AniData(L"../tex/red/death/death", 7, 0.13f, false));
	ani->Add(L"red_run", new AniData(L"../tex/red/run/run", 4, 0.1f));
	ani->Add(L"red_run_back", new AniData(L"../tex/red/run_back/run_back", 4, 0.1f));
	ani->Add(L"red_walk", new AniData(L"../tex/red/walk/walk", 4, 0.13f));
	ani->Add(L"red_walkback", new AniData(L"../tex/red/walkback/walkback", 4, 0.13f));
	ani->Add(L"red_stun", new AniData(L"../tex/red/stun_pistol/stun_pistol", 4, 0.1f, false));
	ani->Add(L"red_stun_forward", new AniData(L"../tex/red/stun_forward_pistol/stun_forward_pistol", 4, 0.1f, false));
	ani->Add(L"red_stand", new AniData(L"../tex/red/stand"));
	arm->Add(L"red_pistol", new AniData(L"../tex/red/pistol"));
	arm->Add(L"red_pistolhold", new AniData(L"../tex/red/pistolhold"));


	gun = ak47;
	arm->Change(gun.hold);

	// 위치 세팅
	SetPos({ 200.0f, 98.0f });
	ChangeState(OnLand);
	OnLandSetting();
}


Character::~Character()
{
	int a = 0;
}


void Character::ChangeState(MotionState _state)
{
	nextState = _state;
}


void Character::ChangeGun(wstring gunname)
{
	nextGun = gunname;
}



void Character::FallingSetting()
{
	arm->Change(gun.fall);
	arm->SetRotation(0.0f);
	ani->Change(L"fall");
	tex->SetRotation(0.0f);

	if (dir)
	{
		arm->SetPivot({ -10.0f, 80.0f });
		tex->SetPivot({ -10.0f, 88.0f });
	}
	else
	{
		arm->SetPivot({ 10.0f, 80.0f });
		tex->SetPivot({ 10.0f, 88.0f });
	}
	
}



void Character::StunSetting()
{
	if (dir)
	{
		tex->SetPivot({ 6.0f, 80.0f });
		arm->SetPivot({ 0.0f, 68.0f });
	}
	else
	{
		tex->SetPivot({ -6.0f, 80.0f });
		arm->SetPivot({ 0.0f, 68.0f });
	}
	if (stunforward)
	{
		if (redguy)
		{
			ani->Change(L"red_stun_forward");
		}
		else
		{
			ani->Change(gun.stun_forward);
		}
	}
	else
	{
		if (redguy)
		{
			ani->Change(L"red_stun");
		}
		else
		{
			ani->Change(gun.stun);
		}
	}
}

void Character::DeathSetting()
{
	if (redguy)
	{
		ani->Change(L"red_death");
		NPC::redguydied = true;
	}
	else
	{
		ani->Change(L"death");
	}
	arm->display = false;
	tex->display = false;
	col->pass = true;
	dead->pass = false;
	if (player)
	{
		Cam::Inst().SetFocus(armpos);
	}
}


void Character::FallingUpdate()
{
	if (col->Find(L"floor"))
	{
		ChangeState(OnLand);
		if (speed.y < -100.0f)
		{
			crouch = true;
		}
		else
		{
			crouch = false;
		}
		speed = { 0.0f, 0.0f };
	}
	else
	{
		speed.y -= 700.0f * DT;

		Move(speed * DT);

		if (player)
		{
			CameraMove();
		}
		if (hp <= 0)
		{
			ChangeState(DeathFall);
		}
	}
}



void Character::StunUpdate()
{
	if (ani->IsEnd() == false)
	{
		arm->display = false;
		tex->display = false;
		if (dir) // 오른쪽 보는 중
		{
			ani->Flip(false);
			if (stunforward)
			{
				int saveLR = LR;
				LR = 1;
				if (BlockTest == false)
				{
					Move(VEC2{ 60.0f, 0.0f } *DT);
				}
				LR = saveLR;
			}
			else
			{
				int saveLR = LR;
				LR = -1;
				if (BlockTest == false)
				{
					Move(VEC2{ -60.0f, 0.0f } *DT);
				}
				LR = saveLR;
			}
		}
		else
		{
			ani->Flip(true);
			if (stunforward)
			{
				int saveLR = LR;
				LR = -1;
				if (BlockTest == false)
				{
					Move(VEC2{ -60.0f, 0.0f } *DT);
				}
				LR = saveLR;
			}
			else
			{
				int saveLR = LR;
				LR = 1;
				if (BlockTest == false)
				{
					Move(VEC2{ 60.0f, 0.0f } *DT);
				}
				LR = saveLR;
			}
		}
	}
	else
	{
		if (hp > 0)
		{
			arm->display = true;
			tex->display = true;
			ChangeState(OnLand);
			crouch = false;
		}
		else
		{
			ChangeState(Death);
		}
	}
	if (player)
	{
		if (IsHoldDn(L"RClick"))
		{
			Cam::Inst().SetFocus(armpos + (InputClass::Inst().IngameCursor() - armpos) * 0.5f);
		}
		else
		{
			Cam::Inst().SetFocus(armpos);
		}
	}
}



bool Character::BlockedByWall()
{
	ColCom* target = col->Get(L"wall");

	if (target != nullptr) // 벽과 충돌
	{
		if (pos.y > target->GetPos().y + target->GetSize().y) // 벽 위에 있음
		{
			return false; // 벽이 없는 거나 마찬가지
		}
		else // 벽 옆에 있음
		{
			if (LR > 0)
			{
				if (target->GetPos().x > pos.x)
				{
					return true; // 막힘
				}
				else
				{
					return false;
				}
			}
			else
			{
				if (target->GetPos().x < pos.x)
				{
					return true; // 막힘
				}
				else
				{
					return false;
				}
			}
		}
	}
	else
	{
		return false; // 벽이 없음
	}
}

bool Character::BlockedByDoor()
{
	ColCom* target = col->Get(L"door");

	if (target != nullptr) // 문과 충돌
	{
		if (((Door*)target->pActor)->open == false)
		{
			if (LR > 0)
			{
				if (target->GetPos().x > pos.x)
				{
					return true; // 막힘
				}
				else
				{
					return false;
				}
			}
			else
			{
				if (target->GetPos().x < pos.x)
				{
					return true; // 막힘
				}
				else
				{
					return false;
				}
			}
		}
		else // 문이 열려 있음
		{
			return false;
		}
	}
	else
	{
		return false; // 문이 없음
	}
}

bool Character::BlockedByGlass()
{
	ColCom* target = col->Get(L"window");

	if (target != nullptr) // 창문과 충돌
	{
		if (pos.y > target->GetPos().y + target->GetSize().y) // 창문 위에 있음
		{
			return false; // 창문이 없는 거나 마찬가지
		}
		else // 창문 옆에 있음
		{
			if (LR > 0)
			{
				if (target->GetPos().x > pos.x)
				{
					return true; // 막힘
				}
				else
				{
					return false;
				}
			}
			else
			{
				if (target->GetPos().x < pos.x)
				{
					return true; // 막힘
				}
				else
				{
					return false;
				}
			}
		}
	}
	else
	{
		return false; // 벽이 없음
	}
}


bool Character::NearCliff()
{
	ColCom* floor = col->Get(L"floor");

	if (floor != nullptr)
	{
		if (floor->pos.x - floor->size.x + 100.0f > pos.x)
		{
			return true;
		}
		if (floor->pos.x + floor->size.x - 100.0f < pos.x)
		{
			return true;
		}
	}
	return false;
}


void Character::MakeStepNoise(float noise)
{
	if (player)
	{
		steptime += DT;
		float stepgap = 0.6f;
		if (boost)
		{
			stepgap = 0.4f;
		}
		if (steptime > stepgap)
		{
			Scene::GetCurScene()->AddActor(new SoundRing(
				{ col->pos.x, col->pos.y - col->size.y }, noise));
			steptime = 0.0f;
			SoundClass::Inst().Play(L"footstep");
		}
	}
}


void Character::DeathFallSetting()
{
	ani->Change(L"deathfall");
	arm->display = false;
	tex->display = false;
	rope->display = false;
	col->pass = true;
	col->SetSize({ 10.0f, 8.0f });
	if (player)
	{
		Cam::Inst().SetFocus(armpos);
	}
}


void Character::DeathFallUpdate()
{
	if (col->Find(L"floor"))
	{
		ani->Change(L"death_faceup_fall");
	}
	else
	{
		speed.y -= 600.0f * DT;

		Move(speed * DT);

		Cam::Inst().SetFocus(armpos);
	}
}


 