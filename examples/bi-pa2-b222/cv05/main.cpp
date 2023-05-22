#ifndef __PROGTEST__

#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>

using namespace std;

class CTimeTester;

#endif /* __PROGTEST__ */

class CTime {
private:
    int m_Hour;
    int m_Minute;
    int m_Second;

    void validate() const {
        if (m_Hour < 0 || m_Hour > 23)
            throw invalid_argument("Invalid [hour] value.");
        if (m_Minute < 0 || m_Minute > 59)
            throw invalid_argument("Invalid [minute] value.");
        if (m_Second < 0 || m_Second > 59)
            throw invalid_argument("Invalid [second] value.");
    }

    void normalize() {
        m_Minute += m_Second / 60;
        m_Second %= 60;
        if (m_Second < 0) {
            m_Minute--;
            m_Second += 60;
        }

        m_Hour += m_Minute / 60;
        m_Minute %= 60;
        if (m_Minute < 0) {
            m_Hour--;
            m_Minute += 60;
        }

        m_Hour %= 24;
        if (m_Hour < 0)
            m_Hour += 24;
    }

    int toSeconds() const {
        return m_Hour * 60 * 60 + m_Minute * 60 + m_Second;
    }

public:

    explicit CTime(int hour = 0, int minute = 0, int second = 0)
            : m_Hour(hour), m_Minute(minute), m_Second(second) {
        validate();
    }

    CTime &operator+=(int sec) {
        m_Second += sec;
        normalize();
        return *this;
    }

    CTime &operator-=(int sec) {
        m_Second -= sec;
        normalize();
        return *this;
    }

    friend CTime operator+(CTime lhs, int sec) {
        lhs += sec;
        return lhs;
    }

    friend CTime operator+(int sec, CTime rhs) {
        rhs += sec;
        return rhs;
    }

    friend CTime operator-(CTime lhs, int sec) {
        lhs -= sec;
        return lhs;
    }

    friend CTime operator-(int sec, CTime lhs) {
        lhs -= sec;
        return lhs;
    }

    CTime &operator++() {
        m_Second++;
        normalize();
        return *this;
    }

    CTime operator++(int) {
        CTime old(m_Hour, m_Minute, m_Second);
        operator++();
        return old;
    }

    CTime &operator--() {
        m_Second--;
        normalize();
        return *this;
    }

    CTime operator--(int) {
        CTime old(m_Hour, m_Minute, m_Second);
        operator--();
        return old;
    }

    friend bool operator==(const CTime &lhs, const CTime &rhs) {
        return lhs.m_Hour == rhs.m_Hour
               && lhs.m_Minute == rhs.m_Minute
               && lhs.m_Second == rhs.m_Second;
    }

    friend bool operator!=(const CTime &lhs, const CTime &rhs) {
        return lhs.m_Hour != rhs.m_Hour
               || lhs.m_Minute != rhs.m_Minute
               || lhs.m_Second != rhs.m_Second;
    }

    friend bool operator<(const CTime &lhs, const CTime &rhs) {
        if (lhs.m_Hour != rhs.m_Hour)
            return lhs.m_Hour < rhs.m_Hour;
        if (lhs.m_Minute != rhs.m_Minute)
            return lhs.m_Minute < rhs.m_Minute;
        if (lhs.m_Second != rhs.m_Second)
            return lhs.m_Second < rhs.m_Second;
        return false;
    }

    friend bool operator>(const CTime &lhs, const CTime &rhs) {
        if (lhs.m_Hour != rhs.m_Hour)
            return lhs.m_Hour > rhs.m_Hour;
        if (lhs.m_Minute != rhs.m_Minute)
            return lhs.m_Minute > rhs.m_Minute;
        if (lhs.m_Second != rhs.m_Second)
            return lhs.m_Second > rhs.m_Second;
        return false;
    }

    friend bool operator<=(const CTime &lhs, const CTime &rhs) {
        return lhs < rhs || lhs == rhs;
    }

    friend bool operator>=(const CTime &lhs, const CTime &rhs) {
        return lhs > rhs || lhs == rhs;
    }

    friend int operator-(CTime lhs, const CTime &rhs) {
        lhs -= rhs.toSeconds();
        int dur1 = lhs.toSeconds();
        int dur2 = 24 * 60 * 60 - dur1;
        if (dur1 < dur2)
            return dur1;
        return dur2;
    }

    friend ostream &operator<<(ostream &os, const CTime &obj) {
        os << setfill(' ') << setw(2) << obj.m_Hour
           << setfill('0') << ":" << setw(2) << obj.m_Minute
           << ":" << setw(2) << obj.m_Second;
        return os;
    }

    friend class ::CTimeTester;
};

#ifndef __PROGTEST__

struct CTimeTester {
    static bool test(const CTime &time, int hour, int minute, int second) {
        return time.m_Hour == hour
               && time.m_Minute == minute
               && time.m_Second == second;
    }
};

#include <cassert>
#include <sstream>

int main() {
    CTime a{12, 30};
    assert(CTimeTester::test(a, 12, 30, 0));

    CTime b{13, 30};
    assert(CTimeTester::test(b, 13, 30, 0));

    assert(b - a == 3600);

    assert(CTimeTester::test(a + 60, 12, 31, 0));
    assert(CTimeTester::test(a - 60, 12, 29, 0));

    assert(a < b);
    assert(a <= b);
    assert(a != b);
    assert(!(a > b));
    assert(!(a >= b));
    assert(!(a == b));

    while (++a != b);
    assert(a == b);

    std::ostringstream output;
    assert(static_cast<bool>( output << a ));
    assert(output.str() == "13:30:00");

    assert(a++ == b++);
    assert(a == b);

    assert(--a == --b);
    assert(a == b);

    assert(a-- == b--);
    assert(a == b);

    return 0;
}

#endif /* __PROGTEST__ */
