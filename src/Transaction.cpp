#include "Transaction.h"

Transaction::Transaction(std::string id, double amt, DateTime dt, std::string cp, bool income, PaymentMethod pm,
                         std::string cat, std::string note)
    : transactionId(id), amount(amt), dateTime(dt), counterpart(cp), notes(note), isIncome(income), paymentMethod(pm),
      category(cat)
{
}

// Print all transaction details in a clean format
void Transaction::displayInfo() const
{
    std::cout << "----------------------------------------\n";
    std::cout << "Transaction ID : " << transactionId << "\n";
    std::cout << "Type           : " << (isIncome ? "Income" : "Expense") << "\n";
    std::cout << "Amount         : " << amount << "\n";
    std::cout << "Category       : " << category << "\n";
    std::cout << "Payment Method : ";

    switch (paymentMethod)
    {
    case PaymentMethod::WECHAT:
        std::cout << "WeChat";
        break;
    case PaymentMethod::ALIPAY:
        std::cout << "Alipay";
        break;
    case PaymentMethod::CASH:
        std::cout << "Cash";
        break;
    case PaymentMethod::CREDIT_CARD:
        std::cout << "Credit Card";
        break;
    case PaymentMethod::BANK_TRANSFER:
        std::cout << "Bank Transfer";
        break;
    }
    std::cout << "\n";

    std::cout << "Counterpart    : " << counterpart << "\n";
    std::cout << "Date & Time    : " << dateTime.year << "-" << dateTime.month << "-" << dateTime.day << " "
              << dateTime.hour << ":" << (dateTime.minute < 10 ? "0" : "") << dateTime.minute << "\n";
    if (!notes.empty())
        std::cout << "Notes          : " << notes << "\n";
    std::cout << "----------------------------------------\n";
}
