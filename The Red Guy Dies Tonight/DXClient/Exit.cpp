#include "Exit.h"



Exit::Exit()
{
	tex = (TexCom*)AddCom(new TexCom(L"../tex/exit.png"));
	col = (ColCom*)AddCom(new ColCom);
	ColMgr::Inst().Add(L"exit", col);
	SetPos({ 8000, 250 });
	col->SetSize({ 300, 100 });
	tex->SetScale(10.0f);
	tex->SetZ(1.0f);
}


Exit::~Exit()
{
}
