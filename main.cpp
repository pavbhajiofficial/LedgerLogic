#include <iostream>
#include <stdexcept>
#include <cstring>

#include "transaction.h"
#include "file_handler.h"
#include "report.h"

using namespace std;

void addExpense() {
    Transaction t;

    cout << "Enter ID: ";
    cin >> t.id;

    cout << "Enter Date: ";
    cin >> t.date;

    cout << "Enter Category: ";
    cin >> t.category;

    cout << "Enter Amount: ";
    cin >> t.amount;

    if (t.amount < 0) {
        throw invalid_argument("Negative expense!");
    }

    addTransaction(t);
    appendTransactionToFile(&t);

    cout << "Added successfully.\n";
}

void viewAll() {
    Transaction* list = getAllTransactions();
    int count = getTransactionCount();

    for (int i = 0; i < count; i++) {
        cout << list[i].id << " "
             << list[i].date << " "
             << list[i].category << " "
             << list[i].amount << endl;
    }
}

void generateReport() {
    int choice;
    cout << "1. Monthly\n2. Category\nChoice: ";
    cin >> choice;

    Report* r = nullptr;

    if (choice == 1) r = new MonthlyReport();
    else r = new CategoryReport();

    r->generate();
    delete r;
}

int main() {
    loadFromFile();

    int choice;

    while (true) {
        cout << "\n1.Add 2.View 3.Report 4.Exit\n";
        cin >> choice;

        try {
            switch (choice) {
                case 1: addExpense(); break;
                case 2: viewAll(); break;
                case 3: generateReport(); break;
                case 4:
                    saveToFile();
                    return 0;
            }
        } catch (exception& e) {
            cout << e.what() << endl;
        }
    }
}
