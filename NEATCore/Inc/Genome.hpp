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
    unsigned id();

private:
    friend std::ostream& operator<<(std::ostream&, const Genome&);
    std::shared_ptr<RandomGenerator> generator_;
    unsigned int last_neuron_;
    Genes genes_;
    unsigned id_;
    static unsigned ID;
    float fitness_;
};
