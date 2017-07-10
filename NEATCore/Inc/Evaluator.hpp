#pragma once

#include <vector>

using Outputs = std::vector<float>;
using Inputs = std::vector<int>;

struct Evaluator
{
    std::pair<float, float> get_fitness_error(const std::vector<Outputs>&, const std::vector<Inputs>& = {});

    static const unsigned number_of_inputs_;
    static const unsigned number_of_outputs_;
};
