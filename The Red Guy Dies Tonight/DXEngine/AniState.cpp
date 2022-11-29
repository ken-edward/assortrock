#include "stdafx.h"
#include "AniState.h"
#include "Tex.h"
#include "DXClass.h"
#include "TimeClass.h"


AniData::AniData()
{

}


AniData::AniData(std::wstring texname, int _framecount, float _freq, bool _loop)
{
	Load(texname, _framecount, _freq);
	Loop = _loop;
}


AniData::~AniData()
{
	for (int i = 0; i < framecount; i++)
	{
		texVector[i]->Shutdown();
		delete texVector[i];
	}

	texVector.clear();
}


void AniData::Load(std::wstring texname, int _framecount, float _freq)
{
	framecount = _framecount;

	freq = _freq;

	texVector.resize(_framecount);

	for (int i = 0; i < _framecount; i++)
	{
		std::wstring filepath;
		wchar_t Arr[3] = { 0, 0, 0 };
		
		if (i < 10)
		{
			Arr[0] = i + '0';
		}
		else if (i >= 10)
		{
			int front = i / 10;
			Arr[0] = front + '0';
			int back = i % 10;
			Arr[1] = back + '0';
		}
		
		filepath = texname + Arr + L".png";

		texVector[i] = new Tex;

		texVector[i]->Init((wchar_t*)filepath.c_str());
	}
}


void AniData::Update()
{
	time += DT;

	frame = (int)(time / freq); // 내림. 0프레임이 0.00~0.99를 차지하는 느낌.

	if (frame >= framecount)
	{
		if (Loop)
		{
			int num = frame / framecount;

			time -= freq * framecount * num;

			frame -= framecount * num;
		}
		else
		{
			frame = framecount - 1;
			End = true;
		}
	}

	curTex = texVector[frame];
}


void AniData::Reset()
{
	frame = 0;
	time = 0.0f;
	End = false;
	curTex = texVector[0];
}



//////////////////////////////////

AniState::AniState()
{
	InitializeBuffers(DXClass::Inst().GetDevice());
}


AniState::~AniState()
{
	m_Texture = 0;

	auto iter = aniMap.begin();
	auto end = aniMap.end();

	for (; iter != end; ++iter)
	{
		delete iter->second;
	}

	aniMap.clear();
}


void AniState::Add(std::wstring aniName, AniData* newData)
{
	aniMap.insert(pair<wstring, AniData*>(aniName, newData));
}


void AniState::Change(std::wstring aniName)
{
	if (curStr == aniName)
	{
		return;
	}
	else
	{
		curStr = aniName;
	}

	curAni = aniMap.find(aniName)->second;
	curAni->Reset();
	
	m_Texture = curAni->GetCurTex();

	VEC2 tSize;
	tSize.x = (float)(m_Texture->GetWidth() / 2);
	tSize.y = (float)(m_Texture->GetHeight() / 2);
	SetSize(tSize);
}


int AniState::GetIndex()
{
	float time = curAni->time + DT;
	int frame = (int)(time / curAni->freq); // 내림. 0프레임이 0.00~0.99를 차지하는 느낌.
	int framecount = curAni->framecount;
	float freq = curAni->freq;

	if (frame >= framecount)
	{
		if (curAni->Loop == true)
		{
			int num = frame / framecount;
			time -= freq * framecount * num;
			frame -= framecount * num;
		}
		else
		{
			frame = framecount - 1;
		}
	}

	return frame;
}


void AniState::Render()
{
	curAni->Update();

	m_Texture = curAni->GetCurTex();

	TexCom::Render();
}