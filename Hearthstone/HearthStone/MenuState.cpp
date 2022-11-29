#include "stdafx.h"
#include "MenuState.h"
#include "RenderMgr.h"
#include "InputMgr.h"
#include "APICORE.h"
#include "ResMgr.h"
#include "CardHeader.h"



MenuState::MenuState()
{
	
	std::ifstream inFile("Text_AllCard.txt");
	char chr[2] = {};

	while (!inFile.eof() && CardCount < CARDS::LAST)
	{
		inFile.getline(chr, 2);

		if (chr[0] == 0)
		{
			chr[0] = 2;
		}
		else if (chr[0] >= '0' && chr[0] <= '9')
		{
			chr[0] -= '0';
		}


		Card* newCard = nullptr;

		switch (CardCount)
		{
			CARDCASES
		}

		newCard->Collected = chr[0];
		if (newCard->legendary == true && chr[0] == 2)
		{
			newCard->Collected = 1;
		}
		CardVector.push_back(newCard);
		CardCount += 1; // 카드 개수
	}

	inFile.close();
}


MenuState::~MenuState()
{
}


void MenuState::Start()
{
	bgm->NewSoundPlay(L"Main_Theme.mp3");


	
}


void MenuState::StateUpdate()
{	
	if (bgm->IsPlay() == false)
	{
		bgm->NewSoundPlay(L"Main_Theme.mp3");
	}

	if (INPUTINST.FindKey("T")->IsDown())
	{
		pack += 1;
	}

	switch (mState)
	{
	case Menu:
	{
		RENDINST.ShowImage(0, 0, L"Menu.bmp"); // 메뉴 이미지

		if (INPUTINST.CursorTestByRect(180, 580, 300, 670)) // 상점 버튼 커서
		{
			RENDINST.ShowImage(0, 0, L"Menu_Shop_Selected.bmp");
		}

		if (INPUTINST.CursorTestByRect(300, 580, 400, 680)) // 상점 버튼 커서
		{
			RENDINST.ShowImage(0, 0, L"Menu_Quest_Selected.bmp");
		}

		if (INPUTINST.CursorTestByRect(580, 570, 820, 660)) // 내 카드 커서
		{
			RENDINST.ShowImage(0, 0, L"Menu_Deck_Selected.bmp");
		}

		if (INPUTINST.CursorTestByRect(455, 570, 580, 660)) // 팩 개봉 커서
		{
			RENDINST.ShowImage(0, 0, L"Menu_Open_Selected.bmp");
		}

		RENDINST.ShowNumber(514, 647, pack); // 팩 수

		if (INPUTINST.ButtonTestByRect(540, 200, 740, 240)) // 대전 버튼
		{
			SetNextState(STATE::SELECT);
		}

		if (INPUTINST.ButtonTestByRect(180, 580, 300, 670)) // 상점 버튼
		{
			mState = Shop;
		}

		if(INPUTINST.ButtonTestByRect(452, 567, 580, 660)) // 팩 개봉 버튼
		{
			mState = Open;

			CardCount = 0;

			std::ifstream inFile("Text_AllCard.txt");
			char chr[2] = {};

			while (!inFile.eof() && CardCount < CARDS::LAST)
			{
				inFile.getline(chr, 2);

				if (chr[0] == 0)
				{
					chr[0] = 2;
				}
				else if (chr[0] >= '0' && chr[0] <= '9')
				{
					chr[0] -= '0';
				}


				CardVector[CardCount]->Collected = chr[0];
				if (CardVector[CardCount]->legendary == true && chr[0] == 2)
				{
					CardVector[CardCount]->Collected = 1;
				}
				CardCount += 1; // 카드 개수
			}

			inFile.close();
			
		}

		if (INPUTINST.ButtonTestByRect(580, 570, 820, 660)) // 덱 버튼
		{
			SetNextState(STATE::DECK);
		}
	}
		break;
	case Shop:
	{
		RENDINST.ShowImage(0, 0, L"Shop.bmp"); // 상점 이미지
		
		if (gold >= 100)
		{
			RENDINST.ShowImage(849, 490, L"Shop_Buy.bmp"); // 구매 버튼
		}

		if (INPUTINST.FindKey("Click")->IsDown()
			&& alert == false
			&& INPUTINST.CursorTestByRect(220, 40, 1060, 640) == false) // 상점 나가기
		{
			mState = Menu;
		}

		if (INPUTINST.ButtonTestByRect(860, 500, 980, 540)
			&& alert == false) // 구매 버튼
		{
			if (gold >= 100)
			{
				gold -= 100;
				alert = true;
				pack++;
			}
		}

		if (alert == true)
		{
			RENDINST.ShowImage(400, 180, L"Shop_Confirm.bmp");

			if (INPUTINST.ButtonTestByRect(600, 500, 710, 540)) // 확인 버튼
			{
				alert = false;
			}
		}
	}
		break;
	case Open:
	{
		RENDINST.ShowImage(0, 0, L"packopen.bmp"); // 팩 개봉 이미지
		RENDINST.ShowNumber(287, 247, pack);

		if (RevealMode == false)
		{
			if (INPUTINST.ButtonTestByRect(1030, 645, 1090, 680)) // 나가기
			{
				mState = Menu;


				std::ofstream outFile("Text_AllCard.txt");

				for (int i = 0; i < CARDS::LAST; i++)
				{
					outFile << CardVector[i]->Collected << std::endl;
				}

				outFile.close();
			}
		}

		if (INPUTINST.ButtonTestByRect(190, 220, 340, 420))
		{
			if (pack > 0)
			{
				openmode = true;
			}
		}

		if (openmode == true)
		{
			RENDINST.ShowImage(
				INPUTINST.Cursor().x - 80, 
				INPUTINST.Cursor().y - 100,
				L"pack.bmp");

			if (INPUTINST.FindKey("Click")->IsUp())
			{
				openmode = false;

				if (INPUTINST.CursorTestByRect(400, 30, 1110, 630))
				{
					RevealMode = true;
					pack--;
					for (size_t i = 0; i < 5; i++)
					{
						Reveal[i] = false;
					}

					for (size_t i = 0; i < 5; i++)
					{
						RandCard[i] = rand() % CARDS::LAST;
						CardVector[RandCard[i]]->Collected++;
					}
				}
			}
		}

		if (RevealMode == true)
		{
			int ImageX = 135;
			int ImageY = 195;

			float scale = 0.5f;

			int CardX = ImageX * 2 * scale;
			int CardY = ImageY * 2 * scale;

			int PosX[5] =
			{
				750, 550, 950, 640, 900
			};
			int PosY[5] =
			{
				150, 300, 300, 545, 545
			};
	

			if (Reveal[0] == false)
			{
				RENDINST.ShowImageByScale(
					PosX[0] - ImageX * scale, 
					PosY[0] - ImageY * scale, L"Card_Back.bmp", scale);


				if (INPUTINST.ButtonTestByRect(
					PosX[0] - ImageX * scale,
					PosY[0] - ImageY * scale,
					PosX[0] - ImageX * scale + CardX,
					PosY[0] - ImageY * scale + CardY))
				{
					if (CardVector[RandCard[0]]->legendary == true)
					{
						ResMgr::Inst().PlaySound(L"card_turn_over_legendary.ogg");
						ResMgr::Inst().PlaySound(L"legendary.mp3");
					}
					Reveal[0] = true;
				}
			}
			else if (Reveal[0] == true)
			{
				RENDINST.ShowImageByScale(
					PosX[0] - ImageX * scale,
					PosY[0] - ImageY * scale,
					CardVector[RandCard[0]]->m_CardFileName,
					scale
				);
			}



			if (Reveal[1] == false)
			{
				RENDINST.ShowImageByScale(
					PosX[1] - ImageX * scale,
					PosY[1] - ImageY * scale, L"Card_Back.bmp", scale);

				if (INPUTINST.ButtonTestByRect(
					PosX[1] - ImageX * scale,
					PosY[1] - ImageY * scale,
					PosX[1] - ImageX * scale + CardX,
					PosY[1] - ImageY * scale + CardY))
				{
					if (CardVector[RandCard[1]]->legendary == true)
					{
						ResMgr::Inst().PlaySound(L"card_turn_over_legendary.ogg");
						ResMgr::Inst().PlaySound(L"legendary.mp3");
					}
					Reveal[1] = true;
				}
			}
			else if (Reveal[1] == true)
			{
				RENDINST.ShowImageByScale(
					PosX[1] - ImageX * scale,
					PosY[1] - ImageY * scale,
					CardVector[RandCard[1]]->m_CardFileName,
					scale
				);
			}



			if (Reveal[2] == false)
			{
				RENDINST.ShowImageByScale(
					PosX[2] - ImageX * scale,
					PosY[2] - ImageY * scale, L"Card_Back.bmp", scale);

				if (INPUTINST.ButtonTestByRect(
					PosX[2] - ImageX * scale,
					PosY[2] - ImageY * scale,
					PosX[2] - ImageX * scale + CardX,
					PosY[2] - ImageY * scale + CardY))
				{
					if (CardVector[RandCard[2]]->legendary == true)
					{
						ResMgr::Inst().PlaySound(L"card_turn_over_legendary.ogg");
						ResMgr::Inst().PlaySound(L"legendary.mp3");
					}
					Reveal[2] = true;
				}
			}
			else if (Reveal[2] == true)
			{
				RENDINST.ShowImageByScale(
					PosX[2] - ImageX * scale,
					PosY[2] - ImageY * scale, 
					CardVector[RandCard[2]]->m_CardFileName, scale);
			}




			if (Reveal[3] == false)
			{
				RENDINST.ShowImageByScale(
					PosX[3] - ImageX * scale,
					PosY[3] - ImageY * scale, L"Card_Back.bmp", scale);

				if (INPUTINST.ButtonTestByRect(
					PosX[3] - ImageX * scale,
					PosY[3] - ImageY * scale,
					PosX[3] - ImageX * scale + CardX,
					PosY[3] - ImageY * scale + CardY))
				{
					if (CardVector[RandCard[3]]->legendary == true)
					{
						ResMgr::Inst().PlaySound(L"card_turn_over_legendary.ogg");
						ResMgr::Inst().PlaySound(L"legendary.mp3");
					}
					Reveal[3] = true;
				}
			}
			else if (Reveal[3] == true)
			{
				RENDINST.ShowImageByScale(
					PosX[3] - ImageX * scale,
					PosY[3] - ImageY * scale,
					CardVector[RandCard[3]]->m_CardFileName, scale);
			}




			if (Reveal[4] == false)
			{
				RENDINST.ShowImageByScale(
					PosX[4] - ImageX * scale,
					PosY[4] - ImageY * scale, L"Card_Back.bmp", scale);

				if (INPUTINST.ButtonTestByRect(
					PosX[4] - ImageX * scale,
					PosY[4] - ImageY * scale,
					PosX[4] - ImageX * scale + CardX,
					PosY[4] - ImageY * scale + CardY))
				{
					if (CardVector[RandCard[4]]->legendary == true)
					{
						ResMgr::Inst().PlaySound(L"card_turn_over_legendary.ogg");
						ResMgr::Inst().PlaySound(L"legendary.mp3");
					}
					Reveal[4] = true;
				}
			}
			else if (Reveal[4] == true)
			{
				RENDINST.ShowImageByScale(
					PosX[4] - ImageX * scale,
					PosY[4] - ImageY * scale, 
					CardVector[RandCard[4]]->m_CardFileName, scale);
			}




			bool AllOpen = true;
			for (size_t i = 0; i < 5; i++)
			{
				if (Reveal[i] == false)
				{
					AllOpen = false;
					break;
				}
			}

			if (AllOpen == true)
			{
				RENDINST.ShowImageByScale(696, 314, L"mul_confirm.bmp", 0.6);

				if (INPUTINST.ButtonTestByRect(710, 335, 810, 390))
				{
					RevealMode = false;
				}
			}
		}
	}
		break;
	default:
		break;
	}


	int goldx = 1183;
	int goldy = 698;

	int goldcount = 0;
	int tempgold = gold;

	do
	{
		tempgold /= 10;
		goldcount++;
	} while (tempgold != 0);

	RENDINST.ShowImage(1072, 697, L"Gold.bmp");
	RENDINST.ShowNumber(goldx - 7 * goldcount, goldy + 10, gold);

	/*
	wchar_t str[100]{};
	swprintf_s(str, L"MousePosX: %d", INPUTINST.Cursor().x);
	RENDINST.ShowText(6, 15, str);
	swprintf_s(str, L"MousePosY: %d", INPUTINST.Cursor().y);
	RENDINST.ShowText(6, 35, str);
	*/
}