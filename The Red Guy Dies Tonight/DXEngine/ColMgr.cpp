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
				continue; // 어느 한쪽의 그룹이 비어 있습니다.
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
					if (*leftGroupIter == *rightGroupIter) // 그룹명이 같을 경우 자신끼리 걸릴 수도 있습니다. 예외 처리.
					{
						continue;
					}

					(*leftGroupIter)->ColToAnother(*rightGroupIter); // specific collision

					// 실행 시 비효율적이므로
					// Link 시 투사체를 왼쪽에 둘 것을 권장합니다.
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

	for (; iter != end; ++iter) // 가장 가까운 게 뭔지 hitpoint 기준으로 골라내는 작업
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

	if (target != nullptr) // target 제외하고 모두 지우는 작업
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

		// 기울기
		float slope = (endpos.y - startpos.y) / (endpos.x - startpos.x);

		// 절편
		float intercept = startpos.y - (startpos.x * slope);

		// 사각형 좌표
		float leftX = other->pos.x - other->size.x;
		float rightX = other->pos.x + other->size.x;
		float botY = other->pos.y - other->size.y;
		float topY = other->pos.y + other->size.y;

		// 시작점이 내부에 있음

		if (leftX < startpos.x && startpos.x < rightX && botY < startpos.y && startpos.y < topY)
		{
			return startpos; // hitpoint
		}

		// 사각형 좌측 테스트

		float leftResult = slope * leftX + intercept;
		if (botY < leftResult && leftResult < topY && startpos.x < leftX)
		{
			hitpoint.x = leftX;
			hitpoint.y = leftResult;
			return hitpoint;
		}

		// 사각형 우측 테스트
		float rightResult = slope * rightX + intercept;
		if (botY < rightResult && rightResult < topY && rightX < startpos.x)
		{
			hitpoint.x = rightX;
			hitpoint.y = rightResult;
			return hitpoint;
		}

		// 사각형 상단 테스트
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


		// 사각형 하단 테스트
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