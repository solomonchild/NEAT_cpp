#include <gtest/gtest.h>

#include "Genome.hpp"



TEST(GenomeTest, CompatibilityTest)
{
    RandomGenerator generator;
    auto genome1 = Genome::create(generator);
    auto genome2 = Genome::create(generator);
    float distance = genome1->compatibility_distance(*genome2);
    ASSERT_LE(0, distance);
}

