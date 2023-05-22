#ifndef __PROGTEST__

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include <algorithm>
#include <functional>

#include <stdexcept>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cstdint>
#include <cctype>
#include <cassert>

using namespace std;
#endif /* __PROGTEST__ */

template<typename T>
class CSet {
private:
    T **m_Arr;
    size_t m_Size;
    size_t m_Cap;

    static bool eq(const T &lhs, const T &rhs) {
        if (lhs < rhs || rhs < lhs)
            return false;
        return true;
    }

    void append(const T &x) {
        if (m_Size >= m_Cap) {
            m_Cap *= 2;
            T **newArr = new T *[m_Cap];
            for (size_t i = 0; i < m_Size; i++)
                newArr[i] = m_Arr[i];
            delete[] m_Arr;
            m_Arr = newArr;
        }

        m_Arr[m_Size++] = new T(x);
    }

    void erase(size_t pos) {
        T **newArr = new T *[m_Cap];
        for (size_t i = 0; i < pos; i++)
            newArr[i] = m_Arr[i];
        delete m_Arr[pos];
        for (size_t i = pos + 1; i < m_Size; i++)
            newArr[i - 1] = m_Arr[i];
        delete[] m_Arr;
        m_Arr = newArr;
        m_Size--;
    }

public:
    // default constructor
    CSet() : m_Arr(new T *[1]), m_Size(0), m_Cap(1) {}

    // copy constructor
    CSet(const CSet &other) : m_Arr(new T *[other.m_Size]), m_Size(other.m_Size), m_Cap(other.m_Size) {
        for (size_t i = 0; i < m_Size; i++)
            m_Arr[i] = new T(*(other.m_Arr[i]));
    }

    // operator=
    CSet &operator=(CSet other) {
        swap(m_Arr, other.m_Arr);
        swap(m_Size, other.m_Size);
        swap(m_Cap, other.m_Cap);
        return *this;
    }

    // destructor
    ~CSet() {
        for (size_t i = 0; i < m_Size; i++)
            delete m_Arr[i];
        delete[] m_Arr;
    }

    // Insert
    bool Insert(const T &x) {
        size_t i;
        for (i = 0; i < m_Size; i++)
            if (eq(x, *(m_Arr[i])))
                return false;
        append(x);
        return true;
    }

    // Remove
    bool Remove(const T &x) {
        size_t i;
        for (i = 0; i < m_Size; i++)
            if (eq(x, *(m_Arr[i]))) {
                erase(i);
                return true;
            }
        return false;
    }

    // Contains
    bool Contains(const T &x) {
        for (size_t i = 0; i < m_Size; i++)
            if (eq(x, *(m_Arr[i])))
                return true;
        return false;
    }

    // Size
    size_t Size() const { return m_Size; }
};

#ifndef __PROGTEST__

#include <cassert>

struct CSetTester {
    static void test0() {
        CSet<string> x0;
        assert(x0.Insert("Jerabek Michal"));
        assert(x0.Insert("Pavlik Ales"));
        assert(x0.Insert("Dusek Zikmund"));
        assert(x0.Size() == 3);
        assert(x0.Contains("Dusek Zikmund"));
        assert(!x0.Contains("Pavlik Jan"));
        assert(x0.Remove("Jerabek Michal"));
        assert(!x0.Remove("Pavlik Jan"));
    }

    static void test1() {
        CSet<string> x0;
        assert(x0.Insert("Jerabek Michal"));
        assert(x0.Insert("Pavlik Ales"));
        assert(x0.Insert("Dusek Zikmund"));
        assert(x0.Size() == 3);
        assert(x0.Contains("Dusek Zikmund"));
        assert(!x0.Contains("Pavlik Jan"));
        assert(x0.Remove("Jerabek Michal"));
        assert(!x0.Remove("Pavlik Jan"));
        CSet<string> x1(x0);
        assert(x0.Insert("Vodickova Saskie"));
        assert(x1.Insert("Kadlecova Kvetslava"));
        assert(x0.Size() == 3);
        assert(x1.Size() == 3);
        assert(x0.Contains("Vodickova Saskie"));
        assert(!x1.Contains("Vodickova Saskie"));
        assert(!x0.Contains("Kadlecova Kvetslava"));
        assert(x1.Contains("Kadlecova Kvetslava"));
    }

    static void test2() {
        CSet<string> x0;
        CSet<string> x1;
        assert(x0.Insert("Jerabek Michal"));
        assert(x0.Insert("Pavlik Ales"));
        assert(x0.Insert("Dusek Zikmund"));
        assert(x0.Size() == 3);
        assert(x0.Contains("Dusek Zikmund"));
        assert(!x0.Contains("Pavlik Jan"));
        assert(x0.Remove("Jerabek Michal"));
        assert(!x0.Remove("Pavlik Jan"));
        x1 = x0;
        assert(x0.Insert("Vodickova Saskie"));
        assert(x1.Insert("Kadlecova Kvetslava"));
        assert(x0.Size() == 3);
        assert(x1.Size() == 3);
        assert(x0.Contains("Vodickova Saskie"));
        assert(!x1.Contains("Vodickova Saskie"));
        assert(!x0.Contains("Kadlecova Kvetslava"));
        assert(x1.Contains("Kadlecova Kvetslava"));
    }

    static void test4() {
        CSet<int> x0;
        assert(x0.Insert(4));
        assert(x0.Insert(8));
        assert(x0.Insert(1));
        assert(x0.Size() == 3);
        assert(x0.Contains(4));
        assert(!x0.Contains(5));
        assert(!x0.Remove(5));
        assert(x0.Remove(4));
    }

};

int main() {
    CSetTester::test0();
    CSetTester::test1();
    CSetTester::test2();
    CSetTester::test4();

    auto s = CSet<int>();

    assert(s.Insert(1));
    assert(s.Insert(3));
    assert(s.Insert(6));
    assert(s.Insert(4));
    assert(s.Insert(2));

    assert(!s.Insert(1));
    assert(!s.Insert(2));
    assert(!s.Insert(3));
    assert(!s.Insert(6));
    assert(!s.Insert(2));
    assert(!s.Insert(1));

    auto s2 = CSet(s);

    assert(s2.Remove(1));
    assert(s2.Remove(2));
    assert(!s2.Remove(2));
    assert(!s.Insert(1));
    assert(s2.Insert(1));
    assert(s2.Insert(2));

    assert(s.Size() == 5);
    assert(s.Remove(1));
    assert(s.Size() == 4);

    assert(s.Contains(2));
    assert(s.Contains(3));
    assert(s.Contains(6));
    assert(!s.Contains(9));

    return 0;
}

#endif /* __PROGTEST__ */