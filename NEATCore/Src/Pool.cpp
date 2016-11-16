#include "Pool.hpp"


//Species should be added on demand

struct Pool::Impl
{

    Impl(std::shared_ptr<RandomGenerator>& generator)
        : generator_(generator)
    {
    }

    std::shared_ptr<RandomGenerator> generator_;
    Species species_;
};

Pool::Pool(std::shared_ptr<RandomGenerator> & generator)
    :impl_(new Impl(generator))
{
}


Pool::~Pool() = default;
