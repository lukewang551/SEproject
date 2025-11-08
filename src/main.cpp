#include "TransactionManager.h"
#include <iostream>

int main()
{
    TransactionManager manager("data.txt");
    manager.load();
    std::cout << "\nWelcome to Personal Finance System!\n";
    std::cout << "1. Add Transaction\n";
    std::cout << "2. Delete Transaction\n";
    std::cout << "3. View All Transactions\n";
    std::cout << "4. Filter by Amount Range\n";
    std::cout << "5. Search by Counterpart\n";
    std::cout << "6. View Monthly Summary\n";
    std::cout << "7. View Annual Summary\n";
    std::cout << "0. Exit\n";

    int choice;
    while (true)
    {
        std::cout << "\nEnter choice: ";
        std::cin >> choice;
        if (choice == 0)
            break;

        if (choice == 1)
        {
            double amt;
            std::string cp, cat, note;
            int pm, incomeFlag;
            DateTime dt;
            std::cout << "Enter amount: ";
            std::cin >> amt;
            std::cout << "Is it income? (1 yes, 0 no): ";
            std::cin >> incomeFlag;
            std::cout << "Enter payment method (0 WeChat, 1 Alipay, 2 Cash, 3 Credit Card, 4 Bank Transfer): ";
            std::cin >> pm;
            std::cout << "Enter counterpart: ";
            std::cin >> cp;
            std::cout << "Enter category: ";
            std::cin >> cat;
            std::cout << "Enter date (year month day hour minute): ";
            std::cin >> dt.year >> dt.month >> dt.day >> dt.hour >> dt.minute;
            std::cout << "Any notes? (1 yes, 0 no): ";
            bool haveNotes;
            std::cin >> haveNotes;
            if (haveNotes)
            {
                std::cout << "Enter notes: ";
                std::cin >> note;
            }
            else
                note = "NULL";

            std::string id = "T" + std::to_string(rand() % 100000);
            Transaction t(id, amt, dt, cp, incomeFlag, (PaymentMethod)pm, cat, note);
            manager.addTransaction(t);
        }
        else if (choice == 2)
        {
            std::string id;
            std::cout << "Enter transaction ID to delete: ";
            std::cin >> id;
            manager.deleteTransaction(id);
        }
        else if (choice == 3)
        {
            manager.listAll();
        }
        else if (choice == 4)
        {
            double min, max;
            std::cout << "Enter min and max amount: ";
            std::cin >> min >> max;
            auto list = manager.filterByAmountRange(min, max);
            for (auto &t : list)
                t.displayInfo();
        }
        else if (choice == 5)
        {
            std::string kw;
            std::cout << "Enter counterpart keyword: ";
            std::cin >> kw;
            auto res = manager.searchByCounterpart(kw);
            for (auto &t : res)
                t.displayInfo();
        }
        else if (choice == 6)
        {
            int y, m;
            std::cout << "Enter year and month: ";
            std::cin >> y >> m;
            manager.getMonthlySummary(y, m).displaySummary();
        }
        else if (choice == 7)
        {
            int y;
            std::cout << "Enter year: ";
            std::cin >> y;
            manager.getAnnualSummary(y).displaySummary();
        }
    }

    manager.save();
    std::cout << "Data saved. Exiting program.\n";
    system("pause");
    return 0;
}
