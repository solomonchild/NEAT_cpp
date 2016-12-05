#pragma once

#include "Evaluator.hpp"

class Environment
{
public:

   static void set_inputs(const Inputs& inputs);
   static Inputs get_inputs();
   static void inc_innovation_number();
   static unsigned get_innovation_number();

  private:
   static Inputs inputs_;
   static unsigned innovation_number_;
};
