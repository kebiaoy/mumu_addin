#include "stdafx.h"
#include <fstream>
#include <sstream>
#include "mu_source.h"

mu_source::mu_source(std::string strSourceFile) :m_strSourceFile(strSourceFile),
m_nClassState(class_state::class_idel), m_nAnalysisState(analysis_state::analysis_idel)
{
	init();
}


mu_source::~mu_source()
{

}

void mu_source::init()
{
	std::ifstream file(m_strSourceFile);
	std::stringstream ss;
	ss << file.rdbuf();
	std::string strContent = ss.str();
	const char* strData = strContent.data();
	std::string strKeyWord;
	size_t nToalLength = strContent.length();
	for (size_t i = 0; i < nToalLength; ++i)
	{
		char cNext = -1;
		if ((i + 1) <= nToalLength)
		{
			cNext = strData[i + 1];
		}

		switch (strData[i])
		{
			case ' ':
				get_key_word(strKeyWord, strData, i, nToalLength);
				strKeyWord = "";
				for (i = i + 1; i < nToalLength; ++i)
				{
					if (strData[i] != ' ')
					{
						i = i - 1;
						break;
					}
				}
				break;
			case '\t':
				get_key_word(strKeyWord, strData, i, nToalLength);
				strKeyWord = "";
				for (i = i + 1; i < nToalLength; ++i)
				{
					if (strData[i] != '\t')
					{
						i = i - 1;
						break;
					}
				}
				break;
			case '/':
				if (cNext == ' /')
				{
					for (i = i + 2; i < nToalLength; ++i)
					{
						if (strData[i] == '\n')
							break;
					}
				}
				else if (cNext == '*')
				{
					for (i = i + 2; i < nToalLength; ++i)
					{
						char cNext = -1;
						if ((i + 1) <= nToalLength)
						{
							cNext = strData[i + 1];
						}
						if (strData[i] == '*' && cNext == '/')
						{
							i = i + 1;
							break;
						}

					}
				}
				break;
			case '\n':
				get_key_word(strKeyWord, strData, i, nToalLength);
				strKeyWord = "";
				break;
			default:
				strKeyWord += strData[i];
				break;
		}
	}
}

void mu_source::get_key_word(std::string& key, const char* data, size_t& cur_pos, size_t& total_size)
{
	if (key.empty())
	{
		return;
	}
	size_t i = 0;
	switch (m_nClassState)
	{
		case class_state::class_idel:
			if (key == "class")
			{
				m_nClassState = class_state::start;
				m_nAnalysisState = mu_source::get_class_name;
				m_nClassEndCount = 0;
				m_listClasses.push_back(mu_class_ptr(new mu_class));
			}
			break;
		case class_state::start:
			switch (m_nAnalysisState)
			{
				case mu_source::analysis_idel:
					if (key == "MU_SIGNALS:")
					{
						m_nAnalysisState = mu_source::get_signal;
						m_nFunctionState = get_return_type;
						m_nCurPos = cur_pos;
					}
					else if (key == "MU_SLOTS:")
					{
						m_nAnalysisState = mu_source::get_slot;
						m_nFunctionState = get_return_type;
						m_nCurPos = cur_pos;
					}
					break;
				case mu_source::get_parent_class:
					switch (m_nParentState)
					{
						case mu_source::parent_state_idel:
							if (key != "public" && key != "private" && key != "protect")
							{
								if (!m_listClasses.empty())
								{
									m_listClasses.pop_back();
								}
								m_nClassState = class_state::class_idel;
							}
							else
							{
								m_nParentState = mu_source::parent_state_start;
							}
							break;
						case mu_source::parent_state_start:
							if (!m_listClasses.empty())
							{
								if (key.find(',') != std::string::npos)
								{
									key.erase(key.find(','));
								}
								m_listClasses.back()->set_parent_name(key);
							}
							m_nAnalysisState = mu_source::get_class_name;
							break;
						default:
							break;
					}
					break;
				case mu_source::get_class_name:
					if (key.find(':') != std::string::npos)
					{
						key.erase(key.find(':'));
						m_nAnalysisState = mu_source::get_parent_class;
					}
					if (!m_listClasses.empty() && m_listClasses.back()->get_name().empty())
					{
						m_listClasses.back()->set_name(key);
					}

					if (key.find('{') != std::string::npos)
					{
						m_nAnalysisState = mu_source::analysis_idel;
					}
					else if (key.find(';') != std::string::npos)
					{
						if (!m_listClasses.empty())
						{
							m_listClasses.pop_back();
						}
						m_nClassState = class_state::class_idel;
					}
					break;
				case mu_source::get_slot:
					if (m_nFunctionState == mu_source::get_return_type)
					{
						if (key != "void")
						{
							cur_pos = m_nCurPos;
							m_nAnalysisState = mu_source::analysis_idel;
						}
						else
						{
							m_listClasses.back()->push_slot(mu_function_ptr(new mu_function));
							m_listClasses.back()->slot_back()->set_return_type(key);
							m_nFunctionState = mu_source::get_function_name;

						}
					}
					else
					{
						get_function(m_listClasses.back()->slot_back(), key, cur_pos);
					}
					break;
				case mu_source::get_signal:
					if (m_nFunctionState == mu_source::get_return_type)
					{
						if (key != "void")
						{
							cur_pos = m_nCurPos;
							m_nAnalysisState = mu_source::analysis_idel;
						}
						else
						{
							m_listClasses.back()->push_signal(mu_function_ptr(new mu_function));
							m_listClasses.back()->signal_back()->set_return_type(key);
							m_nFunctionState = mu_source::get_function_name;

						}
					}
					else
					{
						get_function(m_listClasses.back()->signal_back(), key, cur_pos);
					}
					break;
				default:
					break;
			}

			for (i = 0; i < key.length(); ++i)
			{
				if (key[i] == '{')
				{
					m_nClassEndCount++;
				}
				else if (key[i] == '}')
				{
					m_nClassEndCount--;
					if (m_nClassEndCount == 0)
					{
						m_nClassState = class_state::class_idel;
					}
				}
			}
			break;
		default:
			break;
	}
}

int mu_source::get_class_count()
{
	return m_listClasses.size();
}

mu_class_ptr mu_source::get_class(int nIndex)
{
	return m_listClasses[nIndex];
}

std::string mu_source::get_file_path()
{
	return m_strSourceFile;
}

void mu_source::get_function(mu_function_ptr pFunc, std::string& key, size_t& cur_pos)
{
	switch (m_nFunctionState)
	{
		case mu_source::get_function_name:
		{
											 std::string strFuncName;
											 for (size_t i = 0; i < key.length(); ++i)
											 {
												 if (key[i] == '(')
												 {
													 cur_pos -= (key.length() - i + 1);
													 break;
												 }
												 strFuncName += key[i];
											 }
											 pFunc->set_name(strFuncName);
											 pFunc->set_name_index(add_string_data(strFuncName));
											 m_nFunctionState = mu_source::get_args;
		}
			break;
		case mu_source::get_args:
		{
									if (key[0] == '(')
									{
										key.erase(0, 1);
									}
									if (key.empty())
									{
										return;
									}
									if (key[0] == ')')
									{
										m_nFunctionState = mu_source::get_return_type;
										return;
									}
									std::string strVariable;
									bool bEnd = false;
									if (!m_strVariable.empty())
									{
										if (!m_strArgType.empty() && m_strArgType.back() != '=')
										{
											m_strArgType += " ";
										}
										if (m_strArgType.empty())
										{
											m_strArgType += m_strVariable;
										}
										else if (m_strArgType.back() != '=')
										{
											m_strArgType += m_strVariable;
										}

									}
									for (size_t i = 0; i < key.length(); ++i)
									{
										if (key[i] == ',' || key[i] == ')')
										{
											cur_pos -= (key.length() - i);
											mu_arg_ptr pArg = mu_arg_ptr(new mu_arg);
											if (m_strArgType.back() == '=')
											{
												m_strArgType.pop_back();
												strVariable = m_strVariable;
											}
											for (size_t v = 0; v < strVariable.length(); ++v)
											{
												if (strVariable[v] == '*' || strVariable[v] == '&')
												{
													m_strArgType += strVariable[v];
													strVariable.erase(0, 1);
													continue;
												}
												break;
											}
											std::string strTemp;
											while (!m_strArgType.empty())
											{
												if (m_strArgType.back() == '*' || m_strArgType.back() == '&')
												{
													strTemp = strTemp + m_strArgType.back();
													m_strArgType.pop_back();
													continue;
												}
												else if (m_strArgType.back() == ' ')
												{
													m_strArgType.pop_back();
													continue;
												}
												break;
											}
											m_strArgType += strTemp;
											pArg->set_type(m_strArgType);
											pArg->set_variable(strVariable);
											pArg->set_type_index(add_string_data(m_strArgType));
											pArg->set_variable_index(add_string_data(strVariable));
											pFunc->push_arg(pArg);
											m_strVariable = "";
											m_strArgType = "";
											bEnd = true;
											break;
										}
										else if (key[i] == ';')
										{
											m_nFunctionState = mu_source::get_return_type;
											m_strVariable = "";
											m_strArgType = "";
											bEnd = true;
											break;
										}
										else if (key[i] == '=')
										{
											if (strVariable.empty())
											{
												strVariable = m_strVariable;
												for (size_t arg = 0; arg < m_strVariable.length(); ++arg)
												{
													m_strArgType.pop_back();
												}
											}
											m_strArgType += key[i];
											continue;
										}
										if (!m_strArgType.empty() && m_strArgType.back() == '=')
										{
											strVariable = m_strVariable;
											continue;
										}
										strVariable += key[i];
									}
									if (!bEnd)
									{
										m_strVariable = strVariable;
									}
		}
			break;
		default:
			break;
	}
}

int mu_source::add_string_data(std::string& str)
{
	int i = 0;
	for (; i < m_strStringData.size(); ++i)
	{
		if (m_strStringData[i] == str)
		{
			return i;
		}
	}
	m_strStringData.push_back(str);
	return i;
}

int mu_source::get_string_data_count()
{
	return m_strStringData.size();
}

std::string mu_source::get_string_data()
{
	std::string retStr;
	for (int i = 0; i < get_string_data_count(); ++i)
	{
		if (i == 0)
		{
			retStr += "\t\"" + m_strStringData[i] + "\"";
			continue;
		}
		retStr += ",\n\t\"" + m_strStringData[i] + "\"";
	}
	return retStr;
}

std::string mu_source::get_file_name()
{
	char buf[MAX_PATH] = "";
	memcpy(buf, m_strSourceFile.data(), m_strSourceFile.size());
	PathStripPath(buf);
	PathRemoveExtension(buf);
	return buf;
}
