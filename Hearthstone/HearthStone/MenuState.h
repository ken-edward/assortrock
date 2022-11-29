#pragma once
#include "State.h"
#include <vector>
class Card;
enum InMenu
{
	Menu,
	Shop,
	Open,
};

class MenuState :
	public State
{
private:
	int gold = 800;
	int pack = 10;
	InMenu mState = Menu;
	bool alert = false;

private: // ÆÑ °³ºÀ
	bool Reveal[5] = {};
	bool openmode = false;
	bool RevealMode = false;

public:
	int CardCount = 0;
	std::vector<Card*> CardVector;
	int RandCard[5] = {};

public:
	void StateUpdate() override;
	void Start();

public:
	MenuState();
	~MenuState();
};

