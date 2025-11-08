#include "MonthlySummary.h"
#include <iostream>

MonthlySummary::MonthlySummary(int y, int m, double in, double out)
    : year(y), month(m), totalIncome(in), totalExpense(out)
{
}

void MonthlySummary::displaySummary() const
{
    std::cout << "===== Monthly Summary =====\n";
    std::cout << "Year: " << year << ", Month: " << month << "\n";
    std::cout << "Total Income: " << totalIncome << "\n";
    std::cout << "Total Expense: " << totalExpense << "\n";
    std::cout << "Net Amount: " << (totalIncome - totalExpense) << "\n";
}
