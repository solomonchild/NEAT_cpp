#pragma once

#include <string>
#include <map>
#include <vector>

class ConfigParser
{
public:
    ConfigParser(const std::string&);
    std::map<std::string, std::string> parse(const std::vector<std::string>& params);
private:
    std::string name_;
};
