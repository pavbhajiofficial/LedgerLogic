#include <iostream>
#include <cstring>
#include "transaction.h"

using namespace std;

// DMA variables
static Transaction* transactions = nullptr;
static int capacity = 2;
static int size = 0;

// STL containers
static vector<Transaction> transactionList;
static set<string> categories;

// Expand dynamic array
void expandArray() {
    capacity *= 2;
    Transaction* newArr = new Transaction[capacity];

    for (int i = 0; i < size; i++) {
        newArr[i] = transactions[i];
    }

    delete[] transactions;
    transactions = newArr;
}

// Add transaction
void addTransaction(const Transaction& t) {
    if (size == capacity) {
        expandArray();
    }

    transactions[size++] = t;
    transactionList.push_back(t);
    categories.insert(t.category);
}

// Used when loading from file
void addTransactionFromFile(int id, const char* date, const char* category, float amount) {
    Transaction t;
    t.id = id;
    strcpy(t.date, date);
    strcpy(t.category, category);
    t.amount = amount;

    addTransaction(t);
}

// Delete transaction
void deleteTransaction(int id) {
    for (int i = 0; i < size; i++) {
        if (transactions[i].id == id) {
            for (int j = i; j < size - 1; j++) {
                transactions[j] = transactions[j + 1];
            }
            size--;
            break;
        }
    }
}

// Get all transactions
Transaction* getAllTransactions() {
    return transactions;
}

int getTransactionCount() {
    return size;
}

set<string> getCategories() {
    return categories;
}
