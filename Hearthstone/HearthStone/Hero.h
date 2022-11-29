#pragma once
#include "Character.h"
#include "State.h"
#include <vector>
enum EMOTE
{
	EmoteIntro,
	ThankYou,
	WellPlayed,
	Hello,
	Wow,
	Oops,
	Threaten,
	LackofMana,
	Surrender
};
class Card;
class SoundPlayer;
class Hero :
	public Character
{
public:
	static bool MyTurnFlag;
	static bool PrevFlag;
	static int victory;
	static bool mulligan;
	static bool IntroOn;


public:
	std::vector<Card*> Hand;
	std::vector<Card*> Deck;
	std::vector<Card*> Field;
	Hero* enemy;
	SoundPlayer* cardhover;

public:
	int maxmana;
	int mana;
	int fatiguedmg = 0;
	bool heropower;
	int CardCount = 0;

public:
	bool Busy = false;
	bool Drawing = false;
	Card* WieldingWeapon = nullptr;

public:
	int fatiguestack = 0;
	bool fatigueon = false;
	float fatiguecount = 0.0f;

public:
	bool opp = false;
	void Draw();
	virtual void changecard() {}
	void Attack();
	void Summon(Card* pCard, int pos);
	wchar_t* Herobmp;
	wchar_t* Heropowerbmp;

public:
	EMOTE EmoteState;
	bool EmoteStart = false;
	bool EmoteOn = false;
	float EmoteCount = 0.0f;

public:
	void HeroUpdate();
	void fatigue();
	float fatiguespeed = 0;
	float fatigueX = 0;
	float fatigueY = 0;

public:
	Hero();
	~Hero();
};

