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
		m_backMemDC, // ����� DC�Դϴ�.
		posx,
		posy,		// ����� ��ġ�Դϴ�.
		pieceX,
		pieceY,		// ����� ũ���Դϴ�.
		tempDC, // ������ DC�Դϴ�.
		srcx * pieceX,
		srcy * pieceY,			// ������ �����Դϴ�.
		pieceX,
		pieceY,		// �߶� ũ���Դϴ�.
		RGB(255, 0, 255) // ����ȭ��, ������ ������� ���� �ȼ��Դϴ�.
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
		m_backMemDC, // ����� DC�Դϴ�.
		posx,
		posy,		// ����� ��ġ�Դϴ�.
		pieceX * scale,
		pieceY * scale,		// ����� ũ���Դϴ�.
		tempDC, // ������ DC�Դϴ�.
		srcx * pieceX,
		srcy * pieceY,			// ������ �����Դϴ�.
		pieceX,
		pieceY,		// �߶� ũ���Դϴ�.
		RGB(255, 0, 255) // ����ȭ��, ������ ������� ���� �ȼ��Դϴ�.
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

	// ���� ����
	char str[10] = {};
	sprintf_s(str, "%d", number);
	int NumLen = (int)strlen(str); // ������ ��� ���̳ʽ� ����

	// ���� ���� x ����
	int StartPointX = x - 2; // �̹��� ������ 2�ȼ� ����
	int HalfLen = NumLen / 2;
	if (NumLen % 2 == 0) // ¦���� ���
	{
		StartPointX -= HalfLen * NUMWIDTH;
	}
	else if(NumLen % 2 == 1) // Ȧ���� ���
	{
		StartPointX -= HalfLen * NUMWIDTH;
		StartPointX -= NUMWIDTH / 2;
	}

	// ü���� ������ ��� ���� - ���
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
		else // �̰� ����� ǥ�ÿ� �Ͼ� ���̳ʽ�
		{
			RENDINST.ShowImage(
				StartPointX,
				y - 10,
				L"minus_white.bmp"
			);
		}
	}

	// ���� ���
	int i = 0;
	if (number < 0) // ������ ��� �ϳ� ����� �� ġ�� ����
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


