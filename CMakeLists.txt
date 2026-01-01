cmake_minimum_required(VERSION 3.10)
project(FinanceSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 在包含googletest之前设置编译定义
add_definitions(-DGTEST_HAS_PTHREAD=0)

# 包含本地googletest
add_subdirectory(googletest)

# 主程序
add_executable(FinanceSystem
    src/main.cpp
    src/DatabaseManager.cpp
    src/Summary.cpp
    src/Transaction.cpp
    src/TransactionManager.cpp
)

# 单元测试程序
add_executable(UnitTests
    tests/unit_tests.cpp
    src/DatabaseManager.cpp
    src/Summary.cpp
    src/Transaction.cpp
    src/TransactionManager.cpp
)

# 链接GTest库，并禁用线程支持
target_link_libraries(UnitTests
    gtest
    gtest_main
)

# 为测试程序也禁用线程支持
target_compile_definitions(UnitTests PRIVATE GTEST_HAS_PTHREAD=0)

# 设置包含目录
target_include_directories(UnitTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# 设置输出目录
set_target_properties(FinanceSystem UnitTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

enable_testing()
add_test(NAME UnitTests COMMAND UnitTests)