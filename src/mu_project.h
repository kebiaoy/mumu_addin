#pragma once
#include <memory>
#include <vector>
#include "mu_source.h"
class mu_project
{
public:
	mu_project(EnvDTE::Project* pProjectImpl);
	~mu_project();
	void init();
	int get_source_count();
	mu_source_ptr get_source(int nIndex);
	std::string get_project_path();
	void add_generate_file(std::string strFileName);
private:
	void init_file(EnvDTE::ProjectItem* pItem);
private:
	std::vector<mu_source_ptr> m_listSources;
	EnvDTE::Project* m_pProjectImpl;
	EnvDTE::ProjectItem* m_pFilterItem;
	CComBSTR m_strFilterName;
};

typedef std::shared_ptr<mu_project> mu_project_ptr;

