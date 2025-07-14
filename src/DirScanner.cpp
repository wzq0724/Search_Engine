#include "DirScanner.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>//Linux系统中标准的目录操作接口，包括了DIR和struct dirent等类型
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <algorithm>

using std::string;
using std::vector;
using std::set;

DirScanner::DirScanner():_recursive(true){
    
    _supportedExtensions = {".txt",".md",".html",".htm",".xml",".json",".csv"};
}

DirScanner::DirScanner(const set<string>& extensions):_supportedExtensions(extensions),_recursive(true){}

void DirScanner::operator()(const std::string& dirname){
    clear();
    traverse(dirname);
}

void DirScanner::traverse(const std::string& dirname) {
    //DIR是系统头文件中某个结构体的别名，用于表示目录流（用于遍历），强制由opendir函数返回，
    //而struct dirent没有使用别名，用于代表目录中的一个条目（文件或子目录），但 DIR并不是struct dirent的别名,因为struct dirent 强制由readdir函数返回
    //总结来说，DIR是图书馆通行证，用于打开目录并获得目录流，而readdir用于从目录流中获取目录条目，opendir一定在readdir之前调用
    
    
    DIR* dir = opendir(dirname.c_str());//dirname.c_str()将字符串转换为C风格字符串,因为opendir函数需要C风格字符串
    if(!dir) {
        perror("opendir error");
        throw std::runtime_error("Failed to open directory: "+ dirname);
    }

    struct dirent*entry;//struct dirent 是操作系统提供的标准结构体，用于支持目录遍历操作
    while(entry = readdir(dir)){
        if(strcmp(entry->d_name,".")==0||strcmp(entry->d_name,"..")==0){//strcmp(a,b)==0表示两个字符串内容相同
            continue;
        }

        string fullpath = buildFullPath(dirname,entry->d_name);

        if(entry->d_type == DT_DIR){//DT_DIR表示是一个子目录
            if(_recursive){
                traverse(fullpath);
            }
        }
        else {
            if(isSupportedFile(entry->d_name)){
                _files.push_back(fullpath);
            }
        }
    }

    closedir(dir);
}
    

bool DirScanner::isSupportedFile(const string& filename)const{
    if(_supportedExtensions.empty()){
        return true;
    }

    string lowerFilename = filename;
    std::transform(lowerFilename.begin(),lowerFilename.end(),lowerFilename.begin(),::tolower);


    for(const auto&ext : _supportedExtensions){
        if(lowerFilename.find(ext) != string::npos){
            return true;
        }
    }
    return false;
}

// 构建完整的文件路径
string DirScanner::buildFullPath(const string& dirname, const string& filename) const {
    // 检查目录名是否以斜杠结尾
    if (dirname.back() == '/' || dirname.back() == '\\') {
        return dirname + filename;
    } else {
        return dirname + "/" + filename;
    }
}

// 设置支持的文件扩展名
void DirScanner::setExtensions(const set<string>& extensions) {
    _supportedExtensions = extensions;
}

// 添加支持的文件扩展名
void DirScanner::addExtension(const string& extension) {
    _supportedExtensions.insert(extension);
}
            
    
