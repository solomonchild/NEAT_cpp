#include "Pool.hpp"
#include "Parameters.hpp"


//Species should be added on demand

struct Pool::Impl
{

    Impl(std::shared_ptr<RandomGenerator>& generator)
        : generator_(generator)
    {
        for (unsigned i = 0; i < Parameters::population_size; ++i)
        {
            add_genome({generator});
        }

    }
    void add_genome(const Genome& genome)
    {
        for(auto& species : species_)
        {
            if(species.will_genome_fit(genome))
            {
                species.add_genome(genome);
                return;
            }
        }
        species_.push_back(::Species(generator_));
        species_.back().add_genome(genome);
    }

    std::shared_ptr<RandomGenerator> generator_;
    SpeciesVector species_;
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

Pool::SpeciesVector::iterator Pool::begin()
{
    return impl_->species_.begin();
}

Pool::SpeciesVector::iterator Pool::end()
{
    return impl_->species_.end();
}
