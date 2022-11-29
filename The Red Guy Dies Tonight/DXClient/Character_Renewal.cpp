#include "Character.h"
#include <Cam.h>
#include "Bullet.h"
#include "BulletCase.h"
#include "RappelWall.h"
#include "SoundRing.h"
#include "Door.h"


void Character::BulletFire()
{
	// 팔 처리
	if (aiming) // 조준
	{
		VEC2 gap = pointer - armpos;
		VEC2 headgap = pointer - tex->GetPos();

		bool fliptest = false;

		if (dir)
		{
			if (gap.theta() > flipStandard * DtoR ||
				gap.theta() < -PI + flipStandard * DtoR)
			{
				fliptest = true;
			}
		}
		else // 왼쪽 보는 중
		{
			if (gap.theta() > PI - flipStandard * DtoR ||
				gap.theta() < -flipStandard * DtoR)
			{
				fliptest = true;
			}
		}

		tex->SetRotation(headgap.theta());
		arm->SetRotation(gap.theta());
		if (redguy)
		{
			arm->Change(L"red_pistol");
		}
		else
		{
			arm->Change(gun.aim);
		}

		if (fliptest == false)
		{
			tex->Flip(false);
			arm->Flip(false);
		}
		else
		{
			tex->Flip(true);
			arm->Flip(true);
			tex->Rotate(PI);
			arm->Rotate(PI);
		}
	}
	else // 비조준
	{
		tex->SetRotation(0.0f);
		if (redguy)
		{
			arm->Change(L"red_pistolhold");
		}
		else
		{
			arm->Change(gun.hold);
		}

		if (dir)
		{
			tex->Flip(false);
			arm->Flip(false);
			arm->SetRotation(holdTheta * DtoR); // 일반적으론 0.0f, 래팰링 시엔 30.0f (약간 들어올림)
		}
		else
		{
			tex->Flip(true);
			arm->Flip(true);
			arm->SetRotation(-holdTheta * DtoR);
		}
	}

	// 총알 사격
	if (fireready) // 사격할 준비
	{
		if (firing) // 사격 중
		{
			VEC2 gap = pointer - armpos;

			fireready = false;

			if (magazine <= 0)
			{
				if (emptysound == false)
				{
					emptysound = true;
					SoundClass::Inst().Play(L"emptygun");
				}
			}
			else
			{
				emptysound = false;
				burstfire++;

				// Bullet 액터
				Scene* scene = Scene::GetCurScene();

				scene->AddActor(new Bullet{ armpos, gap.theta(), col });

				// SoundRing 액터
				VEC2 soundpos = armpos + gap.dir() * gun.firelen;
				scene->AddActor(new SoundRing{ soundpos, gun.noise });

				// 총소리
				if (gun.aim == L"ak47")
				{
					SoundClass::Inst().Play(L"gunsound");
				}
				else if (gun.aim == L"spistol")
				{
					SoundClass::Inst().Play(L"silenced");
				}
				else if (gun.aim == L"pistol")
				{
					SoundClass::Inst().Play(L"pistol");
				}

				// 탄피
				VEC2 casepos = armpos + gap.dir() * gun.caselen; // 탄피 위치
				float random = (float)(rand() % 20); // 탄피 각도
				if (dir) // 탄피 방향
				{
					scene->AddActor(new BulletCase{ casepos, gap.theta() + 110.0f * DtoR + random * DtoR });
				}
				else
				{
					scene->AddActor(new BulletCase{ casepos, gap.theta() - 110.0f * DtoR - random * DtoR });
				}
			}
		}
		else
		{
			emptysound = false;
		}
	}
	else // 대기 시간
	{
		firetime += DT;

		if (firetime > gun.firerate)
		{
			firetime = 0.0f;
			fireready = true;
		}
	}
}



void Character::CameraMove(bool fix)
{
	if (player)
	{
		if (fix)
		{
			Cam::Inst().SetFocus(armpos);
		}
		else
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
}


void Character::GunChange()
{
	if (nextGun == L"")
	{
		return;
	}

	if (nextGun == L"ak47")
	{
		gun = ak47;
		arm->Change(gun.hold);
	}
	else if (nextGun == L"spistol")
	{
		gun = spistol;
	}
	else if (nextGun == L"pistol")
	{
		gun = pistol;
	}
	else if (nextGun == L"machete")
	{
		gun = machete;
		arm->Change(gun.hold);
	}

	nextGun = L"";
}