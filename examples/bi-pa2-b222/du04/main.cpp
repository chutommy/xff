#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;
#endif /* __PROGTEST__ */

template<typename T>
class CVector {
private:
    size_t m_Size = 0;
    size_t m_Capacity = 1;
    T *m_Data = nullptr;

public:
    CVector() : m_Size(0), m_Capacity(1), m_Data(new T[1]) {}

    CVector(const T *data, size_t size) : m_Size(size), m_Capacity(size), m_Data(new T[size]) {
        memcpy(m_Data, data, size);
    }

    CVector(const CVector &other) : m_Size(other.m_Size), m_Capacity(other.m_Capacity),
                                    m_Data(new T[other.m_Capacity]) {
        for (size_t i = 0; i < m_Size; ++i)
            m_Data[i] = other.m_Data[i];
    }

    CVector &operator=(CVector other) {
        if (this == &other)
            return *this;

        swap(m_Data, other.m_Data);
        swap(m_Size, other.m_Size);
        swap(m_Capacity, other.m_Capacity);

        return *this;
    }

    ~CVector() {
        delete[] m_Data;
    }

    const T &operator[](size_t idx) const {
        return m_Data[idx];
    }

    T &operator[](size_t idx) {
        return m_Data[idx];
    }

    void push_back(const T &elem) {
        if (m_Size >= m_Capacity) {
            m_Capacity *= 2;
            T *tmpData = new T[m_Capacity];
            for (size_t i = 0; i < m_Size; ++i)
                tmpData[i] = m_Data[i];

            delete[] m_Data;
            m_Data = tmpData;
        }

        m_Data[m_Size++] = elem;
    }

    void insert(size_t idx, const T &elem) {
        if (m_Size >= m_Capacity) {
            m_Capacity *= 2;

            T *tmpData = new T[m_Capacity];
            for (size_t i = 0; i < idx; ++i)
                tmpData[i] = m_Data[i];

            tmpData[idx] = elem;
            ++m_Size;

            for (size_t i = idx + 1; i < m_Size; ++i)
                tmpData[i] = m_Data[i - 1];

            delete[] m_Data;
            m_Data = tmpData;

        } else {
            ++m_Size;
            for (size_t i = m_Size - 1; i > idx; --i)
                m_Data[i] = m_Data[i - 1];
            m_Data[idx] = elem;
        }
    }

    size_t size() const {
        return m_Size;
    }

    bool operator==(const CVector &other) const {
        if (m_Size != other.m_Size)
            return false;

        for (size_t i = 0; i < m_Size; ++i)
            if (m_Data[i] != other.m_Data[i])
                return false;

        return true;
    }

    bool operator<(const CVector &other) const {
        for (size_t i = 0; i < m_Size; ++i)
            if (m_Data[i] != other.m_Data[i])
                return m_Data[i] < other.m_Data[i];

        if (m_Size < other.m_Size)
            return true;

        return false;
    }

    friend ostream &operator<<(ostream &os, const CVector &s) {
        return os << s.m_Data;
    }
};

template<typename T>
class CSharedVector {
    CVector<T> *m_Source;
    int *m_ReferenceCount;

    void copyOnWrite() {
        if (*m_ReferenceCount == 1)
            return;

        auto newPtr = new CVector<T>(*m_Source);
        --(*m_ReferenceCount);

        m_Source = newPtr;
        m_ReferenceCount = new int(1);
    }

public:
    CSharedVector() : m_Source(new CVector<T>()), m_ReferenceCount(new int(1)) {};

    CSharedVector(const CSharedVector &other) : m_Source(other.m_Source), m_ReferenceCount(other.m_ReferenceCount) {
        *m_ReferenceCount += 1;
    }

    ~CSharedVector() {
        --(*m_ReferenceCount);
        if (*m_ReferenceCount == 0) {
            delete m_Source;
            delete m_ReferenceCount;
        }
    }

    CSharedVector &operator=(CSharedVector other) {
        swap(m_Source, other.m_Source);
        swap(m_ReferenceCount, other.m_ReferenceCount);
        return *this;
    }

    void insert(size_t idx, const T &elem) {
        copyOnWrite();
        m_Source->insert(idx, elem);
    }

    friend ostream &operator<<(ostream &oss, const CSharedVector &other) {
        oss << *other.m_Source;
        return oss;
    }

    const T &operator[](size_t idx) const {
        return (*m_Source)[idx];
    }

    T &operator[](size_t idx) {
        copyOnWrite();
        return (*m_Source)[idx];
    }

    void push_back(const T &elem) {
        copyOnWrite();
        m_Source->push_back(elem);
    }

    size_t size() const {
        return m_Source->size();
    }

    bool operator==(const CSharedVector &other) const {
        return m_Source->operator==(other);
    }

    bool operator<(const CSharedVector &other) const {
        return m_Source->operator<(other);
    }
};


class CStr {
private:
    CVector<char> m_Data;

public:
    CStr() = default;

    explicit CStr(const char *str) : m_Data(str, strlen(str) + 1) {}

    bool operator<(const CStr &rhs) const {
        return m_Data < rhs.m_Data;
    }

    friend bool operator==(const CStr &lhs, const CStr &rhs) {
        return lhs.m_Data == rhs.m_Data;
    }

    friend bool operator!=(const CStr &lhs, const CStr &rhs) {
        return !(lhs == rhs);
    }

    friend ostream &operator<<(ostream &os, const CStr &s) {
        return os << s.m_Data;
    }
};

class CMail {
public:
    CStr m_Sender;
    CStr m_Recipient;
    CStr m_Body;

    CMail() = default;

    CMail(const char *from, const char *to, const char *body)
            : m_Sender(from),
              m_Recipient(to),
              m_Body(body) {}

    bool operator==(const CMail &other) const {
        return (m_Sender == other.m_Sender
                && m_Recipient == other.m_Recipient
                && m_Body == other.m_Body);
    }

    friend ostream &operator<<(ostream &os, const CMail &m) {
        return os << "From: " << m.m_Sender
                  << ", To: " << m.m_Recipient
                  << ", Body: " << m.m_Body;
    }
};

class CItem {
public:
    CMail m_Mail;
    CItem *m_Next = nullptr;

    explicit CItem(const CMail &mail) : m_Mail(mail) {}
};

class CBox {
public:
    CItem *m_Begin = nullptr;
    CItem *m_End = nullptr;
    size_t m_Size = 0;

    CBox() = default;

    CBox(const CBox &box) {
        if (box.m_Size == 0)
            return;

        m_Begin = new CItem(*box.m_Begin);
        m_End = m_Begin;
        m_Size = box.m_Size;
        for (CItem *i = box.m_Begin->m_Next; i; i = i->m_Next) {
            auto newItem = new CItem(*i);

            m_End->m_Next = newItem;
            m_End = newItem;
        }
    }

    CBox &operator=(CBox twin) {
        swap(m_Begin, twin.m_Begin);
        swap(m_End, twin.m_End);
        swap(m_Size, twin.m_Size);

        return *this;
    }

    ~CBox() {
        CItem *tmp;
        while (m_Begin) {
            tmp = m_Begin;
            m_Begin = m_Begin->m_Next;
            delete tmp;
        }
    }

    void push_back(const CMail &m) {
        auto *item = new CItem(m);
        ++m_Size;

        if (m_Begin == nullptr) {
            m_Begin = item;
            m_End = item;
            return;
        }

        m_End->m_Next = item;
        m_End = item;
    }
};

class CSharedBox {
    void copyOnWrite() {
        if (*m_ReferenceCount == 1)
            return;

        auto newPtr = new CBox(*m_Source);
        --(*m_ReferenceCount);

        m_Source = newPtr;
        m_ReferenceCount = new int(1);
    }

public:

    CBox *m_Source;
    int *m_ReferenceCount;

    CSharedBox() : m_Source(new CBox()), m_ReferenceCount(new int(1)) {};

    CSharedBox(const CSharedBox &other) : m_Source(other.m_Source), m_ReferenceCount(other.m_ReferenceCount) {
        *m_ReferenceCount += 1;
    }

    ~CSharedBox() {
        --(*m_ReferenceCount);
        if (*m_ReferenceCount == 0) {
            delete m_Source;
            delete m_ReferenceCount;
        }
    }

    CSharedBox &operator=(CSharedBox other) {
        swap(m_Source, other.m_Source);
        swap(m_ReferenceCount, other.m_ReferenceCount);
        return *this;
    }

    void push_back(const CMail &m) {
        copyOnWrite();
        m_Source->push_back(m);
    }

};

class CUser {
public:
    CStr m_Name;
    CSharedBox m_Outbox;
    CSharedBox m_Inbox;

    CUser() = default;

    explicit CUser(CStr name) : m_Name(std::move(name)) {}

    void addOutbox(const CMail &m) {
        m_Outbox.push_back(m);
    }

    void addInbox(const CMail &m) {
        m_Inbox.push_back(m);
    }
};

class CMailIterator {
private:
    CItem *m_Item;
    size_t m_Duration;

public:

    CMailIterator() = default;

    explicit CMailIterator(CItem *item, size_t duration) : m_Item(item), m_Duration(duration) {}

    explicit operator bool() const {
        if (m_Duration == 0)
            return false;
        return true;
    }

    bool operator!() const {
        if (m_Duration == 0)
            return true;
        return false;
    }

    const CMail &operator*() {
        return m_Item->m_Mail;
    }

    CMailIterator &operator++() {
        --m_Duration;
        m_Item = m_Item->m_Next;
        return *this;
    }
};

class CMailServerData {
private:
    CSharedVector<CUser *> m_Users;

    int searchRec(const CStr &name, int lo, int hi) const {
        if (lo > hi)
            return lo;

        int mid = lo + (hi - lo) / 2;

        if (name == m_Users[mid]->m_Name)
            return mid;
        if (name < m_Users[mid]->m_Name)
            return searchRec(name, lo, mid - 1);

        return searchRec(name, mid + 1, hi);
    }

    size_t search(const CStr &name) const {
        return searchRec(name, 0, int(m_Users.size()) - 1);
    }

    void upsert(const CStr &name) {
        size_t idx = search(name);
        if (idx == m_Users.size() || name != m_Users[idx]->m_Name)
            m_Users.insert(idx, new CUser(name));
    }

public:
    CMailServerData() = default;

    CMailServerData(const CMailServerData &s) {
        for (size_t i = 0; i < s.m_Users.size(); i++)
            m_Users.push_back(new CUser(*(s.m_Users[i])));
    }

    CMailServerData &operator=(CMailServerData s) {
        swap(m_Users, s.m_Users);
        return *this;
    }

    ~CMailServerData() {
        for (size_t i = 0; i < m_Users.size(); i++)
            delete m_Users[i];
    }

    void sendMail(const CMail &m) {
        upsert(m.m_Sender);
        upsert(m.m_Recipient);

        size_t idx = search(CStr(m.m_Sender));
        m_Users[idx]->addOutbox(m);

        idx = search(CStr(m.m_Recipient));
        m_Users[idx]->addInbox(m);
    }

    CMailIterator outbox(const char *email) const {
        size_t idx = search(CStr(email));
        if (idx != m_Users.size() && CStr(email) == m_Users[idx]->m_Name)
            return CMailIterator(m_Users[idx]->m_Outbox.m_Source->m_Begin, m_Users[idx]->m_Outbox.m_Source->m_Size);
        return {};
    }

    CMailIterator inbox(const char *email) const {
        size_t idx = search(CStr(email));
        if (idx != m_Users.size() && CStr(email) == m_Users[idx]->m_Name) {
            auto it = CMailIterator(m_Users[idx]->m_Inbox.m_Source->m_Begin, m_Users[idx]->m_Inbox.m_Source->m_Size);
            return it;
        }
        return {};
    }
};

class CMailServer {
    void copyOnWrite() {
        if (*m_ReferenceCount == 1)
            return;

        auto newPtr = new CMailServerData(*m_Source);
        --(*m_ReferenceCount);

        m_Source = newPtr;
        m_ReferenceCount = new int(1);
    }

public:

    CMailServerData *m_Source;
    int *m_ReferenceCount;

    CMailServer() : m_Source(new CMailServerData()), m_ReferenceCount(new int(1)) {};

    CMailServer(const CMailServer &other) : m_Source(other.m_Source),
                                            m_ReferenceCount(other.m_ReferenceCount) {
        *m_ReferenceCount += 1;
    }

    ~CMailServer() {
        --(*m_ReferenceCount);
        if (*m_ReferenceCount == 0) {
            delete m_Source;
            delete m_ReferenceCount;
        }
    }

    CMailServer &operator=(CMailServer other) {
        swap(m_Source, other.m_Source);
        swap(m_ReferenceCount, other.m_ReferenceCount);
        return *this;
    }


    void sendMail(const CMail &m) {
        copyOnWrite();
        m_Source->sendMail(m);
    }

    CMailIterator outbox(const char *email) const {
        return m_Source->outbox(email);
    }

    CMailIterator inbox(const char *email) const {
        return m_Source->inbox(email);
    }
};

#ifndef __PROGTEST__

bool matchOutput(const CMail &m, const char *str) {
    ostringstream oss;
    oss << m;
    return oss.str() == str;
}

int main() {
    char from[100], to[100], body[1024];

    assert(CMail("john", "peter", "progtest deadline") == CMail("john", "peter", "progtest deadline"));
    assert(!(CMail("john", "peter", "progtest deadline") == CMail("john", "progtest deadline", "peter")));
    assert(!(CMail("john", "peter", "progtest deadline") == CMail("peter", "john", "progtest deadline")));
    assert(!(CMail("john", "peter", "progtest deadline") == CMail("peter", "progtest deadline", "john")));
    assert(!(CMail("john", "peter", "progtest deadline") == CMail("progtest deadline", "john", "peter")));
    assert(!(CMail("john", "peter", "progtest deadline") == CMail("progtest deadline", "peter", "john")));
    CMailServer s0;
    s0.sendMail(CMail("john", "peter", "some important mail"));
    strncpy(from, "john", sizeof(from));
    strncpy(to, "thomas", sizeof(to));
    strncpy(body, "another important mail", sizeof(body));
    s0.sendMail(CMail(from, to, body));
    strncpy(from, "john", sizeof(from));
    strncpy(to, "alice", sizeof(to));
    strncpy(body, "deadline notice", sizeof(body));
    s0.sendMail(CMail(from, to, body));
    s0.sendMail(CMail("alice", "john", "deadline confirmation"));
    s0.sendMail(CMail("peter", "alice", "PR bullshit"));
    CMailIterator i0 = s0.inbox("alice");
    assert(i0 && *i0 == CMail("john", "alice", "deadline notice"));
    assert(matchOutput(*i0, "From: john, To: alice, Body: deadline notice"));
    assert(++i0 && *i0 == CMail("peter", "alice", "PR bullshit"));
    assert(matchOutput(*i0, "From: peter, To: alice, Body: PR bullshit"));
    assert(!++i0);

    CMailIterator i1 = s0.inbox("john");
    assert(i1 && *i1 == CMail("alice", "john", "deadline confirmation"));
    assert(matchOutput(*i1, "From: alice, To: john, Body: deadline confirmation"));
    assert(!++i1);

    CMailIterator i2 = s0.outbox("john");
    assert(i2 && *i2 == CMail("john", "peter", "some important mail"));
    assert(matchOutput(*i2, "From: john, To: peter, Body: some important mail"));
    assert(++i2 && *i2 == CMail("john", "thomas", "another important mail"));
    assert(matchOutput(*i2, "From: john, To: thomas, Body: another important mail"));
    assert(++i2 && *i2 == CMail("john", "alice", "deadline notice"));
    assert(matchOutput(*i2, "From: john, To: alice, Body: deadline notice"));
    assert(!++i2);

    CMailIterator i3 = s0.outbox("thomas");
    assert(!i3);

    CMailIterator i4 = s0.outbox("steve");
    assert(!i4);

    CMailIterator i5 = s0.outbox("thomas");
    s0.sendMail(CMail("thomas", "boss", "daily report"));
    assert(!i5);

    CMailIterator i6 = s0.outbox("thomas");
    assert(i6 && *i6 == CMail("thomas", "boss", "daily report"));
    assert(matchOutput(*i6, "From: thomas, To: boss, Body: daily report"));
    assert(!++i6);

    CMailIterator i7 = s0.inbox("alice");
    s0.sendMail(CMail("thomas", "alice", "meeting details"));
    assert(i7 && *i7 == CMail("john", "alice", "deadline notice"));
    assert(matchOutput(*i7, "From: john, To: alice, Body: deadline notice"));
    assert(++i7 && *i7 == CMail("peter", "alice", "PR bullshit"));
    assert(matchOutput(*i7, "From: peter, To: alice, Body: PR bullshit"));
    assert(!++i7);

    CMailIterator i8 = s0.inbox("alice");
    assert(i8 && *i8 == CMail("john", "alice", "deadline notice"));
    assert(matchOutput(*i8, "From: john, To: alice, Body: deadline notice"));
    assert(++i8 && *i8 == CMail("peter", "alice", "PR bullshit"));
    assert(matchOutput(*i8, "From: peter, To: alice, Body: PR bullshit"));
    assert(++i8 && *i8 == CMail("thomas", "alice", "meeting details"));
    assert(matchOutput(*i8, "From: thomas, To: alice, Body: meeting details"));
    assert(!++i8);

    CMailServer s1(s0);
    s0.sendMail(CMail("joe", "alice", "delivery details"));
    s1.sendMail(CMail("sam", "alice", "order confirmation"));
    CMailIterator i9 = s0.inbox("alice");
    assert(i9 && *i9 == CMail("john", "alice", "deadline notice"));
    assert(matchOutput(*i9, "From: john, To: alice, Body: deadline notice"));
    assert(++i9 && *i9 == CMail("peter", "alice", "PR bullshit"));
    assert(matchOutput(*i9, "From: peter, To: alice, Body: PR bullshit"));
    assert(++i9 && *i9 == CMail("thomas", "alice", "meeting details"));
    assert(matchOutput(*i9, "From: thomas, To: alice, Body: meeting details"));
    assert(++i9 && *i9 == CMail("joe", "alice", "delivery details"));
    assert(matchOutput(*i9, "From: joe, To: alice, Body: delivery details"));
    assert(!++i9);

    CMailIterator i10 = s1.inbox("alice");
    assert(i10 && *i10 == CMail("john", "alice", "deadline notice"));
    assert(matchOutput(*i10, "From: john, To: alice, Body: deadline notice"));
    assert(++i10 && *i10 == CMail("peter", "alice", "PR bullshit"));
    assert(matchOutput(*i10, "From: peter, To: alice, Body: PR bullshit"));
    assert(++i10 && *i10 == CMail("thomas", "alice", "meeting details"));
    assert(matchOutput(*i10, "From: thomas, To: alice, Body: meeting details"));
    assert(++i10 && *i10 == CMail("sam", "alice", "order confirmation"));
    assert(matchOutput(*i10, "From: sam, To: alice, Body: order confirmation"));
    assert(!++i10);

    CMailServer s2;
    s2.sendMail(CMail("alice", "alice", "mailbox test"));
    CMailIterator i11 = s2.inbox("alice");
    assert(i11 && *i11 == CMail("alice", "alice", "mailbox test"));
    assert(matchOutput(*i11, "From: alice, To: alice, Body: mailbox test"));
    assert(!++i11);

    s2 = s0;
    s0.sendMail(CMail("steve", "alice", "newsletter"));
    s2.sendMail(CMail("paul", "alice", "invalid invoice"));
    CMailIterator i12 = s0.inbox("alice");
    assert(i12 && *i12 == CMail("john", "alice", "deadline notice"));
    assert(matchOutput(*i12, "From: john, To: alice, Body: deadline notice"));
    assert(++i12 && *i12 == CMail("peter", "alice", "PR bullshit"));
    assert(matchOutput(*i12, "From: peter, To: alice, Body: PR bullshit"));
    assert(++i12 && *i12 == CMail("thomas", "alice", "meeting details"));
    assert(matchOutput(*i12, "From: thomas, To: alice, Body: meeting details"));
    assert(++i12 && *i12 == CMail("joe", "alice", "delivery details"));
    assert(matchOutput(*i12, "From: joe, To: alice, Body: delivery details"));
    assert(++i12 && *i12 == CMail("steve", "alice", "newsletter"));
    assert(matchOutput(*i12, "From: steve, To: alice, Body: newsletter"));
    assert(!++i12);

    CMailIterator i13 = s2.inbox("alice");
    assert(i13 && *i13 == CMail("john", "alice", "deadline notice"));
    assert(matchOutput(*i13, "From: john, To: alice, Body: deadline notice"));
    assert(++i13 && *i13 == CMail("peter", "alice", "PR bullshit"));
    assert(matchOutput(*i13, "From: peter, To: alice, Body: PR bullshit"));
    assert(++i13 && *i13 == CMail("thomas", "alice", "meeting details"));
    assert(matchOutput(*i13, "From: thomas, To: alice, Body: meeting details"));
    assert(++i13 && *i13 == CMail("joe", "alice", "delivery details"));
    assert(matchOutput(*i13, "From: joe, To: alice, Body: delivery details"));
    assert(++i13 && *i13 == CMail("paul", "alice", "invalid invoice"));
    assert(matchOutput(*i13, "From: paul, To: alice, Body: invalid invoice"));
    assert(!++i13);

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */