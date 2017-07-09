#include <iostream>
#include <vector>
#include <cmath>

#include "Parameters.hpp"
#include "Genome.hpp"
#include "Evaluator.hpp"
#include "Logger.hpp"
#include "Pool.hpp"
#include "ConfigParser.hpp"

std::shared_ptr<RandomGenerator> generator = std::make_shared<RandomGenerator>();

void breed_till_full(Pool& pool,std::vector<Inputs>& inputs, Evaluator& eval)
{

    Species::Genomes genomes;
    for(auto& species : pool)
    {
        auto genome = species.breed();

        std::vector<Outputs> all_outputs;
        for(auto input : inputs)
        {
            all_outputs.push_back(genome.evaluate_network(input));
        }
        auto fitness =  eval.get_fitness(all_outputs, inputs);
        genome.set_fitness(fitness);
        genomes.emplace_back(genome);
        INFO("Fitness for a new genome: %f", fitness);
    }

    if(pool.size() != 0)
    {
        while(pool.size() + genomes.size() < Parameters::get_instance().population_size())
        {
            Species& species = pool.at(generator->get_next(pool.number_of_species() - 1));
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

int main(int argc, char** argv)
{
    std::vector<Inputs> inputs = {Inputs{1, 0}, Inputs{0, 1}, Inputs{0,0}, Inputs{1,1}};
    Pool pool(generator);
    Evaluator eval;
    try
    {

        Environment::set_log_level(Environment::LogLevel::Info);
        Environment::set_log_dest(Environment::LogDestination::File);
        Logger::set_filename("out.txt");
        Logger::trunc();

        uint64_t iteration = 0;
        Parameters::get_instance().parse();

        for(auto& species : pool)
        {
            for(auto& genome : species)
            {
                std::vector<Outputs> all_outputs;
                for(auto input : inputs)
                {
                    all_outputs.push_back(genome.evaluate_network(input));
                }
                INFO("Outputs: %f(e. 1), %f (e. 1), %f (e. 0), %f (e. 0)", all_outputs[0][0],
                        all_outputs[0][1], all_outputs[0][2], all_outputs[0][3]);

                float fitness =  eval.get_fitness(all_outputs, inputs);
                INFO("Fitness: %f", fitness);
                genome.set_fitness(fitness);
                INFO("Genomes' iD: %u", genome.id());
                Logger::get_instance().dump(std::string("init_")+std::to_string(species.id()) +"_" + std::to_string(genome.id()) + ".dot", genome);
                INFO(std::string("init_")+std::to_string(genome.id()) + ".dot");
            }
        }

        while(true)
        {
            iteration++;
            INFO("\n\n\nIteration %lu", iteration);
            if(pool.size() == 0)
            {
                INFO("Pool is empty");
                return -1;
            }

            for(auto& species : pool)
            {
                for(auto& genome : species)
                {
                    genome.mutate();
                    std::vector<Outputs> all_outputs;
                    for(auto input : inputs)
                    {
                        all_outputs.push_back(genome.evaluate_network(input));
                    }
                    INFO("Outputs: %f(e. 1), %f (e. 1), %f (e. 0), %f (e. 0)", all_outputs[0][0],
                            all_outputs[0][1], all_outputs[0][2], all_outputs[0][3]);

                    float fitness =  eval.get_fitness(all_outputs, inputs);
                    INFO("Fitness: %f", fitness);
                    genome.set_fitness(fitness);

                    if(fitness > 0)
                    {
                        INFO("Done.");
                        return 0;
                    }
                }
                auto& top_genome = *(species.end() - 1);
                Logger::get_instance().dump(std::to_string(iteration) + "_" + std::to_string(species.id()) + "_" + std::to_string(top_genome.id())+".dot", top_genome);
            }

            pool.remove_weak_species();
            pool.remove_stale_species();
            breed_till_full(pool, inputs, eval);
        }

    }
    catch(std::exception* e)
    {
        std::cout << "Exception: " << e->what();
    }

    return 0;
}
