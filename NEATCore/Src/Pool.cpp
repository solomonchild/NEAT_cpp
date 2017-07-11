#include "Pool.hpp"

#include <algorithm>

#include "Parameters.hpp"
#include "Logger.hpp"
#include <algorithm>



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

void Pool::purge()
{
    species_.erase(std::remove_if(species_.begin(), species_.end(), [](Species& s){return s.empty();}), species_.end());

}

void Pool::remove_stale_species()
{

    for(size_t i = 0; i < species_.size(); i++)
    {
        species_[i].rank();
        auto& genome = *(species_[i].begin());
        auto fitness = genome.get_fitness();
        if(fitness > species_[i].top_fitness_)
        {
            species_[i].top_fitness_ = fitness;
            stale_map_[i] = 0;
        }
        else
        {
            stale_map_[i] += 1;
        }
    }
    for(size_t i = 0; i < stale_map_.size(); i++)
    {
        if(stale_map_[i] > Parameters::get_instance().staleness())
        {
            species_.erase(species_.begin() + i);
            stale_map_[i] = 0;
        }

    }
}

void Pool::remove_weak_species()
{

    float total_avg_fitness = 0;
    for(Species& species : species_)
    {
        species.remove_weak_genomes();
        total_avg_fitness = species.calculate_fitness();
    }
    total_avg_fitness /= species_.size();


    auto predicate = [total_avg_fitness](const Species& s)
    {
        return (s.calculate_fitness() / total_avg_fitness * Parameters::get_instance().population_size()) < 1;
    };
    species_.erase(std::remove_if(species_.begin(), species_.end(), predicate), species_.end());
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
