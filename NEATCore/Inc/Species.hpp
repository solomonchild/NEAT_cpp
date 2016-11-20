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
    void add_genome(const Genome&);

    /**
     * @brief breed
     * @pre genomes_ must contain at least one genome
     * @return
     */
    Genome breed();
    ~Species();
private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
