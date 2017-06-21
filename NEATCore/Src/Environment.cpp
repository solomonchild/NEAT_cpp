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

std::vector<std::string> Environment::get_parameters()
{
    return parameters_;
}
const std::string Environment::LOGGING_DESTINATION = "logging_destination";
const std::string Environment::POPULATION_SIZE = "population_size";
const std::string Environment::GENOME_SIZE = "genome_size";
const std::string Environment::DISJOINT_COEFF = "disjoint_coeff";
const std::string Environment::EXCESS_COEFF = "excess_coeff";
const std::string Environment::WEIGHTS_COEFF = "weights_coeff";
const std::string Environment::DISTANCE_THRESHOLD = "distance_threshold";

const std::string Environment::P_LINK_MUTATION = "link_mutation_chance";
const std::string Environment::P_NODE_MUTATION = "node_mutation_chance";
const std::string Environment::P_WEIGHT_MUTATION = "weight_mutation_chance";
const std::string Environment::P_ENABLE_DISABLE_MUTATION = "enable_disable_mutation_chance";

std::string Environment::get_typename_for(const std::string& tag)
{
    if(types_.find(tag) == types_.end())
    {
        return "null";
    }
    return types_[tag];
}
std::map<std::string, std::string> Environment::types_ = {

    {Environment::LOGGING_DESTINATION, typeid(std::string).name()},
    {Environment::POPULATION_SIZE, typeid(int).name()},
    {Environment::GENOME_SIZE, typeid(int).name()},
    {Environment::DISJOINT_COEFF, typeid(float).name()},
    {Environment::EXCESS_COEFF, typeid(float).name()},
    {Environment::WEIGHTS_COEFF, typeid(float).name()},
    {Environment::DISTANCE_THRESHOLD, typeid(float).name()},
    {Environment::P_LINK_MUTATION, typeid(float).name()},
    {Environment::P_NODE_MUTATION, typeid(float).name()},
    {Environment::P_WEIGHT_MUTATION, typeid(float).name()},
    {Environment::P_ENABLE_DISABLE_MUTATION, typeid(float).name()}
};

std::vector<std::string> Environment::parameters_ = {
    Environment::LOGGING_DESTINATION,
    Environment::POPULATION_SIZE,
    Environment::GENOME_SIZE,
    Environment::DISJOINT_COEFF,
    Environment::EXCESS_COEFF,
    Environment::WEIGHTS_COEFF,
    Environment::DISTANCE_THRESHOLD,
    Environment::P_LINK_MUTATION,
    Environment::P_NODE_MUTATION,
    Environment::P_WEIGHT_MUTATION,
    Environment::P_ENABLE_DISABLE_MUTATION,
};
