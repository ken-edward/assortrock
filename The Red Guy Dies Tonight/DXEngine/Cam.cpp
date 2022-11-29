#include "stdafx.h"
#include "Cam.h"
#include "TimeClass.h"
#include "DXClass.h"


Cam::Cam()
{
	m_position = XMFLOAT3(0.0f, 0.0f, -100.0f);
	m_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
}


Cam::~Cam()
{
}


void Cam::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}


void Cam::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
}


XMFLOAT3 Cam::GetPosition()
{
	return m_position;
}


XMFLOAT3 Cam::GetRotation()
{
	return m_rotation;
}


void Cam::Update()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	if (panning == true)
	{
		MovingFocus();
	}
	if (sizechange == true)
	{
		SizeChange();
	}

	// 위쪽을 가리키는 벡터를 설정합니다.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// XMVECTOR 구조체에 로드한다.
	upVector = XMLoadFloat3(&up);

	// 3D월드에서 카메라의 위치를 ​​설정합니다.
	position = m_position;

	// XMVECTOR 구조체에 로드한다.
	positionVector = XMLoadFloat3(&position);

	// 기본적으로 카메라가 찾고있는 위치를 설정합니다.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// XMVECTOR 구조체에 로드한다.
	lookAtVector = XMLoadFloat3(&lookAt);

	// yaw (Y 축), pitch (X 축) 및 roll (Z 축)의 회전값을 라디안 단위로 설정합니다.
	pitch = m_rotation.x * 0.0174532925f;
	yaw = m_rotation.y * 0.0174532925f;
	roll = m_rotation.z * 0.0174532925f;

	//  yaw, pitch, roll 값을 통해 회전 행렬을 만듭니다.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// lookAt 및 up 벡터를 회전 행렬로 변형하여 뷰가 원점에서 올바르게 회전되도록 합니다.
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// 회전 된 카메라 위치를 뷰어 위치로 변환합니다.
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// 마지막으로 세 개의 업데이트 된 벡터에서 뷰 행렬을 만듭니다.
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}


XMMATRIX Cam::GetViewMatrix()
{
	return m_viewMatrix;
}


void Cam::ChangeFocus(VEC2 val, float _dura)
{
	dura = _dura;

	panning = true;

	pantime = 0.0f;

	start.x = m_position.x;
	start.y = m_position.y;

	target.x = val.x;
	target.y = val.y;

	pos = start;

	gap = target - start;

	speed = gap / dura;

	tempspeed = speed * 2;
}

void Cam::SetFocus(VEC2 val)
{
	panning = false;

	pos.x = val.x;
	pos.y = val.y;

	m_position.x = val.x;
	m_position.y = val.y;
}


void Cam::MovingFocus()
{
	pantime += DT;

	float progression = (pos - start).scalar() / gap.scalar();

	if (progression < 1.0f)
	{
		pos += tempspeed * DT;

		tempspeed = speed * (1 - progression) * 2;
		
		m_position.x = pos.x;
		m_position.y = pos.y;
	}
	else
	{
		panning = false;
		pantime = 0.0f;

		m_position.x = target.x;
		m_position.y = target.y;
	}
}



void Cam::SetSize(float value)
{
	size = value;
	DXClass::Inst().UpdateOrtho(size);
}


void Cam::ChangeSize(float value, float _sizedura)
{
	sizedura = _sizedura;

	sizechange = true;

	sizetime = 0.0f;

	targetsize = value;
	startsize = size;
	
	sizegap = targetsize - startsize;

	sizespeed = sizegap / sizedura;

	tempsizespeed = sizespeed * 2;
}


void Cam::SizeChange()
{
	sizetime += DT;

	float progression = fabsf((size - startsize)) / fabsf(sizegap);

	if (progression < 1.0f)
	{
		size += tempsizespeed * DT;

		tempsizespeed = sizespeed * (1 - progression) * 2;
	}
	else
	{
		sizechange = false;
		sizetime = 0.0f;
		size = targetsize;
	}
	DXClass::Inst().UpdateOrtho(size);
}