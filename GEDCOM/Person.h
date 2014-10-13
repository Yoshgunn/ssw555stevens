#pragma once

#include "stdafx.h"

struct Person
{
public:
	
	string GivenName;
	string Surname;
	string Sex;
	string Birth;
	string Death;
	string Name;
	Person *Spouse;
	Person *Mother;
	Person *Father;
};

