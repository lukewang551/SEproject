set(CMAKE_BUILD_TYPE Debug)

cmake_minimum_required(VERSION 3.10)
project(MyProject LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 包含头文件目录
include_directories(include)

# 找出所有源文件
file(GLOB SOURCES "src/*.cpp")

# 生成可执行文件
add_executable(MyProject ${SOURCES})
