#pragma once
#include <Actor.h>
class RappelWall : public Actor
{
public:
	RappelWall(bool _dir, VEC2 startpos, float height);
	~RappelWall();

public:
	bool dir = 1; // ������ ���� Ÿ�� ��
};

