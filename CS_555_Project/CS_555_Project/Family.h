#pragma once

#include "Person.h"

struct Family
{
private:
	
public:
	std::vector<Person> Children;
	string Id;
	std::tm Marriage;
	std::tm Divorce;
	Person Husband;
	Person Wife;
	

	Family()
	{
	}

	bool AddChild(Person child)
	{
		Children.push_back(child);
		return true;
	}
};

