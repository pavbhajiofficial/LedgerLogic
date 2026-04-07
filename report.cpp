#ifndef REPORT_H
#define REPORT_H

#include <vector>
#include "transaction.h"

class ExpenseSummary {
private:
    std::vector<Transaction> data;

public:
    ExpenseSummary() = default;

    ExpenseSummary(ExpenseSummary&& other) noexcept {
        data = std::move(other.data);
    }

    void add(const Transaction& t);
};

class Report {
public:
    virtual void generate() = 0;
    virtual ~Report() {}
};

class MonthlyReport : public Report {
public:
    void generate() override;
};

class CategoryReport : public Report {
public:
    void generate() override;
};

#endif
