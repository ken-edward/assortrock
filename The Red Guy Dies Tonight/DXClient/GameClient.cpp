#include "GameClient.h"
#include "PlayScene.h"
#include "TitleScene.h"
#include "EndScene.h"


GameClient::GameClient()
{
}


GameClient::~GameClient()
{
}


void GameClient::Init()
{
	Scene::AddScene(L"PlayScene", new PlayScene);
	Scene::AddScene(L"TitleScene", new TitleScene);
	Scene::AddScene(L"EndScene", new EndScene);

	Scene::ChangeScene(L"TitleScene");
}