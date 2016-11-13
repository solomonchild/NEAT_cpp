#pragma once

#include <memory>
#include <vector>

#include "Genome.hpp"

class Species
{
public:
    using Genomes = std::vector<Genome>;
    Species();
private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
