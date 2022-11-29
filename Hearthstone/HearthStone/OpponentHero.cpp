#include "stdafx.h"
#include "OpponentHero.h"
#include "Card.h"
#include "CardHeader.h"


OpponentHero::OpponentHero()
{
	x = 639;
	y = 128;

	Herobmp = L"Hero_Rexxar.bmp";
	Heropowerbmp = L"HeroPower_Hunter.bmp";

	HP(30);

	opp = true;


	for (int i = 0; i < 2; i++)
	{
		Card* newCard = new Murloc();
		newCard->pHero = this;
		newCard->opp = true;
		Deck.push_back(newCard);
	}

	
	for (int i = 0; i < 2; i++)
	{
		Card* newCard = new Raptor();
		newCard->pHero = this;
		newCard->opp = true;
		Deck.push_back(newCard);
	}
	
	for (int i = 0; i < 2; i++)
	{
		Card* newCard = new Novice();
		newCard->pHero = this;
		newCard->opp = true;
		Deck.push_back(newCard);
	}

	for (int i = 0; i < 2; i++)
	{
		Card* newCard = new Loot();
		newCard->pHero = this;
		newCard->opp = true;
		Deck.push_back(newCard);
	}

	for (int i = 0; i < 2; i++)
	{
		Card* newCard = new Rider();
		newCard->pHero = this;
		newCard->opp = true;
		Deck.push_back(newCard);
	}
	
	for (int i = 0; i < 2; i++)
	{
		Card* newCard = new Spider();
		newCard->pHero = this;
		newCard->opp = true;
		Deck.push_back(newCard);
	}


	for (int i = 0; i < 2; i++)
	{
		Card* newCard = new Gnome();
		newCard->pHero = this;
		newCard->opp = true;
		Deck.push_back(newCard);
	}

	for (int i = 0; i < 2; i++)
	{
		Card* newCard = new Fighter();
		newCard->pHero = this;
		newCard->opp = true;
		Deck.push_back(newCard);
	}

	for (int i = 0; i < 2; i++)
	{
		Card* newCard = new Ogre();
		newCard->pHero = this;
		newCard->opp = true;
		Deck.push_back(newCard);
	}

	for (int i = 0; i < 1; i++)
	{
		Card* newCard = new Rag();
		newCard->pHero = this;
		newCard->opp = true;
		Deck.push_back(newCard);
	}
	

	for (size_t i = 0; i < Deck.size(); i++) // 덱 셔플
	{
		Card* tempcard = Deck[i];
		int randindex = rand() % Deck.size();
		Deck[i] = Deck[randindex];
		Deck[randindex] = tempcard;
	}
}



OpponentHero::~OpponentHero()
{
}


void OpponentHero::Update()
{
	if (heropower == true) // 영능
	{
		RENDINST.ShowImage(760 - 67, 156 - 70, Heropowerbmp);
	}

	ShowMana();

	RENDINST.ShowImageByScale(x - ImageX * scale, y - ImageY * scale, Herobmp, scale);

	
	if (IntroOn == false) // 체력 표시
	{
		float hpscale = 0.6f;
		RENDINST.ShowImageByScale(
			(x + 49 * scale) - 35 * hpscale,
			(y + 43 * scale) - 45 * hpscale,
			L"icon_hp.bmp",
			hpscale
		);

		if (hp == Maxhp) 
		{
			RENDINST.ShowNumber(x + 48 * scale, y + 49 * scale, hp);
		}
		else if (hp < Maxhp)
		{
			RENDINST.ShowNumber(x + 48 * scale, y + 49 * scale, hp, 1);
		}
	}

	HeroUpdate();
	usingheropower();
	ShowEmote();

	if (victory == 0
		&& Hero::PrevFlag == false
		&& Hero::MyTurnFlag == false)
	{
		if (AIPlaying()) // 1이면 무언가 했음. 0이면 아무것도 안 함.
		{
			playterm = 0.0f;
		}
	}

	OppCardDisplay();
	OppFieldDisplay();


	showdmg();
	if (hp < prevhp)
	{
		dmg = prevhp - hp;
		damaged = true;
		dmgcount = 0.0f;
	}
	prevhp = hp;
}


void OpponentHero::ShowMana()
{
	int x = 800;
	int y = 45;
	RENDINST.ShowNumber(x, y, mana);
	RENDINST.ShowNumber(x + 30, y, maxmana);
}


void OpponentHero::OppCardDisplay()
{
	int HandSize = Hand.size();
	
	int CardGap = 88;

	int StartPointX = 640 + CardGap / 2; // 반 칸 더해진 분량

	if (HandSize <= 4) // 4장 이하
	{
		StartPointX -= CardGap * (HandSize / 2);

		if (HandSize % 2 == 1) // 홀수면 반 칸 더 뺌
		{
			StartPointX -= CardGap / 2;
		}
	}
	else if (HandSize > 4) // 4장 초과
	{
		StartPointX = 640 - CardGap * 1.5;
		CardGap = CardGap * 3 / (HandSize - 1);
	}


	for (int i = 0; i < HandSize; i++)
	{
		if (Hand[i]->playon == false
			&& Hand[i]->drawon == false
			&& Hand[i]->drawstart == false)
		{
			Hand[i]->x = StartPointX + CardGap * i;
			Hand[i]->y = -30;
		}
	}
}


void OpponentHero::OppFieldDisplay()
{
	int FieldSize = Field.size();

	int UnitGap = 88;

	int StartPointX = 640 + UnitGap / 2; // 반 칸 더해진 분량

	StartPointX -= UnitGap * (FieldSize / 2);

	if (FieldSize % 2 == 1) // 홀수면 반 칸 더 뺌
	{
		StartPointX -= UnitGap / 2;
	}




	for (int i = 0; i < FieldSize; i++)
	{
		if (CardDisplayOn == true && Field[i]->CardDisplayOn == true)
		{
			RENDINST.ShowImageByScale(293, 201, Field[i]->m_CardFileName, 0.6f); // 좌측에 내는 카드 띄움
		}

		if (INPUTINST.ButtonTestByRect(293, 201, 450, 432))
		{
			CardDisplayOn = false;
			Field[i]->CardDisplayOn = false;
		}

		if (Field[i]->atkon == false
			&& Field[i]->playon == false)
		{
			Field[i]->x = StartPointX + UnitGap * i;
			Field[i]->y = 270;
			continue;
		}
	}

	
}


bool OpponentHero::AIPlaying()
{
	if (Busy == false && enemy->Busy == false)
	{
		turncount += DELTATIME;
		playterm += DELTATIME;
	}

	if (playterm < 0.4 || turncount < 3)
	{
		return 0;
	}

	for (size_t i = 0; i < Field.size(); i++) // 공격 지시
	{
		if (Field[i]->attackable == true) // 한 하수인이 공격 가능할 때
		{
			if (Busy == false
				&& enemy->Busy == false) // 여기서 공격할 사람 선택
			{
				Field[i]->atkstart = true; // 공격을 시작함. 대상은 미정.
				Busy = true;

				for (size_t j = 0; j < enemy->Field.size(); j++) // 도발 검사
				{
					if (enemy->Field[j]->Taunt == true)
					{
						Field[i]->target = enemy->Field[j];
						// 도발이 있으면 도발 하수인을 침
						// 효율성은 고려 X
						return 1;
					}
				}

				for (size_t j = 0; j < enemy->Field.size(); j++) // 교환 효율 검사
				{
					if (enemy->Field[j]->hp <= Field[i]->atk // 죽일 수 있고
						&& enemy->Field[j]->atk >= Field[i]->atk) // 자기 공격력 이상이면 죽임
					{
						Field[i]->target = enemy->Field[j];
						return 1;
					}
				}


				Field[i]->target = enemy; // 별일 없으면 상대를 침
				return 1;
			}
		}
	}

	if (Busy == false) 
	{
		for (size_t i = 0; i < Hand.size();) // 카드 내기
		{
			if (Hand[i]->cost <= mana
				&& Field.size() < 7
				&& turncount > 2.0f)
			{
				Card* tempcard = Hand[i];
				Field.push_back(tempcard);
				mana -= tempcard->cost;
				Hand.erase(Hand.begin() + i);
				tempcard->playstartX = tempcard->x;
				tempcard->playstartY = tempcard->y;
				tempcard->playstart = true;
				tempcard->playon = true;
				tempcard->OnPlay();
				tempcard->order = PlayingOrder;
				CardDisplayOn = true;
				Busy = true;
				

				int FieldSize = Field.size();

				int UnitGap = 88;

				int StartPointX = 640 + UnitGap / 2; // 반 칸 더해진 분량

				StartPointX -= UnitGap * (FieldSize / 2);

				if (FieldSize % 2 == 1) // 홀수면 반 칸 더 뺌
				{
					StartPointX -= UnitGap / 2;
				}

				for (int i = 0; i < FieldSize; i++)
				{
					if (Field[i]->playstart == true)
					{
						Field[i]->playtargetX = StartPointX + UnitGap * i;
						Field[i]->playtargetY = 270;
						return 1;
					}
				}

				break;
			}
			i++;
		}
	}
	

	if (mana >= 2
		&& heropower == true)
	{
		heropower = false;
		mana -= 2;
		heropowerstart = true;
		return 1;
	}
	

	

	if (Hero::PrevFlag == false
		&& Hero::MyTurnFlag == false
		&& victory == 0) // 턴종 조건
	{
		
		if (turncount < 3.0f)
		{
			return 0; // 매 턴 최소 3초는 가짐
		}

		if (Busy == true ||
			enemy->Busy == true)
		{
			return 0; // 둘 다 바쁘면 아무것도 안 함
		}

		for (size_t i = 0; i < Field.size(); i++)
		{
			if (Field[i]->attackable == true) // 공격 가능한 게 남아 있으면 턴종 안 함
			{
				return 0;
			}
		}

		Hero::MyTurnFlag = true; // 위를 다 통과하면 턴종
		turncount = 0.0f; // 턴 시간 재기는 0초로 세팅
	}

	return 0;
}


void OpponentHero::changecard()
{
	Card* tempcard[4] = { Deck[0], Deck[1], Deck[2], Deck[3] };
	Deck.erase(Deck.begin());
	Deck.erase(Deck.begin());
	Deck.erase(Deck.begin());
	Deck.erase(Deck.begin());

	for (size_t i = 0; i < 4;)
	{
		if (tempcard[i]->cost > 3) // 비용이 3 넘으면 교체
		{
			int randindex = rand() % Deck.size();
			if (Deck[randindex]->WasOnMulligan == true)
			{
				continue;
			}
			Card* temp = tempcard[i];
			tempcard[i] = Deck[randindex];
			Deck[randindex] = temp;
			Deck[randindex]->WasOnMulligan = true;
		}
		i++;
	}

	for (size_t i = 0; i < 4; i++)
	{
		Deck.insert(Deck.begin() + i, tempcard[i]);
	}

	for (size_t i = 0; i < 4; i++)
	{
		Card* DrawCard = m_State->CreateActor<Card>();
		Card* TempCard = Deck[0];
		*DrawCard = *TempCard; // 정보 복사
		Hand.push_back(DrawCard);
		delete TempCard;
		Deck.erase(Deck.begin());
		DrawCard->visible = true;
	}
}


void OpponentHero::usingheropower()
{
	if (heropowerstart == true)
	{
		heropowerstart = false;
		heropoweron = true;
		Busy = true;
		anicount = 0;
	}
	
	if (heropoweron == true)
	{
		anicount += DELTATIME;

		float preshowing = 0.8f;
		float activating = preshowing + 0.6f;

		if (anicount < preshowing)
		{
			RENDINST.ShowImageByScale(320, 200, L"heroalert_hunter.bmp", 0.8);
		}
		else if (anicount < activating)
		{
			if (phase[0] == false)
			{
				heropower = false;
				phase[0] = true;
				ResMgr::Inst().PlaySound(L"Arrow_Impact.ogg");
				ResMgr::Inst().PlaySound(L"Arrow_Fired.ogg");
			}
		}
		else
		{
			ResMgr::Inst().PlaySound(L"Arrow_Explode.ogg");

			for (size_t i = 0; i < 5; i++)
			{
				phase[i] = false;
			}
			enemy->TakingDamage(2);
			Busy = false;
			heropoweron = false;
			anicount = 0;
		}
	}
}



void OpponentHero::ShowEmote()
{
	if (EmoteStart == true)
	{
		EmoteStart = false;
		EmoteOn = true;
		EmoteCount = 0.0f;

		switch (EmoteState) // 소리
		{
		case EmoteIntro: // ㄴㄴ
			break;
		case ThankYou:
			break;
		case WellPlayed:
			break;
		case Hello:
			break;
		case Wow:
			break;
		case Oops: // 그건 좀 실망인데
			ResMgr::Inst().PlaySound(L"Rexxar_Oops.mp3");
			break;
		case Threaten:
			break;
		case LackofMana:
			break;
		default:
			break;
		}
	}

	if (EmoteOn)
	{
		EmoteCount += DELTATIME;

		if (EmoteCount >= 3.0f)
		{
			EmoteOn = false;
			EmoteCount = 0.0f;
		}

		switch (EmoteState)
		{
		case EMOTE::EmoteIntro:
			RENDINST.ShowImage(x - 250, y + 50, L"Rexxar_Intro.bmp");
			break;
		case EMOTE::Oops:
			RENDINST.ShowImageByScale(x - 250, y + 50, L"Rexxar_Oops.bmp", 0.8);
			break;
		default:
			break;
		}
	}
}