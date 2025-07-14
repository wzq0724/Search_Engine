#pragma once
#include <string>
#include <unordered_map>
#include <stdexcept>

class Configuration {
public:
    // 构造函数（加载配置文件），其中的explicit关键字用来避免隐式转换，防止出现二义性或意外调用
    explicit Configuration(const std::string& filepath);
    
    // 获取配置项
    std::string get(const std::string& key) const;
    
    // 获取配置项（带默认值）
    std::string get(const std::string& key, const std::string& default_val) const;
    
    // 获取整数配置
    int get_int(const std::string& key, int default_val = 0) const;
    
    // 获取布尔配置
    bool get_bool(const std::string& key, bool default_val = false) const;
    
    // 获取所有配置（调试用）
    const std::unordered_map<std::string, std::string>& get_all() const;

private:
    std::unordered_map<std::string, std::string> config_map_;
    
    // 解析配置文件
    void parse_file(const std::string& filepath);
    
    // 移除字符串首尾空格
    static std::string trim(const std::string& str);
};

