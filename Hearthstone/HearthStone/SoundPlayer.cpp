#include "stdafx.h"
#include "SoundPlayer.h"
#include "ResMgr.h"
#include "GameSound.h"


SoundPlayer::SoundPlayer() : m_pChannel(nullptr)
{
}


SoundPlayer::~SoundPlayer()
{
	
}

bool SoundPlayer::IsPlay()
{
	bool IsPlaying;
	m_pChannel->isPlaying(&IsPlaying);
	return IsPlaying;
}

float SoundPlayer::Pitch()
{
	m_pChannel->getPitch(&m_Pitch);
	return m_Pitch;
}

void SoundPlayer::Pitch(float _Value)
{
	m_Pitch = _Value;
	m_pChannel->setPitch(m_Pitch);
	return;
}

void SoundPlayer::NewSoundPlay(const wchar_t* _FileName)
{
	if (nullptr != m_pChannel)
	{
		m_pChannel->stop();
	}

	GameSound* pSound = ResMgr::Inst().FindSound(_FileName);

	TASSERT(nullptr == pSound);

	if (nullptr == pSound)
	{
		return;
	}

	ResMgr::Inst().m_pSoundSystem->playSound(pSound->Sound(), nullptr, false, &m_pChannel);
	Pitch();
}


void SoundPlayer::Stop()
{
	if (nullptr != m_pChannel)
	{
		m_pChannel->stop();
	}
}