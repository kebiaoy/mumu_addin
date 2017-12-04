#pragma once
#include <memory>
#include <string>
#include "mu_project.h"
class mu_solution
{
public:
	mu_solution(EnvDTE::_Solution* pSolutionImpl);
	~mu_solution();
public:
	mu_project_ptr get_project(std::string strProjectName);
private:
	EnvDTE::_Solution* m_pSolutionImpl;
};
typedef std::shared_ptr<mu_solution> mu_solution_ptr;

