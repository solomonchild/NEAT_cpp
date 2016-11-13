#include "Species.hpp"


struct Species::Impl
{

    //TODO: use Environment
    Impl()
    {

    }

    Genomes genomes_;
};

Species::Species()
    : impl_(new Impl)
{
}
