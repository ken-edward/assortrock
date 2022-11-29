#pragma once
#include "TexCom.h"
#include <vector>


class AniCom :
	public TexCom
{
public:
	AniCom();
	~AniCom();

private:
	// ������ Tex* m_Texture�� curFrameó�� ���˴ϴ�.
	std::vector<Tex*> texVector;

public:
	void Load(std::wstring texname, int _framecount, float _freq);
	void Render() override;

private:
	int frame = 0; // ���� ������
	int framecount = 1; // ������ �� (texVector.size()�� ����)
	float time = 0.0f; // ����� �ð�
	float freq = 0.1f; // �����Ӵ� �ð�. 0�̸� ū�ϳ�.
};
