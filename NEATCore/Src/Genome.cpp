#include "Genome.hpp"

#include <iostream>
#include <algorithm>
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
        , last_neuron_(Parameters::inputs)
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
       last_neuron_++;
       auto gene = get_random_gene();
       Gene gene1(gene);
       Gene gene2(gene);
       gene.is_enabled(false);

       gene1.in(gene.in());
       gene1.out(last_neuron_);
       gene1.weight(1);

       gene2.in(last_neuron_);
       gene2.out(gene.out());
       gene2.weight(gene.weight());

       genes_.push_back(gene1);
       genes_.push_back(gene2);

    }

    Gene& get_random_gene()
    {
        int index = generator_.get_next(genes_.size());
        return genes_[index];
    }

    std::vector<unsigned> get_neurons()
    {
        // use set?
        std::vector<unsigned> neurons;
        for (auto g : genes_)
        {
            if(std::find(neurons.begin(), neurons.end(), g.in()) == neurons.end())
            {
                neurons.push_back(g.in());
            }
            if(std::find(neurons.begin(), neurons.end(), g.out()) == neurons.end())
            {
                neurons.push_back(g.out());
            }
        }
        return neurons;
    }

    unsigned int get_random_neuron_number()
    {
        auto neurons = get_neurons();
        int index = generator_.get_next(neurons.size());
        return neurons[index];
    }


    bool contains_gene(const Gene& gene)
    {
        for (auto g : genes_)
        {
            if(g.in() == gene.in() && g.out() == gene.out())
            {
                return true;
            }
        }
        return false;
    }

    void mutate_connection()
    {
        auto neuron1 = get_random_neuron_number();
        auto neuron2 = get_random_neuron_number();
        Gene gene(generator_);
        gene.in(neuron1);
        gene.out(neuron2);
        gene.weight(generator_.get_next(2));

        if(!contains_gene(gene))
        {
            genes_.push_back(gene);
        }
        else
        {
            std::cout << "CONTAINS!" << std::endl;
            std::cout << gene << std::endl;
        }
    }

    void mutate()
    {
        float p_of_node_mutate = generator_.get_next(1);
        if (p_of_node_mutate <= Parameters::node_mutation_chance)
        {
            mutate_node();
        }

        // TODO: Probabilities of node, point and enable/disable mutations
        float p_of_link_mutate = generator_.get_next(1);
        if(p_of_link_mutate <= Parameters::link_mutation_chance)
        {
            mutate_connection();
        }
    }

    RandomGenerator generator_;
    unsigned int last_neuron_;
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
    for (auto g : genome.impl_->genes_)
    {
        if(g.is_enabled())
        {
            stream << g;
        }
    }
    stream << "\n";
    return stream;
}
