#include "Species.hpp"
#include "Evaluator.hpp"
#include "Parameters.hpp"
#include "Evaluator.hpp"
#include "Environment.hpp"

#include <algorithm>
#include <cassert>


struct Species::Impl
{
    Impl(std::shared_ptr<RandomGenerator>& generator)
        :generator_(generator)
    {
        //Genomes should be added on demand
    }

    void add_genome(const Genome& genome)
    {
        genomes_.push_back(genome);
    }


    Genome breed()
    {
        assert(genomes_.size() >= 1);
        if(genomes_.size() == 1)
        {
            return genomes_.front();
        }
        auto genome1 = genomes_.at(generator_->get_next(genomes_.size()));
        auto genome2 = genomes_.at(generator_->get_next(genomes_.size()));
        return genome1.crossover(genome2);

        //TODO: mutate at once
    }

    bool will_genome_fit(const Genome& genome)
    {
        auto comparator = [] (const Genome& g1, const Genome& g2)
        {
            Evaluator eval;
            auto outputs_first = g1.evaluate_network(Environment::get_inputs());
            float fit1 = eval.get_fitness(outputs_first, Environment::get_inputs());

            auto outputs_second = g2.evaluate_network(Environment::get_inputs());
            float fit2 = eval.get_fitness(outputs_second, Environment::get_inputs());

            return  fit1 < fit2;
        };

        //in ascending
        std::sort(genomes_.begin(), genomes_.end(), comparator);
        auto fittest_genome = genomes_.back();
        float distance = fittest_genome.compatibility_distance(genome);
        return distance <= Parameters::distance_threshold;
    }

    std::shared_ptr<RandomGenerator>& generator_;
    Genomes genomes_;
};

Species::Species(std::shared_ptr<RandomGenerator>& generator)
    : impl_(new Impl(generator))
{
}

Species::~Species() = default;


void Species::add_genome(const Genome& genome)
{
    impl_->add_genome(genome);
}


bool Species::will_genome_fit(const Genome& genome)
{
    return impl_->will_genome_fit(genome);
}

Genome Species::breed()
{
    return impl_->breed();
}
