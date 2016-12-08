#pragma once

#include <cstdio>
#include <iomanip>
#include <chrono>
#include <future>

#include "Environment.hpp"

#define ADEBUG(x, ...) std::async(std::launch::async, [=](){Logging::log(Environment::LogLevel::Debug, x, ##__VA_ARGS__);})
#define AINFO(x, ...) std::async(std::launch::async, [=](){Logging::log(Environment::LogLevel::Info, x, ##__VA_ARGS__);})
#define AWARN(x, ...) std::async(std::launch::async, [=](){Logging::log(Environment::LogLevel::Warning, x, ##__VA_ARGS__);})
#define AERROR(x, ...) std::async(std::launch::async, [=](){Logging::log(Environment::LogLevel::Error, x, ##__VA_ARGS__);})
#define ACRIT(x, ...) std::async(std::launch::async, [=](){Logging::log(Environment::LogLevel::Critical, x, ##__VA_ARGS__);})

#define DEBUG(x, ...) Logging::log(Environment::LogLevel::Debug, x, ##__VA_ARGS__);
#define INFO(x, ...) Logging::log(Environment::LogLevel::Info, x, ##__VA_ARGS__)
#define WARN(x, ...) Logging::log(Environment::LogLevel::Warning, x, ##__VA_ARGS__)
#define ERROR(x, ...) Logging::log(Environment::LogLevel::Error, x, ##__VA_ARGS__)
#define CRIT(x, ...) Logging::log(Environment::LogLevel::Critical, x, ##__VA_ARGS__)

template <typename T> void IF_DEBUG(T f) { if(Environment::get_log_level() <= Environment::LogLevel::Debug) { f(); } }

template <typename T> void IF_INFO(T f) { if(Environment::get_log_level() <= Environment::LogLevel::Info) { f(); } }

template <typename T> void IF_WARN(T f) { if(Environment::get_log_level() <= Environment::LogLevel::Warning) { f(); } }

template <typename T> void IF_ERROR(T f) { if(Environment::get_log_level() <= Environment::LogLevel::Error) { f(); } }

template <typename T> void IF_CRIT(T f) { if(Environment::get_log_level() <= Environment::LogLevel::Critical) { f(); } }

namespace Logging
{

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
        std::cout << str << '\n';
    }
}
