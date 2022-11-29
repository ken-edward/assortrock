#pragma once
#include <fmod.hpp>

class GameSound
{
private:
	FMOD::Sound* m_pSound;
	UINT Len;

public:
	FMOD::Sound* Sound()
	{
		return m_pSound;
	}

public:
	bool Load(const wchar_t* _Path);

public:
	GameSound();
	~GameSound();
};

