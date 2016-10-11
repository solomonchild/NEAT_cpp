#include "Genome.hpp"

#include <memory>


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
//        float p_of_node_mutate = generator.get_next(1);
        //float p_of_link_mutate = generator.get_next(1);
//        float p_of_point_mutate = generator.get_next(1);
//        float p_of_enable_disable_mutate = generator.get_next(1);
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
