#pragma once

#include "Person.h"

struct Family
{
public:
	string Id;
	string Marriage;
	Person Husband;
	Person Wife;
	std::vector<Person> Children;

};

