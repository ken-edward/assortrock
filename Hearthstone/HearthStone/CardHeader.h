#pragma once
#include "Card.h"
#include "Hero.h"

/*
	필요 사항
	
	달라란 마법사 소리

*/

enum CARDS
{ // 11번째 줄부터 시작. 텍스트 파일에선 10씩 빼서 대응.
	MURLOC, // 1마나
	RAPTOR, // 2마나
	NOVICE,
	LOOT,
	DARKBOMB,
	FIERY,
	RIDER, // 3마나
	ARCANE,
	SPIDER,
	DALARAN,
	GRIZZLY,
	GNOME, // 4마나
	YETI,
	FIGHTER, // 5마나
	OGRE, // 6마나
	DRBOOM, // 7마나
	FLAMESTRIKE,
	RAG, // 8마나

	// LAST는 마지막 표시용
	LAST
};

// 여긴 순서 상관 없습니다. 그냥 아래에 추가하면 됩니다.
#define CARDCASES \
case MURLOC:\
	newCard = new Murloc;\
	break;\
case RAPTOR:\
	newCard = new Raptor;\
	break;\
case GNOME:\
	newCard = new Gnome;\
	break;\
case YETI:\
	newCard = new Yeti;\
	break;\
case OGRE:\
	newCard = new Ogre;\
	break;\
case DARKBOMB:\
	newCard = new DarkBomb;\
	break;\
case ARCANE:\
	newCard = new Arcane;\
	break;\
case FIGHTER:\
	newCard = new Fighter;\
	break;\
case SPIDER:\
	newCard = new Spider;\
	break;\
case LOOT:\
	newCard = new Loot;\
	break;\
case RAG:\
	newCard = new Rag;\
	break;\
case NOVICE:\
	newCard = new Novice;\
	break;\
case RIDER:\
	newCard = new Rider;\
	break;\
case DRBOOM:\
	newCard = new DrBoom;\
	break;\
case FIERY:\
	newCard = new Fiery;\
	break;\
case FLAMESTRIKE:\
	newCard = new FlameStrike;\
	break;\
case DALARAN:\
	newCard = new Dalaran;\
	break;\
case GRIZZLY:\
	newCard = new Grizzly;\
	break;\





/* 소리 재생 함수 */
// ResMgr::Inst().PlaySound(L"");

#define CARDSOUND \
/* 다른 소리 */ \
ResMgr::Inst().LoadSound(L"Sound", L"victory_screen_start.ogg"); \
ResMgr::Inst().LoadSound(L"Sound", L"Weapon_Durability_Hit_01.ogg"); \
ResMgr::Inst().LoadSound(L"Sound", L"Weapon_Destruction_01.ogg"); \
ResMgr::Inst().LoadSound(L"Sound", L"Taunt.ogg"); \
/* 검은 폭탄 */ \
ResMgr::Inst().LoadSound(L"Sound", L"Darkbomb_Idle_Sound.ogg"); \
ResMgr::Inst().LoadSound(L"Sound", L"Dark_Bomb.ogg"); \
/* 불기둥 */ \
ResMgr::Inst().LoadSound(L"Sound", L"FlameStrike.mp3"); \
/* 신비한 지능 */ \
ResMgr::Inst().LoadSound(L"Sound", L"Arcane_Intellect.ogg"); \
/* 멀록 약탈꾼 */ \
ResMgr::Inst().LoadSound(L"Sound", L"Murloc_Summon.ogg"); \
ResMgr::Inst().LoadSound(L"Sound", L"Murloc_Attack.ogg"); \
ResMgr::Inst().LoadSound(L"Sound", L"Murloc_Death.ogg"); \
/* 돌주먹 오우거 */ \
ResMgr::Inst().LoadSound(L"Sound", L"Ogre_Summon.mp3"); \
ResMgr::Inst().LoadSound(L"Sound", L"Ogre_Attack.mp3"); \
ResMgr::Inst().LoadSound(L"Sound", L"Ogre_Death.mp3"); \
/* 붉은늪지랩터 */ \
ResMgr::Inst().LoadSound(L"Sound", L"Raptor_Summon.mp3");\
ResMgr::Inst().LoadSound(L"Sound", L"Raptor_Attack.mp3");\
ResMgr::Inst().LoadSound(L"Sound", L"Raptor_Death.mp3");\
/* 무쇠가죽 불곰 */ \
ResMgr::Inst().LoadSound(L"Sound", L"Grizzly_Summon.ogg");\
ResMgr::Inst().LoadSound(L"Sound", L"Grizzly_Attack.ogg");\
ResMgr::Inst().LoadSound(L"Sound", L"Grizzly_Death.ogg");\
/* 서리바람 설인 */ \
ResMgr::Inst().LoadSound(L"Sound", L"Yeti_Summon.mp3"); \
ResMgr::Inst().LoadSound(L"Sound", L"Yeti_Attack.mp3"); \
ResMgr::Inst().LoadSound(L"Sound", L"Yeti_Death.mp3"); \
/* 노움 발명가 */ \
ResMgr::Inst().LoadSound(L"Sound", L"Gnome_Summon.mp3"); \
ResMgr::Inst().LoadSound(L"Sound", L"Gnome_Attack.mp3"); \
ResMgr::Inst().LoadSound(L"Sound", L"Gnome_Death.mp3"); \
/* 구덩이 투사 */ \
ResMgr::Inst().LoadSound(L"Sound", L"Fighter_Summon.mp3"); \
ResMgr::Inst().LoadSound(L"Sound", L"Fighter_Attack.mp3"); \
ResMgr::Inst().LoadSound(L"Sound", L"Fighter_Death.mp3"); \
/* 거미 전차 */ \
ResMgr::Inst().LoadSound(L"Sound", L"Spider_Summon.mp3"); \
ResMgr::Inst().LoadSound(L"Sound", L"Spider_Attack.mp3"); \
ResMgr::Inst().LoadSound(L"Sound", L"Spider_Death.mp3"); \
/* 전리품 수집가 */ \
ResMgr::Inst().LoadSound(L"Sound", L"Loot_Summon.mp3"); \
ResMgr::Inst().LoadSound(L"Sound", L"Loot_Attack.mp3"); \
ResMgr::Inst().LoadSound(L"Sound", L"Loot_Death.mp3"); \
/* 전리품 수집가 */ \
ResMgr::Inst().LoadSound(L"Sound", L"Rag_Summon.mp3"); \
ResMgr::Inst().LoadSound(L"Sound", L"Rag_Attack.mp3"); \
ResMgr::Inst().LoadSound(L"Sound", L"Rag_Death.mp3"); \
ResMgr::Inst().LoadSound(L"Sound", L"Rag_TurnEnd.mp3"); \
/* 풋내기 기술자 */ \
ResMgr::Inst().LoadSound(L"Sound", L"Novice_Summon.mp3"); \
ResMgr::Inst().LoadSound(L"Sound", L"Novice_Attack.mp3"); \
ResMgr::Inst().LoadSound(L"Sound", L"Novice_Death.mp3"); \
/* 늑대기수 */ \
ResMgr::Inst().LoadSound(L"Sound", L"Rider_Summon.mp3"); \
ResMgr::Inst().LoadSound(L"Sound", L"Rider_Attack.mp3"); \
ResMgr::Inst().LoadSound(L"Sound", L"Rider_Death.mp3"); \
/* 박사 붐 */ \
ResMgr::Inst().LoadSound(L"Sound", L"DrBoom_Summon.mp3"); \
ResMgr::Inst().LoadSound(L"Sound", L"DrBoom_Attack.mp3"); \
ResMgr::Inst().LoadSound(L"Sound", L"DrBoom_Death.mp3"); \
/* 달라란 마법사 */ \
ResMgr::Inst().LoadSound(L"Sound", L"Dalaran_Summon.mp3"); \
ResMgr::Inst().LoadSound(L"Sound", L"Dalaran_Attack.mp3"); \
ResMgr::Inst().LoadSound(L"Sound", L"Dalaran_Death.mp3"); \
/* 폭탄로봇 */ \
ResMgr::Inst().LoadSound(L"Sound", L"BoomBot_Summon.mp3"); \
ResMgr::Inst().LoadSound(L"Sound", L"BoomBot_Attack.mp3"); \
ResMgr::Inst().LoadSound(L"Sound", L"BoomBot_Death.mp3"); \
ResMgr::Inst().LoadSound(L"Sound", L"BoomBot_Explode.mp3"); \







class Fighter : public Card
{
public:
	Fighter()
	{
		m_Type = CardType::Minion;
		cost = 5;
		atk = 5;
		HP(6);
		Name = L"Fighter";
		m_CardFileName = L"Card_Fighter.bmp";
		m_UnitFileName = L"Unit_Fighter.bmp";
		m_InDeckFile = L"InDeck_Fighter.bmp";
		CardPlaySound = L"Fighter_Summon.mp3";
		CardAttackSound = L"Fighter_Attack.mp3";
		CardDeathSound = L"Fighter_Death.mp3";
	}
};



class Murloc : public Card
{
public:
	Murloc()
	{
		m_Type = CardType::Minion;
		cost = 1;
		atk = 2;
		HP(1);
		Name = L"Murloc";
		m_CardFileName = L"Card_Murloc.bmp";
		m_UnitFileName = L"Unit_Murloc.bmp";
		m_InDeckFile = L"InDeck_Murloc.bmp";
		CardPlaySound = L"Murloc_Summon.ogg";
		CardAttackSound = L"Murloc_Attack.ogg";
		CardDeathSound = L"Murloc_Death.ogg";
	}
};

class Rider : public Card
{
public:
	Rider()
	{
		m_Type = CardType::Minion;
		cost = 3;
		atk = 3;
		HP(1);
		Name = L"Rider";
		m_CardFileName = L"Card_Rider.bmp";
		m_UnitFileName = L"Unit_Rider.bmp";
		m_InDeckFile = L"InDeck_Rider.bmp";
		CardPlaySound = L"Rider_Summon.mp3";
		CardAttackSound = L"Rider_Attack.mp3";
		CardDeathSound = L"Rider_Death.mp3";

		attackable = true;
	}
};






class Raptor : public Card
{
public:
	Raptor()
	{
		m_Type = CardType::Minion;
		cost = 2;
		atk = 3;
		HP(2);
		Name = L"Raptor";
		m_CardFileName = L"Card_Raptor.bmp";
		m_UnitFileName = L"Unit_Raptor.bmp";
		m_InDeckFile = L"InDeck_Raptor.bmp";
		CardPlaySound = L"Raptor_Summon.mp3";
		CardAttackSound = L"Raptor_Attack.mp3";
		CardDeathSound = L"Raptor_Death.mp3";
	}
};


class Grizzly : public Card
{
public:
	Grizzly()
	{
		m_Type = CardType::Minion;
		cost = 3;
		atk = 3;
		HP(3);
		Name = L"Grizzly";
		m_CardFileName = L"Card_Grizzly.bmp";
		m_UnitFileName = L"Unit_Grizzly.bmp";
		m_InDeckFile = L"InDeck_Grizzly.bmp";
		CardPlaySound = L"Grizzly_Summon.ogg";
		CardAttackSound = L"Grizzly_Attack.ogg";
		CardDeathSound = L"Grizzly_Death.ogg";

		Taunt = true;
	}
};


class Dalaran : public Card
{
public:
	Dalaran()
	{
		m_Type = CardType::Minion;
		cost = 3;
		atk = 2;
		HP(4);
		Name = L"Dalaran";
		m_CardFileName = L"Card_Dalaran.bmp";
		m_UnitFileName = L"Unit_Dalaran.bmp";
		m_InDeckFile = L"InDeck_Dalaran.bmp";
		CardPlaySound = L"Dalaran_Summon.mp3";
		CardAttackSound = L"Dalaran_Attack.mp3";
		CardDeathSound = L"Dalaran_Death.mp3";

		spelldamage = 1;
	}
};





class Gnome : public Card
{
public:
	Gnome()
	{
		m_Type = CardType::Minion;
		cost = 4;
		atk = 2;
		HP(4);
		Name = L"Gnome";
		m_CardFileName = L"Card_Gnome.bmp";
		m_UnitFileName = L"Unit_Gnome.bmp";
		m_InDeckFile = L"InDeck_Gnome.bmp";
		CardPlaySound = L"Gnome_Summon.mp3";
		CardAttackSound = L"Gnome_Attack.mp3";
		CardDeathSound = L"Gnome_Death.mp3";
	}
};


class Novice : public Card
{
public:
	Novice()
	{
		m_Type = CardType::Minion;
		cost = 2;
		atk = 1;
		HP(1);
		Name = L"Novice";
		m_CardFileName = L"Card_Novice.bmp";
		m_UnitFileName = L"Unit_Novice.bmp";
		m_InDeckFile = L"InDeck_Novice.bmp";
		CardPlaySound = L"Novice_Summon.mp3";
		CardAttackSound = L"Novice_Attack.mp3";
		CardDeathSound = L"Novice_Death.mp3";
	}
};



class Yeti : public Card
{
public:
	Yeti()
	{
		m_Type = CardType::Minion;
		cost = 4;
		atk = 4;
		HP(5);
		Name = L"Yeti";
		m_CardFileName = L"Card_Yeti.bmp";
		m_UnitFileName = L"Unit_Yeti.bmp";
		m_InDeckFile = L"InDeck_Yeti.bmp";
		CardPlaySound = L"Yeti_Summon.mp3";
		CardAttackSound = L"Yeti_Attack.mp3";
		CardDeathSound = L"Yeti_Death.mp3";
	}
};



class Ogre : public Card
{
public:
	Ogre()
	{
		m_Type = CardType::Minion;
		cost = 6;
		atk = 6;
		HP(7);
		Name = L"Ogre";
		m_CardFileName = L"Card_Ogre.bmp";
		m_UnitFileName = L"Unit_Ogre.bmp";
		m_InDeckFile = L"InDeck_Ogre.bmp";
		CardPlaySound = L"Ogre_Summon.mp3";
		CardAttackSound = L"Ogre_Attack.mp3";
		CardDeathSound = L"Ogre_Death.mp3";
	}
};


class Spider : public Card
{
public:
	Spider()
	{
		m_Type = CardType::Minion;
		cost = 3;
		atk = 3;
		HP(4);
		Name = L"Spider";
		m_CardFileName = L"Card_Spider.bmp";
		m_UnitFileName = L"Unit_Spider.bmp";
		m_InDeckFile = L"InDeck_Spider.bmp";
		CardPlaySound = L"Spider_Summon.mp3";
		CardAttackSound = L"Spider_Attack.mp3";
		CardDeathSound = L"Spider_Death.mp3";
	}
};


class BoomBot : public Card
{
public:
	BoomBot()
	{
		m_Type = CardType::Minion;
		cost = 1;
		atk = 1;
		HP(1);
		Name = L"BoomBot";
		m_CardFileName = L"Card_BoomBot.bmp";
		m_UnitFileName = L"Unit_BoomBot.bmp";
		m_InDeckFile = L"InDeck_BoomBot.bmp";
		CardPlaySound = L"BoomBot_Summon.mp3";
		CardAttackSound = L"BoomBot_Attack.mp3";
		CardDeathSound = L"BoomBot_Death.mp3";

		DeathRattle = true;
	}


};



class DrBoom : public Card
{
public:
	DrBoom()
	{
		m_Type = CardType::Minion;
		cost = 7;
		atk = 7;
		HP(7);
		Name = L"DrBoom";
		m_CardFileName = L"Card_DrBoom.bmp";
		m_UnitFileName = L"Unit_DrBoom.bmp";
		m_InDeckFile = L"InDeck_DrBoom.bmp";
		CardPlaySound = L"DrBoom_Summon.mp3";
		CardAttackSound = L"DrBoom_Attack.mp3";
		CardDeathSound = L"DrBoom_Death.mp3";

		legendary = true;
	}

};



class Loot : public Card
{
public:
	Loot()
	{
		m_Type = CardType::Minion;
		cost = 2;
		atk = 2;
		HP(1);
		Name = L"Loot";
		m_CardFileName = L"Card_Loot.bmp";
		m_UnitFileName = L"Unit_Loot.bmp";
		m_InDeckFile = L"InDeck_Loot.bmp";
		CardPlaySound = L"Loot_Summon.mp3";
		CardAttackSound = L"Loot_Attack.mp3";
		CardDeathSound = L"Loot_Death.mp3";

		DeathRattle = true;
	}
};




class DarkBomb : public Card
{
public:
	DarkBomb()
	{
		m_Type = CardType::Spell;
		m_SpellType = DealCard;
		cost = 2;
		deal = 3;
		Name = L"DarkBomb";
		m_CardFileName = L"Card_DarkBomb.bmp";
		m_InDeckFile = L"InDeck_DarkBomb.bmp";
		CardPlaySound = L"Dark_Bomb.ogg";
		IdleSound = L"Darkbomb_Idle_Sound.ogg";
	}
};


class FlameStrike : public Card
{
public:
	FlameStrike()
	{
		m_Type = CardType::Spell;
		m_SpellType = AreaCard;
		cost = 7;
		deal = 4;
		Name = L"FlameStrike";
		m_CardFileName = L"Card_FlameStrike.bmp";
		m_InDeckFile = L"InDeck_FlameStrike.bmp";
		CardPlaySound = L"FlameStrike.mp3";
	}
};




class Arcane : public Card
{
public:
	Arcane()
	{
		m_Type = CardType::Spell;
		m_SpellType = DrawCard;
		cost = 3;
		draw = 2;
		Name = L"Arcane";
		m_CardFileName = L"Card_Arcane.bmp";
		m_InDeckFile = L"InDeck_Arcane.bmp";
		CardPlaySound = L"Arcane_Intellect.ogg";
	}
};




class Rag : public Card
{
public:
	Rag()
	{
		m_Type = CardType::Minion;
		cost = 8;
		atk = 8;
		HP(8);
		Name = L"Rag";
		m_CardFileName = L"Card_Rag.bmp";
		m_UnitFileName = L"Unit_Rag.bmp";
		m_InDeckFile = L"InDeck_Rag.bmp";
		CardPlaySound = L"Rag_Summon.mp3";
		CardAttackSound = L"Rag_Attack.mp3";
		CardDeathSound = L"Rag_Death.mp3";
		TurnEndSound = L"Rag_TurnEnd.mp3";

		TurnEnd = true;
		notattackable = true;
		legendary = true;
	}
};




class Fiery : public Card
{
public:
	Fiery()
	{
		m_Type = CardType::Weapon;
		cost = 2;
		atk = 3;
		HP(2);
		Name = L"Fiery";
		m_CardFileName = L"Card_Fiery.bmp";
		m_WeaponName = L"Weapon_Fiery.bmp";
		m_InDeckFile = L"InDeck_Fiery.bmp";
	}
};



