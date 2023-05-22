#ifndef __PROGTEST__

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <functional>
#include <memory>
#include <stdexcept>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cctype>
#include <cstdint>
#include <cassert>

using namespace std;
#endif /* __PROGTEST__ */

class CBase {
public:
    virtual ~CBase() = default;

    virtual size_t Size() const = 0;

    virtual CBase *clone() const = 0;

    virtual ostream &print(ostream &out) const = 0;
};

class CFile : public CBase {
private:
    string m_Hash;
    size_t m_Size;

public:
    CFile(string hash, size_t filesize) : m_Hash(std::move(hash)), m_Size(filesize) {}

    size_t Size() const override { return m_Size; }

    CFile *clone() const override { return new CFile(*this); }

    CFile &Change(const string &hash, size_t filesize) {
        m_Hash = hash;
        m_Size = filesize;
        return *this;
    }

    ostream &print(ostream &out) const override {
        return out << " " << m_Hash;
    }
};

class CLink : public CBase {
private:
    string m_Path;

public:
    explicit CLink(string path) : m_Path(std::move(path)) {}

    size_t Size() const override { return m_Path.size() + 1; }

    CLink *clone() const override { return new CLink(*this); }

    CLink &Change(const string &path) {
        m_Path = path;
        return *this;
    }

    ostream &print(ostream &out) const override {
        return out << " -> " << m_Path;
    }
};

class CDirectory : public CBase {
private:
    map<string, CBase *> m_Content;

public:
    CDirectory() = default;

    CDirectory(const CDirectory &dir) {
        for (const auto &file: dir.m_Content)
            m_Content.insert(pair<string, CBase *>(file.first, file.second->clone()));
    }

    CDirectory &operator=(CDirectory dir) {
        swap(m_Content, dir.m_Content);
        return *this;
    }

    ~CDirectory() override {
        for (const auto &file: m_Content)
            delete file.second;
    }

    size_t Size() const override {
        size_t size = 0;
        for (const auto &file: m_Content)
            size += file.first.size() + file.second->Size();
        return size;
    }

    CDirectory *clone() const override { return new CDirectory(*this); }

    CDirectory &Change(const string &filename, const CBase &file) {
        auto it = m_Content.find(filename);
        if (it != m_Content.end()) {
            delete it->second;
            m_Content.erase(it);
        }
        m_Content.insert(pair<string, CBase *>(filename, file.clone()));
        return *this;
    }

    CDirectory &Change(const string &filename, const CBase *) {
        auto it = m_Content.find(filename);
        if (it != m_Content.end()) {
            delete it->second;
            m_Content.erase(it);
        }
        return *this;
    }

    CBase &Get(const string &filename) {
        auto it = m_Content.find(filename);
        if (it == m_Content.end())
            throw out_of_range("file not found");
        return *(it->second);
    }

    CBase &Get(const string &filename) const {
        auto it = m_Content.find(filename);
        if (it == m_Content.end())
            throw out_of_range("file not found");
        return *(it->second);
    }

    ostream &print(ostream &out) const override {
        return out << "/";
    }

    friend ostream &operator<<(ostream &out, const CDirectory &dir) {
        for (const auto &file: dir.m_Content) {
            out << file.second->Size() << "\t" << file.first;
            file.second->print(out) << endl;
        }
        return out;
    }
};

#ifndef __PROGTEST__

int main() {
    CDirectory root;
    stringstream sout;

    root.Change("file.txt", CFile("jhwadkhawkdhajwdhawhdaw=", 1623))
            .Change("file.ln", CLink("").Change("file.txt"))
            .Change("folder", CDirectory()
                    .Change("fileA.txt", CFile("", 0).Change("skjdajdakljdljadkjwaljdlaw=", 1713))
                    .Change("fileB.txt", CFile("kadwjkwajdwhoiwhduqwdqwuhd=", 71313))
                    .Change("fileC.txt", CFile("aihdqhdqudqdiuwqhdquwdqhdi=", 8193))
            );

    sout.str("");
    sout << root;
    assert(sout.str() ==
           "9\tfile.ln -> file.txt\n"
           "1623\tfile.txt jhwadkhawkdhajwdhawhdaw=\n"
           "81246\tfolder/\n");
    assert(root.Size() == 82899);

    string filename = "folder";
    const auto &inner = dynamic_cast<const CDirectory &>( root.Get(filename));

    sout.str("");
    sout << inner;
    assert(sout.str() ==
           "1713\tfileA.txt skjdajdakljdljadkjwaljdlaw=\n"
           "71313\tfileB.txt kadwjkwajdwhoiwhduqwdqwuhd=\n"
           "8193\tfileC.txt aihdqhdqudqdiuwqhdquwdqhdi=\n");
    assert(inner.Size() == 81246);

    return 0;
}

#endif /* __PROGTEST__ */