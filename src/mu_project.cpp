#include "stdafx.h"
#include "mu_pathutil.h"
#include "mu_strutil.h"
#include "mu_project.h"
#include <assert.h>



mu_project::mu_project(EnvDTE::Project* pProjectImpl) :m_pProjectImpl(pProjectImpl)
, m_strFilterName("mumu_generate"), m_pFilterItem(NULL)
{

}


mu_project::~mu_project()
{
}

void mu_project::init()
{
	long nItemCount = 0;
	EnvDTE::ProjectItems* pProjectItems;
	m_pProjectImpl->get_ProjectItems(&pProjectItems);
	assert(pProjectItems != NULL);


	pProjectItems->AddFolder(m_strFilterName, CComBSTR(EnvDTE::vsProjectItemKindVirtualFolder), &m_pFilterItem);
	pProjectItems->get_Count(&nItemCount);
	if (FAILED(m_pProjectImpl->Save(NULL)))
	{
		int a;
		a = 5;
	}
	for (int i = 1; i <= nItemCount; ++i)
	{
		EnvDTE::ProjectItem* pItem;
		CComVariant t = i;
		pProjectItems->Item(t, &pItem);
		if (m_pFilterItem == NULL)
		{
			CComBSTR strName;
			pItem->get_Name(&strName);
			if (strName == m_strFilterName)
			{
				m_pFilterItem = pItem;
			}
		}
		init_file(pItem);
	}
}

void mu_project::init_file(EnvDTE::ProjectItem* pItem)
{
	long nItemCount = 0;
	EnvDTE::ProjectItems* pProjectItems;
	pItem->get_ProjectItems(&pProjectItems);
	assert(pProjectItems != NULL);
	pProjectItems->get_Count(&nItemCount);
	if (nItemCount == 0)
	{

		BSTR str;
		pItem->get_FileNames(0, &str);
		std::string strExtension = PathFindExtension(mu_strutil::w2s(CP_UTF8, str).data());
		if (strExtension == ".h")
		{
			mu_source_ptr pSource = mu_source_ptr(new mu_source(mu_strutil::w2s(CP_UTF8, str)));
			m_listSources.push_back(pSource);
		}
		return;
	}
	for (int i = 1; i <= nItemCount; ++i)
	{
		EnvDTE::ProjectItem* pItem;
		CComVariant t = i;
		pProjectItems->Item(t, &pItem);
		init_file(pItem);
	}
}

int mu_project::get_source_count()
{
	return m_listSources.size();
}

mu_source_ptr mu_project::get_source(int nIndex)
{
	return m_listSources[nIndex];
}

std::string mu_project::get_project_path()
{
	BSTR strPath;
	m_pProjectImpl->get_FullName(&strPath);
	return mu_pathutil::get_path(mu_strutil::w2s(CP_UTF8, strPath));
}

void mu_project::add_generate_file(std::string strFileName)
{
	if (m_pFilterItem)
	{
		long nItemCount = 0;
		EnvDTE::ProjectItems* pProjectItems;
		m_pFilterItem->get_ProjectItems(&pProjectItems);
		EnvDTE::ProjectItem* pItem;
		pProjectItems->get_Count(&nItemCount);
		for (int i = 1; i <= nItemCount; ++i)
		{
			CComVariant t = i;
			pProjectItems->Item(t, &pItem);
			if (pItem != NULL)
			{
				CComBSTR strTFileName;
				pItem->get_FileNames(0, &strTFileName);
				if (strFileName == mu_strutil::w2s(CP_UTF8, strTFileName.m_str))
				{
					return;
				}
			}
		}
		pProjectItems->AddFromFileCopy(CComBSTR(strFileName.data()), &pItem);
	}
}
