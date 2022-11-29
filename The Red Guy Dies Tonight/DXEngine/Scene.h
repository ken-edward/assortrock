#pragma once
#include <unordered_map>
using namespace std;
class Actor;
class Script;
class Scene
{
public:
	Scene();
	virtual ~Scene();

private:
	static std::unordered_map<wstring, Scene*> sceneMap;
	static Scene* curScene;
	static Scene* nextScene;

public:
	static Scene* GetCurScene() { return curScene; }
	static Scene* AddScene(wstring, Scene*);
	static Scene* GetScene(wstring);
	static void ChangeScene(wstring);
	static void Shutdown();

public:
	static void SceneUpdate();
	static void SceneChange();

public:
	virtual void Start() {}
	virtual void End() {}
	virtual void Update1() {}
	virtual void Update2() {}
	virtual void Update3() {}
	virtual void Update4() {}
	virtual void Update5() {}

private:
	unordered_multimap<wstring, Script*> scriptMap;
	unordered_multimap<wstring, Actor*> actorMap;
	unordered_multimap<wstring, Actor*> actorWait;

public:
	Script* AddScript(wstring, Script*);
	Script* AddScript(Script*);
	Script* GetScript(wstring);
	void ScriptUpdate1();
	void ScriptUpdate2();
	void ScriptUpdate3();
	void ScriptUpdate4();
	void ScriptUpdate5();
	void ScriptRelease();
	void ScriptClear();

private:
	void AddWaitingActor();

public:
	Actor* AddActor(wstring, Actor*);
	Actor* AddActor(Actor*);
	Actor* GetActor(wstring);
	void ActorUpdate1();
	void ActorUpdate2();
	void ActorUpdate3();
	void ActorUpdate4();
	void ActorUpdate5();
	void ActorRelease();
	void ActorClear();
};