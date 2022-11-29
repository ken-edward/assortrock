#pragma once
#include <fmod.hpp>
#ifdef _WIN64
#pragma comment(lib, "fmod64_vc")
#else // x86
#pragma comment(lib, "fmod_vc")
#endif

#include <unordered_map>

class SoundData
{
private:
	friend class SoundClass;

private:
	FMOD::Sound* sound;
	FMOD::Channel* channel;

private:
	SoundData() {}
	~SoundData()
	{
		if (sound != nullptr)
		{
			sound->release();
		}
	}
};

class SoundClass
{
private:
	SoundClass();
	~SoundClass();

public:
	static SoundClass& Inst()
	{
		static SoundClass Inst;
		return Inst;
	}

private:
	FMOD::System* soundSystem;

private:
	std::unordered_map<std::wstring, SoundData*> soundMap;

public:
	void Init();
	void Update();

public:
	void Add(std::wstring name, std::string path);
	void Play(std::wstring);
	void NewPlay(std::wstring);
	void Stop(std::wstring);
	bool IsPlaying(std::wstring);
};