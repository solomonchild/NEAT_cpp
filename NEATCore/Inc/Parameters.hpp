#pragma once

#include <cstddef>

namespace Parameters
{
   const size_t population_size = 20;
   const float disjoint_coeff = 2.0;
   const float excess_coeff = 2.0;
   const float weights_coeff = 0.4;
   const float distance_threshold = 1.0;

   const float link_mutation_chance = 0.57;

   //TODO: carry out these to Evaluator
   const unsigned inputs = 2;
   const unsigned outputs = 1;
}
