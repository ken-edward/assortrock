#include "Stairs.h"
#include "MapMgr.h"



Stairs::Stairs(VEC2 midBot, bool up, bool down) : up(up), down(down)
{
	tex = (TexCom*)AddCom(new TexCom(L"../tex/stairs.png"));
	tex->SetZ(10.0f);
	col = (ColCom*)AddCom(new ColCom);
	ColMgr::Inst().Add(L"stairs", col);
	VEC2 tSize = VEC2{ 38.0f, 52.0f } * 4.6f;
	SetSize(tSize / 2.0f);
	SetPos({ midBot.x, midBot.y + tSize.y / 2.0f });
	MapMgr::Inst().RegisterStairs(this);
}


Stairs::~Stairs()
{
}