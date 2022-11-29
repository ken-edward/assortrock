#include "stdafx.h"
#include "Tex.h"
#include "DXClass.h"
#include "TexShader.h"
#include "TexCom.h"
#include "RenderMgr.h"


TexCom::TexCom()
{
}

TexCom::TexCom(wstring texname)
{
	Init((wchar_t*)texname.c_str());
}


TexCom::~TexCom()
{
	Shutdown();
}


void TexCom::Render()
{
	if (display == false)
	{
		return;
	}

	if (bChange == true)
	// 버텍스를 바꾸는 모든 public 함수에서 이를 켜줘야 합니다.
	// 종류: pos, scale, rotation에 변화를 주는 것들.
	{
		bChange = false;

		// 위치, 크기, 회전을 기반으로 버텍스를 다시 세팅해야 합니다.
		// 순서는 크기->회전->위치 순으로 하는 게 좋습니다.

		// 사실 회전 먼저 해도 되는데 위치만은 가장 나중에 해야 합니다.

		// 버텍스를 초기화합니다.
		// 원래 이미지 크기에 따라 버텍스가 바뀝니다.
		ResetVertex();

		// scale에 따라 버텍스의 거리를 조절합니다.
		v0 *= scale;
		v1 *= scale;
		v2 *= scale;
		v3 *= scale;

		// 버텍스를 회전시킵니다.
		v0.rotate(rotation);
		v1.rotate(rotation);
		v2.rotate(rotation);
		v3.rotate(rotation);
		
		// 버텍스를 이동시킵니다.
		v0 += pos + pivot;
		v1 += pos + pivot;
		v2 += pos + pivot;
		v3 += pos + pivot;

		// 버텍스에 따라 버퍼를 업데이트합니다.
		UpdateBuffers();
	}
	
	RenderBuffers();
	TexShader::Inst().Render(GetTexture());
}


void TexCom::Update5()
{
	RenderMgr::Inst().Queue(this, z);
}


void TexCom::SetScale(float value) // 크기 설정
{
	bChange = true;

	scale = value;
}

void TexCom::SetRotation(float value) // 회전 설정
{
	bChange = true;

	rotation = value;
}

void TexCom::Move(VEC2 val) // 위치 변화
{
	bChange = true;

	pos += val;
}

void TexCom::Enlarge(float value) // 크기 변화
{
	bChange = true;

	scale *= value;
}

void TexCom::Rotate(float value) // 회전 변화
{
	bChange = true;

	rotation += value;
}

void TexCom::ResetVertex()
{
	v0.x = -size.x;
	v0.y = size.y;

	v1.x = size.x;
	v1.y = size.y;

	v2.x = size.x;
	v2.y = -size.y;

	v3.x = -size.x;
	v3.y = -size.y;
}


bool TexCom::Init(wchar_t* textureFilename)
{
	fromLeftTop = false;

	// 화면 크기를 멤버변수에 저장
	m_screenWidth = 0;
	m_screenHeight = 0;

	// 이전 렌더링 위치를 음수로 초기화합니다.
	m_previousPosX = -1;
	m_previousPosY = -1;

	// 정점 및 인덱스 버퍼를 초기화합니다.
	if (!InitializeBuffers(DXClass::Inst().GetDevice()))
	{
		return false;
	}

	// 이 모델의 텍스처를 로드합니다.
	LoadTexture(textureFilename);

	m_bitmapWidth = (int)m_Texture->GetWidth();
	m_bitmapHeight = (int)m_Texture->GetHeight();

	size.x = (float)(m_bitmapWidth / 2);
	size.y = (float)(m_bitmapHeight / 2);

	ResetVertex();

	return true;
}




bool TexCom::Initialize(int screenWidth, int screenHeight, WCHAR* textureFilename, int bitmapWidth, int bitmapHeight)
{
	fromLeftTop = true;

	// 화면 크기를 멤버변수에 저장
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// 렌더링할 비트맵의 픽셀의 크기를 저장
	m_bitmapWidth = bitmapWidth;
	m_bitmapHeight = bitmapHeight;

	// 이전 렌더링 위치를 음수로 초기화합니다.
	m_previousPosX = -1;
	m_previousPosY = -1;

	// 정점 및 인덱스 버퍼를 초기화합니다.
	if (!InitializeBuffers(DXClass::Inst().GetDevice()))
	{
		return false;
	}

	// 이 모델의 텍스처를 로드합니다.
	return LoadTexture(textureFilename);
}


void TexCom::Shutdown()
{
	// 모델 텍스쳐를 반환합니다.
	ReleaseTexture();

	// 버텍스 및 인덱스 버퍼를 종료합니다.
	ShutdownBuffers();
}




bool TexCom::Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	// 화면의 다른 위치로 렌더링하기 위해 동적 정점 버퍼를 다시 빌드합니다.
	if (!UpdateBuffers(deviceContext, positionX, positionY))
	{
		return false;
	}

	// 그리기를 준비하기 위해 그래픽 파이프 라인에 꼭지점과 인덱스 버퍼를 놓습니다.
	RenderBuffers();

	return true;
}


int TexCom::GetIndexCount()
{
	return m_indexCount;
}


ID3D11ShaderResourceView* TexCom::GetTexture()
{
	return m_Texture->GetTexture();
}


bool TexCom::InitializeBuffers(ID3D11Device* device)
{
	// 정점 배열의 정점 수와 인덱스 배열의 인덱스 수를 지정합니다.
	m_indexCount = m_vertexCount = 6;

	// 정점 배열을 만듭니다.
	VertexType* vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// 정점 배열을 0으로 초기화합니다.
	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	// 인덱스 배열을 만듭니다.
	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// 데이터로 인덱스 배열을 로드합니다.
	for (int i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}

	// 정적 정점 버퍼의 구조체를 설정합니다.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource 구조에 정점 데이터에 대한 포인터를 제공합니다.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// 이제 정점 버퍼를 만듭니다.
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		return false;
	}

	// 정적 인덱스 버퍼의 구조체를 설정합니다.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 인덱스 데이터를 가리키는 보조 리소스 구조체를 작성합니다.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// 인덱스 버퍼를 생성합니다.
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	{
		return false;
	}

	// 생성되고 값이 할당된 정점 버퍼와 인덱스 버퍼를 해제합니다.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


void TexCom::ShutdownBuffers()
{
	// 인덱스 버퍼를 해제합니다.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// 정점 버퍼를 해제합니다.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}


bool TexCom::UpdateBuffers()
{
	ID3D11DeviceContext* deviceContext = DXClass::Inst().GetDeviceContext();

	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;

	// 정점 배열을 만듭니다.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	if (flip == false)
	{
		// 정점 배열에 데이터를로드합니다.
	// 첫 번째 삼각형
		vertices[0].position = XMFLOAT3(v0.x, v0.y, z);  // Top left.
		vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

		vertices[1].position = XMFLOAT3(v2.x, v2.y, z);  // Bottom right.
		vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

		vertices[2].position = XMFLOAT3(v3.x, v3.y, z);  // Bottom left.
		vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

		// 두 번째 삼각형
		vertices[3].position = XMFLOAT3(v0.x, v0.y, z);  // Top left.
		vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

		vertices[4].position = XMFLOAT3(v1.x, v1.y, z);  // Top right.
		vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

		vertices[5].position = XMFLOAT3(v2.x, v2.y, z);  // Bottom right.
		vertices[5].texture = XMFLOAT2(1.0f, 1.0f);

	}
	else // 좌우반전
	{
		// 정점 배열에 데이터를로드합니다.
		// 첫 번째 삼각형
		vertices[0].position = XMFLOAT3(v0.x, v0.y, z);  // Top left.
		vertices[0].texture = XMFLOAT2(1.0f, 0.0f);

		vertices[1].position = XMFLOAT3(v2.x, v2.y, z);  // Bottom right.
		vertices[1].texture = XMFLOAT2(0.0f, 1.0f);

		vertices[2].position = XMFLOAT3(v3.x, v3.y, z);  // Bottom left.
		vertices[2].texture = XMFLOAT2(1.0f, 1.0f);

		// 두 번째 삼각형
		vertices[3].position = XMFLOAT3(v0.x, v0.y, z);  // Top left.
		vertices[3].texture = XMFLOAT2(1.0f, 0.0f);

		vertices[4].position = XMFLOAT3(v1.x, v1.y, z);  // Top right.
		vertices[4].texture = XMFLOAT2(0.0f, 0.0f);

		vertices[5].position = XMFLOAT3(v2.x, v2.y, z);  // Bottom right.
		vertices[5].texture = XMFLOAT2(0.0f, 1.0f);

	}
	


	// 버텍스 버퍼를 쓸 수 있도록 잠급니다.
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// 정점 버퍼의 데이터를 가리키는 포인터를 얻는다.
	verticesPtr = (VertexType*)mappedResource.pData;

	// 데이터를 정점 버퍼에 복사합니다.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

	// 정점 버퍼의 잠금을 해제합니다.
	deviceContext->Unmap(m_vertexBuffer, 0);

	// 더 이상 필요하지 않은 꼭지점 배열을 해제합니다.
	delete[] vertices;
	vertices = 0;

	return true;
}


bool TexCom::UpdateBuffers(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;

	// 이 비트맵을 렌더링 할 위치가 변경되지 않은 경우 정점 버퍼를 업데이트 하지 마십시오.
	// 현재 올바른 매개 변수가 있습니다.

	float left, right, top, bottom;

	if ((positionX == m_previousPosX) && (positionY == m_previousPosY))
	{
		return true;
	}

	// 변경된 경우 렌더링되는 위치를 업데이트합니다.
	m_previousPosX = positionX;
	m_previousPosY = positionY;

	// 비트 맵 왼쪽의 화면 좌표를 계산합니다.
	left = (float)((m_screenWidth / 2) * -1) + (float)positionX;

	// 비트 맵 오른쪽의 화면 좌표를 계산합니다.
	right = left + (float)m_bitmapWidth;

	// 비트 맵 상단의 화면 좌표를 계산합니다.
	top = (float)(m_screenHeight / 2) - (float)positionY;

	// 비트 맵 아래쪽의 화면 좌표를 계산합니다.
	bottom = top - (float)m_bitmapHeight;


	// 정점 배열을 만듭니다.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// 정점 배열에 데이터를로드합니다.
	// 첫 번째 삼각형
	vertices[0].position = XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[2].position = XMFLOAT3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

	// 두 번째 삼각형
	vertices[3].position = XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[4].position = XMFLOAT3(right, top, 0.0f);  // Top right.
	vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[5].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].texture = XMFLOAT2(1.0f, 1.0f);


	


	// 버텍스 버퍼를 쓸 수 있도록 잠급니다.
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// 정점 버퍼의 데이터를 가리키는 포인터를 얻는다.
	verticesPtr = (VertexType*)mappedResource.pData;

	// 데이터를 정점 버퍼에 복사합니다.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

	// 정점 버퍼의 잠금을 해제합니다.
	deviceContext->Unmap(m_vertexBuffer, 0);

	// 더 이상 필요하지 않은 꼭지점 배열을 해제합니다.
	delete[] vertices;
	vertices = 0;

	return true;
}

void TexCom::RenderBuffers()
{
	ID3D11DeviceContext* deviceContext = DXClass::Inst().GetDeviceContext();

	// 정점 버퍼의 단위와 오프셋을 설정합니다.
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	// 렌더링 할 수 있도록 입력 어셈블러에서 정점 버퍼를 활성으로 설정합니다.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// 렌더링 할 수 있도록 입력 어셈블러에서 인덱스 버퍼를 활성으로 설정합니다.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 정점 버퍼로 그릴 기본형을 설정합니다. 여기서는 삼각형으로 설정합니다.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


bool TexCom::LoadTexture(WCHAR* filename)
{
	// 텍스처 오브젝트를 생성한다.
	m_Texture = new Tex;

	if (!m_Texture)
	{
		return false;
	}

	// 텍스처 오브젝트를 초기화한다.
	return m_Texture->Init(filename);
}


void TexCom::ReleaseTexture()
{
	// 텍스처 오브젝트를 릴리즈한다.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
}