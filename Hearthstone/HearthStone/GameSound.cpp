#include "stdafx.h"
#include "GameSound.h"
#include "ResMgr.h"

// atl�� ������ ���� ���̺귯���� �ϳ��μ� ���ڿ� ���� �Լ����� �����Ѵ�.
// stl�� ���õ� �����Լ����̴�.
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
	// ���̵� ����Ʈ�� ��Ƽ ����Ʈ�� �����ϴ� ����� ���� ����.
	// WideCharToMultiByte()
	// std::wstring WPath = _Path;

	// �˻��غ��ø� �ڼ��� ������ �� �˼� ���� �̴ϴ�.
	// CW2A �Լ���ü
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