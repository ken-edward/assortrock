#include "stdafx.h"
#include "ColCom.h"
#include "TexCom.h"
#include "InputClass.h"
#include "ColMgr.h"

bool ColCom::debug = false;

ColCom::ColCom()
{
	debugRect = new TexCom;
	debugRect->Init(L"../tex/redRect.png");
}

ColCom::ColCom(ColType _type)
{
	debugRect = new TexCom;
	debugRect->Init(L"../tex/redRect.png");
	m_Type = _type;
}


ColCom::~ColCom()
{
	// ����׷�Ʈ ����

	if (debugRect != nullptr)
	{
		delete debugRect;
		debugRect = nullptr;
	}

	// �ʿ��� �ڽ��� ����
	auto group = ColMgr::Inst().GetColGroup(name);
	group->erase(this);

	// �ڽ��� ����Ű�� �����͵� ����
	auto iter = colSet.begin();
	auto end = colSet.end();

	for (; iter != end; ++iter)
	{
		(*iter)->colSet.erase(this);
	}

	colSet.clear();
}


void ColCom::ColToAnother(ColCom* other)
{
	if (OneToOne(this, other))
	{
		// ������ set�� ���θ� ����
		auto thisIter = colSet.find(other);

		if (thisIter == colSet.end())
		{
			colSet.insert(other);
			other->colSet.insert(this);
		}
	}
	else
	{
		this;
		// ������ set���� ����
		auto thisIter = colSet.find(other);

		if (thisIter != colSet.end())
		{
			colSet.erase(other);
			other->colSet.erase(this);
		}
	}
}


bool ColCom::OneToOne(ColCom* left, ColCom* right)
{
	if (left->pass == false && left->name == L"dead")
	{
		int a = 0;
	}
	if (right->pass == false && right->name == L"dead")
	{
		int a = 0;
	}

	switch (left->m_Type)
	{
	case ColRect:
		switch (right->m_Type)
		{
		case ColRect:
			return RectToRect(left, right);
		case ColCircle:
			return RectToCircle(left, right);
		case ColProjectile:
		case ColLine:
			return LineToRect(right, left);
		default:
			return false;
		}
	case ColProjectile:
	case ColLine:
		switch (right->m_Type)
		{
		case ColRect:
			return LineToRect(left, right);
		default:
			return false;
		}
	case ColCircle:
		switch (right->m_Type)
		{
		case ColRect:
			return RectToCircle(right, left);
		default:
			return false;
		}
	default:
		return false;
	}
}



bool ColCom::RectToRect(ColCom* left, ColCom* right)
{
	if (left->pos.x + left->size.x < right->pos.x - right->size.x) // left�� right�� ���ʿ� �ֽ��ϴ�.
	{
		return false;
	}
	if (left->pos.x - left->size.x > right->pos.x + right->size.x) // left�� right�� �����ʿ� �ֽ��ϴ�.
	{
		return false;
	}
	if (left->pos.y - left->size.y > right->pos.y + right->size.y) // left�� right�� ���ʿ� �ֽ��ϴ�.
	{
		return false;
	}
	if (left->pos.y + left->size.y < right->pos.y - right->size.y) // left�� right�� �Ʒ��ʿ� �ֽ��ϴ�.
	{
		return false;
	}

	return true;
}

bool ColCom::RectToRect(VEC2 lpos, VEC2 lsize, VEC2 rpos, VEC2 rsize)
{
	if (lpos.x + lsize.x < rpos.x - rsize.x) // left�� right�� ���ʿ� �ֽ��ϴ�.
	{
		return false;
	}
	if (lpos.x - lsize.x > rpos.x + rsize.x) // left�� right�� �����ʿ� �ֽ��ϴ�.
	{
		return false;
	}
	if (lpos.y - lsize.y > rpos.y + rsize.y) // left�� right�� ���ʿ� �ֽ��ϴ�.
	{
		return false;
	}
	if (lpos.y + lsize.y < rpos.y - rsize.y) // left�� right�� �Ʒ��ʿ� �ֽ��ϴ�.
	{
		return false;
	}

	return true;
}


bool ColCom::RectToCircle(ColCom* rect, ColCom* circle)
{
	VEC2 point = circle->pos;
	float radius = circle->radius;

	// ���� false ����
	if (RectToRect(rect->pos, rect->size, point, { radius, radius }) == false)
	{
		return false;
	}

	// ���η� ���� ���簢��
	if (RectToPoint(rect->pos, { rect->size.x + radius, rect->size.y }, point))
	{
		return true;
	}

	// ���η� ���� ���簢��
	if (RectToPoint(rect->pos, { rect->size.x, rect->size.y + radius }, point))
	{
		return true;
	}

	// �ڳ� �� ����
	if (point.x > rect->pos.x) // ����
	{
		if (point.y > rect->pos.y) // ����
		{
			if (CircleToPoint({ rect->pos.x + rect->size.x, rect->pos.y + rect->size.y}, radius, point))
			{
				return true;
			}
		}
		else // ���ϴ�
		{
			if (CircleToPoint({ rect->pos.x + rect->size.x, rect->pos.y - rect->size.y }, radius, point))
			{
				return true;
			}
		}
	}
	else // ����
	{
		if (point.y > rect->pos.y) // �»��
		{
			if (CircleToPoint({ rect->pos.x - rect->size.x, rect->pos.y + rect->size.y }, radius, point))
			{
				return true;
			}
		}
		else // ���ϴ�
		{
			if (CircleToPoint({ rect->pos.x - rect->size.x, rect->pos.y - rect->size.y }, radius, point))
			{
				return true;
			}
		}
	}

	return false;
}


bool ColCom::LineToRect(ColCom* line, ColCom* rect)
{
	if (line->m_Type == ColProjectile)
	{
		if (line->shooter == rect)
		{
			return false;
		}
		if (rect->pass)
		{
			return false;
		}
		if (rect->character)
		{
			if (line->player == rect->player) // ���� ���� ���� ����
			{
				return false;
			}
		}
	}

	// ���а� �簢���� �浹
	VEC2 startpos = line->startpos;
	VEC2 endpos = line->endpos;

	float slope = (endpos.y - startpos.y)
		/ (endpos.x - startpos.x);

	float intercept = startpos.y - startpos.x * slope;

	float leftX = rect->pos.x - rect->size.x;
	float rightX = rect->pos.x + rect->size.x;
	float topY = rect->pos.y + rect->size.y;
	float botY = rect->pos.y - rect->size.y;

	float leftResult = leftX * slope + intercept;
	float rightResult = rightX * slope + intercept;

	// �������� ������ �� ������ ���̰� �̴��� ��� false
	if (startpos.x < leftX && endpos.x < leftX)
	{
		return false;
	}
	if (startpos.x > rightX && endpos.x > rightX)
	{
		return false;
	}
	if (startpos.y > topY && endpos.y > topY)
	{
		return false;
	}
	if (startpos.y < botY && endpos.y < botY)
	{
		return false;
	}

	// ������ �׽�Ʈ
	if (leftResult > botY && leftResult < topY) // �º� ���
	{
		return true;
	}
	if (rightResult > botY && rightResult < topY) // �캯 ���
	{
		return true;
	}
	if ((leftResult > topY && rightResult < topY) ||
		(leftResult < topY && rightResult > topY)) // �� ���
	{
		return true;
	}
	if ((leftResult > botY && rightResult < botY) ||
		(leftResult < botY && rightResult > botY)) // �Ϻ� ���
	{
		return true;
	}

	// ������ �׽�Ʈ ����
	return false;
}


bool ColCom::RectToPoint(ColCom* rect, ColCom* point)
{
	if ((point->pos.x > rect->pos.x - rect->size.x) && (point->pos.x < rect->pos.x + rect->size.x) &&
		(point->pos.y < rect->pos.y + rect->size.y) && (point->pos.y > rect->pos.y - rect->size.y))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ColCom::CircleToPoint(ColCom* circle, ColCom* _point)
{
	VEC2 point = _point->pos;

	VEC2 gap = point - circle->pos;

	float theta = gap.theta();

	float radius = circle->radius;

	VEC2 pos = circle->pos;

	// ��� theta, ���κ�
	if (theta > PI * 0 / 2 && theta < PI * 1 / 2)
	{
		if (point.x < cos(theta) * radius + pos.x && point.y < sin(theta) * radius + pos.y)
		{
			return true;
		}
	}

	if (theta > PI * 0 / 2 && theta < PI * 1 / 2)
	{
		if (point.x > cos(theta) * radius + pos.x && point.y < sin(theta) * radius + pos.y)
		{
			return true;
		}
	}

	// ���� theta, �Ʒ��κ�
	if (theta > PI * -1 / 2 && theta < PI * -0 / 2)
	{
		if (point.x < cos(theta) * radius + pos.x&& point.y > sin(theta) * radius + pos.y)
		{
			return true;
		}
	}

	if (theta > PI * -2 / 2 && theta < PI * -1 / 2)
	{
		if (point.x > cos(theta) * radius + pos.x&& point.y > sin(theta) * radius + pos.y)
		{
			return true;
		}
	}

	return false;
}


bool ColCom::RectToPoint(VEC2 pos, VEC2 size, VEC2 point)
{
	if ((point.x > pos.x - size.x) && (point.x < pos.x + size.x) &&
		(point.y < pos.y + size.y) && (point.y > pos.y - size.y))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ColCom::CircleToCircle(VEC2 lpos, float lrad, VEC2 rpos, float rrad)
{
	VEC2 gap = rpos - lpos;

	if (gap.scalar() <= lrad + rrad)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ColCom::CircleToPoint(VEC2 pos, float radius, VEC2 point)
{
	return CircleToCircle(pos, radius, point, 0.0f);
}


void ColCom::SetRect(VEC2 _pos, VEC2 _size)
{
	pos = _pos;
	size = _size;
}


void ColCom::Move(VEC2 move)
{
	pos += move;
}


bool ColCom::Find(std::wstring name)
{
	auto iter = colSet.begin();
	auto end = colSet.end();

	for (; iter != end; ++iter)
	{
		if ((*iter)->name == name)
		{
			return true;
		}
	}

	return false;
}

ColCom* ColCom::Get(std::wstring name)
{
	auto iter = colSet.begin();
	auto end = colSet.end();

	for (; iter != end; ++iter)
	{
		if ((*iter)->name == name)
		{
			return *iter;
		}
	}

	return nullptr;
}


bool ColCom::SubFind(std::wstring name)
{
	auto iter = colSet.begin();
	auto end = colSet.end();

	for (; iter != end; ++iter)
	{
		if ((*iter)->subname == name)
		{
			return true;
		}
	}

	return false;
}

ColCom* ColCom::SubGet(std::wstring name)
{
	auto iter = colSet.begin();
	auto end = colSet.end();

	for (; iter != end; ++iter)
	{
		if ((*iter)->subname == name)
		{
			return *iter;
		}
	}

	return nullptr;
}

void ColCom::Update5()
{
	if (debug)
	{
		switch (m_Type)
		{
		case ColRect:
		{
			debugRect->SetPos(pos);
			debugRect->SetSize(size);
			debugRect->SetZ(-5.0f);
			debugRect->Update5();
			break;
		}
		default:
			break;
		}
	}
}