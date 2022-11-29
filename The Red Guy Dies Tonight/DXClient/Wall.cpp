#include "Wall.h"
#include <ColCom.h>
#include <TexCom.h>
#include <ColMgr.h>


Wall::Wall(VEC2 leftBottom, VEC2 rectsize)
{
	tex = (TexCom*)AddCom(new TexCom);
	tex->Init(L"../tex/black.png");

	col = (ColCom*)AddCom(new ColCom);
	ColMgr::Inst().Add(L"wall", col);

	VEC2 center = leftBottom;

	float height = rectsize.y / 2.0f;
	float thickness = rectsize.x / 2.0f;

	center.x += thickness;
	center.y += height;

	SetPos(center);
	SetSize({ thickness, height });
}


Wall::Wall(VEC2 leftBottom, float _height)
{
	tex = (TexCom*)AddCom(new TexCom);
	tex->Init(L"../tex/black.png");

	col = (ColCom*)AddCom(new ColCom);
	ColMgr::Inst().Add(L"wall", col);

	VEC2 center = leftBottom;

	float height = _height / 2.0f;
	float thickness = 50.0f;

	center.x += thickness;
	center.y += height;

	SetPos(center);
	SetSize({ thickness, height });
}


Wall::~Wall()
{
}
