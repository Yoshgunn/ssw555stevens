// CS_555_Project.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Family.h"

string replace(string line,string text, int length)
{
	int pos = line.find( text );
	if ( pos != string::npos ) {
		return line.replace(0, pos+5, "");   // 5 = length( $name )
	}
	return "";
}
std::vector<Person> GetPeople()
{
	string line;
	// ifstream myfile("C:\\Users\\Dipan\\Documents\\GitHub\\ssw555stevens\\CS_555_Project\\CS_555_Project\\Debug\\My-Family-7-Sep-2014.ged",ios::in);
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
				Person p;
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
						ss >> p.Birth;
					}
					else if( buf =="DEAT")
					{
						getline(myfile,line);
						ss >> p.Death;
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
Person FindPerson(string id, std::vector<Person> people)
{
	 for (std::vector<Person>::iterator it = people.begin() ; it != people.end(); ++it)
	 {
		 if((*it).Id== id)
			 return *it;
	 }
}
std::vector<Family> GetFamilies(std::vector<Person> people)
{
	string line;
	// ifstream myfile("C:\\Users\\Dipan\\Documents\\GitHub\\ssw555stevens\\CS_555_Project\\CS_555_Project\\Debug\\My-Family-7-Sep-2014.ged", ios::in);
	ifstream myfile("Debug\\AcceptanceTestFile.ged", ios::in);

	if (myfile.is_open())
	{
		Family f;
		std::vector<Family> familyColl;
		int totalPep=0;
		while (getline(myfile,line) )
		{
			if(line.substr(0,4) == "0 @F")
			{
				if(totalPep >0)
				{
					familyColl.push_back(f);
				}
				Family f;
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
						f.Marriage = "Unmarried";
					}
					else if(buf =="HUSB")
					{
						string id;
						ss >> id;
						f.Husband = FindPerson(id,people);
						//ss >> f.GivenName;
					}
					else if(buf =="WIFE")
					{
						string id;
						ss >> id;
						f.Wife = FindPerson(id,people);
					}
					else if( buf =="CHIL")
					{
						string id;
						ss >> id;
						// f.AddChild(FindPerson(id,people));	This function was not found
						f.Children.push_back(FindPerson(id,people));
					}
					else if( buf =="MARR")
					{
						getline(myfile, line);
						f.Marriage = line.substr(7);
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

	
	std::vector<Person> people = GetPeople();
	std::vector<Family> families = GetFamilies(people);
	
	for (std::vector<Family>::const_iterator it = families.begin(); it!=families.end(); ++it) {
		cout << "Family Id : " << (*it).Id << "\n";
		cout << "\t Husband Name : " << (*it).Husband.GivenName << "\n";
		cout << "\t Wife Name : " << (*it).Wife.GivenName << "\n";
		cout << "\t Date Married : " << (*it).Marriage << "\n";
		for (std::vector<Person>::const_iterator itp = (*it).Children.begin(); itp!=(*it).Children.end(); ++itp) {
			cout << "\t\t Children Name : " <<(*itp).GivenName<< "\n";
		}
		cout << "\n";
	}
	/*else cout << "Unable to open file \n"; */

	cout << "\nType 'C' and press 'ENTER' Key to close the program \n";
	string p;
	cin >> p;
	return 0;
}

