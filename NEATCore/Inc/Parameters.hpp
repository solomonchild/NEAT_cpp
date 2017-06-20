#pragma once

#include <cstddef>
#include "Evaluator.hpp"

namespace Parameters
{

   const size_t population_size = 100;
   const size_t genome_size = 4;
   const float disjoint_coeff = 2.0;
   const float excess_coeff = 2.0;
   const float weights_coeff = 0.4;
   const float distance_threshold = 1.0;

   const float link_mutation_chance = 0.80;
   const float node_mutation_chance = 0.80;
   const float weight_mutation_chance = 0.80;
   const float enable_disable_mutation_chance = 0.10;

   const float gene_weight_multiplier = 0.03;
}
