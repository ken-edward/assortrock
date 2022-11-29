#include "stdafx.h"
#include "Hero.h"
#include "Card.h"

bool Hero::MyTurnFlag = true;
bool Hero::PrevFlag = false;
int Hero::victory = 0;
bool Hero::mulligan = true;
bool Hero::IntroOn = true;

Hero::Hero()
	: mana(0), maxmana(0)
	, fatiguedmg(0)
	, Deck(0)
	, heropower(true)
{
	HP(30);

	ImageX = 78;
	ImageY = 78;
	SizeX = 55;
	SizeY = 64;
	
	

	order = HeroOrder;
	attackable = true;

	cardhover = ResMgr::Inst().GetSoundPlayer();
}


Hero::~Hero()
{
	for (int i = 0; i < Deck.size(); i++)
	{
		delete Deck[i];
	}
	for (int i = 0; i < Hand.size(); i++)
	{
		delete Hand[i];
	}

	ResMgr::Inst().ReturnSoundPlayer(cardhover);
}


void Hero::Draw()
{
	if (Deck.empty() != true) // 덱이 비어 있지 않으면 드로우 시도를 함.
	{
		Card* DrawCard = m_State->CreateActor<Card>();
		Card* TempCard = *Deck.begin();
		*DrawCard = *TempCard; // 정보 복사
		delete TempCard;
		Deck.erase(Deck.begin()); // TempCard 삭제
		// 이제 카드가 핸드 장 수를 파악해서 뽑든가 태우든가 함.
		// 내 덱 좌표는 1075, 430. 상대 덱 좌표는 1075, 230.
		DrawCard->drawstart = true;
		DrawCard->visible = false;
		if (DrawCard->opp == false)
		{
			DrawCard->x = 1075;
			DrawCard->y = 430;
		}
		else
		{
			DrawCard->x = 1075;
			DrawCard->y = 230;
		}
	}
	else // 비어 있으면 탈진
	{
		fatiguestack++;
	}
}


void Hero::Attack()
{
	if (atkstart == true)
	{
		atkon = true;
		atkstart = false;
		attackable = false;
		Busy = true;
		StartX = x;
		StartY = y;
		TargetX = target->x;
		TargetY = target->y;
		order = AttackingOrder;

		if (target->hp <= atk)
		{
			target->ActualDeath = true;
		}
	}

	if (atkon == true)
	{
		atkcount += DELTATIME;

		float reaching = 0.3f;
		float standby = 0.2f;
		float ending = 0.5f;

		if (atkcount <= reaching) // 상대 자리에 가서 때림
		{
			float targetplusX = TargetX - StartX;
			float targetplusY = TargetY - StartY;
			float targetplusScale = -0.2f;
			float changerate = DELTATIME / reaching;
			x += targetplusX * changerate;
			y += targetplusY * changerate;
			scale += targetplusScale * changerate;
		}
		else if (atkcount <=  reaching + reaching) // 다시 돌아옴
		{
			if (attacked == false)
			{
				target->TakingDamage(atk);
				TakingDamage(target->atk);
				attacked = true;
				x = TargetX;
				y = TargetY;
				scale = 1.0f;
				if (atk < 6)
				{
					ResMgr::Inst().PlaySound(L"3attack.mp3");
				}
				else if (atk >= 6)
				{
					ResMgr::Inst().PlaySound(L"6attack.mp3");
				}
			}

			float targetplusX = TargetX - StartX;
			float targetplusY = TargetY - StartY;
			float targetplusScale = 0.2f;
			float changerate = DELTATIME / reaching;
			x -= targetplusX * changerate;
			y -= targetplusY * changerate;
			scale += targetplusScale * changerate;
		}
		else if (atkcount <= reaching + reaching + standby) // 잠시 대기
		{
			x = StartX;
			y = StartY;
		}
		else if (atkcount <= reaching + reaching + standby + ending) // 다시 축소
		{
			float targetplusscale = 0.2f;
			float changerate = DELTATIME / ending;
			scale -= targetplusscale * changerate;
		}
		else // 마무리
		{
			order = HeroOrder;
			atkon = false;
			atkcount = 0.0f;
			scale = 1.0f;
			attacked = false;
			Busy = false;
			target = nullptr;

			if (WieldingWeapon != nullptr)
			{
				WieldingWeapon->hp--;
				

				if (WieldingWeapon->hp == 0)
				{
					WieldingWeapon->m_bDeath = true;
					tempatk -= WieldingWeapon->atk;
					atk -= WieldingWeapon->atk;
					ResMgr::Inst().PlaySound(L"Weapon_Destruction_01.ogg");
					WieldingWeapon = nullptr;
				}
				else
				{
					ResMgr::Inst().PlaySound(L"Weapon_Durability_Hit_01.ogg");
				}
			}
		}
	}
}


void Hero::HeroUpdate()
{
	fatigue();
	Attack();

	for (size_t i = 0; i < Field.size();)
	{
		if (Field[i]->hp <= 0
			&& Field[i]->deathstart == false
			&& Field[i]->deathon == false
			&& Busy == false
			&& enemy->Busy == false)
		{
			if (Field[i]->Name != L"BoomBot")
			{
				Field[i]->m_bDeath = true;
				Field[i]->OnDeath();
				Field.erase(Field.begin() + i);
			}
			else
			{
				if (Field[i]->died == false
					&& Field[i]->deathstart == false
					&& Field[i]->deathon == false)
				{
					Field[i]->deathstart = true;
					Field[i]->OnDeath();
				}
				else
				{
					Field[i]->m_bDeath = true;
					Field.erase(Field.begin() + i);
				}
			}
		}
		else
		{
			i++;
		}
	}
}


void Hero::fatigue()
{
	float fSpeed = 20;

	if (fatiguestack > 0 && Busy == false && enemy->Busy == false)
	{
		fatigueX = 1075;

		
		if (opp == true)
		{
			fatigueY = 230;
		}
		else if (opp == false)
		{
			fatigueY = 430;
		}

		fatiguestack--;
		fatigueon = true;
		fatiguecount = 0.0f;
		fatiguespeed = fSpeed;
		Busy = true;

		ResMgr::Inst().PlaySound(L"Fatigue_card_draw.ogg");
	}

	if (fatigueon == true)
	{
		fatiguecount += DELTATIME;
		float showing = 2.0f;


		if (fatiguecount < showing)
		{
			fatiguespeed -= fSpeed / 2 * DELTATIME;

			fatigueX -= fatiguespeed;

			if (opp == true)
			{
				fatigueY += fatiguespeed * 0.2;
			}
			else if (opp == false)
			{
				fatigueY -= fatiguespeed * 0.2;
			}



			if (fatigueX < 640) // 위치 보정
			{
				fatigueX = 640;
			}
			if (opp == true)
			{
				if (fatigueY > 360)
				{
					fatigueY = 360;
				}
			}
			else
			{
				if (fatigueY < 360)
				{
					fatigueY = 360;
				}
			}
			

			



			float fScale = 0.4f;
			RENDINST.ShowImageByScale(
				fatigueX - 250 * fScale,
				fatigueY - 370 * fScale,
				L"Fatigue_Alert.bmp",
				fScale
			);
			/*
			wchar_t str[100]{};
			swprintf_s(str, L"X: %f", fatigueX);
			RENDINST.ShowText(200, 200, str);
			swprintf_s(str, L"Y: %f", fatigueY);
			RENDINST.ShowText(200, 200 + 20, str);
			swprintf_s(str, L"Speed: %f", fatiguespeed);
			RENDINST.ShowText(200, 200 + 40, str);
			*/
		}
		else
		{
			ResMgr::Inst().PlaySound(L"Fatigue_DismissPoof_01.ogg");
			ResMgr::Inst().PlaySound(L"Fatigue_blade_shing.ogg");
			fatiguedmg++;
			Busy = false;
			fatigueon = false;
			fatiguecount = 0;
			TakingDamage(fatiguedmg);
		}
	}
}


void Hero::Summon(Card* pCard, int pos)
{
	if (Field.size() < 7)
	{
		Card* newCard = m_State->CreateActor<Card>();

		*newCard = *pCard;

		newCard->onfield = true;
		newCard->order = OnField;
		newCard->opp = opp;
		newCard->pHero = this;
		newCard->scale = 1.0f;
		newCard->SizeX = 45;
		newCard->SizeY = 50;
		newCard->ImageX = 47;
		newCard->ImageY = 64;
		newCard->fieldpos = pos;
		newCard->poscertain = false;

		Field.insert(Field.begin() + pos, newCard);
	}
	delete pCard;
}