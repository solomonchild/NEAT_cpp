#pragma once

#include "Evaluator.hpp"
#include <string>
#include <map>
#include <typeindex>


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
   static std::vector<std::string> get_parameters();
   static std::string get_typename_for(const std::string& tag);

  private:
   static const std::string LOGGING_DESTINATION;
   static const std::string POPULATION_SIZE;
   static const std::string GENOME_SIZE;
   static const std::string DISJOINT_COEFF;
   static const std::string EXCESS_COEFF;
   static const std::string WEIGHTS_COEFF;
   static const std::string DISTANCE_THRESHOLD;
   static const std::string P_LINK_MUTATION;
   static const std::string P_NODE_MUTATION;
   static const std::string P_WEIGHT_MUTATION;
   static const std::string P_ENABLE_DISABLE_MUTATION;
   static std::vector<std::string> parameters_;
   static std::map<std::string, std::string> types_;
   static unsigned innovation_number_;
   static LogLevel log_level_;
   static LogDestination log_destination_;
};
