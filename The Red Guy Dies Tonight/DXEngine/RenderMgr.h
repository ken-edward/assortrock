#pragma once
#include <list>
#include "Vector.h"
struct FontData
{
	wstring str;
	VEC2 pos;
	int size = 0;
	VEC4 color;
};

class TexCom;
class RenderMgr
{
private:
	RenderMgr();
	~RenderMgr();

public:
	static RenderMgr& Inst()
	{
		static RenderMgr Inst;
		return Inst;
	}

private:
	std::list<TexCom*> renderQueue;
	std::list<FontData> fontQueue;

public:
	void RenderAll();

public:
	void Queue(TexCom* texcom, float z);
	void Font(wstring str,	VEC2 pos, int size = 20, VEC4 color = { 1.0f, 1.0f, 1.0f, 1.0f });
};

