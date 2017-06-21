#include "ConfigParser.hpp"
#include "Logger.hpp"

#include <exception>
#include <fstream>
#include <iostream>
#include <algorithm>

ConfigParser::ConfigParser(const std::string& name)
    :name_(name)
{ }

std::string truncate(const std::string& str)
{
    unsigned int first = 0;
    while(str[first] == ' ' || str[first] == '\n')
    {
        first ++;
    }
    unsigned int len = str.size() - first;
    while(str[first + len - 1] == ' ' || str[first + len - 1] == '\n')
    {
        len --;
    }
    return str.substr(first, len);
}

std::map<std::string, std::string> ConfigParser::parse(const std::vector<std::string>& existing_params)
{
    auto file = std::ifstream(name_, std::ifstream::binary);
    if(!file.is_open())
    {
        throw new std::runtime_error("Cannot open " + name_);
    }

    std::string label;
    std::string value;
    std::map<std::string, std::string> map;
    INFO("Will list read parameters from %s", name_.c_str());
    while(getline(file, label, '=') && getline(file, value, '\r'))
    {
        label = truncate(label);
        value = truncate(value);
        if(std::find(existing_params.begin(), existing_params.end(), label) != existing_params.end())
        {
            map[label] = value;
            INFO("%s, %s", label.c_str(), value.c_str());
        }
    }
    return map;
}
