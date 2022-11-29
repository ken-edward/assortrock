#include "stdafx.h"
#include "MyHero.h"
#include "RenderMgr.h"
#include "Card.h"
#include "PlayState.h"
#include <stdlib.h>
#include "CardHeader.h"

/* Memo

	계획 중인 것들

	인트로 추가
	항복 기능: 옵션 - 항복
	승패 애니메이션 추가
	광역기 주문: 휘둘러치기
	카드깡 등급 소리 (희귀 카드)

*/

MyHero::MyHero()
{
	x = 638;
	y = 547;
	HP(30);
	atk = 0;

	Herobmp = L"Hero_Lunara.bmp";
	Heropowerbmp = L"HeroPower_Druid.bmp";

	std::ifstream inFile("Text_MyDeck.txt");
	char chr[2] = {};

	while (!inFile.eof() && CardCount < CARDS::LAST)
	{
		inFile.getline(chr, 2);

		if (chr[0] == 0)
		{
			chr[0] = 0;
		}
		else if (chr[0] >= '0' && chr[0] <= '9')
		{
			chr[0] -= '0';
		}

		for (size_t i = 0; i < chr[0]; i++)
		{
			Card* newCard = nullptr;

			switch (CardCount)
			{
				CARDCASES
			}

			newCard->pHero = this;
			Deck.push_back(newCard);
		}

		CardCount += 1;
	}

	inFile.close();



	for (size_t i = 0; i < Deck.size(); i++) // 덱 셔플
	{
		Card* tempcard = Deck[i];
		int randindex = rand() % Deck.size();
		Deck[i] = Deck[randindex];
		Deck[randindex] = tempcard;
	}
}


MyHero::~MyHero()
{
}


void MyHero::Update()
{
	if (INPUTINST.FindKey("T")->IsDown())
	{
		hp = 30;
		enemy->hp = 30;
	}

	if (heropower == true) // 영능
	{
		RENDINST.ShowImage(760 - 67, 548 - 70, Heropowerbmp);

		if (PrevFlag == true)
		{
			if (INPUTINST.ButtonTestByRect(710, 500, 800, 600) && SelectingEmote == false)
			{
				if (mana < 2)
				{
					ResMgr::Inst().PlaySound(L"Lunara_LackofMana.mp3");
					EmoteStart = true;
					EmoteState = LackofMana;
				}
				else
				{
					mana -= 2;
					atk += 1;
					tempatk += 1;
					armor += 1;
					heropower = false;
				}
			}
		}
	}


	RENDINST.ShowImageByScale(x - ImageX * scale, y - ImageY * scale, Herobmp, scale); // 영웅 초상화
	

	if (armor > 0) // 방어도 표시
	{
		float pivotX = 50;
		float pivotY = 0;
		float armorScale = 0.7;
		float armorImageX = 50 * armorScale;
		float armorImageY = 50 * armorScale;

		RENDINST.ShowImageByScale(
			(x + pivotX * scale) - armorImageX,
			(y + pivotY * scale) - armorImageY,
			L"icon_armor.bmp",
			armorScale
		);

		RENDINST.ShowNumber(
			(x + pivotX * scale) - 1,
			(y + pivotY * scale) + 0,
			armor
		);
	}

	if (atk > 0) // 공격력 표시
	{
		float pivotX = -50;
		float pivotY = 45;
		float atkScale = 0.7;
		float atkImageX = 50 * atkScale;
		float atkImageY = 50 * atkScale;

		RENDINST.ShowImageByScale(
			(x + pivotX * scale) - atkImageX,
			(y + pivotY * scale) - atkImageY,
			L"icon_atk.bmp",
			atkScale
		);

		RENDINST.ShowNumber(
			(x + pivotX * scale) + 1,
			(y + pivotY * scale) + 3,
			atk
		);
	}

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

	


	ShowMana(); // 마나 표시
	HeroUpdate();
	


	if (mulligan == true && IntroOn == false)
	{
		domulligan();
	}

	ShowEmote();

	
	
	if (mulligan == false)
	{
		MyCardDisplay();
		MyFieldDisplay();
		SelectEmote();

		if (victory == 0
			&& Hero::PrevFlag == true)
		{
			SelectCard();
			SelectField();

			SelectHero();
		}
	}
	

	showdmg(); // 대미지 표시
	if (hp < prevhp) // 피 차이 나면 대미지 토글
	{
		dmg = prevhp - hp;
		damaged = true;
		dmgcount = 0.0f;
	}
	prevhp = hp;
}


void MyHero::ShowMana()
{
	int x = 824;
	int y = 666;
	RENDINST.ShowNumber(x, y, mana);
	RENDINST.ShowNumber(x + 30, y, maxmana);
	for (int i = 0; i < mana; i++)
	{
		RENDINST.ShowImage(868 + 22 * i, 653, L"smallmana.bmp");
	}
	for (int i = mana; i < maxmana; i++)
	{
		RENDINST.ShowImage(874 + 22 * i, 656, L"usedmana.bmp");
	}
}


void MyHero::MyCardDisplay()
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
			Hand[i]->y = 690;
		}
	}

	bool PlayTest = false;
	for (int i = Hand.size() - 1; i >= 0; i--)
	{
		if (Hand[i]->drawon == false
			&& Hand[i]->drawstart == false
			&& Hand[i]->playmode == false)
		{
			Hand[i]->scale = 0.36f;
		}
		if (Hand[i]->playmode == true)
		{
			PlayTest = true;
		}
	}

	for (int i = Hand.size() - 1; i >= 0; i--)
	{
		if (Hand[i]->drawon == false
			&& Hand[i]->drawstart == false
			&& Hand[i]->playmode == false
			&& PlayTest == false)
		{
			if (INPUTINST.CursorTest(Hand[i]))
			{
				Hand[i]->y -= 60;
				Hand[i]->scale = 0.5f;
				break;
			}
		}
	}
}


void MyHero::SelectCard()
{
	bool Playing = false;
	int PlayingIndex = -1;

	for (int i = 0; i < Hand.size(); i++)
	{
		if (Hand[i]->playmode == true)
		{
			Playing = true;
			PlayingIndex = i;
		}
	}

	if (Playing == false)
	{
		for (int i = Hand.size() - 1; i >= 0; i--)
		{
			if (INPUTINST.ButtonTest(Hand[i])
				&& Busy == false)
			{
				if (MyTurnFlag == true)
				{
					if (Hand[i]->cost <= mana)
					{
						Hand[i]->playmode = true;
						Hand[i]->order = PlayingOrder;
						Hand[i]->scale = 0.36f;
						break;
					}
					else
					{
						
						EmoteStart = true;
						EmoteState = LackofMana;
					}

				}
			}
		}
	}
	else if (Playing == true)
	{
		if (INPUTINST.FindKey("Click")->IsUp()) // 카드를 잡다가 뗐음
		{
			Hand[PlayingIndex]->playmode = false;
			Hand[PlayingIndex]->order = OnHand;
			cardhover->Stop();

			if (Hand[PlayingIndex]->m_Type == Minion) // 하수인일 때의 처리
			{
				if (INPUTINST.Cursor().y < 480
					&& Field.size() < 7
					&& mana >= Hand[PlayingIndex]->cost
					&& Busy == false)
				{
					Card* tempcard = Hand[PlayingIndex];
					int fieldpos = 0;

					if (Field.empty() == true) // 필드에 아무것도 없을 때
					{
						Field.push_back(tempcard);
					}
					else
					{
						for (size_t i = 0; i < Field.size(); i++)
						{
							if (INPUTINST.Cursor().x < Field[i]->x)
							{
								Field.insert(Field.begin() + i, tempcard);
								fieldpos = i;
								break;
							}
							if (i == Field.size() - 1) // 마지막
							{
								Field.push_back(tempcard);
								fieldpos = Field.size() - 1;
								break;
							}
						}
					}
					mana -= tempcard->cost;
					tempcard->OnPlay();
					Hand.erase(Hand.begin() + PlayingIndex);
					tempcard->x = INPUTINST.Cursor().x;
					tempcard->y = INPUTINST.Cursor().y;
					tempcard->playstartX = INPUTINST.Cursor().x;
					tempcard->playstartY = INPUTINST.Cursor().y;
					tempcard->playstart = true;
					tempcard->playon = true;
					tempcard->prevHandIndex = PlayingIndex;
					tempcard->fieldpos = fieldpos;
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
							Field[i]->playtargetY = 400;
							continue;
						}
					}
				}
			}
			else if (Hand[PlayingIndex]->m_Type == Spell) // 주문일 때의 처리
			{
				if (Hand[PlayingIndex]->m_SpellType == DrawCard)
				{
					if (INPUTINST.Cursor().y < 480
						&& mana >= Hand[PlayingIndex]->cost)
					{
						mana -= Hand[PlayingIndex]->cost;
						Hand[PlayingIndex]->caststart = true;
						Busy = true;
						Card* tempcard = Hand[PlayingIndex];
						Hand.erase(Hand.begin() + PlayingIndex);
						for (size_t i = 0; i < tempcard->draw; i++)
						{
							Draw();
						}
						tempcard->OnPlay();
					}
				}
				else if (Hand[PlayingIndex]->m_SpellType == DealCard)
				{
					Character* pTarget = TargetTest();

					if (pTarget != nullptr
						&& pTarget->ActualDeath == false
						&& Hand[PlayingIndex]->cost <= mana)
					{
						mana -= Hand[PlayingIndex]->cost;

						int realdeal = Hand[PlayingIndex]->deal;

						for (size_t i = 0; i < Field.size(); i++)
						{
							realdeal += Field[i]->spelldamage;
						}
						pTarget->TakingDamage(realdeal);
						Hand[PlayingIndex]->caststart = true;
						Busy = true;
						Hand[PlayingIndex]->OnPlay();
						Hand.erase(Hand.begin() + PlayingIndex);
					}
				}
				else if (Hand[PlayingIndex]->m_SpellType == AreaCard) // 광역기
				{
					if (Hand[PlayingIndex]->cost <= mana)
					{
						mana -= Hand[PlayingIndex]->cost;
						Hand[PlayingIndex]->caststart = true;
						Busy = true;
						Hand[PlayingIndex]->OnPlay();

						int realdeal = Hand[PlayingIndex]->deal;

						for (size_t i = 0; i < Field.size(); i++)
						{
							realdeal += Field[i]->spelldamage;
						}

						for (size_t i = 0; i < enemy->Field.size(); i++)
						{
							enemy->Field[i]->TakingDamage(realdeal);
						}

						Hand.erase(Hand.begin() + PlayingIndex);
					}
				}
			}
			else if (Hand[PlayingIndex]->m_Type == Weapon) // 무기일 때의 처리
			{
				Card* tempcard = Hand[PlayingIndex];

				mana -= tempcard->cost;
				Hand.erase(Hand.begin() + PlayingIndex);

				tempcard->x = INPUTINST.Cursor().x;
				tempcard->y = INPUTINST.Cursor().y;

				tempcard->WeaponPlayStart = true;


			}
		}
		else if (INPUTINST.FindKey("RClick")->IsDown())
		{
			Hand[PlayingIndex]->playmode = false;
			Hand[PlayingIndex]->order = OnHand;
			cardhover->Stop();
		}
		else
		{
			if (cardhover->IsPlay() == false
				&& MyTurnFlag == true)
			{
				if (Hand[PlayingIndex]->IdleSound != nullptr)
				{
					cardhover->NewSoundPlay(Hand[PlayingIndex]->IdleSound);
				}
				else
				{
					cardhover->NewSoundPlay(L"card_hover.mp3");
				}
			}
			Hand[PlayingIndex]->x = INPUTINST.Cursor().x;
			Hand[PlayingIndex]->y = INPUTINST.Cursor().y;
		}
	}
}


void MyHero::MyFieldDisplay()
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
		if (Field[i]->atkon == false
			&& Field[i]->playon == false)
		{
			Field[i]->x = StartPointX + UnitGap * i;
			Field[i]->y = 400;
			Field[i]->poscertain = true;
			continue;
		}
	}
}


void MyHero::SelectField()
{
	bool AttackSelecting = false;
	int AttackingIndex = -1;

	for (int i = 0; i < Field.size(); i++)
	{
		if (Field[i]->attackmode == true) // 이미 무언가 공격 중이면
		{
			AttackSelecting = true; // 공격 중
			AttackingIndex = i;
		}
	}

	if (AttackSelecting == false) // 공격 지시 중이 아니면
	{
		for (int i = Field.size() - 1; i >= 0; i--)
		{
			if (INPUTINST.ButtonTest(Field[i]) // 버튼 테스트
				&& Field[i]->attackable == true // 공격 가능하고
				&& Busy == false) // 공격 중인 게 없음
			{
				Field[i]->attackmode = true;
				Field[i]->AttackSound();
				Field[i]->scale = 1.1f;
				break;
			}
		}
	}
	else if (AttackSelecting == true) // 공격 지시 중이면
	{
		if (INPUTINST.FindKey("Click")->IsUp()) // 놓은 거면
		{
			Field[AttackingIndex]->attackmode = false; // 공격모드는 끄고
			
			Character* target = EnemyTargetTest();

			
			if (target != nullptr &&
				target->ActualDeath == false &&
				Busy == false)
			{
				Field[AttackingIndex]->atkstart = true;
				Field[AttackingIndex]->target = target;
				Field[AttackingIndex]->attackable = false;
			}
			else // 안 때렸을 때의 처리
			{
				Field[AttackingIndex]->scale = 1.0f;
				ResMgr::Inst().PlaySound(L"cancel_attack.ogg");
			}
		}
		else if (INPUTINST.FindKey("RClick")->IsDown()) // 우클릭 공격 취소
		{
			Field[AttackingIndex]->scale = 1.0f; // 스케일 돌려 놓고
			Field[AttackingIndex]->attackmode = false; // 공격모드 끄고
			ResMgr::Inst().PlaySound(L"cancel_attack.ogg");
		}
	}
}


void MyHero::SelectHero()
{
	if (HeroAttackSelecting == false)
	{
		if (INPUTINST.ButtonTest(this)
			&& Busy == false
			&& attackable == true
			&& atk != 0)
		{
			HeroAttackSelecting = true;
			scale = 1.2f;
			ResMgr::Inst().PlaySound(L"Lunara_Attack.mp3");
		}
	}
	else
	{
		bool SelectCancel = false;
		scale = 1.2f;

		if (INPUTINST.FindKey("Click")->IsUp())
		{
			Character* pTarget = TargetTest();
			HeroAttackSelecting = false;
			
			if (pTarget != nullptr && pTarget != this)
			{
				if (pTarget->ActualDeath == false &&
					Busy == false)
				{
					atkstart = true;
					target = pTarget;
					attackable = false;
				}
			}
			else
			{
				HeroAttackSelecting = false;
				SelectCancel = true;
			}
		}

		if (INPUTINST.FindKey("RClick")->IsDown())
		{
			HeroAttackSelecting = false;
			SelectCancel = true;
		}

		if (SelectCancel == true)
		{
			scale = 1.0f;
		}
	}
}


void MyHero::domulligan()
{
	for (size_t i = 0; i < 3; i++)
	{
		RENDINST.ShowImageByScale(360 + 200 * i, 220, Deck[i]->m_CardFileName, 0.6);
	}
	for (size_t i = 0; i < 3; i++)
	{
		if (INPUTINST.ButtonTestByRect(
			360 + 200 * i, 220,
			360 + 200 * i + 160, 220 + 230))
		{
			MulCard[i] = !MulCard[i];
		}
	}
	for (size_t i = 0; i < 3; i++)
	{
		if (MulCard[i] == true)
		{
			RENDINST.ShowImageByScale(350 + 200 * i, 190, L"xmark.bmp", 1.0f);
		}
	}
}


void MyHero::changecard()
{
	Card* tempcard[3] = { Deck[0], Deck[1], Deck[2] };
	Deck.erase(Deck.begin());
	Deck.erase(Deck.begin());
	Deck.erase(Deck.begin()); // 27장 남음

	for (size_t i = 0; i < 3;)
	{
		if (MulCard[i] == true) // 바꾸기로 했으면 교체
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

	for (size_t i = 0; i < 3; i++)
	{
		Deck.insert(Deck.begin() + i, tempcard[i]);
	}


	for (size_t i = 0; i < 3; i++)
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


Character* MyHero::TargetTest()
{
	if (INPUTINST.CursorTestByRect(585, 485, 700, 600))
	{
		return this;
	}

	if (INPUTINST.CursorTestByRect(585, 70, 700, 200))
	{
		return enemy;
	}

	for (size_t i = 0; i < this->Field.size(); i++)
	{
		if (INPUTINST.CursorTest(this->Field[i]))
		{
			return this->Field[i];
		}
	}

	for (size_t i = 0; i < enemy->Field.size(); i++)
	{
		if (INPUTINST.CursorTest(enemy->Field[i]))
		{
			return enemy->Field[i];
		}
	}

	return nullptr;
}


Character* MyHero::EnemyTargetTest()
{
	if (INPUTINST.CursorTestByRect(585, 70, 700, 200))
	{
		return enemy;
	}

	for (size_t i = 0; i < enemy->Field.size(); i++)
	{
		if (INPUTINST.CursorTest(enemy->Field[i]))
		{
			return enemy->Field[i];
		}
	}

	return nullptr;
}

Character* MyHero::FriendlyTargetTest()
{
	if (INPUTINST.CursorTestByRect(585, 485, 700, 600))
	{
		return this;
	}

	for (size_t i = 0; i < this->Field.size(); i++)
	{
		if (INPUTINST.CursorTest(this->Field[i]))
		{
			return this->Field[i];
		}
	}

	return nullptr;
}


void MyHero::SelectEmote()
{
	if (INPUTINST.CursorTest(this)
		&& INPUTINST.FindKey("RClick")->IsDown())
	{
		SelectingEmote = !SelectingEmote;
	}

	if (SelectingEmote == true)
	{
		RENDINST.ShowImageByScale(405, 380, L"Emote_List.bmp", 0.8);

		if (INPUTINST.CursorTestByRect(420,380,850,600) == false
			|| INPUTINST.FindKey("Click")->IsDown() == true)
		{
			SelectingEmote = false;
		}
		
		if (INPUTINST.ButtonTestByRect(450, 400, 600, 450) == true) // 감사
		{
			EmoteStart = true;
			EmoteState = ThankYou;
		}

		if (INPUTINST.ButtonTestByRect(424, 464, 574, 514) == true) // 칭찬
		{
			EmoteStart = true;
			EmoteState = WellPlayed;
		}

		if (INPUTINST.ButtonTestByRect(428, 533, 578, 583) == true) // 감사
		{
			EmoteStart = true;
			EmoteState = Hello;
		}

		if (INPUTINST.ButtonTestByRect(680, 400, 820, 439) == true) // 감탄
		{
			EmoteStart = true;
			EmoteState = Wow;
		}

		if (INPUTINST.ButtonTestByRect(717, 467, 850, 500) == true) // 이런
		{
			EmoteStart = true;
			EmoteState = Oops;
		}

		if (INPUTINST.ButtonTestByRect(714, 530, 851, 575) == true) // 위협
		{
			EmoteStart = true;
			EmoteState = Threaten;
		}
	}
}


void MyHero::ShowEmote()
{
	if (EmoteStart == true)
	{
		EmoteStart = false;
		EmoteOn = true;
		EmoteCount = 0.0f;

		
		switch (EmoteState) // 소리
		{
		case EmoteIntro:
			break;
		case ThankYou:
			ResMgr::Inst().PlaySound(L"Lunara_ThankYou.mp3");
			break;
		case WellPlayed:
			ResMgr::Inst().PlaySound(L"Lunara_WellPlayed.mp3");
			break;
		case Hello:
			ResMgr::Inst().PlaySound(L"Lunara_Hello.mp3");
			break;
		case Wow:
			ResMgr::Inst().PlaySound(L"Lunara_Wow.mp3");
			break;
		case Oops:
			ResMgr::Inst().PlaySound(L"Lunara_Oops.mp3");
			break;
		case Threaten:
			ResMgr::Inst().PlaySound(L"Lunara_Threaten.mp3");
			break;
		case LackofMana:
			ResMgr::Inst().PlaySound(L"Lunara_LackofMana.mp3");
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

			if (EmoteState != EmoteIntro)
			{
				enemy->EmoteStart = true;
				enemy->EmoteState = Oops;
			}
		}

		switch (EmoteState)
		{
		case EmoteIntro: 
			RENDINST.ShowImage(x - 230, y - 150, L"Lunara_Intro.bmp");
			break;
		case ThankYou:
			RENDINST.ShowImage(x + 50, y - 200, L"Lunara_ThankYou.bmp");
			break;
		case WellPlayed:
			RENDINST.ShowImage(x + 50, y - 200, L"Lunara_WellPlayed.bmp");
			break;
		case Hello:
			RENDINST.ShowImage(x + 50, y - 200, L"Lunara_Hello.bmp");
			break;
		case Wow:
			RENDINST.ShowImage(x + 50, y - 200, L"Lunara_Wow.bmp");
			break;
		case Oops:
			RENDINST.ShowImage(x + 50, y - 200, L"Lunara_Oops.bmp");
			break;
		case Threaten:
			RENDINST.ShowImage(x + 50, y - 200, L"Lunara_Threaten.bmp");
			break;
		case LackofMana:
			RENDINST.ShowImage(x + 50, y - 200, L"Lunara_LackofMana.bmp");
			break;
		default:
			break;
		}
	}
}