#include "Genome.hpp"

#include <memory>
#include "Parameters.hpp"


struct Genome::Impl
{

    float compatibility_distance(const Genome& rhs)
    {
        return 0;
    }

    void mutate_weight(const RandomGenerator& generator)
    {

    }

    void mutate_enable_disable(const RandomGenerator& generator)
    {

    }

    void mutate_node(const RandomGenerator& generator)
    {

    }

    void mutate_connection(const RandomGenerator& generator)
    {

    }

    void mutate(const RandomGenerator& generator)
    {
        // TODO: Probabilities of node, point and enable/disable mutations
        float p_of_link_mutate = generator.get_next(1);
        if(p_of_link_mutate <= Parameters::link_mutation_chance)
        {
            mutate_connection(generator);
        }
    }
};

std::unique_ptr<Genome, std::function<void(Genome*)>> Genome::create(const RandomGenerator& generator)
{
    auto del = [] (Genome* gen)
    {
        delete gen;
    };

    auto genome = std::unique_ptr<Genome, std::function<void(Genome*)>>(new Genome, del);
    return genome;
}

Genome::Genome()
    :impl_(new Impl)
{}

Genome::~Genome() = default;

void Genome::mutate(const RandomGenerator& generator)
{
    impl_->mutate(generator);
}

float Genome::compatibility_distance(const Genome& rhs)
{
    return impl_->compatibility_distance(rhs);
}
