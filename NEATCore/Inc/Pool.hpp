#pragma once

#include <vector>
#include <memory>
#include "Species.hpp"

class Pool
{
public:
    using Species = std::vector<Species>;
private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
