#include "Pool.hpp"
#include "Parameters.hpp"
#include <algorithm>


//Species should be added on demand

Pool::Pool(std::shared_ptr<RandomGenerator>& generator)
    : generator_(generator)
{
    for (unsigned i = 0; i < Parameters::population_size; ++i)
    {
        add_genome({generator});
    }

}
void Pool::add_genome(const Genome& genome)
{
    for(auto& species : species_)
    {
        if(species.will_genome_fit(genome))
        {
            species.add_genome(genome);
            return;
        }
    }
    species_.push_back(Species(generator_));
    species_.back().add_genome(genome);
}

size_t Pool::size() const
{
    size_t size = 0;
    for(auto& species : species_)
    {
        size += species.size();
    }
    return size;
}
void Pool::purge()
{
    species_.erase(std::remove_if(species_.begin(), species_.end(), [](Species& s){return s.empty();}), species_.end());
}

Pool::SpeciesVector::iterator Pool::begin()
{
    return species_.begin();
}

Pool::SpeciesVector::iterator Pool::end()
{
    return species_.end();
}

Species& Pool::at(size_t index)
{
    return species_[index];
}

size_t Pool::number_of_species() const
{
    return species_.size();
}
