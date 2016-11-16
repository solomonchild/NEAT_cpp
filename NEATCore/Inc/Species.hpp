#pragma once

#include <memory>
#include <vector>

#include "Genome.hpp"
#include "RandomGenerator.hpp"

class Species
{
public:
    using Genomes = std::vector<Genome>;
    Species(std::shared_ptr<RandomGenerator>&);
private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
