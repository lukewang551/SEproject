# 编译器设置
CXX = g++
CXXFLAGS = -Wall -g -Iinclude
TARGET = my_program.exe
BUILD_DIR = build
SRC_DIR = .

# 自动查找所有 .cpp 文件
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# 默认目标
all: $(BUILD_DIR)/$(TARGET)

# 链接目标文件生成可执行文件
$(BUILD_DIR)/$(TARGET): $(OBJS)
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(OBJS) -o $@

# 编译每个 .cpp 文件为 .o 文件
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理构建文件
clean:
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)

# 运行程序
run: all
	$(BUILD_DIR)/$(TARGET)

# 伪目标
.PHONY: all clean run