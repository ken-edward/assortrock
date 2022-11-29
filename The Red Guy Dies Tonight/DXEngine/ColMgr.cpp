#include "stdafx.h"
#include "ColMgr.h"
#include "ColCom.h"


ColMgr::ColMgr()
{
}


ColMgr::~ColMgr()
{
}


void ColMgr::Add(std::wstring groupName, ColCom* colcom)
{
	colcom->name = groupName;

	auto mapIter = colGroupMap.find(groupName);

	if (mapIter == colGroupMap.end())
	{
		std::set<ColCom*> newSet;

		newSet.insert(colcom);

		colGroupMap.insert(pair<wstring, set<ColCom*>>(groupName, newSet));
	}
	else
	{
		mapIter->second.insert(colcom);
	}
}


void ColMgr::ColAll()
{
	auto leftIter = colLinkMap.begin();
	auto leftEnd = colLinkMap.end();

	for (; leftIter != leftEnd; ++leftIter) // left colgroup
	{
		auto rightIter = leftIter->second.begin();
		auto rightEnd = leftIter->second.end();

		for (; rightIter != rightEnd; ++rightIter) // right colgroup
		{
			// collision between two group

			wstring leftname = leftIter->first;
			wstring rightname = *rightIter;

			auto leftGroupMapIter = colGroupMap.find(leftname);
			auto rightGroupMapIter = colGroupMap.find(rightname);

			if (leftGroupMapIter == colGroupMap.end() || rightGroupMapIter == colGroupMap.end())
			{
				continue; // ��� ������ �׷��� ��� �ֽ��ϴ�.
			}

			set<ColCom*>* leftGroup = &colGroupMap.find(leftname)->second;
			set<ColCom*>* rightGroup = &colGroupMap.find(rightname)->second;

			auto leftGroupIter = leftGroup->begin();
			auto leftGroupEnd = leftGroup->end();

			for (; leftGroupIter != leftGroupEnd; ++leftGroupIter)
			{
				auto rightGroupIter = rightGroup->begin();
				auto rightGroupEnd = rightGroup->end();

				for (; rightGroupIter != rightGroupEnd; ++rightGroupIter)
				{
					if (*leftGroupIter == *rightGroupIter) // �׷���� ���� ��� �ڽų��� �ɸ� ���� �ֽ��ϴ�. ���� ó��.
					{
						continue;
					}

					(*leftGroupIter)->ColToAnother(*rightGroupIter); // specific collision

					// ���� �� ��ȿ�����̹Ƿ�
					// Link �� ����ü�� ���ʿ� �� ���� �����մϴ�.
					if ((*rightGroupIter)->GetType() == ColProjectile)
					{
						LeaveOneTarget(*rightGroupIter);
					}
				}

				if ((*leftGroupIter)->GetType() == ColProjectile)
				{
					LeaveOneTarget(*leftGroupIter);
				}
			}
		}
	}
}


void ColMgr::Link(std::wstring left, std::wstring right)
{
	auto mapIter = colLinkMap.find(left);

	if (mapIter == colLinkMap.end())
	{
		list<wstring> linklist;

		linklist.push_back(right);

		colLinkMap.insert(pair<wstring, list<wstring>>(left, linklist));
	}
	else
	{
		mapIter->second.push_back(right);
	}
}


void ColMgr::ClearAll()
{
	colGroupMap.clear();
	colLinkMap.clear();
}


std::set<ColCom*>* ColMgr::GetColGroup(std::wstring groupName)
{
	auto find = colGroupMap.find(groupName);
		
	if (find != colGroupMap.end())
	{
		return &find->second;
	}

	return nullptr;
}



void ColMgr::LeaveOneTarget(ColCom* col)
{
	float dis = 0.0f;
	VEC2 hitpoint;
	ColCom* target = nullptr;

	auto iter = col->colSet.begin();
	auto end = col->colSet.end();

	for (; iter != end; ++iter) // ���� ����� �� ���� hitpoint �������� ��󳻴� �۾�
	{
		VEC2 temphitpoint = GetHitPoint(col, *iter);

		float tempdis = (temphitpoint - col->startpos).scalar();

		if (target == nullptr)
		{
			dis = tempdis;
			target = *iter;
			hitpoint = temphitpoint;
		}
		else
		{
			if (tempdis < dis)
			{
				dis = tempdis;
				target = *iter;
				hitpoint = temphitpoint;
			}
		}
	}

	if (target != nullptr) // target �����ϰ� ��� ����� �۾�
	{
		iter = col->colSet.begin();
		end = col->colSet.end();

		for (; iter != end;)
		{
			if (*iter != target)
			{
				(*iter)->colSet.erase(col);
				iter = col->colSet.erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}

	col->hitpoint = hitpoint;
}


VEC2 ColMgr::GetHitPoint(ColCom* projectile, ColCom* other)
{
	VEC2 hitpoint;

	switch (other->m_Type)
	{
	case ColRect:
	{
		VEC2 startpos = projectile->startpos;
		VEC2 endpos = projectile->endpos;

		// ����
		float slope = (endpos.y - startpos.y) / (endpos.x - startpos.x);

		// ����
		float intercept = startpos.y - (startpos.x * slope);

		// �簢�� ��ǥ
		float leftX = other->pos.x - other->size.x;
		float rightX = other->pos.x + other->size.x;
		float botY = other->pos.y - other->size.y;
		float topY = other->pos.y + other->size.y;

		// �������� ���ο� ����

		if (leftX < startpos.x && startpos.x < rightX && botY < startpos.y && startpos.y < topY)
		{
			return startpos; // hitpoint
		}

		// �簢�� ���� �׽�Ʈ

		float leftResult = slope * leftX + intercept;
		if (botY < leftResult && leftResult < topY && startpos.x < leftX)
		{
			hitpoint.x = leftX;
			hitpoint.y = leftResult;
			return hitpoint;
		}

		// �簢�� ���� �׽�Ʈ
		float rightResult = slope * rightX + intercept;
		if (botY < rightResult && rightResult < topY && rightX < startpos.x)
		{
			hitpoint.x = rightX;
			hitpoint.y = rightResult;
			return hitpoint;
		}

		// �簢�� ��� �׽�Ʈ
		if (startpos.y > topY)
		{
			if ((leftResult > topY && rightResult < topY) ||
				(rightResult > topY && leftResult < topY))
			{
				hitpoint.y = topY;

				float nointercept = topY - intercept;
				float revX = nointercept / slope;

				hitpoint.x = revX;

				return hitpoint;
			}
		}


		// �簢�� �ϴ� �׽�Ʈ
		if (startpos.y < botY)
		{
			if ((leftResult > botY && rightResult < botY) ||
				(rightResult > botY && leftResult < botY))
			{
				hitpoint.y = botY;

				float nointercept = botY - intercept;
				float revX = nointercept / slope;

				hitpoint.x = revX;

				return hitpoint;
			}
		}

	}
	default:
		return hitpoint;
	}
}