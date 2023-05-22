#ifndef __PROGTEST__

#include <cstring>

using namespace std;

class CLinkedSetTester;

#endif /* __PROGTEST__ */


class CLinkedSet {
private:
    struct CNode {
        CNode *m_Next;
        CNode *m_Prev;

        size_t m_Size;
        char *m_Str;

        explicit CNode(const char *str) : m_Next(nullptr), m_Prev(nullptr), m_Size(strlen(str) + 1) {
            m_Str = new char[m_Size];
            memcpy(m_Str, str, m_Size);
        }

        CNode(const CNode &other) : m_Next(nullptr), m_Prev(nullptr), m_Size(other.m_Size),
                                    m_Str(new char[other.m_Size]) {
            memcpy(m_Str, other.m_Str, m_Size);
        }

        ~CNode() {
            delete[] m_Str;
        }

        const char *Value() const {
            return m_Str;
        }
    };

    CNode *m_Begin;
    CNode *m_End;
    size_t m_Size;

public:
    // default constructor
    CLinkedSet() : m_Begin(nullptr), m_End(nullptr), m_Size(0) {}

    // copy constructor
    CLinkedSet(const CLinkedSet &set) : m_Begin(nullptr), m_End(nullptr), m_Size(set.m_Size) {
        if (m_Size == 0)
            return;

        m_Begin = new CNode(*set.m_Begin);
        m_End = m_Begin;

        for (CNode *node = set.m_Begin->m_Next; node; node = node->m_Next) {
            auto *next = new CNode(*node);
            m_End->m_Next = next;
            next->m_Prev = m_End;
            m_End = next;
        }
    }

    // operator=
    CLinkedSet &operator=(const CLinkedSet &other) {
        if (this == &other)
            return *this;

        CLinkedSet twin(other);

        CNode *dummy_Begin = m_Begin;
        CNode *dummy_End = m_End;
        size_t dummy_Size = m_Size;

        m_Begin = twin.m_Begin;
        m_End = twin.m_End;
        m_Size = twin.m_Size;

        twin.m_Begin = dummy_Begin;
        twin.m_End = dummy_End;
        twin.m_Size = dummy_Size;

        return *this;
    }

    // destructor
    ~CLinkedSet() {
        for (CNode *node = m_Begin; node;) {
            CNode *tmp = node;
            node = node->m_Next;
            delete tmp;
        }
    }

    bool Insert(const char *value) {
        auto *newNode = new CNode(value);
        if (m_Size == 0) {
            m_Begin = newNode;
            m_End = newNode;
            m_Size++;
            return true;
        }

        if (strcmp(value, m_Begin->m_Str) < 0) {
            m_Begin->m_Prev = newNode;
            newNode->m_Next = m_Begin;
            m_Begin = newNode;

            m_Size++;
            return true;
        }

        for (CNode *node = m_Begin; node; node = node->m_Next) {
            int cmpRes = strcmp(value, node->Value());
            if (cmpRes == 0) {
                delete newNode;
                return false;
            }

            if (cmpRes < 0) {
                newNode->m_Next = node;
                newNode->m_Prev = node->m_Prev;

                node->m_Prev->m_Next = newNode;
                node->m_Prev = newNode;

                m_Size++;
                return true;
            }
        }

        m_End->m_Next = newNode;
        newNode->m_Prev = m_End;
        m_End = newNode;

        m_Size++;
        return true;
    }

    bool Remove(const char *value) {
        size_t valSize = strlen(value) + 1;
        for (CNode *node = m_Begin; node; node = node->m_Next) {
            if (valSize == node->m_Size && strcmp(value, node->m_Str) == 0) {
                if (node == m_Begin && node == m_End) {
                    m_Begin = nullptr;
                    m_End = nullptr;
                    delete node;

                    m_Size--;
                    return true;
                }

                if (node == m_Begin) {
                    m_Begin = node->m_Next;
                    m_Begin->m_Prev = nullptr;
                    delete node;

                    m_Size--;
                    return true;
                } else if (node == m_End) {
                    m_End = node->m_Prev;
                    m_End->m_Next = nullptr;
                    delete node;

                    m_Size--;
                    return true;
                } else {
                    node->m_Prev->m_Next = node->m_Next;
                    node->m_Next->m_Prev = node->m_Prev;
                    delete node;

                    m_Size--;
                    return true;
                }
            }
        }
        return false;
    }

    bool Empty() const {
        return m_Size == 0;
    }

    size_t Size() const {
        return m_Size;
    }

    bool Contains(const char *value) const {
        size_t valSize = strlen(value) + 1;
        for (CNode *node = m_Begin; node; node = node->m_Next)
            if (valSize == node->m_Size && strcmp(value, node->m_Str) == 0)
                return true;
        return false;
    }

    friend class ::CLinkedSetTester;
};

#ifndef __PROGTEST__

#include <cassert>

struct CLinkedSetTester {
    static void test0() {
        CLinkedSet x0;
        assert(x0.Insert("Jerabek Michal"));
        assert(x0.Insert("Pavlik Ales"));
        assert(x0.Insert("Dusek Zikmund"));
        assert(x0.Size() == 3);
        assert(x0.Contains("Dusek Zikmund"));
        assert(!x0.Contains("Pavlik Jan"));
        assert(x0.Remove("Jerabek Michal"));
        assert(!x0.Remove("Pavlik Jan"));
        assert(!x0.Empty());
    }

    static void test1() {
        CLinkedSet x0;
        assert(x0.Insert("Jerabek Michal"));
        assert(x0.Insert("Pavlik Ales"));
        assert(x0.Insert("Dusek Zikmund"));
        assert(x0.Size() == 3);
        assert(x0.Contains("Dusek Zikmund"));
        assert(!x0.Contains("Pavlik Jan"));
        assert(x0.Remove("Jerabek Michal"));
        assert(!x0.Remove("Pavlik Jan"));
        CLinkedSet x1(x0);
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
        CLinkedSet x0;
        CLinkedSet x1;
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

};

int main() {
    CLinkedSetTester::test0();
    CLinkedSetTester::test1();
    CLinkedSetTester::test2();
    return 0;
}

#endif /* __PROGTEST__ */
