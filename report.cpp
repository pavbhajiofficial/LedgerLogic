#include "report.h"
#include <iostream>
#include <iomanip>
#include <map>
#include <algorithm>

// ─── MonthlyReport ───────────────────────────────────────────────────────────

void MonthlyReport::generate(const std::vector<Transaction>& txns) const {
    if (txns.empty()) {
        std::cout << "  No data available for report.\n";
        return;
    }

    // Key: "YYYY-MM"
    std::map<std::string, double> monthly;
    for (const auto& t : txns) {
        if (t.date.size() >= 7) {
            std::string key = t.date.substr(0, 7); // YYYY-MM
            monthly[key] += t.amount;
        }
    }

    std::cout << "\n  " << title() << "\n";
    std::cout << "  " << std::string(36, '=') << "\n";
    std::cout << std::left << std::setw(14) << "  Month"
              << std::right << std::setw(18) << "Total (Rs.)\n";
    std::cout << "  " << std::string(36, '-') << "\n";

    double grand = 0.0;
    for (const auto& [month, total] : monthly) {
        std::cout << "  " << std::left << std::setw(14) << month
                  << std::right << std::setw(14)
                  << std::fixed << std::setprecision(2) << total << "\n";
        grand += total;
    }
    std::cout << "  " << std::string(36, '-') << "\n";
    std::cout << "  " << std::left << std::setw(14) << "Grand Total"
              << std::right << std::setw(14)
              << std::fixed << std::setprecision(2) << grand << "\n\n";
}

// ─── CategoryReport ──────────────────────────────────────────────────────────

void CategoryReport::generate(const std::vector<Transaction>& txns) const {
    if (txns.empty()) {
        std::cout << "  No data available for report.\n";
        return;
    }

    std::map<std::string, double> catTotals;
    std::map<std::string, int>    catCounts;
    double grand = 0.0;

    for (const auto& t : txns) {
        catTotals[t.category] += t.amount;
        catCounts[t.category]++;
        grand += t.amount;
    }

    // Sort categories by total descending for readability
    std::vector<std::pair<std::string,double>> sorted(catTotals.begin(), catTotals.end());
    std::sort(sorted.begin(), sorted.end(),
              [](const auto& a, const auto& b){ return a.second > b.second; });

    std::cout << "\n  " << title() << "\n";
    std::cout << "  " << std::string(52, '=') << "\n";
    std::cout << std::left  << std::setw(20) << "  Category"
              << std::right << std::setw(8)  << "Count"
              << std::setw(16) << "Total (Rs.)"
              << std::setw(10) << "Share%\n";
    std::cout << "  " << std::string(52, '-') << "\n";

    for (const auto& [cat, total] : sorted) {
        double pct = (grand > 0) ? (total / grand * 100.0) : 0.0;
        std::cout << "  " << std::left  << std::setw(20) << cat
                  << std::right << std::setw(6)  << catCounts[cat]
                  << std::setw(16) << std::fixed << std::setprecision(2) << total
                  << std::setw(8)  << std::fixed << std::setprecision(1) << pct << "%\n";
    }
    std::cout << "  " << std::string(52, '-') << "\n";
    std::cout << "  " << std::left << std::setw(20) << "Grand Total"
              << std::right << std::setw(6)  << (int)txns.size()
              << std::setw(16) << std::fixed << std::setprecision(2) << grand << "\n\n";
}

// ─── ReportSystem ────────────────────────────────────────────────────────────

void ReportSystem::printMonthly(const std::vector<Transaction>& txns) const {
    monthlyRpt.generate(txns);
}

void ReportSystem::printCategory(const std::vector<Transaction>& txns) const {
    categoryRpt.generate(txns);
}
