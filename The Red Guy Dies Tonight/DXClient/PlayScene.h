#pragma once
#include <Scene.h>
#include <Vector.h>
class TexCom;
class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

private:
	float len = 250.0f;
	float height = 600.0f;

private:
	void Start() override;

private:
	void CreateFloor(VEC2 leftBot, int size);
	void CreateDoor(VEC2 pos, bool dir);
	void CreateGlass(VEC2 pos, bool dir);
};

