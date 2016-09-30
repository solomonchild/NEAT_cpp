#include "Genome.hpp"
#include "Parameters.hpp"


struct Genome::Impl
{

    float compatibility_distance(const Genome& rhs)
    {
        return 0;
    }
};


Genome::Genome()
    :impl_(new Impl)
{}

float Genome::compatibility_distance(const Genome& rhs)
{
    return impl_->compatibility_distance(rhs);
}
