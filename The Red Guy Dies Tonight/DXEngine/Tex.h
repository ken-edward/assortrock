#pragma once
#include <DirectXTex.h>
class Tex
{
public:
	Tex();
	~Tex();

	bool Init(WCHAR*, int ext = 0);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* m_texture = nullptr;
	DirectX::ScratchImage m_scratchImage;
	ID3D11Texture2D* m_tex2D = nullptr;

public:
	size_t GetWidth()
	{
		return m_scratchImage.GetMetadata().width;
	}

	size_t GetHeight()
	{
		return m_scratchImage.GetMetadata().height;
	}
};