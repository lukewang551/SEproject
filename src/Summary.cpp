#include "Summary.h"

Summary::Summary(int y, int m, double in, double out) : year(y), month(m), totalIncome(in), totalExpense(out)
{
}

void Summary::displaySummary() const
{
    if (month == 0)
        std::cout << "=== Yearly Summary: " << year << " ===\n";
    else
        std::cout << "=== Monthly Summary: " << year << "-" << month << " ===\n";

    std::cout << "Total Income: " << totalIncome << ", Total Expense: " << totalExpense << "\n";
    std::cout << "Net Amount: " << (totalIncome - totalExpense) << "\n";
}
