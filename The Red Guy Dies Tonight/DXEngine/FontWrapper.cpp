#include "stdafx.h"
#include "FontWrapper.h"
#include "DXClass.h"


FontWrapper::FontWrapper()
{
}


FontWrapper::~FontWrapper()
{
	if (m_fontWrapper)
	{
		m_fontWrapper->Release();
		m_fontWrapper = nullptr;
	}

	if (m_fontFactory)
	{
		m_fontFactory->Release();
		m_fontFactory = nullptr;
	}
}


void FontWrapper::Init()
{
	if (S_OK != FW1CreateFactory(FW1_VERSION, &m_fontFactory))
	{
		assert(false);
	}

	if (S_OK != m_fontFactory->CreateFontWrapper(DXClass::Inst().GetDevice(),
		L"±Ã¼­", &m_fontWrapper))
	{
		assert(false);
	}
}


void FontWrapper::Draw(const wchar_t* string, VEC2 pos,
	float fontSize,	VEC4 color,	FW1_TEXT_FLAG flag)
{
	UINT32 r = (unsigned int)(color.x * 0xff);
	UINT32 g = (unsigned int)(color.y * 0xff) << 16;
	UINT32 b = (unsigned int)(color.z * 0xff) << 8;
	UINT32 a = (unsigned int)(color.w * 0xff) << 24;

	UINT32 input = r | g | b | a;

	m_fontWrapper->DrawString(DXClass::Inst().GetDeviceContext(),
		string, fontSize, pos.x, pos.y, input, flag);
}