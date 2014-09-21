#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <utility>

using namespace std;


int main()
{
    map<string, string> individual;
    map<string, pair<string, string> > family;
    ifstream f("GedcomFile.ged");
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

    for (auto &s : individual)
    {
        cout << s.second << endl;
    }

    for (auto &s : family)
    {
        cout << "Husband: " << individual[s.second.first] << endl;
        cout << "Wife: " << individual[s.second.second] << endl;
    }

    f.close();

    return 0;
}
