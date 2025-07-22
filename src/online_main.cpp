#include "OnlineDataLoader.hpp"
#include <iostream>

int main() {
    OnlineDataLoader loader;
    loader.loadDict("data/dict.dat");
    loader.loadInvertedIndex("data/invertIndex.dat");
    loader.loadWebPages("data/ripepage.dat", "data/offset.dat");

    // 测试输出
    std::cout << "词典词数: " << loader.dict.size() << std::endl;
    std::cout << "倒排索引词数: " << loader.invertedIndex.size() << std::endl;
    std::cout << "文档数: " << loader.docIdToContent.size() << std::endl;

    // 你可以在这里继续实现查询处理和交互
    return 0;
}