#pragma once
#include <string>
class mu_pathutil
{
public:
	mu_pathutil();
	~mu_pathutil();
	static std::string get_filename(std::string strPath);
	static std::string get_path(std::string strPath);
	static bool get_file_write_time(std::string filePath, LPFILETIME pWirteTime);
	static bool set_file_write_time(std::string filePath, LPFILETIME pWirteTime);
};

