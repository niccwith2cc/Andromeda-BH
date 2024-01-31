#ifndef CONFIG_PARSER
#define CONFIG_PARSER

#include<string>
#include <map>

class ConfigParser{
    public:
        std::map<std::string, std::string> config;
        ConfigParser(std::string filename);
};

#endif