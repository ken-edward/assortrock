#include "stdafx.h"
#include "APICORE.h"
#include "TimeMgr.h"
#include "InputMgr.h"
#include "PathMgr.h"
#include "RenderMgr.h"
#include "PlayState.h"
#include "MenuState.h"
#include "SelectState.h"
#include "DeckState.h"
#include "ResMgr.h"
#include "CardHeader.h"


#define menu STATE::MENU
#define play STATE::PLAY

void APICORE::Init()
{
	TimeMgr::Inst().Init();
	InputMgr::Inst().Init();
	RenderMgr::Inst().Init();

	ResMgr::Inst().Init();

	CreateState<PlayState>(STATE::PLAY);
	CreateState<MenuState>(STATE::MENU);
	CreateState<SelectState>(STATE::SELECT);
	CreateState<DeckState>(STATE::DECK);

	State::bgm = ResMgr::Inst().GetSoundPlayer();


	ResMgr::Inst().RootToCreatePath(L"Sound");

	//// ���ҽ� �߰�
	// MenuState
	ResMgr::Inst().LoadSound(L"Sound", L"Main_Theme.mp3");
	// DeckState
	ResMgr::Inst().LoadSound(L"Sound", L"Collection Manager.ogg");
	// PlayState
	ResMgr::Inst().LoadSound(L"Sound", L"Turn_Start.ogg"); // �� ���� �Ҹ�
	ResMgr::Inst().LoadSound(L"Sound", L"board_common_dirt_poke_1.ogg"); // �� ��� �Ҹ�
	ResMgr::Inst().LoadSound(L"Sound", L"board_common_dirt_poke_2.ogg");
	ResMgr::Inst().LoadSound(L"Sound", L"board_common_dirt_poke_3.ogg");
	ResMgr::Inst().LoadSound(L"Sound", L"board_common_dirt_poke_4.ogg");
	ResMgr::Inst().LoadSound(L"Sound", L"board_common_dirt_poke_5.ogg");
	ResMgr::Inst().LoadSound(L"Sound", L"board_common_dirt_poke_5.ogg");
	ResMgr::Inst().LoadSound(L"Sound", L"Don't Let Your Guard Down.ogg"); // ���� �� ���
	ResMgr::Inst().LoadSound(L"Sound", L"endturn.mp3"); // ���� �Ҹ�
	ResMgr::Inst().LoadSound(L"Sound", L"Card_Burn.ogg"); // ī�� Ÿ�� �Ҹ�
	ResMgr::Inst().LoadSound(L"Sound", L"hero_weapon_draw.ogg"); // ���� ���� �Ҹ�
	ResMgr::Inst().LoadSound(L"Sound", L"summon_sound.ogg");
	ResMgr::Inst().LoadSound(L"Sound", L"Arrow_Fired.ogg");
	ResMgr::Inst().LoadSound(L"Sound", L"Arrow_Impact.ogg");
	ResMgr::Inst().LoadSound(L"Sound", L"Arrow_Explode.ogg");

	ResMgr::Inst().LoadSound(L"Sound", L"Lunara_Attack.mp3");
	ResMgr::Inst().LoadSound(L"Sound", L"Lunara_Surrender.mp3");

	ResMgr::Inst().LoadSound(L"Sound", L"card_turn_over_legendary.ogg");
	ResMgr::Inst().LoadSound(L"Sound", L"legendary.mp3");

	ResMgr::Inst().LoadSound(L"Sound", L"Lunara_Hello.mp3");
	ResMgr::Inst().LoadSound(L"Sound", L"Lunara_WellPlayed.mp3");
	ResMgr::Inst().LoadSound(L"Sound", L"Lunara_ThankYou.mp3");
	ResMgr::Inst().LoadSound(L"Sound", L"Lunara_Wow.mp3");
	ResMgr::Inst().LoadSound(L"Sound", L"Lunara_Oops.mp3");
	ResMgr::Inst().LoadSound(L"Sound", L"Lunara_Threaten.mp3");

	ResMgr::Inst().LoadSound(L"Sound", L"Rexxar_Oops.mp3");

	ResMgr::Inst().LoadSound(L"Sound", L"Intro_Versus.mp3"); 
	ResMgr::Inst().LoadSound(L"Sound", L"Intro_Lunara.mp3");
	ResMgr::Inst().LoadSound(L"Sound", L"Intro_Rexxar.mp3");
	ResMgr::Inst().LoadSound(L"Sound", L"Intro_Hero_Rexxar.mp3");
	ResMgr::Inst().LoadSound(L"Sound", L"Intro_Hero_Lunara.mp3");

	ResMgr::Inst().LoadSound(L"Sound", L"Lunara_LackofMana.mp3"); // �糪�� ��������

	CARDSOUND

	ResMgr::Inst().LoadSound(L"Sound", L"card_hover.mp3"); // ī�� ��� �ִ� �Ҹ�
	ResMgr::Inst().LoadSound(L"Sound", L"3attack.mp3"); // ���� �Ҹ�
	ResMgr::Inst().LoadSound(L"Sound", L"6attack.mp3"); // 6��
	ResMgr::Inst().LoadSound(L"Sound", L"cancel_attack.ogg"); // ���� ĵ��

	ResMgr::Inst().LoadSound(L"Sound", L"Fatigue_DismissPoof_01.ogg"); // Ż�� �Ҹ�
	ResMgr::Inst().LoadSound(L"Sound", L"Fatigue_blade_shing.ogg"); // Ż�� Į�Ҹ�
	ResMgr::Inst().LoadSound(L"Sound", L"Fatigue_card_draw.ogg"); // Ż�� �̴� �Ҹ�

	




	if (STARTSTATE == 0)
	{
		m_pCurState = m_AllState[menu];
	}
	else if (STARTSTATE == 1)
	{
		m_pCurState = m_AllState[play];
	}
	m_pCurState->Start();
}