#include "TransactionManager.h"
#include <algorithm>
#include <iostream>

TransactionManager::TransactionManager(std::string dbPath) : dbManager(dbPath)
{
}

void TransactionManager::addTransaction(const Transaction &t)
{
    transactions.push_back(t);
    std::cout << "Transaction added successfully.\n";
}

bool TransactionManager::deleteTransaction(const std::string &id)
{
    auto it =
        std::remove_if(transactions.begin(), transactions.end(), [&](const Transaction &t) { return t.getId() == id; });
    if (it != transactions.end())
    {
        transactions.erase(it, transactions.end());
        std::cout << "Transaction deleted.\n";
        return true;
    }
    std::cout << "Transaction not found.\n";
    return false;
}

bool TransactionManager::editTransaction(const std::string &id, const Transaction &newT)
{
    for (auto &t : transactions)
    {
        if (t.getId() == id)
        {
            t = newT;
            std::cout << "Transaction updated.\n";
            return true;
        }
    }
    std::cout << "Transaction not found.\n";
    return false;
}

void TransactionManager::listAll() const
{
    if (transactions.empty())
    {
        std::cout << "No transactions available.\n";
        return;
    }
    for (const auto &t : transactions)
        t.displayInfo();
}

std::vector<Transaction> TransactionManager::filterByPaymentMethod(PaymentMethod m) const
{
    std::vector<Transaction> result;
    for (const auto &t : transactions)
    {
        if (t.getPaymentMethod() == m)
            result.push_back(t);
    }
    return result;
}

std::vector<Transaction> TransactionManager::filterByAmountRange(double min, double max) const
{
    std::vector<Transaction> result;
    for (const auto &t : transactions)
    {
        if (t.getAmount() >= min && t.getAmount() <= max)
            result.push_back(t);
    }
    return result;
}

std::vector<Transaction> TransactionManager::searchByCounterpart(const std::string &keyword) const
{
    std::vector<Transaction> result;
    for (const auto &t : transactions)
    {
        if (t.getCounterpart().find(keyword) != std::string::npos)
            result.push_back(t);
    }
    return result;
}

MonthlySummary TransactionManager::getMonthlySummary(int year, int month) const
{
    double income = 0, expense = 0;
    for (const auto &t : transactions)
    {
        DateTime dt = t.getDateTime();
        if (dt.year == year && dt.month == month)
        {
            if (t.getIsIncome())
                income += t.getAmount();
            else
                expense += t.getAmount();
        }
    }
    return MonthlySummary(year, month, income, expense);
}

bool TransactionManager::save()
{
    return dbManager.saveTransactions(transactions);
}

bool TransactionManager::load()
{
    transactions = dbManager.loadTransactions();
    return true;
}
