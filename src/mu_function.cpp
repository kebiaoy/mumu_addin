#include "stdafx.h"
#include "mu_function.h"


mu_function::mu_function() :m_nIndexOffset(nullptr)
{
}


mu_function::~mu_function()
{

}

void mu_function::push_arg(mu_arg_ptr pArg)
{
	m_listArgs.push_back(pArg);
	pArg->set_arg_index(m_listArgs.size());
	if (m_stmMetaCall.str().empty())
	{
		m_stmMetaCall << pArg->get_static_metacall_str();
	}
	else
	{
		m_stmMetaCall << ", " << pArg->get_static_metacall_str();
	}
	m_stmMetaData << ", " << pArg->get_type_index();
	if (!m_strFormatArgs.empty())
		m_strFormatArgs += ", ";
	m_strFormatArgs += pArg->get_type() + " " + pArg->get_variable();
	m_strInterpretFormat += ", const_cast<void*>(reinterpret_cast<const void*>(&" + pArg->get_variable() + "))";
}

mu_arg_ptr mu_function::get_arg(int nIndex)
{
	return m_listArgs[nIndex];
}

int mu_function::get_arg_count()
{
	return m_listArgs.size();
}

void mu_function::set_return_type(std::string strReturnType)
{
	m_strReturnType = strReturnType;
}

std::string mu_function::get_return_type()
{
	return m_strReturnType;
}

void mu_function::set_name(std::string strName)
{
	m_strName = strName;
}

std::string mu_function::get_name()
{
	return m_strName;
}

int mu_function::get_name_index()
{
	return m_nNameIndex;
}

void mu_function::set_name_index(int nIndex)
{
	m_nNameIndex = nIndex;
}

void mu_function::set_func_index(int nFuncIndex)
{
	m_nFuncIndex = nFuncIndex;
}

int mu_function::get_func_index()
{
	if (m_nIndexOffset != nullptr)
	{
		return m_nFuncIndex + (*m_nIndexOffset);
	}
	return m_nFuncIndex;
}

std::string mu_function::get_static_metacall_str()
{
	std::stringstream ss;
	ss << "\t\tcase " << get_func_index() << ": _t->" << m_strName << "(" << m_stmMetaCall.str() << "); break;\n";
	return ss.str();
}

void mu_function::set_func_index_offset(int* offset)
{
	m_nIndexOffset = offset;
}

std::string mu_function::get_meta_data_str()
{
	std::stringstream ss;
	ss << get_name_index() << ", " << get_arg_count();
	if (!m_stmMetaData.str().empty())
	{
		ss << m_stmMetaData.str();
	}
	return ss.str();
}

std::string& mu_function::get_format_args()
{
	return m_strFormatArgs;
}

std::string& mu_function::get_interpret_format()
{
	return m_strInterpretFormat;
}

mu_arg::mu_arg()
{

}

mu_arg::~mu_arg()
{

}

std::string mu_arg::get_type()
{
	return m_strType;
}

void mu_arg::set_type(std::string strType)
{
	m_strType = strType;
}

std::string mu_arg::get_variable()
{
	return m_strVariable;
}

void mu_arg::set_variable(std::string strVariable)
{
	m_strVariable = strVariable;
}

void mu_arg::set_type_index(int nTypeIndex)
{
	m_nTypeIndex = nTypeIndex;
}

int mu_arg::get_type_index()
{
	return m_nTypeIndex;
}

void mu_arg::set_variable_index(int nVariabaleIndex)
{
	m_nVariableIndex = nVariabaleIndex;
}

int mu_arg::get_variable_index()
{
	return m_nVariableIndex;
}

void mu_arg::set_arg_index(int nArgIndex)
{
	m_nArgIndex = nArgIndex;
}

int mu_arg::get_arg_index()
{
	return m_nArgIndex;
}

std::string mu_arg::get_static_metacall_str()
{
	std::stringstream ss;
	std::string strType = get_type();
	for (size_t t = strType.length() - 1; t >= 0; --t)
	{
		if (strType[t] == '&')
		{
			strType.erase(t, 1);
			continue;
		}
		if (strType[t] == '*')
		{
			continue;
		}
		break;
	}
	ss << "(*reinterpret_cast< " << strType << "(*)>(_a[" << get_arg_index() << "]))";
	return ss.str();
}
