#include "Environment.hpp"

unsigned Environment::innovation_number_ = 1;

void Environment::inc_innovation_number()
{
   innovation_number_++;
}

unsigned Environment::get_innovation_number()
{
    return innovation_number_;
}
