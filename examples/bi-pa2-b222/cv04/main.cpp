//#ifndef __PROGTEST__

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <array>
#include <cassert>

using namespace std;
//#endif /* __PROGTEST__ */

class CTimeStamp {
    int year, month, day, hour, minute, second;

    bool before(const CTimeStamp &t2) const {
        if (year != t2.year) return year < t2.year;
        if (month != t2.month) return month < t2.month;
        if (day != t2.day) return day < t2.day;
        if (hour != t2.hour) return hour < t2.hour;
        if (minute != t2.minute) return minute < t2.minute;
        if (second != t2.second) return second < t2.second;
        return false;
    }

public:

    CTimeStamp(int year, int month, int day, int hour, int minute, int second)
            : year(year), month(month), day(day), hour(hour), minute(minute), second(second) {};

    bool in_range(const CTimeStamp &from, const CTimeStamp &to) const {
        if (to.before(*this)) return false;
        if (before(from)) return false;
        return true;
    }
};

class CContact {
    CTimeStamp time;
    int person1;
    int person2;

public:
    CContact(const CTimeStamp &time, int phone1, int phone2)
            : time(time), person1(phone1), person2(phone2) {};

    bool is_participant(int phone, int &other) const {
        if (phone == person1) {
            other = person2;
            return true;
        } else if (phone == person2) {
            other = person1;
            return true;
        }
        return false;
    }

    bool in_range(const CTimeStamp &from, const CTimeStamp &to) const {
        return time.in_range(from, to);
    }
};

class CEFaceMask {
    vector<CContact> contacts;

public:
    CEFaceMask &addContact(const CContact &contact) {
        contacts.push_back(contact);
        return *this;
    }

    vector<int> listContacts(int phone) const {
        vector<int> out;
        int other;
        for (const CContact c: contacts) {
            if (c.is_participant(phone, other)) {
                bool unique = true;
                for (int i: out) {
                    if (other == i) {
                        unique = false;
                        break;
                    }
                }
                if (unique && other != phone) {
                    out.push_back(other);
                }
            }
        }

        return out;
    }

    vector<int> listContacts(int phone, const CTimeStamp &from, const CTimeStamp &to) const {
        vector<int> out;
        int other;
        for (const CContact c: contacts) {
            if (c.is_participant(phone, other)) {
                if (!c.in_range(from, to)) continue;

                bool unique = true;
                for (int i: out) {
                    if (other == i) {
                        unique = false;
                        break;
                    }
                }
                if (unique && other != phone) {
                    out.push_back(other);
                }
            }
        }

        return out;
    }
};

//#ifndef __PROGTEST__

int main() {
    CEFaceMask test;

    test.addContact(CContact(CTimeStamp(2021, 1, 10, 12, 40, 10), 123456789, 999888777));
    test.addContact(CContact(CTimeStamp(2021, 1, 12, 12, 40, 10), 123456789, 111222333))
            .addContact(CContact(CTimeStamp(2021, 2, 5, 15, 30, 28), 999888777, 555000222));
    test.addContact(CContact(CTimeStamp(2021, 2, 21, 18, 0, 0), 123456789, 999888777));
    test.addContact(CContact(CTimeStamp(2021, 1, 5, 18, 0, 0), 123456789, 456456456));
    test.addContact(CContact(CTimeStamp(2021, 2, 1, 0, 0, 0), 123456789, 123456789));
    assert (test.listContacts(123456789) == (vector<int>{999888777, 111222333, 456456456}));
    assert (test.listContacts(999888777) == (vector<int>{123456789, 555000222}));
    assert (test.listContacts(191919191) == (vector<int>{})); // NOLINT(readability-container-size-empty)
    assert (test.listContacts(123456789, CTimeStamp(2021, 1, 5, 18, 0, 0), CTimeStamp(2021, 2, 21, 18, 0, 0)) ==
            (vector<int>{999888777, 111222333, 456456456}));
    assert (test.listContacts(123456789, CTimeStamp(2021, 1, 5, 18, 0, 1), CTimeStamp(2021, 2, 21, 17, 59, 59)) ==
            (vector<int>{999888777, 111222333}));
    assert (test.listContacts(123456789, CTimeStamp(2021, 1, 10, 12, 41, 9), CTimeStamp(2021, 2, 21, 17, 59, 59)) ==
            (vector<int>{111222333}));
    return 0;
}

//#endif /* __PROGTEST__ */
