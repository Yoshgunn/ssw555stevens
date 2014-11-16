#pragma once

#include "stdafx.h"

struct Person
{

public:

	string Id;
	string GivenName;
	string Surname;
	string Sex;
	std::tm Birth;
	std::tm Death;
	string LegalName;
	Person *Spouse;
	Person *Mother;
	Person *Father;
	// Person *Mother = NULL;		// Code can't run with these two lines, it fails
	// Person *Father = NULL;
};

