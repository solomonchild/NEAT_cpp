#include "Environment.hpp"

unsigned Environment::innovation_number_ = 1;
Environment::LogLevel Environment::log_level_;
Environment::LogDestination Environment::log_destination_;

void Environment::inc_innovation_number()
{
   innovation_number_++;
}

unsigned Environment::get_innovation_number()
{
    return innovation_number_;
}

Environment::LogLevel Environment::get_log_level()
{
    return log_level_;
}

void Environment::set_log_level(LogLevel level)
{
    log_level_ = level;
}

void Environment::set_log_dest(LogDestination dest)
{
    log_destination_ = dest;
}

Environment::LogDestination Environment::get_log_dest()
{
    return log_destination_;
}

