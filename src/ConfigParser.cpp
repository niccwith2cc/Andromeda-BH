#include<iostream>
#include <fstream>
#include <algorithm>
#include"ConfigParser.h"

ConfigParser::ConfigParser(std::string filename){
    std::ifstream file (filename);
    if (file.is_open())
    {   
        std::string line;
        while(getline(file, line)){
            line.erase(std::remove_if(line.begin(), line.end(), isspace),
                                 line.end());
            auto pos = line.find("=");
            config[line.substr(0, pos)] = line.substr(pos + 1);
        }
    }
    else std::cout << "Unable to open file";
}