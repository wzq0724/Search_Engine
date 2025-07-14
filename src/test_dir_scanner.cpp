#include "DirScanner.hpp"
#include <iostream>
#include <set>

int main() {
    std::cout << "=== DirScanner 测试程序 ===" << std::endl;
    
    try {
        // 测试1: 使用默认构造函数
        std::cout << "\n1. 测试默认构造函数:" << std::endl;
        DirScanner scanner1;
        scanner1("./resources");  // 扫描resources目录
        
        std::cout << "找到 " << scanner1.size() << " 个文件:" << std::endl;
        for (const auto& file : scanner1.files()) {
            std::cout << "  " << file << std::endl;
        }
        
        // 测试2: 自定义文件扩展名
        std::cout << "\n2. 测试自定义文件扩展名:" << std::endl;
        std::set<std::string> customExts = {".txt", ".md"};
        DirScanner scanner2(customExts);
        scanner2("./resources");
        
        std::cout << "支持的文件扩展名: ";
        for (const auto& ext : scanner2.getExtensions()) {
            std::cout << ext << " ";
        }
        std::cout << std::endl;
        
        std::cout << "找到 " << scanner2.size() << " 个文件:" << std::endl;
        for (const auto& file : scanner2.files()) {
            std::cout << "  " << file << std::endl;
        }
        
        // 测试3: 动态添加文件扩展名
        std::cout << "\n3. 测试动态添加文件扩展名:" << std::endl;
        DirScanner scanner3;
        scanner3.addExtension(".conf");
        scanner3("./conf");
        
        std::cout << "支持的文件扩展名: ";
        for (const auto& ext : scanner3.getExtensions()) {
            std::cout << ext << " ";
        }
        std::cout << std::endl;
        
        std::cout << "找到 " << scanner3.size() << " 个文件:" << std::endl;
        for (const auto& file : scanner3.files()) {
            std::cout << "  " << file << std::endl;
        }
        
        // 测试4: 清空和重用
        std::cout << "\n4. 测试清空和重用:" << std::endl;
        DirScanner scanner4;
        scanner4("./src");
        std::cout << "第一次扫描找到 " << scanner4.size() << " 个文件" << std::endl;
        
        scanner4.clear();
        std::cout << "清空后文件数量: " << scanner4.size() << std::endl;
        
        scanner4("./include");
        std::cout << "重新扫描后找到 " << scanner4.size() << " 个文件" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "\n=== 测试完成 ===" << std::endl;
    return 0;
} 