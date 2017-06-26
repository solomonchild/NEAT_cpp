#include "Pool.hpp"

#include <algorithm>

#include "Parameters.hpp"
#include "Logger.hpp"



//Species should be added on demand

Pool::Pool(std::shared_ptr<RandomGenerator>& generator)
    : generator_(generator)
{
    init();
}

void Pool::init()
{
    for (unsigned i = 0; i < Parameters::get_instance().population_size(); ++i)
    {
        Genome g = {generator_};
        add_genome(g);
    }

}

void Pool::add_genome(const Genome& genome)
{
#include <algorithm>
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
    INFO("Before purging pool: %d", species_.size());
    species_.erase(std::remove_if(species_.begin(), species_.end(), [](Species& s){return s.empty();}), species_.end());
    INFO("After purging pool: %d", species_.size());
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
