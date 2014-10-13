#pragma once

#include "stdafx.h"

struct Person
{

public:

	string Id;
	string GivenName;
	string Surname;
	string Sex;
	string Birth;
	string Death;
	string LegalName;
	Person *Spouse;
	Person *Mother;
	Person *Father;
};

