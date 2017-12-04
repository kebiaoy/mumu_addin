#include "stdafx.h"
#include "mu_strutil.h"


mu_strutil::mu_strutil()
{
}


mu_strutil::~mu_strutil()
{
}

std::string mu_strutil::w2s(UINT nCodePage, std::wstring strW)
{
	char* strA = new char[strW.length() + 1];
	memset(strA, 0, strW.length() + 1);
	std::string strRet;
	WideCharToMultiByte(CP_UTF8, 0, strW.data(), strW.length(), strA, strW.length(), 0, 0);
	strRet = strA;
	delete[]strA;
	return strRet;
}
