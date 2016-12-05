#include <iostream>
#include <vector>

#include "Parameters.hpp"
#include "Genome.hpp"
#include "Evaluator.hpp"
#include "Logger.hpp"
#include "Pool.hpp"

int main(int argc, char** argv)
{
    try
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
        std::array<Inputs, 4> inputs = {Inputs{1, 0}, Inputs{0, 1}, Inputs{0,0}, Inputs{1,1}};
        (void) inputs;
    }
    catch(std::exception* e)
    {
        std::cout << "Exception: " << e->what();
    }

    return 0;
}
