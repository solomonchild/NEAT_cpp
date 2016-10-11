#include "Parameters.hpp"




void create_pool()
{
    // use Parameters::population_size;
    for (unsigned i = Parameters::population_size; i > 0; -- i)
    {
    }


}

int main(int argc, char** argv)
{
    create_pool();
    // for each genome apply problem
    // assign each genome to species
    // for each species, cull genomes
    // breed each species(randomly select genomes to crossover and/or mutate)
    return 0;
}
