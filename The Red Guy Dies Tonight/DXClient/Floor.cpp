#include "Floor.h"
#include <ColCom.h>
#include <TexCom.h>
#include <ColMgr.h>


Floor::Floor(VEC2 leftTop, VEC2 rectsize)
{
	tex = (TexCom*)AddCom(new TexCom);
	tex->Init(L"../tex/black.png");

	col = (ColCom*)AddCom(new ColCom);
	ColMgr::Inst().Add(L"floor", col);

	VEC2 center = leftTop;

	float length = rectsize.x / 2.0f;
	float thickness = rectsize.y / 2.0f;

	center.y -= thickness;
	center.x += length;

	SetPos(center);
	SetSize({ length, thickness });
}


Floor::Floor(VEC2 leftTop, float _length)
{
	tex = (TexCom*)AddCom(new TexCom);
	tex->Init(L"../tex/black.png");

	col = (ColCom*)AddCom(new ColCom);
	ColMgr::Inst().Add(L"floor", col);

	VEC2 center = leftTop;

	float length = _length / 2.0f;
	float thickness = 100.0f / 2.0f;

	center.y -= thickness;
	center.x += length;

	SetPos(center);
	SetSize({ length, thickness });
}



Floor::~Floor()
{
}