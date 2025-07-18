#pragma once
#include "Configuration.hpp"
#include "DictProducer.hpp"
#include "InvertedIndex.hpp"
#include "SplitTool.hpp"
#include <memory>
#include <vector>
#include <string>

class InvertedIndexBuilder {
public:
    InvertedIndexBuilder(const Configuration& config, std::shared_ptr<SplitTool> splitTool);
    
    // 构建倒排索引
    void buildInvertedIndex(InvertedIndex& invertedIndex);
    
    // 处理单个文档
    void processDocument(const std::string& filepath, int docId, InvertedIndex& invertedIndex);
    
private:
    const Configuration& _config;
    std::shared_ptr<SplitTool> _splitTool;
    std::string _corpusPath;
    std::unordered_set<std::string> _stopWords;
    
    // 加载停用词
    void loadStopWords();
    
    // 检查是否为停用词
    bool isStopWord(const std::string& word) const;
    
    // 检查是否为有效词汇
    bool isValidWord(const std::string& word) const;
}; 