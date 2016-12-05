#pragma once

#include "Evaluator.hpp"

class Environment
{
public:

   static void inc_innovation_number();
   static unsigned get_innovation_number();

  private:
   static unsigned innovation_number_;
};
