#pragma once

#include <vector>

using Outputs = std::vector<int>;
using Inputs = std::vector<int>;

struct Evaluator
{
    Evaluator();

    float get_fitness(const Outputs&, const Inputs& = {});

    const unsigned int number_of_inputs_;
    const unsigned int number_of_outputs_;
};
