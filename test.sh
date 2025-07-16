#!/bin/bash

echo "=== 搜索引擎项目测试脚本 ==="
echo ""

# 检查必要的目录和文件
echo "1. 检查项目结构..."
if [ ! -d "third_party/cppjieba" ]; then
    echo "错误: cppjieba 目录不存在"
    exit 1
fi

if [ ! -d "resources/corpus" ]; then
    echo "错误: 语料库目录不存在"
    exit 1
fi

if [ ! -f "conf/myconf.conf" ]; then
    echo "错误: 配置文件不存在"
    exit 1
fi

echo "✓ 项目结构检查通过"
echo ""

# 编译项目
echo "2. 编译项目..."
make clean
make all

if [ $? -ne 0 ]; then
    echo "错误: 编译失败"
    exit 1
fi

echo "✓ 编译成功"
echo ""

# 测试目录扫描器
echo "3. 测试目录扫描器..."
./bin/test_dir_scanner

if [ $? -ne 0 ]; then
    echo "错误: 目录扫描器测试失败"
    exit 1
fi

echo "✓ 目录扫描器测试通过"
echo ""

# 测试词典生成器
echo "4. 测试词典生成器..."
./bin/test_dict_producer

if [ $? -ne 0 ]; then
    echo "错误: 词典生成器测试失败"
    exit 1
fi

echo "✓ 词典生成器测试通过"
echo ""

# 检查生成的文件
echo "5. 检查生成的文件..."
if [ -f "data/dict.dat" ]; then
    echo "✓ 词典文件已生成"
    echo "  文件大小: $(ls -lh data/dict.dat | awk '{print $5}')"
else
    echo "警告: 词典文件未生成"
fi

if [ -f "data/dictIndex.dat" ]; then
    echo "✓ 词典索引文件已生成"
    echo "  文件大小: $(ls -lh data/dictIndex.dat | awk '{print $5}')"
else
    echo "警告: 词典索引文件未生成"
fi

echo ""
echo "=== 测试完成 ===" 