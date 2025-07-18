#include "Configuration.hpp"
#include "DirScanner.hpp"
#include "DictProducer.hpp"
#include "InvertedIndex.hpp"
#include "InvertedIndexBuilder.hpp"
#include "SplitToolCppJieba.hpp"
#include <iostream>
#include <chrono>//时间库，用于记录程序运行的时间


int main() {
    std::cout << "=== 搜索引擎离线处理开始 ===" << std::endl;
    
    auto start_time = std::chrono::high_resolution_clock::now();//now()是high_resolution_clock的静态成员函数，用于获取当前程序开始运行的时刻
    //并将该值赋值给变量start_time

    try {
        // 1. 加载配置
        Configuration config("conf/myconf.conf");
        std::cout << "配置加载完成" << std::endl;
        
        // 2. 创建分词工具
        auto splitTool = std::make_shared<SplitToolCppJieba>(
            config.get("Segmentation.jieba_dict"),
            config.get("Segmentation.jieba_hmm"),
            config.get("Segmentation.jieba_user_dict"),
            config.get("Segmentation.jieba_idf")
        );
        
        // 3. 生成词典
        DictProducer dictProducer(config, splitTool);
        dictProducer.buildDict();
        dictProducer.storeDict();
        std::cout << "词典生成完成" << std::endl;
        
        // 4. 构建倒排索引
        InvertedIndex invertedIndex;
        InvertedIndexBuilder indexBuilder(config, splitTool);
        indexBuilder.buildInvertedIndex(invertedIndex);
        invertedIndex.store("data/invertIndex.dat");
        std::cout << "倒排索引构建完成" << std::endl;
        
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        
        std::cout << "=== 离线处理完成，耗时: " << duration.count() << "ms ===" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
