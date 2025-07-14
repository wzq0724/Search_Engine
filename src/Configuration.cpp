#include "Configuration.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

Configuration::Configuration(const std::string& filepath){
    parse_file(filepath);
}
void Configuration::parse_file(const std::string& filepath){
    std::ifstream file(filepath);
    if(!file.is_open()){
        throw std::runtime_error("无法打开配置文件：" + filepath);
    }
    std::string line;
    std::string current_section;

    while (std::getline(file,line)){
        if(!line.empty() && line.back() == '\r') {
            line.pop_back();//移除最后一个字符，结合上述判断条件，即移除回车符
        }
        //判断是否是空行或者是注释行
        if(line.empty() || line[0] == '#' || line[0] == ';'  || (line.size()>=2 && line[0] == '/' && line[1] =='/')) 
        {
            continue;
        }
        //提取段标题，存储到current_section变量中
        if(line.front() == '[' &&line.back() == ']') {
            current_section = line.substr(1,line.size() - 2) + ".";
            continue;
        }

        size_t delimiter_pos = line.find('=');
        if(delimiter_pos != std::string::npos){
            std::string key = trim(line.substr(0,delimiter_pos));
            std::string value = trim(line.substr(delimiter_pos + 1));
       

            //添加段前缀
            if(!current_section.empty()) {
                key = current_section + key;
            }

            config_map_[key] = value;
        }
    }
}


std::string Configuration::get(const std::string& key) const {
    auto it = config_map_.find(key);
    if(it == config_map_.end()){
        throw std::out_of_range("配置项不存在：" + key);
    }
    return it->second;
}

std::string Configuration::get(const std::string& key,const std::string& default_val)const {
    auto it = config_map_.find(key);
    return (it!=config_map_.end()) ? it->second : default_val;
}

int Configuration::get_int(const std::string& key, int default_val) const {
    try {
        return std::stoi(get(key));
    } catch(...) {
        return default_val;
    }
}


bool Configuration::get_bool(const std::string& key, bool default_val) const {
    std::string val = get(key,"");
    std::transform(val.begin(),val.end(),val.begin(),::tolower);

    if (val == "true" || val =="yes" || val == "1") return true;
    if (val == "false" || val =="no" || val =="0") return false;
    return default_val;

}


const std::unordered_map<std::string,std::string>& Configuration::get_all() const {
    return config_map_;
}

std::string Configuration::trim(const std::string& str) {
    auto start = str.begin();
    while(start != str.end() && std::isspace(static_cast<unsigned char>(*start))){
        start++;
    }

    auto end = str.end();
    do {
        end--;
    } while (std::distance(start,end) >0 && std::isspace(static_cast<unsigned char>(*end)));

    return std::string(start,end+1);
}