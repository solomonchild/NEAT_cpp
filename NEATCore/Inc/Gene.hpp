#pragma once

#include <memory>
#include <functional>

#include "RandomGenerator.hpp"

class Gene
{
public:
    bool is_enabled();
    float weight();
    unsigned innovation();
    static std::unique_ptr<Gene, std::function<void(Gene*)>> create(const RandomGenerator&);

private:
    Gene();
    ~Gene();
    struct Impl;
    std::unique_ptr<Impl> impl_;

};
