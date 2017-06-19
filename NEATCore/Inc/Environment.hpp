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
    enum class LogDestination
    {
       File,
       Console
    };

   static void inc_innovation_number();
   static unsigned get_innovation_number();
   static LogLevel get_log_level();
   static void set_log_level(LogLevel);
   static void set_log_dest(LogDestination);
   static LogDestination get_log_dest();

  private:
   static unsigned innovation_number_;
   static LogLevel log_level_;
   static LogDestination log_destination_;
};
