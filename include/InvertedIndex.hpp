#pragma once
#include <string>
#include <unordered_map>
#include <set>
#include <vector>

class InvertedIndex {
public:
    //添加分词以及相应的文档记录编号
    void add(const std::string& word, int docId);

    //查询某个词出现在了哪个文档
    const std::set<int>& query(const std::string& word) const;//std::set是C++标准库中的一个关联容器，用来存储唯一且有序的元素集合
    //而在这里set<int>表示该关联容器存储的是int型的文档记录编号

    //将倒排索引写入文件
    void store(const std::string& filepath) const;//store成员函数后加上了const，意味着这是一个只读函数，它将不能修改类的任何成员变量

    //从文件中加载倒排索引
    void load(const std::string& filepath);

    //获取所有词
    std::vector<std::string> getAllWords() const;

private:
    std::unordered_map<std::string,std::set<int>>_index;
};