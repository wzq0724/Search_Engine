#pragma once
#include <vector>
#include <string>
#include <set>

class DirScanner {
public:
    // 默认构造函数，初始化默认支持的文件扩展名
    DirScanner();
    
    // 带参数的构造函数，可以自定义支持的文件扩展名
    explicit DirScanner(const std::set<std::string>& extensions);
    
    // 析构函数
    ~DirScanner() = default;

    void operator()(const std::string& dirname);

    std::vector<std::string> files() const { return _files; }

    std::vector<std::string>& getFiles(){return _files; }

    // 清空文件列表
    void clear() { _files.clear(); }

    void setExtensions(const std::set<std::string>& extensions);

    void addExtension(const std::string& extension);

    const std::set<std::string>& getExtensions() const { return _supportedExtensions; }

    size_t size() const { return _files.size(); }

    bool empty() const { return _files.empty(); }

private:
    // 递归遍历目录的核心函数
    void traverse(const std::string& dirname);

    // 检查文件扩展名是否支持
    bool isSupportedFile(const std::string& filename) const;
    
    // 构建完整的文件路径
    std::string buildFullPath(const std::string& dirname, const std::string& filename) const;

private:
    std::vector<std::string> _files;                    // 存储扫描到的文件路径
    std::set<std::string> _supportedExtensions;         // 支持的文件扩展名
    bool _recursive;                                     // 是否递归扫描子目录
};