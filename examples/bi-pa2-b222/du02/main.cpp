#ifndef __PROGTEST__

#include <cstdlib>
#include <cassert>
#include <string>
#include <utility>
#include <vector>
#include <list>
#include <functional>
#include <memory>

using namespace std;
#endif /* __PROGTEST__ */

// Handles salary related actions.
class CSalaryStats {
    vector<unsigned int> salaries;

    static bool comp(unsigned int left, unsigned int right) {
        return left < right;
    }

public:

    // Adds a new salary to the tracking.
    void addSalary(unsigned int salary) {
        auto it = lower_bound(salaries.begin(), salaries.end(), salary, comp);
        salaries.insert(it, salary);
    }

    // Removes a salary from the tracking.
    void removeSalary(unsigned int salary) {
        auto it = lower_bound(salaries.begin(), salaries.end(), salary, comp);
        salaries.erase(it);
    }

    // Replaces a salary in the tracking.
    void replaceSalary(unsigned int oldSalary, unsigned int newSalary) {
        removeSalary(oldSalary);
        addSalary(newSalary);
    }

    // Ranks the given salary among other salaries.
    void getRank(unsigned int salary, int &rankMin, int &rankMax) const {
        auto lowerIt = lower_bound(salaries.begin(), salaries.end(), salary, comp);
        auto upperIt = upper_bound(salaries.begin(), salaries.end(), salary, comp);

        rankMin = int(lowerIt - salaries.begin());
        rankMax = int(upperIt - salaries.begin() - 1);
    }
};

struct CEmployee {
    string name, surname, email;
    unsigned int salary;

    CEmployee(string name, string surname, string email, unsigned int salary)
            : name(std::move(name)), surname(std::move(surname)), email(std::move(email)), salary(salary) {}

    bool nameIs(const string &name2, const string &surname2) const {
        return name == name2 && surname == surname2;
    }

    bool emailIs(const string &email2) const {
        return email == email2;
    }

    static bool compName(const CEmployee &e1, const CEmployee &e2) {
        if (e1.surname != e2.surname)
            return e1.surname < e2.surname;
        return e1.name < e2.name;
    }

    static bool compEmail(const CEmployee &e1, const CEmployee &e2) {
        return e1.email < e2.email;
    }
};

// Represents the HR database of employees.
class CPersonalAgenda {
    vector<CEmployee *> nameIndex;
    vector<CEmployee *> emailIndex;
    CSalaryStats salaryStats;

    // Returns an iterator pointing to the insert index of the given employee name (name index).
    auto locateName(const string &name, const string &surname) const {
        CEmployee e(name, surname, "", 0);
        return lower_bound(nameIndex.begin(), nameIndex.end(), &e,
                           [](const CEmployee *pe1, const CEmployee *pe2) {
                               return CEmployee::compName(*pe1, *pe2);
                           });
    }

    // Returns an iterator pointing to the insert index of the given employee email (email index).
    auto locateEmail(const string &email) const {
        CEmployee e("", "", email, 0);
        return lower_bound(emailIndex.begin(), emailIndex.end(), &e,
                           [](const CEmployee *pe1, const CEmployee *pe2) {
                               return CEmployee::compEmail(*pe1, *pe2);
                           });
    }

    // Finds whether the iterator represents an employee with the given name.
    bool isSame(vector<CEmployee *>::const_iterator &it, const string &name, const string &surname) const {
        return it != nameIndex.end() && (*it)->nameIs(name, surname);
    }

    // Finds whether the iterator represents an employee with the given email.
    bool isSame(vector<CEmployee *>::const_iterator &it, const string &email) const {
        return it != emailIndex.end() && (*it)->emailIs(email);
    }

public:

    CPersonalAgenda() = default;

    ~CPersonalAgenda() {
        for (CEmployee *pEmp: nameIndex)
            delete pEmp;
    }

    // Adds a new employee. Returns false if the given values fail to unique constraint.
    bool add(const string &name, const string &surname, const string &email, unsigned int salary) {
        auto itName = locateName(name, surname);
        if (isSame(itName, name, surname))
            return false;

        auto itEmail = locateEmail(email);
        if (isSame(itEmail, email))
            return false;

        auto *newEmployee = new CEmployee(name, surname, email, salary);
        nameIndex.insert(itName, newEmployee);
        emailIndex.insert(itEmail, newEmployee);
        salaryStats.addSalary(salary);

        return true;
    }

    // Removes the given employee from DB. Returns false if fail to locate an employee with the given name.
    bool del(const string &name, const string &surname) {
        auto itName = locateName(name, surname);
        if (!isSame(itName, name, surname))
            return false;

        // remove from email index;
        auto itEmail = locateEmail((*itName)->email);
        emailIndex.erase(itEmail);

        salaryStats.removeSalary((*itName)->salary);
        delete *itName;   // free CEmployee before erasing address
        nameIndex.erase(itName);

        return true;
    }

    // Removes the given employee from DB. Returns false if fail to locate an employee with the given email.
    bool del(const string &email) {
        auto itEmail = locateEmail(email);
        if (!isSame(itEmail, email))
            return false;

        // remove from name index;
        auto itName = locateName((*itEmail)->name, (*itEmail)->surname);
        nameIndex.erase(itName);

        salaryStats.removeSalary((*itEmail)->salary);
        delete *itEmail;   // free CEmployee before erasing address
        emailIndex.erase(itEmail);

        return true;
    }

    // Updates name of the employee with the given email address. Returns false if employee is not found
    // or if the new name and surname isn't unique.
    bool changeName(const string &email, const string &newName, const string &newSurname) {
        auto itEmail = locateEmail(email);
        if (!isSame(itEmail, email))
            return false;

        // ensure unique name and surname
        auto itName = locateName(newName, newSurname);
        if (isSame(itName, newName, newSurname))
            return false;

        // remove the old instance in name index before updating to preserve correct arrangement
        itName = locateName((*itEmail)->name, (*itEmail)->surname);
        nameIndex.erase(itName);

        (*itEmail)->name = newName;
        (*itEmail)->surname = newSurname;

        // re-insert the extracted employee (from the name index) back to the index with a correct position index
        itName = locateName(newName, newSurname);
        nameIndex.insert(itName, *itEmail);

        return true;
    }

    // Updates email of the employee with the given name and surname. Returns false if employee is not found
    // or if the new email address isn't unique.
    bool changeEmail(const string &name, const string &surname, const string &newEmail) {
        auto itName = locateName(name, surname);
        if (!isSame(itName, name, surname))
            return false;

        // ensure unique email
        auto itEmail = locateEmail(newEmail);
        if (isSame(itEmail, newEmail))
            return false;

        // remove the old instance in email index before updating to preserve correct arrangement
        itEmail = locateEmail((*itName)->email);
        emailIndex.erase(itEmail);

        (*itName)->email = newEmail;

        // re-insert the extracted employee (from the email index) back to the index with a correct position index
        itEmail = locateEmail(newEmail);
        emailIndex.insert(itEmail, *itName);

        return true;
    }

    // Updates salary of the employee. Returns false if fails to locate the given employee's name and username.
    bool setSalary(const string &name, const string &surname, unsigned int salary) {
        auto it = locateName(name, surname);
        if (!isSame(it, name, surname))
            return false;

        salaryStats.replaceSalary((*it)->salary, salary);
        (*it)->salary = salary;

        return true;
    }

    // Updates salary of the employee. Returns false if fails to locate the given employee's email address.
    bool setSalary(const string &email, unsigned int salary) {
        auto it = locateEmail(email);
        if (!isSame(it, email))
            return false;

        salaryStats.replaceSalary((*it)->salary, salary);
        (*it)->salary = salary;

        return true;
    }

    // Returns a salary of the employee with the given name.
    unsigned int getSalary(const string &name, const string &surname) const {
        auto it = locateName(name, surname);
        if (!isSame(it, name, surname))
            return 0;

        return (*it)->salary;
    }

    // Returns a salary of the employee with the given email.
    unsigned int getSalary(const string &email) const {
        auto it = locateEmail(email);
        if (!isSame(it, email))
            return 0;

        return (*it)->salary;
    }

    // Returns ranking of the salary of the employee with the given name.
    bool getRank(const string &name, const string &surname, int &rankMin, int &rankMax) const {
        auto it = locateName(name, surname);
        if (!isSame(it, name, surname))
            return false;

        salaryStats.getRank((*it)->salary, rankMin, rankMax);

        return true;
    }

    // Returns ranking of the salary of the employee with the given email address.
    bool getRank(const string &email, int &rankMin, int &rankMax) const {
        auto it = locateEmail(email);
        if (!isSame(it, email))
            return false;

        salaryStats.getRank((*it)->salary, rankMin, rankMax);

        return true;
    }

    // Returns the first employee in the DB (sorted ascending by last name, first name).
    bool getFirst(string &outName, string &outSurname) const {
        if (nameIndex.empty())
            return false;

        outName = nameIndex[0]->name;
        outSurname = nameIndex[0]->surname;

        return true;
    }

    // Returns the next employee in the DB (sorted ascending by last name, first name).
    bool getNext(const string &name, const string &surname, string &outName, string &outSurname) const {
        auto it = locateName(name, surname);
        if (!isSame(it, name, surname))
            return false;

        // check whether there is no more employees in the DB
        if (it == nameIndex.end() || it == nameIndex.end() - 1)
            return false;

        auto nextIt = it + 1;
        outName = (*nextIt)->name;
        outSurname = (*nextIt)->surname;

        return true;
    }
};

#ifndef __PROGTEST__

int main() {
    string outName, outSurname;
    int lo, hi;

    CPersonalAgenda b1;

    assert (b1.add("John", "Smith", "john", 30000));
    assert (b1.add("John", "Miller", "john2", 35000));
    assert (b1.add("Peter", "Smith", "peter", 23000));

    assert (b1.getFirst(outName, outSurname)
            && outName == "John"
            && outSurname == "Miller");
    assert (b1.getNext("John", "Miller", outName, outSurname)
            && outName == "John"
            && outSurname == "Smith");
    assert (b1.getNext("John", "Smith", outName, outSurname)
            && outName == "Peter"
            && outSurname == "Smith");
    assert (!b1.getNext("Peter", "Smith", outName, outSurname));

    assert (b1.setSalary("john", 32000));
    assert (b1.getSalary("john") == 32000);
    assert (b1.getSalary("John", "Smith") == 32000);

    assert (b1.getRank("John", "Smith", lo, hi)
            && lo == 1
            && hi == 1);
    assert (b1.getRank("john", lo, hi)
            && lo == 1
            && hi == 1);
    assert (b1.getRank("peter", lo, hi)
            && lo == 0
            && hi == 0);
    assert (b1.getRank("john2", lo, hi)
            && lo == 2
            && hi == 2);

    assert (b1.setSalary("John", "Smith", 35000));
    assert (b1.getSalary("John", "Smith") == 35000);
    assert (b1.getSalary("john") == 35000);

    assert (b1.getRank("John", "Smith", lo, hi)
            && lo == 1
            && hi == 2);
    assert (b1.getRank("john", lo, hi)
            && lo == 1
            && hi == 2);
    assert (b1.getRank("peter", lo, hi)
            && lo == 0
            && hi == 0);
    assert (b1.getRank("john2", lo, hi)
            && lo == 1
            && hi == 2);

    assert (b1.changeName("peter", "James", "Bond"));
    assert (b1.getSalary("peter") == 23000);
    assert (b1.getSalary("James", "Bond") == 23000);
    assert (b1.getSalary("Peter", "Smith") == 0);
    assert (b1.getFirst(outName, outSurname)
            && outName == "James"
            && outSurname == "Bond");
    assert (b1.getNext("James", "Bond", outName, outSurname)
            && outName == "John"
            && outSurname == "Miller");
    assert (b1.getNext("John", "Miller", outName, outSurname)
            && outName == "John"
            && outSurname == "Smith");
    assert (!b1.getNext("John", "Smith", outName, outSurname));

    assert (b1.changeEmail("James", "Bond", "james"));
    assert (b1.getSalary("James", "Bond") == 23000);
    assert (b1.getSalary("james") == 23000);
    assert (b1.getSalary("peter") == 0);

    assert (b1.del("james"));
    assert (b1.getRank("john", lo, hi)
            && lo == 0
            && hi == 1);
    assert (b1.del("John", "Miller"));
    assert (b1.getRank("john", lo, hi)
            && lo == 0
            && hi == 0);
    assert (b1.getFirst(outName, outSurname)
            && outName == "John"
            && outSurname == "Smith");

    assert (!b1.getNext("John", "Smith", outName, outSurname));
    assert (b1.del("john"));

    assert (!b1.getFirst(outName, outSurname));
    assert (b1.add("John", "Smith", "john", 31000));
    assert (b1.add("john", "Smith", "joHn", 31000));
    assert (b1.add("John", "smith", "jOhn", 31000));

    CPersonalAgenda b2;

    assert (!b2.getFirst(outName, outSurname));

    assert (b2.add("James", "Bond", "james", 70000));
    assert (b2.add("James", "Smith", "james2", 30000));
    assert (b2.add("Peter", "Smith", "peter", 40000));
    assert (!b2.add("James", "Bond", "james3", 60000));
    assert (!b2.add("Peter", "Bond", "peter", 50000));

    assert (!b2.changeName("joe", "Joe", "Black"));
    assert (!b2.changeEmail("Joe", "Black", "joe"));
    assert (!b2.setSalary("Joe", "Black", 90000));
    assert (!b2.setSalary("joe", 90000));
    assert (b2.getSalary("Joe", "Black") == 0);
    assert (b2.getSalary("joe") == 0);

    assert (!b2.getRank("Joe", "Black", lo, hi));
    assert (!b2.getRank("joe", lo, hi));

    assert (!b2.changeName("joe", "Joe", "Black"));
    assert (!b2.changeEmail("Joe", "Black", "joe"));

    assert (!b2.del("Joe", "Black"));
    assert (!b2.del("joe"));
    assert (!b2.changeName("james2", "James", "Bond"));
    assert (!b2.changeEmail("Peter", "Smith", "james"));
    assert (!b2.changeName("peter", "Peter", "Smith"));
    assert (!b2.changeEmail("Peter", "Smith", "peter"));
    assert (b2.del("Peter", "Smith"));
    assert (!b2.changeEmail("Peter", "Smith", "peter2"));
    assert (!b2.setSalary("Peter", "Smith", 35000));
    assert (b2.getSalary("Peter", "Smith") == 0);
    assert (!b2.getRank("Peter", "Smith", lo, hi));
    assert (!b2.changeName("peter", "Peter", "Falcon"));
    assert (!b2.setSalary("peter", 37000));
    assert (b2.getSalary("peter") == 0);
    assert (!b2.getRank("peter", lo, hi));
    assert (!b2.del("Peter", "Smith"));
    assert (!b2.del("peter"));
    assert (b2.add("Peter", "Smith", "peter", 40000));
    assert (b2.getSalary("peter") == 40000);

//    cout << "sorted nameIndex" << endl;
//    for (CEmployee *emp: b2.nameIndex) {
//        cout << emp->surname << " " << emp->name << " "
//             << emp->email << " " << emp->salary << endl;
//    }
//    cout << endl;
//    cout << "email index" << endl;
//    for (CEmployee *emp: b2.emailIndex) {
//        cout << emp->email << " " << emp->surname << " "
//             << emp->name << " " << emp->salary << endl;
//    }

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
