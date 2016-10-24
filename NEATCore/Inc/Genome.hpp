#pragma once

#include <memory>

#include "RandomGenerator.hpp"
#include "Evaluator.hpp"

class Genome
{
public:
    static std::unique_ptr<Genome, std::function<void(Genome*)>> create(const RandomGenerator&);

    float compatibility_distance(const Genome&);
    void mutate();

    Outputs evaluate_network(const Inputs&);

    Genome(const RandomGenerator&);

    Genome(const Genome&);
    Genome& operator=(const Genome&);

    Genome(Genome&&);
    Genome& operator=(Genome&&);
    ~Genome();

private:
    friend std::ostream& operator<<(std::ostream& stream, const Genome& genome);
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
