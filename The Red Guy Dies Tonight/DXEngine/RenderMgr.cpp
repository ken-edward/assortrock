#include "stdafx.h"
#include "RenderMgr.h"
#include "TexCom.h"
#include "Cam.h"
#include "FontWrapper.h"
#include "DXClass.h"


RenderMgr::RenderMgr()
{
}


RenderMgr::~RenderMgr()
{
}


void RenderMgr::RenderAll()
{
	Cam::Inst().Update();

	auto iter = renderQueue.begin();
	auto end = renderQueue.end();

	for (; iter != end; ++iter)
	{
		(*iter)->Render();
	}

	renderQueue.clear();


	auto fontiter = fontQueue.begin();
	auto fontend = fontQueue.end();

	for (; fontiter != fontend; ++fontiter)
	{
		FontWrapper::Inst().Draw(
			fontiter->str.c_str(),
			fontiter->pos,
			(float)fontiter->size,
			fontiter->color
		);
	}

	DXClass::Inst().RollBack();

	fontQueue.clear();
}


void RenderMgr::Queue(TexCom* texcom, float z)
{
	texcom->SetZ(z);

	auto iter = renderQueue.begin();
	auto end = renderQueue.end();

	for (; iter != end; ++iter)
	{
		if (texcom->GetZ() >= (*iter)->GetZ()) // z가 더 크면, 뒤에서부터
		{
			renderQueue.insert(iter, texcom);
			return;
		}
	}
	// 자기가 제일 작다면 제일 마지막에
	renderQueue.push_back(texcom);
}


void RenderMgr::Font(wstring str, VEC2 pos, int size, VEC4 color)
{
	FontData data;
	data.str = str;
	data.pos = pos;
	data.size = size;
	data.color = color;
	fontQueue.push_back(data);
}