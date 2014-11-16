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
				p=Person();
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
*	UC12: Everyone have unique id
*/
void checkIds(std::vector<Person> &people) {
	int count = 0;
	string uids[100];

	for (std::vector<Person>::iterator it = people.begin() ; it != people.end(); ++it) {
		bool duplicate = false;

		for(int i = 0; !duplicate && i < count; i++) {
			if((*it).Id == uids[i]) {
				cout << "* *ERROR FOUND: Duplicate ID (" << (*it).Id << ") found!* *\n" << endl;
				duplicate = true;
			}
		}

		if(!duplicate) {
			uids[count] = (*it).Id;
			count++;
		}
	}
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
						if (f.Husband.Id != "")
						{
							cout << "Line: " << linenum << "\t " << "* *ERROR FOUND: There are two husbands in this marriage!* *" << endl;
							output << "Line: " << linenum << "\t " << "* *ERROR FOUND: There are two husbands in this marriage!* *" << endl;
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
							cout << "Line: " << linenum << "\t " << "* *ERROR FOUND: There are two Wife!* *" << endl;
							output << "Line: " << linenum << "\t " << "* *ERROR FOUND: There are two Wife!* *" << endl;
						}
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
		cout << endl << "Family Id : " << (*it).Id << "\n";
		output << endl << "Family Id : " << (*it).Id << "\n";
		if (it->Husband.Id != "")
		{
			if ((*it).Husband.Sex == (*it).Wife.Sex)
			{
				cout << "Line: " << (*it).linenum << "\t " << "* *ERROR FOUND: Both spouses are the same gender!* *" << endl;
				output << "Line: " << (*it).linenum << "\t " << "* *ERROR FOUND: Both spouses are the same gender!* *" << endl;
			}
			cout << "\t Husband Name : " << (*it).Husband.GivenName << "\n";
			output << "\t Husband Name : " << (*it).Husband.GivenName << "\n";
			if ((*it).Husband.Sex != "M")
			{
				cout << "Line: " << (*it).linenum << "\t " << "* *ERROR FOUND: Husbands must be Male!* *" << endl;
				output << "Line: " << (*it).linenum << "\t " << "* *ERROR FOUND: Husbands must be Male!* *" << endl;
				output << "* *ERROR FOUND: Husbands must be Male!* *" << endl;
			}

			// Husband's Birth
			std::tm husbBirth = (*it).Husband.Birth;
			if (husbBirth.tm_year != 0 && husbBirth.tm_mon != 0 && husbBirth.tm_mday != 0)
			{
				cout << "\t Husband Birth : " << put_time(&husbBirth, "%d %b %Y") << "\n";
				output << "\t Husband Birth : " << put_time(&husbBirth, "%d %b %Y") << "\n";

				// Check if born before father
				if ((*it).Husband.Father) //&& CompareDates(husbBirth, (*it).Husband.Father->Birth) == -1)
				{
					std:tm grandpaTime = (*it).Husband.Father->Birth;
					// string grandpaName = (*it).Husband.Father->GivenName;
					// cout << "Dad's name = " << (*it).Husband.Father->GivenName;
					// cout << "Dad's birth = " << grandpaTime.tm_year << ", " << grandpaTime.tm_mon << ", " << grandpaTime.tm_mday << endl;
					// cout << "Birth before dad!" << endl;
				}		
			}

			// Husband's Death
			std::tm husbDeath = (*it).Husband.Death;
			if (husbDeath.tm_year >= 0 && husbDeath.tm_mon >= 0 && husbDeath.tm_mday >= 0)
			{
				cout << "\t Husband Death : " << put_time(&husbDeath, "%d %b %Y") << "\n";
				output << "\t Husband Death : " << put_time(&husbDeath, "%d %b %Y") << "\n";

				// Check if died before born
				if (CompareDates(husbBirth, husbDeath) == 1)
				{
					cout << "* *ERROR FOUND: Husband's death occurs before his birth!* *" << endl;
					output << "* *ERROR FOUND: Husband's death occurs before his birth!* *" << endl;
				}
			}
		}
		if (it->Husband.Id != "")
		{
			cout << "\t Wife Name : " << (*it).Wife.GivenName << "\n";
			output << "\t Wife Name : " << (*it).Wife.GivenName << "\n";
			if ((*it).Wife.Sex != "F")
			{
				cout << "Line: " << (*it).linenum << "\t " << "* *ERROR FOUND: Wives must be Female!* *" << endl;
				output << "Line: " << (*it).linenum << "\t " << "* *ERROR FOUND: Wives must be Female!* *" << endl;
				output << "* *ERROR FOUND: Wives must be Female!* *" << endl;

			}

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
				output << "\t Wife Death : " << put_time(&wifeDeath, "%d %b %Y") << "\n"; \

					// Check if died before born
					if (CompareDates(wifeBirth, wifeDeath) == 1) 
					{
						cout << "Line: " << (*it).linenum << "\t " << "* *ERROR FOUND: Wife's death occurs before her birth!* *" << endl;
						output << "Line: " << (*it).linenum << "\t " << "* *ERROR FOUND: Wife's death occurs before her birth!* *" << endl;
						// cout << "* *ERROR FOUND: Wife's death occurs before her birth!* *" << endl;
						// output << "* *ERROR FOUND: Wife's death occurs before her birth!* *" << endl;
					}
			}
		}
		// Check siblings
		if ((it->Husband.Mother) && (it->Husband.Mother == it->Wife.Mother) || (it->Husband.Father) && (it->Husband.Father == it->Wife.Father))
		{
			cout << "Line: " << (*it).linenum << "\t " << "* *ERROR FOUND: Marriage contains siblings!* *" << endl;
			output << "Line: " << (*it).linenum << "\t " << "* *ERROR FOUND: Marriage contains siblings!* *" << endl;
		}
		// Check for Marriages
		std::tm marr = (*it).Marriage;
		if ( marr.tm_year != 0 && marr.tm_mon != 0 && marr.tm_mday != 0 )
		{
			cout << "\t Date Married : " << put_time(&marr, "%d %b %Y") << "\n";
			output << "\t Date Married : " << put_time(&marr, "%d %b %Y") << "\n";

			// Marriage Anomaly?
			//    Marriage after one of the spouses died
			if ( CompareDates( (*it).Husband.Death, marr ) == -1 )
			{
				cout << "Line: " << (*it).linenum << "\t " << "* *ERROR FOUND: Husband's death occurs before his marriage!* *" << endl;
				output << "Line: " << (*it).linenum << "\t " << "* *ERROR FOUND: Husband's death occurs before his marriage!* *" << endl;
			}
			if ( CompareDates( (*it).Wife.Death, marr ) == -1 )
			{
				cout << "Line: " << (*it).linenum << "\t " << "* *ERROR FOUND: Wife's death occurs before her marriage!* *" << endl;
				output << "Line: " << (*it).linenum << "\t " << "* *ERROR FOUND: Wife's death occurs before her marriage!* *" << endl;
			}


			//    Marriage before one of the spouses was born
			if ( CompareDates( marr, (*it).Husband.Birth ) == -1 )
			{
				cout << "Line: " << (*it).linenum << "\t " << "* * ERROR FOUND: Husband married before he was born!* *" << endl;
				output << "* * ERROR FOUND: Husband married before he was born!* *" << endl;
			}
			if ( CompareDates( marr, (*it).Wife.Birth ) == -1 )
			{
				cout << "Line: " << (*it).linenum << "\t " << "* * ERROR FOUND: Wife married before she was born!* *" << endl;
				output << "* * ERROR FOUND: Wife married before she was born!* *" << endl;
			}
		}

		// Check for Divorces
		std::tm div = (*it).Divorce;
		if ( div.tm_year != 0 && div.tm_mon != 0 && div.tm_mday != 0 )
		{
			cout << "\t Date Divorced : " << put_time(&div, "%d %b %Y") << "\n";
			output << "\t Date Divorced : " << put_time(&div, "%d %b %Y") << "\n";

			// Divorce Anomaly?
			if ( CompareDates( div, marr ) == -1 )
			{
				cout << "Line: " << (*it).linenum << "\t " << "* *ERROR FOUND: Divorce occurs before marriage!* *" << endl;
				cout << put_time(&div, "%d %b %Y") << " is before " << put_time(&marr, "%d %b %Y") << endl;
				output << "Line: " << (*it).linenum << "\t " << "* *ERROR FOUND: Divorce occurs before marriage!* *" << endl;
				output << "* *ERROR FOUND: Divorce occurs before marriage!* *" << endl;
			}
		}
		if (it->Children.size() >= 10)
		{
			cout << "Line: " << it->linenum << "\t " << "* *ERROR FOUND: This family has more than 10 children!* *" << endl;
			output << "Line: " << it->linenum << "\t " << "* *ERROR FOUND: This family has more than 10 children!* *" << endl;
		}
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

				if( CompareDates( childBirth, childDeath ) == 1 ) {
					cout << "* *ERROR FOUND: Child's death occurs before his/her birth!* *" << endl;
					output << "* *ERROR FOUND: Child's death occurs before his/her birth!* *" << endl;
				}
			}

			// Check if born before father's birth
			if (CompareDates(childBirth, (*it).Husband.Birth) == -1) {
				cout << "* *ERROR FOUND: Child's birth occurs before father's birth!* *" << endl;
				output << "* *ERROR FOUND: Child's birth occurs before father's birth!* *" << endl;
			}

			// Check if born before mother's birth
			if (CompareDates(childBirth, (*it).Wife.Birth) == -1) {
				cout << "* *ERROR FOUND: Child's birth occurs before mother's birth!* *" << endl;
				output << "* *ERROR FOUND: Child's birth occurs before mother's birth!* *" << endl;
			}

			// Check if born after mother's death
			if (CompareDates(childBirth, (*it).Wife.Death) == 1 && (*it).Wife.Death.tm_year >= 0 && (*it).Wife.Death.tm_mon >= 0 && (*it).Wife.Death.tm_mday >= 0) {
				cout << "* *ERROR FOUND: Child's birth occurs after mother's death!* *" << endl;
				output << "* *ERROR FOUND: Child's birth occurs after mother's death!* *" << endl;
			}
		}
		cout << "\n";
	}
	/*else cout << "Unable to open file \n"; */

	output.close();

	cout << "\nType 'C' and press 'ENTER' Key to close the program \n";
	string p;
	cin >> p;
	return 0;
}
