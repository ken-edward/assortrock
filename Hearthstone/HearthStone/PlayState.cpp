#include "stdafx.h"
#include "PlayState.h"
#include "InputMgr.h"
#include "TimeMgr.h"
#include "RenderMgr.h"
#include "MyHero.h"
#include "OpponentHero.h"
#include "EndTurn.h"
#include "Card.h"
#include "ResMgr.h"


PlayState::PlayState()
{
}


PlayState::~PlayState()
{
}


void PlayState::Start()
{
	Hero::MyTurnFlag = true;
	Hero::PrevFlag = false;
	Hero::victory = 0;
	Hero::mulligan = true;
	Hero::IntroOn = true;

	myturnstart = false;
	turnstartcount = 0.0f;
	surrender = false;
	introstart = true;
	anicount = 0;

	Player = CreateActor<MyHero>();
	Opponent = CreateActor<OpponentHero>();
	Player->enemy = Opponent;
	Opponent->enemy = Player;
	Opponent->Start();
	CreateActor<EndTurn>();

	
}



void PlayState::StateUpdate()
{
	

	if (bgm->IsPlay() == false && Hero::mulligan == false)
	{
		bgm->NewSoundPlay(L"Don't Let Your Guard Down.ogg");
	}

	if (Hero::mulligan == true)
	{
		RENDINST.ShowImage(0, 0, L"GameBackground_less.bmp");


		RENDINST.ShowImage(1053, 169, L"Deck_opp_full.bmp");
		RENDINST.ShowImage(1053, 363, L"Deck_my_full.bmp");

		Intro();
	}
	else
	{
		RENDINST.ShowImage(0, 0, L"GameBackground_less.bmp");


		if (Opponent->Deck.size() > 5)
		{
			RENDINST.ShowImage(1053, 169, L"Deck_opp_full.bmp");
		}

		if (Player->Deck.size() > 5)
		{
			RENDINST.ShowImage(1053, 363, L"Deck_my_full.bmp");
		}


		float eScale = 0.3f;

		if (Opponent->Deck.empty())
		{
			RENDINST.ShowImageByScale(1052, 175, L"deck_empty.bmp", eScale);
		}

		if (Player->Deck.empty())
		{
			RENDINST.ShowImageByScale(1056, 380, L"deck_empty.bmp", eScale);
		}
	}

	if (Hero::mulligan == false)
	{
		CheckTurnStart();
	}
	
	if (Hero::victory == 0)
	{
		if (Player->hp <= 0) // 영웅 사망 체크
		{
			ResMgr::Inst().PlaySound(L"defeat_screen_start.ogg");
			Hero::victory = -1;
		}
		else if (Opponent->hp <= 0)
		{
			ResMgr::Inst().PlaySound(L"victory_screen_start.ogg");
			Hero::victory = 1;
		}
	}



	if (Hero::victory != 0)
	{
		if (INPUTINST.FindKey("Click")->IsDown())
		{
			SetNextState(STATE::SELECT);
		}
	}

	if (Hero::mulligan == true && Hero::IntroOn == false)
	{
		if (INPUTINST.ButtonTestByRect(575, 500, 575 + 120, 500 + 90)) // 멀리건 최종 확인
		{
			Hero::mulligan = false;
			Player->changecard();
			Opponent->changecard();
			bgm->NewSoundPlay(L"Don't Let Your Guard Down.ogg");
		}
	}
	else
	{
		for (size_t i = 0; i < Player->Field.size(); i++)
		{
			if (Player->Field[i]->attackable == true
				&& Player->Field[i]->playon == false
				&& Hero::MyTurnFlag == true)
			{
				RENDINST.ShowImage(
					Player->Field[i]->x - Player->Field[i]->ImageX,
					Player->Field[i]->y - Player->Field[i]->ImageY,
					L"Unit_Attackable.bmp");
			}
		}

		HitGround();
	}
	

	/*
	wchar_t str[100]{};
	swprintf_s(str, L"MousePosX: %d", INPUTINST.Cursor().x);
	RENDINST.ShowText(6, 15, str);
	swprintf_s(str, L"MousePosY: %d", INPUTINST.Cursor().y);
	RENDINST.ShowText(6, 35, str);
	swprintf_s(str, L"핸드 카드 수: %d", Opponent->Hand.size());
	RENDINST.ShowText(820, 100, str);
	swprintf_s(str, L"덱 카드 수: %d", Opponent->Deck.size());
	RENDINST.ShowText(1000, 260, str);
	swprintf_s(str, L"현재 탈진: %d", Opponent->fatiguedmg);
	RENDINST.ShowText(1000, 280, str);
	swprintf_s(str, L"핸드 카드 수: %d", Player->Hand.size());
	RENDINST.ShowText(820, 560, str);
	swprintf_s(str, L"덱 카드 수: %d", Player->Deck.size());
	RENDINST.ShowText(1000, 370, str);
	swprintf_s(str, L"현재 탈진: %d", Player->fatiguedmg);
	RENDINST.ShowText(1000, 390, str);
	*/
}




void PlayState::CheckTurnStart()
{
	if (Hero::MyTurnFlag == true
		&& Hero::PrevFlag == false) // 내 턴 시작
	{
		if (Player->Busy == true ||
			Opponent->Busy == true)
		{
			return;
		}

		// 상대 턴 끝
		Opponent->atk -= Opponent->tempatk;
		Opponent->tempatk = 0;

		for (size_t i = 0; i < Opponent->Field.size(); i++)
		{
			if (Opponent->Field[i]->TurnEnd == true)
			{
				Opponent->Field[i]->TurnEndStart = true;
			}
		}

		// 내 턴 시작
		myturnstart = true; // 알림 띄우기용
		Hero::PrevFlag = true;

		if (Player->maxmana < 10)
		{
			Player->maxmana++; // 마나 10까지 증가
		}

		Player->mana = Player->maxmana; // 마나 채우기
		Player->Draw(); // 카드 뽑기
		ResMgr::Inst().PlaySound(L"Turn_Start.ogg"); // 턴 시작 소리
		for (size_t i = 0; i < Player->Field.size(); i++)
		{
			if (Player->Field[i]->notattackable == false)
			{
				Player->Field[i]->attackable = true; // 하수인 공격 가능
			}
		}
		Player->attackable = true; // 영웅 공격 가능
		Player->heropower = true; // 영능 활성화
		
		if (Player->WieldingWeapon != nullptr)
		{
			Player->atk = Player->WieldingWeapon->atk;
			Player->tempatk = Player->WieldingWeapon->atk;
		}
	}




	if (Hero::MyTurnFlag == false
		&& Hero::PrevFlag == true) // 상대 턴 시작
	{
		if (Player->Busy == true ||
			Opponent->Busy == true)
		{
			return;
		}

		// 내 턴 끝
		Player->atk -= Player->tempatk;
		Player->tempatk = 0;

		for (size_t i = 0; i < Player->Field.size(); i++)
		{
			if (Player->Field[i]->TurnEnd == true)
			{
				Player->Field[i]->TurnEndStart = true;
			}
		}

		// 상대 턴 시작
		Hero::PrevFlag = false;
		if (Opponent->maxmana < 10)
		{
			Opponent->maxmana++; // 마나 10까지 증가
		}

		Opponent->mana = Opponent->maxmana; // 마나 채움
		Opponent->Draw(); // 카드 뽑음

		for (size_t i = 0; i < Opponent->Field.size(); i++)
		{
			if (Opponent->Field[i]->notattackable == false)
			{
				Opponent->Field[i]->attackable = true; // 하수인 공격 가능
			}
		}

		Opponent->attackable = true; // 영웅 공격 가능
		Opponent->heropower = true; // 영능 활성화
	}


	if (myturnstart == true)
	{
		if (turnstartcount < 1.5f && Hero::MyTurnFlag == true)
		{
			turnstartcount += DELTATIME;			
		}
		else
		{
			turnstartcount = 0.0f;
			myturnstart = false;
		}
	}
}


void PlayState::HitGround()
{
	if (INPUTINST.FindKey("Click")->IsDown())
	{
		if (INPUTINST.CursorTestByRect(255, 160, 1050, 550)
			&& INPUTINST.CursorTestByRect(985, 300, 1085, 355) == false
			&& Hero::mulligan == false)
		{
			if (nullptr == Player->TargetTest())
			{
				int sound = rand() % 5;
				switch (sound)
				{
				case 1:
					ResMgr::Inst().PlaySound(L"board_common_dirt_poke_1.ogg");
					break;
				case 2:
					ResMgr::Inst().PlaySound(L"board_common_dirt_poke_2.ogg");
					break;
				case 3:
					ResMgr::Inst().PlaySound(L"board_common_dirt_poke_3.ogg");
					break;
				case 4:
					ResMgr::Inst().PlaySound(L"board_common_dirt_poke_4.ogg");
					break;
				case 5:
					ResMgr::Inst().PlaySound(L"board_common_dirt_poke_5.ogg");
					break;
				default:
					break;
				}
			}
		}
	}
}



void PlayState::EndStateUpdate()
{
	
	
	if (Hero::mulligan == false
		&& Hero::victory == 0
		&& INPUTINST.ButtonTestByRect(1232, 690, 1270, 715)) // 항복
	{
		surrender = !surrender;
	}
	else if (surrender == true)
	{
		RENDINST.ShowImageByScale(436, 150, L"surrender.bmp", 0.8f);

		if (INPUTINST.ButtonTestByRect(563, 262, 713, 300))
		{
			Hero::victory = -1;
			surrender = 0;
		}
		
		if (INPUTINST.FindKey("Click")->IsDown() == true
			&& INPUTINST.CursorTestByRect(489, 211, 785, 500) == false)
		{
			surrender = false;
		}
	}
	


	if (Hero::mulligan == true && Hero::IntroOn == false)
	{
		RENDINST.ShowImageByScale(575, 500, L"mul_confirm.bmp", 0.6f);
		RENDINST.ShowImageByScale(380, 50, L"mulligan_alert.bmp", 0.8f);
	}

	if (myturnstart == true
		&& Hero::victory == 0
		&& Hero::mulligan == false)
	{
		RENDINST.ShowImage(0, 0, L"myturnalert.bmp");
	}


	if (Hero::victory == 1)
	{
		float scale = 2.2f;
		RENDINST.ShowImageByScale(640 - Player->ImageX * scale, 319 - Player->ImageX * scale, Player->Herobmp, scale);
		RENDINST.ShowImage(0, 0, L"victory.bmp");
	}
	else if (Hero::victory == -1)
	{
		float scale = 1.9f;
		RENDINST.ShowImageByScale(635 - Player->ImageX * scale, 300 - Player->ImageX * scale, Player->Herobmp, scale);
		RENDINST.ShowImage(-20, 40, L"defeat.bmp");
	}
	
	
}


void PlayState::Intro()
{
	int xgap = 200;
	int ygap = 60;
	float startscale = 2;

	if (introstart == true)
	{
		introstart = false;

		Player->x = 638 - xgap;
		Player->y = 547 - ygap;
		Player->scale = startscale;
		
		Opponent->x = 639 + xgap;
		Opponent->y = 128 + ygap;
		Opponent->scale = startscale;
	}

	if (Hero::IntroOn == true)
	{
		

		anicount += DELTATIME;

		float firstvoice = 2;
		float secondvoice = firstvoice + 2;
		float thirdvoice = secondvoice + 2;
		float moving = thirdvoice + 0.5f;
		float drawing = moving + 2;

		if (anicount < thirdvoice)
		{
			RENDINST.ShowImageByScale(570, 260, L"versus.bmp");
			RENDINST.ShowImageByScale(396, 625, L"Text_Lunara.bmp");
			RENDINST.ShowImageByScale(808, 330, L"Text_Rexxar.bmp");
		}

		if (anicount < firstvoice)
		{
			if (Phase[0] == false)
			{
				Phase[0] = true;
				ResMgr::Inst().PlaySound(L"Intro_Lunara.mp3");
			}

		}
		else if (anicount < secondvoice)
		{
			if (Phase[1] == false)
			{
				Phase[1] = true;
				ResMgr::Inst().PlaySound(L"Intro_Versus.mp3");
			}

		}
		else if (anicount < thirdvoice)
		{
			if (Phase[2] == false)
			{
				Phase[2] = true;
				ResMgr::Inst().PlaySound(L"Intro_Rexxar.mp3");
		
			}
		}
		else if (anicount < moving)
		{
			
						
			float changerate = DELTATIME / (moving - thirdvoice);
			
			Player->x += changerate * xgap;
			Player->y += changerate * ygap;
			Player->scale -= changerate * (startscale - 1);

			Opponent->x -= changerate * xgap;
			Opponent->y -= changerate * ygap;
			Opponent->scale -= changerate * (startscale - 1);
		}
		else if (anicount < drawing)
		{
			if (Phase[3] == false)
			{
				Phase[3] = true;
				ResMgr::Inst().PlaySound(L"Intro_Hero_Rexxar.mp3");
				Opponent->EmoteStart = true;
				Opponent->EmoteState = EMOTE::EmoteIntro;
			}

			float changerate = DELTATIME / (drawing - moving);


			RENDINST.ShowImageByScale(960 - (200 * (anicount - moving) / 2),
				220, Player->Deck[2]->m_CardFileName, 0.6);

			RENDINST.ShowImageByScale(960 - (400 * (anicount - moving) / 2),
				220, Player->Deck[1]->m_CardFileName, 0.6);

			RENDINST.ShowImageByScale(960 - (600 * (anicount - moving) / 2),
				220, Player->Deck[0]->m_CardFileName, 0.6);


			


			Player->x = 638;
			Player->y = 547;
			Player->scale = 1;
			Opponent->x = 639;
			Opponent->y = 128;
			Opponent->scale = 1;
		}
		else
		{
			if (Phase[4] == false)
			{
				Phase[4] = true;
				ResMgr::Inst().PlaySound(L"Intro_Hero_Lunara.mp3");
				Player->EmoteStart = true;
				Player->EmoteState = EMOTE::EmoteIntro;
			}

			Hero::IntroOn = false;
			anicount = 0;

			for (size_t i = 0; i < 5; i++)
			{
				Phase[i] = false;
			}
		}
	}
}