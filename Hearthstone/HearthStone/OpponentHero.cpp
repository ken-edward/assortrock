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
	

	for (size_t i = 0; i < Deck.size(); i++) // �� ����
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
	if (heropower == true) // ����
	{
		RENDINST.ShowImage(760 - 67, 156 - 70, Heropowerbmp);
	}

	ShowMana();

	RENDINST.ShowImageByScale(x - ImageX * scale, y - ImageY * scale, Herobmp, scale);

	
	if (IntroOn == false) // ü�� ǥ��
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
		if (AIPlaying()) // 1�̸� ���� ����. 0�̸� �ƹ��͵� �� ��.
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

	int StartPointX = 640 + CardGap / 2; // �� ĭ ������ �з�

	if (HandSize <= 4) // 4�� ����
	{
		StartPointX -= CardGap * (HandSize / 2);

		if (HandSize % 2 == 1) // Ȧ���� �� ĭ �� ��
		{
			StartPointX -= CardGap / 2;
		}
	}
	else if (HandSize > 4) // 4�� �ʰ�
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

	int StartPointX = 640 + UnitGap / 2; // �� ĭ ������ �з�

	StartPointX -= UnitGap * (FieldSize / 2);

	if (FieldSize % 2 == 1) // Ȧ���� �� ĭ �� ��
	{
		StartPointX -= UnitGap / 2;
	}




	for (int i = 0; i < FieldSize; i++)
	{
		if (CardDisplayOn == true && Field[i]->CardDisplayOn == true)
		{
			RENDINST.ShowImageByScale(293, 201, Field[i]->m_CardFileName, 0.6f); // ������ ���� ī�� ���
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

	for (size_t i = 0; i < Field.size(); i++) // ���� ����
	{
		if (Field[i]->attackable == true) // �� �ϼ����� ���� ������ ��
		{
			if (Busy == false
				&& enemy->Busy == false) // ���⼭ ������ ��� ����
			{
				Field[i]->atkstart = true; // ������ ������. ����� ����.
				Busy = true;

				for (size_t j = 0; j < enemy->Field.size(); j++) // ���� �˻�
				{
					if (enemy->Field[j]->Taunt == true)
					{
						Field[i]->target = enemy->Field[j];
						// ������ ������ ���� �ϼ����� ħ
						// ȿ������ ��� X
						return 1;
					}
				}

				for (size_t j = 0; j < enemy->Field.size(); j++) // ��ȯ ȿ�� �˻�
				{
					if (enemy->Field[j]->hp <= Field[i]->atk // ���� �� �ְ�
						&& enemy->Field[j]->atk >= Field[i]->atk) // �ڱ� ���ݷ� �̻��̸� ����
					{
						Field[i]->target = enemy->Field[j];
						return 1;
					}
				}


				Field[i]->target = enemy; // ���� ������ ��븦 ħ
				return 1;
			}
		}
	}

	if (Busy == false) 
	{
		for (size_t i = 0; i < Hand.size();) // ī�� ����
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

				int StartPointX = 640 + UnitGap / 2; // �� ĭ ������ �з�

				StartPointX -= UnitGap * (FieldSize / 2);

				if (FieldSize % 2 == 1) // Ȧ���� �� ĭ �� ��
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
		&& victory == 0) // ���� ����
	{
		
		if (turncount < 3.0f)
		{
			return 0; // �� �� �ּ� 3�ʴ� ����
		}

		if (Busy == true ||
			enemy->Busy == true)
		{
			return 0; // �� �� �ٻڸ� �ƹ��͵� �� ��
		}

		for (size_t i = 0; i < Field.size(); i++)
		{
			if (Field[i]->attackable == true) // ���� ������ �� ���� ������ ���� �� ��
			{
				return 0;
			}
		}

		Hero::MyTurnFlag = true; // ���� �� ����ϸ� ����
		turncount = 0.0f; // �� �ð� ���� 0�ʷ� ����
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
		if (tempcard[i]->cost > 3) // ����� 3 ������ ��ü
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
		*DrawCard = *TempCard; // ���� ����
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

		switch (EmoteState) // �Ҹ�
		{
		case EmoteIntro: // ����
			break;
		case ThankYou:
			break;
		case WellPlayed:
			break;
		case Hello:
			break;
		case Wow:
			break;
		case Oops: // �װ� �� �Ǹ��ε�
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