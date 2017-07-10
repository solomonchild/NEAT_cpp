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

bool contains_gene (Genome::Genes& genes, const Gene& gene)
{
    for (auto g : genes)
    {
        if(g.in() == gene.in() && g.out() == gene.out())
        {
            return true;
        }
    }
    return false;
}

using Neurons = std::vector<Neuron>;
Genome::Genome(std::shared_ptr<RandomGenerator> generator, const Genes& genes)
    :generator_(generator)
    , last_neuron_(Evaluator::number_of_inputs_ - 1)
    , genes_(genes)
    , id_(ID++)
{
    if(genes.size() == 0)
    {
        for (Genes::size_type output_index = Parameters::get_instance().genome_size()  - Evaluator::number_of_outputs_;
             output_index < Parameters::get_instance().genome_size(); ++output_index)
        {
            for (Genes::size_type input_index = 0; input_index < Evaluator::number_of_inputs_; ++input_index)
            {
                Gene gene(generator_);
                gene.in(input_index);
                gene.out(output_index);
                gene.weight(generator_->get_next());
                assert(!contains_gene(genes_, gene));
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
    auto result = Parameters::get_instance().disjoint_coeff() * num_of_disjoint + Parameters::get_instance().excess_coeff() * num_of_excess
            + Parameters::get_instance().weights_coeff() * avg_weight_diff;
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
        if(genes_.size() >= Parameters::get_instance().genome_size())
        {
            //AERROR("Cannot mutate node. Genome full.");
            return;
        }
        auto& gene = get_random_gene();
        Gene gene1(gene);
        Gene gene2(gene);
        last_neuron_++;
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

        if(!contains_gene(genes_, gene1) && !contains_gene(genes_, gene2))
        {
            genes_.push_back(gene1);
            genes_.push_back(gene2);
        }
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
        std::sort(neurons.begin(), neurons.end(), [](Neuron& n1, Neuron& n2){return n1.index_ < n2.index_;});
        return neurons;
    };
    auto get_random_neuron = [=](bool notOutput = false)
    {
        auto neurons = get_neurons();
        int index = 0;
        if(notOutput)
        {
            index = generator_->get_next(neurons.size() - 1 - Evaluator::number_of_outputs_);
        }
        else
        {
            index = generator_->get_next(neurons.size() - 1);
        }
        return neurons[index];
    };
    auto mutate_connection = [=]()
    {
        Neuron neuron1 = get_random_neuron(true);
        Neuron neuron2 = get_random_neuron();
        Gene gene(generator_);

        if(neuron1.index_ <= Evaluator::number_of_inputs_ && neuron2.index_ <= Evaluator::number_of_inputs_)
        {
            return;
        }
        if(neuron1.index_ == neuron2.index_)
        {
            return;
        }
        if(neuron2.index_ <= Evaluator::number_of_inputs_ )
        {
            std::swap(neuron1, neuron2);
        }
        if(neuron1.index_ > neuron2.index_)
        {
            std::swap(neuron1, neuron2);
        }
        assert(neuron1.index_ < neuron2.index_);
        gene.in(neuron1.index_);
        gene.out(neuron2.index_);
        if(!contains_gene(genes_, gene))
        {
            Environment::inc_innovation_number();
            gene.innovation(Environment::get_innovation_number());
            gene.weight(generator_->get_next(2));
            genes_.push_back(gene);
            last_neuron_++;
        }
    };
    auto mutate_weight = [=]()
    {
        auto& gene = get_random_gene();
        auto coef = generator_->get_next(1);
        gene.weight(gene.weight() * coef);
    };
    DEBUG("Mutating");
    // TODO: Probabilities of point and enable/disable mutations
    float p_of_node_mutate = generator_->get_next();
    if (p_of_node_mutate <= Parameters::get_instance().node_mutation_chance())
    {
        DEBUG("Will add node gene");
        mutate_node();
    }

    float p_of_link_mutate = generator_->get_next();
    if(p_of_link_mutate <= Parameters::get_instance().link_mutation_chance())
    {
        DEBUG("Will mutate connection");
        mutate_connection();
    }
    float p_of_enable_disable_mutate = generator_->get_next();
    if(p_of_enable_disable_mutate <= Parameters::get_instance().enable_disable_mutation_chance())
    {
        DEBUG("Will mutate enable/disable");
        mutate_enable_disable();
    }
    float p_of_weight_mutate = generator_->get_next();
    if(p_of_weight_mutate <= Parameters::get_instance().weight_mutation_chance())
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
            if(!contains_gene(genes, it->second))
            {
                genes.push_back(g);
            }
        }
        else
        {
            auto chance = generator_->get_next();
            // TODO: replace with a constant
            if(chance > 0.5 && it->second.is_enabled())
            {
                if(!contains_gene(genes, it->second))
                {
                    genes.push_back(it->second);
                }
            }
            else
            {
                if(!contains_gene(genes, g))
                {
                    genes.push_back(g);
                }
            }
        }
    }
    auto gen = Genome(generator_, genes);
    gen.last_neuron_ = std::max(last_neuron_, other.last_neuron_);
    return gen;
}

Outputs Genome::evaluate_network(const Inputs& inputs) const
{
    Inputs biased_inputs(inputs);
    //pushback bias
    // TODO: fix bias
    biased_inputs.push_back(1);

    std::vector<Neuron> network;
    Outputs outputs;

    for(unsigned i = 0; i < biased_inputs.size(); ++i)
    {
        Neuron n(i);
        n.value_ = biased_inputs[i];
        network.emplace_back(n);
    }

    for(unsigned i = 0; i < Evaluator::number_of_outputs_; ++i)
    {
        auto index = Parameters::get_instance().genome_size() - Evaluator::number_of_outputs_ + i;
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
        return n.index_ >= Parameters::get_instance().genome_size() - Evaluator::number_of_outputs_;
    };

    auto sigmoid = [] (float x)
    {
        float res = 2.0/(1+std::exp(-4.9*x))-1;
        return res;
    };

    auto original_sigmoid = [] (float x)
    {
        float res = 1.0/(1+std::exp(-x*4.924273));
        return res;
    };
    (void) sigmoid;
    (void) original_sigmoid;

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
            n.value_ = original_sigmoid(sum);
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
    this->id_ = other.id_;
    this->generator_ = other.generator_;
    this->last_neuron_ = other.last_neuron_;
    this->genes_ = other.genes_;
    this->fitness_ = other.fitness_;
}

Genome& Genome::operator=(const Genome& other)
{
    this->id_ = other.id_;
    this->generator_ = other.generator_;
    this->last_neuron_ = other.last_neuron_;
    this->genes_ = other.genes_;
    this->fitness_ = other.fitness_;
    return *this;
}

Genome::Genome(Genome&& other)
{
    this->id_ = other.id_;
    this->generator_ = std::move(other.generator_);
    this->last_neuron_ = other.last_neuron_;
    this->genes_ = std::move(other.genes_);
    this->fitness_ = other.fitness_;
}

Genome& Genome::operator=(Genome&& other)
{
    this->id_ = other.id_;
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
    stream << "/*Genome {\n";
    stream << "  Last neuron: " << genome.last_neuron_ << '\n';
    for (auto g : genome.genes_)
    {
        stream << "  " << g;
    }
    stream << "}*/\n";

    stream << "\ndigraph {\n" << "label = \"fitness: " + std::to_string(genome.get_fitness()) + "error: " + std::to_string(genome.get_error()) +
              " \"\n";
    stream << "node [shape=record,width=.1,height=.1];\n"
           << "inputs [label = \"<i0>Input1|<i1>Input2\",height=.5];\n";
    for (unsigned int i = 0; i < genome.genes_.size(); ++i)
    {
        const Gene& g = genome.genes_[i];
        if(g.in() < Evaluator::number_of_inputs_)
        {
//            stream << "inputs:i" << g.in() << "->n" << g.out() <<"[label=\""<<g.weight() <<"\"];\n";
        }
//        else
        {
            std::string out = std::string("n") + std::to_string(g.out());
            std::string in = std::to_string(g.in());
            if(g.in() < Evaluator::number_of_inputs_)
            {
                in = "inputs:i" + in;
            }
            else
            {
                in = "n" + in;
            }
            if(g.out() >= (Parameters::get_instance().genome_size() - Evaluator::number_of_outputs_))
            {
                out = "Output";
            }

            if(g.is_enabled())
            {
                stream << in <<"->" << out <<"[label=\""<<g.weight() <<"\"];\n";
            }
            else
            {
                stream << in <<"->" << out <<"[style=\"dotted\"];\n";
            }
        }
    }
    stream << "}\n";
    return stream;
}
unsigned Genome::id()
{
    return id_;
}

Genome::~Genome()
{
    INFO("Genome %d deleted. Fitness: %f. Error: %f", id_, fitness_, error_);

}

void Genome::set_error(float error)
{
    error_ = error;
}

float Genome::get_error() const
{
    return error_;
}
unsigned Genome::ID = 0;
