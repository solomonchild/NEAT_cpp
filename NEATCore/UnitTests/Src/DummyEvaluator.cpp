#include <cmath>

#include "Evaluator.hpp"

const unsigned Evaluator::number_of_inputs_ = 2;

const unsigned Evaluator::number_of_outputs_ = 1;

float Evaluator::get_fitness(const std::vector<Outputs>& outputs, const std::vector<Inputs>& inputs)
{
    auto arg1 = inputs[0][0];
    auto arg2 = inputs[0][1];
    auto real_value = arg1 ^ arg2;
    return std::fabs(outputs[0][0] - real_value);
}
