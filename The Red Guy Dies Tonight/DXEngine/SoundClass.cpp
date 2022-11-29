#include "stdafx.h"
#include "SoundClass.h"


SoundClass::SoundClass()
{

}


SoundClass::~SoundClass()
{
	auto iter = soundMap.begin();
	auto end = soundMap.end();

	for (; iter != end; iter++)
	{
		delete iter->second;
	}

	soundMap.clear();

	if (soundSystem != nullptr)
	{
		soundSystem->release();
	}
}


void SoundClass::Init()
{
	FMOD::System_Create(&soundSystem);
	soundSystem->init(32, FMOD_DEFAULT, nullptr);

	Add(L"silenced", "../sound/silenced.mp3");
	Add(L"gunsound", "../sound/gunsound.mp3");
	Add(L"glasshit", "../sound/glasshit.mp3");
	Add(L"glassbreak", "../sound/glassbreak.mp3");
	Add(L"emptygun", "../sound/emptygun.mp3");
	Add(L"pistol", "../sound/pistol.mp3");
	Add(L"siren", "../sound/siren.mp3");
	Add(L"footstep", "../sound/footstep.mp3");
	Add(L"round gunpoint", "../sound/round gunpoint.mp3");
}


void SoundClass::Update()
{
	soundSystem->update();
}


void SoundClass::Add(std::wstring name, std::string path)
{
	SoundData* newData = new SoundData;

	soundSystem->createSound(
		path.c_str(), FMOD_DEFAULT, NULL, &newData->sound);
	
	soundMap.insert(pair<wstring, SoundData*>(name, newData));
}


void SoundClass::Play(std::wstring name)
{
	SoundData* sound = soundMap.find(name)->second;

	soundSystem->playSound(sound->sound, NULL, FALSE, &sound->channel);
}


void SoundClass::Stop(std::wstring name)
{
	SoundData* sound = soundMap.find(name)->second;

	if (sound->channel != nullptr)
	{
		sound->channel->stop();
	}
}


bool SoundClass::IsPlaying(std::wstring name)
{
	SoundData* sound = soundMap.find(name)->second;

	if (sound->channel != nullptr)
	{
		bool Return;
		sound->channel->isPlaying(&Return);
		return Return;
	}
	else
	{
		return false;
	}
}



void SoundClass::NewPlay(std::wstring name)
{
	SoundData* sound = soundMap.find(name)->second;
	
	if (sound->channel != nullptr)
	{
		sound->channel->stop();
	}

	soundSystem->playSound(sound->sound, NULL, FALSE, &sound->channel);
}