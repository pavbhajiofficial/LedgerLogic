#include "transaction.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <algorithm>

// ─── Transaction ────────────────────────────────────────────────────────────

Transaction::Transaction(int id, const std::string& date,
                         const std::string& category, double amount)
    : id(id), date(date), category(category), amount(amount) {}

Transaction::Transaction(Transaction&& other) noexcept
    : id(other.id),
      date(std::move(other.date)),
      category(std::move(other.category)),
      amount(other.amount) {
    other.id = 0;
    other.amount = 0.0;
}

Transaction& Transaction::operator=(Transaction&& other) noexcept {
    if (this != &other) {
        id       = other.id;
        date     = std::move(other.date);
        category = std::move(other.category);
        amount   = other.amount;
        other.id = 0;
        other.amount = 0.0;
    }
    return *this;
}

void Transaction::display() const {
    std::cout << std::left
              << std::setw(6)  << id
              << std::setw(14) << date
              << std::setw(18) << category
              << std::fixed << std::setprecision(2) << amount
              << "\n";
}

std::string Transaction::getSummary() const {
    std::ostringstream oss;
    oss << "ID:" << id << " [" << date << "] "
        << category << " - Rs." << std::fixed << std::setprecision(2) << amount;
    return oss.str();
}

// ─── TransactionManager ─────────────────────────────────────────────────────

TransactionManager::TransactionManager() : nextId(1) {}

void TransactionManager::addTransaction(const std::string& date,
                                        const std::string& category,
                                        double amount) {
    if (category.empty())
        throw std::invalid_argument("Category cannot be empty.");
    if (amount <= 0.0)
        throw std::invalid_argument("Amount must be positive.");

    transactions.emplace_back(nextId++, date, category, amount);
    categories.insert(category);
}

bool TransactionManager::deleteTransaction(int id) {
    auto it = std::find_if(transactions.begin(), transactions.end(),
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
        std::cout << "  No transactions recorded yet.\n";
        return;
    }
    std::cout << std::left
              << std::setw(6)  << "ID"
              << std::setw(14) << "Date"
              << std::setw(18) << "Category"
              << "Amount (Rs.)\n";
    std::cout << std::string(52, '-') << "\n";
    for (const auto& t : transactions) t.display();
}

std::vector<Transaction>
TransactionManager::searchByCategory(const std::string& category) const {
    std::vector<Transaction> result;
    for (const auto& t : transactions) {
        if (t.category == category) result.push_back(t);
    }
    return result;
}

const std::vector<Transaction>& TransactionManager::getAll() const {
    return transactions;
}

const std::set<std::string>& TransactionManager::getCategories() const {
    return categories;
}

void TransactionManager::setTransactions(std::vector<Transaction>&& txns) {
    transactions = std::move(txns);
    categories.clear();
    nextId = 1;
    for (const auto& t : transactions) {
        categories.insert(t.category);
        if (t.id >= nextId) nextId = t.id + 1;
    }
}
