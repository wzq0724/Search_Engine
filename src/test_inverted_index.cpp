#include "InvertedIndex.hpp"
#include <iostream>

int main() {
    InvertedIndex index;
    index.add("篮球", 1);
    index.add("篮球", 2);
    index.add("足球", 1);
    index.add("篮球", 3);

    index.store("data/invertIndex.dat");

    InvertedIndex loaded;
    loaded.load("data/invertIndex.dat");

    for (const auto& word : loaded.getAllWords()) {
        std::cout << word << ":";
        for (int docId : loaded.query(word)) {
            std::cout << " " << docId;
        }
        std::cout << std::endl;
    }
    return 0;
}
