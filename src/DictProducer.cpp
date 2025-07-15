#include "DictProducer.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <cctype>
#include <unordered_set>

DictProducer::DictProducer(const Configuration& config, std::shared_ptr<SplitTool> splitTool)
    : _config(config), _splitTool(splitTool) {
    
    // 从配置文件获取路径
    _corpusPath = _config.get("Paths.corpus_path");
    _dictOutputPath = _config.get("Paths.dict_output");
    _dictIndexPath = _config.get("Paths.dict_index");
    _stopWordsPath = _config.get("Paths.stop_words");
    
    // 加载停用词表
    loadStopWords();
}

void DictProducer::buildDict() {
    std::cout << "开始构建词典..." << std::endl;
    std::cout << "语料库路径: " << _corpusPath << std::endl;
    
    // 扫描语料库目录
    _dirScanner(_corpusPath);
    std::vector<std::string> files = _dirScanner.files();
    
    std::cout << "找到 " << files.size() << " 个文件" << std::endl;
    
    // 处理每个文件
    for (const auto& filepath : files) {
        std::cout << "处理文件: " << filepath << std::endl;
        processFile(filepath);
    }
    
    std::cout << "词典构建完成，共 " << _dict.size() << " 个词汇" << std::endl;
}

void DictProducer::processFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filepath << std::endl;
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // 去除回车符
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        
        if (!line.empty()) {
            processText(line);
        }
    }
}

void DictProducer::processText(const std::string& text) {
    // 使用分词工具进行分词
    std::vector<std::string> words = _splitTool->cut(text);
    
    // 统计词频
    for (const auto& word : words) {
        if (isValidWord(word) && !isStopWord(word)) {
            _dict[word]++;
        }
    }
}

void DictProducer::loadStopWords() {
    std::ifstream file(_stopWordsPath);
    if (!file.is_open()) {
        std::cerr << "Warning: 无法打开停用词文件: " << _stopWordsPath << std::endl;
        return;
    }
    
    std::string word;
    while (std::getline(file, word)) {
        // 去除首尾空格和回车符
        word.erase(0, word.find_first_not_of(" \t\r\n"));
        word.erase(word.find_last_not_of(" \t\r\n") + 1);
        
        if (!word.empty()) {
            _stopWords.insert(word);
        }
    }
    
    std::cout << "加载了 " << _stopWords.size() << " 个停用词" << std::endl;
}

bool DictProducer::isStopWord(const std::string& word) const {
    return _stopWords.find(word) != _stopWords.end();
}

bool DictProducer::isValidWord(const std::string& word) const {
    if (word.empty() || word.length() < 2) {
        return false;
    }
    
    // 检查是否包含有效字符（至少包含一个中文字符或字母）
    bool hasValidChar = false;
    for (char c : word) {
        if (std::isalpha(c) || (static_cast<unsigned char>(c) > 127)) {
            hasValidChar = true;
            break;
        }
    }
    
    return hasValidChar;
}

void DictProducer::storeDict() {
    std::cout << "开始存储词典..." << std::endl;
    
    // 将词典转换为vector并排序
    std::vector<std::pair<std::string, int>> sortedDict;
    sortedDict.reserve(_dict.size());
    
    for (const auto& pair : _dict) {
        sortedDict.push_back(pair);
    }
    
    // 按词频降序排序
    std::sort(sortedDict.begin(), sortedDict.end(), 
              [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                  return a.second > b.second;
              });
    
    // 存储词典文件
    std::ofstream dictFile(_dictOutputPath);
    if (!dictFile.is_open()) {
        throw std::runtime_error("无法创建词典文件: " + _dictOutputPath);
    }
    
    // 存储词典索引文件
    std::ofstream indexFile(_dictIndexPath);
    if (!indexFile.is_open()) {
        throw std::runtime_error("无法创建词典索引文件: " + _dictIndexPath);
    }
    
    // 写入词典文件
    for (const auto& pair : sortedDict) {
        dictFile << pair.first << " " << pair.second << std::endl;
    }
    
    // 写入索引文件（词 -> 偏移量）
    std::streampos offset = 0;
    dictFile.clear();
    dictFile.seekp(0, std::ios::beg);
    for (const auto& pair : sortedDict) {
        indexFile << pair.first << " " << offset << std::endl;
        offset = dictFile.tellp();
    }
    
    std::cout << "词典存储完成" << std::endl;
    std::cout << "词典文件: " << _dictOutputPath << std::endl;
    std::cout << "索引文件: " << _dictIndexPath << std::endl;
}

