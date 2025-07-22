#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>

class OnlineDataLoader {
public:

    void loadDict(const std::string& dictPath);

    void loadInvertedIndex(const std::string& indexPath);

    void loadWebPages(const std::string& pagePath,const std::string& offsetPath);

    std::unordered_map<std::string,int> dict;
    std::unordered_map<std::string,std::set<int>> invertedIndex;
    std::unordered_map<int,std::string>docIdToContent;
};