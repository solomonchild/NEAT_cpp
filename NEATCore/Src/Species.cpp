#include "Species.hpp"
#include "Evaluator.hpp"


struct Species::Impl
{

    Impl(std::shared_ptr<RandomGenerator>& generator)
        :generator_(generator)
    {

    }

    std::shared_ptr<RandomGenerator>& generator_;
    Genomes genomes_;
};

Species::Species(std::shared_ptr<RandomGenerator>& generator)
    : impl_(new Impl(generator))
{
}
