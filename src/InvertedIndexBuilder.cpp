#include "InvertedIndexBuilder.hpp"
#include "DirScanner.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>

InvertedIndexBuilder::InvertedIndexBuilder(const Configuration& config, std::shared_ptr<SplitTool> splitTool)
    : _config(config), _splitTool(splitTool) {
    
    _corpusPath = _config.get("Paths.corpus_path");
    loadStopWords();
}

void InvertedIndexBuilder::buildInvertedIndex(InvertedIndex& invertedIndex) {
    std::cout << "开始构建倒排索引..." << std::endl;
    std::cout << "语料库路径: " << _corpusPath << std::endl;
    
    // 扫描语料库目录
    DirScanner dirScanner;
    dirScanner(_corpusPath);
    std::vector<std::string> files = dirScanner.files();
    
    std::cout << "找到 " << files.size() << " 个文件" << std::endl;
    
    // 处理每个文档
    int docId = 1;
    for (const auto& filepath : files) {
        std::cout << "处理文档 " << docId << ": " << filepath << std::endl;
        processDocument(filepath, docId, invertedIndex);
        docId++;
    }
    
    std::cout << "倒排索引构建完成" << std::endl;
}

void InvertedIndexBuilder::processDocument(const std::string& filepath, int docId, InvertedIndex& invertedIndex) {
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
            // 使用分词工具进行分词
            std::vector<std::string> words = _splitTool->cut(line);
            
            // 统计词频并添加到倒排索引
            for (const auto& word : words) {
                if (isValidWord(word) && !isStopWord(word)) {
                    invertedIndex.add(word, docId);
                }
            }
        }
    }
}

void InvertedIndexBuilder::loadStopWords() {
    std::string stopWordsPath = _config.get("Paths.stop_words");
    std::ifstream file(stopWordsPath);
    if (!file.is_open()) {
        std::cerr << "Warning: 无法打开停用词文件: " << stopWordsPath << std::endl;
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

bool InvertedIndexBuilder::isStopWord(const std::string& word) const {
    return _stopWords.find(word) != _stopWords.end();
}

bool InvertedIndexBuilder::isValidWord(const std::string& word) const {
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