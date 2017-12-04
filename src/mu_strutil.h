#pragma once

#include <string>

class mu_strutil
{
public:
	mu_strutil();
	~mu_strutil();
	static std::string w2s(UINT nCodePage, std::wstring strW);
};

