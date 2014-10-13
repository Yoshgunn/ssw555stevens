// Project.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Person.h"


int main(int argc, _TCHAR* argv[])
{
	std::string str[] = { "INDI","NAME","SEX","BIRT","DEAT","FAMC","FAMS","FAM","MARR","HUSB","WIFE","CHIL","DIV","DATE","TRLR","NOTE"};
	vector< std::string > validTags( str, str + ( sizeof ( str ) /  sizeof ( std::string ) ) );

	string line;
	ifstream myfile("C:\\Users\\Dipan\\Documents\\GitHub\\ssw555stevens\\GEDCOM\\My-Family-7-Sep-2014.ged");
	//  ifstream myfile("Test.txt");
	if (myfile.is_open())
	{
		Person* p;
		Person* people[15];
		int totalPep=0;
		while (getline(myfile,line) )
		{
			if(line.substr(0,4) == "0 @I")
			{
				people[totalPep] = new Person();
				p = people[totalPep];
				totalPep++;
			}
			//cout << "Original Line: \"" << line <<"\"";
			string buf; // Have a buffer string
			stringstream ss(line); // Insert the string into a stream

			vector<string> tokens; // Create vector to hold our words
			int index =0;
			while (ss >> buf && index <2)
			{
				/*if(index ==0)
				cout << " \t\t " << "Line Number: " << buf << " | ";
				else if( index ==1 && std::find(validTags.begin(),validTags.end(),buf) != validTags.end())
				cout << "Tag Name: " << buf << "\n\n";
				else
				cout << "Invalid Tag Name: " << buf << "\n\n";*/
				//if(index ==0)
				//cout << "\n";
				if( index ==1)
				{
					if(buf =="NAME")
					{
						int pos = line.find( "NAME" );
						if ( pos != string::npos ) {
							//p->Name = line.replace(0, pos+5, "");   // 5 = length( $name )
						}
						/*string first, last;
						ss >> first;
						ss >> last;*/
						//p->Name = first+ ' ' +last; 
					}
					else if( buf =="GIVN")
					{
						ss >> p->GivenName;
					}
					else if( buf =="SURN")
					{
						ss >> p->Surname;
					}
					else if( buf =="SEX")
					{
						ss >> p->Sex;
					}
					else if( buf =="BIRT")
					{
						getline(myfile,line);

						int pos = line.find( "DATE" );
						if ( pos != string::npos ) {
							p->Birth = line.replace(0, pos+5, "");   // 5 = length( $name )
						}

					}
					else if( buf =="DEAT")
					{
						getline(myfile,line);

						int pos = line.find( "DATE" );
						if ( pos != string::npos ) {
							p->Death = line.replace(0, pos+5, "");   // 5 = length( $name )
						}
					}

				}
				/*else
					cout << buf;*/
				index++;
			}
		}
		myfile.close();

	}

	else cout << "Unable to open file \n"; 

	cout << "\nType 'C' and press 'ENTER' Key to close the program \n";
	string p;
	cin >> p;
	return 0;
}

