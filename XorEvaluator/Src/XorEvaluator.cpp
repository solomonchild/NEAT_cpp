#include <stdexcept>
#include <sstream>
#include <cmath>

#include "Evaluator.hpp"

Evaluator::Evaluator()
   : number_of_inputs_(2)
   , number_of_outputs_(1)
{
}

float Evaluator::get_fitness(const Outputs& outputs, const Inputs& inputs)
{
    if (outputs.size() != number_of_outputs_
            || inputs.size() != number_of_inputs_)
    {
        std::stringstream ss;
        ss << "Number of inputs or outputs of provided vector do not match with actual";
        throw new std::invalid_argument(ss.str());
    }
    auto arg1 = inputs[0];
    auto arg2 = inputs[1];
    auto real_value = arg1 ^ arg2;
    return std::fabs(outputs[0] - real_value);
}
