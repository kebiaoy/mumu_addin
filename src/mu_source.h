#pragma once
#include <memory>
#include <string>
#include <vector>
#include "mu_class.h"
class mu_source
{
public:
	mu_source(std::string strSourceFile);
	~mu_source();
	int get_class_count();
	mu_class_ptr get_class(int nIndex);
	std::string get_file_path();
	int get_string_data_count();
	std::string get_string_data();
private:
	enum class_state
	{
		class_idel,
		start
	};
	enum parent_state
	{
		parent_state_idel,
		parent_state_start
	};
	enum analysis_state
	{
		analysis_idel,
		get_class_name,
		get_parent_class,
		get_signal,
		get_slot
	};
	enum function_state
	{
		get_return_type,
		get_function_name,
		get_args
	};
private:
	void init();
	void get_key_word(std::string& key, const char* data, size_t& cur_pos, size_t& total_size);
	void get_function(mu_function_ptr pFunc, std::string& key, size_t& cur_pos);
	int add_string_data(std::string& str);
private:
	std::string m_strSourceFile;
	class_state m_nClassState;
	analysis_state m_nAnalysisState;
	function_state m_nFunctionState;
	parent_state m_nParentState;
	int m_nClassEndCount;
	std::vector<mu_class_ptr> m_listClasses;
	size_t m_nCurPos;
	std::string m_strArgType;
	std::string m_strVariable;
	std::vector<std::string> m_strStringData;
};

typedef std::shared_ptr<mu_source> mu_source_ptr;

