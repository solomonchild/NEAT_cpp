#include "Pool.hpp"


//Species should be added on demand

struct Pool::Impl
{

    Impl(std::shared_ptr<RandomGenerator>& generator)
        : generator_(generator)
    {
    }
    void add_genome(const Genome& genome)
    {
        for(auto& species : species_)
        {
            if(species.will_genome_fit(genome))
            {
                //add to species
            }
        }
    }

    std::shared_ptr<RandomGenerator> generator_;
    Species species_;
};

Pool::Pool(std::shared_ptr<RandomGenerator> & generator)
    :impl_(new Impl(generator))
{
}

void Pool::add_genome(const Genome& genome)
{
    impl_->add_genome(genome);
}


Pool::~Pool() = default;
