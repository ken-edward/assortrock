#pragma once
#include "TexCom.h"
#include <unordered_map>

class AniData
{
public:
	friend class AniState;
	AniData();
	AniData(std::wstring texname, int _framecount = 1, float _freq = 0.13f, bool _loop = true);
	~AniData();

private:
	std::vector<Tex*> texVector;
	Tex* curTex = nullptr; // AniState의 m_Texture가 됩니다.

public:
	void Load(std::wstring texname, int _framecount = 1, float _freq = 0.13f);
	void Update();
	Tex* GetCurTex() { return curTex; }

private:
	int frame = 0; // 현재 프레임
	int framecount = 1; // 프레임 수
	float time = 0.0f; // 경과 시간
	float freq = 0.1f; // 프레임당 시간

public:
	bool Loop = true;
	bool End = false;

public:
	void Reset();
};

class AniState :
	public TexCom
{
public:
	AniState();
	~AniState();

private:
	std::unordered_map<std::wstring, AniData*> aniMap;
	AniData* curAni = nullptr;
	// m_Texture를 세팅해야 합니다.

public:
	void Add(std::wstring, AniData*);
	void Change(std::wstring);
	int GetIndex();
	wstring GetName() { return curStr; }
	std::wstring curStr;
	void Render() override;
	bool IsEnd() { return curAni->End; }
};

