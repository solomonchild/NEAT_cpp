#pragma once

#include <memory>
#include <vector>
#include <map>

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

    Species(Species&&);
    Species& operator=(Species&&);
    Species& operator=(const Species&);
    ~Species();

    Genomes::iterator begin();
    Genomes::iterator end();
    bool empty() const;
    size_t size() const;

    void remove_weak_genomes();
    void remove_stale_genomes();
private:
    std::shared_ptr<RandomGenerator> generator_;
    long long id_;
    float top_fitness_;
    static long long ID;
    Genomes genomes_;
    std::map<int, short> stale_map_;
};
