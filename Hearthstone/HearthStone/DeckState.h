#pragma once
#include "State.h"
#include <vector>

enum InDeck
{
	DeckMenu,
	DeckEdit,
};

class Card;
class DeckState :
	public State
{
public:
	InDeck m_InDeck;
	void Start() override;
	void StateUpdate() override;
	std::vector<Card*> CardVector;
	std::vector<int> AllCardIndex;
	int Page = 0;
	void ShowCard();
	int CardCount = 0;

public:
	int GetIndex();

public:
	DeckState();
	~DeckState();
};

