#pragma once
#include <iostream>

class MonthlySummary
{
  private:
    int year, month;
    double totalIncome, totalExpense;

  public:
    MonthlySummary(int y, int m, double in, double out);
    void displaySummary() const;
};
