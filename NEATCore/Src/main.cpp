#include <iostream>
#include <vector>

#include "Parameters.hpp"
#include "Genome.hpp"
#include "Evaluator.hpp"


int main(int argc, char** argv)
{
    // for each genome apply problem
    // assign each genome to species
    // for each species, cull genomes
    // breed each species(randomly select genomes to crossover and/or mutate)
    std::shared_ptr<RandomGenerator> generator = std::make_shared<RandomGenerator>();
    std::vector<Genome> population;
    for(unsigned int i = 0; i < Parameters::population_size; ++i)
    {
        population.emplace_back(Genome (generator));
    }
    Evaluator eval;
    std::cout << eval.number_of_inputs_ << std::endl;
    std::cout << eval.number_of_outputs_ << std::endl;
    float fitness = 1;
    while(fitness >= 0.8)
    {
        break;
    }

    return 0;
}
