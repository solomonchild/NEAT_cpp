#include "Genome.hpp"

#include <memory>
#include <map>

#include "Parameters.hpp"
#include "Gene.hpp"


struct Genome::Impl
{
    std::map<int, Gene> genes_;

    Impl(const RandomGenerator& generator)
        :generator_(generator)
    {
    }

    float compatibility_distance(const Genome& rhs)
    {
        return 0;
    }

    void mutate_weight()
    {

    }

    void mutate_enable_disable()
    {

    }

    void mutate_node()
    {

    }

    void mutate_connection()
    {

    }

    void mutate()
    {
        // TODO: Probabilities of node, point and enable/disable mutations
        float p_of_link_mutate = generator_.get_next(1);
        if(p_of_link_mutate <= Parameters::link_mutation_chance)
        {
            mutate_connection();
        }
    }

    RandomGenerator generator_;
};

Genome::Genome(const RandomGenerator& generator)
    :impl_(new Impl(generator))
{}

Genome::~Genome() = default;

Genome::Genome(const Genome& other)
{
    this->impl_ = std::make_unique<Impl>(*other.impl_);
}

Genome& Genome::operator=(const Genome& other)
{
    this->impl_ = std::make_unique<Impl>(*other.impl_);
    return *this;
}

Genome::Genome(Genome&& other)
{
    this->impl_ = std::make_unique<Impl>(std::move(*other.impl_));
}

Genome& Genome::operator=(Genome&& other)
{
    this->impl_ = std::make_unique<Impl>(std::move(*other.impl_));
    return *this;
}

void Genome::mutate()
{
    impl_->mutate();
}

float Genome::compatibility_distance(const Genome& rhs)
{
    return impl_->compatibility_distance(rhs);
}
