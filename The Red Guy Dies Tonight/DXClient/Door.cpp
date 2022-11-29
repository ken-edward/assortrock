#include "Door.h"



Door::Door(VEC2 leftBot, VEC2 fullsize, bool dir)
{
	tex = (TexCom*)AddCom(new TexCom(L"../tex/door_closed.png"));
	dooropen = (TexCom*)AddCom(new TexCom(L"../tex/door_open.png"));
	dooropen->SetZ(10.0f);

	col = (ColCom*)AddCom(new ColCom);
	ColMgr::Inst().Add(L"door", col);

	size = fullsize / 2.0f;

	SetPos(leftBot + size);
	SetSize(size);

	VEC2 doorsize;
	doorsize.x = size.y * 0.57f;
	doorsize.y = size.y;

	dooropen->SetSize(doorsize);
	dooropen->display = false;

	if (dir == 0) // 왼쪽으로 열림
	{
		dooropen->Flip(false); // 기본이 왼쪽
		
		VEC2 doorpos;

		doorpos.x = leftBot.x - doorsize.x + fullsize.x;
		doorpos.y = pos.y;

		dooropen->SetPos(doorpos);
	}
	else
	{
		dooropen->Flip(true); // 뒤집으면 오른쪽

		VEC2 doorpos;

		doorpos.x = leftBot.x + fullsize.x + doorsize.x - fullsize.x;
		doorpos.y = pos.y;

		dooropen->SetPos(doorpos);
	}
}


Door::~Door()
{
}



void Door::Toggle()
{
	if (open) // 열려 있던 걸 닫음
	{
		open = false;
		tex->display = true;
		dooropen->display = false;
		col->pass = false;
	}
	else // 닫혀 있던 걸 열음
	{
		open = true;
		tex->display = false;
		dooropen->display = true;
		col->pass = true;
	}
}




