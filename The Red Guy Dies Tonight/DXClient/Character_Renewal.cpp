#include "Character.h"
#include <Cam.h>
#include "Bullet.h"
#include "BulletCase.h"
#include "RappelWall.h"
#include "SoundRing.h"
#include "Door.h"


void Character::BulletFire()
{
	// �� ó��
	if (aiming) // ����
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
		else // ���� ���� ��
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
	else // ������
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
			arm->SetRotation(holdTheta * DtoR); // �Ϲ������� 0.0f, ���Ӹ� �ÿ� 30.0f (�ణ ���ø�)
		}
		else
		{
			tex->Flip(true);
			arm->Flip(true);
			arm->SetRotation(-holdTheta * DtoR);
		}
	}

	// �Ѿ� ���
	if (fireready) // ����� �غ�
	{
		if (firing) // ��� ��
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

				// Bullet ����
				Scene* scene = Scene::GetCurScene();

				scene->AddActor(new Bullet{ armpos, gap.theta(), col });

				// SoundRing ����
				VEC2 soundpos = armpos + gap.dir() * gun.firelen;
				scene->AddActor(new SoundRing{ soundpos, gun.noise });

				// �ѼҸ�
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

				// ź��
				VEC2 casepos = armpos + gap.dir() * gun.caselen; // ź�� ��ġ
				float random = (float)(rand() % 20); // ź�� ����
				if (dir) // ź�� ����
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
	else // ��� �ð�
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