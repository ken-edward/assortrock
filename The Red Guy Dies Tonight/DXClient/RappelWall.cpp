#include "RappelWall.h"
#include <ColCom.h>
#include <ColMgr.h>


RappelWall::RappelWall(bool _dir, VEC2 startpos, float height)
{
	dir = !_dir;

	col = (ColCom*)AddCom(new ColCom);
	ColMgr::Inst().Add(L"wallarea", col);

	SetSize({ 100.0f, height / 2.0f });
	SetPos({ startpos.x, startpos.y + height / 2.0f });
}


RappelWall::~RappelWall()
{
}
