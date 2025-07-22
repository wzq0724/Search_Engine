#include "QueryProcessor.hpp"
#include <algorithm>
#include <unordered_map>
#include <iostream>

QueryProcessor::QueryProcessor(const OnlineDataLoader& loader,std::shared_ptr<SplitTool> splitTool)
:_loader(loader),_splitTool(splitTool){}

std::vector<int> QueryProcessor::processQuery(const std::string& query){
    //1.分词
    std::vector<std::string> words = _splitTool->cut(query);
    if(words.empty()) return {};
     
    //2.查找用户输入文本的分词结果的倒排索引，收集分词结果所对应的所有文档ID集合
    std::vector<std::set<int>> docSets;
    for(const auto& word : words) {
        auto it = _loader.invertedIndex.find(word);
        if(it!=_loader.invertedIndex.end()){
            docSets.push_back(it->second);
        }
    }
    if(docSets.empty()) return {};

    //3.求交集
    std::set<int> resultSet=docSets[0];
    for(size_t i =1;i<docSets.size();++i) { //size_t代表无符号整型
        std::set<int> temp;
        std::set_intersection(resultSet.begin(),resultSet.end(),
                            docSets[i].begin(),docSets[i].end(),
                            std::inserter(temp,temp.begin()));
        resultSet = std::move(temp);
        if(resultSet.empty()) break;
    }


    //4.简单相关性排序
    std::vector<std::pair<int,int>> docScore;//pair也是用来存储键值对的数据结构，但与
    //std::map以及std::unordered_map相比，它是最基础的键值对结构，其不提供查找、排序、去重
    //等高级功能，常用于临时组合两个值，例如作为函数返回值，或作为vector中的元素
    for(int docId:resultSet) {
        int score =0;
        for(const auto& word : words) {
            auto it = _loader.invertedIndex.find(word);
            if(it != _loader.invertedIndex.end()&& it->second.count(docId))
            {
                ++score;
            }
        }
        docScore.emplace_back(docId,score);
    }
    std::sort(docScore.begin(),docScore.end(),[](const auto& a,const auto& b)
    {
        if(a.second != b.second) return a.second>b.second;
        return a.first<b.first;
    }
);

    //5.返回排序后的文档ID
    std::vector<int>docIds;
    for(const auto& p : docScore) {
        docIds.push_back(p.first);
    }
    return docIds;

}

std::string QueryProcessor::getDocContent(int docId) const {
    auto it = _loader.docIdToContent.find(docId);
    if (it != _loader.docIdToContent.end()) {
        return it->second;
    }
    return "";
}