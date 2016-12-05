#pragma once

#include <memory>
#include <vector>

#include "RandomGenerator.hpp"
#include "Evaluator.hpp"
#include "Gene.hpp"

class Genome
{
public:
    using Genes = std::vector<Gene>;
    static std::unique_ptr<Genome, std::function<void(Genome*)>> create(const RandomGenerator&);

    float compatibility_distance(const Genome&) const;
    void mutate();

    Outputs evaluate_network(const Inputs&) const;

    Genome crossover(const Genome&);

    void set_fitness(float);
    float get_fitness() const;

    Genome(std::shared_ptr<RandomGenerator>, const Genes& = {});

    Genome(const Genome&);
    Genome& operator=(const Genome&);

    Genome(Genome&&);
    Genome& operator=(Genome&&);
    ~Genome();

    bool operator ==(const Genome&) const;

private:
    friend std::ostream& operator<<(std::ostream& stream, const Genome& genome);
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
