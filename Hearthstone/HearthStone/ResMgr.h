#pragma once
#include <map>
#include <string>
#include <list>
#include <set>
#include <fmod.hpp>

// dll�� �߰��� �ε��Ҷ� �׳� �ڱ� ������ �ִ� ������ ���θ� �ڵ����� �ε��ϰ�
// ���� �ٸ� ������ �ִٸ� ��θ� ��Ȯ�� ����� �ε尡 �ɰ��̴�.


#pragma comment(lib, "fmod_vc")

// ���ҽ���� ���� �ᱹ ��ΰ� ���� ���̴�.

class GameSound;
class SoundPlayer;
class GameImage;
class GameSprite;
class MultiSprite;
class ResMgr
{
public:
	static ResMgr& Inst()
	{
		static ResMgr Inst = ResMgr();
		return Inst;
	}

public:
	ResMgr();
	~ResMgr();
	///////////////////////////////////////// Path
private:
	//       //texture     c:\\game\bin\aaa.bmp
	std::map<std::wstring, std::wstring> m_PathMap;
	std::wstring m_Root; // �� ������ ����ǰ� �ִ� ������ ���

public:
	void Init();
	std::wstring FindPath(const wchar_t* _Key);

	bool RootToCreatePath(const wchar_t* _Key);
	bool RootToCreatePath(const wchar_t* _Key, const wchar_t* _NewFolder);

public:
	std::wstring RootPath()
	{
		return m_Root;
	}

	const wchar_t* RootPathPtr()
	{
		return m_Root.c_str();
	}


	////////////////////////////////////// Sound
	// ���� ����ü

private:
	// � �ܺ� ��ġ�� ���̺귯�� ���� ��κ�
	// ������� ���� com��ü ��� Ȥ�� ��ġ �������̽��� �����ͷ� �Ѱ��ִ� �����
	// ����ϴ� ��찡 ����. 
	FMOD::System* m_pSoundSystem;
	std::map<std::wstring, GameSound*> m_SoundMap;

	std::set<SoundPlayer*> m_RentalSound;

public:
	SoundPlayer* GetSoundPlayer();
	bool ReturnSoundPlayer(SoundPlayer* _SoundPlayer);

public:
	friend GameSound;
	friend SoundPlayer;

public:
	GameSound* LoadSound(const wchar_t* _FolderKey, const wchar_t* _SoundName);
	GameSound* FindSound(const wchar_t* _SoundName);

	void SoundUpdate();
	void PlaySound(const wchar_t* _SoundName);
};

