#pragma once
#include <math.h>
struct VEC2
{
public:
	VEC2() : x(0.0f), y(0.0f) {}
	VEC2(float _x, float _y) : x(_x), y(_y) {}
	VEC2(const VEC2& other) : x(other.x), y(other.y) {}

public:
	float x;
	float y;

public:
	void operator+=(const VEC2& other)
	{
		x += other.x;
		y += other.y;
	}
	void operator-=(const VEC2& other)
	{
		x -= other.x;
		y -= other.y;
	}
	void operator*=(const float& value)
	{
		x *= value;
		y *= value;
	}
	void operator/=(const float& value)
	{
		x /= value;
		y /= value;
	}

public:
	VEC2 operator+(const VEC2& other)
	{
		VEC2 Return;
		Return.x = x + other.x;
		Return.y = y + other.y;
		return Return;
	}
	VEC2 operator-(const VEC2& other)
	{
		VEC2 Return;
		Return.x = x - other.x;
		Return.y = y - other.y;
		return Return;
	}
	VEC2 operator*(const float& value)
	{
		VEC2 Return;
		Return.x = x * value;
		Return.y = y * value;
		return Return;
	}
	VEC2 operator/(const float& value)
	{
		VEC2 Return;
		Return.x = x / value;
		Return.y = y / value;
		return Return;
	}

public:
	VEC2& rotate(const float& value)
	{
		float presentRad = theta();
		float len = scalar();
		float targetRad = presentRad + value;

		x = cosf(targetRad) * len;
		y = sinf(targetRad) * len;

		return *this;
	}

public:
	float theta()
	{
		return atan2f(y, x);
	}

	float scalar()
	{
		return sqrtf(x * x + y * y);
	}

	VEC2 dir()
	{
		float theta = this->theta();
		return { cosf(theta), sinf(theta) };
	}
};


struct VEC4
{
public:
	VEC4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	VEC4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	VEC4(const VEC4& other) : x(other.x), y(other.y), z(other.z), w(other.w) {}

public:
	float x;
	float y;
	float z;
	float w;
};