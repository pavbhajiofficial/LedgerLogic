#include "report.h"
#include <iostream>
#include <iomanip>
#include <map>
#include <algorithm>

using namespace std;

// ─── MonthlyReport ───────────────────────────────────────────────────────────

void MonthlyReport::generate(const vector<Transaction>& txns) const {
    if (txns.empty()) {
        cout << "  No data available for report.\n";
        return;
    }

    // Key: "YYYY-MM"
    map<string, double> monthly;
    for (const auto& t : txns) {
        if (t.date.size() >= 7) {
            string key = t.date.substr(0, 7); // YYYY-MM
            monthly[key] += t.amount;
        }
    }

    cout << "\n  " << title() << "\n";
    cout << "  " << string(36, '=') << "\n";
    cout << left << setw(14) << "  Month"
              << right << setw(18) << "Total (Rs.)\n";
    cout << "  " << string(36, '-') << "\n";

    double grand = 0.0;
    for (const auto& [month, total] : monthly) {
        cout << "  " << left << setw(14) << month
                  << right << setw(14)
                  << fixed << setprecision(2) << total << "\n";
        grand += total;
    }
    cout << "  " << string(36, '-') << "\n";
    cout << "  " << left << setw(14) << "Grand Total"
              << right << setw(14)
              << fixed << setprecision(2) << grand << "\n\n";
}

// ─── CategoryReport ──────────────────────────────────────────────────────────

void CategoryReport::generate(const vector<Transaction>& txns) const {
    if (txns.empty()) {
        cout << "  No data available for report.\n";
        return;
    }

    map<string, double> catTotals;
    map<string, int>    catCounts;
    double grand = 0.0;

    for (const auto& t : txns) {
        catTotals[t.category] += t.amount;
        catCounts[t.category]++;
        grand += t.amount;
    }

    // Sort categories by total descending for readability
    vector<pair<string,double>> sorted(catTotals.begin(), catTotals.end());
    sort(sorted.begin(), sorted.end(),
              [](const auto& a, const auto& b){ return a.second > b.second; });

    cout << "\n  " << title() << "\n";
    cout << "  " << string(52, '=') << "\n";
    cout << left  << setw(20) << "  Category"
              << right << setw(8)  << "Count"
              << setw(16) << "Total (Rs.)"
              << setw(10) << "Share%\n";
    cout << "  " << string(52, '-') << "\n";

    for (const auto& [cat, total] : sorted) {
        double pct = (grand > 0) ? (total / grand * 100.0) : 0.0;
        cout << "  " << left  << setw(20) << cat
                  << right << setw(6)  << catCounts[cat]
                  << setw(16) << fixed << setprecision(2) << total
                  << setw(8)  << fixed << setprecision(1) << pct << "%\n";
    }
    cout << "  " << string(52, '-') << "\n";
    cout << "  " << left << setw(20) << "Grand Total"
              << right << setw(6)  << (int)txns.size()
              << setw(16) << fixed << setprecision(2) << grand << "\n\n";
}

// ─── ReportSystem ────────────────────────────────────────────────────────────

void ReportSystem::printMonthly(const vector<Transaction>& txns) const {
    monthlyRpt.generate(txns);
}

void ReportSystem::printCategory(const vector<Transaction>& txns) const {
    categoryRpt.generate(txns);
}
