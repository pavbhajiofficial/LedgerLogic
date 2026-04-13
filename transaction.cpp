#include "transaction.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <algorithm>

using namespace std;

// ─── Transaction ────────────────────────────────────────────────────────────

Transaction::Transaction(int id, const string& date,
                         const string& category, double amount)
    : id(id), date(date), category(category), amount(amount) {}

Transaction::Transaction(Transaction&& other) noexcept
    : id(other.id),
      date(move(other.date)),
      category(move(other.category)),
      amount(other.amount) {
    other.id = 0;
    other.amount = 0.0;
}

Transaction& Transaction::operator=(Transaction&& other) noexcept {
    if (this != &other) {
        id       = other.id;
        date     = move(other.date);
        category = move(other.category);
        amount   = other.amount;
        other.id = 0;
        other.amount = 0.0;
    }
    return *this;
}

void Transaction::display() const {
    cout << left
         << setw(6)  << id
         << setw(14) << date
         << setw(18) << category
         << fixed << setprecision(2) << amount
         << "\n";
}

string Transaction::getSummary() const {
    ostringstream oss;
    oss << "ID:" << id << " [" << date << "] "
        << category << " - Rs." << fixed << setprecision(2) << amount;
    return oss.str();
}

// ─── TransactionManager ─────────────────────────────────────────────────────

TransactionManager::TransactionManager() : nextId(1) {}

void TransactionManager::addTransaction(const string& date,
                                        const string& category,
                                        double amount) {
    if (category.empty())
        throw invalid_argument("Category cannot be empty.");
    if (amount <= 0.0)
        throw invalid_argument("Amount must be positive.");

    transactions.emplace_back(nextId++, date, category, amount);
    categories.insert(category);
}

bool TransactionManager::deleteTransaction(int id) {
    auto it = find_if(transactions.begin(), transactions.end(),
                      [id](const Transaction& t) { return t.id == id; });
    if (it == transactions.end()) return false;

    transactions.erase(it);

    // Rebuild category set
    categories.clear();
    for (const auto& t : transactions) categories.insert(t.category);
    return true;
}

void TransactionManager::viewAll() const {
    if (transactions.empty()) {
        cout << "  No transactions recorded yet.\n";
        return;
    }
    cout << left
         << setw(6)  << "ID"
         << setw(14) << "Date"
         << setw(18) << "Category"
         << "Amount (Rs.)\n";
    cout << string(52, '-') << "\n";
    for (const auto& t : transactions) t.display();
}

vector<Transaction>
TransactionManager::searchByCategory(const string& category) const {
    vector<Transaction> result;
    for (const auto& t : transactions) {
        if (t.category == category) result.push_back(t);
    }
    return result;
}

const vector<Transaction>& TransactionManager::getAll() const {
    return transactions;
}

const set<string>& TransactionManager::getCategories() const {
    return categories;
}

void TransactionManager::setTransactions(vector<Transaction>&& txns) {
    transactions = move(txns);
    categories.clear();
    nextId = 1;
    for (const auto& t : transactions) {
        categories.insert(t.category);
        if (t.id >= nextId) nextId = t.id + 1;
    }
}
