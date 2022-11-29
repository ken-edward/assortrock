#include "stdafx.h"
#include "AniCom.h"
#include "TimeClass.h"
#include "DXClass.h"
#include "Tex.h"


AniCom::AniCom()
{
}


AniCom::~AniCom()
{
	m_Texture = 0;

	for (int i = 0; i < framecount; i++)
	{
		texVector[i]->Shutdown();
		delete texVector[i];
	}

	texVector.clear();
}


void AniCom::Load(std::wstring texname, int _framecount, float _freq)
{
	framecount = _framecount;

	freq = _freq;

	texVector.resize(_framecount);

	InitializeBuffers(DXClass::Inst().GetDevice());

	for (int i = 0; i < _framecount; i++)
	{
		std::wstring filepath;
		wchar_t Arr[2] = { 0, 0 };
		Arr[0] = i + '0';

		filepath = texname + Arr + L".png";

		texVector[i] = new Tex;

		texVector[i]->Init((wchar_t*)filepath.c_str());
	}

	m_Texture = texVector[0];

	size.x = (float)(m_Texture->GetWidth() / 2);
	size.y = (float)(m_Texture->GetHeight() / 2);

	ResetVertex();
}


void AniCom::Render()
{
	time += DT;

	frame = (int)(time / freq); // 내림. 0프레임이 0.00~0.99를 차지하는 느낌.

	if (frame >= framecount)
	{
		int num = frame / framecount;

		time -= freq * framecount * num;

		frame -= framecount * num;
	}

	m_Texture = texVector[frame];

	TexCom::Render();
}