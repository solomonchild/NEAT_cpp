#pragma once

#include <cstddef>
#include "Evaluator.hpp"
#include "ConfigParser.hpp"

class Parameters
{

public:
    static Parameters& get_instance();

    void parse();

    size_t population_size();
    size_t genome_size();
    float disjoint_coeff();
    float excess_coeff();
    float weights_coeff();
    float distance_threshold();
    float link_mutation_chance();
    float node_mutation_chance();
    float weight_mutation_chance();
    float enable_disable_mutation_chance();
    static std::vector<std::string> get_parameters();
    static std::string get_typename_for(const std::string& tag);
private:
    Parameters() {}

   size_t population_size_ = 100;
   size_t genome_size_ = 15;
   float disjoint_coeff_ = 2.0;
   float excess_coeff_ = 2.0;
   float weights_coeff_ = 0.4;
   float distance_threshold_ = 1.0;

   float link_mutation_chance_ = 0.80;
   float node_mutation_chance_ = 0.80;
   float weight_mutation_chance_ = 0.80;
   float enable_disable_mutation_chance_ = 0.10;
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
};
