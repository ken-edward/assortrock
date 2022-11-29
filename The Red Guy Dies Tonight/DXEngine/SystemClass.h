#pragma once
class GameCore;
class SystemClass
{
private:
	SystemClass();
	~SystemClass();

public:
	static SystemClass& Inst()
	{
		static SystemClass Inst;
		return Inst;
	}

public:
	void Init(GameCore*);
	void Run();
	void End();
	bool corelive = true;

private:
	HINSTANCE m_hinst;
	HWND m_hwnd;
	LPCWSTR m_appName;
	int m_width = 0;
	int m_height = 0;

private: // Init
	void InitializeWindow();

private: // Run
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	void Frame();

private: // End
	void ReleaseWindow();
};

