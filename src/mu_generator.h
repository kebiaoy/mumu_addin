#pragma once
#include <memory>
#include "mu_project.h"


class mu_generator
{
public:
	mu_generator(mu_project_ptr pProject);
	~mu_generator();
	void generate();
private:
	mu_project_ptr m_pProject;
};

typedef std::shared_ptr<mu_generator> mu_generator_ptr;
