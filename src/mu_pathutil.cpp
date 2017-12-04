#include "stdafx.h"
#include "mu_pathutil.h"


mu_pathutil::mu_pathutil()
{
}


mu_pathutil::~mu_pathutil()
{
}

std::string mu_pathutil::get_filename(std::string strPath)
{
	char* str = PathFindFileName(strPath.data());
	PathRemoveExtension(str);
	std::string strRet = str;
	return strRet;
}

std::string mu_pathutil::get_path(std::string strPath)
{
	char str_path[MAX_PATH] = "";
	memcpy(str_path, strPath.data(), strPath.length());
	PathRemoveFileSpec(str_path);
	return str_path;
}

bool mu_pathutil::get_file_write_time(std::string filePath, LPFILETIME pWirteTime)
{
	HANDLE hFile;
	FILETIME ftCreate, ftAccess;
	hFile = CreateFile(filePath.data(), GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	GetFileTime(hFile, &ftCreate, &ftAccess, pWirteTime);

	CloseHandle(hFile);
	return true;
}

bool mu_pathutil::set_file_write_time(std::string filePath, LPFILETIME pWirteTime)
{
	HANDLE hFile;
	FILETIME ftCreate, ftAccess, ftWrite;
	hFile = CreateFile(filePath.data(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite);
	SetFileTime(hFile, &ftCreate, &ftAccess, pWirteTime);
	CloseHandle(hFile);
	return true;
}
