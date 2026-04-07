#include <stdio.h>
#include "file_handler.h"

#define FILE_NAME "ledger.txt"

// From transaction.cpp
extern void addTransactionFromFile(int id, const char* date, const char* category, float amount);
extern Transaction* getAllTransactions();
extern int getTransactionCount();

// Load file
void loadFromFile() {
    FILE* file = fopen(FILE_NAME, "r");
    if (!file) return;

    int id;
    char date[20];
    char category[50];
    float amount;

    while (fscanf(file, "%d %s %s %f", &id, date, category, &amount) != EOF) {
        addTransactionFromFile(id, date, category, amount);
    }

    fclose(file);
}

// Save entire data
void saveToFile() {
    FILE* file = fopen(FILE_NAME, "w");
    if (!file) return;

    Transaction* list = getAllTransactions();
    int count = getTransactionCount();

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d %s %s %.2f\n",
                list[i].id,
                list[i].date,
                list[i].category,
                list[i].amount);
    }

    fclose(file);
}

// Append one transaction
void appendTransactionToFile(const Transaction* t) {
    FILE* file = fopen(FILE_NAME, "a");
    if (!file) return;

    fprintf(file, "%d %s %s %.2f\n",
            t->id, t->date, t->category, t->amount);

    fclose(file);
}
