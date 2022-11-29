#include "stdafx.h"
#include "DeckState.h"
#include "RenderMgr.h"
#include "InputMgr.h"
#include "CardHeader.h"


DeckState::DeckState()
	: AllCardIndex(CARDS::LAST)
{
	std::ifstream inFile("Text_AllCard.txt"); // ������ ��� ī�� �б�
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



	std::ifstream inFile2("Text_MyDeck.txt"); // ���� �� ���� �б�

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

	std::ifstream inFile("Text_AllCard.txt"); // ������ ��� ī�� �б�
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



	std::ifstream inFile2("Text_MyDeck.txt"); // ���� �� ���� �б�

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

	
	// ���� ����
	switch (m_InDeck)
	{
	case DeckMenu:
	{
		RENDINST.ShowImage(0, 0, L"Deck_Menu.bmp");

		if (INPUTINST.ButtonTestByRect(1025, 650, 1090, 680)) // �ڷ� ��ư
		{
			SetNextState(STATE::MENU);

		}

		if (INPUTINST.ButtonTestByRect(908, 66, 1063, 128)) // �� ����
		{
			m_InDeck = DeckEdit;
		}
	}
		break;
	case DeckEdit:
	{
		RENDINST.ShowImage(0, 0, L"Deck_Edit.bmp");

		if (INPUTINST.ButtonTestByRect(1025, 650, 1090, 680)) // �Ϸ� ��ư
		{
			m_InDeck = DeckMenu;

			std::ofstream outFile("Text_MyDeck.txt");

			for (int i = 0; i < CARDS::LAST; i++)
			{
				outFile << AllCardIndex[i] << std::endl;
			}

			outFile.close();
		}


		// �� ä���

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


		// AllCardIndex�� ���� �� ǥ��

		int ActualIndex[100] = {}; // �ϴ� 100���� �س����� ī��� 30������ ���ѵž� ��.
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

				CardLineY += 26; // ī�� �ϳ��� 26�ȼ��� ����
				ActualIndex[IndexCount] = i;
				IndexCount++;
			}
		}

		// ���� �ִ� ī�� ���� �� ����
		CardLineY = FirstCardLineY;
		if (INPUTINST.ButtonTestByRect(907, 75, 1066, 637)) // �� ���� ����
		{
			for (size_t i = 0; i < 100; i++) // ActualIndex ��ȯ.
			{
				if (ActualIndex[i] == -1) // �������� �ε����� �پ� �ֱ� ������ -1�̸� �ٷ� ����.
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

	// ���� ����: ī�� ǥ��
	ShowCard(); // ī�� 8�� ǥ��, ������ �ѱ�� ���.

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
	// ������ �ѱ��
	if (INPUTINST.ButtonTestByRect(152, 52, 200, 610)
		|| INPUTINST.ButtonTestByRect(195, 333, 340, 374)) // ���� ������
	{
		if (Page != 0)
		{
			Page--;
		}
	}
	if (INPUTINST.ButtonTestByRect(811, 52, 880, 610)
		|| INPUTINST.ButtonTestByRect(671, 334, 828, 378)) // ���� ������
	{
		if (CardCount > (Page + 1) * 8)
		{
			Page++;
		}
	}


	// ī�� ǥ��
	for (size_t i = 0; i < 8; i++) // �� �������� 8�徿
	{
		// ��� ���� �ڵ�
		int CardIndex = Page * 8 + i; // �� ��° ī���ΰ�. 0���� ����.

		if (CardIndex + 1 > CardCount) // ���� ī�� ���� �ѱ�� �׸���.
		{
			break;
		}

		// ī�� ǥ�� �ڵ�

		int displayX = i % 4; // 0, 1, 2, 3
		int displayY = i / 4; // 0 �Ǵ� 1

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
		// ��ư�鿡 ���� ��ȯ

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