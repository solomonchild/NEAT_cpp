#include "RandomGenerator.hpp"

#include <ctime>
#include <cstdlib>
#include <iostream>

struct RandomGenerator::Impl
{
    Impl()
    {
        srand(time(NULL));
    }

    float get_next(uint16_t rand_max)
    {
        if(rand_max <= 0)
        {
            throw new std::invalid_argument("Maximum random number must be larger than 0");
        }
        float next = rand() / static_cast<float>(RAND_MAX) * static_cast<float>(rand_max);
        return next;
    }

};


RandomGenerator::RandomGenerator()
    :impl_(new Impl)
{
}

RandomGenerator::~RandomGenerator() = default;

RandomGenerator::RandomGenerator(const RandomGenerator& other)
{
    this->impl_ = std::make_unique<Impl>(*other.impl_);
}



float RandomGenerator::get_next(uint16_t rand_max) const
{
    return impl_->get_next(rand_max);
}
