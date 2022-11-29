#include "stdafx.h"
#include "DeckState.h"
#include "RenderMgr.h"
#include "InputMgr.h"
#include "CardHeader.h"


DeckState::DeckState()
	: AllCardIndex(CARDS::LAST)
{
	std::ifstream inFile("Text_AllCard.txt"); // 구현된 모든 카드 읽기
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

		if (chr[0] > 2)
		{
			chr[0] = 2;
		}

		newCard->Collected = chr[0];
		if (newCard->legendary == true && chr[0] >= 2)
		{
			newCard->Collected = 1;
		}
		CardVector.push_back(newCard);
		CardCount += 1;
	}

	inFile.close();



	std::ifstream inFile2("Text_MyDeck.txt"); // 실제 덱 상태 읽기

	int CardIndex = 0;
	while (!inFile2.eof() && CardIndex < CARDS::LAST)
	{
		inFile2.getline(chr, 2);

		if (chr[0] == 0)
		{
			chr[0] = 0;
		}
		else if (chr[0] >= '0' && chr[0] <= '9')
		{
			chr[0] -= '0';
		}

		if (chr[0] > 2)
		{
			chr[0] = 2;
		}

		if (CardVector[CardIndex]->legendary == true && chr[0] == 2)
		{
			chr[0] = 2;
		}

		if (chr[0] > CardVector[CardIndex]->Collected)
		{
			chr[0] = CardVector[CardIndex]->Collected;
		}


		AllCardIndex[CardIndex] = chr[0];

		CardIndex += 1;
	}

	inFile2.close();
}


DeckState::~DeckState()
{
	
}


void DeckState::Start()
{
	bgm->NewSoundPlay(L"Collection Manager.ogg");

	CardCount = 0;

	std::ifstream inFile("Text_AllCard.txt"); // 구현된 모든 카드 읽기
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
		if (CardVector[CardCount]->legendary == true && chr[0] >= 2)
		{
			CardVector[CardCount]->Collected = 1;
		}
		CardCount += 1;
	}

	inFile.close();



	std::ifstream inFile2("Text_MyDeck.txt"); // 실제 덱 상태 읽기

	int CardIndex = 0;
	while (!inFile2.eof() && CardIndex < CARDS::LAST)
	{
		inFile2.getline(chr, 2);


		if (chr[0] == 0)
		{
			chr[0] = 0;
		}
		else if (chr[0] >= '0' && chr[0] <= '9')
		{
			chr[0] -= '0';
		}

		if (chr[0] > 2)
		{
			chr[0] = 2;
		}

		if (CardVector[CardIndex]->legendary == true && chr[0] == 2)
		{
			chr[0] = 2;
		}

		if (chr[0] > CardVector[CardIndex]->Collected)
		{
			chr[0] = CardVector[CardIndex]->Collected;
		}


		AllCardIndex[CardIndex] = chr[0];

		CardIndex += 1;
	}

	inFile2.close();
}


void DeckState::StateUpdate()
{
	if (bgm->IsPlay() == false)
	{
		bgm->NewSoundPlay(L"Collection Manager.ogg");
	}

	
	// 개별 사항
	switch (m_InDeck)
	{
	case DeckMenu:
	{
		RENDINST.ShowImage(0, 0, L"Deck_Menu.bmp");

		if (INPUTINST.ButtonTestByRect(1025, 650, 1090, 680)) // 뒤로 버튼
		{
			SetNextState(STATE::MENU);

		}

		if (INPUTINST.ButtonTestByRect(908, 66, 1063, 128)) // 덱 편집
		{
			m_InDeck = DeckEdit;
		}
	}
		break;
	case DeckEdit:
	{
		RENDINST.ShowImage(0, 0, L"Deck_Edit.bmp");

		if (INPUTINST.ButtonTestByRect(1025, 650, 1090, 680)) // 완료 버튼
		{
			m_InDeck = DeckMenu;

			std::ofstream outFile("Text_MyDeck.txt");

			for (int i = 0; i < CARDS::LAST; i++)
			{
				outFile << AllCardIndex[i] << std::endl;
			}

			outFile.close();
		}


		// 덱 채우기

		int PressedIndex = GetIndex();

		if (PressedIndex != -1 && PressedIndex < AllCardIndex.size())
		{
			if (INPUTINST.FindKey("Click")->IsDown())
			{
				if (AllCardIndex[PressedIndex] < 2
					&& CardVector[PressedIndex]->Collected > AllCardIndex[PressedIndex])
				{
					AllCardIndex[PressedIndex] += 1;
				}
			}
			else if (INPUTINST.FindKey("RClick")->IsDown())
			{
				if (AllCardIndex[PressedIndex] > 0)
				{
					AllCardIndex[PressedIndex] -= 1;
				}
			}
		}


		// AllCardIndex에 따른 덱 표시

		int ActualIndex[100] = {}; // 일단 100으로 해놓지만 카드는 30장으로 제한돼야 함.
		int IndexCount = 0;
		for (size_t i = 0; i < 100; i++)
		{
			ActualIndex[i] = -1;
		}
		int FirstCardLineY = 111;
		int CardLineY = FirstCardLineY;
		for (size_t i = 0; i < AllCardIndex.size(); i++)
		{
			if (AllCardIndex[i] >= 1)
			{
				RENDINST.ShowImage(905, CardLineY, CardVector[i]->m_InDeckFile);

				if (AllCardIndex[i] >= 2)
				{
					RENDINST.ShowImage(1042, CardLineY, L"deck_2.bmp");
				}

				CardLineY += 26; // 카드 하나에 26픽셀씩 차지
				ActualIndex[IndexCount] = i;
				IndexCount++;
			}
		}

		// 덱에 있는 카드 누를 때 빼기
		CardLineY = FirstCardLineY;
		if (INPUTINST.ButtonTestByRect(907, 75, 1066, 637)) // 덱 편집 영역
		{
			for (size_t i = 0; i < 100; i++) // ActualIndex 순환.
			{
				if (ActualIndex[i] == -1) // 실질적인 인덱스만 붙어 있기 때문에 -1이면 바로 나감.
				{
					break;
				}

				if (INPUTINST.ButtonTestByRect(907, CardLineY, 1066, CardLineY + 26))
				{
					AllCardIndex[ActualIndex[i]] -= 1;
					break;
				}

				CardLineY += 26;
			}
		}
	}
		break;
	default:
		break;
	}

	// 공통 사항: 카드 표시
	ShowCard(); // 카드 8장 표시, 페이지 넘기기 담당.

	/*
	wchar_t str[100]{};
	swprintf_s(str, L"MousePosX: %d", INPUTINST.Cursor().x);
	RENDINST.ShowText(6, 15, str);
	swprintf_s(str, L"MousePosY: %d", INPUTINST.Cursor().y);
	RENDINST.ShowText(6, 35, str);
	*/
}


void DeckState::ShowCard()
{
	// 페이지 넘기기
	if (INPUTINST.ButtonTestByRect(152, 52, 200, 610)
		|| INPUTINST.ButtonTestByRect(195, 333, 340, 374)) // 이전 페이지
	{
		if (Page != 0)
		{
			Page--;
		}
	}
	if (INPUTINST.ButtonTestByRect(811, 52, 880, 610)
		|| INPUTINST.ButtonTestByRect(671, 334, 828, 378)) // 다음 페이지
	{
		if (CardCount > (Page + 1) * 8)
		{
			Page++;
		}
	}


	// 카드 표시
	for (size_t i = 0; i < 8; i++) // 한 페이지에 8장씩
	{
		// 출력 방지 코드
		int CardIndex = Page * 8 + i; // 몇 번째 카드인가. 0부터 시작.

		if (CardIndex + 1 > CardCount) // 보유 카드 수를 넘기면 그만함.
		{
			break;
		}

		// 카드 표시 코드

		int displayX = i % 4; // 0, 1, 2, 3
		int displayY = i / 4; // 0 또는 1

		float scale = 0.56f;

		float tagscale = 0.6f;

		if (CardVector[CardIndex]->Collected >= 2)
		{
			RENDINST.ShowImageByScale(
				(275 + 156 * displayX) - 54 * tagscale,
				(350 + 240 * displayY) - 34 * tagscale,
				L"deck_multiple.bmp",
				tagscale
			);
		}
		


		RENDINST.ShowImageByScale(
			(273 + 156 * displayX) - CardVector[CardIndex]->ImageX * scale,
			(235 + 240 * displayY) - CardVector[CardIndex]->ImageY * scale,
			CardVector[CardIndex]->m_CardFileName,
			scale
		);

		float xScale = 0.6f;
		if (CardVector[CardIndex]->Collected == 0)
		{
			RENDINST.ShowImageByScale(
				(273 + 156 * displayX) - 92 * xScale,
				(235 + 240 * displayY) - 148 * xScale,
				L"xmark.bmp",
				xScale
			);
		}
	}
}



int DeckState::GetIndex()
{
	if (INPUTINST.FindKey("Click")->IsDown()
		|| INPUTINST.FindKey("RClick")->IsDown())
	{
		// 버튼들에 따라 반환

		if (INPUTINST.CursorTestByRect(195, 125, 350, 340))
		{
			return Page * 8 + 0;
		}

		if (INPUTINST.CursorTestByRect(352, 125, 502, 340))
		{
			return Page * 8 + 1;
		}

		if (INPUTINST.CursorTestByRect(500, 125, 650, 340))
		{
			return Page * 8 + 2;
		}

		if (INPUTINST.CursorTestByRect(657, 125, 818, 340))
		{
			return Page * 8 + 3;
		}



		
		if (INPUTINST.CursorTestByRect(195, 360, 350, 580))
		{
			return Page * 8 + 4;
		}
		
		if (INPUTINST.CursorTestByRect(352, 360, 502, 580))
		{
			return Page * 8 + 5;
		}
		
		if (INPUTINST.CursorTestByRect(500, 360, 650, 580))
		{
			return Page * 8 + 6;
		}

		if (INPUTINST.CursorTestByRect(657, 360, 818, 580))
		{
			return Page * 8 + 7;
		}
	}


	return -1;
}