#include "Species.hpp"
#include "Evaluator.hpp"
#include "Parameters.hpp"
#include "Evaluator.hpp"
#include "Environment.hpp"
#include "Logger.hpp"

#include <algorithm>
#include <cassert>
#include <map>


    Species::Species(std::shared_ptr<RandomGenerator>& generator)
        :generator_(generator)
        , id_(ID++)
    {
        // genomes should be added on demand
    }

    void Species::add_genome(const Genome& genome)
    {
        genomes_.push_back(genome);
    }


    Genome Species::breed()
    {
        assert(genomes_.size() >= 1);
        if(genomes_.size() == 1)
        {
            Genome gen = genomes_.front();
            gen.mutate();
            return gen;
        }
        unsigned index_1 = generator_->get_next(genomes_.size() - 1);
        unsigned index_2 = generator_->get_next(genomes_.size() - 1);
        do
        {
            index_2 = generator_->get_next(genomes_.size() - 1);
        } while(index_1 == index_2);

        Genome& genome1 = genomes_.at(index_1);
        Genome& genome2 = genomes_.at(index_2);

        Genome gen = genome1.crossover(genome2);

        INFO("G1");
        INFO_STREAM(genome1);

        INFO("G2");
        INFO_STREAM(genome2);
        INFO("Crossover");
        INFO_STREAM(gen);

        gen.mutate();
        return gen;
    }

    bool Species::will_genome_fit(const Genome& genome)
    {
        auto comparator = [] (const Genome& g1, const Genome& g2)
        {
            return  g1.get_fitness() < g2.get_fitness();
        };

        if(genomes_.empty())
        {
            return true;
        }
        // in ascending order
        std::sort(genomes_.begin(), genomes_.end(), comparator);
        auto fittest_genome = genomes_.back();
        INFO("Fittest: %f, lowest: %f", genomes_.back().get_fitness(), genomes_.front().get_fitness());
        float distance = fittest_genome.compatibility_distance(genome);
        return distance <= Parameters::get_instance().distance_threshold();
    }


    void Species::remove_weak_genomes()
    {
        if(genomes_.empty())
        {
            INFO("No genomes to purge (empty species)");
            return;
        }

        INFO("[%lld] Genomes before purge: %d", id_, genomes_.size());

        auto calculate_avg_fitness = [this] () {
            float avg_fitness = 0.0f;
            for(auto& g: genomes_)
            {
                avg_fitness +=  g.get_fitness();
            }
            avg_fitness /= genomes_.size();
            // TODO: use some balancing factor for avg_fitness
            //avg_fitness *= 1.05;
            return avg_fitness;
        };
        float avg_fitness = calculate_avg_fitness();


        INFO("[%lld] Min: %f", id_, genomes_.front().get_fitness());
        INFO("[%lld] Max: %f", id_, genomes_.back().get_fitness());

        INFO("[%lld] Average fitness: %f", id_, avg_fitness);
        auto predicate = [avg_fitness](const Genome& genome){ return genome.get_fitness() <= avg_fitness; };
        genomes_.erase(std::remove_if(genomes_.begin(), genomes_.end(), predicate), genomes_.end());
        INFO("[%lld] Genomes after purge: %d", id_, genomes_.size());
        INFO("[%lld] Average fitness after purge: %f", id_, calculate_avg_fitness());
    }

    size_t Species::size() const
    {
       return genomes_.size();
    }

    bool Species::empty() const
    {
       return genomes_.empty();
    }

    Species::Species(const Species& other)
    {
        this->generator_ = other.generator_;
        this->id_ = other.id_;
        this->top_fitness_ = other.top_fitness_;
        this->genomes_ = other.genomes_;
        this->stale_map_ = other.stale_map_;
    }

    Species::Species(Species&& other)
    {
        this->generator_ = std::move(other.generator_);
        this->id_ = other.id_;
        this->top_fitness_ = other.top_fitness_;
        this->genomes_ = std::move(other.genomes_);
        this->stale_map_ = std::move(other.stale_map_);
    }

    Species& Species::operator=(Species&& other)
    {
        this->generator_ = std::move(other.generator_);
        this->id_ = other.id_;
        this->top_fitness_ = other.top_fitness_;
        this->genomes_ = std::move(other.genomes_);
        this->stale_map_ = std::move(other.stale_map_);
        return *this;
    }

    Species& Species::operator=(const Species& other)
    {
        this->generator_ = other.generator_;
        this->id_ = other.id_;
        this->top_fitness_ = other.top_fitness_;
        this->genomes_ = other.genomes_;
        this->stale_map_ = other.stale_map_;
        return *this;
    }

    Species::Genomes::iterator Species::begin()
    {
        return genomes_.begin();
    }

    Species::Genomes::iterator Species::end()
    {
        return genomes_.end();
    }

    void Species::remove_stale_genomes()
    {
        for(size_t i = 0; i < genomes_.size(); i++)
        {
            auto fitness = genomes_[i].get_fitness();
            if(fitness > top_fitness_)
            {
                top_fitness_ = fitness;
                stale_map_[i] = 0;
            }
            else
            {
                stale_map_[i] += 1;
            }
        }
        for(size_t i = 0; i < stale_map_.size(); i++)
        {
            if(stale_map_[i] > 5)
            {
                genomes_.erase(genomes_.begin() + i);
                stale_map_[i] = 0;
            }

        }

    }

unsigned Species::ID = 0;

Species::~Species() = default;


std::ostream& operator<<(std::ostream& stream, const Species& species)
{
    stream << "Species #" << species.id_ << "\n{\n";
    stream << "Contains: " << species.genomes_.size() << " genomes.\n";
    for(auto& genome : species.genomes_)
    {
        stream << genome;
    }
    stream << "}\n";
    return stream;
}
unsigned Species::id()
{
    return id_;
}
