#pragma once

#include <vector>
#include <memory>

#include "Species.hpp"
#include "RandomGenerator.hpp"

class Pool
{
public:
    using SpeciesVector = std::vector<Species>;
    Pool(std::shared_ptr<RandomGenerator>&);
    void init();

    // TODO: use max_population constant
    void add_genome(const Genome& genome);

    SpeciesVector::iterator begin();
    SpeciesVector::iterator end();
    Species& at(size_t index);
    size_t number_of_species() const;
    size_t size() const;
    void remove_weak_species();
    void remove_stale_species();
    void purge();
private:
    std::shared_ptr<RandomGenerator> generator_;
    SpeciesVector species_;
    std::map<int, unsigned short> stale_map_;
};
