#pragma once
#include "Com.h"
#include <set>
#include "Vector.h"
enum ColType
{
	ColRect, // a rectangle with no spin
	ColOBB, // a rectangle with potential spin
	ColCircle, // a regular circle with consistent radius
	ColLine, // a line with start and end point
	ColProjectile, // line collision but hit only one target
	ColPoint, // a point with exact position
	ColPolygon, // a shape with multiple vertex
};

class TexCom;
class ColCom :
	public Com
{
public:
	ColCom();
	ColCom(ColType);
	~ColCom();

public:
	std::wstring name;

public:
	void ColToAnother(ColCom*);

private:
	static bool OneToOne(ColCom* left, ColCom* right);
	static bool RectToRect(ColCom*, ColCom*);
	static bool LineToRect(ColCom*, ColCom*);
	static bool RectToCircle(ColCom*, ColCom*);
	static bool RectToPoint(ColCom*, ColCom*);
	static bool CircleToPoint(ColCom*, ColCom*);

private: // 내부 함수
	static bool RectToRect(VEC2, VEC2, VEC2, VEC2);
	static bool RectToPoint(VEC2, VEC2, VEC2);
	static bool CircleToPoint(VEC2, float, VEC2);
	static bool CircleToCircle(VEC2, float, VEC2, float);

public:
	void SetRect(VEC2 pos, VEC2 size);
	void SetPos(VEC2 val) { pos = val; }
	void SetSize(VEC2 val) { size = val; }
	void Move(VEC2 value) override;
	bool Find(std::wstring);
	ColCom* Get(std::wstring);
	bool SubFind(std::wstring);
	ColCom* SubGet(std::wstring);
	VEC2 GetSize() { return size; }
	VEC2 GetPos() { return pos; }
	void SetType(ColType _type) { m_Type = _type; }
	ColType GetType() { return m_Type; }
	VEC2 hitpoint;
	std::set<ColCom*>* GetColSet() { return &colSet; }
	friend class ColMgr;

public:
	union
	{
		VEC2 pos; // point

		struct // Rect
		{
			VEC2 rectpos; // same with pos
			VEC2 size;
		};

		struct // Circle
		{
			VEC2 circlepos; // same with pos
			float radius;
		};

		struct // Line
		{
			VEC2 startpos; // share with pos
			VEC2 endpos;
		};
	};

private:
	ColType m_Type = ColRect;
	std::set<ColCom*> colSet;
	ColCom* target = nullptr; // 투사체만 사용

private:
	TexCom* debugRect = nullptr;
	static bool debug;

public:
	ColCom* shooter = nullptr;
	wstring subname;
	bool pass = false;
	bool player = false;
	bool character = false;

public:
	static void Toggle() { debug = !debug; }

private:
	void Update5() override;
};

