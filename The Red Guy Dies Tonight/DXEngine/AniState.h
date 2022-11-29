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
	Tex* curTex = nullptr; // AniState�� m_Texture�� �˴ϴ�.

public:
	void Load(std::wstring texname, int _framecount = 1, float _freq = 0.13f);
	void Update();
	Tex* GetCurTex() { return curTex; }

private:
	int frame = 0; // ���� ������
	int framecount = 1; // ������ ��
	float time = 0.0f; // ��� �ð�
	float freq = 0.1f; // �����Ӵ� �ð�

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
	// m_Texture�� �����ؾ� �մϴ�.

public:
	void Add(std::wstring, AniData*);
	void Change(std::wstring);
	int GetIndex();
	wstring GetName() { return curStr; }
	std::wstring curStr;
	void Render() override;
	bool IsEnd() { return curAni->End; }
};

