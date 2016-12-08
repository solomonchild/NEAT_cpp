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

    // TODO: use max_population constant
    void add_genome(const Genome& genome);
    ~Pool();

    SpeciesVector::iterator begin();
    SpeciesVector::iterator end();
private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
