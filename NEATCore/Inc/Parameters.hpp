#pragma once

#include <cstddef>

namespace Parameters
{
   const size_t population_size = 20;
   const size_t genome_size = 30;
   const float disjoint_coeff = 2.0;
   const float excess_coeff = 2.0;
   const float weights_coeff = 0.4;
   const float distance_threshold = 1.0;

   const float link_mutation_chance = 0.97;
   const float node_mutation_chance = 0.97;
}
