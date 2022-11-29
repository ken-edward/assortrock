#include "stdafx.h"
#include "ResMgr.h"
#include "GameSound.h"
#include "SoundPlayer.h"

ResMgr::ResMgr() : m_pSoundSystem(nullptr)
{
}


ResMgr::~ResMgr()
{
	
	std::map<std::wstring, GameSound*>::iterator StartSoundSpIter = m_SoundMap.begin();
	std::map<std::wstring, GameSound*>::iterator EndSoundSpIter = m_SoundMap.end();

	for (; StartSoundSpIter != EndSoundSpIter; ++StartSoundSpIter)
	{
		if (nullptr != StartSoundSpIter->second)
		{
			delete StartSoundSpIter->second;
		}
	}

	std::set<SoundPlayer*>::iterator StartSPIter = m_RentalSound.begin();
	std::set<SoundPlayer*>::iterator EndSPIter = m_RentalSound.end();

	for (; StartSPIter != EndSPIter; StartSPIter++)
	{
		delete *StartSPIter;
	}

	if (nullptr != m_pSoundSystem)
	{
		m_pSoundSystem->release();
	}
}

void ResMgr::Init() {

	wchar_t ArrStr[256] = {};
	GetCurrentDirectoryW(sizeof(wchar_t) * 256, ArrStr);
	m_Root = ArrStr;

	// 어떤 문자열의 특정 문자를 뒤에서 부터 찾아서
	// 그게 나오면 그 위치를 리턴한다.

	// 
	// 012345678910
	// \\SSSD\\SSSS\\DBD
	size_t CutCount = m_Root.find_last_of(L'\\', m_Root.size());
	m_Root = m_Root.replace(CutCount, m_Root.size(), L"\\Bin\\");


	// 시스템을 얻어서 주겠다.
	FMOD::System_Create(&m_pSoundSystem);
	TASSERT(nullptr == m_pSoundSystem);

	if (nullptr == m_pSoundSystem)
	{
		return;
	}

	// 최대 동시에 1000개를 재생할수 있다.
	FMOD_RESULT Return = m_pSoundSystem->init(32, FMOD_DEFAULT, nullptr);

	TASSERT(FMOD_OK != Return);

	// 시스템에게 준비를 시켜야 한다.

}

std::wstring ResMgr::FindPath(const wchar_t* _Key)
{
	std::map<std::wstring, std::wstring>::iterator FindIter = m_PathMap.find(_Key);

	if (FindIter != m_PathMap.end())
	{
		return FindIter->second;
	}

	return L"";
}

bool ResMgr::RootToCreatePath(const wchar_t* _Key)
{
	return RootToCreatePath(_Key, _Key);
}

bool ResMgr::RootToCreatePath(const wchar_t* _Key, const  wchar_t* _NewFolder)
{
	std::map<std::wstring, std::wstring>::iterator FindIter = m_PathMap.find(_Key);

	if (FindIter != m_PathMap.end())
	{
		TASSERT(true);
		return false;
	}

	m_PathMap.insert(std::map<std::wstring, std::wstring>::value_type(_Key, m_Root + _NewFolder + L"\\"));

	return true;
}

GameSound* ResMgr::LoadSound(const wchar_t* _FolderKey, const wchar_t* _SoundName) {

	GameSound* pSound = FindSound(_SoundName);

	if (nullptr != pSound)
	{
		return pSound;
	}

	pSound = new GameSound();

	std::wstring Path = FindPath(_FolderKey);
	Path += _SoundName;

	if (false == pSound->Load(Path.c_str()))
	{
		TASSERT(true);
		delete pSound;
	}

	m_SoundMap.insert(std::map<std::wstring, GameSound*>::value_type(_SoundName, pSound));

	// m_pSoundSystem->playSound(pSound->Sound(), nullptr, false, nullptr);
	return pSound;
}

void ResMgr::SoundUpdate() {
	m_pSoundSystem->update();
}

GameSound* ResMgr::FindSound(const wchar_t* _SoundName) 
{

	std::map<std::wstring, GameSound*>::iterator FindIter = m_SoundMap.find(_SoundName);

	if (FindIter == m_SoundMap.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

void ResMgr::PlaySound(const wchar_t* _SoundName)
{
	GameSound* pSound = FindSound(_SoundName);

	TASSERT(nullptr == pSound);

	if (nullptr == pSound)
	{
		return;
	}

	m_pSoundSystem->playSound(pSound->Sound(), nullptr, false, nullptr);

}



SoundPlayer* ResMgr::GetSoundPlayer()
{
	SoundPlayer* NewPlayer = new SoundPlayer();
	m_RentalSound.insert(NewPlayer);
	return NewPlayer;
}

bool ResMgr::ReturnSoundPlayer(SoundPlayer* _SoundPlayer)
{
	std::set<SoundPlayer*>::iterator FindIter = m_RentalSound.find(_SoundPlayer);

	if (m_RentalSound.end() == FindIter)
	{
		return false;
	}

	delete *FindIter;
	m_RentalSound.erase(FindIter);

	return true;
}