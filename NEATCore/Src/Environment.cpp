#include "Environment.hpp"

Inputs Environment::inputs_;

Inputs Environment::get_inputs()
{
    return inputs_;
}

void Environment::set_inputs(const Inputs &inputs)
{
    inputs_ = inputs;
}
