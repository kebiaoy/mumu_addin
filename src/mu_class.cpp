#include "stdafx.h"
#include "mu_class.h"


mu_class::mu_class() :m_nSignalCount(0)
{

}


mu_class::~mu_class()
{
}

void mu_class::set_name(std::string strName)
{
	m_strName = strName;
}

std::string mu_class::get_name()
{
	return m_strName;
}

int mu_class::get_signal_counts()
{
	return m_listSignals.size();
}

int mu_class::get_slot_counts()
{
	return m_listSlots.size();
}

mu_function_ptr mu_class::get_sinal(int nIndex)
{
	return m_listSignals[nIndex];
}

mu_function_ptr mu_class::get_slot(int nIndex)
{
	return m_listSlots[nIndex];
}

void mu_class::push_signal(mu_function_ptr pSignal)
{
	pSignal->set_func_index(m_listSignals.size());
	m_listSignals.push_back(pSignal);
	m_nSignalCount = m_listSignals.size();
}

void mu_class::push_slot(mu_function_ptr pSlot)
{
	pSlot->set_func_index(m_listSlots.size());
	m_listSlots.push_back(pSlot);
	pSlot->set_func_index_offset(&m_nSignalCount);
}

mu_function_ptr mu_class::signal_back()
{
	return m_listSignals.back();
}

mu_function_ptr mu_class::slot_back()
{
	return m_listSlots.back();
}

std::string mu_class::get_parent_name()
{
	return m_strParent;
}

void mu_class::set_parent_name(std::string strParent)
{
	m_strParent = strParent;
}

void mu_class::add_string_set(std::string& strRet, std::string strName)
{
	if (!strName.empty())
	{
		for (size_t it = 0; it < m_vStringData.size(); ++it)
		{
			if (m_vStringData[it] == strName)
			{
				return;
			}
		}
		m_vStringData.push_back(strName);
		strRet += "\t\"" + strName + "\",\n";
	}
}
std::string mu_class::get_string_data()
{
	return "";
}

int mu_class::get_string_data_count()
{
	return m_vStringData.size();
}

int mu_class::string_index(std::string str)
{
	for (size_t it = 0; it < m_vStringData.size(); ++it)
	{
		if (m_vStringData[it] == str)
		{
			return it;
		}
	}
	return -1;
}

std::string mu_class::get_type_string(int nIndex)
{
	if (nIndex < m_vTypeStringData.size())
	{
		return m_vTypeStringData[nIndex];
	}
	return "";
}

size_t mu_class::get_type_string_count()
{
	return m_vTypeStringData.size();
}

void mu_class::add_type_string(std::string strType)
{
	if (!strType.empty())
	{
		for (size_t it = 0; it < m_vTypeStringData.size(); ++it)
		{
			if (m_vTypeStringData[it] == strType)
			{
				return;
			}
		}
		m_vTypeStringData.push_back(strType);
	}
}

