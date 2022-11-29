#include "stdafx.h"
#include "APICORE.h"
#include "TimeMgr.h"
#include "InputMgr.h"
#include "RenderMgr.h"
#include "PlayState.h"
#include "ResMgr.h"


void APICORE::StateUpdate()
{
	TimeMgr::Inst().Update();
	InputMgr::Inst().Update();
	GetClientRect(m_hWnd, &m_WndSize);
	RenderMgr::Inst().Update();
	ResMgr::Inst().SoundUpdate();

	// backBMP 생성
	m_backBMP = CreateCompatibleBitmap(m_HDC, m_WndSize.right, m_WndSize.bottom);

	// 백버퍼와 backBMP 연결
	SelectObject(m_backMemDC, m_backBMP);

	// 흰색 배경
	FillRect(m_backMemDC, &m_WndSize, (HBRUSH)GetStockObject(WHITE_BRUSH));

	// 스테이트 패턴
	m_pCurState->StateUpdate();
	m_pCurState->Update();
	m_pCurState->EndStateUpdate();

	// 버퍼 복사
	BitBlt(m_HDC, 0, 0, m_WndSize.right, m_WndSize.bottom, m_backMemDC, 0, 0, SRCCOPY);
	
	// 액터들 해제
	m_pCurState->ReleaseActor();

	// backBMP 지우기
	DeleteObject(m_backBMP);

	// State 변경
	if (m_pCurState->m_NextState != -1)
	{
		m_pCurState->EndState(); // 액터들 해제

		State* PrevState = m_pCurState; // 기존 State

		m_pCurState = m_AllState[m_pCurState->m_NextState]; // 스테이트 변경

		PrevState->m_NextState = -1; // 다시 -1로 변경

		m_pCurState->Start();
	}
}