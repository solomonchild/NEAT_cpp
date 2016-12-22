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
                INFO("Next species");
                for(auto& genome : species)
                {
                    float max_fitness = 0;
                    for(auto input : inputs)
                    {
                        auto outputs = genome.evaluate_network(input);
                        auto fitness =  eval.get_fitness(outputs, input);
                        max_fitness = std::max(max_fitness, fitness);
                    }
                    genome.set_fitness(max_fitness);
                    // TODO: review
                    genome.mutate();
                    INFO("Fitness: %f", max_fitness);
                    if(max_fitness < 0.5)
                    {
                        INFO("Done.");
                        IF_INFO([&genome](){std::cout << genome;});
                        return 0;
                    }
                }
            }
            Species::Genomes genomes;
            for(auto& species : pool)
            {
                species.remove_weak_genomes();
                if(species.empty())
                {
                    continue;
                }
                CRIT("Before breed");
                auto genome = species.breed();
                CRIT("After breed");
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
