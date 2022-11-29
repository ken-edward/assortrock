#include "SoundRing.h"
#include <TimeClass.h>


SoundRing::SoundRing(VEC2 _pos, float _size)
{
	tex = (TexCom*)AddCom(new TexCom(L"../tex/greyring.png"));
	tex->SetZ(-1.0f);
	col = (ColCom*)AddCom(new ColCom);
	col->pos = _pos;
	col->SetType(ColCircle);
	col->radius = _size;
	ColMgr::Inst().Add(L"sound", col);

	size = _size;
	SetPos(_pos);
}


SoundRing::~SoundRing()
{
}


void SoundRing::Update4()
{
	time += DT;

	float limit = 1.0f;
	if (time < limit)
	{
		float speed = size * 2.0f * (limit - time);
		cursize += speed * DT;
		tex->SetSize({ cursize, cursize });
		col->radius = size;
	}
	else
	{
		bAlive = false;
	}
}