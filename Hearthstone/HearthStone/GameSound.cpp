#include "stdafx.h"
#include "GameSound.h"
#include "ResMgr.h"

// atl은 윈도우 지원 라이브러리중 하나로서 문자열 편의 함수들을 제공한다.
// stl과 관련된 편의함수들이다.
#include <atlstr.h>


GameSound::GameSound() : Len(0)
{
}


GameSound::~GameSound()
{
	m_pSound->release();
}

bool GameSound::Load(const wchar_t* _Path)
{
	// 와이드 바이트를 멀티 바이트로 변경하는 방법은 정말 많다.
	// WideCharToMultiByte()
	// std::wstring WPath = _Path;

	// 검색해보시면 자세한 내용을 더 알수 있을 겁니다.
	// CW2A 함수객체
	std::string MPath = CW2A(_Path);

	if (FMOD_OK != ResMgr::Inst().m_pSoundSystem->createSound(MPath.c_str(), FMOD_DEFAULT, nullptr, &m_pSound))
	{
		TASSERT(true);
		return false;
	}

	TASSERT(nullptr == m_pSound);

	if (nullptr == m_pSound)
	{
		return false;
	}

	m_pSound->getLength(&Len, FMOD_TIMEUNIT_MS);

	return true;
}