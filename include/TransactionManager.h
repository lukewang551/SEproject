#pragma once
#include "DatabaseManager.h"
#include "MonthlySummary.h"
#include "Transaction.h"
#include <string>
#include <vector>

class TransactionManager
{
  private:
    std::vector<Transaction> transactions;
    DatabaseManager dbManager;

  public:
    TransactionManager(std::string dbPath);

    void addTransaction(const Transaction &t);
    bool deleteTransaction(const std::string &id);
    bool editTransaction(const std::string &id, const Transaction &newT);
    void listAll() const;

    std::vector<Transaction> filterByPaymentMethod(PaymentMethod m) const;
    std::vector<Transaction> filterByAmountRange(double min, double max) const;
    std::vector<Transaction> searchByCounterpart(const std::string &keyword) const;

    MonthlySummary getMonthlySummary(int year, int month) const;
    bool save();
    bool load();
};
