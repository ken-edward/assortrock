#pragma once
class DXClass
{
private:
	DXClass();
	~DXClass();

public:
	static DXClass& Inst()
	{
		static DXClass Inst;
		return Inst;
	}

public:
	void Init(HWND, int, int);
	void Clear();
	void Present();
	void End();

private:
	HWND m_hwnd;
	int m_width = 0;
	int m_height = 0;

private:
	float m_screenNear = 0.1f;
	float m_screenDepth = 1000.0f;

private:
	int m_deviceflag = 2; // 0: ����� ����, 2: ����� ��� (������ enum)

private: // CreateDeviceAndSwapChain
	IDXGISwapChain* m_swapChain = nullptr;
	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_deviceContext = nullptr;

private: // CreateRenderTargetView
	ID3D11RenderTargetView* m_renderTargetView = nullptr;

private: // CreateDepthStencil Series
	ID3D11Texture2D* m_depthStencilBuffer = nullptr;
	ID3D11DepthStencilState* m_depthStencilState = nullptr;
	ID3D11DepthStencilState* m_depthDisabledStencilState = nullptr;
	ID3D11DepthStencilView* m_depthStencilView = nullptr;

private: // CreateRasterizerState
	ID3D11RasterizerState* m_rasterState = nullptr;

private: // CreateBlendState
	ID3D11BlendState* m_blendState = nullptr;

public:
	ID3D11Device* GetDevice() { return m_device; }
	ID3D11DeviceContext* GetDeviceContext() { return m_deviceContext; }
	HWND GetHWND() { return m_hwnd; }

private: // ���� ���
	XMMATRIX m_persMatrix; // ���� ����
	XMMATRIX m_orthMatrix; // ���� ����
	XMMATRIX m_projMatrix; // ���� ���

public:
	XMMATRIX GetProjectionMatrix() { return m_projMatrix; }

private: // Init
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void CreateDepthStencilBuffer();
	void CreateDepthStencilState();
	void CreateDepthStencilView();
	void CreateRasterizerState();
	void CreateBlendState();
	void SetViewPort();
	void SetProjectionMatrix();

public:
	void ZBufferOn();
	void ZBufferOff();
	void UpdateOrtho(float size);
	void RollBack();
};

