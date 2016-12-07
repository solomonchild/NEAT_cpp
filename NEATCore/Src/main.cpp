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
        std::shared_ptr<RandomGenerator> generator = std::make_shared<RandomGenerator>();
        std::array<Inputs, 4> inputs = {Inputs{1, 0}, Inputs{0, 1}, Inputs{0,0}, Inputs{1,1}};
        Pool pool(generator);
        Evaluator eval;
        Environment::set_log_level(Environment::LogLevel::Debug);

        while(true)
        {
            for(auto& species : pool)
            {
                for(auto& genome : species)
                {
                    float min_fitness = 0;
                    for(auto input : inputs)
                    {
                        auto outputs = genome.evaluate_network(input);
                        auto fitness =  eval.get_fitness(outputs, input);
                        min_fitness = std::min(min_fitness, fitness);
                    }
                    genome.set_fitness(min_fitness);
                    if(min_fitness > 0.3)
                    {
                        std::cout << genome;
                        std::cout << "DONEDONE";
                        break;
                    }
                }
            }
            Species::Genomes genomes;
            for(auto& species : pool)
            {
                auto genome = species.breed();
                genomes.emplace_back(genome);
            }
            for(auto genome : genomes)
            {
                pool.add_genome(genome);
            }
        }

    }
    catch(std::exception* e)
    {
        std::cout << "Exception: " << e->what();
    }

    return 0;
}
