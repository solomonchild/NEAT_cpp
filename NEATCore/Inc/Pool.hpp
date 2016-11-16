#pragma once

#include <vector>
#include <memory>

#include "Species.hpp"
#include "RandomGenerator.hpp"

class Pool
{
public:
    using Species = std::vector<Species>;
    Pool(std::shared_ptr<RandomGenerator>&);
    ~Pool();
private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
