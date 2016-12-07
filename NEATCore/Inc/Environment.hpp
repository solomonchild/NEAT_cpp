#pragma once

#include "Evaluator.hpp"


class Environment
{
public:

    enum class LogLevel
    {
        Debug,
        Info,
        Warning,
        Error,
        Critical,
        None
    };

   static void inc_innovation_number();
   static unsigned get_innovation_number();
   static LogLevel get_log_level();
   static void set_log_level(LogLevel);

  private:
   static unsigned innovation_number_;
   static LogLevel log_level_;
};
