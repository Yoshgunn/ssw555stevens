#pragma once

#include "Person.h"

struct Family
{
public:
	string Id;
	Person Husband;
	Person Wife;
	std::vector<Person> Children;

};

