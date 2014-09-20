#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const vector<string> tags { "INDI", "NAME", "SEX", "BIRT", "DEAT",
                            "FAMC", "FAMS", "FAM", "MARR", "HUSB",
                            "WIFE", "CHIL", "DIV", "DATE", "TRLR", "NOTE" };

void FindTag(string s)
{
    for (auto t:tags)
    {
        if (s.find(t)!=string::npos)
        {
            cout<<t<<endl;
            return;
        }
    }
    cout << "Invalid tag" << endl;
}

int main()
{
    ifstream f("GedcomFile.ged");
    string line;
    while (getline(f, line))
    {
        cout << line << endl;
        cout << "Level is: " << line[0] << endl;
        FindTag(line);
        cout << endl;
    }
    f.close();
    return 0;
}
