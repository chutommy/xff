#ifndef __PROGTEST__

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

class CDate {
public:
    //---------------------------------------------------------------------------------------------
    CDate(int y, int m, int d) : m_Year(y), m_Month(m), m_Day(d) {}

    //---------------------------------------------------------------------------------------------
    int compare(const CDate &x) const {
        if (m_Year != x.m_Year)
            return m_Year - x.m_Year;
        if (m_Month != x.m_Month)
            return m_Month - x.m_Month;
        return m_Day - x.m_Day;
    }

    //---------------------------------------------------------------------------------------------
    int year() const { return m_Year; }

    //---------------------------------------------------------------------------------------------
    int month() const { return m_Month; }

    //---------------------------------------------------------------------------------------------
    int day() const { return m_Day; }

    //---------------------------------------------------------------------------------------------
    friend ostream &operator<<(ostream &os, const CDate &x) {
        char oldFill = os.fill();
        return os << setfill('0') << setw(4) << x.m_Year << "-" << setw(2)
                  << static_cast<int>(x.m_Month) << "-" << setw(2)
                  << static_cast<int>(x.m_Day) << setfill(oldFill);
    }
    //---------------------------------------------------------------------------------------------
private:
    int16_t m_Year;
    int8_t m_Month;
    int8_t m_Day;
};

#endif /* __PROGTEST__ */

bool notSpace(unsigned char ch) { return ch != 32; }

bool isBothSpace(char const &lhs, char const &rhs) { return lhs == 32 && lhs == rhs; }

// Removes leading and trailing spaces.
void trim(string &s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(), notSpace));
    s.erase(find_if(s.rbegin(), s.rend(), notSpace).base(), s.end());
}

// Replaces multiple spaces with a single space.
void removeConsecutiveSpaces(string &s) {
    auto it = unique(s.begin(), s.end(), isBothSpace);
    s.erase(it, s.end());
}

string toLower(const string &str) {
    string s = str;
    for (auto &ch: s)
        ch = tolower(ch);
    return s;
}

// Transforms a company's name into ID.
string getID(string name) {
    trim(name);
    removeConsecutiveSpaces(name);
    for (auto &ch: name)
        ch = tolower(ch);
    return name;
}

string getIDLowered(string name) {
    trim(name);
    removeConsecutiveSpaces(name);
    return name;
}

// CInvoice represents a time-stamped document that records a transaction between a buyer and a seller
class CInvoice {
private:
    CDate m_Date;
    string m_Seller;
    string m_Buyer;
    unsigned m_Amount;
    double m_VAT;

    string m_SellerLower;
    string m_BuyerLower;

    string m_SellerID;
    string m_BuyerID;

    size_t m_OrderID = 0;

public:
    CInvoice(const CDate &date, const string &seller, const string &buyer, unsigned int amount, double vat)
            : m_Date(date), m_Seller(seller), m_Buyer(buyer), m_Amount(amount), m_VAT(vat),
              m_SellerLower(toLower(seller)), m_BuyerLower(toLower(buyer)),
              m_SellerID(getIDLowered(m_SellerLower)), m_BuyerID(getIDLowered(m_BuyerLower)) {}

    CDate date() const { return m_Date; }

    const string &buyer() const { return m_Buyer; }

    const string &seller() const { return m_Seller; }

    unsigned int amount() const { return m_Amount; }

    double vat() const { return m_VAT; }

    const string &buyerID() const { return m_BuyerID; }

    const string &sellerID() const { return m_SellerID; }

    const string &buyerLower() const { return m_BuyerLower; }

    const string &sellerLower() const { return m_SellerLower; }

    size_t orderID() const { return m_OrderID; }

    void setOrderID(size_t i) { m_OrderID = i; }

    void setBuyer(const string &name) {
        m_Buyer = name;
        m_BuyerLower = toLower(name);
    }

    void setSeller(const string &name) {
        m_Seller = name;
        m_SellerLower = toLower(name);
    }

    bool operator<(const CInvoice &rhs) const {
        if (m_SellerID != rhs.m_SellerID)
            return m_SellerID < rhs.m_SellerID;
        if (m_BuyerID != rhs.m_BuyerID)
            return m_BuyerID < rhs.m_BuyerID;
        if (m_Amount != rhs.m_Amount)
            return m_Amount < rhs.m_Amount;
        if (m_VAT != rhs.m_VAT)
            return m_VAT < rhs.m_VAT;
        int dateOrder = m_Date.compare(rhs.m_Date);
        return dateOrder < 0;
    };

    bool operator==(const CInvoice &rhs) const {
        if (m_SellerID != rhs.m_SellerID
            || m_BuyerID != rhs.m_BuyerID
            || m_Amount != rhs.m_Amount
            || m_VAT != rhs.m_VAT)
            return false;
        return true;
    };

    bool operator!=(const CInvoice &rhs) const {
        return !(*this == rhs);
    };

    struct HashFunction {
        size_t operator()(const CInvoice &x) const {
            size_t hashYear = hash<int>()(x.m_Date.year());
            size_t hashMonth = hash<int>()(x.m_Date.month());
            size_t hashDay = hash<int>()(x.m_Date.day());
            size_t hashDate = hashYear ^ hashMonth ^ hashDay;

            size_t hashSeller = hash<string>()(x.m_Seller);
            size_t hashBuyer = hash<string>()(x.m_Buyer);
            size_t hashAmount = hash<unsigned>()(x.m_Amount);
            size_t hashVAT = hash<double>()(x.m_VAT);

            return hashSeller ^ hashBuyer ^ hashAmount ^ hashVAT ^ hashDate;
        }
    };
};

// Represents a single sorting criteria.
struct COpt {
    int m_SortBy;
    bool m_Asc;
};

// Represents a sorting configuration.
class CSortOpt {
private:
    deque<COpt> m_Opts;

public:
    static const int BY_DATE = 0;
    static const int BY_BUYER = 1;
    static const int BY_SELLER = 2;
    static const int BY_AMOUNT = 3;
    static const int BY_VAT = 4;

    CSortOpt() = default;

    CSortOpt &addKey(int key, bool ascending = true) {
        for (auto opt: m_Opts)
            if (opt.m_SortBy == key)
                return *this;
        m_Opts.push_back(COpt{key, ascending});
        return *this;
    }

    auto opts() const { return m_Opts; }
};

int sortByOrderCmp(const CInvoice &lhs, const CInvoice &rhs) { return (lhs.orderID() < rhs.orderID()) - (lhs.orderID() > rhs.orderID()); }

int sortByDateCmpASC(const CInvoice &lhs, const CInvoice &rhs) { return (lhs.date().compare(rhs.date()) < 0) - (lhs.date().compare(rhs.date()) > 0); }

int sortByDateCmpDES(const CInvoice &lhs, const CInvoice &rhs) { return (lhs.date().compare(rhs.date()) > 0) - (lhs.date().compare(rhs.date()) < 0); }

int sortByBuyerCmpASC(const CInvoice &lhs, const CInvoice &rhs) { return (lhs.buyerLower() < rhs.buyerLower()) - (lhs.buyerLower() > rhs.buyerLower()); }

int sortByBuyerCmpDES(const CInvoice &lhs, const CInvoice &rhs) { return (lhs.buyerLower() > rhs.buyerLower()) - (lhs.buyerLower() < rhs.buyerLower()); }

int sortBySellerCmpASC(const CInvoice &lhs, const CInvoice &rhs) { return (lhs.sellerLower() < rhs.sellerLower()) - (lhs.sellerLower() > rhs.sellerLower()); }

int sortBySellerCmpDES(const CInvoice &lhs, const CInvoice &rhs) { return (lhs.sellerLower() > rhs.sellerLower()) - (lhs.sellerLower() < rhs.sellerLower()); }

int sortByAmountCmpASC(const CInvoice &lhs, const CInvoice &rhs) { return (lhs.amount() < rhs.amount()) - (lhs.amount() > rhs.amount()); }

int sortByAmountCmpDES(const CInvoice &lhs, const CInvoice &rhs) { return (lhs.amount() > rhs.amount()) - (lhs.amount() < rhs.amount()); }

int sortByVATCmpASC(const CInvoice &lhs, const CInvoice &rhs) { return (lhs.vat() < rhs.vat()) - (lhs.vat() > rhs.vat()); }

int sortByVATCmpDES(const CInvoice &lhs, const CInvoice &rhs) { return (lhs.vat() > rhs.vat()) - (lhs.vat() < rhs.vat()); }

struct CMultiSort {
    deque<int (*)(const CInvoice &, const CInvoice &)> m_Cmps;

    explicit CMultiSort(const CSortOpt &sortBy) {
        for (auto it: sortBy.opts()) {
            switch (it.m_SortBy) {
                case CSortOpt::BY_DATE:
                    if (it.m_Asc) m_Cmps.push_back(sortByDateCmpASC);
                    else m_Cmps.push_back(sortByDateCmpDES);
                    break;
                case CSortOpt::BY_BUYER:
                    if (it.m_Asc) m_Cmps.push_back(sortByBuyerCmpASC);
                    else m_Cmps.push_back(sortByBuyerCmpDES);
                    break;
                case CSortOpt::BY_SELLER:
                    if (it.m_Asc) m_Cmps.push_back(sortBySellerCmpASC);
                    else m_Cmps.push_back(sortBySellerCmpDES);
                    break;
                case CSortOpt::BY_AMOUNT:
                    if (it.m_Asc) m_Cmps.push_back(sortByAmountCmpASC);
                    else m_Cmps.push_back(sortByAmountCmpDES);
                    break;
                case CSortOpt::BY_VAT:
                    if (it.m_Asc) m_Cmps.push_back(sortByVATCmpASC);
                    else m_Cmps.push_back(sortByVATCmpDES);
            }
        }
        m_Cmps.push_back(sortByOrderCmp);
    }

    bool operator()(const CInvoice &lhs, const CInvoice &rhs) const {
        for (const auto &cmp: m_Cmps) {
            int res = cmp(lhs, rhs);
            if (res > 0) return true;
            else if (res < 0) return false;
        }

        return false;
    }
};

// CVATRegister is an invoice manager for handling correct invoice pairing.
class CVATRegister {
private:
    map<string, string> m_RegisteredCompanies;
    set<CInvoice> m_IssuedInvoices;
    set<CInvoice> m_AcceptedInvoices;

    size_t m_InvoiceLastID = 0;

    deque<CInvoice> m_UnmachedCache;
    bool m_Updated = false;

    // Validates and ensures correct seller/buyer names against the DB of registered companies.
    bool validateInvoice(CInvoice &invoice) const {
        if (invoice.sellerID() == invoice.buyerID())
            return false;

        auto sellerIt = m_RegisteredCompanies.find(invoice.sellerID());
        auto buyerIt = m_RegisteredCompanies.find(invoice.buyerID());
        if (sellerIt == m_RegisteredCompanies.end()
            || buyerIt == m_RegisteredCompanies.end())
            return false;

        invoice.setSeller(sellerIt->second);
        invoice.setBuyer(buyerIt->second);

        return true;
    }

public:

    CVATRegister() = default;

    bool registerCompany(const string &name) {
        string id = getID(name);
        if (!m_RegisteredCompanies.insert(pair<string, string>(id, name)).second)
            return false;
        return true;
    }

    bool addIssued(const CInvoice &x) {
        CInvoice invoice = x;
        invoice.setOrderID(m_InvoiceLastID++);
        if (!validateInvoice(invoice)
            || !m_IssuedInvoices.insert(invoice).second)
            return false;
        m_Updated = true;
        return true;
    }

    bool addAccepted(const CInvoice &x) {
        CInvoice invoice = x;
        invoice.setOrderID(m_InvoiceLastID++);
        if (!validateInvoice(invoice)
            || !m_AcceptedInvoices.insert(invoice).second)
            return false;
        m_Updated = true;
        return true;
    }

    bool delIssued(const CInvoice &x) {
        if (!m_IssuedInvoices.erase(x))
            return false;
        m_Updated = true;
        return true;
    }

    bool delAccepted(const CInvoice &x) {
        if (!m_AcceptedInvoices.erase(x))
            return false;
        m_Updated = true;
        return true;
    }

    // Represents a simple filter for a certain company.
    struct isCompanyFilter {
        string companyID;

        explicit isCompanyFilter(string id) : companyID(std::move(id)) {}

        bool operator()(const CInvoice &x) const {
            return x.buyerID() == companyID || x.sellerID() == companyID;
        }
    };

    list<CInvoice> unmatched(const string &company, const CSortOpt &sortBy) {
        // select the invoices with no matching pair (issued/accepted)
        if (m_Updated) {
            m_UnmachedCache.clear();
            set_symmetric_difference(m_IssuedInvoices.begin(), m_IssuedInvoices.end(),
                                     m_AcceptedInvoices.begin(), m_AcceptedInvoices.end(),
                                     back_inserter(m_UnmachedCache));
            m_Updated = false;
        }

        // filter out the queried company, insert in sorted order
        auto cmp = CMultiSort(sortBy);
        set<CInvoice, decltype(cmp)> filtered(cmp);
        isCompanyFilter isCompany(getID(company));
        copy_if(m_UnmachedCache.begin(), m_UnmachedCache.end(), inserter(filtered, filtered.begin()), isCompany);

        return {filtered.begin(), filtered.end()};
    }
};

#ifndef __PROGTEST__

bool equalLists(const list<CInvoice> &a, const list<CInvoice> &b) {
    auto aIt = a.begin();
    auto bIt = b.begin();

    while (aIt != a.end() && bIt != b.end()) {
        if (*aIt != *bIt)
            return false;
        aIt++;
        bIt++;
    }

    if (aIt != a.end() || bIt != b.end())
        return false;
    return true;
}

int main() {
    CVATRegister r;
    assert(r.registerCompany("first Company"));
    assert(r.registerCompany("Second     Company"));
    assert(r.registerCompany("ThirdCompany, Ltd."));
    assert(r.registerCompany("Third Company, Ltd."));
    assert(!r.registerCompany("Third Company, Ltd."));
    assert(!r.registerCompany(" Third  Company,  Ltd.  "));
    assert(r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company ", 100, 20)));
    assert(r.addIssued(CInvoice(CDate(2000, 1, 2), "FirSt Company", "Second Company ", 200, 30)));
    assert(r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company ", 100, 30)));
    assert(r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company ", 300, 30)));
    assert(r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", " Third  Company,  Ltd.   ", 200, 30)));
    assert(r.addIssued(CInvoice(CDate(2000, 1, 1), "Second Company ", "First Company", 300, 30)));
    assert(r.addIssued(CInvoice(CDate(2000, 1, 1), "Third  Company,  Ltd.", "  Second    COMPANY ", 400, 34)));
    assert(!r.addIssued(CInvoice(CDate(2000, 1, 1), "Fourth  Company,  Ltd.", "  Second    COMPANY ", 600, 34)));
    assert(!r.addIssued(CInvoice(CDate(2000, 1, 1), "Third  Company,  Ltd.", "  Fourth    COMPANY ", 600, 34)));
    assert(!r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company ", 300, 30)));
    assert(!r.addIssued(CInvoice(CDate(2000, 1, 4), "First Company", "First   Company", 200, 30)));
    assert(!r.addIssued(CInvoice(CDate(2000, 1, 4), "Another Company", "First   Company", 200, 30)));
    assert(equalLists(r.unmatched("First Company", CSortOpt()
                              .addKey(CSortOpt::BY_SELLER, true)
                              .addKey(CSortOpt::BY_BUYER, false)
                              .addKey(CSortOpt::BY_DATE, false)),
                      list<CInvoice>{
                              CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200, 30.000000),
                              CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 20.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 30.000000)}));
    assert(equalLists(r.unmatched("First Company", CSortOpt()
                              .addKey(CSortOpt::BY_DATE, true)
                              .addKey(CSortOpt::BY_SELLER, true)
                              .addKey(CSortOpt::BY_BUYER, true)),
                      list<CInvoice>{
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 20.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 30.000000),
                              CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200, 30.000000)}));
    assert(equalLists(r.unmatched("First Company", CSortOpt()
                              .addKey(CSortOpt::BY_VAT, true)
                              .addKey(CSortOpt::BY_AMOUNT, true)
                              .addKey(CSortOpt::BY_DATE, true)
                              .addKey(CSortOpt::BY_SELLER, true)
                              .addKey(CSortOpt::BY_BUYER, true)),
                      list<CInvoice>{
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 20.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200, 30.000000),
                              CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 30.000000)}));
    assert(equalLists(r.unmatched("First Company", CSortOpt()),
                      list<CInvoice>{
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 20.000000),
                              CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 30.000000)}));
    assert(equalLists(r.unmatched("second company",
                                  CSortOpt().addKey(CSortOpt::BY_DATE, false)),
                      list<CInvoice>{
                              CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 20.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "Third Company, Ltd.", "Second     Company", 400, 34.000000)}));
    assert(equalLists(r.unmatched("last company", CSortOpt().addKey(CSortOpt::BY_VAT, true)), list<CInvoice>{}));
    assert(r.addAccepted(CInvoice(CDate(2000, 1, 2), "First Company", "Second Company ", 200, 30)));
    assert(r.addAccepted(CInvoice(CDate(2000, 1, 1), "First Company", " Third  Company,  Ltd.   ", 200, 30)));
    assert(r.addAccepted(CInvoice(CDate(2000, 1, 1), "Second company ", "First Company", 300, 32)));
    assert(equalLists(r.unmatched("First Company", CSortOpt()
                              .addKey(CSortOpt::BY_SELLER, true)
                              .addKey(CSortOpt::BY_BUYER, true)
                              .addKey(CSortOpt::BY_DATE, true)),
                      list<CInvoice>{
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 20.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 32.000000)}));
    assert(!r.delIssued(CInvoice(CDate(2001, 1, 1), "First Company", "Second Company ", 200, 30)));
    assert(!r.delIssued(CInvoice(CDate(2000, 1, 1), "A First Company", "Second Company ", 200, 30)));
    assert(!r.delIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Hand", 200, 30)));
    assert(!r.delIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company", 1200, 30)));
    assert(!r.delIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company", 200, 130)));
    assert(r.delIssued(CInvoice(CDate(2000, 1, 2), "First Company", "Second Company", 200, 30)));
    assert(equalLists(r.unmatched("First Company", CSortOpt()
                              .addKey(CSortOpt::BY_SELLER, true)
                              .addKey(CSortOpt::BY_BUYER, true)
                              .addKey(CSortOpt::BY_DATE, true)),
                      list<CInvoice>{
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 20.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300, 30.000000),
                              CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 32.000000)}));
    assert(r.delAccepted(CInvoice(CDate(2000, 1, 1), "First Company", " Third  Company,  Ltd.   ", 200, 30)));
    assert(equalLists(r.unmatched("First Company", CSortOpt()
                              .addKey(CSortOpt::BY_SELLER, true)
                              .addKey(CSortOpt::BY_BUYER, true)
                              .addKey(CSortOpt::BY_DATE, true)),
                      list<CInvoice>{
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 20.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300, 30.000000),
                              CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 30.000000),
                              CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 32.000000)}));
    assert(r.delIssued(CInvoice(CDate(2000, 1, 1), "First Company", " Third  Company,  Ltd.   ", 200, 30)));
    for (size_t i = 0; i < 1 << 16; i++)
        assert(equalLists(r.unmatched("First Company", CSortOpt()
                                  .addKey(CSortOpt::BY_SELLER, true)
                                  .addKey(CSortOpt::BY_SELLER, true)
                                  .addKey(CSortOpt::BY_BUYER, true)
                                  .addKey(CSortOpt::BY_BUYER, true)
                                  .addKey(CSortOpt::BY_DATE, true)
                                  .addKey(CSortOpt::BY_SELLER, true)
                                  .addKey(CSortOpt::BY_SELLER, true)
                                  .addKey(CSortOpt::BY_BUYER, true)
                                  .addKey(CSortOpt::BY_BUYER, true)
                                  .addKey(CSortOpt::BY_BUYER, true)
                                  .addKey(CSortOpt::BY_DATE, true)),
                          list<CInvoice>{
                                  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 20.000000),
                                  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100, 30.000000),
                                  CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300, 30.000000),
                                  CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200, 30.000000),
                                  CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 30.000000),
                                  CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300, 32.000000)}));
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */