#pragma once
#include "OnlineDataLoader.hpp"
#include <vector>
#include <string>
#include <memory>
#include <set>

class QueryProcessor {
public:
    QueryProcessor(const OnlineDataLoader& Loader,std::shared_ptr<SplitTool> splitTool);

    std::vector<int> processQuery(const std::string& query);

    std::string getDocContent(int docId) const;

private:
    const OnlineDataLoader& _loader;//这里在私有成员变量中设置引用的原因时外部传入的OnlineDataLoader对象的引用只在构造函数
    //期间生效，想要使得传入的OnlineDataLoader对象在QueryProcessor类的整个生命周期中都有效，必须设置这个私有成员变量（引用）_loader
    std::shared_ptr<SplitTool> _splitTool;
};