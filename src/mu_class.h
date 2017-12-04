#pragma once
#include <string>
#include <vector>
#include <memory>
#include "mu_function.h"

class mu_class
{
public:
	mu_class();
	~mu_class();
	
	void set_name(std::string strName);

	std::string get_name();

	int get_signal_counts();

	int get_slot_counts();

	mu_function_ptr get_sinal(int nIndex);

	mu_function_ptr get_slot(int nIndex);

	void push_signal(mu_function_ptr pSignal);

	void push_slot(mu_function_ptr pSlot);

	mu_function_ptr signal_back();

	mu_function_ptr slot_back();

	void set_parent_name(std::string strParent);

	std::string get_parent_name();

	std::string get_string_data();

	int get_string_data_count();

	int string_index(std::string str);

	std::string get_type_string(int nIndex);

	size_t get_type_string_count();
private:
	void add_string_set(std::string& strRet, std::string strName);
	void add_type_string(std::string strType);
private:
	std::string m_strName;
	std::string m_strParent;
	std::vector<mu_function_ptr> m_listSignals;
	std::vector<mu_function_ptr> m_listSlots;
	std::vector<std::string> m_vStringData;
	std::vector<std::string> m_vTypeStringData;
	int m_nSignalCount;
};

typedef std::shared_ptr<mu_class> mu_class_ptr;