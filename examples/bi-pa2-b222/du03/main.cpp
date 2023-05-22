#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <ctime>
#include <climits>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <memory>

using namespace std;
#endif /* __PROGTEST__ */

// uncomment if your code implements initializer lists
#define EXTENDED_SYNTAX

struct CRange {
    long long lo;
    long long hi;

    CRange() = default;

    CRange(long long lo, long long hi) : lo(lo), hi(hi) {
        if (lo > hi)
            throw logic_error("Lower bound is greater than upper bound.");
    }

    static bool cmpLo(const CRange &range, long long val) {
        return range.lo < val;
    }

    static bool cmpHi(const CRange &range, long long val) {
        return range.hi < val;
    }

    friend ostream &operator<<(ostream &out, const CRange &range) {
        ostream oldFmt(nullptr);
        oldFmt.copyfmt(out);
        out << dec;

        if (range.lo == range.hi)
            out << range.lo;
        else
            out << "<" << range.lo << ".." << range.hi << ">";

        out.copyfmt(oldFmt);
        return out;
    }
};

class CRangeList {
private:
    vector<CRange> ranges;

    auto locateLeft(long long left) const {
        return lower_bound(ranges.begin(), ranges.end(), left, CRange::cmpLo);
    }

    auto locateRight(long long right) const {
        return lower_bound(ranges.begin(), ranges.end(), right, CRange::cmpHi);
    }

public:
    // constructor
    CRangeList() = default;

    CRangeList(const CRange &range) {
        ranges.push_back(range);
    }

    CRangeList(initializer_list<CRange> list) {
        for (const CRange &range: list)
            *this += range;
    }

    using iterator = vector<CRange>::iterator;

    iterator begin() {
        return ranges.begin();
    }

    iterator end() {
        return ranges.end();
    }

    // includes long long / range
    bool includes(long long val) const {
        auto loIt = locateLeft(val);

        if (loIt != ranges.end() && loIt->lo == val)
            return true;

        if (loIt != ranges.begin() && (loIt - 1)->hi >= val)
            return true;

        return false;
    }

    bool includes(const CRange &range) const {
        long long lo = range.lo, hi = range.hi;

        auto loIt = locateLeft(lo);

        if (loIt != ranges.end() && loIt->lo == lo && loIt->hi >= hi)
            return true;

        if (loIt != ranges.begin() && (loIt - 1)->hi >= hi)
            return true;

        return false;
    }

    // += range / range list
    CRangeList &operator+=(const CRange &range) {
        long long lo = range.lo, hi = range.hi;

        auto loIt = locateLeft(lo);
        auto hiIt = locateRight(hi);

        if (loIt != ranges.begin()) {
            if ((loIt - 1)->hi >= lo
                || ((loIt - 1)->hi != LLONG_MAX && (loIt - 1)->hi + 1 == lo)) {
                lo = (loIt - 1)->lo;
                --loIt;
            }
        }

        if (hiIt != ranges.end()) {
            if (hiIt->lo <= hi
                || (hiIt->lo != LLONG_MIN && hiIt->lo - 1 == hi)) {
                hi = hiIt->hi;
                ++hiIt;
            }
        }

        ranges.erase(loIt, hiIt);
        ranges.insert(locateLeft(lo), CRange(lo, hi));

        return *this;
    }

    CRangeList &operator+=(const CRangeList &list) {
        for (const CRange &range: list.ranges)
            *this += range;
        return *this;
    }

    // -= range / range list
    CRangeList &operator-=(const CRange &range) {
        long long lo = range.lo, hi = range.hi;
        bool loRecreate = false, hiRecreate = false;
        CRange loTmp{}, hiTmp{};

        auto loIt = locateLeft(lo);
        auto hiIt = locateRight(hi);

        if (loIt != ranges.begin() && (loIt - 1)->hi >= lo) {
            if (lo != LLONG_MIN && (loIt - 1)->lo != lo) {
                loTmp = CRange((loIt - 1)->lo, lo - 1);
                loRecreate = true;
            }
            --loIt;
        }

        if (hiIt != ranges.end() && hiIt->lo <= hi) {
            if (hi != LLONG_MAX && hiIt->hi != hi) {
                hiTmp = CRange(hi + 1, hiIt->hi);
                hiRecreate = true;
            }
            ++hiIt;
        }

        ranges.erase(loIt, hiIt);
        if (loRecreate)
            ranges.insert(locateLeft(loTmp.lo), loTmp);
        if (hiRecreate)
            ranges.insert(locateLeft(hiTmp.lo), hiTmp);

        return *this;
    }

    CRangeList &operator-=(const CRangeList &list) {
        for (const CRange &range: list.ranges)
            *this -= range;
        return *this;
    }

    // = range / range list
    // operator ==
    friend bool operator==(const CRangeList &lhs, const CRangeList &rhs) {
        return lhs.ranges == rhs.ranges;
    }

    // operator !=
    friend bool operator!=(const CRangeList &lhs, const CRangeList &rhs) {
        return !(lhs == rhs);
    }

    // operator <<
    friend ostream &operator<<(ostream &out, const CRangeList &list) {
        ostream oldFmt(nullptr);
        oldFmt.copyfmt(out);
        out << dec;

        out << "{";

        for (auto it = list.ranges.begin(); it != list.ranges.end(); ++it) {
            out << *it;
            if (it != (list.ranges.end() - 1))
                out << ",";
        }

        out << "}";

        out.copyfmt(oldFmt);
        return out;
    }
};

bool operator==(const CRange &left, const CRange &right) {
    return left.lo == right.lo && left.hi == right.hi;
}

CRangeList operator+(const CRange &left, const CRange &right) {
    return CRangeList(left) += right;
}

CRangeList operator+(CRangeList list, const CRange &right) {
    return list += right;
}

CRangeList operator-(const CRange &left, const CRange &right) {
    return CRangeList(left) -= right;
}

CRangeList operator-(CRangeList list, const CRange &right) {
    return list -= right;
}

#ifndef __PROGTEST__

string toString(const CRangeList &x) {
    ostringstream oss;
    oss << x;
    return oss.str();
}

int main() {
    CRangeList a, b;

    assert(sizeof(CRange) <= 2 * sizeof(long long));
    a = CRange(5, 10);
    a += CRange(25, 100);
    assert(toString(a) == "{<5..10>,<25..100>}");
    a += CRange(-5, 0);

    assert(a.includes(5));
    assert(a.includes(6));
    assert(a.includes(10));
    assert(!a.includes(4));
    assert(!a.includes(13));

    a += CRange(8, 50);
    assert(toString(a) == "{<-5..0>,<5..100>}");
    a += CRange(101, 105) + CRange(120, 150) + CRange(160, 180) + CRange(190, 210);
    assert(toString(a) == "{<-5..0>,<5..105>,<120..150>,<160..180>,<190..210>}");
    a += CRange(106, 119) + CRange(152, 158);
    assert(toString(a) == "{<-5..0>,<5..150>,<152..158>,<160..180>,<190..210>}");
    a += CRange(-3, 170);
    a += CRange(-30, 1000);
    assert(toString(a) == "{<-30..1000>}");
    b = CRange(-500, -300) + CRange(2000, 3000) + CRange(700, 1001);
    a += b;
    assert(toString(a) == "{<-500..-300>,<-30..1001>,<2000..3000>}");
    a -= CRange(-400, -400);
    assert(toString(a) == "{<-500..-401>,<-399..-300>,<-30..1001>,<2000..3000>}");
    a -= CRange(10, 20) + CRange(900, 2500) + CRange(30, 40) + CRange(10000, 20000);
    assert(toString(a) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}");
    try {
        a += CRange(15, 18) + CRange(10, 0) + CRange(35, 38);
        assert("Exception not thrown" == nullptr);
    }
    catch (const std::logic_error &e) {
    }
    catch (...) {
        assert("Invalid exception thrown" == nullptr);
    }
    assert(toString(a) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}");
    b = a;
    assert(a == b);
    assert(!(a != b));
    b += CRange(2600, 2700);
    assert(toString(b) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}");
    assert(a == b);
    assert(!(a != b));
    b += CRange(15, 15);
    assert(toString(b) == "{<-500..-401>,<-399..-300>,<-30..9>,15,<21..29>,<41..899>,<2501..3000>}");
    assert(!(a == b));
    assert(a != b);
    assert(b.includes(15));
    assert(b.includes(2900));
    assert(b.includes(CRange(15, 15)));
    assert(b.includes(CRange(-350, -350)));
    assert(b.includes(CRange(100, 200)));
    assert(!b.includes(CRange(800, 900)));
    assert(!b.includes(CRange(-1000, -450)));
    assert(!b.includes(CRange(0, 500)));
    a += CRange(-10000, 10000) + CRange(10000000, 1000000000);
    assert(toString(a) == "{<-10000..10000>,<10000000..1000000000>}");
    b += a;
    assert(toString(b) == "{<-10000..10000>,<10000000..1000000000>}");
    b -= a;
    assert(toString(b) == "{}");
    b += CRange(0, 100) + CRange(200, 300) - CRange(150, 250) + CRange(160, 180) - CRange(170, 170);
    assert(toString(b) == "{<0..100>,<160..169>,<171..180>,<251..300>}");
    b -= CRange(10, 90) - CRange(20, 30) - CRange(40, 50) - CRange(60, 90) + CRange(70, 80);
    assert(toString(b) == "{<0..9>,<20..30>,<40..50>,<60..69>,<81..100>,<160..169>,<171..180>,<251..300>}");

#ifdef EXTENDED_SYNTAX
    CRangeList x{{5,   20},
                 {150, 200},
                 {-9,  12},
                 {48,  93}};
    assert(toString(x) == "{<-9..20>,<48..93>,<150..200>}");
    ostringstream oss;
    oss << setfill('=') << hex << left;
    for (const auto &v: x + CRange(-100, -100))
        oss << v << endl;
    oss << setw(10) << 1024;
    assert(oss.str() == "-100\n<-9..20>\n<48..93>\n<150..200>\n400=======");
#endif /* EXTENDED_SYNTAX */

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
