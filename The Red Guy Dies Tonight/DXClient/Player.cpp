#include "Player.h"
#include <Cam.h>
#include "Door.h"
#include "RappelWall.h"
#include <RenderMgr.h>
#include "NPC.h"


bool Player::killmission = false;

Player::Player(VEC2 _pos)
{
	player = true;
	col->player = true;
	dead->player = true;
	hp = 5000;
	killmission = false;
	intro = true;

	SetPos({ _pos.x, _pos.y + 98.0f });
}


Player::~Player()
{
}


void Player::Update2()
{
	if (IsJustDn(L"I"))
	{
		Scene::ChangeScene(L"EndScene");
	}
	if (SoundClass::Inst().IsPlaying(L"round gunpoint") == false)
	{
		SoundClass::Inst().Play(L"round gunpoint");
	}
	if (IsJustDn(L"O"))
	{
		SoundClass::Inst().Stop(L"round gunpoint");
	}
	if (intro)
	{
		IntroUpdate();
		return;
	}


	if (1) // 치트
	{
		if ((state == OnLand || state == Falling)) // 텔레포트 치트
		{
			if (IsJustDn(L"X"))
			{
				SetPos({ 5250, 4300 }); // 오른쪽 빌딩 옥상 위
			}
			if (IsJustDn(L"V"))
			{
				SetPos({ 1200, 700 }); // 건물 사이
			}
			if (IsJustDn(L"Z"))
			{
				SetPos({ 2800, 1900 }); // 왼쪽 건물 옥상 위
			}
			
		}
		if (IsJustDn(L"T"))
		{
			godmode = !godmode;
		}
		if (IsJustDn(L"B")) // 부활
		{
			ChangeState(OnLand);
			tex->display = true;
			arm->display = true;
			col->pass = false;
			col->SetSize({ 30.0f, 98.0f });
			hp = 5000;
		}
	}

	if (godmode)
	{
		hp = 5000;
	}

	if (IsJustDn(L"Up"))
	{
		Cam::Inst().ChangeSize(zoomlevel -= 0.5f);
	}
	if (IsJustDn(L"Down"))
	{
		Cam::Inst().ChangeSize(zoomlevel += 0.5f);
	}
	if (IsJustDn(L"P"))
	{
		ColCom::Toggle();
	}


	if ((IsHoldDn(L"A") && IsHoldDn(L"D")) ||
		(IsHoldUp(L"A") && IsHoldUp(L"D")))
	{
		LR = 0;
	}
	else
	{
		if (IsHoldDn(L"D"))
		{
			LR = 1;
		}
		else if (IsHoldDn(L"A"))
		{
			LR = -1;
		}
	}

	if ((IsHoldDn(L"W") && IsHoldDn(L"S")) ||
		(IsHoldUp(L"W") && IsHoldUp(L"S")))
	{
		UD = 0;
	}
	else if (IsHoldDn(L"W"))
	{
		UD = 1;
	}
	else if (IsHoldDn(L"S"))
	{
		UD = -1;
	}

	if (IsHoldDn(L"Click") || IsHoldDn(L"RClick"))
	{
		aiming = true;
		pointer = InputClass::Inst().IngameCursor();
	}
	else
	{
		aiming = false;
	}

	if (IsHoldDn(L"Click"))
	{
		firing = true;
	}
	else
	{
		firing = false;
	}

	

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
	case Breaching:
		BreachingUpdate();
		break;
	default:
		break;
	}

	wchar_t arr[3] = {};
	if (magazine >= 0 && magazine < 100)
	{
		arr[0] = magazine / 10 + '0';
		arr[1] = magazine % 10 + '0';
	}
	//RenderMgr::Inst().Font(arr, { WIDTH - 180, HEIGHT - 100 }, 40);
	//RenderMgr::Inst().Font(L"/ 30", { WIDTH - 130, HEIGHT - 70 }, 30);

	if (col->Find(L"exit"))
	{
		if (NPC::redguydied)
		{
			SoundClass::Inst().Stop(L"siren");
			Scene::ChangeScene(L"EndScene");
		}
	}
}

void Player::IntroUpdate()
{
	introtime += DT;

	if (introtime < 3.0f)
	{
		LR = 1;
		Cam::Inst().SetFocus(armpos);
	}
	else if (introtime < 5.0f)
	{
		LR = 0;
		camchange = false;
	}
	else if (introtime < 6.0f)
	{
		if (camchange == false)
		{
			Cam::Inst().ChangeFocus({ 5700, 3600 }, 1);
			camchange = true;
		}
	}
	else if (introtime < 8.0f)
	{
		camchange = false;
		killmission = true;
	}
	else if (introtime < 10.0f)
	{
		if (camchange == false)
		{
			Cam::Inst().ChangeFocus(armpos, 1);
			camchange = true;
		}
	}
	else
	{
		intro = false;
	}

	if (IsJustDn(L"Space"))
	{
		killmission = true;
		intro = false;
	}
}


void Player::FallingUpdate()
{
}


void Player::OnLandUpdate()
{
	if (IsJustDn(L"C"))
	{
		crouch = !crouch;
	}
	
	if (IsJustDn(L"Space"))
	{
		rappeltry = true;
	}

	if (IsJustDn(L"E")) // 상호작용 키
	{
		doortry = true;
	}

	if (IsHoldDn(L"Shift"))
	{
		boost = 1;
	}
	else
	{
		boost = 0;
	}

	if (crouch)
	{
		if (IsJustDn(L"Shift"))
		{
			crouch = false;
		}
	}

	if (IsJustDn(L"1"))
	{
		ChangeGun(L"ak47");
	}
	else if (IsJustDn(L"2"))
	{
		ChangeGun(L"spistol");
	}

	if (UD > 0 || UD < 0)
	{
		stairstry = true;
	}
	else
	{
		stairstry = false;
	}
}


void Player::RappellingUpdate()
{
	if (IsJustDn(L"E")) // 상호작용 키
	{
		if (nearchara->Find(L"door"))
		{
			((Door*)nearchara->Get(L"door")->pActor)->Toggle();
		}
	}

	if (IsJustDn(L"Space"))
	{
		breachtry = true;
	}

	if (IsJustDn(L"1"))
	{
		ChangeGun(L"ak47");
	}
	else if (IsJustDn(L"2"))
	{
		ChangeGun(L"spistol");
	}
}


void Player::BreachingUpdate()
{
	if (IsHoldDn(L"Space"))
	{
		charging = true;
	}
	else
	{
		charging = false;
	}
}