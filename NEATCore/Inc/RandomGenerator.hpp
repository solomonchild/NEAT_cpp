#pragma once

#include <memory>

class RandomGenerator
{
public:
    RandomGenerator();
    RandomGenerator(const RandomGenerator&);
    virtual ~RandomGenerator();

    virtual float get_next(uint16_t rand_max = 1) const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
