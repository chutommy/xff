#ifndef __PROGTEST__

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <cassert>

using namespace std;
#endif /* __PROGTEST__ */

enum State {
    OK, NL, ERR
};

class Contact {
    string firstName;
    string lastName;
    int phone = 0;

    static bool isPhoneNumber(const string &ph) {
        if (ph.length() != 9) return false;
        if (ph[0] == '0') return false;
        for (char const &ch: ph)
            if (!isdigit(ch)) return false;
        return true;
    }

public:

    State scan(istream &in) {
        string line;
        if (!(getline(in, line))) return ERR;
        if (line.length() == 0) return NL;

        stringstream ss(line);
        string fn, ln, ph, end;
        if (!(ss >> fn && ss >> ln && ss >> ph)) return ERR;
        if (ss >> end) return ERR;
        if (!isPhoneNumber(ph)) return ERR;

        firstName = fn;
        lastName = ln;
        phone = stoi(ph);
        return OK;
    }

    ostream &print(ostream &out) const {
        return out << firstName << " " << lastName << " " << phone;
    }

    bool match(const string &s) const {
        return s == firstName || s == lastName;
    }
};

bool report(const string &fileName, ostream &out) {
    ifstream fin(fileName);
    if (!fin) return false;

    vector<Contact> contacts;
    Contact c;
    while (true) {
        State s = c.scan(fin);
        if (s == NL) break;
        if (s == ERR) return false;
        contacts.push_back(c);
    }

    while (true) {
        string query;
        if (!(getline(fin, query))) break;
        size_t count = 0;
        for (const Contact &contact: contacts) {
            if (contact.match(query)) {
                contact.print(out) << endl;
                count++;
            }
        }
        out << "-> " << count << endl;
    }
    return true;
}

#ifndef __PROGTEST__

int main() {
    ostringstream oss;
    oss.str("");
    assert (report("tests/test0_in.txt", oss) == true);
    assert (oss.str() ==
            "John Christescu 258452362\n"
            "John Harmson 861647702\n"
            "-> 2\n"
            "-> 0\n"
            "Josh Dakhov 264112084\n"
            "Dakhov Speechley 865216101\n"
            "-> 2\n"
            "John Harmson 861647702\n"
            "-> 1\n");
    oss.str("");
    assert (report("tests/test1_in.txt", oss) == false);
    return 0;
}

#endif /* __PROGTEST__ */
