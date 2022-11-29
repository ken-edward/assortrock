#pragma once
#include "Com.h"
#include "Vector.h"
#include <DirectXMath.h>
#include "stdafx.h"
#include <list>
// 스크립트를 만들어서 리스트에 등록시켰다가
// 나중에 순서대로 Render를 호출해야 합니다.
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
	std::wstring name; // 디버깅용

protected:
	VEC2 size; // 기본 이미지 사이즈

private:
	VEC2 pos; // 위치
	float scale = 1.0f; // 비율
	float rotation = 0.0f; // rad 단위
	VEC2 pivot; // 피봇
	bool flip = false;

public:
	void SetZ(float _z) { z = _z; } // z 위치 설정
	void SetSize(VEC2 val) { bChange = true; size = val; }
	void SetPos(VEC2 val) { bChange = true; pos = val; }
	void SetScale(float); // 크기 설정
	void SetRotation(float); // 회전 설정
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
	void Move(VEC2) override; // 위치 변화
	void Enlarge(float); // 크기 변화
	void Rotate(float); // 회전 변화

public:
	virtual void Render();
	void Update5() override;

protected:
	void ResetVertex();

public:
	void Refresh() { bChange = true; }

public:
	bool Init(wchar_t*); // 만듬
	bool Initialize(int, int, WCHAR*, int, int);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

protected:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	bool UpdateBuffers(); // 만듬
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