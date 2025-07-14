#pragma once
#include "Configuration.hpp"
#include "DirScanner.hpp"
#include "SplitTool.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

class DictProducer {
public:
    // 构造函数，需要配置对象和分词工具
    DictProducer(const Configuration& config, std::shared_ptr<SplitTool> splitTool);
    
    // 构建词典
    void buildDict();
    
    // 存储词典到文件
    void storeDict();
    
    // 获取词典信息（用于调试）
    const std::unordered_map<std::string, int>& getDict() const { return dict_; }
    
    // 获取词典大小
    size_t getDictSize() const { return dict_.size(); }

private:
    // 处理单个文件
    void processFile(const std::string& filepath);
    
    // 处理文本内容
    void processText(const std::string& text);
    
    // 加载停用词表
    void loadStopWords();
    
    // 过滤停用词
    bool isStopWord(const std::string& word) const;
    
    // 检查是否为有效词汇
    bool isValidWord(const std::string& word) const;

private:
    const Configuration& config_;
    std::shared_ptr<SplitTool> splitTool_;
    DirScanner dirScanner_;
    
    // 词典：词 -> 词频
    std::unordered_map<std::string, int> dict_;
    
    // 停用词表
    std::unordered_set<std::string> stopWords_;
    
    // 文件路径
    std::string corpusPath_;
    std::string dictOutputPath_;
    std::string dictIndexPath_;
    std::string stopWordsPath_;
};

