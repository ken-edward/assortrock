#pragma once
#include "Character.h"

enum NPCState
{
	Unaware,
	Suspicious,
	Cautious,
	Hostile
};

class NPC : public Character
{
public:
	NPC(VEC2, bool, bool pistol = false);
	~NPC();

private: // 플레이어 발견
	ColCom* armSightLine = nullptr;
	ColCom* posSightLine = nullptr;
	Character* pPlayer = nullptr;
	VEC2 lastKnownPosition;
	bool playerInit = false;
	void WitnessPlayer();
	bool CheckDegree(float);
	bool seeing = false;
	float lastSeen = 0.0f;

public:
	static bool alarmed;
	static bool redguydied;
	void SetAsRedGuy();

private: // 시체 발견
	list<ColCom*> bodySeen;
	ColCom* bodySight = nullptr;
	void WitnessBody();
	bool seeingbody = false;
	ColCom* curBody = nullptr;

private: // 소리
	void HearSound();
	void NewSuspicious(VEC2);
	void SusUpdate();
	VEC2 suspoint;
	bool justSuspicious = false;
	float sustime = 0.0f;
	float suswait = 3.0f;

private: // 공통 사항
	void Update2();
	float awareness = 0.0f;
	void HigherCaution(NPCState);
	NPCState aware = Unaware;
	NPCState nextAware = Unaware;
	void ChangeAware(NPCState);
	void AwareChange();

private: // Unaware
	void UnawareSetting(); // Unaware 진입 시 세팅
	void UnawareUpdate(); // Unaware 상태 시 업데이트
	VEC2 firstPosition; // 처음에 있던 위치
	bool firstDir = 1; // 처음에 바라보던 방향
	bool patrol = false; // 순찰하기로 지정된 유닛임
	void Patrol(); // 지정된 경로를 순찰함
	void KeepPosition(); // 지정된 위치를 유지함
	VEC2 leftPatrolPoint; // 순찰 경로 좌측 포인트
	VEC2 rightPatrolPoint; // 순찰 경로 우측 포인트
	float staytime = 0.0f; // 반환점 대기 시간
	float justleft = 0.0f; // 제자리 회전 방지용

private: // 이동
	bool Going(float dis = 20.0f); // 목적지에 가게 만듬
	bool arrived = false; // 도착했음을 알림
	float arrivetime = 0.0f;
	VEC2 destination; // 목적지

public: // Unaware
	void SetPatrolRoute(float, float, float);

private: // Suspicous
	void SuspicousSetting();
	void SuspicousUpdate();

private: // Cautious
	void CautiousSetting();
	void CautiousUpdate();
	void Search();
	float blockcool = 0.0f;
	float stairscool = 0.0f;
	ColCom* stairscol = nullptr;
	ColCom* prevstairs = nullptr;

private: // Hostile
	void HostileSetting();
	void HostileUpdate();
	void FindSiren();
	bool checked = true;
	bool findingsiren = false;
	float ceasefire = 0.0f;
	float burstcool = 0.0f;
};