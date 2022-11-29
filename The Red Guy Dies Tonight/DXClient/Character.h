#pragma once
#include <Actor.h>
#include <Scene.h>
#include <ColCom.h>
#include <ColMgr.h>
#include <AniState.h>
#include <TimeClass.h>
#include <SoundClass.h>
#include <InputClass.h>

#define armpos arm->GetPos()
#define BlockTest BlockedByWall() || BlockedByDoor() || BlockedByGlass()

struct GunInfo
{
	wstring aim; // ���� ����
	wstring hold; // �ܴ��� ����
	wstring fall; // ���� ��
	float firelen = 100.0f; // �ѱ� ��
	float caselen = 40.0f; // ź�� ��ġ
	float noise = 800.0f; // ���� ����
	AniData* aimtex = nullptr;
	AniData* holdtex = nullptr;
	AniData* falltex = nullptr;
	float firerate = 0.1f;
	bool melee = false; // ���� ����
	bool thrown = false; // ���� ����
	AniData* swing;
	AniData* swing2;

	// ����
	wstring stun;
	wstring stun_forward;
	wstring stab;
	wstring stabfront;

	void SetInfo(wstring gun)
	{
		aim = gun;
		hold = gun + L"hold";
		fall = gun + L"fall";
	}
};


enum MotionState
{
	// ����
	OnLand,
	Falling,
	Stun,
	Death,
	DeathFall,

	// �÷��̾�
	Rappelling,
	Breaching,
	Intro,
};


class Character : public Actor
{
public:
	Character();
	~Character();

protected:
	GunInfo ak47;
	GunInfo spistol;
	GunInfo pistol;
	GunInfo machete;
	GunInfo gun; // ���� ����ϰ� �ִ� ��
	wstring nextGun;

public:
	AniState* arm = nullptr;
	bool dir = 1;
	bool redguy = false;
	bool intro = false;

protected:
	ColCom* nearchara = nullptr;
	MotionState state = OnLand;
	MotionState nextState = OnLand;
	TexCom* rope = nullptr;
	VEC2 ropeend;
	ColCom* dead = nullptr;
	TexCom* redhead = nullptr;
	int LR = 0;
	int UD = 0;
	bool aiming = 0;
	bool firing = 0;
	int ammo = 30;
	int maxammo = 30;
	bool player = 0;
	VEC2 pointer;
	float firetime = 0.0f;
	bool fireready = 1;
	bool stunforward = 1;
	bool boost = 0;
	bool swingdir = 0;
	bool crouch = false;
	VEC2 gravity = { 0.0f, -600.0f };
	int magazine = 30;
	int burstfire = 0;
	bool emptysound = false;

public:
	int hp = 300;

private:
	float steptime = 0.0f;
	Character* meleetarget = nullptr;
	bool goingback = false;
	bool gettingin = false;

public:
	void ChangeState(MotionState);
	void ChangeGun(wstring);

private:
	void Update1();
	void OnLandSetting();
	void FallingSetting();
	void RappellingSetting();
	void StunSetting();
	void DeathSetting();
	void DeathFallSetting();
	void BreachingSetting();

private:
	void Update4();
	void OnLandUpdate();
	void FallingUpdate();
	void RappellingUpdate();
	void StunUpdate();
	void DeathUpdate() {} // �� �� ����
	void DeathFallUpdate();
	void BreachingUpdate();

protected:
	bool BlockedByWall();
	bool BlockedByDoor();
	bool BlockedByGlass();
	bool NearCliff();

protected: // Breaching
	bool charging = true; // ��¡ ����. false �� ���� ����.
	bool prevcharging = true;
	float chargetime = 0.0f;

private:
	void MakeStepNoise(float noise);
	void BulletFire();
	void CameraMove(bool fix = false);
	void GunChange();

private:
	float flipStandard = 90.0f;
	float holdTheta = 0.0f;

protected:
	bool doortry = false;
	bool rappeltry = false;
	bool stairstry = false;
	bool breachtry = false;

private:
	bool fly = false;
	bool breachstarted = false;
	float firstlen = 0.0f;
	float breachtime = 0.0f;
	float stairstime = 0.0f;
	bool stairsready = true;
};