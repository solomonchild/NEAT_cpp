#include "Parameters.hpp"
#include <typeinfo>

Parameters& Parameters::get_instance()
{
    static Parameters params;
    return params;
}

void Parameters::parse()
{
    ConfigParser parser("config.ini");
    auto map = parser.parse(get_parameters());
    auto contains = [&map](const std::string& what)
    {
        return (map.find(what) != map.end());
    };

    auto set = [contains, &map](const std::string& what, auto& field)
    {
        if(contains(what))
        {
            if(std::is_same<typename std::remove_reference<decltype(field)>::type, int>::value)
            {
               field = std::atoi(map[what].c_str());
            }
            else if(std::is_same<typename std::remove_reference<decltype(field)>::type, float>::value)
            {
               field = std::stof(map[what].c_str());
            }
            else if(std::is_same<typename std::remove_reference<decltype(field)>::type, size_t>::value)
            {
               field = std::atoi(map[what].c_str());
            }
        }

    };

    set(POPULATION_SIZE, population_size_);
    set(GENOME_SIZE, genome_size_);
    set(DISJOINT_COEFF, disjoint_coeff_);
    set(SURVIVAL_TRESH, survival_tresh_);
    set(EXCESS_COEFF, excess_coeff_);
    set(WEIGHTS_COEFF, weights_coeff_);
    set(DISTANCE_THRESHOLD, distance_threshold_);
    set(P_LINK_MUTATION, link_mutation_chance_);
    set(P_NODE_MUTATION, node_mutation_chance_);
    set(P_WEIGHT_MUTATION, weight_mutation_chance_);
    set(P_ENABLE_DISABLE_MUTATION, enable_disable_mutation_chance_);
    set(MIN_WEIGHT, min_weight_);
    set(MAX_WEIGHT, max_weight_);
}
std::vector<std::string> Parameters::get_parameters()
{
    return parameters_;
}
const std::string Parameters::LOGGING_DESTINATION = "logging_destination";
const std::string Parameters::POPULATION_SIZE = "population_size";
const std::string Parameters::GENOME_SIZE = "genome_size";
const std::string Parameters::SURVIVAL_TRESH = "survival_tresh";
const std::string Parameters::DISJOINT_COEFF = "disjoint_coeff";
const std::string Parameters::EXCESS_COEFF = "excess_coeff";
const std::string Parameters::WEIGHTS_COEFF = "weights_coeff";
const std::string Parameters::DISTANCE_THRESHOLD = "distance_threshold";

const std::string Parameters::P_LINK_MUTATION = "link_mutation_chance";
const std::string Parameters::P_NODE_MUTATION = "node_mutation_chance";
const std::string Parameters::P_WEIGHT_MUTATION = "weight_mutation_chance";
const std::string Parameters::P_ENABLE_DISABLE_MUTATION = "enable_disable_mutation_chance";

const std::string Parameters::MIN_WEIGHT = "min_weight";
const std::string Parameters::MAX_WEIGHT = "max_weight";

size_t Parameters::population_size()
{
        return population_size_;
}

size_t Parameters::genome_size()
{
    return genome_size_;
}

float Parameters::survival_tresh()
{
    return survival_tresh_;
}

float Parameters::disjoint_coeff()
{
    return disjoint_coeff_;
}

float Parameters::excess_coeff()
{
    return excess_coeff_;
}

float Parameters::weights_coeff()
{
   return weights_coeff_;
}

float Parameters::distance_threshold()
{
    return distance_threshold_;
}

float Parameters::link_mutation_chance()
{
    return link_mutation_chance_;
}

float Parameters::node_mutation_chance()
{
    return node_mutation_chance_;
}

float Parameters::weight_mutation_chance()
{
    return weight_mutation_chance_;
}

float Parameters::enable_disable_mutation_chance()
{
   return enable_disable_mutation_chance_;
}

float Parameters::min_weight()
{
   return min_weight_;
}

float Parameters::max_weight()
{
   return max_weight_;
}

std::vector<std::string> Parameters::parameters_ = {
    Parameters::LOGGING_DESTINATION,
    Parameters::POPULATION_SIZE,
    Parameters::GENOME_SIZE,
    Parameters::SURVIVAL_TRESH,
    Parameters::DISJOINT_COEFF,
    Parameters::EXCESS_COEFF,
    Parameters::WEIGHTS_COEFF,
    Parameters::DISTANCE_THRESHOLD,
    Parameters::P_LINK_MUTATION,
    Parameters::P_NODE_MUTATION,
    Parameters::P_WEIGHT_MUTATION,
    Parameters::P_ENABLE_DISABLE_MUTATION,
    Parameters::MIN_WEIGHT,
    Parameters::MAX_WEIGHT,
};
