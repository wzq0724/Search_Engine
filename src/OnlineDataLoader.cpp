#include "OnlineDataLoader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

void OnlineDataLoader::loadDict(const std::string& dictPath) {
    std::ifstream ifs(dictPath);
    if (!ifs) {
        std::cerr << "Failed to open dict file: " << dictPath << std::endl;
        return;
    }
    std::string word;
    int freq;
    while (ifs >> word >> freq) {
        dict[word] = freq;
    }
}

void OnlineDataLoader::loadInvertedIndex(const std::string& indexPath) {
    std::ifstream ifs(indexPath);
    if (!ifs) {
        std::cerr << "Failed to open inverted index file: " << indexPath << std::endl;
        return;
    }
    std::string line;
    while (std::getline(ifs, line)) {
        std::istringstream iss(line);
        std::string word;
        int docCount;
        iss >> word >> docCount;
        std::set<int> docSet;
        for (int i = 0; i < docCount; ++i) {
            int docId;
            iss >> docId;
            docSet.insert(docId);
        }
        invertedIndex[word] = docSet;
    }
}

void OnlineDataLoader::loadWebPages(const std::string& pagePath, const std::string& offsetPath) {
    std::ifstream offsetFile(offsetPath);
    std::ifstream pageFile(pagePath, std::ios::binary);
    if (!offsetFile) {
        std::cerr << "Failed to open offset file: " << offsetPath << std::endl;
        return;
    }
    if (!pageFile) {
        std::cerr << "Failed to open page file: " << pagePath << std::endl;
        return;
    }
    int docId, offset, length;
    while (offsetFile >> docId >> offset >> length) {
        pageFile.seekg(offset);
        std::string content(length, '\0');
        pageFile.read(&content[0], length);
        docIdToContent[docId] = content;
    }
}