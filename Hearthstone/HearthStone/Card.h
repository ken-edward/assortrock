#pragma once
#include "Character.h"
#include <string>
enum CardType
{
	Minion,
	Spell,
	Weapon,
};

enum SpellType
{
	DealCard,
	DrawCard,
	AreaCard
};

class Hero;
class Card :
	public Character
{
	
public:
	Hero* pHero;
	CardType m_Type;
	SpellType m_SpellType;
	int cost; // 비용
	wchar_t* m_CardFileName;
	wchar_t* m_UnitFileName;
	wchar_t* m_InDeckFile = nullptr;
	wchar_t* CardPlaySound = nullptr;
	wchar_t* CardAttackSound = nullptr;
	wchar_t* CardDeathSound = nullptr;
	wchar_t* TurnEndSound = nullptr;
	wchar_t* m_WeaponName = nullptr;
	int fieldpos = 0;
	float anicount = 0.0f;
	bool poscertain = true;
	void CastingSpell();
	bool caststart = false;
	bool caston = false;


	// 주문
public:
	int deal = 0;
	int draw = 2;
	bool opp = false;
	bool CardDisplayOn = true;
	wchar_t* IdleSound = nullptr;
	bool WeaponPlayStart = false;
	bool WeaponPlayOn = false;
	void WeaponPlaying();
	int spelldamage = 0;

public:
	bool playmode = false;
	bool attackmode = false;
	bool onfield = false;
	bool burning = false;

	
public:
	bool WasOnMulligan = false;

public:
	void Playing();
	bool playstart = false;
	bool playon = false;
	float playcount = 0.0f;
	float playtargetX = 0.0f;
	float playtargetY = 0.0f;
	float playstartX = 0.0f;
	float playstartY = 0.0f;
	bool playchange = false;

public:
	virtual void OnPlay();
	void AttackSound();
	void Update();
	void Attack() override;
	
public:
	void Drawing();
	bool drawstart = false;
	bool drawon = false;
	float drawcount = 0.0f;
	float drawspeed = 200;
	bool drawchange = false;
	float drawstartX = 0.0f;
	float drawstartY = 0.0f;
	float drawtargetX = 0.0f;
	float drawtargetY = 0.0f;
	bool visible = false;
	bool justadded = false;
	bool wielded = false;

public: // 덱 편집
	int Collected = 0;
	

public: // 전투의 함성, 미사용 중.
	int prevHandIndex = -1; // 전함 취소 시 핸드로 다시 돌아갈 때 사용할 핸드 인덱스
	Character* BattleCryTarget = nullptr; // 타겟 캐릭터
	bool phase[5] = {};

public: // 죽음
	void OnDeath();
	bool deathstart = false;
	bool deathon = false;
	void BoomDying();
	bool died = false;

public: // 라그
	Character* pTarget = nullptr;
	int targetplusX = 0;
	int targetplusY = 0;
	float fireX = 0;
	float fireY = 0;

public: // 기본 속성
	// 돌진의 경우 attackable이 true
	bool BattleCry = false;
	bool DeathRattle = false;
	bool Taunt = false;
	bool TauntShow = false;
	bool TurnEnd = false;

public:
	Character* GetRandomTarget();
	bool TurnEndStart = false;
	bool TurnEndOn = false;
	float TurnEndCount = 0.0f;
	bool notattackable = false;
	bool legendary = false;

public:
	virtual void OnTurnEnd();


public:
	Card();
	~Card();
};

