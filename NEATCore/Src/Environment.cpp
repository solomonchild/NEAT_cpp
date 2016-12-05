#include "Environment.hpp"

Inputs Environment::inputs_;
unsigned Environment::innovation_number_ = 1;

Inputs Environment::get_inputs()
{
    return inputs_;
}

void Environment::set_inputs(const Inputs &inputs)
{
    inputs_ = inputs;
}


void Environment::inc_innovation_number()
{
   innovation_number_++;
}

unsigned Environment::get_innovation_number()
{
    return innovation_number_;
}
