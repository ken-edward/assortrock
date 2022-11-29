#include "stdafx.h"
#include "RenderMgr.h"
#include "APICORE.h"
#include "PathMgr.h"
#include <math.h>
#pragma comment(lib, "msimg32.lib")


RenderMgr::RenderMgr()
{
}


RenderMgr::~RenderMgr()
{
}


void RenderMgr::Init()
{
	m_hInst = APICORE::Inst().HINST();
	m_backMemDC = APICORE::Inst().BackDC();
	m_backBMP = APICORE::Inst().BackBMP();
	m_hWnd = APICORE::Inst().MHWND();
}


void RenderMgr::Update()
{
	m_WndSize = APICORE::Inst().WNDSIZE();
}


void RenderMgr::ShowImage(int posx, int posy, wchar_t* ImageName,
	int divx, int divy, int srcx, int srcy)
{
	HDC tempDC;
	HBITMAP tempBMP;

	tempDC = CreateCompatibleDC(m_backMemDC);

	tempBMP = (HBITMAP)LoadImageW(
		m_hInst,
		PathMgr::Inst().FindImage(ImageName),
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE
	);

	SelectObject(tempDC, tempBMP);

	BITMAP bmp_info;
	GetObject(tempBMP, sizeof(BITMAP), &bmp_info);

	int pieceX = bmp_info.bmWidth / divx;
	int pieceY = bmp_info.bmHeight / divy;

	TransparentBlt(
		m_backMemDC, // 출력할 DC입니다.
		posx,
		posy,		// 출력할 위치입니다.
		pieceX,
		pieceY,		// 출력할 크기입니다.
		tempDC, // 가져올 DC입니다.
		srcx * pieceX,
		srcy * pieceY,			// 가져올 지점입니다.
		pieceX,
		pieceY,		// 잘라낼 크기입니다.
		RGB(255, 0, 255) // 투명화할, 요컨대 출력하지 않을 픽셀입니다.
	);

	DeleteDC(tempDC);
	DeleteObject(tempBMP);
}


void RenderMgr::ShowImageByScale(int posx, int posy, wchar_t* ImageName, float scale,
	int divx, int divy, int srcx, int srcy)
{
	HDC tempDC;
	HBITMAP tempBMP;

	tempDC = CreateCompatibleDC(m_backMemDC);

	tempBMP = (HBITMAP)LoadImageW(
		m_hInst,
		PathMgr::Inst().FindImage(ImageName),
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE
	);

	SelectObject(tempDC, tempBMP);

	BITMAP bmp_info;
	GetObject(tempBMP, sizeof(BITMAP), &bmp_info);

	int pieceX = bmp_info.bmWidth / divx;
	int pieceY = bmp_info.bmHeight / divy;

	TransparentBlt(
		m_backMemDC, // 출력할 DC입니다.
		posx,
		posy,		// 출력할 위치입니다.
		pieceX * scale,
		pieceY * scale,		// 출력할 크기입니다.
		tempDC, // 가져올 DC입니다.
		srcx * pieceX,
		srcy * pieceY,			// 가져올 지점입니다.
		pieceX,
		pieceY,		// 잘라낼 크기입니다.
		RGB(255, 0, 255) // 투명화할, 요컨대 출력하지 않을 픽셀입니다.
	);

	DeleteDC(tempDC);
	DeleteObject(tempBMP);
}



void RenderMgr::AlphaImageByScale(int posx, int posy, wchar_t* ImageName, int alpha, float scale,
	int divx, int divy , int srcx , int srcy)
{
	
}



void RenderMgr::ShowRect(int left, int top, int right, int bottom)
{
	Rectangle(m_backMemDC, left, top, right, bottom);
}


void RenderMgr::ShowText(int x, int y, const wchar_t* Text)
{
	TextOutW(m_backMemDC, x, y, Text, lstrlenW(Text));
}


void RenderMgr::ShowNumber(int x, int y, int number, int type)
{
#define NUMWIDTH 14

	// 숫자 길이
	char str[10] = {};
	sprintf_s(str, "%d", number);
	int NumLen = (int)strlen(str); // 음수일 경우 마이너스 포함

	// 시작 지점 x 설정
	int StartPointX = x - 2; // 이미지 때문에 2픽셀 왼쪽
	int HalfLen = NumLen / 2;
	if (NumLen % 2 == 0) // 짝수일 경우
	{
		StartPointX -= HalfLen * NUMWIDTH;
	}
	else if(NumLen % 2 == 1) // 홀수일 경우
	{
		StartPointX -= HalfLen * NUMWIDTH;
		StartPointX -= NUMWIDTH / 2;
	}

	// 체력이 음수일 경우 빨간 - 출력
	if (number < 0)
	{
		if (type == 1)
		{
			RENDINST.ShowImage(
				StartPointX,
				y - 10,
				L"minus.bmp"
			);
		}
		else // 이건 대미지 표시용 하얀 마이너스
		{
			RENDINST.ShowImage(
				StartPointX,
				y - 10,
				L"minus_white.bmp"
			);
		}
	}

	// 숫자 출력
	int i = 0;
	if (number < 0) // 음수일 경우 하나 출력한 셈 치고 시작
	{
		i = 1;
	}
	for (; i < NumLen; i++)
	{
		RENDINST.ShowImage(
			StartPointX + i * NUMWIDTH,
			y - 10,
			L"Numbers.bmp",
			10,
			3,
			str[i] - 48,
			type
		);
	}
}


void RenderMgr::ShowDamage(int x, int y, int dmg)
{
	float scale = 0.8f;
	RENDINST.ShowImageByScale(x - 60 * scale, y - 60 * scale, L"damaged.bmp", scale);
	RENDINST.ShowNumber(x, y, dmg * -1);
}


