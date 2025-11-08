#pragma once
#include "PaymentMethod.h"
#include <iostream>
#include <string>

struct DateTime
{
    int year, month, day, hour, minute;
};

class Transaction
{
  private:
    std::string transactionId;
    double amount;
    DateTime dateTime;
    std::string counterpart;
    std::string notes;
    bool isIncome;
    PaymentMethod paymentMethod;
    std::string category;

  public:
    Transaction() = default;
    Transaction(std::string id, double amt, DateTime dt, std::string cp, bool income, PaymentMethod pm, std::string cat,
                std::string note = "");

    std::string getId() const
    {
        return transactionId;
    }
    double getAmount() const
    {
        return amount;
    }
    bool getIsIncome() const
    {
        return isIncome;
    }
    PaymentMethod getPaymentMethod() const
    {
        return paymentMethod;
    }
    std::string getCounterpart() const
    {
        return counterpart;
    }
    DateTime getDateTime() const
    {
        return dateTime;
    }
    std::string getCategory() const
    {
        return category;
    }
    std::string getNotes() const
    {
        return notes;
    }

    std::string getAmountColor() const;
    void displayInfo() const;
};
