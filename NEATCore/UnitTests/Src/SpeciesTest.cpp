#include <gtest/gtest.h>

#include "Environment.hpp"
#include "Species.hpp"
#include "RandomGenerator.hpp"


TEST(Species, GenomeMustFitTest)
{
    auto generator = std::make_shared<RandomGenerator>();
    Environment::set_inputs({0, 1});
    Species species(generator);
    Genome g{generator, {
        {generator, 0, 1}
       ,{generator, 0, 2}
       ,{generator, 0, 3}
    }};
    species.add_genome(g);
    bool result = species.will_genome_fit(g);
    ASSERT_TRUE(result);
}

TEST(Species, GenomeMustNotFitTest)
{
    auto generator = std::make_shared<RandomGenerator>();

    auto genome1 = Genome {generator,
      {
        {generator, 0, 1}
        ,{generator, 0, 3}
      }
    };

    auto genome2 = Genome {generator,
      {
         {generator, 0, 1}
        ,{generator, 0, 2}
        ,{generator, 0, 3}
        ,{generator, 0, 4}
        ,{generator, 0, 5}
      }
    };

    Environment::set_inputs({0, 1});
    Species species(generator);

    species.add_genome(genome1);
    bool result = species.will_genome_fit(genome2);
    ASSERT_FALSE(result);
}
