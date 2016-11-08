#include "Genome.hpp"

#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>
#include <cassert>

#include "Parameters.hpp"
#include "Gene.hpp"

#define DEBUG(x, ...) printf(x"\n", ##__VA_ARGS__)



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
    Genome::Genes input_;
    float value_;
};

using Neurons = std::vector<Neuron>;
struct Genome::Impl
{
    Impl(std::shared_ptr<RandomGenerator> generator)
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
    Impl(std::shared_ptr<RandomGenerator> generator, const Genes& genes)
        :generator_(generator)
        , last_neuron_(Parameters::inputs)
        , genes_(genes)
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
       last_neuron_++;
       auto& gene = get_random_gene();
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
        int index = generator_->get_next(genes_.size());
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
        int index = generator_->get_next(neurons.size());
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
        gene.weight(generator_->get_next(2));

        if(!contains_gene(gene))
        {
            genes_.push_back(gene);
        }
    }

    void mutate()
    {
        // TODO: Probabilities of point and enable/disable mutations
        float p_of_node_mutate = generator_->get_next(1);
        if (p_of_node_mutate <= Parameters::node_mutation_chance)
        {
            mutate_node();
        }

        float p_of_link_mutate = generator_->get_next(1);
        if(p_of_link_mutate <= Parameters::link_mutation_chance)
        {
            mutate_connection();
        }
    }


    Outputs evaluate_network(const Inputs& inputs)
    {
        Inputs biased_inputs(inputs);
        //pushback bias
        // TODO: fix bias
        biased_inputs.push_back(1);
        auto sigmoid = [] (float x)
        {
            auto res = 2/(1+std::exp(-4.9*x))-1;
            return res;
        };

        // TODO: add sigmoid
        std::vector<Neuron> network;
        Outputs outputs;

        for(unsigned i = 0; i < Parameters::inputs; ++i)
        {
            Neuron n(i);
            n.value_ = biased_inputs[i];
            DEBUG("Placing neuron %d with input %f", n.index_, n.value_);
            network.emplace_back(n);
        }

        for(unsigned i = 0; i < Parameters::outputs; ++i)
        {
            DEBUG("Adding neuron with number %d", Parameters::genome_size + i);
            network.emplace_back(Neuron(Parameters::genome_size + i));
        }

        Genes genes(genes_);

        auto comp = [] (const Gene& g1, const Gene& g2)
        {
            return g1.out() < g2.out();
        };

        std::sort(genes.begin(), genes.end(), comp);
        DEBUG("Will enumerate genes");
        for (auto& g : genes)
        {
            auto out_predicate = [&g] (const Gene& gene)
            {
                    return g.out() == gene.out();
            };

            if (std::find_if(genes.begin(), genes.end(), out_predicate) == genes.end())
            {
                DEBUG("Adding neuron with number %d", g.out());
                network.emplace_back(Neuron (g.out()));
            }

            {
                auto it = std::find(network.begin(), network.end(), g.out());
                DEBUG("Found neuron with index %d", g.out());
                assert(it != network.end());
                Neuron& found_neuron = *it;
                DEBUG("Pushing gene into found neuron");
                std::cout << g << std::endl;
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
        auto is_input = [] (const Neuron& n)
        {
            return n.index_ < Parameters::inputs;
        };

        auto is_output = [this] (const Neuron& n)
        {
            return n.index_ >= last_neuron_ + Parameters::outputs;
        };

        for(auto& n : network)
        {

            float sum = 0;
            if(!is_input(n))
            {
                for (auto& incoming : n.input_)
                {
                    auto find_in_neuron = [&incoming] (const Neuron& neuron)
                    {
                        return incoming.in() == neuron.index_;
                    };
                    auto it = std::find_if(network.begin(), network.end(), find_in_neuron);
                    assert(it != network.end());
                    const Neuron& found_neuron = *it;
                    sum += found_neuron.value_ * incoming.weight();
                }
                n.value_ = sigmoid(sum);
            }
            if (is_output(n))
            {
                outputs.push_back(n.value_);
            }
        }
        return outputs;
    }

    std::shared_ptr<RandomGenerator> generator_;
    unsigned int last_neuron_;
    Genes genes_;
};

Genome::Genome(std::shared_ptr<RandomGenerator> generator, const Genes& genes)
    :impl_(new Impl(generator, genes))
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
