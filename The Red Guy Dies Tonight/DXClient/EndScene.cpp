#include "EndScene.h"
#include <Windows.h>
#include <InputClass.h>
#include <RenderMgr.h>
#include <stdafx.h>
#include <SoundClass.h>


EndScene::EndScene()
{
}


EndScene::~EndScene()
{
}



void EndScene::Update2()
{
	RenderMgr::Inst().Font(L"Thanks for Watching!", { WIDTH / 2 - 550, HEIGHT / 2 - 70 }, 100);

	if (IsJustDn(L"Space") || IsJustDn(L"Click"))
	{
		ChangeScene(L"TitleScene");
	}

	if (SoundClass::Inst().IsPlaying(L"round gunpoint") == false)
	{
		SoundClass::Inst().Play(L"round gunpoint");
	}
}