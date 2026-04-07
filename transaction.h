#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <vector>
#include <set>

class Transaction {
public:
    int id;
    char date[20];
    char category[50];
    float amount;
};

// Core APIs
void addTransaction(const Transaction& t);
void addTransactionFromFile(int id, const char* date, const char* category, float amount);
void deleteTransaction(int id);

Transaction* getAllTransactions();
int getTransactionCount();

// Category handling
std::set<std::string> getCategories();

#endif
