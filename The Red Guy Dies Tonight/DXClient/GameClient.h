#pragma once
#include <GameCore.h>
class GameClient : public GameCore
{
public:
	GameClient();
	~GameClient();

public:
	static GameClient& Inst()
	{
		static GameClient Inst;
		return Inst;
	}

public:
	void Init() override;
};

