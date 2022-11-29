#pragma once
#include <fmod.hpp>

class GameSound;
class ResMgr;
class SoundPlayer
{
private:
	friend ResMgr;

public:
	GameSound* m_pSound;
	FMOD::Channel* m_pChannel;
	float m_Pitch;

public:
	float Pitch();
	void Pitch(float _Value);
	bool IsPlay();

	void NewSoundPlay(const wchar_t* _FileName);
	void Stop();

public:
	SoundPlayer();
private:
	~SoundPlayer();
};

