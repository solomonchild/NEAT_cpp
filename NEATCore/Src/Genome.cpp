#include "Genome.hpp"

#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>
#include <map>
#include <cassert>

#include "Parameters.hpp"
#include "Gene.hpp"
#include "Logger.hpp"
#include "Evaluator.hpp"
#include "Environment.hpp"




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
Genome::Genome(std::shared_ptr<RandomGenerator> generator, const Genes& genes)
    :generator_(generator)
    , last_neuron_(Evaluator::number_of_inputs_ - 1)
    , genes_(genes)
    , fitness_(0)
{
    if(genes.size() == 0)
    {
        for (Genes::size_type output_index = Parameters::genome_size  - Evaluator::number_of_outputs_; output_index < Parameters::genome_size; ++output_index)
        {
            for (Genes::size_type input_index = 0; input_index < Evaluator::number_of_inputs_; ++input_index)
            {
                Gene gene(generator_);
                gene.in(input_index);
                gene.out(output_index);
                gene.weight(generator_->get_next());
                genes_.push_back(gene);
            }
        }
    }
}


float Genome::compatibility_distance(const Genome& rhs) const
{
    float num_of_disjoint = 0;
    float num_of_excess = 0;
    float avg_weight_diff = 0;
    unsigned largest = 0;
    std::map<unsigned, bool> innovations1;
    std::map<unsigned, bool> innovations2;
    std::map<int, float> weights1;
    std::map<int, float> weights2;
    for(auto& g : genes_)
    {
        innovations1[g.innovation()] = true;
        weights1[g.innovation()] = g.weight();
    }
    for(auto& g : rhs.genes_)
    {
        innovations2[g.innovation()] = true;
        weights2[g.innovation()] = g.weight();
        if(g.innovation() > largest && innovations1[g.innovation()])
        {
            largest = g.innovation();
        }
    }
    for(unsigned i = 0; i < genes_.size(); ++i)
    {
        auto innovation = genes_[i].innovation();
        if(innovation > largest)
        {
            ++num_of_excess;
        }
        else if(innovations2.find(innovation) == innovations2.end() || innovations2[innovation] == false)
        {
            ++num_of_disjoint;
        }
        else
        {
            avg_weight_diff += std::abs(weights1[innovation] - weights2[innovation]);
        }
    }

    for(unsigned i = 0; i < rhs.genes_.size(); ++i)
    {
        auto innovation = rhs.genes_[i].innovation();
        if(innovation > largest)
        {
            ++num_of_excess;
        }
        else if(innovations1.find(innovation) == innovations1.end() || innovations1[innovation] == false)
        {
            ++num_of_disjoint;
        }
    }
    auto max_genes = std::max(genes_.size(), rhs.genes_.size());
    avg_weight_diff /= max_genes;
    num_of_disjoint /= max_genes;
    num_of_excess /= max_genes;
    auto result = Parameters::disjoint_coeff * num_of_disjoint + Parameters::excess_coeff * num_of_excess
            + Parameters::weights_coeff * avg_weight_diff;
    return result;
}









void Genome::mutate()
{
    auto get_random_gene = [this]() -> Gene&
    {
        int index = generator_->get_next(genes_.size() - 1);
        return genes_[index];
    };
    auto mutate_enable_disable = [=]()
    {
        auto& gene = get_random_gene();
        gene.is_enabled(!gene.is_enabled());
    };
    auto mutate_node = [=]()
    {
        if(genes_.size() >= Parameters::genome_size)
        {
            //AERROR("Cannot mutate node. Genome full.");
            return;
        }
        auto& gene = get_random_gene();
        Gene gene1(gene);
        Gene gene2(gene);
        gene.is_enabled(false);

        gene1.in(gene.in());
        gene1.out(last_neuron_);
        gene1.weight(1);
        Environment::inc_innovation_number();
        gene1.innovation(Environment::get_innovation_number());

        gene2.in(last_neuron_);
        gene2.out(gene.out());
        gene2.weight(gene.weight());
        Environment::inc_innovation_number();
        gene2.innovation(Environment::get_innovation_number());

        genes_.push_back(gene1);
        genes_.push_back(gene2);
        last_neuron_++;
    };
    auto contains_gene = [=](const Gene& gene)
    {
        for (auto g : genes_)
        {
            if(g.in() == gene.in() && g.out() == gene.out())
            {
                return true;
            }
        }
        return false;
    };
    auto get_neurons = [this]()
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
    };
    auto get_random_neuron = [=]()
    {
        auto neurons = get_neurons();
        int index = generator_->get_next(neurons.size() - 1);
        return neurons[index];
    };
    auto mutate_connection = [=]()
    {
        auto neuron1 = get_random_neuron();
        auto neuron2 = get_random_neuron();
        Gene gene(generator_);
        gene.in(neuron1.index_);
        gene.out(neuron2.index_);
        gene.weight(generator_->get_next());

        if(!contains_gene(gene))
        {
            genes_.push_back(gene);
            last_neuron_++;
        }
    };
    auto mutate_weight = [=]()
    {
        auto& gene = get_random_gene();
        auto coef = generator_->get_next(2);
        gene.weight(gene.weight() * coef);
    };
    DEBUG("Mutating");
    // TODO: Probabilities of point and enable/disable mutations
    float p_of_node_mutate = generator_->get_next();
    if (p_of_node_mutate <= Parameters::node_mutation_chance)
    {
        DEBUG("Will add node gene");
        mutate_node();
    }

    float p_of_link_mutate = generator_->get_next();
    if(p_of_link_mutate <= Parameters::link_mutation_chance)
    {
        DEBUG("Will mutate connection");
        mutate_connection();
    }
    float p_of_enable_disable_mutate = generator_->get_next();
    if(p_of_enable_disable_mutate <= Parameters::enable_disable_mutation_chance)
    {
        DEBUG("Will mutate enable/disable");
        mutate_enable_disable();
    }
    float p_of_weight_mutate = generator_->get_next();
    if(p_of_weight_mutate <= Parameters::weight_mutation_chance)
    {
        DEBUG("Will mutate weight");
        mutate_weight();
    }
}

Genome Genome::crossover(const Genome& other)
{
    Genome::Genes genes;
    std::map<int, Gene> innovations;

    for (const auto& g : other.genes_)
    {
        innovations[g.innovation()]  = g;
    }

    for (const auto& g : genes_)
    {
        unsigned innovation = g.innovation();
        auto it = innovations.find(innovation);
        if(it == innovations.end())
        {
            genes.push_back(g);
        }
        else
        {
            auto chance = generator_->get_next();
            // TODO: replace with a constant
            if(chance > 0.5 && it->second.is_enabled())
            {
                genes.push_back(it->second);
            }
            else
            {
                genes.push_back(g);
            }
        }
    }
    auto gen = Genome(generator_, genes);
    // TODO: cover this
    gen.last_neuron_ = std::max(last_neuron_, other.last_neuron_);
    return Genome(generator_, genes);
}

Outputs Genome::evaluate_network(const Inputs& inputs) const
{
    Inputs biased_inputs(inputs);
    //pushback bias
    // TODO: fix bias
    //biased_inputs.push_back(1);
    auto sigmoid = [] (float x)
    {
        float res = 2.0/(1+std::exp(-4.9*x))-1;
        return res;
    };

    // TODO: add sigmoid
    std::vector<Neuron> network;
    Outputs outputs;

    for(unsigned i = 0; i < Evaluator::number_of_inputs_; ++i)
    {
        Neuron n(i);
        n.value_ = biased_inputs[i];
        network.emplace_back(n);
    }

    for(unsigned i = 0; i < Evaluator::number_of_outputs_; ++i)
    {
        auto index = Parameters::genome_size - Evaluator::number_of_outputs_ + i;
        network.emplace_back(Neuron(index));
    }

    Genes genes(genes_);

    auto comp = [] (const Gene& g1, const Gene& g2)
    {
        return g1.out() < g2.out();
    };

    std::sort(genes.begin(), genes.end(), comp);
    for (auto& g : genes)
    {
        if(std::find(network.begin(), network.end(), g.out()) == network.end())
        {
            DEBUG("Emplacing %d(out)", g.out());
            network.emplace_back(Neuron (g.out()));
        }

        if(std::find(network.begin(), network.end(), g.in()) == network.end())
        {
            DEBUG("Emplacing %d(in)", g.in());
            network.emplace_back(Neuron (g.in()));
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
    auto is_input = [] (const Neuron& n)
    {
        return n.index_ < Evaluator::number_of_inputs_;
    };

    auto is_output = [this] (const Neuron& n)
    {
        // TODO: review/cover
        DEBUG("Is output: %d", n.index_);
        return n.index_ >= Parameters::genome_size - Evaluator::number_of_outputs_;
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

                DEBUG("Will find neuron with index %d", incoming.in());
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

bool Genome::operator ==(const Genome& other) const
{
    return genes_ == other.genes_
        && last_neuron_ == other.last_neuron_;
}




Genome::Genome(const Genome& other)
{
    this->generator_ = other.generator_;
    this->last_neuron_ = other.last_neuron_;
    this->genes_ = other.genes_;
    this->fitness_ = other.fitness_;
}

Genome& Genome::operator=(const Genome& other)
{
    this->generator_ = other.generator_;
    this->last_neuron_ = other.last_neuron_;
    this->genes_ = other.genes_;
    this->fitness_ = other.fitness_;
    return *this;
}

Genome::Genome(Genome&& other)
{
    this->generator_ = std::move(other.generator_);
    this->last_neuron_ = other.last_neuron_;
    this->genes_ = std::move(other.genes_);
    this->fitness_ = other.fitness_;
}

Genome& Genome::operator=(Genome&& other)
{
    this->generator_ = std::move(other.generator_);
    this->last_neuron_ = other.last_neuron_;
    this->genes_ = std::move(other.genes_);
    this->fitness_ = other.fitness_;
    return *this;
}



void Genome::set_fitness(float fitness)
{
    fitness_ = fitness;
}

float Genome::get_fitness() const
{
    return fitness_;
}

std::ostream& operator<<(std::ostream& stream, const Genome& genome)
{
    stream << "Genome {\n";
    stream << "  Last neuron: " << genome.last_neuron_ << '\n';
    for (auto g : genome.genes_)
    {
        stream << "  " << g;
    }
    stream << "}\n";
    return stream;
}
