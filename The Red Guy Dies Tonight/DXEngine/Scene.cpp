#include "stdafx.h"
#include "Scene.h"
#include "Script.h"
#include "Actor.h"
#include "ColMgr.h"
#include "Cam.h"
#include "RenderMgr.h"


unordered_map<wstring, Scene*> Scene::sceneMap;
Scene* Scene::curScene = nullptr;
Scene* Scene::nextScene = nullptr;


Scene::Scene()
{

}


Scene::~Scene()
{

}


Scene* Scene::AddScene(wstring name, Scene* scene)
{
	Scene* pScene = GetScene(name);

	if (pScene == nullptr)
	{
		sceneMap.insert(pair<wstring, Scene*>(name, scene));
		return scene;
	}
	else
	{
		delete scene;
		return pScene;
	}
}


Scene* Scene::GetScene(wstring name)
{
	auto iter = sceneMap.find(name);

	if (iter == sceneMap.end())
	{
		return nullptr;
	}
	else
	{
		return iter->second;
	}
}


void Scene::ChangeScene(wstring name)
{
	Scene* pScene = GetScene(name);

	if (pScene == nullptr)
	{
		return;
	}

	if (curScene == nullptr)
	{
		curScene = pScene;
		curScene->Start();
	}
	else if (nextScene == nullptr)
	{
		nextScene = pScene;
	}
}

void Scene::Shutdown()
{
	curScene->ActorClear();
	curScene->ScriptClear();
	curScene->End();

	auto iter = sceneMap.begin();
	auto end = sceneMap.end();

	for (; iter != end; ++iter)
	{
		delete iter->second;
	}

	curScene = 0;
}


void Scene::SceneUpdate()
{
	curScene->Update1();
	curScene->ScriptUpdate1();
	curScene->ActorUpdate1();

	curScene->Update2();
	curScene->ScriptUpdate2();
	curScene->ActorUpdate2();

	ColMgr::Inst().ColAll();
	
	curScene->Update3();
	curScene->ScriptUpdate3();
	curScene->ActorUpdate3();

	curScene->Update4();
	curScene->ScriptUpdate4();
	curScene->ActorUpdate4();

	curScene->Update5();
	curScene->ScriptUpdate5();
	curScene->ActorUpdate5();

	Cam::Inst().Update();
	RenderMgr::Inst().RenderAll();

	curScene->ActorRelease();
	curScene->ScriptRelease();

	curScene->AddWaitingActor();

	SceneChange();
}


void Scene::SceneChange()
{
	if (nextScene != nullptr)
	{
		curScene->ActorClear();
		curScene->ScriptClear();
		curScene->End();

		curScene = nextScene;
		nextScene = nullptr;
		ColMgr::Inst().ClearAll();
		curScene->Start();
	}
}


Script* Scene::AddScript(wstring name, Script* script)
{
	scriptMap.insert(pair<wstring, Script*>(name, script));
	return script;
}
Script* Scene::AddScript(Script* script)
{
	return AddScript(L"None", script);
}
Script* Scene::GetScript(wstring name)
{
	auto iter = scriptMap.find(name);
	if (iter == scriptMap.end())
	{
		return nullptr;
	}
	else
	{
		return iter->second;
	}
}
void Scene::ScriptUpdate1()
{
	auto iter = scriptMap.begin();
	auto end = scriptMap.end();

	for (; iter != end; ++iter)
	{
		iter->second->Update1();
	}
}
void Scene::ScriptUpdate2()
{
	auto iter = scriptMap.begin();
	auto end = scriptMap.end();

	for (; iter != end; ++iter)
	{
		iter->second->Update2();
	}
}
void Scene::ScriptUpdate3()
{
	auto iter = scriptMap.begin();
	auto end = scriptMap.end();

	for (; iter != end; ++iter)
	{
		iter->second->Update3();
	}
}
void Scene::ScriptUpdate4()
{
	auto iter = scriptMap.begin();
	auto end = scriptMap.end();

	for (; iter != end; ++iter)
	{
		iter->second->Update4();
	}
}
void Scene::ScriptUpdate5()
{
	auto iter = scriptMap.begin();
	auto end = scriptMap.end();

	for (; iter != end; ++iter)
	{
		iter->second->Update5();
	}
}
void Scene::ScriptRelease()
{
	auto iter = scriptMap.begin();
	auto end = scriptMap.end();

	for (; iter != end;)
	{
		if (iter->second->bAlive == true)
		{
			++iter;
		}
		else
		{
			delete iter->second;
			iter = scriptMap.erase(iter);
		}
	}
}
void Scene::ScriptClear()
{
	auto iter = scriptMap.begin();
	auto end = scriptMap.end();

	for (; iter != end; ++iter)
	{
		delete iter->second;
	}

	scriptMap.clear();
}

void Scene::AddWaitingActor()
{
	auto iter = actorWait.begin();
	auto end = actorWait.end();

	for (; iter != end; ++iter)
	{
		actorMap.insert(pair<wstring, Actor*>(iter->first, iter->second));
	}

	actorWait.clear();
}

Actor* Scene::AddActor(wstring name, Actor* actor)
{
	actorWait.insert(pair<wstring, Actor*>(name, actor));
	return actor;
}
Actor* Scene::AddActor(Actor* actor)
{
	return AddActor(L"None", actor);
}
Actor* Scene::GetActor(wstring name)
{
	auto iter = actorMap.find(name);

	if (iter == actorMap.end())
	{
		return nullptr;
	}
	else
	{
		return iter->second;
	}
}
void Scene::ActorUpdate1()
{
	auto actorIter = actorMap.begin();
	auto actorEnd = actorMap.end();

	for (; actorIter != actorEnd; ++actorIter)
	{
		actorIter->second->Update1();
		actorIter->second->ComUpdate1();
	}
}
void Scene::ActorUpdate2()
{
	auto actorIter = actorMap.begin();
	auto actorEnd = actorMap.end();

	for (; actorIter != actorEnd; ++actorIter)
	{
		actorIter->second->Update2();
		actorIter->second->ComUpdate2();
	}
}
void Scene::ActorUpdate3()
{
	auto actorIter = actorMap.begin();
	auto actorEnd = actorMap.end();

	for (; actorIter != actorEnd; ++actorIter)
	{
		actorIter->second->Update3();
		actorIter->second->ComUpdate3();
	}
}
void Scene::ActorUpdate4()
{
	auto actorIter = actorMap.begin();
	auto actorEnd = actorMap.end();

	for (; actorIter != actorEnd; ++actorIter)
	{
		actorIter->second->Update4();
		actorIter->second->ComUpdate4();
	}
}
void Scene::ActorUpdate5()
{
	auto actorIter = actorMap.begin();
	auto actorEnd = actorMap.end();

	for (; actorIter != actorEnd; ++actorIter)
	{
		actorIter->second->Update5();
		actorIter->second->ComUpdate5();
	}
}
void Scene::ActorRelease()
{
	auto actorIter = actorMap.begin();
	auto actorEnd = actorMap.end();

	for (; actorIter != actorEnd;)
	{
		if (actorIter->second->bAlive == true)
		{
			actorIter->second->ComRelease();
			++actorIter;
		}
		else
		{
			actorIter->second->ComClear();
			delete actorIter->second;
			actorIter = actorMap.erase(actorIter);
		}
	}
}
void Scene::ActorClear()
{
	auto iter = actorMap.begin();
	auto end = actorMap.end();

	for (; iter != end; ++iter)
	{
		iter->second->ComClear();
		delete iter->second;
	}

	actorMap.clear();
}