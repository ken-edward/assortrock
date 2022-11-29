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

private: // �÷��̾� �߰�
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

private: // ��ü �߰�
	list<ColCom*> bodySeen;
	ColCom* bodySight = nullptr;
	void WitnessBody();
	bool seeingbody = false;
	ColCom* curBody = nullptr;

private: // �Ҹ�
	void HearSound();
	void NewSuspicious(VEC2);
	void SusUpdate();
	VEC2 suspoint;
	bool justSuspicious = false;
	float sustime = 0.0f;
	float suswait = 3.0f;

private: // ���� ����
	void Update2();
	float awareness = 0.0f;
	void HigherCaution(NPCState);
	NPCState aware = Unaware;
	NPCState nextAware = Unaware;
	void ChangeAware(NPCState);
	void AwareChange();

private: // Unaware
	void UnawareSetting(); // Unaware ���� �� ����
	void UnawareUpdate(); // Unaware ���� �� ������Ʈ
	VEC2 firstPosition; // ó���� �ִ� ��ġ
	bool firstDir = 1; // ó���� �ٶ󺸴� ����
	bool patrol = false; // �����ϱ�� ������ ������
	void Patrol(); // ������ ��θ� ������
	void KeepPosition(); // ������ ��ġ�� ������
	VEC2 leftPatrolPoint; // ���� ��� ���� ����Ʈ
	VEC2 rightPatrolPoint; // ���� ��� ���� ����Ʈ
	float staytime = 0.0f; // ��ȯ�� ��� �ð�
	float justleft = 0.0f; // ���ڸ� ȸ�� ������

private: // �̵�
	bool Going(float dis = 20.0f); // �������� ���� ����
	bool arrived = false; // ���������� �˸�
	float arrivetime = 0.0f;
	VEC2 destination; // ������

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