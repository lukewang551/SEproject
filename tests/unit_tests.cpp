#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "../src/Transaction.h"
#include "../src/TransactionManager.h"

// ==============================================
// 子功能1: TransactionManager::filterByAmountRange()
// ==============================================

TEST(FilterByAmountRangeTest, EmptyList) {
    TransactionManager manager("test.txt");
    auto result = manager.filterByAmountRange(0, 100);
    EXPECT_EQ(result.size(), 0);
}

TEST(FilterByAmountRangeTest, SingleTransactionInRange) {
    TransactionManager manager("test.txt");
    DateTime dt{2024, 12, 25, 10, 0};
    manager.addTransaction(Transaction("T1", 50.0, dt, "Test", true, 
                                       PaymentMethod::CASH, "Test"));
    
    auto result = manager.filterByAmountRange(0, 100);
    EXPECT_EQ(result.size(), 1);
}

TEST(FilterByAmountRangeTest, SingleTransactionOutOfRange) {
    TransactionManager manager("test.txt");
    DateTime dt{2024, 12, 25, 10, 0};
    manager.addTransaction(Transaction("T1", 150.0, dt, "Test", true, 
                                       PaymentMethod::CASH, "Test"));
    
    auto result = manager.filterByAmountRange(0, 100);
    EXPECT_EQ(result.size(), 0);
}

TEST(FilterByAmountRangeTest, MultipleTransactionsSomeInRange) {
    TransactionManager manager("test.txt");
    DateTime dt{2024, 12, 25, 10, 0};
    
    manager.addTransaction(Transaction("T1", 30.0, dt, "A", true, PaymentMethod::CASH, "Test"));
    manager.addTransaction(Transaction("T2", 60.0, dt, "B", true, PaymentMethod::CASH, "Test"));
    manager.addTransaction(Transaction("T3", 90.0, dt, "C", true, PaymentMethod::CASH, "Test"));
    manager.addTransaction(Transaction("T4", 120.0, dt, "D", true, PaymentMethod::CASH, "Test"));
    
    auto result = manager.filterByAmountRange(50, 100);
    EXPECT_EQ(result.size(), 2);  // 60和90
}

TEST(FilterByAmountRangeTest, ExactLowerBound) {
    TransactionManager manager("test.txt");
    DateTime dt{2024, 12, 25, 10, 0};
    manager.addTransaction(Transaction("T1", 50.0, dt, "Test", true, 
                                       PaymentMethod::CASH, "Test"));
    
    auto result = manager.filterByAmountRange(50, 100);
    EXPECT_EQ(result.size(), 1);
}

TEST(FilterByAmountRangeTest, ExactUpperBound) {
    TransactionManager manager("test.txt");
    DateTime dt{2024, 12, 25, 10, 0};
    manager.addTransaction(Transaction("T1", 100.0, dt, "Test", true, 
                                       PaymentMethod::CASH, "Test"));
    
    auto result = manager.filterByAmountRange(50, 100);
    EXPECT_EQ(result.size(), 1);
}

TEST(FilterByAmountRangeTest, NegativeAmount) {
    TransactionManager manager("test.txt");
    DateTime dt{2024, 12, 25, 10, 0};
    manager.addTransaction(Transaction("T1", -50.0, dt, "Refund", true, 
                                       PaymentMethod::CASH, "Refund"));
    
    auto result = manager.filterByAmountRange(-100, 0);
    EXPECT_EQ(result.size(), 1);
}

TEST(FilterByAmountRangeTest, ZeroAmount) {
    TransactionManager manager("test.txt");
    DateTime dt{2024, 12, 25, 10, 0};
    manager.addTransaction(Transaction("T1", 0.0, dt, "Test", true, 
                                       PaymentMethod::CASH, "Test"));
    
    auto result = manager.filterByAmountRange(0, 0);
    EXPECT_EQ(result.size(), 1);
}

TEST(FilterByAmountRangeTest, LargeRange) {
    TransactionManager manager("test.txt");
    DateTime dt{2024, 12, 25, 10, 0};
    
    for (int i = 1; i <= 20; i++) {
        manager.addTransaction(Transaction("T" + std::to_string(i), i * 10.0, dt, 
                                           "Test", true, PaymentMethod::CASH, "Test"));
    }
    
    auto result = manager.filterByAmountRange(50, 150);
    EXPECT_EQ(result.size(), 11);  // 50,60,...,150
}

TEST(FilterByAmountRangeTest, InvalidRange) {
    TransactionManager manager("test.txt");
    DateTime dt{2024, 12, 25, 10, 0};
    manager.addTransaction(Transaction("T1", 50.0, dt, "Test", true, 
                                       PaymentMethod::CASH, "Test"));
    
    // 下限大于上限
    auto result = manager.filterByAmountRange(100, 50);
    EXPECT_EQ(result.size(), 0);
}

// ==============================================
// 子功能2: TransactionManager::searchByCounterpart()
// ==============================================

TEST(SearchByCounterpartTest, EmptyList) {
    TransactionManager manager("test.txt");
    auto result = manager.searchByCounterpart("Test");
    EXPECT_EQ(result.size(), 0);
}

TEST(SearchByCounterpartTest, ExactMatch) {
    TransactionManager manager("test.txt");
    DateTime dt{2024, 12, 25, 10, 0};
    manager.addTransaction(Transaction("T1", 100.0, dt, "Apple Store", false,
                                       PaymentMethod::CREDIT_CARD, "Electronics"));
    
    auto result = manager.searchByCounterpart("Apple Store");
    EXPECT_EQ(result.size(), 1);
}

TEST(SearchByCounterpartTest, PartialMatch) {
    TransactionManager manager("test.txt");
    DateTime dt{2024, 12, 25, 10, 0};
    manager.addTransaction(Transaction("T1", 100.0, dt, "Apple Store", false,
                                       PaymentMethod::CREDIT_CARD, "Electronics"));
    
    auto result = manager.searchByCounterpart("Apple");
    EXPECT_EQ(result.size(), 1);
}

TEST(SearchByCounterpartTest, CaseSensitive) {
    TransactionManager manager("test.txt");
    DateTime dt{2024, 12, 25, 10, 0};
    manager.addTransaction(Transaction("T1", 100.0, dt, "Apple Store", false,
                                       PaymentMethod::CREDIT_CARD, "Electronics"));
    
    auto result = manager.searchByCounterpart("apple");  // 小写
    EXPECT_EQ(result.size(), 0);  // 应该是大小写敏感
}

TEST(SearchByCounterpartTest, MultipleMatches) {
    TransactionManager manager("test.txt");
    DateTime dt{2024, 12, 25, 10, 0};
    
    manager.addTransaction(Transaction("T1", 100.0, dt, "Apple Store", false,
                                       PaymentMethod::CREDIT_CARD, "Electronics"));
    manager.addTransaction(Transaction("T2", 50.0, dt, "Microsoft Store", false,
                                       PaymentMethod::ALIPAY, "Software"));
    manager.addTransaction(Transaction("T3", 30.0, dt, "Starbucks", false,
                                       PaymentMethod::WECHAT, "Food"));
    
    auto result = manager.searchByCounterpart("Store");
    EXPECT_EQ(result.size(), 2);
}

TEST(SearchByCounterpartTest, NoMatch) {
    TransactionManager manager("test.txt");
    DateTime dt{2024, 12, 25, 10, 0};
    manager.addTransaction(Transaction("T1", 100.0, dt, "Apple Store", false,
                                       PaymentMethod::CREDIT_CARD, "Electronics"));
    
    auto result = manager.searchByCounterpart("Banana");
    EXPECT_EQ(result.size(), 0);
}

TEST(SearchByCounterpartTest, EmptySearchString) {
    TransactionManager manager("test.txt");
    DateTime dt{2024, 12, 25, 10, 0};
    manager.addTransaction(Transaction("T1", 100.0, dt, "Apple Store", false,
                                       PaymentMethod::CREDIT_CARD, "Electronics"));
    
    auto result = manager.searchByCounterpart("");
    EXPECT_EQ(result.size(), 1);  // 空字符串应该匹配所有
}

TEST(SearchByCounterpartTest, SpecialCharacters) {
    TransactionManager manager("test.txt");
    DateTime dt{2024, 12, 25, 10, 0};
    manager.addTransaction(Transaction("T1", 100.0, dt, "Café & Restaurant", false,
                                       PaymentMethod::CASH, "Food"));
    
    auto result = manager.searchByCounterpart("Café");
    EXPECT_EQ(result.size(), 1);
}

TEST(SearchByCounterpartTest, LongString) {
    TransactionManager manager("test.txt");
    DateTime dt{2024, 12, 25, 10, 0};
    std::string longName = "Very Long Business Name With Multiple Words Corporation Limited";
    manager.addTransaction(Transaction("T1", 100.0, dt, longName, false,
                                       PaymentMethod::BANK_TRANSFER, "Service"));
    
    auto result = manager.searchByCounterpart("Corporation");
    EXPECT_EQ(result.size(), 1);
}

TEST(SearchByCounterpartTest, WhitespaceInSearch) {
    TransactionManager manager("test.txt");
    DateTime dt{2024, 12, 25, 10, 0};
    manager.addTransaction(Transaction("T1", 100.0, dt, "Apple  Store", false,  // 两个空格
                                       PaymentMethod::CREDIT_CARD, "Electronics"));
    
    auto result = manager.searchByCounterpart("Apple Store");

    EXPECT_EQ(result.size(), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}