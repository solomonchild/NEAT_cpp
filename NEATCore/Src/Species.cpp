#include "Species.hpp"
#include "Evaluator.hpp"
#include "Parameters.hpp"
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

Genome Species::breed()
{
    return impl_->breed();
}
