#pragma once
#define RENDINST RenderMgr::Inst()
class RenderMgr
{
public: // 싱글톤
	static RenderMgr& Inst()
	{
		static RenderMgr RenderInst = RenderMgr();
		return RenderInst;
	}

private:
	HINSTANCE m_hInst;
	HDC m_backMemDC;
	HBITMAP m_backBMP;
	HWND m_hWnd;
	RECT m_WndSize;

public:
	void Init();
	void Update();

public:
	void ShowImage(int posx, int posy, wchar_t* ImageName,
		int divx = 1, int divy = 1, int srcx = 0, int srcy = 0); // 애니메이션 전용 인자
	void ShowImageByScale(int posx, int posy, wchar_t* ImageName, float scale = 1.0f,
		int divx = 1, int divy = 1, int srcx = 0, int srcy = 0);
	void AlphaImageByScale(int posx, int posy, wchar_t* ImageName, int alpha = 255, float scale = 1.0f,
		int divx = 1, int divy = 1, int srcx = 0, int srcy = 0);
	void ShowRect(int left, int top, int right, int bottom);
	void ShowText(int x, int y, const wchar_t* Text);
	// 사용 패턴
	/*
		wchar_t str[100]{};
		swprintf_s(str, L": %d", );
		RENDINST.ShowText(, , str);
	*/
	void ShowNumber(int x, int y, int number = 0, int type = 0); // type: 0 일반, 1 부상, 2 버프
	void ShowDamage(int x, int y, int dmg);

public: // float 버전. 그냥 경고 해결용.
	void ShowNumber(float x, float y, int number = 0, int type = 0)
	{
		ShowNumber((int)x, (int)y, number, type);
	}
	void ShowImage(float posx, float posy, wchar_t* ImageName,
		int divx = 1, int divy = 1, int srcx = 0, int srcy = 0)
	{
		ShowImage((int)posx, (int)posy, ImageName,
			divx, divy, srcx, srcy);
	}
	void ShowRect(float left, float top, float right, float bottom)
	{
		ShowRect((int)left, (int)top, (int)right, (int)bottom);
	}
	void ShowText(float x, float y, const wchar_t* Text)
	{
		ShowText((int)x, (int)y, Text);
	}

public:
	RenderMgr();
	~RenderMgr();
};

