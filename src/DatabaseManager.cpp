#include "DatabaseManager.h"
#include <fstream>
#include <iostream>
#include <sstream>

DatabaseManager::DatabaseManager(std::string path) : dbPath(path)
{
}

bool DatabaseManager::saveTransactions(const std::vector<Transaction> &ts) const
{
    std::ofstream ofs(dbPath);
    if (!ofs.is_open())
    {
        std::cerr << "Failed to open file for writing: " << dbPath << "\n";
        return false;
    }

    for (const auto &t : ts)
    {
        DateTime dt = t.getDateTime();
        ofs << t.getId() << "," << t.getAmount() << "," << dt.year << "," << dt.month << "," << dt.day << "," << dt.hour
            << "," << dt.minute << "," << t.getCounterpart() << "," << (t.getIsIncome() ? 1 : 0) << ","
            << static_cast<int>(t.getPaymentMethod()) << "," << t.getCategory() << ","
            << t.getCounterpart() // notes, optional, can be empty
            << "\n";
    }

    ofs.close();
    return true;
}

std::vector<Transaction> DatabaseManager::loadTransactions() const
{
    std::vector<Transaction> ts;
    std::ifstream ifs(dbPath);
    if (!ifs.is_open())
    {
        std::cerr << "Failed to open file for reading: " << dbPath << "\n";
        return ts; // 返回空向量
    }

    std::string line;
    while (std::getline(ifs, line))
    {
        std::stringstream ss(line);
        std::string item;

        std::string id, counterpart, category, notes;
        double amount;
        int year, month, day, hour, minute;
        bool isIncome;
        int pm;

        std::getline(ss, id, ',');
        std::getline(ss, item, ',');
        amount = std::stod(item);
        std::getline(ss, item, ',');
        year = std::stoi(item);
        std::getline(ss, item, ',');
        month = std::stoi(item);
        std::getline(ss, item, ',');
        day = std::stoi(item);
        std::getline(ss, item, ',');
        hour = std::stoi(item);
        std::getline(ss, item, ',');
        minute = std::stoi(item);
        std::getline(ss, counterpart, ',');
        std::getline(ss, item, ',');
        isIncome = (item == "1");
        std::getline(ss, item, ',');
        pm = std::stoi(item);
        std::getline(ss, category, ',');
        std::getline(ss, notes, ',');

        DateTime dt{year, month, day, hour, minute};
        Transaction t(id, amount, dt, counterpart, isIncome, static_cast<PaymentMethod>(pm), category, notes);
        ts.push_back(t);
    }

    ifs.close();
    return ts;
}
