#pragma once
#include "Transaction.h"
#include <iostream>
#include <vector>

class Summary
{
  private:
    int year, month; 
    double totalIncome;
    double totalExpense;

  public:
    
    Summary(int y, int m, double in, double out);
    void displaySummary() const;
};
