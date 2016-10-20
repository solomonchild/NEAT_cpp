#include "Genome.hpp"

#include <iostream>
#include <memory>
#include <vector>

#include "Parameters.hpp"
#include "Gene.hpp"


struct Genome::Impl
{
    using Genes = std::vector<Gene>;
    Genes genes_;

    Impl(const RandomGenerator& generator)
        :generator_(generator)
    {
        for (Genes::size_type output_index = Parameters::genome_size  - Parameters::outputs; output_index < Parameters::genome_size; ++output_index)
        {
            for (Genes::size_type input_index = 0; input_index < Parameters::inputs; ++input_index)
            {
                Gene gene(generator_);
                gene.in(input_index);
                gene.out(output_index);
                genes_.push_back(gene);
            }
        }
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

std::ostream& operator<<(std::ostream& stream, const Genome& genome)
{
    for (auto i : genome.impl_->genes_)
    {
        stream << i;
    }
    stream << "\n";
    return stream;
}
