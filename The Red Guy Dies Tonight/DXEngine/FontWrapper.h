#pragma once
#include <FW1CompileSettings.h>
#include <FW1FontWrapper.h>
#include "Vector.h"


class FontWrapper
{
private:
	FontWrapper();
	~FontWrapper();

public:
	static FontWrapper& Inst()
	{
		static FontWrapper Inst;
		return Inst;
	}

private:
	IFW1Factory* m_fontFactory = nullptr;
	IFW1FontWrapper* m_fontWrapper = nullptr;

public:
	void Init();
	void Draw(const wchar_t* string, VEC2 pos,
		float fontSize,
		VEC4 color = { 0.0f, 0.0f, 0.0f, 1.0f },
		FW1_TEXT_FLAG flag = FW1_TEXT_FLAG::FW1_TOP);
};

