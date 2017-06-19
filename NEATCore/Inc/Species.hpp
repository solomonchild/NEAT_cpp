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
    bool will_genome_fit(const Genome&);
    void calculate_fitness();

    Species(const Species&);
    Species& operator=(const Species&);

    Species(Species&&);
    Species& operator=(Species&&);
    ~Species();

    Genomes::iterator begin();
    Genomes::iterator end();
    bool empty() const;
    size_t size() const;

    void remove_weak_genomes();
    void remove_stale_genomes();
private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
