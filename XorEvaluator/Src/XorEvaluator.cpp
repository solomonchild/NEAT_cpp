#include <stdexcept>
#include <sstream>
#include <cmath>
#include <cassert>
#include <iostream>

#include "Evaluator.hpp"

const unsigned Evaluator::number_of_inputs_ = 2;
const unsigned Evaluator::number_of_outputs_ = 1;

std::pair<float, float> Evaluator::get_fitness_error(const std::vector<Outputs>& outputs, const std::vector<Inputs>& inputs)
{
    assert(outputs.size() == inputs.size());
//    if (outputs.size() != number_of_outputs_
//            || inputs.size() != number_of_inputs_)
//    {
//        std::stringstream ss;
//        ss << "Number of inputs or outputs of provided vector do not match with actual";
//        throw new std::invalid_argument(ss.str());
//    }
    float square = 0;
    for(unsigned i = 0; i < outputs.size(); i++)
    {
        auto o = outputs[i];
        auto in = inputs[i];

        auto arg1 = in[0];
        auto arg2 = in[1];
        auto expected = arg1 ^ arg2;
        square += std::fabs(o[0] - expected);
    }
    float error = square;
    square = std::pow(4-square, 2);
    if(std::isinf(square))
    {
        square = 0;
    }
    std::cout << "Error: " << error << '\n';
    return std::make_pair(square, error);
}
