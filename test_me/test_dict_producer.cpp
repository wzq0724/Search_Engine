#include "Configuration.hpp"
#include "DictProducer.hpp"
#include "SplitToolCppJieba.hpp"
#include <iostream>
#include <memory>

int main() {
    try {
        // 1. 读取配置文件
        Configuration config("conf/myconf.conf");

        // 2. 初始化分词工具（根据配置文件路径）
        std::string jieba_dict = config.get("Segmentation.jieba_dict");
        std::string jieba_hmm = config.get("Segmentation.jieba_hmm");
        std::string jieba_user = ""; // 可选：用户词典路径
        std::string stop_words = config.get("Paths.stop_words");

        std::shared_ptr<SplitTool> splitter = std::make_shared<SplitToolCppJieba>(
            jieba_dict, jieba_hmm, jieba_user, stop_words
        );

        // 3. 创建 DictProducer 对象
        DictProducer dictProducer(config, splitter);

        // 4. 构建词典
        dictProducer.buildDict();

        // 5. 存储词典到文件
        dictProducer.storeDict();

        std::cout << "测试完成！" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "发生异常: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}