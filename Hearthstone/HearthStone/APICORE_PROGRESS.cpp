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

	// backBMP ����
	m_backBMP = CreateCompatibleBitmap(m_HDC, m_WndSize.right, m_WndSize.bottom);

	// ����ۿ� backBMP ����
	SelectObject(m_backMemDC, m_backBMP);

	// ��� ���
	FillRect(m_backMemDC, &m_WndSize, (HBRUSH)GetStockObject(WHITE_BRUSH));

	// ������Ʈ ����
	m_pCurState->StateUpdate();
	m_pCurState->Update();
	m_pCurState->EndStateUpdate();

	// ���� ����
	BitBlt(m_HDC, 0, 0, m_WndSize.right, m_WndSize.bottom, m_backMemDC, 0, 0, SRCCOPY);
	
	// ���͵� ����
	m_pCurState->ReleaseActor();

	// backBMP �����
	DeleteObject(m_backBMP);

	// State ����
	if (m_pCurState->m_NextState != -1)
	{
		m_pCurState->EndState(); // ���͵� ����

		State* PrevState = m_pCurState; // ���� State

		m_pCurState = m_AllState[m_pCurState->m_NextState]; // ������Ʈ ����

		PrevState->m_NextState = -1; // �ٽ� -1�� ����

		m_pCurState->Start();
	}
}