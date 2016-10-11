#include <gtest/gtest.h>

#include <RandomGenerator.hpp>

TEST(RandomGeneratorTests, ConstructWithRandMax0)
{
    RandomGenerator gen;
    ASSERT_THROW(gen.get_next(0), std::invalid_argument*);
}
