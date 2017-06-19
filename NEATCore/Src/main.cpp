#include <iostream>
#include <vector>

#include "Parameters.hpp"
#include "Genome.hpp"
#include "Evaluator.hpp"
#include "Logger.hpp"
#include "Pool.hpp"
#include <cmath>

int main(int argc, char** argv)
{
    try
    {
        std::shared_ptr<RandomGenerator> generator = std::make_shared<RandomGenerator>();
        std::array<Inputs, 4> inputs = {Inputs{1, 0}, Inputs{0, 1}, Inputs{0,0}, Inputs{1,1}};
        Pool pool(generator);
        Evaluator eval;
        Environment::set_log_level(Environment::LogLevel::Info);
        Environment::set_log_dest(Environment::LogDestination::Console);

        uint64_t iteration = 0;
        while(true)
        {
            iteration++;
            INFO("Iteration %lu", iteration);
            for(auto& species : pool)
            {
                //TODO: add species Id
                for(auto& genome : species)
                {
                    float min_fitness = 100;
                    for(auto input : inputs)
                    {
                        auto outputs = genome.evaluate_network(input);
                        auto fitness =  eval.get_fitness(outputs, input);
                        min_fitness = std::min(min_fitness, fitness);
                        std::cout << min_fitness << '\n';
                    }
                    genome.set_fitness(min_fitness);
                    // TODO: review
                    genome.mutate();
                    if(fabs(min_fitness) < 0.5)
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
                species.remove_stale_genomes();
                if(iteration % 5 == 0)
                {
                    species.remove_weak_genomes();
                }
            }
            pool.purge();
            for(auto& species : pool)
            {
                auto genome = species.breed();
                genomes.emplace_back(genome);
            }
            while(pool.size() + genomes.size() < Parameters::population_size)
            {
                auto species = pool.at(generator->get_next(pool.number_of_species() - 1));
                auto genome = species.breed();
                genomes.emplace_back(genome);

            }
            for(auto genome : genomes)
            {
                pool.add_genome(genome);
            }
            pool.purge();
        }

    }
    catch(std::exception* e)
    {
        std::cout << "Exception: " << e->what();
    }

    return 0;
}
