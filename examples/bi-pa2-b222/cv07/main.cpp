#ifndef __PROGTEST__

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <numeric>
#include <vector>
#include <array>
#include <set>
#include <map>
#include <cassert>

using namespace std;
#endif /* __PROGTEST__ */

typedef pair<int, int> intPair;

class CTimeStamp {
    int year, month, day, hour, minute, second;

public:

    CTimeStamp(int year, int month, int day, int hour, int minute, int second)
            : year(year), month(month), day(day), hour(hour), minute(minute), second(second) {};

    bool operator<(const CTimeStamp &t2) const {
        if (year != t2.year) return year < t2.year;
        if (month != t2.month) return month < t2.month;
        if (day != t2.day) return day < t2.day;
        if (hour != t2.hour) return hour < t2.hour;
        if (minute != t2.minute) return minute < t2.minute;
        if (second != t2.second) return second < t2.second;
        return false;
    }
};

class CContact {
public:
    CTimeStamp m_TimeStamp;
    int m_Phone1 = 0;
    int m_Phone2 = 0;

    CContact(const CTimeStamp &timeStamp, int phone1, int phone2)
            : m_TimeStamp(timeStamp), m_Phone1(phone1), m_Phone2(phone2) {
        if (phone1 > phone2)
            swap(m_Phone1, m_Phone2);
    }
};

bool cmpContact(const CContact &lhs, const CContact &rhs) {
    return lhs.m_TimeStamp < rhs.m_TimeStamp;
}

bool cmpContactTimeStamp(const CContact &lhs, const CTimeStamp &rhs) {
    return lhs.m_TimeStamp < rhs;
}

class CEFaceMask {
    vector<CContact> m_Contacts;

public:
    CEFaceMask() = default;

    CEFaceMask &addContact(const CContact &contact) {
        if (contact.m_Phone1 == contact.m_Phone2)
            return *this;

        auto pos = lower_bound(m_Contacts.begin(), m_Contacts.end(), contact, cmpContact);
        m_Contacts.insert(pos, contact);

        return *this;
    }

    vector<int> getSuperSpreaders(const CTimeStamp &from, const CTimeStamp &to) const {
        auto lo = lower_bound(m_Contacts.begin(), m_Contacts.end(), from, cmpContactTimeStamp);

        set<intPair> madeContact;
        map<int, int> contactCount;
        for (auto it = lo; it != m_Contacts.end() && !(to < (*it).m_TimeStamp); it++) {
            if (madeContact.insert(intPair(it->m_Phone1, it->m_Phone2)).second) {
                contactCount[it->m_Phone2]++;
                contactCount[it->m_Phone1]++;
            }
        }

        int max = 0;
        for (auto elem: contactCount)
            if (elem.second > max)
                max = elem.second;

        if (max == 0)
            return vector<int>{};

        set<int> super;
        for (auto elem: contactCount)
            if (elem.second == max)
                super.insert(elem.first);

        vector<int> out;
        for (auto elem: super)
            out.push_back(elem);

        return out;
    }
};

#ifndef __PROGTEST__

int main() {
    CEFaceMask test, test2, test3, test4;

    test.addContact(CContact(CTimeStamp(2021, 1, 10, 12, 40, 10), 111111111, 222222222));
    test.addContact(CContact(CTimeStamp(2021, 1, 10, 12, 40, 10), 222222222, 222222222));
    test.addContact(CContact(CTimeStamp(2021, 1, 12, 12, 40, 10), 333333333, 222222222))
            .addContact(CContact(CTimeStamp(2021, 2, 14, 15, 30, 28), 222222222, 444444444));
    test.addContact(CContact(CTimeStamp(2021, 2, 15, 18, 0, 0), 555555555, 444444444));
    assert(test.getSuperSpreaders(CTimeStamp(2021, 1, 1, 0, 0, 0), CTimeStamp(2022, 1, 1, 0, 0, 0)) ==
           (vector<int>{222222222}));
    test.addContact(CContact(CTimeStamp(2021, 3, 20, 18, 0, 0), 444444444, 666666666));
    test.addContact(CContact(CTimeStamp(2021, 3, 25, 0, 0, 0), 111111111, 666666666));
    assert(test.getSuperSpreaders(CTimeStamp(2021, 1, 1, 0, 0, 0), CTimeStamp(2022, 1, 1, 0, 0, 0)) ==
           (vector<int>{222222222, 444444444}));

    test2.addContact(CContact(CTimeStamp(2021, 3, 10, 12, 40, 10), 5, 6));
    test2.addContact(CContact(CTimeStamp(2021, 2, 10, 12, 40, 10), 5, 6));
    test2.addContact(CContact(CTimeStamp(2021, 3, 10, 12, 40, 10), 5, 6));
    test2.addContact(CContact(CTimeStamp(2021, 4, 10, 12, 40, 10), 5, 6));
    test2.addContact(CContact(CTimeStamp(2021, 2, 10, 12, 40, 10), 11, 12));
    test2.addContact(CContact(CTimeStamp(2021, 6, 10, 12, 40, 10), 9, 10));
    test2.addContact(CContact(CTimeStamp(2021, 9, 10, 12, 40, 10), 9, 10));
    test2.addContact(CContact(CTimeStamp(2021, 8, 10, 12, 40, 10), 9, 10));
    test2.addContact(CContact(CTimeStamp(2021, 1, 10, 12, 40, 10), 9, 10));
    test2.addContact(CContact(CTimeStamp(2021, 2, 10, 12, 40, 10), 7, 8));
    test2.addContact(CContact(CTimeStamp(2021, 6, 10, 12, 40, 10), 7, 8));
    test2.addContact(CContact(CTimeStamp(2021, 3, 10, 12, 40, 10), 7, 8));
    test2.addContact(CContact(CTimeStamp(2021, 7, 10, 12, 40, 10), 3, 4));
    test2.addContact(CContact(CTimeStamp(2021, 2, 10, 12, 40, 10), 3, 4));
    test2.addContact(CContact(CTimeStamp(2021, 7, 10, 12, 40, 10), 3, 4));
    test2.addContact(CContact(CTimeStamp(2021, 4, 10, 12, 40, 10), 1, 2));
    test2.addContact(CContact(CTimeStamp(2021, 9, 10, 12, 40, 10), 1, 2));
    assert(test2.getSuperSpreaders(CTimeStamp(2020, 1, 1, 0, 0, 0), CTimeStamp(2023, 1, 1, 0, 0, 0)) ==
           (vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}));

    assert(test3.getSuperSpreaders(CTimeStamp(2020, 1, 1, 0, 0, 0), CTimeStamp(2023, 1, 1, 0, 0, 0)).empty());

    test4.addContact(CContact(CTimeStamp(2021, 1, 1, 0, 0, 0), 111111111, 222222222));
    test4.addContact(CContact(CTimeStamp(2021, 1, 10, 12, 40, 10), 222222222, 222222222));
    test4.addContact(CContact(CTimeStamp(2021, 1, 12, 12, 40, 10), 333333333, 222222222))
            .addContact(CContact(CTimeStamp(2021, 2, 14, 15, 30, 28), 222222222, 444444444));
    test4.addContact(CContact(CTimeStamp(2021, 2, 15, 18, 0, 0), 555555555, 444444444));
    assert(test4.getSuperSpreaders(CTimeStamp(2021, 1, 1, 0, 0, 0), CTimeStamp(2021, 1, 1, 0, 2, 0)) ==
           (vector<int>{111111111, 222222222}));
    assert(test4.getSuperSpreaders(CTimeStamp(2020, 1, 1, 0, 0, 0), CTimeStamp(2021, 1, 1, 0, 0, 0)) ==
           (vector<int>{111111111, 222222222}));
    assert(test4.getSuperSpreaders(CTimeStamp(2021, 1, 1, 0, 0, 0), CTimeStamp(2021, 1, 1, 0, 0, 0)) ==
           (vector<int>{111111111, 222222222}));

    return 0;
}

#endif /* __PROGTEST__ */