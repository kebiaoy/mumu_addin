#include "stdafx.h"
#include "mu_solution.h"
#include "mu_strutil.h"
#include <assert.h>

mu_solution::mu_solution(EnvDTE::_Solution* pSolutionImpl) :m_pSolutionImpl(pSolutionImpl)
{

}


mu_solution::~mu_solution()
{
}


mu_project_ptr mu_solution::get_project(std::string strProjectName)
{
	long nProjectCount = 0;
	m_pSolutionImpl->get_Count(&nProjectCount);
	
	for (int i = 1; i <= nProjectCount; ++i)
	{
		CComVariant t = i;
		EnvDTE::Project* pProjectImpl;
		m_pSolutionImpl->Item(t, &pProjectImpl);
		assert(m_pSolutionImpl != NULL);
		BSTR strName;
		pProjectImpl->get_Name(&strName);
		if (strProjectName == mu_strutil::w2s(CP_UTF8, strName))
		{
			mu_project_ptr pRetProject = mu_project_ptr(new mu_project(pProjectImpl));
			pRetProject->init();
			return pRetProject;
		}
	}
	return NULL;
}
