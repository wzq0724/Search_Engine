# 搜索引擎项目 Makefile
CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -g
INCLUDES = -I./include -I./third_party/cppjieba/include -I./third_party/cppjieba/deps/limonp/include
LIBS = -lpthread

# 源文件目录
SRC_DIR = src
BIN_DIR = bin
INCLUDE_DIR = include

# 源文件
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BIN_DIR)/%.o)

# 测试文件
TEST_SOURCES = $(wildcard $(SRC_DIR)/test_*.cpp)
TEST_BINS = $(TEST_SOURCES:$(SRC_DIR)/%.cpp=$(BIN_DIR)/%)

# 主要目标
all: $(BIN_DIR) $(TEST_BINS)

# 创建bin目录
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# 编译目标文件
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# 编译测试程序
$(BIN_DIR)/test_%: $(BIN_DIR)/test_%.o $(filter-out $(BIN_DIR)/test_%.o, $(OBJECTS))
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LIBS)

# 清理
clean:
	rm -rf $(BIN_DIR)

# 测试目标
test: all
	@echo "=== 测试目录扫描器 ==="
	./$(BIN_DIR)/test_dir_scanner
	@echo ""
	@echo "=== 测试词典生成器 ==="
	./$(BIN_DIR)/test_dict_producer

# 单独测试目标
test-dir-scanner: $(BIN_DIR)/test_dir_scanner
	./$(BIN_DIR)/test_dir_scanner

test-dict-producer: $(BIN_DIR)/test_dict_producer
	./$(BIN_DIR)/test_dict_producer

# 显示帮助
help:
	@echo "可用目标:"
	@echo "  all              - 编译所有模块和测试程序"
	@echo "  test             - 运行所有测试"
	@echo "  test-dir-scanner - 只测试目录扫描器"
	@echo "  test-dict-producer - 只测试词典生成器"
	@echo "  clean            - 清理编译文件"
	@echo "  help             - 显示此帮助信息"

.PHONY: all clean test test-dir-scanner test-dict-producer help 