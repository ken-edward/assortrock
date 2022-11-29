#pragma once
#include "TexCom.h"
#include <vector>


class AniCom :
	public TexCom
{
public:
	AniCom();
	~AniCom();

private:
	// 기존의 Tex* m_Texture는 curFrame처럼 사용됩니다.
	std::vector<Tex*> texVector;

public:
	void Load(std::wstring texname, int _framecount, float _freq);
	void Render() override;

private:
	int frame = 0; // 현재 프레임
	int framecount = 1; // 프레임 수 (texVector.size()와 동일)
	float time = 0.0f; // 경과된 시간
	float freq = 0.1f; // 프레임당 시간. 0이면 큰일남.
};
