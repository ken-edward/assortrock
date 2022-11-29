#pragma once
#include <string>
class PathMgr
{
public:
	static PathMgr& Inst()
	{
		static PathMgr PathInst = PathMgr();
		return PathInst;
	}

private:
	std::wstring m_Root;
	std::wstring m_ImageRoot;
	std::wstring m_ImagePath;

public:
	wchar_t* FindImage(const wchar_t* _ImageName);
	wchar_t* Root();

private:
	PathMgr();
	~PathMgr();
};