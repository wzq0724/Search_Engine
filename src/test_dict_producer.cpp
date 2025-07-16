#include "Configuration.hpp"
#include "DictProducer.hpp"
#include "SplitToolCppJieba.hpp"
#include <iostream>
#include <memory>

int main() {
    try {
        // 创建配置对象
        Configuration config("conf/myconf.conf");
        
        // 创建分词工具
        std::string dictPath = config.get("Segmentation.jieba_dict");
        std::string hmmPath = config.get("Segmentation.jieba_hmm");
        std::string stopWordsPath = config.get("Paths.stop_words");
        
        auto splitTool = std::make_shared<SplitToolCppJieba>(dictPath, hmmPath, "", stopWordsPath);
        
        // 创建词典生产者
        DictProducer producer(config, splitTool);
        
        // 构建词典
        producer.buildDict();
        
        // 存储词典
        producer.storeDict();
        
        std::cout << "词典生产完成！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
} 