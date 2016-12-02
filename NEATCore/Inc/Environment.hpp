#pragma once

#include "Evaluator.hpp"

class Environment
{
public:

   static void set_inputs(const Inputs& inputs);
   static Inputs get_inputs();
  private:
   static Inputs inputs_;
};
