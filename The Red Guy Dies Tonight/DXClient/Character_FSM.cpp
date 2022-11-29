#include "Character.h"
#include <Cam.h>
#include "Bullet.h"
#include "BulletCase.h"
#include "RappelWall.h"
#include "SoundRing.h"
#include "Door.h"
#include "Window.h"
#include "Stairs.h"



void Character::Update1() // State Setting
{
	if (state != nextState)
	{
		switch (nextState)
		{
		case Falling:
			FallingSetting();
			break;
		case OnLand:
			OnLandSetting();
			break;
		case Rappelling:
			RappellingSetting();
			break;
		case Stun:
			StunSetting();
			break;
		case Death:
			DeathSetting();
			break;
		case DeathFall:
			DeathFallSetting();
			break;
		case Breaching:
			BreachingSetting();
			break;
		default:
			break;
		}

		state = nextState;
	}

	GunChange();
}


void Character::Update4() // State Update
{
	switch (state)
	{
	case OnLand:
		OnLandUpdate();
		break;
	case Falling:
		FallingUpdate();
		break;
	case Rappelling:
		RappellingUpdate();
		break;
	case Stun:
		StunUpdate();
		break;
	case DeathFall:
		DeathFallUpdate();
		break;
	case Breaching:
		BreachingUpdate();
		break;
	default:
		break;
	}

	LR = 0;
	UD = 0;
}



void Character::OnLandSetting()
{
	speed = { 0.0f, 0.0f };
	holdTheta = 0.0f;
	flipStandard = 90.0f;
	rope->display = false;
	if (col->Find(L"floor"))
	{
		SetPos({ pos.x, col->Get(L"floor")->GetPos().y + col->Get(L"floor")->GetSize().y + col->size.y });
	}
	
}


void Character::OnLandUpdate()
{
	// dir ����
	if (aiming)
	{
		if (pointer.x > pos.x) // ���� ����
		{
			dir = 1;
		}
		else // ���� ����
		{
			dir = 0;
		}
	}
	else
	{
		if (LR > 0) // ���� �̵�
		{
			dir = 1;
		}
		else if (LR < 0) // ���� �̵�
		{
			dir = 0;
		}
	}

	if (dir)
	{
		ani->Flip(false);
		tex->Flip(false);
		arm->Flip(false);
	}
	else
	{
		ani->Flip(true);
		tex->Flip(true);
		arm->Flip(true);
	}


	// �Ǻ� ����
	if (crouch == false)
	{
		if (dir)
		{
			ani->Flip(false);
			tex->SetPivot({ 6.0f, 80.0f });
			arm->SetPivot({ 0.0f, 68.0f });
		}
		else
		{
			ani->Flip(true);
			tex->SetPivot({ -6.0f, 80.0f });
			arm->SetPivot({ 0.0f, 68.0f });
		}
	}
	else
	{
		if (dir)
		{
			ani->Flip(false);
			tex->SetPivot({ 6.0f, 20.0f });
			arm->SetPivot({ 0.0f, 8.0f });
		}
		else
		{
			ani->Flip(true);
			tex->SetPivot({ -6.0f, 20.0f });
			arm->SetPivot({ 0.0f, 8.0f });
		}
	}

	// �ִϸ��̼� ����

	bool blocktest = false; // ���� �׽�Ʈ

	if (BlockedByWall() || BlockedByDoor() || BlockedByGlass() || LR == 0)
	{
		blocktest = true;
	}

	if (blocktest == false)
	{
		if (crouch == false && boost == true) // �޸��� ��
		{
			speed.x = 280.0f;

			if (LR > 0) // ���������� ���� ��
			{
				if (dir) // ������ ���� ��
				{
					if (redguy)
					{
						ani->Change(L"red_run");
					}
					else
					{
						ani->Change(L"run");
					}
				}
				else // ���� ���� ��
				{
					if (redguy)
					{
						ani->Change(L"red_run_back");
					}
					else
					{
						ani->Change(L"run_back");
					}
				}
			}
			else // �������� ���� ��
			{
				speed.x *= -1.0f;

				if (dir) // ������ ���� ��
				{
					if (redguy)
					{
						ani->Change(L"red_run_back");
					}
					else
					{
						ani->Change(L"run_back");
					}
				}
				else // ���� ���� ��
				{
					if (redguy)
					{
						ani->Change(L"red_run");
					}
					else
					{
						ani->Change(L"run");
					}
				}
			}

			MakeStepNoise(700.0f);
		}
		else if (crouch == false) // �� �ִ� ��
		{
			speed.x = 100.0f;

			if (LR > 0) // ���������� ���� ��
			{
				if (dir) // ������ ���� ��
				{
					if (redguy)
					{
						ani->Change(L"red_walk");
					}
					else
					{
						ani->Change(L"walk");
					}
				}
				else // ���� ���� ��
				{
					if (redguy)
					{
						ani->Change(L"red_walkback");
					}
					else
					{
						ani->Change(L"walkback");
					}
				}
			}
			else // �������� ���� ��
			{
				speed.x *= -1.0f;

				if (dir) // ������ ���� ��
				{
					if (redguy)
					{
						ani->Change(L"red_walkback");
					}
					else
					{
						ani->Change(L"walkback");
					}
				}
				else // ���� ���� ��
				{
					if (redguy)
					{
						ani->Change(L"red_walk");
					}
					else
					{
						ani->Change(L"walk");
					}
				}
			}

			MakeStepNoise(200.0f);
		}
		else // �ɾ� ����
		{
			speed.x = 80.0f;

			if (LR > 0) // ���������� ���� ��
			{
				if (dir) // ������ ���� ��
				{
					ani->Change(L"crouchwalk");
				}
				else
				{
					ani->Change(L"crouchback");
				}
			}
			else // �������� ���� ��
			{
				speed.x *= -1.0f;

				if (dir) // ������ ���� ��
				{
					ani->Change(L"crouchback");
				}
				else
				{
					ani->Change(L"crouchwalk");
				}
			}
			MakeStepNoise(1.0f);
		}
	}
	else // ����
	{
		speed.x = 0.0f;
		if (crouch == false)
		{
			if (redguy)
			{
				ani->Change(L"red_stand");
			}
			else
			{
				ani->Change(L"stand");
			}
		}
		else
		{
			ani->Change(L"crouch");
		}
	}

	Move(speed * DT); // �̵���Ű��


	// ���� ���, ���縵 ����, �� ����/�ݱ�, ��� ����������

	if (gun.melee == false)
	{
		BulletFire();
	}
	else
	{
		tex->SetRotation(0.0f);
		arm->SetRotation(0.0f);
	}

	if (doortry)
	{
		doortry = false;

		if (nearchara->Find(L"door"))
		{
			((Door*)nearchara->Get(L"door")->pActor)->Toggle();
		}
	}
	if (rappeltry)
	{
		rappeltry = false;

		bool rappeltest = true;

		if (col->Find(L"wallarea") == false)
		{
			rappeltest = false;
		}
		else // ���縵�� ���� ����
		{
			RappelWall* wall = (RappelWall*)col->Get(L"wallarea")->pActor;

			if (nearchara->Find(L"door"))
			{
				Door* door = (Door*)nearchara->Get(L"door")->pActor;

				if (door->open == false) // ���� ���� ��ó�� ����
				{
					if (wall->dir) // ���� �ܺ�
					{
						if (wall->pos.x < pos.x)
						{
							rappeltest = false;
						}
					}
					else // ������ �ܺ�
					{
						if (wall->pos.x > pos.x)
						{
							rappeltest = false;
						}
					}
				}
			}
			if (nearchara->Find(L"window"))
			{
				ColCom* window = nearchara->Get(L"window"); // ����â�� ��ó�� ����

				if (wall->dir) // ���� �ܺ�
				{
					if (window->pos.x < pos.x)
					{
						rappeltest = false;
					}
				}
				else // ������ �ܺ�
				{
					if (window->pos.x > pos.x)
					{
						rappeltest = false;
					}
				}
			}
		}

		if (rappeltest)
		{
			ChangeState(Rappelling);
		}
	}

	if (stairsready)
	{
		if (stairstry)
		{
			stairstry = false;

			ColCom* stairscom = col->Get(L"stairs");

			if (stairscom != nullptr)
			{
				Stairs* stairs = (Stairs*)stairscom->pActor;

				if (UD == 1)
				{
					if (stairs->up)
					{
						stairsready = false;

						Move({ 0.0f, 600.0f });
					}
				}
				else
				{
					if (stairs->down)
					{
						stairsready = false;

						Move({ 0.0f, -600.0f });
					}
				}
			}
		}
	}
	else
	{
		stairstime += DT;

		if (stairstime > 1.0f)
		{
			stairstime = 0.0f;
			stairsready = true;
		}
	}

	if (intro == false)
	{
		CameraMove(); // ī�޶� �̵�
	}


	// StateChange
	if (col->Find(L"bullet"))
	{
		ChangeState(Stun);
		if (col->Get(L"bullet")->hitpoint.y > tex->GetPos().y && player == false)
		{
			hp = -100;
			ChangeState(Death);
		}

		if (col->Get(L"bullet")->startpos.x > pos.x) // �����ʿ��� ��
		{
			if (dir) // �������� ���� �־���
			{
				stunforward = 0;
			}
			else // ������ ���� �־���
			{
				stunforward = 1;
			}
		}
		else // ���ʿ��� ��
		{
			if (dir) // �������� ���� �־���
			{
				stunforward = 1;
			}
			else // ������ ���� �־���
			{
				stunforward = 0;
			}
		}

		crouch = false; // ���� �⸳
	}
	if (col->Find(L"floor") == false)
	{
		ChangeState(Falling);
	}
	if (hp <= 0)
	{
		ChangeState(Death);
	}
}


void Character::RappellingSetting()
{
	tex->display = true;
	arm->display = true;
	ani->SetRotation(0.0f);

	if (gun.aim == L"machete")
	{
		ChangeGun(L"ak47");
	}

	ColCom* wall = col->Get(L"wallarea");

	speed = { 0.0f, 0.0f };

	dir = ((RappelWall*)wall->pActor)->dir; // ���縵 ����

	rope->display = true;

	ropeend.x = wall->GetPos().x;
	ropeend.y = wall->GetPos().y + wall->GetSize().y;

	if (dir)
	{
		arm->Flip(false);
		tex->Flip(false);
		arm->SetPivot({ -23.0f, 62.0f });
		tex->SetPivot({ -25.0f, 68.0f });
		// x ��ġ ����
		SetPos({ wall->GetPos().x - wall->GetSize().x + 100.0f - 86.0f, pos.y });
		if (pos.y > wall->GetPos().y + wall->GetSize().y)
		{
			// ������ �Ʒ��� ���縵 �õ��� ��� y ��ġ ����
			SetPos({ pos.x, wall->GetPos().y + wall->GetSize().y });
		}
	}
	else
	{
		arm->Flip(true);
		tex->Flip(true);
		arm->SetPivot({ 23.0f, 62.0f });
		tex->SetPivot({ 25.0f, 68.0f });
		// x ��ġ ����
		SetPos({ wall->GetPos().x + wall->GetSize().x - 100.0f + 86.0f, pos.y });
		if (pos.y > wall->GetPos().y + wall->GetSize().y)
		{
			// ������ �Ʒ��� ���縵 �õ��� ��� y ��ġ ����
			SetPos({ pos.x, wall->GetPos().y + wall->GetSize().y });
		}
	}

	speed = { 0.0f, 0.0f };
	holdTheta = 30.0f;
	flipStandard = 110.0f;
}


void Character::RappellingUpdate()
{
	// ���� ���� ���縵
	if (dir)
	{
		ani->Flip(false);
	}
	else
	{
		ani->Flip(true);
	}

	float UDspeed = 80.0f;
	if (UD > 0)
	{
		ani->Change(L"rappelup");

		ColCom* wall = col->Get(L"wallarea");

		if (pos.y > wall->GetPos().y + wall->GetSize().y) // �� ��
		{
			ChangeState(OnLand);
			crouch = true;
			SetPos({ wall->GetPos().x, wall->GetPos().y + wall->GetSize().y + col->GetSize().y });
			speed.y = 0.0f;
		}
		else
		{
			speed.y = UDspeed;
		}
		MakeStepNoise(10.0f);
	}
	else if (UD < 0)
	{
		ani->Change(L"rappeldown");

		if (col->Find(L"floor"))
		{
			ChangeState(OnLand);
			crouch = false;
			speed.y = 0.0f;
		}
		else
		{
			speed.y = -UDspeed;
		}
		MakeStepNoise(10.0f);
	}
	else
	{
		ani->Change(L"rappel");
		speed.y = 0.0f;
	}

	// �̵�
	Move(speed * DT);

	// �Ѿ�
	if (gun.melee == false)
	{
		BulletFire();
	}
	else
	{
		if (dir)
		{
			arm->SetRotation(30.0f * DtoR);
		}
		else
		{
			arm->SetRotation(-30.0f * DtoR);
		}
		tex->SetRotation(0.0f);
	}

	// ����
	VEC2 gap = ropeend - pos;
	VEC2 ropepos = pos + gap / 2.0f;
	float ropelen = gap.scalar() / 2.0f;
	rope->SetPos(ropepos);
	rope->SetSize({ ropelen, 2.0f });
	rope->SetRotation(gap.theta());

	// ī�޶�

	CameraMove();

	// state
	if (hp <= 0)
	{
		ChangeState(DeathFall);
	}
	if (breachtry)
	{
		breachtry = false;

		ColCom* frame = nearchara->Get(L"frame");
		if (frame != nullptr)
		{
			if (pos.y < frame->pos.y + frame->size.y - 200.0f &&
				pos.y > frame->pos.y - frame->size.y + 100.0f)
			{
				ChangeState(Breaching);
			}
		}
	}
}



void Character::BreachingSetting()
{
	charging = true;
	prevcharging = true;
	chargetime = 0.0f;
	fly = false;
	breachstarted = false;

	if (dir)
	{
		tex->SetRotation(0.0f);
		arm->Change(gun.hold);
		arm->SetRotation(30.0f * DtoR);
		arm->Flip(false);
	}
	else
	{
		tex->SetRotation(0.0f);
		arm->Change(gun.hold);
		arm->SetRotation(-30.0f * DtoR);
		arm->Flip(true);
	}
}


void Character::BreachingUpdate()
{
	if (charging && breachstarted == false) // ������ �� ������ ��
	{
		ani->Change(L"rappelcharge");

		chargetime += DT;

		if (chargetime > 0.6f)
		{
			chargetime = 0.6f;
		}
	}
	else
	{
		if (prevcharging == true)
		{
			prevcharging = false;

			breachstarted = true;

			ani->Change(L"rappelkick");

			float power = 400.0f * (chargetime / 0.6f);

			VEC2 gap = ropeend - pos;
			
			VEC2 powerdir = gap;

			if (dir)
			{
				powerdir.rotate(PI / 2);
			}
			else
			{
				powerdir.rotate(-PI / 2);
			}

			speed = powerdir.dir() * power;

			fly = false;

			firstlen = gap.scalar();

			goingback = false;

			gettingin = false;

			breachtime = 0.0f;
		}

		// ���� �, ���� ����

		if (fly == false)
		{
			if (speed.y < 0) // �ϰ� ����
			{
				if (goingback == false)
				{
					ani->Change(L"rappelbreach");
				}
				goingback = true;
			}

			// ����â ����
			if (col->Find(L"window"))
			{
				if (speed.scalar() > 300.0f) // ����� �����ٸ� â���� ���߸�
				{
					((Window*)col->Get(L"window")->pActor)->Break();
					TimeClass::Inst().SlowMo(0.3f);
				}
				else
				{
					ChangeState(Rappelling); // �ƴ� ��� ���縵���� �ٽ� ���ư�. ��ġ�� �˾Ƽ� ������.
				}
			}
			if (col->Find(L"wall"))
			{
				ChangeState(Rappelling);
			}

			// �̵�
			VEC2 prevGap = ropeend - pos;
			acc = prevGap.dir() * speed.scalar() * speed.scalar() / prevGap.scalar(); // ���ɷ�
			VEC2 Gravity = gravity;
			float innertheta = PI / 2 - prevGap.theta();
			VEC2 tension = prevGap.dir() * cosf(innertheta) * Gravity.scalar();
			acc += Gravity + tension; // �߷� + ���
			speed += acc * DT;
			Move(speed * DT);
			VEC2 reGap = ropeend - pos;
			SetPos(ropeend - reGap.dir() * firstlen); // ����

			// ����
			VEC2 gap = ropeend - pos;
			VEC2 ropepos = pos + gap / 2.0f;
			float ropelen = gap.scalar() / 2.0f;
			rope->SetPos(ropepos);
			rope->SetSize({ ropelen, 2.0f });
			rope->SetRotation(gap.theta());
			
			BulletFire();


			if (dir)
			{
				if (pos.x > ropeend.x)
				{
					if (gettingin == false)
					{
						int num = (int)pos.y / 600; // 0���� ����
						ropeend.y = 600.0f * (float)num + 500.0f;
						firstlen = (ropeend - pos).scalar();
					}

					gettingin = true;
				}
			}
			else
			{
				if (pos.x < ropeend.x)
				{
					if (gettingin == false)
					{
						int num = (int)pos.y / 600; // 0���� ����
						ropeend.y = 600.0f * (float)num + 500.0f;
						firstlen = (ropeend - pos).scalar();
					}

					gettingin = true;
				}
			}

			if (gettingin)
			{
				breachtime += DT;

				if (breachtime > 0.3f)
				{
					fly = true;
				}
			}

		}
		else // fly
		{
			ani->Change(L"rappelbreachin");
			rope->display = false;

			BulletFire();

			if (col->Find(L"floor"))
			{
				ChangeState(OnLand);
				crouch = true;
				TimeClass::Inst().SlowMo(1.0f);
			}

			speed += gravity * DT;

			Move(speed * DT);

		}
	}
	CameraMove();

	if (hp <= 0)
	{
		ChangeState(DeathFall);
	}
}