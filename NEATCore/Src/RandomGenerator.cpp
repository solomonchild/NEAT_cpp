#include "RandomGenerator.hpp"

#include <ctime>
#include <cstdlib>

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
        return rand() % rand_max;
    }

};


RandomGenerator::RandomGenerator()
    :impl_(new Impl)
{
}

RandomGenerator::~RandomGenerator() = default;

RandomGenerator::RandomGenerator(const RandomGenerator& other)
{
    this->impl_ = std::unique_ptr<Impl>(new Impl(*other.impl_));

}




float RandomGenerator::get_next(uint16_t rand_max) const
{
    return impl_->get_next(rand_max);
}
