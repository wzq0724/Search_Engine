#include "SplitToolCppJieba.hpp"
#include "cppjieba/Jieba.hpp"
#include <fstream>
#include <algorithm>

SplitToolCppJieba::SplitToolCppJieba(const std::string& dictPath,
                                     const std::string& hmmPath,
                                     const std::string& userDictPath,
                                     const std::string& stopWordsPath) {
    _jieba = std::make_unique<cppjieba::Jieba>(
        dictPath, hmmPath, userDictPath
    );
    loadStopWords(stopWordsPath);
}

SplitToolCppJieba::~SplitToolCppJieba() = default;

std::vector<std::string> SplitToolCppJieba::cut(const std::string& sentence) {
    std::vector<std::string> words;
    _jieba->Cut(sentence, words, true); // 使用HMM
    std::vector<std::string> result;
    for (const auto& word : words) {
        if (!isStopWord(word)) {
            result.push_back(word);
        }
    }
    return result;
}

void SplitToolCppJieba::loadStopWords(const std::string& stopWordsPath) {
    std::ifstream file(stopWordsPath);
    std::string word;
    while (std::getline(file, word)) {
        word.erase(0, word.find_first_not_of(" \t\r\n"));
        word.erase(word.find_last_not_of(" \t\r\n") + 1);
        if (!word.empty()) _stopWords.push_back(word);
    }
}

bool SplitToolCppJieba::isStopWord(const std::string& word) const {
    return std::find(_stopWords.begin(), _stopWords.end(), word) != _stopWords.end();
}

