#include "stdafx.h"
#include "PathMgr.h"


PathMgr::PathMgr()
{
	wchar_t ArrStr[256] = {};
	GetCurrentDirectoryW(sizeof(wchar_t) * 256, ArrStr);
	m_Root = ArrStr;

	m_ImageRoot = m_Root;
	int CutCount = (int)m_Root.find_last_of(L'\\', m_Root.size());
	m_Root.replace(CutCount, m_Root.size(), L"");
	m_ImageRoot.replace(CutCount, m_ImageRoot.size(), L"\\Image\\");
}


PathMgr::~PathMgr()
{
}


wchar_t* PathMgr::FindImage(const wchar_t* _ImageName)
{
	m_ImagePath = m_ImageRoot;
	m_ImagePath += _ImageName;
	return (wchar_t*)m_ImagePath.c_str();
}


wchar_t* PathMgr::Root()
{
	return (wchar_t*)m_Root.c_str();
}