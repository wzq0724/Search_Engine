#pragma once
#include "SplitTool.hpp"
#include <string>
#include <vector>
#include <unordered_set>

// 前向声明
namespace cppjieba {
    class Jieba;
}

class SplitToolCppJieba : public SplitTool {
public:
    // 构造函数，需要词典路径
    SplitToolCppJieba(const std::string& dictPath, 
                     const std::string& hmmPath,
                     const std::string& userDictPath = "",
                     const std::string& stopWordsPath = "");
    
    ~SplitToolCppJieba();
    
    // 实现基类接口
    std::vector<std::string> cut(const std::string& sentence) override;
    void setStopWords(const std::vector<std::string>& stopWords) override;
    
    // 加载停用词表
    void loadStopWords(const std::string& stopWordsPath);
    
    // 过滤停用词
    std::vector<std::string> filterStopWords(const std::vector<std::string>& words);

private:
    cppjieba::Jieba* jieba_;
    std::unordered_set<std::string> stopWords_;
}; 