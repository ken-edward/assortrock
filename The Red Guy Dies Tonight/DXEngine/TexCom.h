#pragma once
#include "Com.h"
#include "Vector.h"
#include <DirectXMath.h>
#include "stdafx.h"
#include <list>
// ��ũ��Ʈ�� ���� ����Ʈ�� ��Ͻ��״ٰ�
// ���߿� ������� Render�� ȣ���ؾ� �մϴ�.
class Tex;
class TexCom : public Com
{
public:
	TexCom();
	TexCom(wstring);
	~TexCom();

private:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};

public:
	VEC2 v0;
	VEC2 v1;
	VEC2 v2;
	VEC2 v3;
	float z = 0.0f;
	std::wstring name; // ������

protected:
	VEC2 size; // �⺻ �̹��� ������

private:
	VEC2 pos; // ��ġ
	float scale = 1.0f; // ����
	float rotation = 0.0f; // rad ����
	VEC2 pivot; // �Ǻ�
	bool flip = false;

public:
	void SetZ(float _z) { z = _z; } // z ��ġ ����
	void SetSize(VEC2 val) { bChange = true; size = val; }
	void SetPos(VEC2 val) { bChange = true; pos = val; }
	void SetScale(float); // ũ�� ����
	void SetRotation(float); // ȸ�� ����
	void SetPivot(VEC2 val) { pivot = val; }

public:
	void Flip() { flip = !flip; }
	void Flip(bool val) { flip = val; }

public:
	float GetZ() { return z; }
	VEC2 GetPos() { return pos + pivot; }
	VEC2 GetSize() { return size; }
	float GetScale() { return scale; }
	float GetRotation() { return rotation; }
	VEC2 GetPivot() { return pivot; }

public:
	void Move(VEC2) override; // ��ġ ��ȭ
	void Enlarge(float); // ũ�� ��ȭ
	void Rotate(float); // ȸ�� ��ȭ

public:
	virtual void Render();
	void Update5() override;

protected:
	void ResetVertex();

public:
	void Refresh() { bChange = true; }

public:
	bool Init(wchar_t*); // ����
	bool Initialize(int, int, WCHAR*, int, int);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

protected:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	bool UpdateBuffers(); // ����
	bool UpdateBuffers(ID3D11DeviceContext*, int, int);
	void RenderBuffers();

	bool LoadTexture(WCHAR*);
	void ReleaseTexture();

protected:
	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;
	int m_vertexCount = 0;
	int m_indexCount = 0;
	Tex* m_Texture = nullptr;
	int m_screenWidth = 0;
	int m_screenHeight = 0;
	int m_bitmapWidth = 0;
	int m_bitmapHeight = 0;
	int m_previousPosX = 0;
	int m_previousPosY = 0;
	bool fromLeftTop = false;
	bool bChange = true;

public:
	bool display = true;
};