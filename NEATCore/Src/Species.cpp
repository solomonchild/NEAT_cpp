#include "Species.hpp"
#include "Evaluator.hpp"
#include "Parameters.hpp"
#include "Evaluator.hpp"
#include "Environment.hpp"
#include "Logger.hpp"

#include <algorithm>
#include <cassert>
#include <map>


struct Species::Impl
{
    Impl(std::shared_ptr<RandomGenerator>& generator)
        :generator_(generator)
        , id_(ID++)
    {
        // genomes should be added on demand
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

        auto genome1 = genomes_.at(index_1);
        auto genome2 = genomes_.at(index_2);

        auto gen = genome1.crossover(genome2);

        gen.mutate();
        return gen;
    }

    bool will_genome_fit(const Genome& genome)
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
        float distance = fittest_genome.compatibility_distance(genome);
        return distance <= Parameters::distance_threshold;
    }


    void remove_weak_genomes()
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
        auto predicate = [avg_fitness](const Genome& genome){ return genome.get_fitness() > avg_fitness; };
        genomes_.erase(std::remove_if(genomes_.begin(), genomes_.end(), predicate), genomes_.end());
        INFO("[%lld] Genomes after purge: %d", id_, genomes_.size());
        INFO("[%lld] Average fitness after purge: %f", id_, calculate_avg_fitness());
    }

    void remove_stale_genomes()
    {
        for(size_t i = 0; i < genomes_.size(); i++)
        {
            auto fitness = genomes_[i].get_fitness();
            if(fitness < top_fitness_)
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

    std::shared_ptr<RandomGenerator>& generator_;
    long long id_;
    float top_fitness_;
    static long long ID;
    Genomes genomes_;
    std::map<int, short> stale_map_;
};
long long Species::Impl::ID = 0;

Species::Species(std::shared_ptr<RandomGenerator>& generator)
    : impl_(new Impl(generator))
{
}

Species::~Species() = default;

Species::Species(const Species& other)
{
    this->impl_ = std::make_unique<Impl>(*other.impl_);
}

Species& Species::operator=(const Species& other)
{
    this->impl_ = std::make_unique<Impl>(*other.impl_);
    return *this;
}

Species::Species(Species&& other)
{
    this->impl_ = std::make_unique<Impl>(std::move(*other.impl_));
}

Species& Species::operator=(Species&& other)
{
    this->impl_ = std::make_unique<Impl>(std::move(*other.impl_));
    return *this;
}


void Species::add_genome(const Genome& genome)
{
    impl_->add_genome(genome);
}


bool Species::will_genome_fit(const Genome& genome)
{
    return impl_->will_genome_fit(genome);
}

Genome Species::breed()
{
    return impl_->breed();
}

Species::Genomes::iterator Species::begin()
{
    return impl_->genomes_.begin();
}

Species::Genomes::iterator Species::end()
{
    return impl_->genomes_.end();
}

void Species::remove_weak_genomes()
{
    impl_->remove_weak_genomes();
}

bool Species::empty() const
{
    return impl_->genomes_.empty();
}

size_t Species::size() const
{
    return impl_->genomes_.size();
}

void Species::remove_stale_genomes()
{
    impl_->remove_stale_genomes();
}
