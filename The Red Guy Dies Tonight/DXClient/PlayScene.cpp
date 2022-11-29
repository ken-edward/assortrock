#include "PlayScene.h"
#include "Player.h"
#include "Floor.h"
#include "Wall.h"
#include <ColMgr.h>
#include <Cam.h>
#include "NPC.h"
#include "RappelWall.h"
#include "Window.h"
#include "Door.h"
#include "Stairs.h"
#include "UI.h"
#include "Siren.h"
#include "Exit.h"


PlayScene::PlayScene()
{
}


PlayScene::~PlayScene()
{
}

class Background : public Actor
{
public:
	Background(float _scale)
	{
		tex = (TexCom*)AddCom(new TexCom(L"../tex/background.png"));
		tex->SetZ(30.0f);
		tex->SetScale(_scale);
		SetSize(tex->GetSize());
		tex->SetPos({ size.x * _scale, size.y * _scale });
	}
};

class Backwall : public Actor
{
public:
	Backwall(VEC2 leftBot, VEC2 _size)
	{
		tex = (TexCom*)AddCom(new TexCom(L"../tex/tile.png"));
		tex->SetZ(20.0f);
		VEC2 texpos = leftBot + _size / 2.0f;
		tex->SetSize(_size / 2.0f);
		tex->SetPos(texpos);
	}
};


void PlayScene::Start()
{
	float backscale = 4.5f;
	Actor* back = AddActor(new Background(backscale));

	// UI
	AddActor(new UI);

	// �������� ��
	AddActor(new Wall({ -20000.0f, 0.0f }, { 20000.0f, 30000.0f })); // ���� ū ��
	AddActor(new Floor({ -20000.0f, 0.0f }, { 40000.0f, 30000.0f })); // �Ʒ��� ū �ٴ�
	AddActor(new Wall({ back->size.x * 2 * backscale, 0.0f }, { 20000.0f, 30000.0f })); // ������ ū ��
	AddActor(new Floor({ 0.0f, back->size.y * 2 * backscale + 10000.0f }, { back->size.x * 2 * backscale, 10000.0f })); // ���� ū õ��

	// �÷��̾� ��ġ
	AddActor(L"Player", new Player({ 200.0f, 0.0f }));

	NPC* curnpc = nullptr;
	//////// ��ܽ� ����
	//// 1�� (1000~3000, 0~600)
	CreateFloor({ 1000.0f, 0.0f }, 8);
	CreateDoor({ 1000.0f, 0.0f }, 0); // ���� ��
	CreateDoor({ 3000.0f, 0.0f }, 1); // ���� ��
	AddActor(new RappelWall(0, { 1000.0f, 0.0f }, 600.0f)); // 1�� ���� ���纮
	AddActor(new RappelWall(1, { 3000.0f, 0.0f }, 1800.0f)); // 1~3�� ���� ���纮
	AddActor(new NPC({ 1600.0f, 0.0f }, 1, 1));
	AddActor(new NPC({ 2000.0f, 0.0f }, 0, 1));
	AddActor(new Stairs({ 2500.0f, 0 }, 1, 0));

	//// 2�� (1500~3000, 600~1200)
	CreateFloor({ 1500.0f, 600.0f }, 6);
	CreateDoor({ 1500.0f, 600.0f }, 0); // ���� ��
	CreateGlass({ 3000.0f, 600.0f }, 1); // ���� ����â
	AddActor(new RappelWall(0, { 1500.0f, 600.0f }, 600.0f)); // 2�� ���� ���纮
	curnpc = (NPC*)AddActor(new NPC({ 2800.0f, 600.0f }, 1));
	curnpc->SetPatrolRoute(1800, 2700, 700);
	AddActor(new Stairs({ 2500.0f, 600 }, 1, 1));
	AddActor(new Siren({ 2750, 600 }));


	//// 3�� (2000~3000, 1200~1800)
	CreateFloor({ 2000, 1200.0f }, 4);
	CreateDoor({ 2000.0f, 1200.0f }, 0); // ���� ��
	CreateGlass({ 3000.0f, 1200.0f }, 1); // ���� ����â
	AddActor(new RappelWall(0, { 2000.0f, 1200.0f }, 600.0f)); // 3�� ���� ���纮
	AddActor(new NPC({ 1850.0f, 1200.0f }, 0));
	AddActor(new NPC({ 2800.0f, 1200.0f }, 0));
	AddActor(new Stairs({ 2500.0f, 1200 }, 0, 1));


	//////// ���� ����
	// 1�� (4250~6750, 0~600)
	CreateFloor({ 4250, 0 }, 10);
	AddActor(new RappelWall(0, { 4250, 0 }, 1200)); // 1~2�� ���� ���纮
	AddActor(new RappelWall(1, { 6750, 0 }, 1200)); // 1~2�� ���� ���纮
	CreateDoor({ 4250.0f, 0 }, 0); // ���� ��
	CreateDoor({ 6750.0f, 0 }, 1); // ���� ��
	AddActor(new NPC({ 4900, 0 }, 0, 1));
	AddActor(new NPC({ 5400, 0 }, 1, 1));
	AddActor(new NPC({ 5800, 0 }, 0, 1));
	AddActor(new NPC({ 6000, 0 }, 0, 1));
	AddActor(new Siren({ 6500, 0 }));
	AddActor(new Stairs({ 5000, 0 }, 1, 0));
	AddActor(new Stairs({ 6250, 0 }, 1, 0));
	// 2�� (4250~6750, 600~1200)
	CreateFloor({ 4250, 600 }, 10);
	CreateGlass({ 4250, 600 }, 0); // ���� ����â
	CreateGlass({ 6750, 600 }, 1); // ���� ����â
	AddActor(new Stairs({ 5000, 600 }, 1, 1));
	AddActor(new Stairs({ 6250, 600 }, 0, 1));
	AddActor(new NPC({ 5500, 600 }, 1, 0));
	// 3�� (4500~6500, 1200~1800)
	CreateFloor({ 4500, 1200 }, 8);
	CreateDoor({ 4500, 1200 }, 0); // ���� ��
	CreateDoor({ 6500, 1200 }, 1); // ���� ��
	AddActor(new Stairs({ 5000, 1200 }, 1, 1));
	AddActor(new NPC({ 5750, 1200 }, 1, 1));
	AddActor(new Siren({ 6000, 1200 }));
	// 4�� (4500~6500, 1800~2400)
	CreateFloor({ 4500, 1800 }, 8);
	AddActor(new RappelWall(0, { 4500, 600 }, 1800)); // 2~4�� ���� ���纮
	AddActor(new RappelWall(1, { 6500, 600 }, 1800)); // 2~4�� ���� ���纮
	CreateGlass({ 4500, 1800 }, 0); // ���� ����â
	CreateGlass({ 6500, 1800 }, 1); // ���� ����â
	curnpc = (NPC*)AddActor(new NPC({ 4800, 1800 }, 1, 0));
	curnpc->SetPatrolRoute(4800, 6200, 1900);
	curnpc = (NPC*)AddActor(new NPC({ 6200, 1800 }, 0, 0));
	curnpc->SetPatrolRoute(4800, 6200, 1900);
	AddActor(new NPC({ 6000, 0 }, 0, 1));
	AddActor(new Stairs({ 5000, 1800 }, 0, 1));
	AddActor(new Stairs({ 6000, 1800 }, 1, 0));
	// 5�� (4750~6250, 2400~3000)
	CreateFloor({ 4750, 2400 }, 6);
	CreateDoor({ 4750, 2400 }, 0); // ���� ��
	CreateDoor({ 6250, 2400 }, 1); // ���� ��
	AddActor(new NPC({ 5250, 2400 }, 0, 1));
	AddActor(new Siren({ 5600, 2400 }));
	AddActor(new Stairs({ 6000, 2400 }, 1, 1));
	// 6�� (4750~6250, 3000~3600)
	CreateFloor({ 4750, 3000 }, 6);
	AddActor(new RappelWall(0, { 4750, 2400 }, 1200)); // 5~6�� ���� ���纮
	AddActor(new RappelWall(1, { 6250, 2400 }, 1200)); // 5~6�� ���� ���纮
	CreateGlass({ 4750, 3000 }, 0); // ���� ����â
	CreateGlass({ 6250, 3000 }, 1); // ���� ����â
	AddActor(new Stairs({ 5250, 3000 }, 1, 0));
	AddActor(new Stairs({ 6000, 3000 }, 0, 1));
	AddActor(new NPC({ 5000, 3000 }, 0, 0));
	AddActor(new NPC({ 5500, 3000 }, 1, 0));
	// 7�� (5000~6000, 3600~4200)
	CreateFloor({ 5000, 3600 }, 4);
	AddActor(new RappelWall(0, { 5000, 3600 }, 600)); // 7�� ���� ���纮
	AddActor(new RappelWall(1, { 6000, 3600 }, 600)); // 7�� ���� ���纮
	CreateGlass({ 5000, 3600 }, 0); // ���� ����â
	CreateGlass({ 6000, 3600 }, 1); // ���� ����â
	AddActor(new Stairs({ 5250, 3600 }, 0, 1));
	AddActor(new NPC({ 5400, 3600 }, 1, 0));
	((NPC*)AddActor(new NPC({ 5700, 3600 }, 0, 1)))->SetAsRedGuy();
	AddActor(new Exit);
	

	ColMgr::Inst().Link(L"character", L"floor");
	ColMgr::Inst().Link(L"character", L"wallarea");
	ColMgr::Inst().Link(L"character", L"sound");

	ColMgr::Inst().Link(L"character", L"wall");
	ColMgr::Inst().Link(L"character", L"door");
	ColMgr::Inst().Link(L"character", L"window");
	ColMgr::Inst().Link(L"character", L"stairs");
	ColMgr::Inst().Link(L"character", L"siren");

	ColMgr::Inst().Link(L"nearchara", L"door");
	ColMgr::Inst().Link(L"nearchara", L"window");
	ColMgr::Inst().Link(L"nearchara", L"frame");

	ColMgr::Inst().Link(L"bullet", L"floor");
	ColMgr::Inst().Link(L"bullet", L"wall");
	ColMgr::Inst().Link(L"bullet", L"character");
	ColMgr::Inst().Link(L"bullet", L"window");
	ColMgr::Inst().Link(L"bullet", L"door");

	ColMgr::Inst().Link(L"bulletcase", L"floor");
	ColMgr::Inst().Link(L"bulletcase", L"wall");

	ColMgr::Inst().Link(L"sight", L"character");
	ColMgr::Inst().Link(L"sight", L"door");
	ColMgr::Inst().Link(L"sight", L"wall");
	ColMgr::Inst().Link(L"sight", L"floor");
	ColMgr::Inst().Link(L"deadsight", L"dead");
	ColMgr::Inst().Link(L"deadsight", L"door");
	ColMgr::Inst().Link(L"deadsight", L"wall");
	ColMgr::Inst().Link(L"deadsight", L"floor");
	ColMgr::Inst().Link(L"character", L"exit");


	Cam::Inst().SetSize(2.0f);
}


void PlayScene::CreateFloor(VEC2 leftBot, int size) // ������ õ���� �����մϴ�.
{
	// 1�� ����
	for (int i = 0; i < size; i++)
	{
		AddActor(new Backwall({ leftBot.x + len * i, leftBot.y }, { len, 500.0f }));
	}
	// 1�� õ��
	AddActor(new Floor({ leftBot.x, leftBot.y + 600.0f }, len * size));
}


void PlayScene::CreateDoor(VEC2 pos, bool dir)
{
	if (dir == 0) // ���� ��, pos�� ���ϴ�
	{
		AddActor(new Door(pos, { 10.0f, 250.0f }, dir));

		AddActor(new Wall({ pos.x, pos.y + 250.0f }, 250.0f));
	}
	else // ������ ��, pos�� ���ϴ�
	{
		AddActor(new Door({ pos.x - 10.0f, pos.y }, { 10.0f, 250.0f }, dir));

		AddActor(new Wall({ pos.x - 100.0f, pos.y + 250.0f }, 250.0f));
	}
}


void PlayScene::CreateGlass(VEC2 pos, bool dir)
{
	if (dir == 0) // ���� ����â, pos�� ���ϴ�
	{
		AddActor(new Window(pos, { 40.0f, 500.0f }, dir));
	}
	else // ������ ����â, pos�� ���ϴ�
	{
		AddActor(new Window({ pos.x - 40.0f, pos.y }, { 40.0f, 500.0f }, dir));
	}
}