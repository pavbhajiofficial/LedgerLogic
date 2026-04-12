#ifndef REPORT_H
#define REPORT_H

#include "transaction.h"
#include <string>
#include <map>

// Abstract base for polymorphism
class ReportBase {
public:
    virtual ~ReportBase() = default;
    virtual void generate(const std::vector<Transaction>& txns) const = 0;
    virtual std::string title() const = 0;
};

class MonthlyReport : public ReportBase {
public:
    void generate(const std::vector<Transaction>& txns) const override;
    std::string title() const override { return "Monthly Expense Report"; }
};

class CategoryReport : public ReportBase {
public:
    void generate(const std::vector<Transaction>& txns) const override;
    std::string title() const override { return "Category-wise Expense Report"; }
};

class ReportSystem {
public:
    void printMonthly (const std::vector<Transaction>& txns) const;
    void printCategory(const std::vector<Transaction>& txns) const;

private:
    MonthlyReport  monthlyRpt;
    CategoryReport categoryRpt;
};

#endif
