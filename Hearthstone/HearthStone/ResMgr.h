#pragma once
#include <map>
#include <string>
#include <list>
#include <set>
#include <fmod.hpp>

// dll을 추가로 로드할때 그냥 자기 파일이 있는 곳에만 놔두면 자동으로 로드하고
// 만약 다른 폴더에 있다면 경로를 명확히 해줘야 로드가 될것이다.


#pragma comment(lib, "fmod_vc")

// 리소스라는 것은 결국 경로가 있을 것이다.

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
	std::wstring m_Root; // 이 게임이 실행되고 있는 폴더의 경로

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
	// 사운드 그자체

private:
	// 어떤 외부 장치를 라이브러리 들은 대부분
	// 윈도우와 같이 com객체 방식 혹은 장치 인터페이스를 포인터로 넘겨주는 방식을
	// 사용하는 경우가 많다. 
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

