#include "InvertedIndex.hpp"
#include <fstream>//引入C++标准库中的文件流操作
#include <sstream>//引入C++标准库中的字符串流
#include <stdexcept>//引入C++标准库中的异常处理

void InvertedIndex::add(const std::string& word, int docId){
    _index[word].insert(docId);
}

const std::set<int>& InvertedIndex::query(const std::string& word) const {
    static const std::set<int> empty;
    auto it =_index.find(word);
    return it == _index.end() ? empty : it->second;
}

void InvertedIndex::store(const std::string& filepath) const {
    std::ofstream ofs(filepath);
    if(!ofs) throw std::runtime_error("Cannot open file for writing" + filepath);
    for(const auto& [word,docSet]:_index){
        ofs<<word;
        for(int docId: docSet){
            ofs<<" "<<docId;
        }
        ofs<<"\n";
    }

}


void InvertedIndex::load(const std::string& filepath){
    _index.clear();
    std::ifstream ifs(filepath);
    if (!ifs) throw std::runtime_error("Cannot open file for reading: " + filepath);
    std::string line;
    while (std::getline(ifs, line)) {
        std::istringstream iss(line);
        std::string word;
        iss >> word;
        int docId;
        while (iss >> docId) {
            _index[word].insert(docId);
        }
    }
}

std::vector<std::string> InvertedIndex::getAllWords() const {
    std::vector<std::string> words;
    for (const auto& [word, _] : _index) {
        words.push_back(word);
    }
    return words;
}



