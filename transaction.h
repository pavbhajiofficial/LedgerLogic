#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <vector>
#include <set>

struct Transaction {
    int id;
    std::string date;
    std::string category;
    double amount;

    Transaction() : id(0), amount(0.0) {}
    Transaction(int id, const std::string& date, const std::string& category, double amount);
    Transaction(Transaction&& other) noexcept;
    Transaction& operator=(Transaction&& other) noexcept;
    Transaction(const Transaction&) = default;
    Transaction& operator=(const Transaction&) = default;
    virtual ~Transaction() = default;

    virtual void display() const;
    virtual std::string getSummary() const;
};

class TransactionManager {
private:
    std::vector<Transaction> transactions;
    std::set<std::string> categories;
    int nextId;

public:
    TransactionManager();
    ~TransactionManager() = default;

    void addTransaction(const std::string& date, const std::string& category, double amount);
    bool deleteTransaction(int id);
    void viewAll() const;
    std::vector<Transaction> searchByCategory(const std::string& category) const;
    const std::vector<Transaction>& getAll() const;
    const std::set<std::string>& getCategories() const;
    void setTransactions(std::vector<Transaction>&& txns);
    int getNextId() const { return nextId; }
    void setNextId(int id) { nextId = id; }
};

#endif
