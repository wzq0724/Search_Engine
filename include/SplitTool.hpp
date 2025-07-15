#pragma once
#include <string>
#include <vector>

class SplitTool {
public:
    SplitTool() = default;
    virtual ~SplitTool() = default;

    virtual std::vector<std::string> cut(const std::string& text)=0;

   
};
