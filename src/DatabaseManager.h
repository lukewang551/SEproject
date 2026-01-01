#pragma once
#include "Transaction.h"
#include <string>
#include <vector>

class DatabaseManager
{
  private:
    std::string dbPath;

  public:
    DatabaseManager(std::string path);
    bool saveTransactions(const std::vector<Transaction> &ts) const;
    std::vector<Transaction> loadTransactions() const;
};
