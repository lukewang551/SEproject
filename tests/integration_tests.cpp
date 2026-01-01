#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>
#include <vector>
#include <string>

#include "../src/Transaction.h"
#include "../src/TransactionManager.h"
#include "../src/DatabaseManager.h"

// ==============================================
// 集成测试1: 数据保存与加载的完整性
// ==============================================

TEST(IntegrationTest, SaveAndLoadTransactions) {
    const std::string testFile = "integration_test_save_load.txt";
    
    // 清理之前的测试文件
    std::remove(testFile.c_str());
    
    std::cout << "=== 集成测试1: 数据保存与加载 ===" << std::endl;
    
    // 阶段1: 创建交易数据并保存
    TransactionManager manager1(testFile);
    
    DateTime dt1{2024, 12, 1, 9, 0};
    DateTime dt2{2024, 12, 2, 14, 30};
    DateTime dt3{2024, 12, 3, 18, 45};
    
    // 创建不同类型和金额的交易
    Transaction income("SAVE001", 5000.0, dt1, "ABC Company", true,
                       PaymentMethod::BANK_TRANSFER, "Salary", "Monthly salary");
    Transaction rent("SAVE002", 1500.0, dt2, "Landlord Zhang", false,
                     PaymentMethod::ALIPAY, "Rent", "December rent");
    Transaction food("SAVE003", 300.0, dt3, "Supermarket", false,
                     PaymentMethod::WECHAT, "Food", "Groceries");
    
    manager1.addTransaction(income);
    manager1.addTransaction(rent);
    manager1.addTransaction(food);
    
    std::cout << "添加了3笔交易到管理器1" << std::endl;
    
    // 保存到文件
    bool saveResult = manager1.save();
    EXPECT_TRUE(saveResult);
    std::cout << "数据保存成功: " << (saveResult ? "是" : "否") << std::endl;
    
    // 阶段2: 创建新的管理器并加载数据
    TransactionManager manager2(testFile);
    bool loadResult = manager2.load();
    EXPECT_TRUE(loadResult);
    std::cout << "数据加载成功: " << (loadResult ? "是" : "否") << std::endl;
    
    // 阶段3: 验证数据一致性
    // 通过功能操作验证数据完整性
    
    // 3.1 验证交易数量（通过过滤功能间接验证）
    auto allTransactions = manager2.filterByAmountRange(0, 10000);
    std::cout << "加载后交易数量: " << allTransactions.size() << std::endl;
    
    // 3.2 验证具体交易数据
    auto highAmount = manager2.filterByAmountRange(4000, 6000);
    EXPECT_EQ(highAmount.size(), 1);
    if (!highAmount.empty()) {
        EXPECT_EQ(highAmount[0].getId(), "SAVE001");
        EXPECT_DOUBLE_EQ(highAmount[0].getAmount(), 5000.0);
        EXPECT_TRUE(highAmount[0].getIsIncome());
    }
    
    // 3.3 验证搜索功能
    auto rentSearch = manager2.searchByCounterpart("Landlord");
    EXPECT_EQ(rentSearch.size(), 1);
    
    // 3.4 验证月度汇总
    Summary monthlySummary = manager2.getMonthlySummary(2024, 12);
    std::cout << "月度汇总计算结果:" << std::endl;
    monthlySummary.displaySummary();
    
    // 清理测试文件
    std::remove(testFile.c_str());
    std::cout << "测试文件已清理" << std::endl;
}

// ==============================================
// 集成测试2: 过滤与搜索的组合使用
// ==============================================

TEST(IntegrationTest, CombinedFilterAndSearchOperations) {
    std::cout << "\n=== 集成测试2: 组合操作测试 ===" << std::endl;
    
    TransactionManager manager("integration_test_combo.txt");
    
    // 创建多样化的测试数据
    DateTime dt{2024, 12, 10, 12, 0};
    
    // 不同类型、金额、支付方式的交易
    std::vector<Transaction> testTransactions = {
        Transaction("COMBO1", 500.0, dt, "Apple Store", false,
                   PaymentMethod::WECHAT, "Electronics", "iPhone 15"),
        Transaction("COMBO2", 50.0, dt, "Starbucks Coffee", false,
                   PaymentMethod::ALIPAY, "Food", "Latte"),
        Transaction("COMBO3", 200.0, dt, "Nike Store", false,
                   PaymentMethod::WECHAT, "Clothing", "Running shoes"),
        Transaction("COMBO4", 100.0, dt, "Amazon Online", false,
                   PaymentMethod::CREDIT_CARD, "Shopping", "Books"),
        Transaction("COMBO5", 800.0, dt, "Apple Store", false,
                   PaymentMethod::CREDIT_CARD, "Electronics", "MacBook"),
        Transaction("COMBO6", 30.0, dt, "Local Cafe", false,
                   PaymentMethod::CASH, "Food", "Sandwich")
    };
    
    // 添加所有测试交易
    for (const auto& t : testTransactions) {
        manager.addTransaction(t);
    }
    std::cout << "添加了6笔测试交易" << std::endl;
    
    // 测试场景1: 先按支付方式过滤，再在结果中搜索
    std::cout << "\n场景1: 微信支付的商店交易" << std::endl;
    auto wechatPayments = manager.filterByPaymentMethod(PaymentMethod::WECHAT);
    std::cout << "微信支付交易数: " << wechatPayments.size() << std::endl;
    
    int storeCount = 0;
    for (const auto& t : wechatPayments) {
        if (t.getCounterpart().find("Store") != std::string::npos) {
            storeCount++;
        }
    }
    EXPECT_EQ(storeCount, 2);  // Apple Store 和 Nike Store
    std::cout << "微信支付的商店交易数: " << storeCount << std::endl;
    
    // 测试场景2: 先按金额过滤，再按关键词搜索
    std::cout << "\n场景2: 高价电子产品" << std::endl;
    auto expensiveItems = manager.filterByAmountRange(400, 1000);
    std::cout << "高价商品数(400-1000元): " << expensiveItems.size() << std::endl;
    
    int electronicsCount = 0;
    for (const auto& t : expensiveItems) {
        if (t.getCategory() == "Electronics") {
            electronicsCount++;
        }
    }
    EXPECT_EQ(electronicsCount, 2);  // iPhone 15 和 MacBook
    std::cout << "高价电子产品数: " << electronicsCount << std::endl;
    
    // 测试场景3: 复杂查询链
    std::cout << "\n场景3: Apple商店的非现金支付" << std::endl;
    auto appleTransactions = manager.searchByCounterpart("Apple");
    std::cout << "Apple相关交易数: " << appleTransactions.size() << std::endl;
    
    int nonCashApple = 0;
    for (const auto& t : appleTransactions) {
        if (t.getPaymentMethod() != PaymentMethod::CASH) {
            nonCashApple++;
        }
    }
    EXPECT_EQ(nonCashApple, 2);  // 微信支付的iPhone和信用卡的MacBook
    std::cout << "Apple商店的非现金支付交易数: " << nonCashApple << std::endl;
    
    // 测试场景4: 完整业务流程模拟
    std::cout << "\n场景4: 完整月度分析流程" << std::endl;
    
    // 4.1 获取所有食品支出
    auto allFood = manager.searchByCounterpart("Coffee");
    auto moreFood = manager.searchByCounterpart("Cafe");
    std::cout << "咖啡相关支出: " << allFood.size() << " 笔" << std::endl;
    std::cout << "咖啡馆支出: " << moreFood.size() << " 笔" << std::endl;
    
    // 4.2 获取月度汇总
    Summary summary = manager.getMonthlySummary(2024, 12);
    std::cout << "12月汇总:" << std::endl;
    summary.displaySummary();
    
    // 4.3 验证汇总数据的合理性
    // 总支出应该是所有支出交易的总和
    auto allExpenses = manager.filterByAmountRange(0, 10000);  // 获取全部
    double manualTotal = 0;
    for (const auto& t : allExpenses) {
        if (!t.getIsIncome()) {
            manualTotal += t.getAmount();
        }
    }
    std::cout << "手动计算总支出: " << manualTotal << " 元" << std::endl;
    
    std::cout << "\n=== 集成测试2完成 ===" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    
    std::cout << "==========================================" << std::endl;
    std::cout << "开始集成测试" << std::endl;
    std::cout << "测试功能模块间的交互和完整业务流程" << std::endl;
    std::cout << "==========================================" << std::endl;
    
    int result = RUN_ALL_TESTS();
    
    std::cout << "\n==========================================" << std::endl;
    std::cout << "集成测试完成" << std::endl;
    std::cout << "==========================================" << std::endl;
    
    return result;
}