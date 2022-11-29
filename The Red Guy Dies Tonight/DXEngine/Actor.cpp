#include "stdafx.h"
#include "Actor.h"
#include "Com.h"


Actor::Actor()
{
}


Actor::~Actor()
{
}


Com* Actor::AddCom(wstring name, Com* com)
{
	com->pActor = this;
	comMap.insert(pair<wstring, Com*>(name, com));
	return com;
}
Com* Actor::AddCom(Com* com)
{
	return AddCom(L"None", com);
}
Com* Actor::GetCom(wstring name)
{
	auto iter = comMap.find(name);

	if (iter == comMap.end())
	{
		return nullptr;
	}
	else
	{
		return iter->second;
	}
}


void Actor::ComUpdate1()
{
	auto iter = comMap.begin();
	auto end = comMap.end();

	for (; iter != end; ++iter)
	{
		iter->second->Update1();
	}
}
void Actor::ComUpdate2()
{
	auto iter = comMap.begin();
	auto end = comMap.end();

	for (; iter != end; ++iter)
	{
		iter->second->Update2();
	}
}
void Actor::ComUpdate3()
{
	auto iter = comMap.begin();
	auto end = comMap.end();

	for (; iter != end; ++iter)
	{
		iter->second->Update3();
	}
}
void Actor::ComUpdate4()
{
	auto iter = comMap.begin();
	auto end = comMap.end();

	for (; iter != end; ++iter)
	{
		iter->second->Update4();
	}
}
void Actor::ComUpdate5()
{
	auto iter = comMap.begin();
	auto end = comMap.end();

	for (; iter != end; ++iter)
	{
		iter->second->Update5();
	}
}
void Actor::ComRelease()
{
	auto iter = comMap.begin();
	auto end = comMap.end();

	for (; iter != end; )
	{
		if (iter->second->bAlive == true)
		{
			++iter;
		}
		else
		{
			delete iter->second;
			iter = comMap.erase(iter);
		}
	}
}
void Actor::ComClear()
{
	auto iter = comMap.begin();
	auto end = comMap.end();

	for (; iter != end; ++iter)
	{
		delete iter->second;
	}

	comMap.clear();
}


void Actor::Move(VEC2 val)
{
	pos += val;

	auto comiter = comMap.begin();
	auto comend = comMap.end();

	for (; comiter != comend; ++comiter)
	{
		comiter->second->Move(val);
	}
}



void Actor::SetPos(VEC2 val)
{
	pos = val;

	auto comiter = comMap.begin();
	auto comend = comMap.end();

	for (; comiter != comend; ++comiter)
	{
		comiter->second->SetPos(val);
	}
}



void Actor::SetSize(VEC2 val)
{
	size = val;

	auto comiter = comMap.begin();
	auto comend = comMap.end();

	for (; comiter != comend; ++comiter)
	{
		comiter->second->SetSize(val);
	}
}