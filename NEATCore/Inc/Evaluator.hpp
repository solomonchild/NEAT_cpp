#pragma once

#include <vector>

using Outputs = std::vector<float>;
using Inputs = std::vector<int>;

struct Evaluator
{
    float get_fitness(const std::vector<Outputs>&, const std::vector<Inputs>& = {});

    static const unsigned number_of_inputs_;
    static const unsigned number_of_outputs_;
};
