#include <iostream>
#include <vector>

#include "Parameters.hpp"
#include "Genome.hpp"
#include "Evaluator.hpp"
#include "Logger.hpp"

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
        Evaluator eval;
        std::array<Inputs, 4> inputs = {Inputs{1, 0}, Inputs{0, 1}, Inputs{0,0}, Inputs{1,1}};
        std::array<float, 4> fitness = {};
        Genome genome(generator);
        while(true)
        {
            std::cout << genome << std::endl;
            std::array<Outputs, 4> outputs = {};
            bool to_break = true;
            for (unsigned i = 0; i < inputs.size(); ++i)
            {
                outputs[i] = genome.evaluate_network(inputs[i]);
                fitness[i] = eval.get_fitness(outputs[i], inputs[i]);
                if(fitness[i] > 0.8)
                {
                    to_break = false;
                }
                DEBUG("Fitness: %f", fitness[i]);
            }

            if(to_break)
            {
                break;
            }
            genome.mutate();
        }
    }
    catch(std::exception* e)
    {
        std::cout << "Exception: " << e->what();
    }

    return 0;
}
