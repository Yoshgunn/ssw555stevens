// CS_555_Project.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Family.h"



int CompareDates(std::tm first,std::tm second)
{
	// 1	First came later
	// 0	Same date
	// -1	First came before
	
	// Special Death case
	if ( first.tm_year == -858993460 )
	{
		// This event did not happen, so it is infinitely later.
		return 1;
	}
	else if ( second.tm_year == -858993460 )
	{		
		// Same thing
		return 1;
	}


	if((first.tm_year == second.tm_year) && (first.tm_mon == second.tm_mon) && (first.tm_mday == second.tm_mday))
	{
		cout << "Same date." << endl;
		return 0;
	}
	else if( first.tm_year > second.tm_year )
		return 1;
	else if ( first.tm_year < second.tm_year )
	{
		// cout << first.tm_year << " is before " << second.tm_year << endl;
		return -1;
	}
	else if ( first.tm_mon > second.tm_mon )
		return 1;
	else if ( first.tm_mon < second.tm_mon )
		return -1;
	else if ( first.tm_mday > second.tm_mday )
		return 1;
	else
		return -1;
}
string replace(string line,string textToBeReplaced, int length,string newText)
{
	int pos = line.find( textToBeReplaced );
	if ( pos != string::npos ) {
		return line.replace(pos, length, newText);   // 5 = length( $name )
	}
	return line;
}
string replace(string line,string textToBeReplaced, int length)
{
	int pos = line.find( textToBeReplaced );
	if ( pos != string::npos ) {
		return line.replace(0, pos+length, "");   // 5 = length( $name )
	}
	return "";
}
std::tm try_get_date(const std::string& s)
{
		string newSt = replace(s,"JAN",3,"Jan");
		newSt = replace(newSt,"FEB",3,"Feb");
		newSt = replace(newSt,"MAR",3,"Mar");
		newSt = replace(newSt,"APR",3,"Apr");
		newSt = replace(newSt,"MAY",3,"May");
		newSt = replace(newSt,"JUN",3,"Jun");
		newSt = replace(newSt,"JUL",3,"Jul");
		newSt = replace(newSt,"AUG",3,"Aug");
		newSt = replace(newSt,"SEP",3,"Sep");
		newSt = replace(newSt,"OCT",3,"Oct");
		newSt = replace(newSt,"NOV",3,"Nov");
		newSt = replace(newSt,"DEC",3,"Dec");


		std::tm t;
		std::istringstream ss(newSt);
		ss.imbue(std::locale(""));
		//ss >> std::get_time(&t, "%Y-%b-%d %H:%M:%S");
		ss >> std::get_time(&t, "%d %b %Y");
		 //std::cout << std::put_time(&t, "%Y %b %d") << '\n';
		return t;
}
Person CreatePerson()
{
	Person pe = Person();
	pe.Father = &Person();
	pe.Father->Id = "0";
	pe.Mother = &Person();
	pe.Mother->Id = "0";
	return pe;
}
std::vector<Person> GetPeople()
{
	string line;
	// ifstream myfile("C:\\Users\\Dipan\\Documents\\GitHub\\ssw555stevens\\CS_555_Project\\CS_555_Project\\Debug\\AcceptanceTestFile.ged",ios::in);
	ifstream myfile("Debug\\AcceptanceTestFile.ged", ios::in);

	if (myfile.is_open())
	{
		Person p;
		std::vector<Person> people;
		int totalPep=0;
		while (getline(myfile,line) )
		{
			if(line.substr(0,4) == "0 @I")
			{
				if(totalPep >0)
				people.push_back(p);
				p=CreatePerson();
				totalPep++;
			}
			//cout << "Original Line: \"" << line <<"\"";
			string buf; // Have a buffer string
			stringstream ss(line); // Insert the string into a stream

			vector<string> tokens; // Create vector to hold our words
			int index =0;
			while (ss >> buf && index <2)
			{
				if( index ==1)
				{
					if(buf =="NAME")
					{
						string a =replace(line,"NAME",5);
						p.LegalName =a;
					}
					else if( buf.substr(0,2) =="@I")
					{
						p.Id = buf;
						p.Father = NULL;		// Experimental
						p.Mother = NULL;
					}
					else if( buf =="GIVN")
					{
						ss >> p.GivenName;
					}
					else if( buf =="SURN")
					{
						ss >> p.Surname;
					}
					else if( buf =="SEX")
					{
						ss >> p.Sex;
					}
					else if( buf =="BIRT")
					{
						getline(myfile,line);
						//std::tm ds = try_get_date("1 MAR 1985");
						std::tm ds = try_get_date(replace(line,"DATE",5));
						p.Birth = ds;
						//ss >> p.Birth;
					}
					else if( buf =="DEAT")
					{
						getline(myfile,line);
						std::tm ds = try_get_date(replace(line,"DATE",5));
						p.Death = ds;
						//ss >> p.Death;
					}
					else if( buf =="FAMC")
					{
					}

				}
				/*else
					cout << buf;*/
				index++;
			}
		}
		people.push_back(p);
		myfile.close();
		return people;
	}
	else
	{
		cout << "Couldn't find the requested file." << endl;
	}
}
Person *FindPerson(string id, std::vector<Person> &people)
{
	 for (std::vector<Person>::iterator it = people.begin() ; it != people.end(); ++it)
	 {
		 if((*it).Id== id)
			 return &*it;
	 }
}

/*
*	US-12: Everyone have unique id
*/
void checkIds(std::vector<Person> &people) {
	int count = 0;
	string uids[100];

	for (std::vector<Person>::iterator it = people.begin() ; it != people.end(); ++it) {
		bool duplicate = false;

		for(int i = 0; !duplicate && i < count; i++) {
			if((*it).Id == uids[i]) {
				cout << "US-12 ERROR: Duplicate ID (" << (*it).Id << ") found!\n" << endl;
				duplicate = true;
			}
		}

		if(!duplicate) {
			uids[count] = (*it).Id;
			count++;
		}
	}
}

bool FindParent(std::vector<Family> &families, string id,Person& peo, bool findFather)
{
	if(id!="")
	{
		for (std::vector<Family>::const_iterator it = families.begin(); it!=families.end(); ++it) 
		{
			for (std::vector<Person>::const_iterator itp = (*it).Children.begin(); itp!=(*it).Children.end(); ++itp) 
			{
				if((*itp).Id == id)
				{
					if(findFather)
						peo = ((*it).Husband);
					else
						peo = ((*it).Wife);
					return true;
				}
			}
		}
	}
	return false;
}
std::vector<Family> GetFamilies(std::vector<Person> &people, ofstream &output)
{
	string line;
	int linenum = 0;
	 //ifstream myfile("C:\\Users\\Dipan\\Documents\\GitHub\\ssw555stevens\\CS_555_Project\\CS_555_Project\\Debug\\AcceptanceTestFile.ged", ios::in);
	ifstream myfile("Debug\\AcceptanceTestFile.ged", ios::in);

	if (myfile.is_open())
	{
		Family f;
		std::vector<Family> familyColl;
		int totalPep=0;
		while (getline(myfile,line) )
		{
			linenum++;
			if(line.substr(0,4) == "0 @F")
			{
				if(totalPep >0)
				{
					familyColl.push_back(f);
				}
				f=Family();
				f.linenum = linenum;
				totalPep++;
			}
			//cout << "Original Line: \"" << line <<"\"";
			string buf; // Have a buffer string
			stringstream ss(line); // Insert the string into a stream

			vector<string> tokens; // Create vector to hold our words
			int index =0;
			while (ss >> buf && index <2)
			{
				if( index == 1)
				{
					if( buf.substr(0,2) =="@F")
					{
						f.Id = buf;
						f.Children.clear();
						f.Marriage.tm_year = 0;
						f.Marriage.tm_mon = 0;
						f.Marriage.tm_mday = 0;
						f.Divorce.tm_year = 0;
						f.Divorce.tm_mon = 0;
						f.Divorce.tm_mday = 0;
					}
					else if(buf =="HUSB")
					{
						string id;
						ss >> id;
						if (f.Husband.Id != "")		// this is not REDUNDANT, it is US-16
						{
							output << "Family Id : " <<f.Id <<endl;
							output << "US-16 ERROR: The number of husband is more than one, the other is: " << FindPerson(id, people)->GivenName<< "!\t Line " << linenum << endl;
							output << endl;
						}

						else 
						f.Husband = *FindPerson(id,people);
						//ss >> f.GivenName;
					}
					else if(buf =="WIFE")
					{
						string id;
						ss >> id;
						if (f.Wife.Id != "")
						{
							output << "Family Id : " << f.Id << endl;
							output << "US-16 ERROR: The number of wife is more than one, the other is: " << FindPerson(id, people)->GivenName << "!\t Line " << linenum << endl;
							output << endl;
						}// US-16
						else 
						f.Wife = *FindPerson(id,people);
					}
					else if( buf =="CHIL")
					{
						string id;
						ss >> id;
						auto child = FindPerson(id, people);
						child->Mother = &f.Wife;
						child->Father = &f.Husband;
						f.AddChild(*child);
					}
					else if( buf =="MARR")
					{
						getline(myfile, line);
						linenum++;
						std::tm ds = try_get_date(line.substr(7));
						f.Marriage = ds;
					}
					else if ( buf =="DIV")
					{
						getline(myfile, line);
						linenum++;
						std::tm ds = try_get_date(line.substr(7));
						f.Divorce = ds;
					}
				}
				index++;
			}
		}
		familyColl.push_back(f);
		myfile.close();
		return familyColl;
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	std::string str[] = { "INDI","NAME","SEX","BIRT","DEAT","FAMC","FAMS","FAM","MARR","HUSB","WIFE","CHIL","DIV","DATE","TRLR","NOTE"};
	vector< std::string > validTags( str, str + ( sizeof ( str ) /  sizeof ( std::string ) ) );

	///Test compare date function

	/*switch(CompareDates(try_get_date("1 Jan 2014"),try_get_date("1 Jan 2014")))
		{case 1:
			cout << "2 Jan 2014 is greater than 1 Jan 2014";
			break;
		case 0:
			cout << "1 Jan 2014 is equal to 1 Jan 2014";
			break;
		case -1:
			cout << "1 Jan 2014 is less than 2 Jan 2014";
			break;

	}*/
	ofstream output("TestResults.txt");

	std::vector<Person> people = GetPeople();
		
	checkIds(people);

	std::vector<Family> families = GetFamilies(people,output);

	for (std::vector<Family>::const_iterator it = families.begin(); it!=families.end(); ++it) 
	{
		output << "Family Id : " << (*it).Id << "\n";
		if (it->Husband.Id != "")
		{
			if ((*it).Husband.Sex == (*it).Wife.Sex)
				output << "US-15 ERROR: Both spouses are the same gender!" << "\t\t\t Line " << (*it).linenum << endl; 
			
			cout << "\t Husband Name : " << (*it).Husband.GivenName << "\n";
			output << "\t Husband Name : " << (*it).Husband.GivenName << "\n";

			if ((*it).Husband.Sex != "M")
				output << "US-13 ERROR: Husbands must be Male!" << "\t\t\t\t Line " << (*it).linenum << endl;

			// Husband's Birth
			std::tm husbBirth = (*it).Husband.Birth;
			if (husbBirth.tm_year != 0 && husbBirth.tm_mon != 0 && husbBirth.tm_mday != 0)
			{
				cout << "\t Husband Birth : " << put_time(&husbBirth, "%d %b %Y") << "\n";
				output << "\t Husband Birth : " << put_time(&husbBirth, "%d %b %Y") << "\n";

				// Check if born before father
				// Cannot access "grandfather" without crashing the program.
			}

			// Husband's Death
			std::tm husbDeath = (*it).Husband.Death;
			if (husbDeath.tm_year >= 0 && husbDeath.tm_mon >= 0 && husbDeath.tm_mday >= 0)
			{
				cout << "\t Husband Death : " << put_time(&husbDeath, "%d %b %Y") << "\n";
				output << "\t Husband Death : " << put_time(&husbDeath, "%d %b %Y") << "\n";

				// Check if died before born
				if (CompareDates(husbBirth, husbDeath) == 1)
					output << "Line: " << (*it).linenum << "\t " << "US-04 ERROR: Husband's death occurs before his birth!" << endl;
			}
		}
		if (it->Husband.Id != "")
		{
			cout << "\t Wife Name : " << (*it).Wife.GivenName << "\n";
			output << "\t Wife Name : " << (*it).Wife.GivenName << "\n";

			if ((*it).Wife.Sex != "F")
				output << "US-14 ERROR: Wives must be Female!" << "\t\t\t\t Line " << (*it).linenum << endl;

			// Wife's Birth
			std::tm wifeBirth = (*it).Wife.Birth;
			if (wifeBirth.tm_year != 0 && wifeBirth.tm_mon != 0 && wifeBirth.tm_mday != 0)
			{
				cout << "\t Wife Birth : " << put_time(&wifeBirth, "%d %b %Y") << "\n";
				output << "\t Wife Birth : " << put_time(&wifeBirth, "%d %b %Y") << "\n";
			}

			// Wife's Death
			std::tm wifeDeath = (*it).Wife.Death;
			if (wifeDeath.tm_year >= 0 && wifeDeath.tm_mon >= 0 && wifeDeath.tm_mday >= 0)
			{
				cout << "\t Wife Death : " << put_time(&wifeDeath, "%d %b %Y") << "\n";
				output << "\t Wife Death : " << put_time(&wifeDeath, "%d %b %Y") << "\n";

					// Check if died before born
					if (CompareDates(wifeBirth, wifeDeath) == 1) 
						output << "US-04 ERROR: Wife's death occurs before her birth!" << "\t\t Line " << (*it).linenum << endl; 
			}
		}
		// Check for siblings married to one another
		if ((it->Husband.Mother) && (it->Husband.Mother == it->Wife.Mother) || (it->Husband.Father) && (it->Husband.Father == it->Wife.Father))
			output << "US-17 ERROR: Marriage between siblings!" << "\t\t\t\t Line " << (*it).linenum << endl; 

		// Check for Marriages
		std::tm marr = (*it).Marriage;
		if ( marr.tm_year != 0 && marr.tm_mon != 0 && marr.tm_mday != 0 )
		{
			cout << "\t Date Married : " << put_time(&marr, "%d %b %Y") << "\n";
			output << "\t Date Married : " << put_time(&marr, "%d %b %Y") << "\n";

			// Marriage Anomaly?
			//    Marriage after one of the spouses died
			if ( CompareDates( (*it).Husband.Death, marr ) == -1 )
				output << "US-06 ERROR: Husband's death occurs before his marriage!" << "\t\t Line " << (*it).linenum << endl; 

			if ( CompareDates( (*it).Wife.Death, marr ) == -1 )
				output << "US-06 ERROR: Wife's death occurs before her marriage!" << "\t\t Line " << (*it).linenum << endl; 


			//    Marriage before one of the spouses was born
			if ( CompareDates( marr, (*it).Husband.Birth ) == -1 )
				output << "US-07 ERROR: Husband married before he was born!" << "\t\t Line " << (*it).linenum << endl; 

			if ( CompareDates( marr, (*it).Wife.Birth ) == -1 )
				output << "US-07 ERROR: Wife married before she was born!" << "\t\t\t Line " << (*it).linenum << endl; 

		}

		// Check for Divorces
		std::tm div = (*it).Divorce;
		if ( div.tm_year != 0 && div.tm_mon != 0 && div.tm_mday != 0 )
		{
			cout << "\t Date Divorced : " << put_time(&div, "%d %b %Y") << "\n";
			output << "\t Date Divorced : " << put_time(&div, "%d %b %Y") << "\n";

			// Divorce Anomaly?
			//	Check for Divorce before marriage
			if ( CompareDates( div, marr ) == -1 )
				output << "US-10 ERROR: Divorce occurs before marriage!" << "\t\t\t Line " << (*it).linenum << endl; 
		}
		if (it->Children.size() >= 10)
			output << "US-18 ERROR: This family has more than 10 children!" << "\t\t Line " << (*it).linenum << endl; 

		for (std::vector<Person>::const_iterator itp = (*it).Children.begin(); itp!=(*it).Children.end(); ++itp) 
		{
			cout << "\t\t Child Name : " << (*itp).GivenName << "\n";
			output << "\t\t Child Name : " << (*itp).GivenName << "\n";

			std::tm childBirth = (*itp).Birth;
			if ( childBirth.tm_year != 0 && childBirth.tm_mon != 0 && childBirth.tm_mday != 0 )
			{
				cout << "\t\t Child Birth : " << put_time(&childBirth, "%d %b %Y") << "\n";
			}

			std::tm childDeath = (*itp).Death;
			if ( childDeath.tm_year >= 0 && childDeath.tm_mon >= 0 && childDeath.tm_mday >= 0 )
			{
				cout << "\t\t Child Death : " << put_time(&childDeath, "%d %b %Y") << "\n";

				if( CompareDates( childBirth, childDeath ) == 1 ) 
					cout << "US-14 ERROR: Child's death occurs before birth!" << "\t\t\t Line " << (*it).linenum << endl;
				
			}

			// Check if born before father's birth
			if (CompareDates(childBirth, (*it).Husband.Birth) == -1)
				output << "US-02 ERROR: Child's birth occurs before father's birth!" << "\t Line " << (*it).linenum << endl; 

			// Check if born before mother's birth
			if (CompareDates(childBirth, (*it).Wife.Birth) == -1)
				output << "US-03 ERROR: Child's birth occurs before mother's birth!" << "\t Line " << (*it).linenum << endl; 

			// Check if born after mother's death
			if (CompareDates(childBirth, (*it).Wife.Death) == 1 && (*it).Wife.Death.tm_year >= 0 && (*it).Wife.Death.tm_mon >= 0 && (*it).Wife.Death.tm_mday >= 0)
				output << "US-05 ERROR: Child's birth occurs after mother's death!" << "\t\t Line " << (*it).linenum << endl; 

			// Check if born before grandfather's or grandmother's birth
			Person father;
			if(FindParent(families, (*itp).Id,father,true))
			{
				Person grandfather;
				if(FindParent(families,father.Id,grandfather,true))
				{
					if (CompareDates(childBirth, grandfather.Birth) == -1)
						output << "US-08 ERROR: Child's birth occurs before grandfather's birth!" << "\t Line " << (*it).linenum << endl; 
				}
				Person grandMother;
				if(FindParent(families,father.Id,grandMother,false))
				{
					if (CompareDates(childBirth, grandMother.Birth) == -1)
						output << "US-09 ERROR: Child's birth occurs before grandmother's birth!" << "\t Line " << (*it).linenum << endl; 
				}
			}
				// Check if born before grandfather's or grandmother's birth
			Person mother;
			if(FindParent(families, (*itp).Id,mother,false))
			{
				Person grandfather;
				if(FindParent(families,mother.Id,grandfather,true))
				{
					if (CompareDates(childBirth, grandfather.Birth) == -1)
						output << "US-08 ERROR: Child's birth occurs before grandfather's birth!" << "\t Line " << (*it).linenum << endl; 
				}
				Person grandMother;
				if(FindParent(families,mother.Id,grandMother,false))
				{
					if (CompareDates(childBirth, grandMother.Birth) == -1)
						output << "US-09 ERROR: Child's birth occurs before grandmother's birth!" << "\t Line " << (*it).linenum << endl; 
				}
			}
				
		}
		output << "\n\n\n";
	}
	/*else cout << "Unable to open file \n"; */

	output.close();

	cout << "\nType 'C' and press 'ENTER' Key to close the program \n";
	string p;
	cin >> p;
	return 0;
}
