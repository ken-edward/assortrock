#include "TitleScene.h"
#include "TitleActor.h"
#include <Cam.h>
#include <SoundClass.h>


TitleScene::TitleScene()
{
}


TitleScene::~TitleScene()
{
}


void TitleScene::Start()
{
	Cam::Inst().Reset();
	AddActor(new TitleActor);
}

void TitleScene::Update2()
{
	if (SoundClass::Inst().IsPlaying(L"round gunpoint") == false)
	{
		SoundClass::Inst().Play(L"round gunpoint");
	}
}