#include "stdafx.h"
#include "Tex.h"
#include "DXClass.h"


Tex::Tex()
{
}

Tex::~Tex()
{
}



bool Tex::Init(WCHAR* filename, int ext)
{
	// 텍스처를 파일로부터 읽어온다.
	switch (ext)
	{
	case 0: // jpg, png, bmp, and so on
		if (FAILED(DirectX::LoadFromWICFile(filename, DirectX::WIC_FLAGS_NONE, nullptr, m_scratchImage)))
		{
			return false;
		}
		break;
	case 1: // dds
		if (FAILED(DirectX::LoadFromDDSFile(filename, DirectX::DDS_FLAGS_NONE, nullptr, m_scratchImage)))
		{
			return false;
		}
		break;
	default:
		break;
	}

	DirectX::CreateShaderResourceView(
		DXClass::Inst().GetDevice(),
		m_scratchImage.GetImages(),
		m_scratchImage.GetImageCount(),
		m_scratchImage.GetMetadata(),
		&m_texture
	);

	assert(nullptr != m_texture);

	m_texture->GetResource((ID3D11Resource**)(&m_tex2D));

	assert(nullptr != m_tex2D);

	return true;
}


void Tex::Shutdown()
{
	if (m_tex2D)
	{
		m_tex2D->Release();
	}
	else
	{
		this;
	}


	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}
}


ID3D11ShaderResourceView* Tex::GetTexture()
{
	return m_texture;
}