#pragma once
#include "Configuration.hpp"
#include "DirScanner.hpp"
#include "SplitTool.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
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
    const std::unordered_map<std::string, int>& getDict() const { return _dict; }

    // 获取词典大小
    size_t getDictSize() const { return _dict.size(); }

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
    const Configuration& _config;
    std::shared_ptr<SplitTool> _splitTool;
    DirScanner _dirScanner;

    // 词典：词 -> 词频
    std::unordered_map<std::string, int> _dict;

    // 停用词表
    std::unordered_set<std::string> _stopWords;

    // 文件路径
    std::string _corpusPath;
    std::string _dictOutputPath;
    std::string _dictIndexPath;
    std::string _stopWordsPath;
};

