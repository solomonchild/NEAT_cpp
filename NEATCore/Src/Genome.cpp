#include "Genome.hpp"

#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>
#include <cassert>

#include "Parameters.hpp"
#include "Gene.hpp"


using Genes = std::vector<Gene>;

struct Neuron
{
    Neuron(int idx)
    :index_(idx)
        , value_(0.0f)
    {
    }

    operator int()
    {
        return index_;
    }

    unsigned int index_;
    Genes input_;
    float value_;
};

using Neurons = std::vector<Neuron>;
struct Genome::Impl
{
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

    Neurons get_neurons()
    {
        Neurons neurons;
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

    Neuron get_random_neuron()
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
        auto neuron1 = get_random_neuron();
        auto neuron2 = get_random_neuron();
        Gene gene(generator_);
        gene.in(neuron1.index_);
        gene.out(neuron2.index_);
        gene.weight(generator_.get_next(2));

        if(!contains_gene(gene))
        {
            genes_.push_back(gene);
        }
    }

    void mutate()
    {
        // TODO: Probabilities of point and enable/disable mutations
        float p_of_node_mutate = generator_.get_next(1);
        if (p_of_node_mutate <= Parameters::node_mutation_chance)
        {
            mutate_node();
        }

        float p_of_link_mutate = generator_.get_next(1);
        if(p_of_link_mutate <= Parameters::link_mutation_chance)
        {
            mutate_connection();
        }
    }


    Outputs evaluate_network(const Inputs& inputs)
    {
        std::vector<Neuron> network;
        Outputs outputs;
        Evaluator ev;

        for(unsigned i = 0; i < ev.number_of_inputs_; ++i)
        {
            Neuron n(i);
            n.value_ = inputs[i];
            network.emplace_back(n);
        }

        for(unsigned i = 0; i < ev.number_of_outputs_; ++i)
        {
            network.emplace_back(Neuron(Parameters::genome_size + i));
        }

        Genes genes(genes_);

        auto comp = [] (const Gene& g1, const Gene& g2)
        {
            return g1.out() < g2.out();
        };

        std::sort(genes.begin(), genes.end(), comp);
        for (auto& g : genes)
        {
            auto out_predicate = [&g] (const Gene& gene)
            {
                    return g.out() == gene.out();
            };

            if (std::find_if(genes.begin(), genes.end(), out_predicate) == genes.end())
            {
                network.emplace_back(Neuron (g.out()));
            }

            {
                auto it = std::find(network.begin(), network.end(), g.out());
                assert(it != network.end());
                Neuron& found_neuron = *it;
                found_neuron.input_.push_back(g);
            }

            auto in_predicate = [&g] (const Gene& gene)
            {
                    return g.in() == gene.in();
            };
            if (std::find_if(genes.begin(), genes.end(), in_predicate) == genes.end())
            {
                Neuron n(g.in());
                network.emplace_back(n);
            }
        }

        for(auto& n : network)
        {
            // TODO: calculate
            (void)n;
        }
        return outputs;
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


Outputs Genome::evaluate_network(const Inputs& inputs)
{
    return impl_->evaluate_network(inputs);
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
