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
        std::vector<Inputs> inputs = {Inputs{1, 0}, Inputs{0, 1}, Inputs{0,0}, Inputs{1,1}};
        Pool pool(generator);
        Evaluator eval;
        Environment::set_log_level(Environment::LogLevel::Info);
        Environment::set_log_dest(Environment::LogDestination::Console);
        Logging::init();


        uint64_t iteration = 0;
        while(true)
        {
            iteration++;
            INFO("\n\n\nIteration %lu", iteration);
            if(pool.size() == 0)
            {
                INFO("Pool is empty");
            }
            for(auto& species : pool)
            {
                //TODO: add species Id
                for(auto& genome : species)
                {
                    std::vector<Outputs> all_outputs;
                    for(auto input : inputs)
                    {
                        all_outputs.push_back(genome.evaluate_network(input));
                    }

                    auto fitness =  eval.get_fitness(all_outputs, inputs);
                    genome.set_fitness(fitness);
                    // TODO: review
                    genome.mutate();
                    INFO("Fitness: %f", fitness);
                    if(fitness > 0.9)
                    {
                        INFO("Done.");
                        INFO_STREAM(genome);
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

                    std::vector<Outputs> all_outputs;
                    for(auto input : inputs)
                    {
                        all_outputs.push_back(genome.evaluate_network(input));
                    }
                    auto fitness =  eval.get_fitness(all_outputs, inputs);
                    genome.set_fitness(fitness);
                INFO("Fitness for a new genome: %f", fitness);
            }
            pool.purge();
            if(pool.size() != 0)
            {
                while(pool.size() + genomes.size() < Parameters::population_size)
                {
                    auto& species = pool.at(generator->get_next(pool.number_of_species() - 1));
                    auto genome = species.breed();

                    std::vector<Outputs> all_outputs;
                    for(auto input : inputs)
                    {
                        all_outputs.push_back(genome.evaluate_network(input));
                    }

                    auto fitness =  eval.get_fitness(all_outputs, inputs);
                    genome.set_fitness(fitness);
                    INFO("Fitness for a new genome: %f", fitness);
                    genomes.emplace_back(genome);
                }
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
