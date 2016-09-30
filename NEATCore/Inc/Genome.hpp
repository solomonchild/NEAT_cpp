#pragma once

#include <memory>

class Genome
{
public:
    Genome();

    float compatibility_distance(const Genome&);


private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
