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
            if(std::is_same<decltype(field), int>::value)
            {
               field = std::atoi(map[what].c_str());
            }
            else if(std::is_same<decltype(field), float>::value)
            {
               field = std::stof(map[what].c_str());
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
//    if(contains(map, POPULATION_SIZE))
//        population_size_ = std::atoi(map[POPULATION_SIZE].c_str());

//    if(contains(map, GENOME_SIZE))
//        genome_size_ = std::atoi(map[GENOME_SIZE].c_str());
//    if(contains(map, DISJOINT_COEFF))
//        disjoint_coeff_ = std::stof(map[DISJOINT_COEFF].c_str());
//    if(contains(map, EXCESS_COEFF))
//        excess_coeff_ = std::stof(map[EXCESS_COEFF].c_str());
//    if(contains(map, WEIGHTS_COEFF))
//        weights_coeff_ = std::stof(map[WEIGHTS_COEFF].c_str());
//    distance_threshold_ = std::stof(map[DISTANCE_THRESHOLD].c_str());
//    link_mutation_chance_ = std::stof(map[P_LINK_MUTATION].c_str());
//    node_mutation_chance_ = std::stof(map[P_NODE_MUTATION].c_str());
//    weight_mutation_chance_ = std::stof(map[P_WEIGHT_MUTATION].c_str());
//    enable_disable_mutation_chance_ = std::stof(map[P_ENABLE_DISABLE_MUTATION].c_str());
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

std::string Parameters::get_typename_for(const std::string& tag)
{
    if(types_.find(tag) == types_.end())
    {
        return "null";
    }
    return types_[tag];
}
std::map<std::string, std::string> Parameters::types_ = {

    {Parameters::LOGGING_DESTINATION, typeid(std::string).name()},
    {Parameters::POPULATION_SIZE, typeid(int).name()},
    {Parameters::GENOME_SIZE, typeid(int).name()},
    {Parameters::SURVIVAL_TRESH, typeid(float).name()},
    {Parameters::DISJOINT_COEFF, typeid(float).name()},
    {Parameters::EXCESS_COEFF, typeid(float).name()},
    {Parameters::WEIGHTS_COEFF, typeid(float).name()},
    {Parameters::DISTANCE_THRESHOLD, typeid(float).name()},
    {Parameters::P_LINK_MUTATION, typeid(float).name()},
    {Parameters::P_NODE_MUTATION, typeid(float).name()},
    {Parameters::P_WEIGHT_MUTATION, typeid(float).name()},
    {Parameters::P_ENABLE_DISABLE_MUTATION, typeid(float).name()}
};

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
};
