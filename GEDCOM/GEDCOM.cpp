//
//  main.cpp
//
//  Created by Zhenbao Yang on 14-10-1.
//	Modified by Frank DiCola on 14-10-2.
//  Copyright (c) 2014 Zhenbao Yang. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <utility>
#include <algorithm>
using namespace std;


int main1()
{
	// Users must enter the name of the file manually.
    string filename;
    cout << "Please enter the file name." << endl;
    cin >> filename;

    // We cannot deal with incorrect file types, so the user must enter a .ged file.
    if ( filename.substr(max(0, (int)(filename.size()-4))) != string(".ged") )
    {
        cout << "This program only accepts filenames that end in .ged." << endl;
        return 1;
    }


    map<string, string> individual;
    map<string, pair<string, string> > family;
    ifstream f(filename.c_str()); // The constructor takes a const char*, not a string, hence the conversion
    ofstream output;
	output.open("TestResults.txt");
	string line, id;

    while (getline(f, line))
    {
        istringstream iss(line);
        int level;
        string token;
        iss >> level;
        iss >> token;
        if (level == 0)
        {
            if (token[0] == '@')
            {
                id = token;
            }
        }
        else
        {
            string s, t;
            iss >> s;

            if (token == "NAME")
            {
                iss >> t;
                individual[id] = s + " " + t.substr(1, t.size() - 2);
            }
            else if (token == "HUSB")
            {
                family[id].first = s;
            }
            else if (token == "WIFE")
            {
                family[id].second = s;
            }
        }


    }

	// Give the user a list of the individuals found
	cout << "The following individuals were found:" << endl;
	output << "The following individuals were found:" << endl;
	for( map<string, string>::iterator ii=individual.begin(); ii != individual.end(); ++ii)
	{
		cout << (*ii).first << "  " << ii->second << endl;
		output << (*ii).first << "  " << ii->second << endl;
	}

	// Give the user a list of the families found
	cout << "\nThe following families were found: " << endl;
	for( map<string, pair<string, string>>::iterator ii=family.begin(); ii != family.end(); ++ii)
	{
		cout << (*ii).first << endl;
	}
	
	/* This code generated numerous errors I could not fix
    for (auto &s : individual)
    {
        cout << s.second << endl;
    }

    for (auto &s : family)
    {
        cout << "Husband: " << individual[s.second.first] << endl;
        cout << "Wife: " << individual[s.second.second] << endl;
    }*/

    f.close();
	output.close();

	cout << "Sanity test complete" << endl;

    return 0;
}