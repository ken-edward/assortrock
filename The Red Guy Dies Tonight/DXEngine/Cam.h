#pragma once
#include "Vector.h"
class Cam
{
private:
	Cam();
	~Cam();

public:
	static Cam& Inst()
	{
		static Cam Inst;
		return Inst;
	}

public:
	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Update();
	XMMATRIX GetViewMatrix();
	void ChangeFocus(VEC2, float dura = 0.5f);
	void SetFocus(VEC2);
	void MovingFocus();
	void SetSize(float);
	void ChangeSize(float, float sizedura = 0.5f);
	void SizeChange();

private:
	bool panning = false;
	float pantime = 0.0f;
	float dura = 0.5f;
	VEC2 start;
	VEC2 target;
	VEC2 gap;
	VEC2 pos;
	VEC2 speed;
	VEC2 tempspeed;

private:
	bool sizechange = false;
	float size = 1.0f;
	float startsize = 1.0f;
	float targetsize = 1.0f;
	float sizetime = 0.0f;
	float sizegap = 0.0f;
	float sizespeed = 0.0f;
	float tempsizespeed = 0.0f;
	float sizedura = 0.5f;

public:
	float GetSize() { return size; }
	VEC2 GetPos() { return pos; }
	void Reset()
	{
		SetSize(1.0f);
		SetFocus({ 0, 0 });
	}

private:
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
	XMMATRIX m_viewMatrix;
};