#pragma once
#include <string>
#include <vector>
#include <memory>
#include <sstream>

class mu_arg
{
public:
	mu_arg();
	~mu_arg();
public:
	std::string get_type();
	void set_type(std::string strType);

	std::string get_variable();
	void set_variable(std::string strVariable);

	void set_type_index(int nTypeIndex);
	int get_type_index();

	void set_variable_index(int nVariabaleIndex);

	int get_variable_index();

	void set_arg_index(int nArgIndex);

	int get_arg_index();

	std::string get_static_metacall_str();
private:
	std::string m_strType;
	std::string m_strVariable;
	int m_nTypeIndex;
	int m_nVariableIndex;
	int m_nArgIndex;
};
typedef std::shared_ptr<mu_arg> mu_arg_ptr;

class mu_function
{
public:
	mu_function();
	~mu_function();
public:
	std::string get_name();
	void set_name(std::string strName);
	
	void set_name_index(int nIndex);
	int get_name_index();

	std::string get_return_type();
	void set_return_type(std::string strReturnType);

	int get_arg_count();
	mu_arg_ptr get_arg(int nIndex);
	void push_arg(mu_arg_ptr pArg);

	void set_func_index(int nFuncIndex);

	int get_func_index();

	std::string get_static_metacall_str();

	void set_func_index_offset(int* offset);

	std::string get_meta_data_str();

	std::string& get_format_args();

	std::string& get_interpret_format();
private:
	std::string m_strName;
	std::string m_strReturnType;
	std::vector<mu_arg_ptr> m_listArgs;
	int m_nNameIndex;
	int m_nFuncIndex;
	std::stringstream m_stmMetaCall;
	std::stringstream m_stmMetaData;
	std::string m_strFormatArgs;
	std::string m_strInterpretFormat;
	int* m_nIndexOffset;
};

typedef std::shared_ptr<mu_function> mu_function_ptr;