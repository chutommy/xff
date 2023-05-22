#ifndef __PROGTEST__

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
#include <iterator>
#include <stdexcept>

using namespace std;
#endif /* __PROGTEST__ */

template<typename R>
class CContest {
private:
    struct Match {
        string name1;
        string name2;
        R result;

        Match(string n1, string n2, R res) : name1(std::move(n1)), name2(std::move(n2)), result(std::move(res)) {}

        bool operator<(const Match &rhs) const {
            if (name1 != rhs.name1)
                return name1 < rhs.name1;
            return name2 < rhs.name2;
        }
    };

    set<Match> matches;

    inline map<string, size_t> emptyVisitMap() const {
        map<string, size_t> visited;
        for (const Match &m: matches) {
            if (m.name1 == m.name2)
                throw std::logic_error("unsortable");

            visited.insert(pair<string, bool>(m.name1, 0));
            visited.insert(pair<string, bool>(m.name2, 0));
        }

        return visited;
    }

    template<typename C>
    inline map<string, set<string>> adjacencyMap(C cmp) const {
        map<string, set<string>> adj;

        for (const Match &m: matches) {
            int res = cmp(m.result);

            if (res < 0) {
                auto pos = adj.find(m.name2);
                if (pos == adj.end())
                    adj.insert(pos, pair<string, set<string>>(m.name2, set<string>{m.name1}));
                else
                    pos->second.insert(m.name1);

            } else if (res > 0) {
                auto pos = adj.find(m.name1);
                if (pos == adj.end())
                    adj.insert(pos, pair<string, set<string>>(m.name1, set<string>{m.name2}));
                else
                    pos->second.insert(m.name2);

            } else
                throw std::logic_error("unsortable");
        }

        return adj;
    }

    void topSortRec(const string &name, map<string, size_t> &visited, list<string> &ordered,
                    const map<string, set<string>> &adj) const {
        auto vpos = visited.find(name);
        vpos->second = 2;

        auto pos = adj.find(name);
        if (pos != adj.end())
            for (const auto &child: pos->second) {
                auto cpos = visited.find(child);
                if (cpos->second == 2)
                    throw std::logic_error("unsortable");
                else if (!cpos->second)
                    topSortRec(child, visited, ordered, adj);
            }

        vpos->second = 1;
        ordered.push_front(name);
    }

public:
    CContest &addMatch(const string &n1, const string &n2, const R &res) {
        Match m(n1, n2, res);

        // check for the existence of a match with swapped names
        std::swap(m.name1, m.name2);
        auto pos = matches.find(m);
        std::swap(m.name1, m.name2);
        if (pos != matches.end())
            throw std::logic_error("duplicate match");

        // check for the existence of a match without swapped names
        pos = matches.find(m);
        if (pos != matches.end())
            throw std::logic_error("duplicate match");

        matches.insert(pos, m);
        return *this;
    }

    template<typename C>
    list<string> results(C cmp) const {
        auto visited = emptyVisitMap();
        auto adj = adjacencyMap(cmp);

        list<string> ordered;
        for (const auto &v: visited)
            if (!v.second)
                topSortRec(v.first, visited, ordered, adj);

        for (auto it = ordered.begin(); it != ordered.end(); it++) {
            auto nx = std::next(it, 1);
            if (nx != ordered.end())
                if (!adj.at(*it).count(*nx))
                    throw std::logic_error("unsortable");
        }

        return ordered;
    }

    template<typename C>
    bool isOrdered(C cmp) const {
        try { results(cmp); }
        catch (const logic_error &) {
            return false;
        }
        return true;
    }
};

#ifndef __PROGTEST__

struct CMatch {
    int m_A;
    int m_B;

    CMatch(int a, int b) : m_A(a), m_B(b) {}
};

class HigherScoreThreshold {
private:
    int m_DiffAtLeast;

public:
    explicit HigherScoreThreshold(int diffAtLeast) : m_DiffAtLeast(diffAtLeast) {}

    int operator()(const CMatch &x) const { return (x.m_A > x.m_B + m_DiffAtLeast) - (x.m_B > x.m_A + m_DiffAtLeast); }
};

int HigherScore(const CMatch &x) { return (x.m_A > x.m_B) - (x.m_B > x.m_A); }

int main() {
    CContest<CMatch> a1;
    a1.addMatch("A", "B", CMatch(10, 3))
            .addMatch("C", "D", CMatch(8, 1));
    assert(!a1.isOrdered(HigherScore));

    CContest<CMatch> a2;
    a2.addMatch("A", "B", CMatch(3, 3))
            .addMatch("C", "D", CMatch(8, 1));
    assert(!a2.isOrdered(HigherScore));

    CContest<CMatch> a3;
    a3.addMatch("A", "B", CMatch(3, 0))
            .addMatch("A", "D", CMatch(12, 9))
            .addMatch("C", "D", CMatch(11, 9));
    assert(!a3.isOrdered(HigherScore));

    CContest<CMatch> a4;
    a4.addMatch("A", "B", CMatch(3, 0))
            .addMatch("A", "C", CMatch(12, 9))
            .addMatch("C", "D", CMatch(11, 9));
    assert(!a4.isOrdered(HigherScore));

    CContest<CMatch> a5;
    a5.addMatch("A", "A", CMatch(3, 0))
            .addMatch("A", "C", CMatch(12, 9));
    assert(!a5.isOrdered(HigherScore));

    CContest<CMatch> a6;
    a6.addMatch("A", "B", CMatch(3, 0))
            .addMatch("B", "C", CMatch(12, 9))
            .addMatch("C", "A", CMatch(12, 9));
    assert(!a6.isOrdered(HigherScore));

    CContest<CMatch> x;

    x.addMatch("C++", "Pascal", CMatch(10, 3))
            .addMatch("C++", "Java", CMatch(8, 1))
            .addMatch("Pascal", "Basic", CMatch(40, 0))
            .addMatch("Java", "PHP", CMatch(6, 2))
            .addMatch("Java", "Pascal", CMatch(7, 3))
            .addMatch("PHP", "Basic", CMatch(10, 0));

    assert(!x.isOrdered(HigherScore));
    try {
        list<string> res = x.results(HigherScore);
        assert("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert("Invalid exception thrown!" == nullptr);
    }

    x.addMatch("PHP", "Pascal", CMatch(3, 6));

    assert(x.isOrdered(HigherScore));
    try {
        list<string> res = x.results(HigherScore);
        assert((res == list<string>{"C++", "Java", "Pascal", "PHP", "Basic"}));
    }
    catch (...) {
        assert("Unexpected exception!" == nullptr);
    }


    assert(!x.isOrdered(HigherScoreThreshold(3)));
    try {
        list<string> res = x.results(HigherScoreThreshold(3));
        assert("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert("Invalid exception thrown!" == nullptr);
    }

    assert(x.isOrdered([](const CMatch &x) {
        return (x.m_A < x.m_B) - (x.m_B < x.m_A);
    }));
    try {
        list<string> res = x.results([](const CMatch &x) {
            return (x.m_A < x.m_B) - (x.m_B < x.m_A);
        });
        assert((res == list<string>{"Basic", "PHP", "Pascal", "Java", "C++"}));
    }
    catch (...) {
        assert("Unexpected exception!" == nullptr);
    }

    CContest<bool> y;

    y.addMatch("Python", "PHP", true)
            .addMatch("PHP", "Perl", true)
            .addMatch("Perl", "Bash", true)
            .addMatch("Bash", "JavaScript", true)
            .addMatch("JavaScript", "VBScript", true);

    assert(y.isOrdered([](bool v) {
        return v ? 10 : -10;
    }));
    try {
        list<string> res = y.results([](bool v) {
            return v ? 10 : -10;
        });
        assert((res == list<string>{"Python", "PHP", "Perl", "Bash", "JavaScript", "VBScript"}));
    }
    catch (...) {
        assert("Unexpected exception!" == nullptr);
    }

    y.addMatch("PHP", "JavaScript", false);
    assert(!y.isOrdered([](bool v) {
        return v ? 10 : -10;
    }));
    try {
        list<string> res = y.results([](bool v) {
            return v ? 10 : -10;
        });
        assert("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert("Invalid exception thrown!" == nullptr);
    }

    try {
        y.addMatch("PHP", "JavaScript", false);
        assert("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert("Invalid exception thrown!" == nullptr);
    }

    try {
        y.addMatch("JavaScript", "PHP", true);
        assert("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert("Invalid exception thrown!" == nullptr);
    }
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */