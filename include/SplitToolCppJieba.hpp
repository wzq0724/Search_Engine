#pragma once
#include "SplitTool.hpp"
#include <string>
#include <vector>
#include <memory>

// 前向声明，避免头文件依赖过重
namespace cppjieba {
    class Jieba;
}

// cppjieba分词工具实现
class SplitToolCppJieba : public SplitTool {
public:
    SplitToolCppJieba(const std::string& dictPath,
                      const std::string& hmmPath,
                      const std::string& userDictPath,
                      const std::string& stopWordsPath);

    ~SplitToolCppJieba();

    std::vector<std::string> cut(const std::string& sentence) override;

private:
    std::unique_ptr<cppjieba::Jieba> _jieba;
    std::vector<std::string> _stopWords;
    void loadStopWords(const std::string& stopWordsPath);
    bool isStopWord(const std::string& word) const;
}; 