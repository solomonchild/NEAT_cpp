#pragma once

#include <memory>

#include "RandomGenerator.hpp"

class Genome
{
public:
    static std::unique_ptr<Genome, std::function<void(Genome*)>> create(const RandomGenerator&);

    float compatibility_distance(const Genome&);
    void mutate();

    Genome(const RandomGenerator&);

    Genome(const Genome&);
    Genome& operator=(const Genome&);

    Genome(Genome&&);
    Genome& operator=(Genome&&);
    ~Genome();

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
