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
	// ���ؽ��� �ٲٴ� ��� public �Լ����� �̸� ����� �մϴ�.
	// ����: pos, scale, rotation�� ��ȭ�� �ִ� �͵�.
	{
		bChange = false;

		// ��ġ, ũ��, ȸ���� ������� ���ؽ��� �ٽ� �����ؾ� �մϴ�.
		// ������ ũ��->ȸ��->��ġ ������ �ϴ� �� �����ϴ�.

		// ��� ȸ�� ���� �ص� �Ǵµ� ��ġ���� ���� ���߿� �ؾ� �մϴ�.

		// ���ؽ��� �ʱ�ȭ�մϴ�.
		// ���� �̹��� ũ�⿡ ���� ���ؽ��� �ٲ�ϴ�.
		ResetVertex();

		// scale�� ���� ���ؽ��� �Ÿ��� �����մϴ�.
		v0 *= scale;
		v1 *= scale;
		v2 *= scale;
		v3 *= scale;

		// ���ؽ��� ȸ����ŵ�ϴ�.
		v0.rotate(rotation);
		v1.rotate(rotation);
		v2.rotate(rotation);
		v3.rotate(rotation);
		
		// ���ؽ��� �̵���ŵ�ϴ�.
		v0 += pos + pivot;
		v1 += pos + pivot;
		v2 += pos + pivot;
		v3 += pos + pivot;

		// ���ؽ��� ���� ���۸� ������Ʈ�մϴ�.
		UpdateBuffers();
	}
	
	RenderBuffers();
	TexShader::Inst().Render(GetTexture());
}


void TexCom::Update5()
{
	RenderMgr::Inst().Queue(this, z);
}


void TexCom::SetScale(float value) // ũ�� ����
{
	bChange = true;

	scale = value;
}

void TexCom::SetRotation(float value) // ȸ�� ����
{
	bChange = true;

	rotation = value;
}

void TexCom::Move(VEC2 val) // ��ġ ��ȭ
{
	bChange = true;

	pos += val;
}

void TexCom::Enlarge(float value) // ũ�� ��ȭ
{
	bChange = true;

	scale *= value;
}

void TexCom::Rotate(float value) // ȸ�� ��ȭ
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

	// ȭ�� ũ�⸦ ��������� ����
	m_screenWidth = 0;
	m_screenHeight = 0;

	// ���� ������ ��ġ�� ������ �ʱ�ȭ�մϴ�.
	m_previousPosX = -1;
	m_previousPosY = -1;

	// ���� �� �ε��� ���۸� �ʱ�ȭ�մϴ�.
	if (!InitializeBuffers(DXClass::Inst().GetDevice()))
	{
		return false;
	}

	// �� ���� �ؽ�ó�� �ε��մϴ�.
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

	// ȭ�� ũ�⸦ ��������� ����
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// �������� ��Ʈ���� �ȼ��� ũ�⸦ ����
	m_bitmapWidth = bitmapWidth;
	m_bitmapHeight = bitmapHeight;

	// ���� ������ ��ġ�� ������ �ʱ�ȭ�մϴ�.
	m_previousPosX = -1;
	m_previousPosY = -1;

	// ���� �� �ε��� ���۸� �ʱ�ȭ�մϴ�.
	if (!InitializeBuffers(DXClass::Inst().GetDevice()))
	{
		return false;
	}

	// �� ���� �ؽ�ó�� �ε��մϴ�.
	return LoadTexture(textureFilename);
}


void TexCom::Shutdown()
{
	// �� �ؽ��ĸ� ��ȯ�մϴ�.
	ReleaseTexture();

	// ���ؽ� �� �ε��� ���۸� �����մϴ�.
	ShutdownBuffers();
}




bool TexCom::Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	// ȭ���� �ٸ� ��ġ�� �������ϱ� ���� ���� ���� ���۸� �ٽ� �����մϴ�.
	if (!UpdateBuffers(deviceContext, positionX, positionY))
	{
		return false;
	}

	// �׸��⸦ �غ��ϱ� ���� �׷��� ������ ���ο� �������� �ε��� ���۸� �����ϴ�.
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
	// ���� �迭�� ���� ���� �ε��� �迭�� �ε��� ���� �����մϴ�.
	m_indexCount = m_vertexCount = 6;

	// ���� �迭�� ����ϴ�.
	VertexType* vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// ���� �迭�� 0���� �ʱ�ȭ�մϴ�.
	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	// �ε��� �迭�� ����ϴ�.
	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// �����ͷ� �ε��� �迭�� �ε��մϴ�.
	for (int i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}

	// ���� ���� ������ ����ü�� �����մϴ�.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource ������ ���� �����Ϳ� ���� �����͸� �����մϴ�.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// ���� ���� ���۸� ����ϴ�.
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		return false;
	}

	// ���� �ε��� ������ ����ü�� �����մϴ�.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// �ε��� �����͸� ����Ű�� ���� ���ҽ� ����ü�� �ۼ��մϴ�.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// �ε��� ���۸� �����մϴ�.
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	{
		return false;
	}

	// �����ǰ� ���� �Ҵ�� ���� ���ۿ� �ε��� ���۸� �����մϴ�.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


void TexCom::ShutdownBuffers()
{
	// �ε��� ���۸� �����մϴ�.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// ���� ���۸� �����մϴ�.
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

	// ���� �迭�� ����ϴ�.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	if (flip == false)
	{
		// ���� �迭�� �����͸��ε��մϴ�.
	// ù ��° �ﰢ��
		vertices[0].position = XMFLOAT3(v0.x, v0.y, z);  // Top left.
		vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

		vertices[1].position = XMFLOAT3(v2.x, v2.y, z);  // Bottom right.
		vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

		vertices[2].position = XMFLOAT3(v3.x, v3.y, z);  // Bottom left.
		vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

		// �� ��° �ﰢ��
		vertices[3].position = XMFLOAT3(v0.x, v0.y, z);  // Top left.
		vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

		vertices[4].position = XMFLOAT3(v1.x, v1.y, z);  // Top right.
		vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

		vertices[5].position = XMFLOAT3(v2.x, v2.y, z);  // Bottom right.
		vertices[5].texture = XMFLOAT2(1.0f, 1.0f);

	}
	else // �¿����
	{
		// ���� �迭�� �����͸��ε��մϴ�.
		// ù ��° �ﰢ��
		vertices[0].position = XMFLOAT3(v0.x, v0.y, z);  // Top left.
		vertices[0].texture = XMFLOAT2(1.0f, 0.0f);

		vertices[1].position = XMFLOAT3(v2.x, v2.y, z);  // Bottom right.
		vertices[1].texture = XMFLOAT2(0.0f, 1.0f);

		vertices[2].position = XMFLOAT3(v3.x, v3.y, z);  // Bottom left.
		vertices[2].texture = XMFLOAT2(1.0f, 1.0f);

		// �� ��° �ﰢ��
		vertices[3].position = XMFLOAT3(v0.x, v0.y, z);  // Top left.
		vertices[3].texture = XMFLOAT2(1.0f, 0.0f);

		vertices[4].position = XMFLOAT3(v1.x, v1.y, z);  // Top right.
		vertices[4].texture = XMFLOAT2(0.0f, 0.0f);

		vertices[5].position = XMFLOAT3(v2.x, v2.y, z);  // Bottom right.
		vertices[5].texture = XMFLOAT2(0.0f, 1.0f);

	}
	


	// ���ؽ� ���۸� �� �� �ֵ��� ��޴ϴ�.
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// ���� ������ �����͸� ����Ű�� �����͸� ��´�.
	verticesPtr = (VertexType*)mappedResource.pData;

	// �����͸� ���� ���ۿ� �����մϴ�.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

	// ���� ������ ����� �����մϴ�.
	deviceContext->Unmap(m_vertexBuffer, 0);

	// �� �̻� �ʿ����� ���� ������ �迭�� �����մϴ�.
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

	// �� ��Ʈ���� ������ �� ��ġ�� ������� ���� ��� ���� ���۸� ������Ʈ ���� ���ʽÿ�.
	// ���� �ùٸ� �Ű� ������ �ֽ��ϴ�.

	float left, right, top, bottom;

	if ((positionX == m_previousPosX) && (positionY == m_previousPosY))
	{
		return true;
	}

	// ����� ��� �������Ǵ� ��ġ�� ������Ʈ�մϴ�.
	m_previousPosX = positionX;
	m_previousPosY = positionY;

	// ��Ʈ �� ������ ȭ�� ��ǥ�� ����մϴ�.
	left = (float)((m_screenWidth / 2) * -1) + (float)positionX;

	// ��Ʈ �� �������� ȭ�� ��ǥ�� ����մϴ�.
	right = left + (float)m_bitmapWidth;

	// ��Ʈ �� ����� ȭ�� ��ǥ�� ����մϴ�.
	top = (float)(m_screenHeight / 2) - (float)positionY;

	// ��Ʈ �� �Ʒ����� ȭ�� ��ǥ�� ����մϴ�.
	bottom = top - (float)m_bitmapHeight;


	// ���� �迭�� ����ϴ�.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// ���� �迭�� �����͸��ε��մϴ�.
	// ù ��° �ﰢ��
	vertices[0].position = XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[2].position = XMFLOAT3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

	// �� ��° �ﰢ��
	vertices[3].position = XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[4].position = XMFLOAT3(right, top, 0.0f);  // Top right.
	vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[5].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].texture = XMFLOAT2(1.0f, 1.0f);


	


	// ���ؽ� ���۸� �� �� �ֵ��� ��޴ϴ�.
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// ���� ������ �����͸� ����Ű�� �����͸� ��´�.
	verticesPtr = (VertexType*)mappedResource.pData;

	// �����͸� ���� ���ۿ� �����մϴ�.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

	// ���� ������ ����� �����մϴ�.
	deviceContext->Unmap(m_vertexBuffer, 0);

	// �� �̻� �ʿ����� ���� ������ �迭�� �����մϴ�.
	delete[] vertices;
	vertices = 0;

	return true;
}

void TexCom::RenderBuffers()
{
	ID3D11DeviceContext* deviceContext = DXClass::Inst().GetDeviceContext();

	// ���� ������ ������ �������� �����մϴ�.
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	// ������ �� �� �ֵ��� �Է� ��������� ���� ���۸� Ȱ������ �����մϴ�.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// ������ �� �� �ֵ��� �Է� ��������� �ε��� ���۸� Ȱ������ �����մϴ�.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// ���� ���۷� �׸� �⺻���� �����մϴ�. ���⼭�� �ﰢ������ �����մϴ�.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


bool TexCom::LoadTexture(WCHAR* filename)
{
	// �ؽ�ó ������Ʈ�� �����Ѵ�.
	m_Texture = new Tex;

	if (!m_Texture)
	{
		return false;
	}

	// �ؽ�ó ������Ʈ�� �ʱ�ȭ�Ѵ�.
	return m_Texture->Init(filename);
}


void TexCom::ReleaseTexture()
{
	// �ؽ�ó ������Ʈ�� �������Ѵ�.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
}