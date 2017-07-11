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
    void set_error(float);
    float get_error() const;

    Genome(std::shared_ptr<RandomGenerator>, const Genes& = {});

    Genome(const Genome&);
    Genome& operator=(const Genome&);

    Genome(Genome&&);
    Genome& operator=(Genome&&);
    ~Genome();
    bool operator ==(const Genome&) const;
    unsigned id();

private:

    struct Neuron
    {
        Neuron(int idx)
            :index_(idx)
            , value_(0.0f)
        {
        }

        operator int()
        {
            return index_;
        }

        unsigned int index_;
        Genome::Genes input_;
        float value_;
    };

    void mutate_weight();
    void mutate_connection();
    float get_weight();
    using Neurons = std::vector<Neuron>;
    Neuron get_random_neuron(bool notOutput = false);
    Neurons get_neurons();
    void mutate_node();
    Gene& get_random_gene();
    void mutate_enable_disable();
    friend std::ostream& operator<<(std::ostream&, const Genome&);
    std::shared_ptr<RandomGenerator> generator_;
    unsigned int last_neuron_;
    Genes genes_;
    unsigned id_;
    static unsigned ID;
    float fitness_;
    float error_;
};
