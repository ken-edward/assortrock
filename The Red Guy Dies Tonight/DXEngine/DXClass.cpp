#include "stdafx.h"
#include "DXClass.h"
#include "TexShader.h"
#include "Cam.h"


DXClass::DXClass()
{
}


DXClass::~DXClass()
{
}


void DXClass::Init(HWND hwnd, int width, int height)
{
	m_hwnd = hwnd;
	m_width = width;
	m_height = height;

	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	CreateDepthStencilBuffer();
	CreateDepthStencilState();
	CreateDepthStencilView();
	CreateRasterizerState();
	CreateBlendState();
	SetViewPort();
	SetProjectionMatrix();

	TexShader::Inst().Initialize(m_device, m_hwnd);
}


void DXClass::Clear()
{
	// 버퍼를 지울 색을 설정합니다
	float grey = 0.5f;
	float color[4] = { grey, grey, grey, 1.0f };

	// 백버퍼를 지웁니다
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	// 깊이 버퍼를 지웁니다
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

}


void DXClass::Present()
{
	m_swapChain->Present(0, 0);
}


void DXClass::End()
{	
	TexShader::Inst().Shutdown();

	if (m_blendState)
	{
		m_blendState->Release();
		m_blendState = 0;
	}

	if (m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = 0;
	}

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}

	if (m_depthDisabledStencilState)
	{
		m_depthDisabledStencilState->Release();
		m_depthDisabledStencilState = 0;
	}

	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = 0;
	}

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}
	
	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = 0;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = 0;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = 0;
	}
}


void DXClass::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;

	swapChainDesc.BufferDesc.Width = m_width;
	swapChainDesc.BufferDesc.Height = m_height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	swapChainDesc.Flags = 0;
	
	swapChainDesc.OutputWindow = m_hwnd;

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapChainDesc.Windowed = true;


	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	D3D11CreateDeviceAndSwapChain(
		NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 
		m_deviceflag,
		&featureLevel, 1, D3D11_SDK_VERSION,
		&swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext
	);
}


void DXClass::CreateRenderTargetView()
{
	// 백버퍼 포인터를 얻어옵니다
	ID3D11Texture2D* backBufferPtr = nullptr;
	m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);

	// 백 버퍼 포인터로 렌더타겟뷰를 생성한다.
	m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);

	// 백버퍼 포인터를 해제합니다
	backBufferPtr->Release();
	backBufferPtr = 0;
}


void DXClass::CreateDepthStencilBuffer()
{
	// 깊이 버퍼 구조체를 초기화합니다
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// 깊이 버퍼 구조체를 작성합니다
	depthBufferDesc.Width = m_width;
	depthBufferDesc.Height = m_height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// 설정된 깊이버퍼 구조체를 사용하여 깊이 버퍼 텍스쳐를 생성합니다
	m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
}


void DXClass::CreateDepthStencilState()
{
	// 스텐실 상태 구조체를 초기화합니다.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// 스텐실 상태 구조체를 작성합니다.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// 픽셀 정면의 스텐실 설정입니다.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// 픽셀 뒷면의 스텐실 설정입니다
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// 깊이 스텐실 상태를 생성합니다.
	m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	
	// 2D 렌더링을위한 Z 버퍼를 끄는 두 번째 깊이 스텐실 상태를 만듭니다.
	// 유일한 차이점은 DepthEnable이 false라는 점입니다.
	depthStencilDesc.DepthEnable = false;

	// 장치를 사용하여 상태를 만듭니다.
	m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthDisabledStencilState);

	// 깊이 스텐실 상태를 설정합니다.
	m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
}


void DXClass::CreateDepthStencilView()
{
	// 깊이 스텐실 뷰의 구조체를 초기화합니다.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// 깊이 스텐실 뷰 구조체를 설정합니다.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// 깊이 스텐실 뷰를 생성합니다.
	m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);

	// 렌더링 대상 뷰와 깊이 스텐실 버퍼를 출력 렌더 파이프 라인에 바인딩합니다.
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
}


void DXClass::CreateRasterizerState()
{
	// 그려지는 폴리곤과 방법을 결정할 래스터 구조체를 설정합니다
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// 방금 작성한 구조체에서 래스터 라이저 상태를 만듭니다
	m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);

	// 이제 래스터 라이저 상태를 설정합니다
	m_deviceContext->RSSetState(m_rasterState);
}


void DXClass::CreateBlendState()
{
	D3D11_BLEND_DESC blendDesc;

	blendDesc.AlphaToCoverageEnable = false;
	// false라고 한순간 블랜드 옵션을 독립적으로 쓰지 않겠다고 한거다.

	blendDesc.IndependentBlendEnable = false;

	blendDesc.RenderTarget[0].BlendEnable = true;

	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

	m_device->CreateBlendState(&blendDesc, &m_blendState);

	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	m_deviceContext->OMSetBlendState(m_blendState, color, 0xffffffff);

	
}


void DXClass::SetViewPort()
{
	// 렌더링을 위해 뷰포트를 설정합니다
	D3D11_VIEWPORT viewport;
	viewport.Width = (float)m_width;
	viewport.Height = (float)m_height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// 뷰포트를 생성합니다
	m_deviceContext->RSSetViewports(1, &viewport);
}


void DXClass::SetProjectionMatrix()
{
	// 투영 행렬을 설정합니다.
	float fieldOfView = XM_PI / 4.0f; // PI / 4, 45도와 동일.
	float screenAspect = (float)m_width / (float)m_height;

	// 3D 렌더링을 위한 원근 투영 행렬을 만듭니다.
	m_persMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, m_screenNear, m_screenDepth);

	// 2D 렌더링을 위한 직교 투영 행렬을 만듭니다.
	m_orthMatrix = XMMatrixOrthographicLH((float)m_width, (float)m_height, m_screenNear, m_screenDepth);

	// 기본 투영 방식을 설정합니다.
	m_projMatrix = m_orthMatrix;
}


void DXClass::ZBufferOn()
{
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
}


void DXClass::ZBufferOff()
{
	m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
}

void DXClass::UpdateOrtho(float size)
{
	float tempwidth = m_width * size;
	float tempheight = m_height * size;

	m_orthMatrix = XMMatrixOrthographicLH(tempwidth, tempheight, m_screenNear, m_screenDepth);

	m_projMatrix = m_orthMatrix;
}

void DXClass::RollBack()
{
	//DXClass::Inst().GetDeviceContext()->VSSetShader(nullptr, nullptr, 0);
	//DXClass::Inst().GetDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	//DXClass::Inst().GetDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	//DXClass::Inst().GetDeviceContext()->CSSetShader(nullptr, nullptr, 0);
	//DXClass::Inst().GetDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	//DXClass::Inst().GetDeviceContext()->PSSetShader(nullptr, nullptr, 0);
	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_deviceContext->OMSetBlendState(m_blendState, color, 0xffffffff);
	m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
}
