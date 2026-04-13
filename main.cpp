#include "transaction.h"
#include "report.h"
#include "utils.h"
#include "file_handler.h"

#include <iostream>
#include <vector>
#include <cstring>
#include <iomanip>
#include <stdexcept>
using namespace std;

static void loadFromFile(TransactionManager& mgr) {
    RawTransaction* raw = nullptr;
    int count = load_transactions(&raw);
    if (count <= 0) { free_raw_transactions(raw); return; }

   vector<Transaction> txns;
    txns.reserve(count);
    for (int i = 0; i < count; i++) {
        txns.emplace_back(raw[i].id, raw[i].date, raw[i].category, raw[i].amount);
    }
    free_raw_transactions(raw);
    mgr.setTransactions(move(txns));
}

static void saveToFile(const TransactionManager& mgr) {
    const auto& txns = mgr.getAll();
    int count = static_cast<int>(txns.size());

    // Use DMA to build the C array
    RawTransaction* raw = new RawTransaction[count == 0 ? 1 : count];
    for (int i = 0; i < count; i++) {
        raw[i].id     = txns[i].id;
        raw[i].amount = txns[i].amount;
        strncpy(raw[i].date,     txns[i].date.c_str(),     MAX_DATE_LEN - 1);
        strncpy(raw[i].category, txns[i].category.c_str(), MAX_CATEGORY_LEN - 1);
        raw[i].date[MAX_DATE_LEN - 1]         = '\0';
        raw[i].category[MAX_CATEGORY_LEN - 1] = '\0';
    }

    if (save_transactions(raw, count) != 0) {
        cerr << "  [!] Warning: failed to save data to " << LEDGER_FILE << "\n";
    }
    delete[] raw;
}

// ─── Menu actions ─────────────────────────────────────────────────────────────

static void addExpense(TransactionManager& mgr) {
    Utils::printHeader("Add Expense");

    string date = Utils::readNonEmptyString("  Date (YYYY-MM-DD) [leave blank for today]: ");
    if (date.empty() || date == " ") date = Utils::currentDate();

    // Re-read if the user just pressed Enter (blank)
    if (!Utils::isValidDate(date)) {
        // Offer today as fallback
        cout << "  [!] Invalid date format. Using today's date: "
                  << Utils::currentDate() << "\n";
        date = Utils::currentDate();
    }

    string category = Utils::readNonEmptyString("  Category (e.g. Food, Travel): ");
    double amount        = Utils::readPositiveDouble("  Amount (Rs.): ");

    try {
        mgr.addTransaction(date, category, amount);
        cout << "  [✓] Transaction added successfully.\n";
    } catch (const exception& e) {
        cout << "  [!] Error: " << e.what() << "\n";
    }
}

static void viewAll(const TransactionManager& mgr) {
    Utils::printHeader("All Transactions");
    mgr.viewAll();
}

static void deleteExpense(TransactionManager& mgr) {
    Utils::printHeader("Delete Transaction");
    mgr.viewAll();
    if (mgr.getAll().empty()) return;

    int id = Utils::readPositiveInt("\n  Enter Transaction ID to delete: ");
    if (mgr.deleteTransaction(id)) {
        cout << "  [✓] Transaction #" << id << " deleted.\n";
    } else {
        cout << "  [!] No transaction found with ID " << id << ".\n";
    }
}

static void searchByCategory(const TransactionManager& mgr) {
    Utils::printHeader("Search by Category");

    const auto& cats = mgr.getCategories();
    if (cats.empty()) {
       cout << "  No categories recorded yet.\n";
        return;
    }

    cout << "  Available categories: ";
    bool first = true;
    for (const auto& c : cats) {
        if (!first) cout << ", ";
        cout << c;
        first = false;
    }
    cout << "\n\n";

    string cat = Utils::readNonEmptyString("  Enter category: ");
    auto results    = mgr.searchByCategory(cat);

    if (results.empty()) {
        cout << "  No transactions found for category: " << cat << "\n";
        return;
    }

   cout << "\n  Results for \"" << cat << "\":\n";
    cout << string(52, '-') << "\n";
    double total = 0.0;
    for (const auto& t : results) { t.display(); total += t.amount; }
    cout << string(52, '-') << "\n";
    cout << "  Total: Rs." << fixed
              << setprecision(2) << total << "\n";
}

static void generateReport(const TransactionManager& mgr) {
    Utils::printHeader("Generate Report");

    cout << "  1. Monthly Report\n";
    cout << "  2. Category Report\n";
    cout << "  3. Both\n";
    string choice = Utils::readNonEmptyString("\n  Select [1-3]: ");

    ReportSystem rs;
    const auto&  all = mgr.getAll();

    if (choice == "1" || choice == "3") rs.printMonthly(all);
    if (choice == "2" || choice == "3") rs.printCategory(all);
    if (choice != "1" && choice != "2" && choice != "3")
        cout << "  [!] Invalid choice.\n";
}

// ─── Main ─────────────────────────────────────────────────────────────────────

int main() {
    TransactionManager mgr;

    // Load persisted data
    loadFromFile(mgr);

    bool running = true;
    while (running) {
        Utils::printHeader("LedgerLogic – Personal Finance Manager");

        cout << "  1. Add Expense\n";
        cout << "  2. View All Transactions\n";
        cout << "  3. Delete Transaction\n";
        cout << "  4. Search by Category\n";
        cout << "  5. Generate Report\n";
        cout << "  6. Exit\n\n";

        string choice = Utils::readNonEmptyString("  Select option [1-6]: ");

        try {
            if      (choice == "1") addExpense(mgr);
            else if (choice == "2") viewAll(mgr);
            else if (choice == "3") deleteExpense(mgr);
            else if (choice == "4") searchByCategory(mgr);
            else if (choice == "5") generateReport(mgr);
            else if (choice == "6") { running = false; continue; }
            else cout << "  [!] Invalid option. Choose 1-6.\n";
        } catch (const exception& e) {
            cout << "  [!] Unexpected error: " << e.what() << "\n";
        }

        // Save after every mutating operation
        if (choice == "1" || choice == "3") saveToFile(mgr);

        Utils::pausePrompt();
    }

    cout << "\n  Goodbye! Your data has been saved to " << LEDGER_FILE << ".\n\n";
    return 0;
}
