#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <typeinfo>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <utility>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <algorithm>
#include <type_traits>

using namespace std;

class CRect {
public:
    double m_X;
    double m_Y;
    double m_W;
    double m_H;

    CRect(double x, double y, double w, double h)
            : m_X(x), m_Y(y), m_W(w), m_H(h) {}

    friend ostream &operator<<(ostream &os, const CRect &x) {
        return os << '(' << x.m_X << ',' << x.m_Y << ',' << x.m_W << ',' << x.m_H << ')';
    }
};

#endif /* __PROGTEST__ */

void printIndent(std::ostream &os, const vector<bool> &prev) {
    for (auto last: prev)
        if (!last)
            os << "|  ";
        else
            os << "   ";
}

class CElement {
public:
    int m_ID;
    CRect m_RelPos;
    CRect m_AbsPos;

    CElement(int id, const CRect &pos) : m_ID(id), m_RelPos(pos), m_AbsPos({0, 0, 0, 0}) {}

    virtual ~CElement() = default;

    virtual CElement *clone() const = 0;

    virtual string elementType() const = 0;

    virtual void updateAbsPos(const CRect &w) {
        m_AbsPos.m_X = m_RelPos.m_X * w.m_W + w.m_X;
        m_AbsPos.m_Y = m_RelPos.m_Y * w.m_H + w.m_Y;
        m_AbsPos.m_W = m_RelPos.m_W * w.m_W;
        m_AbsPos.m_H = m_RelPos.m_H * w.m_H;
    };

    virtual CElement *search(int id) const {
        return nullptr;
    }

    virtual void print(std::ostream &os, const vector<bool> prev) const {
        os << "[" << m_ID << "] " << elementType() << " " << m_AbsPos << endl;
    }
};

std::ostream &operator<<(std::ostream &os, const CElement &el) {
    el.print(os, {});
    return os;
}

class CStringElement : public CElement {
protected:
    string m_Value;

public:
    CStringElement(int id, const CRect &pos, string value)
            : CElement(id, pos), m_Value(std::move(value)) {}

    void print(std::ostream &os, const vector<bool> prev) const override {
        os << "[" << m_ID << "] " << elementType() << " \"" << m_Value << "\" " << m_AbsPos << endl;
    }
};

class CWindow : public CStringElement {
private:
    vector<CElement *> m_Elements;

public:
    CWindow(int id, const string &title, const CRect &absPos) : CStringElement(id, {0, 0, 0, 0}, title) {
        m_AbsPos = absPos;
    }

    CWindow(const CWindow &w) : CStringElement(w.m_ID, {0, 0, 0, 0}, w.m_Value) {
        m_AbsPos = w.m_AbsPos;
        for (const auto el: w.m_Elements)
            m_Elements.push_back(el->clone());
    }

    CWindow &operator=(CWindow w) {
        std::swap(m_ID, w.m_ID);
        std::swap(m_RelPos, w.m_RelPos);
        std::swap(m_AbsPos, w.m_AbsPos);
        std::swap(m_Value, w.m_Value);
        std::swap(m_Elements, w.m_Elements);
        return *this;
    }

    ~CWindow() override {
        for (const auto el: m_Elements)
            delete el;
    }

    CElement *clone() const override { return new CWindow(*this); }

    string elementType() const override { return "Window"; }

    void print(std::ostream &os, const vector<bool> prev) const override {
        CStringElement::print(os, {});
        for (auto it = m_Elements.begin(); it != m_Elements.end() - 1; it++) {
            os << "+- ";
            (*it)->print(os, {false});
        }
        os << "+- ";
        (*(m_Elements.end() - 1))->print(os, {true});
    }

    CWindow &add(const CElement &el) {
        CElement *newEl = el.clone();
        newEl->updateAbsPos(m_AbsPos);
        m_Elements.push_back(newEl);
        return *this;
    }

    CElement *search(int id) const override {
        for (const auto &el: m_Elements) {
            if (el->m_ID == id)
                return el;
            CElement *res = el->search(id);
            if (res != nullptr)
                return res;
        }
        return nullptr;
    }

    void setPosition(const CRect &absPos) {
        m_AbsPos = absPos;
        for (auto el: m_Elements)
            el->updateAbsPos(m_AbsPos);
    }
};

class CPanel : public CElement {
private:
    vector<CElement *> m_Elements;

public:
    CPanel(int id, const CRect &relPos) : CElement(id, relPos) {}

    CPanel(const CPanel &p) : CElement(p.m_ID, p.m_RelPos) {
        m_AbsPos = p.m_AbsPos;
        for (const auto el: p.m_Elements)
            m_Elements.push_back(el->clone());
    }

    CPanel &operator=(CPanel p) {
        std::swap(m_ID, p.m_ID);
        std::swap(m_RelPos, p.m_RelPos);
        std::swap(m_AbsPos, p.m_AbsPos);
        std::swap(m_Elements, p.m_Elements);
        return *this;
    }

    ~CPanel() override {
        for (const auto el: m_Elements)
            delete el;
    }

    CPanel *clone() const override { return new CPanel(*this); }

    string elementType() const override { return "Panel"; }

    void updateAbsPos(const CRect &w) override {
        CElement::updateAbsPos(w);
        for (auto el: m_Elements)
            el->updateAbsPos(m_AbsPos);
    };

    void print(std::ostream &os, vector<bool> prev) const override {
        CElement::print(os, {});
        prev.push_back(false);
        for (auto it = m_Elements.begin(); it != m_Elements.end() - 1; it++) {
            prev.pop_back();
            printIndent(os, prev);
            prev.push_back(false);
            os << "+- ";
            (*it)->print(os, prev);
        }
        prev.pop_back();
        printIndent(os, prev);
        prev.push_back(true);
        os << "+- ";
        (*(m_Elements.end() - 1))->print(os, prev);
    }

    CElement *search(int id) const override {
        for (const auto &el: m_Elements) {
            if (el->m_ID == id)
                return el;
            CElement *res = el->search(id);
            if (res != nullptr)
                return res;
        }
        return nullptr;
    }

    CPanel &add(const CElement &el) {
        CElement *newEl = el.clone();
        newEl->updateAbsPos(m_AbsPos);
        m_Elements.push_back(newEl);
        return *this;
    }
};

class CButton : public CStringElement {
public:
    CButton(int id, const CRect &relPos, const string &name)
            : CStringElement(id, relPos, name) {}

    CButton *clone() const override { return new CButton(*this); }

    string elementType() const override { return "Button"; }
};

class CInput : public CStringElement {
public:
    CInput(int id, const CRect &relPos, const string &value)
            : CStringElement(id, relPos, value) {}

    CInput *clone() const override { return new CInput(*this); }

    string elementType() const override { return "Input"; }

    void setValue(const string &val) { m_Value = val; }

    const string &getValue() const { return m_Value; }
};

class CLabel : public CStringElement {
public:
    CLabel(int id, const CRect &relPos, const string &label)
            : CStringElement(id, relPos, label) {}

    CLabel *clone() const override { return new CLabel(*this); }

    string elementType() const override { return "Label"; }
};

class CComboBox : public CElement {
private:
    vector<string> m_Options;
    size_t m_Select;

public:
    CComboBox(int id, const CRect &relPos) : CElement(id, relPos), m_Select(0) {}

    CComboBox *clone() const override { return new CComboBox(*this); }

    string elementType() const override { return "ComboBox"; }

    void print(std::ostream &os, vector<bool> prev) const override {
        CElement::print(os, {});
        for (size_t i = 0; i < m_Options.size(); i++) {
            printIndent(os, prev);
            if (i == m_Select)
                os << "+->" << m_Options[i] << "<" << endl;
            else
                os << "+- " << m_Options[i] << endl;
        }
    }

    CComboBox &add(const string &opt) {
        m_Options.push_back(opt);
        return *this;
    }

    void setSelected(size_t sel) { m_Select = sel; }

    size_t getSelected() const { return m_Select; }
};

#ifndef __PROGTEST__

template<typename T>
string toString(const T &x) {
    ostringstream oss;
    oss << x;
    return oss.str();
}

int main() {
    assert(sizeof(CButton) - sizeof(string) < sizeof(CComboBox) - sizeof(vector<string>));
    assert(sizeof(CInput) - sizeof(string) < sizeof(CComboBox) - sizeof(vector<string>));
    assert(sizeof(CLabel) - sizeof(string) < sizeof(CComboBox) - sizeof(vector<string>));
    CWindow a(0, "Sample window", CRect(10, 10, 600, 480));
    a.add(CButton(1, CRect(0.1, 0.8, 0.3, 0.1), "Ok")).add(CButton(2, CRect(0.6, 0.8, 0.3, 0.1), "Cancel"));
    a.add(CLabel(10, CRect(0.1, 0.1, 0.2, 0.1), "Username:"));
    a.add(CInput(11, CRect(0.4, 0.1, 0.5, 0.1), "chucknorris"));
    a.add(CComboBox(20, CRect(0.1, 0.3, 0.8, 0.1)).add("Karate").add("Judo").add("Box").add("Progtest"));
    assert(toString(a) ==
           "[0] Window \"Sample window\" (10,10,600,480)\n"
           "+- [1] Button \"Ok\" (70,394,180,48)\n"
           "+- [2] Button \"Cancel\" (370,394,180,48)\n"
           "+- [10] Label \"Username:\" (70,58,120,48)\n"
           "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
           "+- [20] ComboBox (70,154,480,48)\n"
           "   +->Karate<\n"
           "   +- Judo\n"
           "   +- Box\n"
           "   +- Progtest\n");
    CWindow b = a;
    assert(toString(*b.search(20)) ==
           "[20] ComboBox (70,154,480,48)\n"
           "+->Karate<\n"
           "+- Judo\n"
           "+- Box\n"
           "+- Progtest\n");
    assert(dynamic_cast<CComboBox &> ( *b.search(20)).getSelected() == 0);
    dynamic_cast<CComboBox &> ( *b.search(20)).setSelected(3);
    assert(dynamic_cast<CInput &> ( *b.search(11)).getValue() == "chucknorris");
    dynamic_cast<CInput &> ( *b.search(11)).setValue("chucknorris@fit.cvut.cz");
    b.add(CComboBox(21, CRect(0.1, 0.5, 0.8, 0.1)).add("PA2").add("OSY").add("Both"));
    assert(toString(b) ==
           "[0] Window \"Sample window\" (10,10,600,480)\n"
           "+- [1] Button \"Ok\" (70,394,180,48)\n"
           "+- [2] Button \"Cancel\" (370,394,180,48)\n"
           "+- [10] Label \"Username:\" (70,58,120,48)\n"
           "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
           "+- [20] ComboBox (70,154,480,48)\n"
           "|  +- Karate\n"
           "|  +- Judo\n"
           "|  +- Box\n"
           "|  +->Progtest<\n"
           "+- [21] ComboBox (70,250,480,48)\n"
           "   +->PA2<\n"
           "   +- OSY\n"
           "   +- Both\n");
    assert(toString(a) ==
           "[0] Window \"Sample window\" (10,10,600,480)\n"
           "+- [1] Button \"Ok\" (70,394,180,48)\n"
           "+- [2] Button \"Cancel\" (370,394,180,48)\n"
           "+- [10] Label \"Username:\" (70,58,120,48)\n"
           "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
           "+- [20] ComboBox (70,154,480,48)\n"
           "   +->Karate<\n"
           "   +- Judo\n"
           "   +- Box\n"
           "   +- Progtest\n");
    b.setPosition(CRect(20, 30, 640, 520));
    assert(toString(b) ==
           "[0] Window \"Sample window\" (20,30,640,520)\n"
           "+- [1] Button \"Ok\" (84,446,192,52)\n"
           "+- [2] Button \"Cancel\" (404,446,192,52)\n"
           "+- [10] Label \"Username:\" (84,82,128,52)\n"
           "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
           "+- [20] ComboBox (84,186,512,52)\n"
           "|  +- Karate\n"
           "|  +- Judo\n"
           "|  +- Box\n"
           "|  +->Progtest<\n"
           "+- [21] ComboBox (84,290,512,52)\n"
           "   +->PA2<\n"
           "   +- OSY\n"
           "   +- Both\n");
    assert(sizeof(CButton) - sizeof(string) < sizeof(CComboBox) - sizeof(vector<string>));
    assert(sizeof(CInput) - sizeof(string) < sizeof(CComboBox) - sizeof(vector<string>));
    assert(sizeof(CLabel) - sizeof(string) < sizeof(CComboBox) - sizeof(vector<string>));
    assert(sizeof(CButton) - sizeof(string) <= sizeof(CPanel) - sizeof(vector<void *>));
    assert(sizeof(CInput) - sizeof(string) <= sizeof(CPanel) - sizeof(vector<void *>));
    assert(sizeof(CLabel) - sizeof(string) <= sizeof(CPanel) - sizeof(vector<void *>));
    a = CWindow(0, "Sample window", CRect(10, 10, 600, 480));
    a.add(CButton(1, CRect(0.1, 0.8, 0.3, 0.1), "Ok")).add(CButton(2, CRect(0.6, 0.8, 0.3, 0.1), "Cancel"));
    a.add(CLabel(10, CRect(0.1, 0.1, 0.2, 0.1), "Username:"));
    a.add(CInput(11, CRect(0.4, 0.1, 0.5, 0.1), "chucknorris"));
    a.add(CPanel(12, CRect(0.1, 0.3, 0.8, 0.7)).add(
            CComboBox(20, CRect(0.1, 0.3, 0.8, 0.1)).add("Karate").add("Judo").add("Box").add("Progtest")));
    assert(toString(a) ==
           "[0] Window \"Sample window\" (10,10,600,480)\n"
           "+- [1] Button \"Ok\" (70,394,180,48)\n"
           "+- [2] Button \"Cancel\" (370,394,180,48)\n"
           "+- [10] Label \"Username:\" (70,58,120,48)\n"
           "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
           "+- [12] Panel (70,154,480,336)\n"
           "   +- [20] ComboBox (118,254.8,384,33.6)\n"
           "      +->Karate<\n"
           "      +- Judo\n"
           "      +- Box\n"
           "      +- Progtest\n");
    b = a;
    assert(toString(*b.search(20)) ==
           "[20] ComboBox (118,254.8,384,33.6)\n"
           "+->Karate<\n"
           "+- Judo\n"
           "+- Box\n"
           "+- Progtest\n");
    assert(dynamic_cast<CComboBox &> ( *b.search(20)).getSelected() == 0);
    dynamic_cast<CComboBox &> ( *b.search(20)).setSelected(3);
    assert(dynamic_cast<CInput &> ( *b.search(11)).getValue() == "chucknorris");
    dynamic_cast<CInput &> ( *b.search(11)).setValue("chucknorris@fit.cvut.cz");
    auto &p = dynamic_cast<CPanel &> ( *b.search(12));
    p.add(CComboBox(21, CRect(0.1, 0.5, 0.8, 0.1)).add("PA2").add("OSY").add("Both"));
    assert(toString(b) ==
           "[0] Window \"Sample window\" (10,10,600,480)\n"
           "+- [1] Button \"Ok\" (70,394,180,48)\n"
           "+- [2] Button \"Cancel\" (370,394,180,48)\n"
           "+- [10] Label \"Username:\" (70,58,120,48)\n"
           "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
           "+- [12] Panel (70,154,480,336)\n"
           "   +- [20] ComboBox (118,254.8,384,33.6)\n"
           "   |  +- Karate\n"
           "   |  +- Judo\n"
           "   |  +- Box\n"
           "   |  +->Progtest<\n"
           "   +- [21] ComboBox (118,322,384,33.6)\n"
           "      +->PA2<\n"
           "      +- OSY\n"
           "      +- Both\n");
    assert(toString(a) ==
           "[0] Window \"Sample window\" (10,10,600,480)\n"
           "+- [1] Button \"Ok\" (70,394,180,48)\n"
           "+- [2] Button \"Cancel\" (370,394,180,48)\n"
           "+- [10] Label \"Username:\" (70,58,120,48)\n"
           "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
           "+- [12] Panel (70,154,480,336)\n"
           "   +- [20] ComboBox (118,254.8,384,33.6)\n"
           "      +->Karate<\n"
           "      +- Judo\n"
           "      +- Box\n"
           "      +- Progtest\n");
    assert(toString(p) ==
           "[12] Panel (70,154,480,336)\n"
           "+- [20] ComboBox (118,254.8,384,33.6)\n"
           "|  +- Karate\n"
           "|  +- Judo\n"
           "|  +- Box\n"
           "|  +->Progtest<\n"
           "+- [21] ComboBox (118,322,384,33.6)\n"
           "   +->PA2<\n"
           "   +- OSY\n"
           "   +- Both\n");
    b.setPosition(CRect(20, 30, 640, 520));
    assert(toString(b) ==
           "[0] Window \"Sample window\" (20,30,640,520)\n"
           "+- [1] Button \"Ok\" (84,446,192,52)\n"
           "+- [2] Button \"Cancel\" (404,446,192,52)\n"
           "+- [10] Label \"Username:\" (84,82,128,52)\n"
           "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
           "+- [12] Panel (84,186,512,364)\n"
           "   +- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
           "   |  +- Karate\n"
           "   |  +- Judo\n"
           "   |  +- Box\n"
           "   |  +->Progtest<\n"
           "   +- [21] ComboBox (135.2,368,409.6,36.4)\n"
           "      +->PA2<\n"
           "      +- OSY\n"
           "      +- Both\n");
    p.add(p);
    assert(toString(p) ==
           "[12] Panel (84,186,512,364)\n"
           "+- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
           "|  +- Karate\n"
           "|  +- Judo\n"
           "|  +- Box\n"
           "|  +->Progtest<\n"
           "+- [21] ComboBox (135.2,368,409.6,36.4)\n"
           "|  +->PA2<\n"
           "|  +- OSY\n"
           "|  +- Both\n"
           "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
           "   +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
           "   |  +- Karate\n"
           "   |  +- Judo\n"
           "   |  +- Box\n"
           "   |  +->Progtest<\n"
           "   +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
           "      +->PA2<\n"
           "      +- OSY\n"
           "      +- Both\n");
    p.add(p);
    assert(toString(p) ==
           "[12] Panel (84,186,512,364)\n"
           "+- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
           "|  +- Karate\n"
           "|  +- Judo\n"
           "|  +- Box\n"
           "|  +->Progtest<\n"
           "+- [21] ComboBox (135.2,368,409.6,36.4)\n"
           "|  +->PA2<\n"
           "|  +- OSY\n"
           "|  +- Both\n"
           "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
           "|  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
           "|  |  +- Karate\n"
           "|  |  +- Judo\n"
           "|  |  +- Box\n"
           "|  |  +->Progtest<\n"
           "|  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
           "|     +->PA2<\n"
           "|     +- OSY\n"
           "|     +- Both\n"
           "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
           "   +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
           "   |  +- Karate\n"
           "   |  +- Judo\n"
           "   |  +- Box\n"
           "   |  +->Progtest<\n"
           "   +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
           "   |  +->PA2<\n"
           "   |  +- OSY\n"
           "   |  +- Both\n"
           "   +- [12] Panel (176.16,371.64,327.68,178.36)\n"
           "      +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
           "      |  +- Karate\n"
           "      |  +- Judo\n"
           "      |  +- Box\n"
           "      |  +->Progtest<\n"
           "      +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
           "         +->PA2<\n"
           "         +- OSY\n"
           "         +- Both\n");
    p.add(p);
    assert(toString(p) ==
           "[12] Panel (84,186,512,364)\n"
           "+- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
           "|  +- Karate\n"
           "|  +- Judo\n"
           "|  +- Box\n"
           "|  +->Progtest<\n"
           "+- [21] ComboBox (135.2,368,409.6,36.4)\n"
           "|  +->PA2<\n"
           "|  +- OSY\n"
           "|  +- Both\n"
           "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
           "|  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
           "|  |  +- Karate\n"
           "|  |  +- Judo\n"
           "|  |  +- Box\n"
           "|  |  +->Progtest<\n"
           "|  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
           "|     +->PA2<\n"
           "|     +- OSY\n"
           "|     +- Both\n"
           "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
           "|  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
           "|  |  +- Karate\n"
           "|  |  +- Judo\n"
           "|  |  +- Box\n"
           "|  |  +->Progtest<\n"
           "|  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
           "|  |  +->PA2<\n"
           "|  |  +- OSY\n"
           "|  |  +- Both\n"
           "|  +- [12] Panel (176.16,371.64,327.68,178.36)\n"
           "|     +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
           "|     |  +- Karate\n"
           "|     |  +- Judo\n"
           "|     |  +- Box\n"
           "|     |  +->Progtest<\n"
           "|     +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
           "|        +->PA2<\n"
           "|        +- OSY\n"
           "|        +- Both\n"
           "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
           "   +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
           "   |  +- Karate\n"
           "   |  +- Judo\n"
           "   |  +- Box\n"
           "   |  +->Progtest<\n"
           "   +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
           "   |  +->PA2<\n"
           "   |  +- OSY\n"
           "   |  +- Both\n"
           "   +- [12] Panel (176.16,371.64,327.68,178.36)\n"
           "   |  +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
           "   |  |  +- Karate\n"
           "   |  |  +- Judo\n"
           "   |  |  +- Box\n"
           "   |  |  +->Progtest<\n"
           "   |  +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
           "   |     +->PA2<\n"
           "   |     +- OSY\n"
           "   |     +- Both\n"
           "   +- [12] Panel (176.16,371.64,327.68,178.36)\n"
           "      +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
           "      |  +- Karate\n"
           "      |  +- Judo\n"
           "      |  +- Box\n"
           "      |  +->Progtest<\n"
           "      +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
           "      |  +->PA2<\n"
           "      |  +- OSY\n"
           "      |  +- Both\n"
           "      +- [12] Panel (208.928,425.148,262.144,124.852)\n"
           "         +- [20] ComboBox (235.142,462.604,209.715,12.4852)\n"
           "         |  +- Karate\n"
           "         |  +- Judo\n"
           "         |  +- Box\n"
           "         |  +->Progtest<\n"
           "         +- [21] ComboBox (235.142,487.574,209.715,12.4852)\n"
           "            +->PA2<\n"
           "            +- OSY\n"
           "            +- Both\n");
    assert(toString(b) ==
           "[0] Window \"Sample window\" (20,30,640,520)\n"
           "+- [1] Button \"Ok\" (84,446,192,52)\n"
           "+- [2] Button \"Cancel\" (404,446,192,52)\n"
           "+- [10] Label \"Username:\" (84,82,128,52)\n"
           "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
           "+- [12] Panel (84,186,512,364)\n"
           "   +- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
           "   |  +- Karate\n"
           "   |  +- Judo\n"
           "   |  +- Box\n"
           "   |  +->Progtest<\n"
           "   +- [21] ComboBox (135.2,368,409.6,36.4)\n"
           "   |  +->PA2<\n"
           "   |  +- OSY\n"
           "   |  +- Both\n"
           "   +- [12] Panel (135.2,295.2,409.6,254.8)\n"
           "   |  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
           "   |  |  +- Karate\n"
           "   |  |  +- Judo\n"
           "   |  |  +- Box\n"
           "   |  |  +->Progtest<\n"
           "   |  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
           "   |     +->PA2<\n"
           "   |     +- OSY\n"
           "   |     +- Both\n"
           "   +- [12] Panel (135.2,295.2,409.6,254.8)\n"
           "   |  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
           "   |  |  +- Karate\n"
           "   |  |  +- Judo\n"
           "   |  |  +- Box\n"
           "   |  |  +->Progtest<\n"
           "   |  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
           "   |  |  +->PA2<\n"
           "   |  |  +- OSY\n"
           "   |  |  +- Both\n"
           "   |  +- [12] Panel (176.16,371.64,327.68,178.36)\n"
           "   |     +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
           "   |     |  +- Karate\n"
           "   |     |  +- Judo\n"
           "   |     |  +- Box\n"
           "   |     |  +->Progtest<\n"
           "   |     +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
           "   |        +->PA2<\n"
           "   |        +- OSY\n"
           "   |        +- Both\n"
           "   +- [12] Panel (135.2,295.2,409.6,254.8)\n"
           "      +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
           "      |  +- Karate\n"
           "      |  +- Judo\n"
           "      |  +- Box\n"
           "      |  +->Progtest<\n"
           "      +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
           "      |  +->PA2<\n"
           "      |  +- OSY\n"
           "      |  +- Both\n"
           "      +- [12] Panel (176.16,371.64,327.68,178.36)\n"
           "      |  +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
           "      |  |  +- Karate\n"
           "      |  |  +- Judo\n"
           "      |  |  +- Box\n"
           "      |  |  +->Progtest<\n"
           "      |  +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
           "      |     +->PA2<\n"
           "      |     +- OSY\n"
           "      |     +- Both\n"
           "      +- [12] Panel (176.16,371.64,327.68,178.36)\n"
           "         +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
           "         |  +- Karate\n"
           "         |  +- Judo\n"
           "         |  +- Box\n"
           "         |  +->Progtest<\n"
           "         +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
           "         |  +->PA2<\n"
           "         |  +- OSY\n"
           "         |  +- Both\n"
           "         +- [12] Panel (208.928,425.148,262.144,124.852)\n"
           "            +- [20] ComboBox (235.142,462.604,209.715,12.4852)\n"
           "            |  +- Karate\n"
           "            |  +- Judo\n"
           "            |  +- Box\n"
           "            |  +->Progtest<\n"
           "            +- [21] ComboBox (235.142,487.574,209.715,12.4852)\n"
           "               +->PA2<\n"
           "               +- OSY\n"
           "               +- Both\n");
    assert(toString(a) ==
           "[0] Window \"Sample window\" (10,10,600,480)\n"
           "+- [1] Button \"Ok\" (70,394,180,48)\n"
           "+- [2] Button \"Cancel\" (370,394,180,48)\n"
           "+- [10] Label \"Username:\" (70,58,120,48)\n"
           "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
           "+- [12] Panel (70,154,480,336)\n"
           "   +- [20] ComboBox (118,254.8,384,33.6)\n"
           "      +->Karate<\n"
           "      +- Judo\n"
           "      +- Box\n"
           "      +- Progtest\n");
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */