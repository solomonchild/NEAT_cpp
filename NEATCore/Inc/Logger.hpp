#pragma once

#include <cstdio>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <future>
#include <fstream>

#include "Environment.hpp"

#define ADEBUG(x, ...) std::async(std::launch::async, [=](){Logger::get_instance()::log(Environment::LogLevel::Debug, x, ##__VA_ARGS__);})
#define AINFO(x, ...) std::async(std::launch::async, [=](){Logger::get_instance().log(Environment::LogLevel::Info, x, ##__VA_ARGS__);})
#define AWARN(x, ...) std::async(std::launch::async, [=](){Logger::get_instance().log(Environment::LogLevel::Warning, x, ##__VA_ARGS__);})
#define AERROR(x, ...) std::async(std::launch::async, [=](){Logger::get_instance().log(Environment::LogLevel::Error, x, ##__VA_ARGS__);})
#define ACRIT(x, ...) std::async(std::launch::async, [=](){Logger::get_instance().log(Environment::LogLevel::Critical, x, ##__VA_ARGS__);})

#define DEBUG(x, ...) Logger::get_instance().log(Environment::LogLevel::Debug, x, ##__VA_ARGS__);
#define INFO(x, ...) Logger::get_instance().log(Environment::LogLevel::Info, x, ##__VA_ARGS__)
#define WARN(x, ...) Logger::get_instance().log(Environment::LogLevel::Warning, x, ##__VA_ARGS__)
#define ERROR(x, ...) Logger::get_instance().log(Environment::LogLevel::Error, x, ##__VA_ARGS__)
#define CRIT(x, ...) Logger::get_instance().log(Environment::LogLevel::Critical, x, ##__VA_ARGS__)

template <typename T> void IF_DEBUG(T f) { if(Environment::get_log_level() <= Environment::LogLevel::Debug) { f(); } }

template <typename T> void IF_INFO(T f) { if(Environment::get_log_level() <= Environment::LogLevel::Info) { f(); } }

template <typename T> void IF_WARN(T f) { if(Environment::get_log_level() <= Environment::LogLevel::Warning) { f(); } }

template <typename T> void IF_ERROR(T f) { if(Environment::get_log_level() <= Environment::LogLevel::Error) { f(); } }

template <typename T> void IF_CRIT(T f) { if(Environment::get_log_level() <= Environment::LogLevel::Critical) { f(); } }

#define INFO_STREAM(t) Logger::get_instance().put_stream(Environment::LogLevel::Info, t)

class Logger
{
public:
    static inline void trunc()
    {
            std::ofstream file = std::ofstream(get_instance().filename_, std::ofstream::trunc);
            file.close();
    }

    static Logger& get_instance()
    {
        static Logger logger;
        return logger;
    }
    static Logger& set_filename(const std::string& name)
    {
        get_instance().filename_ = name;
        return get_instance();
    }

    Logger for_file(const std::string& name)
    {
       Logger logger;
       logger.set_filename(name);
       return logger;
    }

    template<typename T>
    void put_stream(Environment::LogLevel level, T param)
    {
        if(Environment::get_log_dest() == Environment::LogDestination::File)
        {
            auto file = std::ofstream("out.txt", std::ofstream::binary | std::ofstream::app);
            file << param << "\n";
            file.close();
        }
        if(Environment::get_log_dest() == Environment::LogDestination::Console)
        {
            std::cout << param << '\n';
        }
    }

    template<typename T>
    void dump(const std::string& name, const T& param)
    {
        auto file = std::ofstream(name, std::ofstream::binary | std::ofstream::trunc);
        file << param;
        file.close();
    }

    template<typename... Params>
    void log(Environment::LogLevel level, const std::string& format, Params... params)
    {
        if(Environment::get_log_level() > level || Environment::get_log_level() == Environment::LogLevel::None)
        {
            return;
        }
        auto level_to_string = [] (Environment::LogLevel level)
        {
            switch(level)
            {
            case Environment::LogLevel::Debug:
                return "DEBUG";
            case Environment::LogLevel::Info:
                return "INFO";
            case Environment::LogLevel::Warning:
                return "WARN";
            case Environment::LogLevel::Error:
                return "ERROR";
            case Environment::LogLevel::Critical:
                return "CRIT";
            default:
                return "Unknown";
            }
        };

        auto print = [] (const std::string& format, auto... params)
        {
            std::string str;
            auto len = snprintf(nullptr, 0, format.c_str(), params...) + 1;
            auto buf = std::make_unique<char[]>(len);
            snprintf(buf.get(), len, format.c_str(), params...);
            str += buf.get();
            return str;
        };

        auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        auto t = std::localtime(&time);
        std::string str = print("[%d:%d:%d] %s: ", t->tm_hour, t->tm_min, t->tm_sec, level_to_string(level));
        str += print(format, params...);
        if(Environment::get_log_dest() == Environment::LogDestination::File)
        {
            auto file = std::ofstream("out.txt", std::ofstream::binary | std::ofstream::app);
            file << str << "\n";
            file.close();
        }
        if(Environment::get_log_dest() == Environment::LogDestination::Console)
        {
            std::cout << str << '\n';
        }
    }
private:
    std::string filename_;
    Logger() {}

};
